#include <stdio.h>

#include "inc/main.h"
#include "inc/cdr_session.h"
#include "inc/csv_export.h"


static int GetFieldNum(int nPrtcVer)
{
	// max index to dump session data. 
	switch (nPrtcVer) { 
	case CDR_PRTC_V120:
			return 186 ;
	case CDR_PRTC_V110:
			return 176 ;
	case CDR_PRTC_V100:
	default:
			return 190 ;
	}
}

static char * GetDataStr(struct cdr_session *ps, int nIdx, int nPrtcVer) 
{
	switch (nIdx) {
	case 0:		return ps->header.company_info ;
	case 1:		return ps->header.model_name ;
	case 2:		return ps->header.ver_numb ;
	case 3:		return ps->header.patient_id ;
	case 4:		return ps->header.date ;
	case 5:		return ps->patient.age ;
	case 6:		return ps->patient.domin_eye ;
	case 7:		return ps->patient.pd ;
	case 8:		return ps->patient.npd ;
	case 9:		return ps->patient.wd ;
	case 10:	return ps->un_va.f_va.ou ;
	case 11:	return ps->un_va.f_va.r_va ;
	case 12:	return ps->un_va.f_va.l_va ;
	case 13:	return ps->un_va.n_va.ou ;
	case 14:	return ps->un_va.n_va.r_va ;
	case 15:	return ps->un_va.n_va.l_va ;
	case 16:	return ps->lens.f_r_main.sph ;
	case 17:	return ps->lens.f_r_main.cyl ;
	case 18:	return ps->lens.f_r_main.axs ;
	case 19:	return ps->lens.f_r_main.add ;
	case 20:	return ps->lens.f_r_main.p_x ;
	case 21:	return ps->lens.f_r_main.p_y ;
	case 22:	return ps->lens.f_l_main.sph ;
	case 23:	return ps->lens.f_l_main.cyl ;
	case 24:	return ps->lens.f_l_main.axs ;
	case 25:	return ps->lens.f_l_main.add ;
	case 26:	return ps->lens.f_l_main.p_x ;
	case 27:	return ps->lens.f_l_main.p_y ;
	case 28:	return ps->lens.f_va.ou ;
	case 29:	return ps->lens.f_va.r_va ;
	case 30:	return ps->lens.f_va.l_va ;
	case 31:	return ps->refr.f_r_sca.sph ;
	case 32:	return ps->refr.f_r_sca.cyl ;
	case 33:	return ps->refr.f_r_sca.axs ;
	case 34:	return ps->refr.f_l_sca.sph ;
	case 35:	return ps->refr.f_l_sca.cyl ;
	case 36:	return ps->refr.f_l_sca.axs ;
	case 37:	return ps->subj.f_r_main.sph ;
	case 38:	return ps->subj.f_r_main.cyl ;
	case 39:	return ps->subj.f_r_main.axs ;
	case 40:	return ps->subj.f_r_main.add ;
	case 41:	return ps->subj.f_r_main.p_x ;
	case 42:	return ps->subj.f_r_main.p_y ;
	case 43:	return ps->subj.f_l_main.sph ;
	case 44:	return ps->subj.f_l_main.cyl ;
	case 45:	return ps->subj.f_l_main.axs ;
	case 46:	return ps->subj.f_l_main.add ;
	case 47:	return ps->subj.f_l_main.p_x ;
	case 48:	return ps->subj.f_l_main.p_y ;
	case 49:	return ps->subj.f_addva.ou ;
	case 50:	return ps->subj.f_addva.r_va ;
	case 51:	return ps->subj.f_addva.l_va ;
	case 52:	return ps->subj.f_va.ou ;
	case 53:	return ps->subj.f_va.r_va ;
	case 54:	return ps->subj.f_va.l_va ;
	case 55:	return ps->subj.n_r_nmain.sph ;
	case 56:	return ps->subj.n_r_nmain.cyl ;
	case 57:	return ps->subj.n_r_nmain.axs ;
	case 58:	return ps->subj.n_r_nmain.p_x ;
	case 59:	return ps->subj.n_r_nmain.p_y ;
	case 60:	return ps->subj.n_l_nmain.sph ;
	case 61:	return ps->subj.n_l_nmain.cyl ;
	case 62:	return ps->subj.n_l_nmain.axs ;
	case 63:	return ps->subj.n_l_nmain.p_x ;
	case 64:	return ps->subj.n_l_nmain.p_y ;
	case 65:	return ps->subj.n_va.ou ;
	case 66:	return ps->subj.n_va.r_va ;
	case 67:	return ps->subj.n_va.l_va ;
	case 68:	return ps->fin.f_r_main.sph ;
	case 69:	return ps->fin.f_r_main.cyl ;
	case 70:	return ps->fin.f_r_main.axs ;
	case 71:	return ps->fin.f_r_main.add ;
	case 72:	return ps->fin.f_r_main.p_x ;
	case 73:	return ps->fin.f_r_main.p_y ;
	case 74:	return ps->fin.f_l_main.sph ;
	case 75:	return ps->fin.f_l_main.cyl ;
	case 76:	return ps->fin.f_l_main.axs ;
	case 77:	return ps->fin.f_l_main.add ;
	case 78:	return ps->fin.f_l_main.p_x ;
	case 79:	return ps->fin.f_l_main.p_y ;
	case 80:	return ps->fin.f_addva.ou ;
	case 81:	return ps->fin.f_addva.r_va ;
	case 82:	return ps->fin.f_addva.l_va ;
	case 83:	return ps->fin.f_va.ou ;
	case 84:	return ps->fin.f_va.r_va ;
	case 85:	return ps->fin.f_va.l_va ;
	case 86:	return ps->fin.n_r_nmain.sph ;
	case 87:	return ps->fin.n_r_nmain.cyl ;
	case 88:	return ps->fin.n_r_nmain.axs ;
	case 89:	return ps->fin.n_r_nmain.p_x ;
	case 90:	return ps->fin.n_r_nmain.p_y ;
	case 91:	return ps->fin.n_l_nmain.sph ;
	case 92:	return ps->fin.n_l_nmain.cyl ;
	case 93:	return ps->fin.n_l_nmain.axs ;
	case 94:	return ps->fin.n_l_nmain.p_x ;
	case 95:	return ps->fin.n_l_nmain.p_y ;
	case 96:	return ps->fin.n_va.ou ;
	case 97:	return ps->fin.n_va.r_va ;
	case 98:	return ps->fin.n_va.l_va ;
	}
	
	if (nPrtcVer == CDR_PRTC_V100) 
	{
		switch (nIdx) {
		case 99:	return ps->np_ra.npc.cm ;
		case 100:	return ps->np_ra.npc.ma ;
		case 101:	return ps->np_ra.npc.prism ;
		case 102:	return ps->np_ra.npa.cm ;
		case 103:	return ps->np_ra.npa.d ;
		case 104:	return ps->np_ra.nra.blur ;
		case 105:	return ps->np_ra.nra.recv ;
		case 106:	return ps->np_ra.pra.blur ;
		case 107:	return ps->np_ra.pra.recv ;
		case 108:	return ps->rc_test.f_nrc.blur ;
		case 109:	return ps->rc_test.f_nrc._break ;
		case 110:	return ps->rc_test.f_nrc.recv ;
		case 111:	return ps->rc_test.f_prc.blur ;
		case 112:	return ps->rc_test.f_prc._break ;
		case 113:	return ps->rc_test.f_prc.recv ;
		case 114:	return ps->rc_test.n_nrc.blur ;
		case 115:	return ps->rc_test.n_nrc._break ;
		case 116:	return ps->rc_test.n_nrc.recv ;
		case 117:	return ps->rc_test.n_prc.blur ;
		case 118:	return ps->rc_test.n_prc._break ;
		case 119:	return ps->rc_test.n_prc.recv ;
		case 120:	return ps->prism.f_prism_r[0].p_x ;
		case 121:	return ps->prism.f_prism_r[0].p_y ;
		case 122:	return ps->prism.f_prism_l[0].p_x ;
		case 123:	return ps->prism.f_prism_l[0].p_y ;
		case 124:	return ps->prism.n_prism_r[0].p_x ;
		case 125:	return ps->prism.n_prism_r[0].p_y ;
		case 126:	return ps->prism.n_prism_l[0].p_x ;
		case 127:	return ps->prism.n_prism_l[0].p_y ;
		case 128:	return ps->prism.f_prism_r[1].p_x ;
		case 129:	return ps->prism.f_prism_r[1].p_y ;
		case 130:	return ps->prism.f_prism_l[1].p_x ;
		case 131:	return ps->prism.f_prism_l[1].p_y ;
		case 132:	return ps->prism.n_prism_r[1].p_x ;
		case 133:	return ps->prism.n_prism_r[1].p_y ;
		case 134:	return ps->prism.n_prism_l[1].p_x ;
		case 135:	return ps->prism.n_prism_l[1].p_y ;
		case 136:	return ps->prism.f_prism_r[2].p_x ;
		case 137:	return ps->prism.f_prism_r[2].p_y ;
		case 138:	return ps->prism.f_prism_l[2].p_x ;
		case 139:	return ps->prism.f_prism_l[2].p_y ;
		case 140:	return ps->prism.n_prism_r[2].p_x ;
		case 141:	return ps->prism.n_prism_r[2].p_y ;
		case 142:	return ps->prism.n_prism_l[2].p_x ;
		case 143:	return ps->prism.n_prism_l[2].p_y ;
		case 144:	return ps->prism.f_prism_r[3].p_x ;
		case 145:	return ps->prism.f_prism_r[3].p_y ;
		case 146:	return ps->prism.f_prism_l[3].p_x ;
		case 147:	return ps->prism.f_prism_l[3].p_y ;
		case 148:	return ps->prism.n_prism_r[3].p_x ;
		case 149:	return ps->prism.n_prism_r[3].p_y ;
		case 150:	return ps->prism.n_prism_l[3].p_x ;
		case 151:	return ps->prism.n_prism_l[3].p_y ;
		case 152:	return ps->prism.f_prism_r[4].p_x ;
		case 153:	return ps->prism.f_prism_r[4].p_y ;
		case 154:	return ps->prism.f_prism_l[4].p_x ;
		case 155:	return ps->prism.f_prism_l[4].p_y ;
		case 156:	return ps->prism.n_prism_r[4].p_x ;
		case 157:	return ps->prism.n_prism_r[4].p_y ;
		case 158:	return ps->prism.n_prism_l[4].p_x ;
		case 159:	return ps->prism.n_prism_l[4].p_y ;
		case 160:	return ps->prism.f_prism_r[5].p_x ;
		case 161:	return ps->prism.f_prism_r[5].p_y ;
		case 162:	return ps->prism.f_prism_l[5].p_x ;
		case 163:	return ps->prism.f_prism_l[5].p_y ;
		case 164:	return ps->prism.n_prism_r[5].p_x ;
		case 165:	return ps->prism.n_prism_r[5].p_y ;
		case 166:	return ps->prism.n_prism_l[5].p_x ;
		case 167:	return ps->prism.n_prism_l[5].p_y ;
		case 168:	return ps->bvf_test.f_bvf.fusion ;
		case 169:	return ps->bvf_test.f_bvf.stereo ;
		case 170:	return ps->bvf_test.f_bvf.min_stereo ;
		case 171:	return ps->bvf_test.f_bvf.horiz ;
		case 172:	return ps->bvf_test.f_bvf.vert ;
		case 173:	return ps->bvf_test.n_bvf.fusion ;
		case 174:	return ps->bvf_test.n_bvf.stereo ;
		case 175:	return ps->bvf_test.n_bvf.min_stereo ;
		case 176:	return ps->bvf_test.n_bvf.horiz ;
		case 177:	return ps->bvf_test.n_bvf.vert ;
		case 178:	return ps->ker.r1_r.mm ;
		case 179:	return ps->ker.r1_r.d ;
		case 180:	return ps->ker.r1_r.axs ;
		case 181:	return ps->ker.r1_l.mm ;
		case 182:	return ps->ker.r1_l.d ;
		case 183:	return ps->ker.r1_l.axs ;
		case 184:	return ps->ker.r2_r.mm ;
		case 185:	return ps->ker.r2_r.d ;
		case 186:	return ps->ker.r2_r.axs ;
		case 187:	return ps->ker.r2_l.mm ;
		case 188:	return ps->ker.r2_l.d ;
		case 189:	return ps->ker.r2_l.axs ;
		}		
	}
	else 
	{
		switch (nIdx) {
		case 99:	return ps->np_ra.npc.brk_cm ;
		case 100:	return ps->np_ra.npc.brk_ma ;
		case 101:	return ps->np_ra.npc.brk_prism ;
		case 102:	return ps->np_ra.npc.rcv_cm ;
		case 103:	return ps->np_ra.npc.rcv_ma ;
		case 104:	return ps->np_ra.npc.rcv_prism ;
		case 105:	return ps->np_ra.npa.ou_cm ;
		case 106:	return ps->np_ra.npa.ou_d ;
		case 107:	return ps->np_ra.npa.r_cm ;
		case 108:	return ps->np_ra.npa.r_d ;
		case 109:	return ps->np_ra.npa.l_cm ;
		case 110:	return ps->np_ra.npa.l_d ;
		case 111:	return ps->np_ra.nra.blur ;
		case 112:	return ps->np_ra.nra.recv ;
		case 113:	return ps->np_ra.pra.blur ;
		case 114:	return ps->np_ra.pra.recv ;
		case 115:	return ps->rc_test.f_nrc.blur ;
		case 116:	return ps->rc_test.f_nrc._break ;
		case 117:	return ps->rc_test.f_nrc.recv ;
		case 118:	return ps->rc_test.f_prc.blur ;
		case 119:	return ps->rc_test.f_prc._break ;
		case 120:	return ps->rc_test.f_prc.recv ;
		case 121:	return ps->rc_test.n_nrc.blur ;
		case 122:	return ps->rc_test.n_nrc._break ;
		case 123:	return ps->rc_test.n_nrc.recv ;
		case 124:	return ps->rc_test.n_prc.blur ;
		case 125:	return ps->rc_test.n_prc._break ;
		case 126:	return ps->rc_test.n_prc.recv ;
		case 127:	return ps->prism.f_prism_r[0].p_x ;
		case 128:	return ps->prism.f_prism_r[0].p_y ;
		case 129:	return ps->prism.f_prism_l[0].p_x ;
		case 130:	return ps->prism.f_prism_l[0].p_y ;
		case 131:	return ps->prism.f_prism_r[1].p_x ;
		case 132:	return ps->prism.f_prism_r[1].p_y ;
		case 133:	return ps->prism.f_prism_l[1].p_x ;
		case 134:	return ps->prism.f_prism_l[1].p_y ;
		case 135:	return ps->prism.n_prism_r[1].p_x ;
		case 136:	return ps->prism.n_prism_r[1].p_y ;
		case 137:	return ps->prism.n_prism_l[1].p_x ;
		case 138:	return ps->prism.n_prism_l[1].p_y ;
		case 139:	return ps->prism.f_prism_r[2].p_x ;
		case 140:	return ps->prism.f_prism_r[2].p_y ;
		case 141:	return ps->prism.f_prism_l[2].p_x ;
		case 142:	return ps->prism.f_prism_l[2].p_y ;
		case 143:	return ps->prism.f_prism_r[3].p_x ;
		case 144:	return ps->prism.f_prism_r[3].p_y ;
		case 145:	return ps->prism.f_prism_l[3].p_x ;
		case 146:	return ps->prism.f_prism_l[3].p_y ;
		case 147:	return ps->prism.f_prism_r[4].p_x ;
		case 148:	return ps->prism.f_prism_r[4].p_y ;
		case 149:	return ps->prism.f_prism_l[4].p_x ;
		case 150:	return ps->prism.f_prism_l[4].p_y ;
		case 151:	return ps->prism.f_prism_r[5].p_x ;
		case 152:	return ps->prism.f_prism_r[5].p_y ;
		case 153:	return ps->prism.f_prism_l[5].p_x ;
		case 154:	return ps->prism.f_prism_l[5].p_y ;
		case 155:	return ps->prism.n_prism_r[5].p_x ;
		case 156:	return ps->prism.n_prism_r[5].p_y ;
		case 157:	return ps->prism.n_prism_l[5].p_x ;
		case 158:	return ps->prism.n_prism_l[5].p_y ;
		case 159:	return ps->bvf_test.f_bvf.fusion ;
		case 160:	return ps->bvf_test.f_bvf.stereo ;
		case 161:	return ps->bvf_test.f_bvf.min_stereo ;
		case 162:	return ps->bvf_test.f_bvf.horiz ;
		case 163:	return ps->bvf_test.n_bvf.fusion ;
		case 164:	return ps->ker.r1_r.mm ;
		case 165:	return ps->ker.r1_r.d ;
		case 166:	return ps->ker.r1_r.axs ;
		case 167:	return ps->ker.r1_l.mm ;
		case 168:	return ps->ker.r1_l.d ;
		case 169:	return ps->ker.r1_l.axs ;
		case 170:	return ps->ker.r2_r.mm ;
		case 171:	return ps->ker.r2_r.d ;
		case 172:	return ps->ker.r2_r.axs ;
		case 173:	return ps->ker.r2_l.mm ;
		case 174:	return ps->ker.r2_l.d ;
		case 175:	return ps->ker.r2_l.axs ;
		}

		if (nPrtcVer == CDR_PRTC_V120) 
		{ 
			switch (nIdx) {
			case 176:	return ps->retino.f_r_sca.sph ;
			case 177:	return ps->retino.f_r_sca.cyl ;
			case 178:	return ps->retino.f_r_sca.axs ;
			case 179:	return ps->retino.f_l_sca.sph ;
			case 180:	return ps->retino.f_l_sca.cyl ;
			case 181:	return ps->retino.f_l_sca.axs ;
			case 182:	return ps->patient.pd_l ;
			case 183:	return ps->patient.pd_r ;
			case 184:	return ps->patient.npd_l ;
			case 185:	return ps->patient.npd_r ;
			}
		}
	}

	return NULL ;
}

BOOL ExportAsCSV(struct cdr_session *ps, char *pPath, int nPrtcVer) 
{
	int		nField ;
	char	*p ;
	FILE *fp;
	int i, j;

	if (pPath == NULL) { 
		return FALSE ;
	}
	
	fp = fopen(pPath, "w");
	if ( fp == NULL ) {
		ERROR_LOG("fopen error. %s\n", pPath);
		return FALSE;
	}

	if (nPrtcVer == CDR_PRTC_V110) { 
		fputs("\"V1.1\",", fp);
	}
	else if (nPrtcVer == CDR_PRTC_V120) { 
		fputs("\"V1.2\",", fp) ;
	}

	nField = GetFieldNum(nPrtcVer) ;
	for (i = 0; i < nField; i++) { 
		if ((p = GetDataStr(ps, i, nPrtcVer)) != NULL) { 
			fputs("\"", fp);
			fputs(p, fp);
			fputs("\",", fp);
		}
		else { 
			fputs("\"\",", fp);
		}

		if (nPrtcVer == CDR_PRTC_V120) 
		{
			if (i == 7) 
			{
				for (j = 182; j <= 183; j++) { 
					if ((p = GetDataStr(ps, j, nPrtcVer)) != NULL) { 
						fputs("\"", fp);
						fputs(p, fp);
						fputs("\",", fp);
					}
					else { 
						fputs("\"\",", fp);
					}
				}
			}
			else if (i == 8)
			{
				for (j = 184; j <= 185; j++) { 
					if ((p = GetDataStr(ps, j, nPrtcVer)) != NULL) { 
						fputs("\"", fp);
						fputs(p, fp);
						fputs("\",", fp);
					}
					else { 
						fputs("\"\",", fp);
					}
				}
			}
			else if (i == 36)
			{
				for (j = 176; j <= 181; j++) { 
					if ((p = GetDataStr(ps, j, nPrtcVer)) != NULL) { 
						fputs("\"", fp);
						fputs(p, fp);
						fputs("\",", fp);
					}
					else { 
						fputs("\"\",", fp);
					}
				}
			}
			else if (i == 175)
			{
				break ;
			}
		}
	}

	fputs("\"@\"", fp);
	fclose(fp);
	return TRUE ;
}
