#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "inc/main.h"
#include "inc/console.h"
#include "inc/conf.h"
#include "inc/clm_data.h"
#include "inc/mrk_data.h"
#include "inc/cdr_session.h"
#include "inc/auto_send.h"
#include "inc/manual_send.h"

/// gui message 처리 
/// \param fd_serial
/// \param buf
static void process_gui_msg(int fd_serial, char *buf)
{
	char *key;
	char *value;
	char path[1024];
	int ret;

	ltrim(buf);
	rtrim(buf);

	key = strtok(buf, " ");
	if ( key == NULL ) {
		ERROR_LOG("no key [%s]\n", buf);
		return;
	}

	value = strtok(NULL, " ");
	if ( value == NULL ) {
		ERROR_LOG("no value [%s]\n", buf);
		return;
	}

	if ( strcmp(key, "SLT") == 0 ) {
		snprintf(path, sizeof(path)-1, "%s/%s", global_conf.transfer, value);

		if ( (ret = send_data_to_cdr(fd_serial, path)) < 0 ) {
			PRINT_ERROR(ret, "SLT: transfer error. %s\n", path);
		} else {
			PRINT("SLT: %s transferred.\n", path);
		}
	} else if ( strcmp(key, "SLD") == 0 ) {
		snprintf(path, sizeof(path)-1, "%s/%s", global_conf.received, value);
		unlink(path);
		PRINT("SLD: %s deleted.\n", path);
	} else if ( strcmp(key, "SLU") == 0 ) {
	} else {
		ERROR_LOG("unknown key [%s]\n", buf);
	}
}


/// 콘솔 입력 처리
/// \param buf command string
/// \param args serial fd
int handle_console(char *buf, int args)
{
	char *p;
	int fd_serial = args;

	// \n 제거 
	p = strrchr(buf, '\n');
	if ( p ) {
		*p = 0;
	}

	if ( global_conf.flag | CONF_FLAG_CONSOLE ) {
		if ( strcmp(buf, "q") == 0 ) {
			return -ERR_STOP;
		} else if (strncmp(buf, "clm ", 4) == 0 ) {
			// test clm data 

			struct clm_data test;
			char path[1024];

			sprintf(path, "%s/%s.clm", global_conf.transfer, &buf[4]);

			clm_data_ImportFile(&test, path);
		} else if (strncmp(buf, "mrk ", 4) == 0 ) {
			// test mrk data 

			struct mrk_data test;
			char path[1024];

			sprintf(path, "%s/%s.mrk", global_conf.transfer, &buf[4]);

			mrk_data_ImportFile(&test, path);

		} else if (strncmp(buf, "send ", 5) == 0 ) {
			// send test 
			struct clm_data clm_data;
			struct mrk_data mrk_data;
			char packet[1024];
			int packet_size;
			int ret;
			char path[1024];

			sprintf(path, "%s/%s", global_conf.transfer, &buf[5]);

			if ( strstr(&buf[5], ".mrk") != NULL) {
				if ( mrk_data_ImportFile(&mrk_data, path) == FALSE) {
					return 0;
				}

				packet_size = mrk_data_GetPacketStr(packet, sizeof(packet), 1, &mrk_data);
				DBG("mrk packet = [%s]\n", packet);

				ret = send_raw_packet(fd_serial, packet, packet_size);
				DBG("ret = %d\n", ret);
			} else if ( strstr(&buf[5], ".clm") != NULL ) {
				if ( clm_data_ImportFile(&clm_data, path) == FALSE ) {
					return 0;
				}
				;
				packet_size = clm_data_GetPacketStr(packet, sizeof(packet), 1, &clm_data);
				DBG("cdr packet = [%s]\n", packet);

				ret = send_raw_packet(fd_serial, packet, packet_size);
				DBG("ret = %d\n", ret);
			} else {
				printf("Invalid filename. %s\n", &buf[5]);
			}
		} else if (strcmp(buf, "h") == 0 ) {
			printf("console command -------\n");
			printf("q : quit\n");
			printf("clm [filename] : load clm data. file ext not needed.\n");
			printf("mrk [filename] : load mrk data. file ext not needed.\n");
			printf("send [filename.ext] : send clm/mrk data to CDR.\n");
		}
	}
	if ( is_slave_mode() ){
		// slave mode
		process_gui_msg(fd_serial, buf);
	}
	return 0;
}
