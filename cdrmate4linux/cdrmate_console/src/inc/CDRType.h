
#ifndef __CDRTYPE_H__
#define __CDRTYPE_H__

#include "comm_pc_def.h"


typedef unsigned int BOOL;
#define FALSE 0
#define TRUE 1


#define CDR_PRTC_UNKNOWN		0xffff
#define CDR_PRTC_V100			0x0000
#define CDR_PRTC_V110			0x0001
#define CDR_PRTC_V120			0x0002

#define FIELD_SIZE				128
#define MSG_LEN_MIN				17 
#define SYS_ID_MIN				0 
#define SYS_ID_MAX				7
#define DATA_DESC_N				34

#define WAV_ZPOLY				9 


typedef struct _CAN_HD 
{
	char			rcv_sys_id[FIELD_SIZE] ;		// {0-a}
	char			rcv_node_id[FIELD_SIZE] ;		// {0-3}
	char			snd_sys_id[FIELD_SIZE] ;		// {0-a}
	char			snd_node_id[FIELD_SIZE] ;		// {0-3}
	char			can_command[FIELD_SIZE] ;		// {a-z}
	char			can_data_size[FIELD_SIZE] ;		// "$"

} CAN_HD ;


typedef struct _HEADER
{
	/*-------- Transmission Starter ----*/
	// product info 
	char			company_info[FIELD_SIZE] ;
	char			model_name[FIELD_SIZE] ;
	char			ver_numb[FIELD_SIZE] ;
	/*
	char			ver_symb ;		// {V}
	char			major_num ;		// {0-9}
	char			minor_num ;		// {00-00}
	char			bug_fix_num ;	// {a-z} */

	// general data info
	char			patient_id[FIELD_SIZE] ;
	char			date[FIELD_SIZE] ;
	/*
	char			patient_id_symb ;	// {P}
	char			patient_id_date	;	// {date without delimeter}
	char			system_id ;			// {0-7} 
	char			seq_no ;			// {000-999} 
	char			year ;				// {0000-9999}
	char			month ;				// {01-12}
	char			day	;				// {01-31} */

} HEADER ;


typedef struct _PATIENT_DATA 
{
	/*------ Patient's Basic Data ------*/ 
	char			age[FIELD_SIZE] ;			// {00-99}
	char			domin_eye[FIELD_SIZE] ;		// {L|R}
	char			pd[FIELD_SIZE] ;			// {48.0-80.0}
	char			npd[FIELD_SIZE] ;			// {45.0-75.0}
	char			wd[FIELD_SIZE] ;			// {35-70}
	char			pd_l[FIELD_SIZE] ;
	char			pd_r[FIELD_SIZE] ;
	char			npd_l[FIELD_SIZE] ;
	char			npd_r[FIELD_SIZE] ;

} PATIENT_DATA ;


typedef struct _VA_DATA 
{
	char			ou[FIELD_SIZE] ;		
	char			r_va[FIELD_SIZE] ;
	char			l_va[FIELD_SIZE] ;

} VA_DATA ;


typedef struct _UN_VA_DATA
{
	/*--------- Unaided VA ---------------*/
	struct _VA_DATA		f_va ;
	struct _VA_DATA		n_va ;

	BOOL	bFVA ;
	BOOL	bNVA ;

} UN_VA_DATA ;


typedef struct _M_DATA 
{
	char			sph[FIELD_SIZE] ;
	char			cyl[FIELD_SIZE] ;
	char			axs[FIELD_SIZE] ;
	char			add[FIELD_SIZE] ;
	char			p_x[FIELD_SIZE] ;
	char			p_y[FIELD_SIZE] ;

} M_DATA ;


typedef struct _M_N_DATA 
{
	char			sph[FIELD_SIZE] ;
	char			cyl[FIELD_SIZE] ;
	char			axs[FIELD_SIZE] ;
	char			p_x[FIELD_SIZE] ;
	char			p_y[FIELD_SIZE] ;

} M_N_DATA ;


typedef struct _LENS_DATA 
{
	/*------- Lensometry data ------------*/
	struct _M_DATA		f_r_main ;
	struct _M_DATA		f_l_main ;
	struct _VA_DATA		f_va ;
	
	BOOL	bMain ;
	BOOL	bVA ;

} LENS_DATA ;


typedef struct _SCA_DATA 
{
	char			sph[FIELD_SIZE] ;
	char			cyl[FIELD_SIZE] ;
	char			axs[FIELD_SIZE] ;
	char			se[FIELD_SIZE] ;

} SCA_DATA ;


typedef struct _REFR_DATA 
{
	/*------ Refractometry Data ------------*/
	struct _SCA_DATA	f_r_sca ;
	struct _SCA_DATA	f_l_sca ;
	
	BOOL	bRefr ;

} REFR_DATA ;


typedef struct _SUBJ_DATA 
{
	/*------- Subjective Data --------------*/
	struct _M_DATA		f_r_main ;
	struct _M_DATA		f_l_main ;
	struct _VA_DATA		f_addva ;
	struct _VA_DATA		f_va ;
	struct _M_N_DATA	n_r_nmain ;
	struct _M_N_DATA	n_l_nmain ;
	struct _VA_DATA		n_va ;

	BOOL	bFMain ;
	BOOL	bFAddVA ;
	BOOL	bFVA ;
	BOOL	bNMain ;
	BOOL	bNVA ;

} SUBJ_DATA ;


typedef struct _FIN_DATA
{
	/*------ Final Prescription Data ---------*/
	struct _M_DATA		f_r_main ;
	struct _M_DATA		f_l_main ;
	struct _VA_DATA		f_addva ;
	struct _VA_DATA		f_va ;
	struct _M_N_DATA	n_r_nmain ;
	struct _M_N_DATA	n_l_nmain ;
	struct _VA_DATA		n_va ;

	BOOL	bFMain ;
	BOOL	bFAddVA ;
	BOOL	bFVA ;
	BOOL	bNMain ;
	BOOL	bNVA ;

} FIN_DATA ;


typedef struct _NPC_DATA 
{
	char			brk_cm[FIELD_SIZE] ;
	char			brk_ma[FIELD_SIZE] ;
	char			brk_prism[FIELD_SIZE] ;
	char			rcv_cm[FIELD_SIZE] ;
	char			rcv_ma[FIELD_SIZE] ;
	char			rcv_prism[FIELD_SIZE] ;

	// for compatilbity with previous ver. 
	char			cm[FIELD_SIZE] ;
	char			ma[FIELD_SIZE] ;
	char			prism[FIELD_SIZE] ;

} NPC_DATA ;


typedef struct _NPA_DATA 
{
	char			ou_cm[FIELD_SIZE] ;
	char			ou_d[FIELD_SIZE] ;
	char			r_cm[FIELD_SIZE] ;
	char			r_d[FIELD_SIZE] ;
	char			l_cm[FIELD_SIZE] ;
	char			l_d[FIELD_SIZE] ;

	// for compatilbity with previous ver. 
	char			cm[FIELD_SIZE] ;
	char			d[FIELD_SIZE] ;

} NPA_DATA ;


typedef struct _RA_DATA 
{
	char			blur[FIELD_SIZE] ;
	char			recv[FIELD_SIZE] ;

} RA_DATA ;


typedef struct _NP_RA_DATA 
{
	/*------ NP&RA Test Data ------------------*/
	struct _NPC_DATA	npc ;
	struct _NPA_DATA	npa ;
	struct _RA_DATA		nra ;
	struct _RA_DATA		pra ;

} NP_RA_DATA ;


typedef struct _RC_DATA 
{
	char			blur[FIELD_SIZE] ;
	char			_break[FIELD_SIZE] ;
	char			recv[FIELD_SIZE] ;

} RC_DATA ;


typedef struct _RC_TEST_DATA
{
	/*--- Relational Convergence Test Data ----*/
	struct _RC_DATA		f_nrc ;
	struct _RC_DATA		f_prc ;
	struct _RC_DATA		n_nrc ;
	struct _RC_DATA		n_prc ;

	BOOL	bNNRC ;
	BOOL	bNPRC ;
	BOOL	bFNRC ;
	BOOL	bFPRC ;

} RC_TEST_DATA ;


typedef struct _P_DATA 
{
	char			p_x[FIELD_SIZE] ;
	char			p_y[FIELD_SIZE] ;

} P_DATA ;


typedef struct _PRISM_DATA
{
	/*-------- Prism Test Data --------------*/

	/*
		- prism data array - 
		1th element	:		Schober 
		2th			:		Von Grafe
		3th			:		Coincidence 
		4th			:		Polarized Cross
		5th			:		Polarized Cross Test with Fixation 
		6th			:		Maddox Rod Test
	*/

	struct _P_DATA		f_prism_r[6] ;
	struct _P_DATA		f_prism_l[6] ;
	struct _P_DATA		n_prism_r[6] ;
	struct _P_DATA		n_prism_l[6] ;

	BOOL	bFPrism[6] ;
	BOOL	bNPrism[6] ;

} PRISM_DATA ;


typedef struct _BVF_DATA 
{
	char			fusion[FIELD_SIZE] ;
	char			stereo[FIELD_SIZE] ;
	char			min_stereo[FIELD_SIZE] ;
	char			horiz[FIELD_SIZE] ;
	char			vert[FIELD_SIZE] ;

} BVF_DATA ;


typedef struct _BVF_TEST_DATA 
{
	/*----------- BVF Test Data ---------------*/
	struct _BVF_DATA	f_bvf ;
	struct _BVF_DATA	n_bvf ;

} BVF_TEST_DATA ;


typedef struct _KR_DATA 
{
	char			mm[FIELD_SIZE] ;
	char			d[FIELD_SIZE] ;
	char			axs[FIELD_SIZE] ;

} KR_DATA ;

typedef struct _KER_DATA 
{
	/*--------- Keratometry Data --------------*/
	struct _KR_DATA		r1_r ;
	struct _KR_DATA		r1_l ;
	struct _KR_DATA		r2_r ;
	struct _KR_DATA		r2_l ;

	BOOL	bRight ;
	BOOL	bLeft ;

} KER_DATA ;

typedef struct _COEFF_DATA
{
	char		sph[FIELD_SIZE] ;
	char		cyl[FIELD_SIZE] ;
	char		axs[FIELD_SIZE] ;
	char		c_01[FIELD_SIZE] ;
	char		c_02[FIELD_SIZE] ;
	char		c_03[FIELD_SIZE] ;
	char		c_04[FIELD_SIZE] ;
	char		c_05[FIELD_SIZE] ;
	char		c_06[FIELD_SIZE] ;
	char		c_07[FIELD_SIZE] ;
	char		c_08[FIELD_SIZE] ;
	char		c_09[FIELD_SIZE] ;

} COEFF_DATA ;

typedef struct _ZERNIKE_DATA
{
	char		u_rat[FIELD_SIZE] ;
	char		h_max[FIELD_SIZE] ;
	char		h_min[FIELD_SIZE] ;
	char		l_max[FIELD_SIZE] ;
	char		l_min[FIELD_SIZE] ;
	
	struct _COEFF_DATA		l_eye ;
	struct _COEFF_DATA		r_eye ;

	BOOL		isLeft ;
	BOOL		isRight ;

} ZERNIKE_DATA ;

typedef struct _RETINO_DATA 
{
	struct _SCA_DATA	f_r_sca ;
	struct _SCA_DATA	f_l_sca ;

	BOOL		bRetino ;

} RETINO_DATA ;

typedef struct _TRANS_END
{
	/*--------- Transaction end ---------------*/ 
	char			apparatus[FIELD_SIZE] ;
	char			company[FIELD_SIZE] ;

} TRANS_END ;


#endif 
