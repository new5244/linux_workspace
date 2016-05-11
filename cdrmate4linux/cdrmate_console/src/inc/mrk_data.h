#ifndef _MRK_DATA_H_
#define _MRK_DATA_H_

#include "CDRType.h"

#define MAX_MRK_FIELD 14

struct mrk_data {
	REFR_DATA		m_Refr ;
	KER_DATA		m_Ker ;
	char m_sDataNo[FIELD_SIZE] ;
	char m_sPD[FIELD_SIZE];
};

BOOL mrk_data_ImportFile(struct mrk_data *pmrk, char *path);
int mrk_data_GetPacketStr(char *buf, int buf_size, int nComputerNo, struct mrk_data *pmrk);

#endif
