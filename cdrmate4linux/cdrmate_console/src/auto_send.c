#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "inc/main.h"
#include "inc/conf.h"
#include "inc/clm_data.h"
#include "inc/mrk_data.h"
#include "inc/cdr_session.h"
#include "inc/auto_send.h"

/// 소문자로 변환 
/// \param str
static void lower_case(char *str)
{
	while( *str ) {

		*str = tolower(*str);
		str++;
	}
}

/// file 확장자 검사 
/// clm, CLM, mrk, MRK 
/// \return 0 clm
/// \return 1 mrk
int check_file_ext(char *path)
{
	char *p;
	char ext[8];


	p = strrchr(path, '.');
	if ( p== NULL ) { 
		return -ERR_INVAL;
	}
	p++;

	strncpy(ext, p, sizeof(ext)-1);
	ext[7] = 0;

	lower_case(ext);

	if ( strcmp(ext, "clm") == 0 ) {
		return 0;
	} else if ( strcmp(ext, "mrk") == 0 ) {
		return 1;
	}
	return -ERR_INVAL;
}


/// file 이동
/// \param src
/// \param dest
/// \return 0 success
static int file_move(char *src, char *dest)
{
	char buf[4096];
	int fd_src, fd_dest;
	int ret;

	if ( rename(src, dest ) == 0 ) {
		return 0;
	}

	// rename 실패,  직접 카피

	fd_dest = creat(dest, 0666);
	if ( fd_dest < 0 ) {
		DBG("creat error %s\n", dest);
		return -ERR_CREATE;
	}

	fd_src = open(src, O_RDONLY);
	if ( fd_src < 0 ) {
		DBG("open error %s\n", src);
		close(fd_dest);
		return -ERR_IO;
	}


	while ( (ret = read(fd_src, buf, sizeof(buf)) ) > 0 ) {
		if ( write(fd_dest, buf, ret) != ret ) { 
			DBG("write error.\n");
			ret = -ERR_WRITE;
			break;
		}
	}

	close(fd_dest);
	close(fd_src);

	if ( (ret = unlink(src)) < 0 ) {
		ret = -ERR_DEL;
	}
	return ret;
}

/// clm, mrk 데이터 파일 전송
/// \param fd
/// \param type 0 - clm, 1- mrk
/// \param path
int send_clm_mrk_file(int fd, int type, char *path)
{
	struct clm_data clm_data;
	struct mrk_data mrk_data;
	char packet[1024];
	int packet_size;

	if ( type == 0 ) {
		// clm data
		if ( clm_data_ImportFile(&clm_data, path) == FALSE ) {
			ERROR_LOG("clm data import error. %s\n", path);
			return -ERR_CLM_IMPORT;
		}
		packet_size = clm_data_GetPacketStr(packet, sizeof(packet), 1, &clm_data);
		DBG("clm packet = [%s]\n", packet);
	} else {
		// mrk data
		if ( mrk_data_ImportFile(&mrk_data, path) == FALSE) {
			ERROR_LOG("mrk data import error. %s\n", path);
			return -ERR_MRK_IMPORT;
		}
		packet_size = mrk_data_GetPacketStr(packet, sizeof(packet), 1, &mrk_data);
		DBG("mrk packet = [%s]\n", packet);

	}
	return send_raw_packet(fd, packet, packet_size);
}


/// clm, mrk data 자동 전송 
/// \param fd
/// \param path filename fullpath
/// \return 0 success
int send_data_to_cdr(int fd, char *path)
{
	int type;
	int ret;

	if ( (type = check_file_ext(path)) < 0 ) {
		ERROR_LOG("invalid file %s\n", path);
		return -ERR_INVALID_EXT;
	}

	if ( ( ret = send_clm_mrk_file(fd, type, path)) < 0 ) {
		DBG("send error %d.\n", ret);
		return ret;
	}
	return ret;
}

/// transfer 에서 파일을 읽어 데이터를 CDR 에 전송 후 transferred 로 이동 
/// \param fd
/// \param name name 파일이름만 , 내부에서 transfer, transfered path 와 결합 
int auto_send_data(int fd, char *name)
{
	char src_path[1024];
	char dest_path[1024];
	int ret;

	snprintf(src_path, sizeof(src_path)-1, "%s/%s", global_conf.transfer, name);

	if ( ( ret = send_data_to_cdr(fd, src_path)) < 0 ) {
		goto out;
	}

	snprintf(dest_path, sizeof(dest_path)-1, "%s/%s", global_conf.transferred, name);

	if ( global_conf.flag & CONF_FLAG_MOVE_FILE ) {
		if ( (ret = file_move(src_path, dest_path)) < 0 ) {
			ERROR_LOG("file move error. %d\n", ret);
			ret = -ERR_FILE_MOVE;
		}
	}

out:
	if ( ret < 0 ) {
		PRINT_ERROR(ret, "transfer error. %s\n", name);
	} else {
		PRINT("%s transferred.\n", name);
	}
	return ret;
}
