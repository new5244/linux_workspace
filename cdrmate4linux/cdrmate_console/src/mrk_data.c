#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "inc/data_dump.h"
#include "inc/main.h"
#include "inc/mrk_data.h"

/// mrk_data 필드 pointer 얻음
/// \param pmrk
/// \param idx
/// \return ptr
static char * get_mrk_field_ptr(struct mrk_data *pmrk, int idx)
{
	switch(idx) {
		case 0:
			return pmrk->m_sDataNo;

		case 1:
			return pmrk->m_Refr.f_r_sca.sph;

		case 2:
			return pmrk->m_Refr.f_r_sca.cyl;

		case 3:
			return pmrk->m_Refr.f_r_sca.axs;

		case 4:
			return pmrk->m_Refr.f_l_sca.sph;

		case 5:
			return pmrk->m_Refr.f_l_sca.cyl;

		case 6:
			return pmrk->m_Refr.f_l_sca.axs;

		case 7:
			return pmrk->m_Ker.r1_r.mm;

		case 8:
			return pmrk->m_Ker.r2_r.mm;

		case 9:
			// 필드가 두 개 
			return pmrk->m_Ker.r1_r.axs;
			//m_Ker.r2_r.axs

		case 10:
			return pmrk->m_Ker.r1_l.mm;

		case 11:
			return pmrk->m_Ker.r2_l.mm;

		case 12:
			// 필드가 두 개 
			return pmrk->m_Ker.r1_l.axs;
			//m_Ker.r2_l.axs;

		case 13:
			return pmrk->m_sPD;
	}

	return NULL;
}

#define SEP ","

BOOL mrk_data_ImportFile(struct mrk_data *pmrk, char *path) 
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

	memset(pmrk, 0, sizeof(struct mrk_data));

	idx = 0;
	ptr = strtok(buf, SEP);
	while ( ptr != NULL && idx < MAX_MRK_FIELD ) {
		DBG("idx = %d, ptr = [%s]\n", idx, ptr);
		extract_str(ptr, get_mrk_field_ptr(pmrk, idx), FIELD_SIZE);
		idx ++;
		ptr = strtok(NULL, SEP);
	}
	if ( idx < MAX_MRK_FIELD ) {
		DBG("insufficient data field. %d\n", idx);
	}

	// 필드가 두 개, idx 9, 12 
	strncpy(pmrk->m_Ker.r2_r.axs, pmrk->m_Ker.r1_r.axs, FIELD_SIZE);
	strncpy(pmrk->m_Ker.r2_l.axs, pmrk->m_Ker.r1_l.axs, FIELD_SIZE);

#ifndef NDEBUG
	dump_mrk_data(pmrk, 0);
#endif

	return TRUE;
}

/// mrk packet 생성
/// \param buf (out) packet
/// \param buf_size (in)
/// \param nComputerNo 사용하지 않는 필드같다. 1 사용할 것
/// \param pmrk (in)
/// \return packet_size
int mrk_data_GetPacketStr(char *buf, int buf_size, int nComputerNo, struct mrk_data *pmrk)
{
	char tmp[1024];
	char size ;

	snprintf(tmp, sizeof(tmp)-1, "%s|%s^%s^%s|%s^%s^%s|%s^%s^%s|%s^%s^%s|%s|", 
		pmrk->m_sDataNo,
		pmrk->m_Refr.f_r_sca.sph,
		pmrk->m_Refr.f_r_sca.cyl, 
		pmrk->m_Refr.f_r_sca.axs,
		pmrk->m_Refr.f_l_sca.sph,
		pmrk->m_Refr.f_l_sca.cyl,
		pmrk->m_Refr.f_l_sca.axs,
		pmrk->m_Ker.r1_r.mm,
		pmrk->m_Ker.r2_r.mm,
		pmrk->m_Ker.r1_r.axs,
		pmrk->m_Ker.r1_l.mm,
		pmrk->m_Ker.r2_l.mm,
		pmrk->m_Ker.r1_l.axs,
		pmrk->m_sPD);
	
	// encode the packet body's length. 
	size		= (char)(strlen(buf) + 128)	;

	snprintf(buf, buf_size -1, "#00a%d %c%s$", nComputerNo-1, size, tmp) ;

	return strlen(buf);
}
