#ifndef _DATA_DUMP_H_
#define _DATA_DUMP_H_

#include "CDRType.h"
#include "clm_data.h"
#include "mrk_data.h"

void dump_HEADER(HEADER *p, int indent);
void dump_PATIENT_DATA (PATIENT_DATA *p, int indent);
void dump_VA_DATA(VA_DATA *p, int indent);
void dump_UN_VA_DATA(UN_VA_DATA *p, int indent);
void dump_M_DATA(M_DATA *p, int indent);
void dump_LENS_DATA (LENS_DATA *p, int indent);
void dump_SCA_DATA(SCA_DATA *p, int indent);
void dump_REFR_DATA(REFR_DATA *p, int indent);
void dump_RETINO_DATA(RETINO_DATA *p, int indent);
void dump_M_N_DATA(M_N_DATA *p, int indent);
void dump_SUBJ_DATA(SUBJ_DATA *p, int indent);
void dump_FIN_DATA(FIN_DATA *p, int indent);
void dump_NP_RA_DATA(NP_RA_DATA *p, int indent);
void dump_NPC_DATA(NPC_DATA *p, int indent);
void dump_NPA_DATA(NPA_DATA *p, int indent);
void dump_RA_DATA(RA_DATA *p, int indent);
void dump_RC_DATA(RC_DATA *p, int indent);
void dump_RC_TEST_DATA(RC_TEST_DATA *p, int indent);
void dump_PRISM_DATA(PRISM_DATA *p, int indent);
void dump_BVF_DATA(BVF_DATA *p, int indent);
void dump_BVF_TEST_DATA(BVF_TEST_DATA *p, int indent);
void dump_KR_DATA(KR_DATA *p, int indent);
void dump_KER_DATA(KER_DATA *p, int indent);
void dump_ZERNIKE_DATA(ZERNIKE_DATA *p, int indent);
void dump_COEFF_DATA(COEFF_DATA *p, int indent);
void dump_P_DATA(P_DATA *p, int indent);
void dump_clm_data(struct clm_data *p, int indent);
void dump_mrk_data(struct mrk_data *p, int indent);

#endif
