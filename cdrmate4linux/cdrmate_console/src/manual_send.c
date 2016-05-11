#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#include "inc/main.h"
#include "inc/conf.h"
#include "inc/clm_data.h"
#include "inc/mrk_data.h"
#include "inc/cdr_session.h"
#include "inc/auto_send.h"
#include "inc/manual_send.h"


/// 왼쪽 space 제거
/// \param buf (in/out)
void ltrim(char *buf)
{
	char *start_ptr = buf;

	while (*buf && isspace(*buf)) {
		buf++;
	}
	if ( start_ptr != buf) {
		strcpy(start_ptr, buf);
	}
}

/// 오른쪽 space 제거
/// \param buf (in/out)
void rtrim(char *buf)
{
	int len = strlen(buf);


	while(len > 0 && isspace(buf[len-1])) {
		len--;
	}
	buf[len] = 0;
}


/// 전송 파일 목록 생성
/// \param plist (out) 
/// \return file_count
static int make_file_list(BUF_LIST *plist)
{
	FILE *fp;
	char line[1024];
	int count = 0;
	
	fp = fopen(global_conf.manual_path, "r");
	if ( fp == NULL ) {
		ERROR_LOG("data file list error.\n");
		return -ERR_IO;
	}

	while( fgets(line, sizeof(line), fp) ) {

		// space 제거
		rtrim(line);
		ltrim(line);

		if (line[0] == 0 ) {
			continue;
		}

		add_buf_list(plist, line);
		count++;
	}
	fclose(fp);

	return count;
}


/// 데이터 전송
/// \param fd
/// \param plist
/// \return count
static int proc_file_list(int fd, BUF_LIST *plist)
{
	char tmp[1024];
	char path[1024];
	int count = 0;
	int ret;

	while ( get_buf_list(plist, tmp, sizeof(tmp)) == 0 ) {

		if ( tmp[0] == '/') {
			// full path 
			snprintf(path, sizeof(path)-1, "%s", tmp);
		} else {
			snprintf(path, sizeof(path)-1, "%s/%s", global_conf.transfer, tmp);
		}
		
		if ( (ret = send_data_to_cdr(fd, path)) < 0 ) {
			PRINT_ERROR(ret, "manual transfer error. %s\n", path);
		} else {
			PRINT("%s transferred.\n", path);
			count++;
		}
	}
	return count;
}

/// 수동으로 데이터 전송
/// \param fd serial port
/// \return count 전송한 데이터 개수
int manual_send_data(int fd)
{
	int count = 0;
	BUF_LIST manual_list;

	init_buf_list(&manual_list);

	if ( make_file_list(&manual_list) > 0 ) {
		count = proc_file_list(fd, &manual_list);
		if ( count > 0 ) {
			PRINT("Total %d files transferred.\n", count);
		}
	}
	free_buf_list(&manual_list);
	return count;
}
