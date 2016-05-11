#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "inc/serial.h"
#include "inc/conf.h"
#include "inc/main.h"


struct serial_rec {
	int flag;
	int fd;
	struct termios oldtio;

	char connect_id[8+1];

} g_serial;


#define SFLAG_OPEN 0x01



/// 환경설정에 따라 시리얼 포트 열기 
/// \param port
/// \param baud
/// \return fd file descriptor on success
/// \return -1 already opened
/// \return -2 open error
int open_serial_port(char *port, int baud)
{
	int fd;
	struct termios newtio;

	if ( g_serial.flag & SFLAG_OPEN ) {
		ERROR_LOG("already opened. %s\n", port);
		return -ERR_DUP;
	}

	fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY );
	if ( fd < 0 ) {
		ERROR_LOG("open error. %s\n", port);
		return -ERR_INVAL;
	}

	memset(&g_serial, 0, sizeof(g_serial));

	tcgetattr(fd, &g_serial.oldtio); /* save current serial port settings */
	memset(&newtio, 0, sizeof(newtio)); /* clear struct for new port settings */

	newtio.c_cflag = baud | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR | IGNBRK;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;

	newtio.c_cc[VTIME]    = 1;     /* inter-character timer unused */
	newtio.c_cc[VMIN]     = 32;     /* blocking read until xxx character arrives */



	/* 
	now clean the modem line and activate the settings for the port
	*/
	tcflush(fd, TCOFLUSH);
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW, &newtio);

	g_serial.fd = fd;
	g_serial.flag = SFLAG_OPEN;


	{ 
		struct timeval tv;

		gettimeofday(&tv, NULL);

		sprintf(g_serial.connect_id, "%08x", (unsigned int)tv.tv_sec);
	}

	return fd;
}


/// 시리얼 포트 닫기
/// \return 0 success
/// \return -1 not opened
int close_serial_port(void)
{
	if ( !(g_serial.flag & SFLAG_OPEN) ) {
		return -1;
	}

	tcsetattr(g_serial.fd, TCSANOW, &g_serial.oldtio);
	close(g_serial.fd);

	memset(&g_serial, 0, sizeof(g_serial));

	return 0;
}

/// connet id string 얻음
/// \return str
char * get_connect_id_str(void)
{
	if ( !(g_serial.flag & SFLAG_OPEN) ) {
		return "00000000";
	}

	return g_serial.connect_id;
}

#if 0
/// serial port 에서 데이터 읽음
/// \param buf (out)
/// \param buf_size
/// \return read_size
int read_serial_port(char *buf, int buf_size)
{
	int ret = 0;
	int i = 0;
	int remain_size = buf_size;

	while( i < remain_size ) {
		ret = read(g_serial.fd, &buf[i], remain_size);
		if ( ret <= 0 ) {
			break;
		}
		i += ret;
		remain_size -= ret;
	}
	if ( ret < 0 ) { 
		return ret;
	}
	return i;
}
#endif 




