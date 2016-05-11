#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "inc/main.h"
#include "inc/conf.h"
#include "inc/serial.h"
#include "inc/cdr_session.h"
#include "inc/data_dump.h"
#include "inc/csv_export.h"

struct cdr_session g_session;

/// protocol version 설정
/// \param ver_str
void set_protocol_version(char *ver_str)
{
	struct {
		char *str;
		int ver;
	} list[] = {
		{"1.0", CDR_PRTC_V100},
		{"1.1", CDR_PRTC_V110},
		{"1.2", CDR_PRTC_V120},
	};
	int i;

	g_session.m_nProtocolVer = CDR_PRTC_V120;
	for (i = 0 ; i < 3 ; i ++ ) {
		if ( strstr(ver_str, list[i].str) ) {
			g_session.m_nProtocolVer = list[i].ver;
			break;
		}
	}
}

/// system id 설정
/// \param id
void set_favored_system_id(int id)
{
	if ( id < 0 || id > 7 ) {
		id = 0;
	}
	g_session.m_nFavoredSysID = id;
}


/// session 완료??
/// \return TRUE
/// \return FALSE
BOOL is_session_completed(void)
{
	return (g_session.m_bCompleted);
}

static void ClearData(struct cdr_session *ps) 
{ 
	int i;

	/*
	 *	 Clear CDR data structures. 
	 */
	memset((void*)&ps->header, 0, sizeof(ps->header)) ;
	memset((void*)&ps->patient, 0, sizeof(ps->patient)) ;
	memset((void*)&ps->un_va, 0, sizeof(ps->un_va)) ;
	memset((void*)&ps->lens, 0, sizeof(ps->lens)) ;
	memset((void*)&ps->refr, 0, sizeof(ps->refr)) ;
	memset((void*)&ps->subj, 0, sizeof(ps->subj)) ;
	memset((void*)&ps->fin, 0, sizeof(ps->fin)) ;
	memset((void*)&ps->np_ra, 0, sizeof(ps->np_ra)) ;
	memset((void*)&ps->rc_test, 0, sizeof(ps->rc_test)) ;
	memset((void*)&ps->prism, 0, sizeof(ps->prism)) ;
	memset((void*)&ps->bvf_test, 0, sizeof(ps->bvf_test)) ;
	memset((void*)&ps->ker, 0, sizeof(ps->ker)) ;
	memset((void*)&ps->trans, 0, sizeof(ps->trans)) ;
	memset((void*)&ps->zdata, 0, sizeof(ps->zdata)) ;
	memset((void*)&ps->retino, 0, sizeof(ps->retino)) ;

	/*
	 *	Initialize received data descriptions. 
	 */
	for (i = 0; i < DATA_DESC_N; i++) { 
		ps->m_arRcvDataDesc[i]		= FALSE ;
	}
	

	ps->m_arRcvDataDesc[0]		= TRUE ; // for header descriptor. 
	ps->m_bCompleted			= FALSE ; // completion of CDR data. 
}


static BOOL CheckRcvDataDesc(struct cdr_session *ps, char *pDesc)
{ 

	int				nIdx ;

	/*
	 *	Change the data descriptor to a data index. 
	 */
	if (!strcmp(pDesc, "__HD"))
		nIdx = 1 ;
	else if (!strcmp(pDesc, "__BA"))
		nIdx = 2 ;
	else if (!strcmp(pDesc, "AAUA"))
		nIdx = 3 ;
	else if (!strcmp(pDesc, "FBLM"))
		nIdx = 4 ;
	else if (!strcmp(pDesc, "FBRM"))
		nIdx = 5 ;
	else if (!strcmp(pDesc, "FRSU"))
		nIdx = 6 ;
	else if (!strcmp(pDesc, "FLSU"))
		nIdx = 7 ;
	else if (!strcmp(pDesc, "FDSU"))
		nIdx = 8 ;
	else if (!strcmp(pDesc, "FVSU"))
		nIdx = 9 ;
	else if (!strcmp(pDesc, "NRSU"))
		nIdx = 10 ;
	else if (!strcmp(pDesc, "NLSU"))
		nIdx = 11 ;
	else if (!strcmp(pDesc, "NVSU"))
		nIdx = 12 ;
	else if (!strcmp(pDesc, "FRFI"))
		nIdx = 13 ;
	else if (!strcmp(pDesc, "FLFI"))
		nIdx = 14 ;
	else if (!strcmp(pDesc, "FDFI"))
		nIdx = 15 ;
	else if (!strcmp(pDesc, "FVFI"))
		nIdx = 16 ;
	else if (!strcmp(pDesc, "NRFI"))
		nIdx = 17 ;
	else if (!strcmp(pDesc, "NLFI"))
		nIdx = 18 ;
	else if (!strcmp(pDesc, "NVFI"))
		nIdx = 19 ;
	else if (!strcmp(pDesc, "A_RA"))
		nIdx = 20 ;
	else if (!strcmp(pDesc, "A_RC"))
		nIdx = 21 ;
	else if (!strcmp(pDesc, "F_P1"))
		nIdx = 22 ;
	else if (!strcmp(pDesc, "A_P2"))
		nIdx = 23 ;
	else if (!strcmp(pDesc, "F_P3"))
		nIdx = 24 ;
	else if (!strcmp(pDesc, "F_P4"))
		nIdx = 25 ;
	else if (!strcmp(pDesc, "F_P5"))
		nIdx = 26 ;
	else if (!strcmp(pDesc, "A_P6"))
		nIdx = 27 ;
	else if (!strcmp(pDesc, "A_VF"))
		nIdx = 28 ;
	else if (!strcmp(pDesc, "_BKR"))
		nIdx = 29 ;
	else if (!strcmp(pDesc, "__TR"))
		nIdx = 30 ;
	else if (!strcmp(pDesc, "HDZN"))
		nIdx = 31 ;
	else if (!strcmp(pDesc, "LCZN"))
		nIdx = 32 ;
	else if (!strcmp(pDesc, "RCZN"))
		nIdx = 33 ;
	else if (!strcmp(pDesc, "FBRE"))
		nIdx = 34 ;
	else 
		nIdx = -1 ;

	/*
	 *	Return the received flag for data descriptor.
	 */
	if (nIdx >= 1 && nIdx <= DATA_DESC_N) 
	{ 
		if (ps->m_arRcvDataDesc[nIdx-1] == TRUE) { 
			return TRUE ;
		}
		else { 
			ps->m_arRcvDataDesc[nIdx-1]		= TRUE ;	
		}
	}

	return FALSE ;
}

static BOOL Set_Header(struct cdr_session *ps)
{

	char		fld[FIELD_SIZE] ;
	int			nHeadVer ;


	// company information... default "HUVITZ".  
	if (FetchField(&ps->packet, ps->header.company_info) < 0) 
		return FALSE ;

	// model name... "CDR-####"
	if (FetchField(&ps->packet, ps->header.model_name) < 0) 
		return FALSE ;

	// version number... "{V}{0-9}.{00-00}{a-z}"
	if (FetchField(&ps->packet, fld) < 0) {
		return FALSE ;
	}
	else {
		strcpy(ps->header.ver_numb, fld) ;
	}

	// patient id 
	if (FetchField(&ps->packet, fld) < 0) {
		return FALSE ;
	}
	else {
		strcpy(ps->header.patient_id, fld) ;
	}

	if (FetchField(&ps->packet, fld) < 0) {
		return FALSE ;
	}
	else {
		strcpy(ps->header.date, fld) ;
	}
	
	if (FetchField(&ps->packet, fld) < 0) { 
		// in case that the cdr pannel supports the ver 1.0.
		// SetProtocolVer(CDR_PRTC_V100) ;
		nHeadVer = CDR_PRTC_V100 ;
	}
	else { 
		if (!strcmp(fld, "V1.20")) { 
			nHeadVer		= CDR_PRTC_V120 ;
		}
		else if (!strcmp(fld, "V1.10")) { 
			nHeadVer		= CDR_PRTC_V110 ;
		}
		else if (!strcmp(fld, "V1.00")) { 
			nHeadVer		= CDR_PRTC_V100 ;
		}
		else { 
			nHeadVer		= CDR_PRTC_V120 ;
		}
	}

	DBG("set protocol: req= %s => res= %d\n", fld, nHeadVer) ;

	if (nHeadVer >= ps->m_nProtocolVer) { 
		ps->packet.m_nProtocolVer = ps->m_nProtocolVer;
		return TRUE ;
	}
	else { 
		ps->m_bPrtcVerErr	= TRUE ;
		ps->m_nErrVersion	= nHeadVer ;
		return FALSE ;
	}
}

static BOOL Set_Patient(struct cdr_session *ps)
{
	if (FetchField(&ps->packet, ps->patient.age) < 0)
		return FALSE ;
	if (FetchField(&ps->packet, ps->patient.domin_eye) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->patient.pd) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->patient.npd) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->patient.wd) < 0) 
		return FALSE ;

	if (ps->packet.m_nProtocolVer >= CDR_PRTC_V120) { 
		if (FetchField(&ps->packet, ps->patient.pd_l) < 0) 
			return FALSE ;
		if (FetchField(&ps->packet, ps->patient.pd_r) < 0) 
			return FALSE ;
		if (FetchField(&ps->packet, ps->patient.npd_l) < 0) 
			return FALSE ;
		if (FetchField(&ps->packet, ps->patient.npd_r) < 0) 
			return FALSE ;
	}
	else 
	{
		/*
		int			pdv ;

		pdv			= atoi(patient.pd) / 2 ;
		pds.Format(_T("%d"), pdv) ;
		strcpy(patient.pd_l, pds) ;
		strcpy(patient.pd_r, pds) ;

		pdv			= atoi(patient.npd) / 2 ;
		pds.Format(_T("%d"), pdv) ;
		strcpy(patient.npd_l, pds) ;
		strcpy(patient.npd_r, pds) ;
		*/
	}

	return TRUE ;
}

static BOOL Set_TransEnd(struct cdr_session *ps) 
{

	if (FetchField(&ps->packet, ps->trans.apparatus) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->trans.company) < 0) 
		return FALSE ;

	return TRUE ;
}

static BOOL Set_UN_VA(struct cdr_session *ps)
{

	char			fld[FIELD_SIZE] ;


	// ignore both distance and eye modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;

	if (FetchVA(&ps->packet, &ps->un_va.f_va) < 0) 
		return FALSE ;

	// ignore both distance and eye modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;

	if (FetchVA(&ps->packet, &ps->un_va.n_va) < 0) 
		return FALSE ;

	
	return TRUE ;
}

static BOOL Set_LENS(struct cdr_session *ps)
{
	char			fld[FIELD_SIZE] ;

	// ignore both far and right modifiers. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;
	
	if (FetchMain(&ps->packet, &ps->lens.f_r_main) < 0) 
		return FALSE ;

	// ignore both far and left modifiers. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;
	
	if (FetchMain(&ps->packet, &ps->lens.f_l_main) < 0) 
		return FALSE ;

	// ignore both far and va modifiers. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;
	
	if (FetchVA(&ps->packet, &ps->lens.f_va) < 0) 
		return FALSE ;

	
	return TRUE ;
}

static BOOL Set_REF(struct cdr_session *ps)
{
	
	char			fld[FIELD_SIZE] ;


	// ignore both far and right modifiers. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;
	
	if (FetchField(&ps->packet, ps->refr.f_r_sca.sph) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->refr.f_r_sca.cyl) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->refr.f_r_sca.axs) < 0) 
		return FALSE ;

	if (ps->packet.m_nProtocolVer >= CDR_PRTC_V110) 
	{
		if (FetchField(&ps->packet, ps->refr.f_r_sca.se) < 0) 
			return FALSE ;
	}

	// ignore both far and left modifiers. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;
	
	if (FetchField(&ps->packet, ps->refr.f_l_sca.sph) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->refr.f_l_sca.cyl) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->refr.f_l_sca.axs) < 0) 
		return FALSE ;

	if (ps->packet.m_nProtocolVer >= CDR_PRTC_V110) 
	{
		if (FetchField(&ps->packet, ps->refr.f_l_sca.se) < 0) 
			return FALSE ;
	}

	return TRUE ;
}

static BOOL Set_Retino(struct cdr_session *ps)
{
	char			fld[FIELD_SIZE] ;


	// ignore both far and right modifiers. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;
	
	if (FetchField(&ps->packet, ps->retino.f_r_sca.sph) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->retino.f_r_sca.cyl) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->retino.f_r_sca.axs) < 0) 
		return FALSE ;

	// ignore both far and left modifiers. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;
	
	if (FetchField(&ps->packet, ps->retino.f_l_sca.sph) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->retino.f_l_sca.cyl) < 0) 
		return FALSE ;
	if (FetchField(&ps->packet, ps->retino.f_l_sca.axs) < 0) 
		return FALSE ;

	return TRUE ;
}

static BOOL Set_SUBJ(struct cdr_session *ps, char *pDesc)
{
	
	char			fld[FIELD_SIZE] ;
	char			type[FIELD_SIZE] ;


	/*
	 *	Get sub type of Subject data descriptor. 
	 */
	if (strlen(pDesc) < 4) { 
		return FALSE ;
	}
	else { 
		strncpy(type, pDesc, 2) ;
		type[2]				= 0 ;
	}


	if (!strcmp(type, "FR"))
	{
		// FRSU 
		// ignore both far and right modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchMain(&ps->packet, &ps->subj.f_r_main) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "FL"))
	{	
		// FLSU
		// ignore both far and left modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchMain(&ps->packet, &ps->subj.f_l_main) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "FD"))
	{	
		// FDSU
		// ignore both far and addva modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchVA(&ps->packet, &ps->subj.f_addva) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "FV"))
	{
		// FVSU
		// ignore both far and va modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchVA(&ps->packet, &ps->subj.f_va) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "NR"))
	{
		// NRSU 
		// ignore both near and right modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchNMain(&ps->packet, &ps->subj.n_r_nmain) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "NL"))
	{
		// NLSU
		// ignore both near and left modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchNMain(&ps->packet, &ps->subj.n_l_nmain) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "NV"))
	{
		// NVSU
		// ignore both near and va modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchVA(&ps->packet, &ps->subj.n_va) < 0) 
			return FALSE ;
	}
	else 
	{
		return FALSE ;
	}
	
	return TRUE ;		
}

static BOOL Set_FIN(struct cdr_session *ps, char *pDesc)
{
	char		type[FIELD_SIZE] ;
	char		fld[FIELD_SIZE] ;


	/*
	 *	Get sub type of FIN data descriptor. 
	 */
	if (strlen(pDesc) < 4) { 
		return FALSE ;
	}
	else { 
		strncpy(type, pDesc, 2) ;
		type[2]				= 0 ;
	}

	
	if (!strcmp(type, "FR"))
	{
		// FRFI 
		// ignore both far and right modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchMain(&ps->packet, &ps->fin.f_r_main) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "FL"))
	{
		// FLFI
		// ignore both far and left modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchMain(&ps->packet, &ps->fin.f_l_main) < 0) 
			return FALSE ;
	}
	else if	(!strcmp(type, "FD"))
	{
		// FDFI
		// ignore both far and addva modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchVA(&ps->packet, &ps->fin.f_addva) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "FV"))
	{	
		// FVFI
		// ignore both far and va modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchVA(&ps->packet, &ps->fin.f_va) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "NR"))
	{
		// NRFI 
		// ignore both near and right modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchNMain(&ps->packet, &ps->fin.n_r_nmain) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "NL"))
	{
		// NLFI
		// ignore both near and left modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchNMain(&ps->packet, &ps->fin.n_l_nmain) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "NV"))
	{
		// NVFI
		// ignore both near and va modifiers. 
		if (FetchField(&ps->packet, fld) < 0)
			return FALSE ;
		if (FetchVA(&ps->packet, &ps->fin.n_va) < 0) 
			return FALSE ;
	}
	else 
	{
		return FALSE ;
	}


	return TRUE ;		
}

static BOOL Set_NP_RA(struct cdr_session *ps)
{

	if (ps->packet.m_nProtocolVer >= CDR_PRTC_V110) 
	{
		// ver 1.1, NPC Break & Recovery.
		if (FetchField(&ps->packet, ps->np_ra.npc.brk_cm) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npc.brk_ma) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npc.brk_prism) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npc.rcv_cm) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npc.rcv_ma) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npc.rcv_prism) < 0)
			return FALSE ;
	}
	else 
	{
		if (FetchField(&ps->packet, ps->np_ra.npc.brk_cm) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npc.brk_ma) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npc.brk_prism) < 0)
			return FALSE ;
	}

	if (ps->packet.m_nProtocolVer >= CDR_PRTC_V110) 
	{
		// ver 1.1, NPA OU, R, L. 
		if (FetchField(&ps->packet, ps->np_ra.npa.ou_cm) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npa.ou_d) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npa.r_cm) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npa.r_d) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npa.l_cm) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npa.l_d) < 0)
			return FALSE ;
	}
	else 
	{ 
		if (FetchField(&ps->packet, ps->np_ra.npa.ou_cm) < 0)
			return FALSE ;
		if (FetchField(&ps->packet, ps->np_ra.npa.ou_d) < 0)
			return FALSE ;
	}

	if (FetchField(&ps->packet, ps->np_ra.nra.blur) < 0)
		return FALSE ;
	if (FetchField(&ps->packet, ps->np_ra.nra.recv) < 0)
		return FALSE ;
	if (FetchField(&ps->packet, ps->np_ra.pra.blur) < 0)
		return FALSE ;
	if (FetchField(&ps->packet, ps->np_ra.pra.recv) < 0)
		return FALSE ;


	return TRUE ;
}

static BOOL Set_PRISM(struct cdr_session *ps, char *pDesc)
{
	int				idx ;
	char			fld[FIELD_SIZE] ;


	/*
	 *	Get prism data index from data descriptor. 
	 */
	if (strlen(pDesc) < 4 || pDesc[3] < '1' || pDesc[3] > '6') { 
		return FALSE ;
	}
	else { 
		idx				= pDesc[3] - '1' ;		 
	}


	// ignore a far modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	

	// ignore a right modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	
	if (FetchPrism(&ps->packet, &ps->prism.f_prism_r[idx]) < 0)
		return FALSE ;

	// ignore a left modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	
	if (FetchPrism(&ps->packet, &ps->prism.f_prism_l[idx]) < 0)
		return FALSE ;

	if (ps->packet.m_nProtocolVer >= CDR_PRTC_V110 &&  
		(idx == 0 || idx == 2 || idx == 3 || idx == 4)) 
	{
		// in ver 1.1, schober, coincidence, polarized cross, 
		// polarized cross w/Fix test data of Near point are skipped. 
		return TRUE ;
	}

	// ignore a near modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	

	// ignore a right modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	
	if (FetchPrism(&ps->packet, &ps->prism.n_prism_r[idx]) < 0)
		return FALSE ;

	// ignore a left modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	
	if (FetchPrism(&ps->packet, &ps->prism.n_prism_l[idx]) < 0)
		return FALSE ;

	return TRUE ;
}

static BOOL Set_BVF(struct cdr_session *ps)
{

	char			fld[FIELD_SIZE] ;


	// ignore a far modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	
	if (FetchBVF(&ps->packet, &ps->bvf_test.f_bvf, TRUE) < 0) 
		return FALSE ;

	// ignore a near modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	
	if (FetchBVF(&ps->packet, &ps->bvf_test.n_bvf, FALSE) < 0) 
		return FALSE ;

	return TRUE ;
}

static BOOL Set_KER(struct cdr_session *ps)
{
	
	char		fld[FIELD_SIZE] ;

	// ignore a right modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	
	if (FetchKR(&ps->packet, &ps->ker.r1_r) < 0)
		return FALSE ;

	// ignore a left modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	
	if (FetchKR(&ps->packet, &ps->ker.r1_l) < 0)
		return FALSE ;

	// ignore a right modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	
	if (FetchKR(&ps->packet, &ps->ker.r2_r) < 0)
		return FALSE ;

	// ignore a left modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;	
	if (FetchKR(&ps->packet, &ps->ker.r2_l) < 0)
		return FALSE ;

	return TRUE ;
}

static BOOL Set_Zernike(struct cdr_session *ps, char *pDesc) 
{
	char		type[FIELD_SIZE] ;
	// char		fld[FIELD_SIZE] ;

	if (strlen(pDesc) < 4) {
		return FALSE ;
	}
	else {
		strncpy(type, pDesc, 2) ;
		type[2]		= 0 ;
	}

	if (!strcmp(type, "HD"))
	{
		//	HDZN
		//	ignore both far and right modifiers. 
		/*
		if (FetchField(fld) < 0) {
			LTRACE("HD field error") ;
			return FALSE ;
		}
		*/
		if (FetchZHead(&ps->packet, &ps->zdata) < 0) {
			return FALSE ;
		}
	}
	else if (!strcmp(type, "LC"))
	{
		//	LCZN
		//	ignore both far and right modifiers. 
		/*
		if (FetchField(fld) < 0)
			return FALSE ;
		*/
		if (FetchZCoeff(&ps->packet, &ps->zdata.l_eye) < 0) 
			return FALSE ;
	}
	else if (!strcmp(type, "RC"))
	{
		//	RCZN
		//	ignore both far and right modifiers. 
		/*
		if (FetchField(fld) < 0) {
			LTRACE("RD field error") ;
			return FALSE ;
		}
		*/
		if (FetchZCoeff(&ps->packet, &ps->zdata.r_eye) < 0) {
			return FALSE ;
		}
	}
	else {
		return FALSE ;
	}

	return TRUE ;
}

static BOOL Set_RC_TEST(struct cdr_session *ps)
{
	char			fld[FIELD_SIZE] ;


	// ignore a far modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;

	if (FetchRC(&ps->packet, &ps->rc_test.f_nrc) < 0) 
		return FALSE ;
	if (FetchRC(&ps->packet, &ps->rc_test.f_prc) < 0)
		return FALSE ;
	
	// ignore a near modifier. 
	if (FetchField(&ps->packet, fld) < 0)
		return FALSE ;

	if (FetchRC(&ps->packet, &ps->rc_test.n_nrc) < 0) 
		return FALSE ;
	if (FetchRC(&ps->packet, &ps->rc_test.n_prc) < 0)
		return FALSE ;


	return TRUE ;
}

static BOOL ProcessPacketData(struct cdr_session *ps)
{
	char		*p ;
	BOOL		ret ;

	p			= ps->packet.m_sDesc ;

	if (!strcmp(p, "__HD")) 
	{ 
		ret		= Set_Header(ps) ; 
	}
	else if (!strcmp(p, "__BA")) 
	{ 
		ret		= Set_Patient(ps) ;
	}
	else if (!strcmp(p, "AAUA")) 
	{ 
		ret		= Set_UN_VA(ps) ;
	}
	else if (!strcmp(p, "FBLM"))
	{ 
		ret		= Set_LENS(ps) ;
	}
	else if (!strcmp(p, "FBRM")) 
	{ 
		ret		= Set_REF(ps) ;
	}
	else if (!strcmp(p, "FBRE"))
	{
		ret		= Set_Retino(ps) ;
	}
	else if (!strcmp(p + 2, "SU")) 
	{
		ret		= Set_SUBJ(ps, p) ;
	}
	else if (!strcmp(p + 2, "FI"))
	{
		ret		= Set_FIN(ps, p) ;
	}
	else if (!strcmp(p, "A_RA"))
	{
		ret		= Set_NP_RA(ps) ;
	}
	else if (!strcmp(p, "A_RC"))
	{ 
		ret		= Set_RC_TEST(ps) ;
	}
	else if (!strncmp(p, "A_P", 3) || !strncmp(p, "F_P", 3)) 
	{
		ret		= Set_PRISM(ps, p) ;
	}
	else if (!strcmp(p, "A_VF")) 
	{
		ret		= Set_BVF(ps) ;
	}
	else if (!strcmp(p, "_BKR")) 
	{ 
		ret		= Set_KER(ps) ;
	}
	else if (!strcmp(p, "__TR") || !strcmp(p, "@__T")) 
	{ 
		ret		= Set_TransEnd(ps) ;
		
		/*
		 *	This session data is completed. 
		 */
		ps->m_bCompleted	= TRUE ;

		DBG("session data is completed with transaction end\n") ;

#ifndef NDEBUG
		dump_HEADER(&ps->header, 0);
		dump_PATIENT_DATA(&ps->patient, 0);
		dump_UN_VA_DATA(&ps->un_va, 0);
		dump_LENS_DATA(&ps->lens, 0);
		dump_REFR_DATA(&ps->refr, 0);
		dump_RETINO_DATA(&ps->retino, 0);
		dump_SUBJ_DATA(&ps->subj, 0);
		dump_FIN_DATA(&ps->fin, 0);
		dump_NP_RA_DATA(&ps->np_ra, 0);
		dump_RC_TEST_DATA(&ps->rc_test, 0);
		dump_PRISM_DATA(&ps->prism, 0);
		dump_BVF_TEST_DATA(&ps->bvf_test, 0);
		dump_KER_DATA(&ps->ker, 0);
		dump_ZERNIKE_DATA(&ps->zdata, 0);
#endif 
	}
	else if (!strcmp(p + 2, "ZN"))
	{
		ret		= Set_Zernike(ps, p) ;
	}
	else 
	{
		// invalid packet descriptor. 
		ret		= FALSE ;

		ERROR_LOG("invalid packet data descriptor - desc:'%s'\n", p) ;
	}
	return ret ;
}

/// raw packet 전송
/// write() 로 한 번에 데이터를 전송못하는 경우를 대비해
/// \param fd
/// \param buf
/// \param buf_size
int send_raw_packet(int fd, char *buf, int buf_size)
{
	int sent = 0;
	int ret;

	while ( sent < buf_size) {
		ret = write(fd, &buf[sent], buf_size - sent);
		if ( ret < 0 ) {
			ERROR_LOG("write error.\n");
			return -ERR_SEND_RAW_PACKET;
		}
		sent += ret;
	}
	return 0;
}

static BOOL AddPacket(struct cdr_session *ps, char *pData, int nSize)
{ 
	
	/*
	 *	Initialize the current packet structure. 
	 *  ------------------------------------------------------------
	 *	1. compare data length with the size field. 
	 *	2. check validity of system id field. 
	 *	3. get data descriptor. 
	 *	4. move the pointer to before the first field. 
	 *
	 */
	ps->m_bPrtcVerErr		= FALSE ;

	if ( cdr_packet_setup(&ps->packet, pData, nSize) == FALSE ) {
		ERROR_LOG("failed to setup packet.\n") ;
		return FALSE ;
	}

	
	/*
	 *	Compare the system id of the message with the favored one. 
	 */
	if (ps->m_nFavoredSysID != ps->packet.m_nSystemID) { 
		ERROR_LOG("not favored system id - fav_sys_id:%d, pack_sys_id:%d\n", ps->m_nFavoredSysID, ps->packet.m_nSystemID) ;
		return FALSE ;
	}

	if (!strcmp(ps->packet.m_sDesc, "__HD")) 
	{
		//SetProtocolVer(nPrtcVer) ;
		
		/*
		*	Clear off CDR data structure and start new session. 
		*/	
		ClearData(ps) ;
		
		DBG("new session data started with header packet (%d)\n", ps->m_nProtocolVer) ;
	}
	else 
	{ 
		/*
		 *	Check it is already a received data descriptior. 
		 */
		if (CheckRcvDataDesc(ps, ps->packet.m_sDesc) == TRUE) { 
			DBG("already received data packet - desc:'%s'\n", ps->packet.m_sDesc);
			return TRUE ;			// data duplication is not error!
		}
	}
	return TRUE ;
}

/// 패킷 처리 
/// \param raw_packet
/// \param packet_size
int raw_packet_processor(int fd, char *raw_packet, int packet_size)
{
	int ret;
	char buf[1024];

	DBG("raw packet %d [%s]\n", packet_size, raw_packet);

	if ( AddPacket(&g_session, raw_packet, packet_size) == FALSE ) {
		return -ERR_ADD_PACKET;
	}

	if ( ProcessPacketData(&g_session) == FALSE) {
		ret = make_response_packet(&g_session.packet, MSG_NAK, buf, sizeof(buf)-1);
	} else {
		ret = make_response_packet(&g_session.packet, MSG_ACK, buf, sizeof(buf)-1);
	}
	ret = send_raw_packet(fd, buf, ret);

	cdr_packet_free(&g_session.packet);
	return ret;
}


/// default file name 생성 
/// \param patient_id
/// \param buf
/// \param buf_size
static char * make_def_file_name(char *patient_id)
{
	time_t cur_time;
	struct tm *p;
	static char buf[1024];

	time(&cur_time);
	p = localtime(&cur_time);

	snprintf(buf, sizeof(buf)-1, "%04d%02d%02d_%02d%02d%02d_%s", 
		p->tm_year + 1900,
		p->tm_mon + 1,
		p->tm_mday,

		p->tm_hour,
		p->tm_min,
		p->tm_sec,

		patient_id);

	return buf;
}

/// 수신한 데이터 파일에 기록
/// \return TRUE success
/// \return FALSE error
int auto_write_session_data(void)
{
	char path[1024];
	char *p;
	int ret;

	p = make_def_file_name(g_session.header.patient_id);

	snprintf(path, sizeof(path)-1, "%s/%s.csv", global_conf.received, p);
	
	mkdir(global_conf.received, 0777);

	ret = ExportAsCSV(&g_session, path, g_session.m_nProtocolVer);
	if ( ret == TRUE ) {
		PRINT("%s created.\n", path);
		SMSG("SLR %s.csv\n", p);
	} else {
		ERROR_LOG("Export error.\n");
	}
	return ret;
}
