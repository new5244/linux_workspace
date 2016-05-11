#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "inc/data_dump.h"
#include "inc/main.h"
#include "inc/clm_data.h"


/// quatation mark 로 둘러싸인 스트링에서 실제 문자열만 추출
/// "string"  -> string
/// " 가 하나 밖에 없을 경우 에러로 간주
/// target buffer size 가 내용물 보다 적으면 target size 만큼만 저장 (\0 포함)
/// \param src
/// \param target
/// \param target_size
/// \return 0 success
/// \return -1 fail
int extract_str(char *src, char *target, int target_size)
{
	char *st, *ed;
	int len;

	st = strchr(src, '"');
	if ( st == NULL ) {
		return -1;
	}
	st++;

	ed = strchr( st, '"');
	if ( ed == NULL ) {
		return -1;
	}

	len = (int)ed - (int)st;

	if ( len >= target_size ) {
		len = target_size - 1;
	} 
	strncpy(target, st, len);
	target[len] = 0;
	return 0;
}

/// clm_data 필드 pointer 얻음
/// \param plm
/// \param idx
/// \return ptr
static char * get_clm_field_ptr(struct clm_data *plm, int idx)
{
	switch( idx) {
		case 0: 
			return plm->m_sDataNo;

		case 1:
		       	return plm->m_Lens.f_r_main.sph;

		case 2:
			return plm->m_Lens.f_r_main.cyl;

		case 3:
			return plm->m_Lens.f_r_main.axs;

		case 4:
			return plm->m_Lens.f_r_main.p_x;

		case 5:
			return plm->m_Lens.f_r_main.p_y;

		case 6:
		       	return plm->m_Lens.f_r_main.add;

		case 7:
			return plm->m_Lens.f_l_main.sph;

		case 8:
			return plm->m_Lens.f_l_main.cyl;

		case 9:
			return plm->m_Lens.f_l_main.axs;

		case 10:
			return plm->m_Lens.f_l_main.p_x;

		case 11:
			return plm->m_Lens.f_l_main.p_y;

		case 12: 
			return plm->m_Lens.f_l_main.add;

		case 13:
			return plm->m_sPD;
	}
	return NULL;
}

#define SEP ","

BOOL clm_data_ImportFile(struct clm_data *plm, char *path)
{
	FILE *fp;
	char buf[2048];
	int read_size;
	char *ptr;
	int idx;

	if ( (fp = fopen(path, "r")) == NULL ) {
		ERROR_LOG("file open error. %s\n", path);
		return FALSE;
	}

	read_size = fread(buf, 1, sizeof(buf)-1, fp);
	fclose(fp);

	if ( read_size == 0 ) {
		ERROR_LOG("file read error. %s\n", path);
		return FALSE;
	}
	buf[read_size] = 0;

	memset(plm, 0 , sizeof(struct clm_data));

	idx = 0;
	ptr = strtok(buf, SEP);
	while ( ptr != NULL && idx < MAX_CLM_FIELD ) {
		DBG("idx = %d, ptr = [%s]\n", idx, ptr);
		extract_str(ptr, get_clm_field_ptr(plm, idx), FIELD_SIZE);
		idx ++;
		ptr = strtok(NULL, SEP);
	}
	if ( idx < MAX_CLM_FIELD ) {
		DBG("insufficient data field. %d\n", idx);
	}

#ifndef NDEBUG 
	dump_clm_data(plm, 0);
#endif

	return TRUE;
}

/// clm packet 생성
/// \param buf (out) packet
/// \param buf_size (in)
/// \param nComputerNo 사용하지 않는 필드같다. 1 사용할 것
/// \param plm (in)
/// \return packet_size
int clm_data_GetPacketStr(char *buf, int buf_size, int nComputerNo, struct clm_data *plm)
{
	char tmp[1024];
	char size ;

	snprintf(tmp, sizeof(tmp)-1, "%s|%s^%s^%s^%s^%s^%s|%s^%s^%s^%s^%s^%s|%s|", 
			plm->m_sDataNo,
			plm->m_Lens.f_r_main.sph,
			plm->m_Lens.f_r_main.cyl,
			plm->m_Lens.f_r_main.axs,
			plm->m_Lens.f_r_main.p_x,
			plm->m_Lens.f_r_main.p_y,
			plm->m_Lens.f_r_main.add,
			plm->m_Lens.f_l_main.sph,
			plm->m_Lens.f_l_main.cyl,
			plm->m_Lens.f_l_main.axs,
			plm->m_Lens.f_l_main.p_x,
			plm->m_Lens.f_l_main.p_y,
			plm->m_Lens.f_l_main.add,
			plm->m_sPD);

	// encode the packet body's length. 
	size		= (char)(strlen(tmp) + 128)	;

	snprintf(buf, buf_size-1, "#00a%d %c%s$", nComputerNo-1, size, tmp) ;
	return strlen(buf);
}
