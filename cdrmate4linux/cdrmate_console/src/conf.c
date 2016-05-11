#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "inc/main.h"
#include "inc/conf.h"
#include "inc/minIni.h"
#include "inc/manual_send.h"

static char conf_path[1024];

struct conf_rec global_conf;


/// 기본 설정 파일 경로 생성
/// arg[0] 의 경로를 활용하여 생성
void make_conf_path(char *path)
{
	char tmp[1024];

	strncpy(tmp, path, sizeof(tmp)-1);

	ltrim(tmp);
	rtrim(tmp);
	if ( tmp[0] != '/' ) {
		getcwd(conf_path, sizeof(conf_path)-1);
		strcat(conf_path, "/");
		strcat(conf_path, tmp);
	} else {
		strncpy(conf_path, tmp, sizeof(conf_path)-1);
	}
	DBG("conf_path [%s]\n", conf_path);
}

/// baud rate string 을 timio 의 brate 상수로 변경 
/// \param str
/// \return brate
static int speed_str_to_brate(char *str)
{
	char *support_speed[] = {
		"1200",
		"2400",
		"4800",
		"9600",
		"19200",
		"38400",
		"57600",
		"115200",
		NULL
	};
	int brate_val[] = {
		B1200,
		B2400,
		B4800,
		B9600,
		B19200,
		B38400,
		B57600,
		B115200,
		0,
	};
	int i;

	for ( i = 0 ; support_speed[i] != NULL; i++) {
		if ( strcmp(str, support_speed[i]) == 0 ) {
			DBG("i = %d, %d\n", i, brate_val[i]);
			return brate_val[i];
		}
	}
	// default
	return B9600;
}

#define DEFAULT_STR "ERROR"

/// 설정 파일 로드
/// \return 0 success
int load_conf(void)
{
	char buf[1024] = {0, };

	printf("Load configuration data.\n");
	syslog(LOG_ERR, "Load configuration data.");

	memset(&global_conf, 0, sizeof(global_conf));

	ini_gets("Advanced", "syslog", "", buf, sizeof(buf)-1, conf_path);
	if ( strcmp(buf, "yes") == 0 ) {
		global_conf.flag |= CONF_FLAG_SYSLOG;
	}


	ini_gets("General", "port", DEFAULT_STR, buf, sizeof(buf)-1, conf_path);
	if ( buf[0] == 0 || strcmp(buf, DEFAULT_STR) == 0 ) {
		if ( !is_slave_mode() ) {
			PRINT("Use default port = /dev/ttyS0\n");
			strcpy(buf, "/dev/ttyS0");
		}
	}	       
	DBG("port = %s\n", buf);
	strncpy(global_conf.port, buf, sizeof(global_conf.port)-1);

	ini_gets("General", "speed", DEFAULT_STR, buf, sizeof(buf)-1, conf_path);
	if ( buf[0] == 0 ||  strcmp(buf, DEFAULT_STR) == 0 ) {
		if ( !is_slave_mode() ) {
			PRINT("Use default speed = 9600\n");
			strcpy(buf, "9600");
		}
	}	       
	global_conf.speed = speed_str_to_brate(buf);
	DBG("speed = %s, %d\n", buf, global_conf.speed);

	ini_gets("General", "protocol_ver", "1.0", buf, sizeof(buf)-1, conf_path);
	DBG("protocol_ver = %s\n", buf);
	strncpy(global_conf.protocol_ver, buf, sizeof(global_conf.protocol_ver)-1);

	global_conf.system_id = (int)ini_getl("General", "system_id", 0, conf_path);
	DBG("system_id = %d\n", global_conf.system_id);

	ini_gets("General", "received", DEFAULT_STR, buf, sizeof(buf)-1, conf_path);
	if ( buf[0] == 0 || strcmp(buf, DEFAULT_STR) == 0 ) {
		getcwd(buf, sizeof(buf)-1);
		strncat(buf, "/received", sizeof(buf)-1);
		PRINT("Use default received = %s\n", buf);
	}
	DBG("received = %s\n", buf);
	strncpy(global_conf.received, buf, sizeof(global_conf.received)-1);


	ini_gets("General", "transfer", DEFAULT_STR, buf, sizeof(buf)-1, conf_path);
	if ( buf[0] == 0 || strcmp(buf, DEFAULT_STR) == 0 ) {
		getcwd(buf, sizeof(buf)-1);
		strncat(buf, "/transfer", sizeof(buf)-1);
		PRINT("Use default transfer = %s\n", buf);
	}
	DBG("transfer = %s\n", buf);
	strncpy(global_conf.transfer, buf, sizeof(global_conf.transfer)-1);

	ini_gets("General", "transferred", DEFAULT_STR, buf, sizeof(buf)-1, conf_path);
	if ( buf[0] == 0 || strcmp(buf, DEFAULT_STR) == 0 ) {
		getcwd(buf, sizeof(buf)-1);
		strncat(buf, "/transferred", sizeof(buf)-1);
		PRINT("Use default transferred = %s\n", buf);
	}
	DBG("transferred = %s\n", buf);
	strncpy(global_conf.transferred, buf, sizeof(global_conf.transferred)-1);

	if ( strcmp(global_conf.transfer, global_conf.transferred) == 0 ) {
		PRINT_ERROR(-ERR_CONF, "transfer and transfered field cannot be same.\n");
		return -ERR_INVAL;
	}
	ini_gets("General", "move_file", "", buf, sizeof(buf)-1, conf_path);
	if ( strcmp(buf, "yes") == 0 ) {
		global_conf.flag |= CONF_FLAG_MOVE_FILE;
	}

	ini_gets("Advanced", "console", "", buf, sizeof(buf)-1, conf_path);
	if ( strcmp(buf, "yes") == 0 ) {
		global_conf.flag |= CONF_FLAG_CONSOLE;
	}

	ini_gets("Advanced", "error_log", "", buf, sizeof(buf)-1, conf_path);
	if ( strcmp(buf, "yes") == 0 ) {
		global_conf.flag |= CONF_FLAG_ERROR_LOG;
	}

	ini_gets("Advanced", "manual_transfer", "", buf, sizeof(buf)-1, conf_path);
	if ( strcmp(buf, "yes") == 0 ) {
		global_conf.flag |= CONF_FLAG_MANUAL_TRANSFER;
	}

	ini_gets("Advanced", "data_list", "/tmp/cdrmate.dat", buf, sizeof(buf)-1, conf_path);
	strncpy(global_conf.manual_path, buf, sizeof(global_conf.transferred)-1);

	// craete default directory
	mkdir(global_conf.received, 0755);
	mkdir(global_conf.transfer, 0755);
	mkdir(global_conf.transferred, 0755);


	// slave mode 에서는 무조건 manual transfer
	if ( is_slave_mode() ) {
		global_conf.flag |= CONF_FLAG_MANUAL_TRANSFER;
	}
	return 0;
}
