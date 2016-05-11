#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/data_dump.h"
#include "inc/main.h"

#define INDENT 2

/// 공백 생성 (나중에 free 로 해제할 것)
static char * make_space(int indent)
{
	char *buf;

	buf = (char*)malloc(indent + 1 );
	if ( buf ) {
		memset(buf, ' ', indent);
		buf[indent] = 0;
		return buf;
	} 
	DBG("insufficient memory.\n");
	exit(1);
	return NULL;
}


void dump_HEADER(HEADER *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sHEADER ----------\n", spc);
	printf("%scompany_info = [%s]\n", spc, p->company_info);
	printf("%smodel_name = [%s]\n", spc, p->model_name);
	printf("%sver_numb = [%s]\n", spc, p->ver_numb);
	printf("%spatient_id = [%s]\n", spc, p->patient_id);
	printf("%sdate = [%s]\n", spc, p->date);

	free(spc);
}

void dump_PATIENT_DATA (PATIENT_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sPATIENT_DATA ----------\n", spc);
	printf("%sage = [%s]\n", spc, p->age);
	printf("%sdomin_eye = [%s]\n", spc, p->domin_eye);
	printf("%spd = [%s]\n", spc, p->pd);
	printf("%snpd = [%s]\n", spc, p->npd);
	printf("%swd = [%s]\n", spc, p->wd);
	printf("%spd_l = [%s]\n", spc, p->pd_l);
	printf("%spd_r = [%s]\n", spc, p->pd_r);
	printf("%snpd_l = [%s]\n", spc, p->npd_l);
	printf("%snpd_r = [%s]\n", spc, p->npd_r);

	free(spc);
}

void dump_VA_DATA(VA_DATA *p, int indent) 
{
	char *spc;

	spc = make_space(indent);

	printf("%sVA_DATA ----------\n", spc);
	printf("%sou = [%s]\n", spc, p->ou);
	printf("%sr_va = [%s]\n", spc, p->r_va);
	printf("%sl_va = [%s]\n", spc, p->l_va);

	free(spc);
}

void dump_UN_VA_DATA(UN_VA_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sUN_VA_DATA ----------\n", spc);
	printf("%sf_va =\n", spc);
	dump_VA_DATA(&p->f_va, indent+INDENT);
	printf("%sn_va =\n", spc);
	dump_VA_DATA(&p->n_va, indent+INDENT);

	printf("%sbFVA = %d\n", spc, p->bFVA );
	printf("%sbNVA = %d\n", spc, p->bNVA );

	free(spc);
}

void dump_M_DATA(M_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sM_DATA ----------\n",spc);
	printf("%ssph = [%s]\n", spc, p->sph);
	printf("%scyl = [%s]\n", spc, p->cyl);
	printf("%saxs = [%s]\n", spc, p->axs);
	printf("%sadd = [%s]\n", spc, p->add);
	printf("%sp_x = [%s]\n", spc, p->p_x);
	printf("%sp_y = [%s]\n", spc, p->p_y);

	free(spc);
}

void dump_LENS_DATA (LENS_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sLENS_DATA ----------\n", spc);
	printf("%sf_r_main =\n", spc);
	dump_M_DATA(&p->f_r_main, indent+INDENT);
	printf("%sf_l_main =\n", spc);
	dump_M_DATA(&p->f_l_main, indent+INDENT);
	printf("%sf_va =\n", spc);
	dump_VA_DATA(&p->f_va, indent+INDENT);
	
	printf("%sbMain = %d\n", spc, p->bMain );
	printf("%sbVA = %d\n", spc, p->bVA);

	free(spc);
}

void dump_SCA_DATA(SCA_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sSCA_DATA ----------\n", spc);
	printf("%ssph = [%s]\n", spc, p->sph);
	printf("%scyl = [%s]\n", spc, p->cyl);
	printf("%saxs = [%s]\n", spc, p->axs);
	printf("%sse = [%s]\n", spc, p->se);

	free(spc);
}


void dump_REFR_DATA(REFR_DATA *p, int indent) 
{
	char *spc;

	spc = make_space(indent);

	printf("%sREFR_DATA ----------\n", spc);
	printf("%sf_r_sca =\n", spc);
	dump_SCA_DATA(&p->f_r_sca, indent+INDENT);
	printf("%sf_l_sca =\n", spc);
	dump_SCA_DATA(&p->f_l_sca, indent+INDENT);
	
	printf("%sbRefr = %d\n", spc, p->bRefr);

	free(spc);
}

void dump_RETINO_DATA(RETINO_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sRETINO_DATA ----------\n", spc);
	printf("%sf_r_sca =\n", spc);
	dump_SCA_DATA(&p->f_r_sca, indent+INDENT);
	printf("%sf_l_sca =\n", spc);
	dump_SCA_DATA(&p->f_l_sca, indent+INDENT);
	printf("%sbRetino = %d\n", spc, p->bRetino);
	free(spc);
}

void dump_M_N_DATA(M_N_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sM_N_DATA ----------\n", spc);
	printf("%ssph = [%s]\n", spc, p->sph);
	printf("%scyl = [%s]\n", spc, p->cyl);
	printf("%saxs = [%s]\n", spc, p->axs);
	printf("%sp_x = [%s]\n", spc, p->p_x);
	printf("%sp_y = [%s]\n", spc, p->p_y);

	free(spc);
}

void dump_SUBJ_DATA(SUBJ_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sSUBJ_DATA ----------\n", spc);
	printf("%sf_r_main =\n", spc);
	dump_M_DATA(&p->f_r_main, indent+INDENT);
	printf("%sf_l_main =\n", spc);
	dump_M_DATA(&p->f_l_main, indent+INDENT);

	printf("%sf_addva =\n", spc);
	dump_VA_DATA(&p->f_addva, indent+INDENT);
	printf("%sf_va =\n", spc);
	dump_VA_DATA(&p->f_va, indent+INDENT);

	printf("%sn_r_nmain =\n", spc);
	dump_M_N_DATA(&p->n_r_nmain, indent+INDENT);
	printf("%sn_l_nmain =\n", spc);
	dump_M_N_DATA(&p->n_l_nmain, indent+INDENT);

	printf("%sn_va =\n", spc);
	dump_VA_DATA(&p->n_va, indent+INDENT);

	printf("%sbFMain = %d\n", spc, p->bFMain);
	printf("%sbFAddVA = %d\n", spc, p->bFAddVA);
	printf("%sbFVA = %d\n", spc, p->bFVA);
	printf("%sbNMain  = %d\n", spc, p->bNMain);
	printf("%sbNVA = %d\n", spc, p->bNVA);

	free(spc);
}

void dump_FIN_DATA(FIN_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sFIN_DATA ----------\n", spc);
	printf("%sf_r_main =\n",spc);
	dump_M_DATA(&p->f_r_main, indent+INDENT);
	printf("%sf_l_main =\n", spc);
	dump_M_DATA(&p->f_l_main, indent+INDENT);

	printf("%sf_addva =\n", spc);
	dump_VA_DATA(&p->f_addva, indent+INDENT);
	printf("%sf_va =\n", spc);
	dump_VA_DATA(&p->f_va, indent+INDENT);
	
	printf("%sn_r_nmain =\n", spc);
	dump_M_N_DATA(&p->n_r_nmain, indent+INDENT);
	printf("%sn_l_nmain =\n", spc);
	dump_M_N_DATA(&p->n_l_nmain, indent+INDENT);
	printf("%sn_va =\n", spc);
	dump_VA_DATA(&p->n_va, indent+INDENT);

	printf("%sbFMain = %d\n", spc, p->bFMain);
	printf("%sbFAddVA = %d\n", spc, p->bFAddVA);
	printf("%sbFVA = %d\n", spc, p->bFVA);
	printf("%sbNMain = %d\n", spc, p->bNMain);
	printf("%sbNVA = %d\n", spc, p->bNVA);

	free(spc);
}

void dump_NPC_DATA(NPC_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sNPC_DATA ----------\n", spc);
	printf("%sbrk_cm = [%s]\n", spc, p->brk_cm);
	printf("%sbrk_ma = [%s]\n", spc, p->brk_ma);
	printf("%sbrk_prism = [%s]\n", spc, p->brk_prism);
	printf("%srcv_cm = [%s]\n", spc, p->rcv_cm);
	printf("%srcv_ma = [%s]\n", spc, p->rcv_ma);
	printf("%srcv_prism = [%s]\n", spc, p->rcv_prism);

	printf("%scm = [%s]\n", spc, p->cm);
	printf("%sma = [%s]\n", spc, p->ma);
	printf("%sprism = [%s]\n", spc, p->prism);

	free(spc);
}

void dump_NPA_DATA(NPA_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sNPA_DATA ----------\n", spc);
	printf("%sou_cm = [%s]\n", spc, p->ou_cm);
	printf("%sou_d = [%s]\n", spc, p->ou_d);
	printf("%sr_cm = [%s]\n", spc, p->r_cm);
	printf("%sr_d = [%s]\n", spc, p->r_d);
	printf("%sl_cm = [%s]\n", spc, p->l_cm);
	printf("%sl_d = [%s]\n", spc, p->l_d);
	printf("%scm = [%s]\n", spc, p->cm);
	printf("%sd = [%s]\n", spc, p->d);

	free(spc);
}

void dump_RA_DATA(RA_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sRA_DATA ----------\n", spc);
	printf("%sblur = [%s]\n", spc, p->blur);
	printf("%srecv = [%s]\n", spc, p->recv);
	free(spc);
}

void dump_NP_RA_DATA(NP_RA_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sNP_RA_DATA ----------\n", spc);
	printf("%snpc =\n", spc);
	dump_NPC_DATA(&p->npc, indent+INDENT);
	printf("%snpa =\n", spc);
	dump_NPA_DATA(&p->npa, indent+INDENT);

	printf("%snra =\n", spc);
	dump_RA_DATA(&p->nra, indent+INDENT);
	printf("%spra =\n", spc);
	dump_RA_DATA(&p->pra, indent+INDENT);

	free(spc);
}

void dump_RC_DATA(RC_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sRC_DATA ----------\n", spc);
	printf("%sblur = [%s]\n", spc, p->blur);
	printf("%s_break = [%s]\n", spc, p->_break);
	printf("%srecv = [%s]\n", spc, p->recv);

	free(spc);
}

void dump_RC_TEST_DATA(RC_TEST_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sRC_TEST_DATA ----------\n", spc);
	printf("%sf_nrc =\n", spc);
	dump_RC_DATA(&p->f_nrc, indent+INDENT);
	printf("%sf_prc =\n", spc);
	dump_RC_DATA(&p->f_prc, indent+INDENT);
	printf("%sn_nrc =\n", spc);
	dump_RC_DATA(&p->n_nrc, indent+INDENT);
	printf("%sn_prc =\n", spc);
	dump_RC_DATA(&p->n_prc, indent+INDENT);

	printf("%sbNNRC = %d\n", spc, p->bNNRC);
	printf("%sbNPRC = %d\n", spc, p->bNPRC);
	printf("%sbFNRC = %d\n", spc, p->bFNRC);
	printf("%sbFPRC = %d\n", spc, p->bFPRC);
	free(spc);
}

void dump_P_DATA(P_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sP_DATA ----------\n", spc);
	printf("%sp_x = [%s]\n", spc, p->p_x);
	printf("%sp_y = [%s]\n", spc, p->p_y);

	free(spc);
}

void dump_PRISM_DATA(PRISM_DATA *p, int indent)
{
	int i;

	char *spc;

	spc = make_space(indent);

	printf("%sPRISM_DATA ----------\n", spc);
	for ( i = 0 ; i < 6 ; i++) {
		printf("%sf_prism_r[%d] =\n", spc, i);
		dump_P_DATA(&p->f_prism_r[i], indent+INDENT);
		printf("%sf_prism_l[%d] =\n", spc, i);
		dump_P_DATA(&p->f_prism_l[i], indent+INDENT);
		printf("%sn_prism_r[%d] =\n", spc, i);
		dump_P_DATA(&p->n_prism_r[i], indent+INDENT);
		printf("%sn_prism_l[%d] =\n", spc, i);
		dump_P_DATA(&p->n_prism_l[i], indent+INDENT);

		printf("%sbFPrism[%d] = %d\n", spc, i, p->bFPrism[i]);
		printf("%sbNPrism[%d] = %d\n", spc, i, p->bNPrism[i]);
	}
	free(spc);
}

void dump_BVF_DATA(BVF_DATA *p, int indent) 
{
	char *spc;

	spc = make_space(indent);
	printf("%sBVF_DATA ----------\n", spc);
	printf("%sfusion = [%s]\n", spc, p->fusion);
	printf("%sstereo = [%s]\n", spc, p->stereo);
	printf("%smin_stereo = [%s]\n", spc, p->min_stereo);
	printf("%shoriz = [%s]\n", spc, p->horiz);
	printf("%svert = [%s]\n", spc, p->vert);
	free(spc);
}

void dump_BVF_TEST_DATA(BVF_TEST_DATA *p, int indent) 
{
	char *spc;

	spc = make_space(indent);
	printf("%sBVF_TEST_DATA ----------\n", spc);
	printf("%sf_bvf =\n", spc);
	dump_BVF_DATA(&p->f_bvf, indent+INDENT);
	printf("%sn_bvf =\n", spc);
	dump_BVF_DATA(&p->n_bvf, indent+INDENT);
	free(spc);
} 

void dump_KR_DATA(KR_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);

	printf("%sKR_DATA ----------\n", spc);
	printf("%smm = [%s]\n", spc, p->mm);
	printf("%sd = [%s]\n", spc, p->d);
	printf("%saxs = [%s]\n", spc, p->axs);
	free(spc);
} 

void dump_KER_DATA(KER_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);
	printf("%sKER_DATA ----------\n", spc);
	printf("%sr1_r =\n", spc);
	dump_KR_DATA(&p->r1_r, indent+INDENT);
	printf("%sr1_l =\n", spc);
	dump_KR_DATA(&p->r1_l, indent+INDENT);
	printf("%sr2_r =\n", spc);
	dump_KR_DATA(&p->r2_r, indent+INDENT);
	printf("%sr2_l =\n", spc);
	dump_KR_DATA(&p->r2_l, indent+INDENT);

	printf("%sbRight = %d\n", spc, p->bRight);
	printf("%sbLeft = %d\n", spc, p->bLeft);
	free(spc);
}

void dump_COEFF_DATA(COEFF_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);
	printf("%sCOEFF_DATA ----------\n", spc);
	printf("%ssph = [%s]\n", spc, p->sph);
	printf("%scyl = [%s]\n", spc, p->cyl);
	printf("%saxs = [%s]\n", spc, p->axs);
	printf("%sc_01 = [%s]\n", spc, p->c_01);
	printf("%sc_02 = [%s]\n", spc, p->c_02);
	printf("%sc_03 = [%s]\n", spc, p->c_03);
	printf("%sc_04 = [%s]\n", spc, p->c_04);
	printf("%sc_05 = [%s]\n", spc, p->c_05);
	printf("%sc_06 = [%s]\n", spc, p->c_06);
	printf("%sc_07 = [%s]\n", spc, p->c_07);
	printf("%sc_08 = [%s]\n", spc, p->c_08);
	printf("%sc_09 = [%s]\n", spc, p->c_09);
	free(spc);
}

void dump_ZERNIKE_DATA(ZERNIKE_DATA *p, int indent)
{
	char *spc;

	spc = make_space(indent);
	printf("%sZERNIKE_DATA ----------\n", spc);
	printf("%su_rat = [%s]\n", spc, p->u_rat);
	printf("%sh_max = [%s]\n", spc, p->h_max);
	printf("%sh_min = [%s]\n", spc, p->h_min);
	printf("%sl_max = [%s]\n", spc, p->l_max);
	printf("%sl_min = [%s]\n", spc, p->l_min);
	
	printf("%sl_eye =\n", spc);
	dump_COEFF_DATA(&p->l_eye, indent+INDENT);
	printf("%sr_eye =\n", spc);
	dump_COEFF_DATA(&p->r_eye, indent+INDENT);

	printf("%sisLeft = %d\n", spc, p->isLeft);
	printf("%sisRight = %d\n", spc, p->isRight);
	free(spc);
} 

void dump_clm_data(struct clm_data *p, int indent)
{
	char *spc;

	spc = make_space(indent);
	printf("%sclm_data ----------\n", spc);
	printf("%sm_sDataNo = [%s]\n", spc, p->m_sDataNo);
	printf("%sm_sPD = [%s]\n", spc, p->m_sPD);
	printf("%sm_Lens =\n", spc);
	dump_LENS_DATA(&p->m_Lens, indent+INDENT);

	free(spc);
}

void dump_mrk_data(struct mrk_data *p, int indent)
{
	char *spc;

	spc = make_space(indent);
	printf("%smrk_data ----------\n", spc);
	printf("%sm_sDataNo = [%s]\n", spc, p->m_sDataNo);
	printf("%sm_sPD = [%s]\n", spc, p->m_sPD);
	printf("%sm_Refr =\n", spc);
	dump_REFR_DATA(&p->m_Refr, indent+INDENT);
	printf("%sm_Ker=\n", spc);
	dump_KER_DATA(&p->m_Ker, indent+INDENT);

	free(spc);
}
