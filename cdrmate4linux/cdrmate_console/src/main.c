#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <termio.h>
#include <errno.h>

#include "inc/main.h"
#include "inc/conf.h"
#include "inc/serial.h"
#include "inc/cdr_packet.h"
#include "inc/raw_packet.h"
#include "inc/cdr_session.h"
#include "inc/console.h"
#include "inc/inotify.h"
#include "inc/buf_list.h"
#include "inc/auto_send.h"
#include "inc/manual_send.h"

#define VERSION_STR "0.1"
#define DEF_CONF "cdrmate.ini"


int global_flag = 0;

#define EXIT_MAIN_LOOP 0x01
#define EXIT_PROGRAM 0x02
#define MANUAL_TRANSFER 0x04
#define SLAVE_MODE 0x08

/// slave mode 인가?
int is_slave_mode(void)
{
	return ((global_flag & SLAVE_MODE) ? 1 : 0 );
}

/// /tmp/cdrmate.pid 생성
/// \return 0 success
static int write_pid(void)
{
	int fd;
	char buf[128];

	fd = creat("/tmp/cdrmate.pid", 0666);
	if ( fd < 0 ) {
		PRINT_ERROR(-ERR_PID, "ERROR(%d) : pid file.\n", -ERR_PID);
		return -ERR_PID;
	}
	sprintf(buf, "%d", (int)getpid());
	write(fd, buf, strlen(buf));
	close(fd);
	return 0;
}

/// pid 파일 삭제
static void delete_pid(void)
{
	unlink("/tmp/cdrmate.pid");
}

/// command line help
/// \param argv0
static void usage(char *argv0)
{
	fprintf(stderr, "usage : %s -hv | -c path\n", argv0);
	fprintf(stderr, "           -c path : configuration file path\n");
	fprintf(stderr, "           -h : help\n");
	fprintf(stderr, "           -v : version\n");
}

/// 현재 디렉토리에 설정파일이 있는지 검사한 후 default conf path 설정
/// \return 0 설정 파일 있음
/// \return -ERR_NOT_FOUND 없음 
static int check_cur_dir_conf(void)
{
	char cwd[1024] = {0, };

	getcwd(cwd, sizeof(cwd)-1);

	if ( strlen(cwd) < sizeof(cwd) - 16) {
		strcat(cwd, "/" DEF_CONF );

		FILE *tmp;

		if ( (tmp = fopen(cwd, "r")) ) {
			fclose(tmp);
			make_conf_path(cwd);
			return 0;
		}
	}
	return -ERR_NOT_FOUND;
}

/// cmd line option 처리
/// \param argc
/// \param argv
/// \return 0 ok
/// \return 음수 error
static int proc_cmd_options(int argc, char *argv[])
{
	int opt;
	int ret = 0;

	if ( argc == 1 ) {
		if ( (ret = check_cur_dir_conf()) < 0 ) {
			usage(argv[0]);
			return ret;
		}
	} else {
		while((opt = getopt(argc, argv, "c:hvs")) != -1 ) {
			switch(opt) {
				case 'c':
					make_conf_path(optarg);
					ret = 1;
					break;

				case ':':
					fprintf(stderr, "Option '-%c' needs a path.\n", optopt);
					usage(argv[0]);
					return -ERR_INVAL;

				case '?':
					fprintf(stderr, "No such option '-%c'\n", optopt);
					usage(argv[0]);
					return -ERR_INVAL;

				case 'h':
					fprintf(stderr, "CDR Mate console Linux V%s (This is not the official version)\n", VERSION_STR);
					usage(argv[0]);
					return -ERR_STOP;

				case 'v':
					fprintf(stdout, "CDR Mate console Linux V%s (This is not the official version)\n", VERSION_STR);
					return -ERR_STOP;

				case 's':
					// slave mode
					global_flag |= SLAVE_MODE;
					break;
			}
		}
	}
	if ( !ret ) {
		if ( (ret = check_cur_dir_conf()) < 0 ) {
			usage(argv[0]);
			return ret;
		}
	}
	return 0;
}

/// serial data 수신 thread
static void * serial_receiver(void *args)
{
	struct serial_receiver_param *param;
	int fd_stdin, maxfd;
	fd_set readfd;
	char buf[1024];
	int ret;
	struct timeval tval;

	DBG("Serial receiver thread started.\n");

	param = (struct serial_receiver_param *)args;
	fd_stdin = STDIN_FILENO;
	ret = MAX(fd_stdin, param->fd_serial);
	maxfd = MAX(ret, param->fd_monitor) + 1;

	while( !(global_flag & EXIT_MAIN_LOOP) ) {
		FD_ZERO(&readfd);
		FD_SET(fd_stdin, &readfd);
		if ( param->fd_serial > 0 ) {
			FD_SET(param->fd_serial, &readfd);
		}
		if ( param->fd_monitor > 0 ) {
			FD_SET(param->fd_monitor, &readfd);
		}

		tval.tv_sec = 5;
		tval.tv_usec = 0;

		ret = select(maxfd, &readfd, NULL, NULL, &tval);
		if ( ret < 0 ) {
			ERROR_LOG("select error. %d\n", errno);
		} else if ( ret == 0 ) {
			// timeout
		} else {

			if ( FD_ISSET(fd_stdin, &readfd)) {
				if ( (ret = read(fd_stdin, buf, sizeof(buf)-1)) > 0 ){
					buf[ret] = 0;
					if ( handle_console(buf, param->fd_serial) == -ERR_STOP) {;
						global_flag |= EXIT_MAIN_LOOP | EXIT_PROGRAM;
					}
				}
			}

			if ( param->fd_serial > 0 && FD_ISSET(param->fd_serial, &readfd)) {
				ret = handle_serial_data(param->fd_serial, param->raw_packet_list);
			}

			if ( param->fd_monitor > 0 && FD_ISSET(param->fd_monitor, &readfd)) {
				handle_dir_monitor(param->fd_monitor, param->transfer_list); 
			}
		}
	}
	DBG("Serial receiver end.\n");
	return NULL;
}


/// main routine
static void main_routine(void)
{
	pthread_t receiver_thread;
	struct serial_receiver_param param;
	char buf[1024];
	BUF_LIST transfer_list, raw_packet_list;

	DBG("main routine start\n");

	if ( load_conf() < 0 ) {
		PRINT_ERROR(-ERR_CONF, "Configuration file.\n");
		exit(2);
	}


	// BUF_LIST 초기화 
	init_buf_list(&transfer_list);
	init_buf_list(&raw_packet_list);
	param.transfer_list = &transfer_list;
	param.raw_packet_list = &raw_packet_list;


	set_protocol_version(global_conf.protocol_ver);
	set_favored_system_id(global_conf.system_id);

	param.fd_serial = open_serial_port(global_conf.port, global_conf.speed);
	if ( param.fd_serial < 0 ) {
		PRINT_ERROR(-ERR_PORT, "Serial port. %s\n", global_conf.port);
		exit(3);
	}

	// monitor 초기화 
	param.fd_monitor = init_dir_monitor(global_conf.transfer);
	if ( param.fd_monitor < 0 ) {
		PRINT_ERROR(-ERR_MONITOR, "Directory monitor. %s\n", global_conf.transfer);
		exit(4);
	}

	pthread_create(&receiver_thread, NULL, serial_receiver, (void*)&param);

	DBG("Main loop started.\n");
	while (!(global_flag & EXIT_MAIN_LOOP)) {
		if ( get_buf_list(&transfer_list, buf, sizeof(buf)) == 0 ) {
			DBG("file %s created.\n", buf);
			auto_send_data(param.fd_serial, buf);
		}
		usleep(10);

		if ( get_buf_list(&raw_packet_list, buf, sizeof(buf)) == 0 ) {
			DBG("%s packet received.\n", buf);

			if ( raw_packet_processor(param.fd_serial, strdup(buf), strlen(buf)) < 0 ) {
				continue;
			}
			if ( is_session_completed() ) {
				auto_write_session_data();
			}

		}
		usleep(10);

		if ( (global_flag & MANUAL_TRANSFER) && (global_conf.flag & CONF_FLAG_MANUAL_TRANSFER) ) {
			manual_send_data(param.fd_serial);

			global_flag &= (~MANUAL_TRANSFER);
		}
		usleep(10);
	}
	DBG("Main loop end.\n");

	pthread_join( receiver_thread, NULL);

	close_serial_port();
	close(param.fd_monitor);

	free_buf_list(&transfer_list);
	free_buf_list(&raw_packet_list);

	DBG("main routine end\n");
}


/// signal handler
static void signal_handler(int sig)
{
	DBG("catch signal %d\n", sig);
	if ( sig == SIGHUP) {
		global_flag |= EXIT_MAIN_LOOP ;
	} else if ( sig == SIGUSR1 ) {
		// manual transfer
		global_flag |= MANUAL_TRANSFER;
	} else if ( sig == SIGTERM ) {
		global_flag |= (EXIT_MAIN_LOOP | EXIT_PROGRAM);
	}
}


/// signal handler 설정 
/// \return 0 success
static int setup_signal(void)
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));

	act.sa_handler = signal_handler;
	if ( sigaction(SIGHUP, &act, NULL) < 0 ) {
		PRINT_ERROR(-ERR_SIGNAL, "SIGHUP\n");
		return -ERR_SIGNAL;
	}
	if ( sigaction(SIGUSR1, &act, NULL) < 0 ) {
		PRINT_ERROR(-ERR_SIGNAL, "SIGUSR1\n");
		return -ERR_SIGNAL;
	}
	if ( sigaction(SIGTERM, &act, NULL) < 0 ) {
		PRINT_ERROR(-ERR_SIGNAL, "SIGTERM\n");
		return -ERR_SIGNAL;
	}
	return 0;
}

/// Main
int main(int argc, char *argv[])
{
	if ( proc_cmd_options(argc, argv) < 0 ) {
		exit(1);
	}

	if ( setup_signal() < 0 ) {
		exit(1);
	}
	openlog("CDRMate", LOG_PID, LOG_USER);

	if ( write_pid() < 0 ) {
		exit(1);
	}

	printf("CDRMate Linux console V%s started. (This is not the official version)\n", VERSION_STR);
	syslog(LOG_ERR, "CDRMate Linux console V%s started. (This is not the official version)\n", VERSION_STR);
	while ( !(global_flag & EXIT_PROGRAM) ) {
		global_flag &=  ~(EXIT_MAIN_LOOP | EXIT_PROGRAM);
		main_routine();
	}
	PRINT("CDRMate Linux console terminated.\n");

	closelog();
	delete_pid();
	return 0;
}

