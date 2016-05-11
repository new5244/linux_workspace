#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#include "inc/main.h"
#include "inc/CDRType.h"
#include "inc/raw_packet.h"
#include "inc/buf_list.h"

#define MAX_BUF (4096)

// new code
static char internal_buf[MAX_BUF * 2];
static int internal_buf_pos = 0;

/// packet 생성하여 BUF_LIST 에 추가 
/// \param plist
/// \return count 
static int make_raw_packet(BUF_LIST *plist)
{
	char *scp, *ecp;
	int len;
	char tmp_buf[MAX_BUF];
	int packet_count = 0;

	scp = strchr(internal_buf, PC_SCP);
	while( scp != NULL) {
		ecp = strchr(scp, PC_ECP);
		if ( ecp == NULL ) {

			if ( scp != internal_buf ) {

				len = (int)(scp - internal_buf);
				memcpy(internal_buf, scp, internal_buf_pos - len);
				internal_buf_pos -= len; 
			}
			return packet_count;
		} else {
			len = (int)(ecp - scp) + 1;	
			memcpy(tmp_buf, scp, len);
			tmp_buf[len] = 0;
			packet_count++;
			DBG("tmp_buf = [%s]\n", tmp_buf);

			add_buf_list(plist, tmp_buf);
		}
		scp = strchr(ecp+1, PC_SCP);
	}
	internal_buf[0] = 0;
	internal_buf_pos = 0;
	return packet_count;
}

/// serial data 읽음 
/// \param fd
/// \param plist
int handle_serial_data(int fd, BUF_LIST *plist)
{
	char buf[MAX_BUF];
	int ret;
	int count = 0;

	while( ( ret = read(fd, buf, sizeof(buf)-1)) > 0 ) {
		buf[ret] = 0;

		memcpy(&internal_buf[internal_buf_pos], buf, ret +1);
		internal_buf_pos += ret;

		if (strchr(buf, PC_ECP) ) {
			count += make_raw_packet(plist);
		}
	}
	return count;
}


// old code

#if 0

pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

#define MAX_BUF (2048)

struct raw_buffer_rec {
	char buf[MAX_BUF];
	int pos;
	int available;

	char read_buf[MAX_BUF * 2];
	int read_pos;
} g_raw_buf;

/// raw buffer 초기화
void init_raw_buffer(void)
{
	memset( &g_raw_buf, 0, sizeof (g_raw_buf));
}




#if 0
/// serial port 에서 데이터를 읽어 버퍼에 저장 
/// \param fd
/// \return available_raw_packet_count
int copy_serial_data_to_raw_buffer(int fd)
{
	char buf[MAX_BUF];
	int ret;
	static int found_scp = 0;

	while( ( ret = read(fd, buf, sizeof(buf)-1)) > 0 ) {

		buf[ret] = 0;

		pthread_mutex_lock(&buffer_mutex);

		if ( ( MAX_BUF - g_raw_buf.pos ) > ret ) {
			memcpy(&g_raw_buf.buf[g_raw_buf.pos], buf, ret);
			g_raw_buf.pos += ret;
			g_raw_buf.buf[g_raw_buf.pos] = 0;
		} else { 
			// TO-DO
			// buffer overflow
			DBG("============================================\n");
			DBG("buffer overflow.\n");
			DBG("============================================\n");
		}
		if ( found_scp ) {
			if (strchr(buf, PC_ECP) ) {
				g_raw_buf.available ++;
				DBG("found packet end, count = %d, raw buf [%s]\n", g_raw_buf.available, g_raw_buf.buf);
				found_scp = 0;
			}
		} else {
			if (strchr(buf, PC_SCP) ) {
				found_scp = 1;
			}
		}
		pthread_mutex_unlock(&buffer_mutex);
	}
	return g_raw_buf.available;
}
#endif 

/// packet string 얻음 
/// \param pbuf (out) raw packet 버퍼 (내부에서 malloc() 으로 할당되므로 사용 후 해제할 것)
/// \param psize (out)
/// \return available_raw_packet_count 
int get_raw_packet(char **pbuf, int *psize)
{
	int ret = 0;
	char *start_p, *end_p;
	char *buf;
	int len;

	pthread_mutex_lock(&buffer_mutex);
	if ( g_raw_buf.available <= 0 ) {
		pthread_mutex_unlock(&buffer_mutex);
		return -ERR_NOENT;
	}

	memcpy(&g_raw_buf.read_buf[g_raw_buf.read_pos], g_raw_buf.buf, g_raw_buf.pos + 1);
	g_raw_buf.read_pos += g_raw_buf.pos;
	g_raw_buf.pos = 0;
	g_raw_buf.buf[0] = 0;
	ret = --g_raw_buf.available;
	pthread_mutex_unlock(&buffer_mutex);

	//DBG("read_pos = %d, read_buf[%s]\n", g_raw_buf.read_pos, g_raw_buf.read_buf);


	start_p = memchr(g_raw_buf.read_buf, PC_SCP, g_raw_buf.read_pos);
	if ( start_p == NULL ) {
		// 버퍼에 쓰레기 값이 들어 있으므로 모두 버린다.
		g_raw_buf.read_buf[0] = 0;
		g_raw_buf.read_pos = 0;
		DBG("no scp\n");
		return -ERR_INVAL;
	}

	if ( (end_p = memchr((char*)(start_p + 1), PC_ECP, g_raw_buf.read_pos - (int)(start_p - g_raw_buf.read_buf) - 1 )) == NULL ) {
		// end character 가 없으므로 다음 패킷을 받아야 한다.
		DBG("no ecp\n");
		return -ERR_CONT;
	}


	// raw packet length 계산 
	len = (int)end_p - (int)start_p + 1;

	buf = (char*)malloc( len + 1); // NULL terminate string
	if ( buf ) {
		memcpy(buf, start_p, len);
		buf[len] = 0;
	}


	// read_buf 재설정 
	memcpy(g_raw_buf.read_buf, (char*)(end_p + 1), g_raw_buf.read_pos - len + 1);
	g_raw_buf.read_pos -= len;

	DBG("raw packet created. cnt = %d [%s]\n", g_raw_buf.available, buf); 

	if ( pbuf) {
		*pbuf = buf;
	} else {
		free(buf);
	}
	if ( psize ) {
		*psize = len;
	}
	return ret;
}


#if 0
/// packet string 얻음 
/// \param pbuf (out) raw packet 버퍼 (내부에서 malloc() 으로 할당되므로 사용 후 해제할 것)
/// \param psize (out)
/// \return available_raw_packet_count 
int get_raw_packet(char **pbuf, int *psize)
{
	int ret = 0;
	char *start_p, *end_p;
	char *buf;
	int len;
	char tmp_buf[MAX_BUF];


#if 0
	if ( ( ret = pthread_mutex_trylock(&buffer_mutex) ) != 0 ) {
		if ( errno == EBUSY ) {
			return -ERR_BUSY;
		}
		DBG("mutex error %d\n", errno);
		return -ERR_IO;
	}
#endif 

	pthread_mutex_lock(&buffer_mutex);
			
	if ( g_raw_buf.available <= 0 ) {
		ret = -ERR_NOENT;
		goto out;
	}

	memcpy(tmp_buf, g_raw_buf.buf, MAX_BUF);


	start_p = strchr(tmp_buf, PC_SCP);
	if ( start_p == NULL ) {
		ret = -ERR_NOENT;
		if ( (end_p = strchr(start_p, PC_ECP)) == NULL ) {
			g_raw_buf.pos = 0;
			g_raw_buf.buf[0] = 0;
		} else {
			memcpy(g_raw_buf.buf, (char*)(end_p + 1), MAX_BUF - ( (int)end_p - (int)tmp_buf ) );
		}
		goto out;
	}

	if ( (end_p = strchr(start_p, PC_ECP)) == NULL ) {
		goto out;
	}

	len = (int)end_p - (int)start_p + 1;
	buf = (char*)malloc( len + 1); // NULL terminate string
	if ( buf == NULL ) {
		ret = -ERR_NOMEM;
		DBG("insufficient memory\n");
		goto out;
	}

	memcpy(buf, start_p, len);
	buf[len] = 0;


	memcpy(g_raw_buf.buf, (char*)(end_p + 1), g_raw_buf.pos - len);

	g_raw_buf.pos -= len;

	if ( g_raw_buf.available > 0 ) {
		g_raw_buf.available--;
	}

	if ( pbuf) {
		*pbuf = buf;
	} else {
		free(buf);
	}
	if ( psize ) {
		*psize = len;
	}

	ret = g_raw_buf.available;
out:
	pthread_mutex_unlock(&buffer_mutex);

	return ret;
}
#endif 



#endif 
