#ifndef _CLM_DATA_H_
#define _CLM_DATA_H_

#include "CDRType.h"

#define MAX_CLM_FIELD 14

struct clm_data {
	LENS_DATA m_Lens;
	char m_sDataNo[FIELD_SIZE];
	char m_sPD[FIELD_SIZE];
};


BOOL clm_data_ImportFile(struct clm_data *plm, char *path);

int extract_str(char *src, char *target, int target_size);

int clm_data_GetPacketStr(char *buf, int buf_size, int nComputerNo, struct clm_data *plm);

#endif
