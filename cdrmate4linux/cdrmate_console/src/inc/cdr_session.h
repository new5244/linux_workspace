#ifndef _CDR_SESSION_H_
#define _CDR_SESSION_H_

#include "CDRType.h"
# include"cdr_packet.h"




struct cdr_session {
	struct _HEADER			header ;
	struct _PATIENT_DATA	patient ;
	struct _UN_VA_DATA		un_va ;
	struct _LENS_DATA		lens ;
	struct _REFR_DATA		refr ;
	struct _SUBJ_DATA		subj ;
	struct _FIN_DATA		fin ;
	struct _NP_RA_DATA		np_ra ;
	struct _RC_TEST_DATA	rc_test ;
	struct _PRISM_DATA		prism ;
	struct _BVF_TEST_DATA	bvf_test ;
	struct _KER_DATA		ker ;
	struct _ZERNIKE_DATA	zdata ;
	struct _RETINO_DATA		retino ;
	struct _TRANS_END		trans ;

	int						m_nFavoredSysID ;					// 0 ~ 7.
	int						m_nProtocolVer ;
	BOOL					m_arRcvDataDesc[DATA_DESC_N] ;
	BOOL					m_bCompleted ;

	struct cdr_packet packet;
	BOOL					m_bPrtcVerErr ;
	int						m_nErrVersion ;
};





void set_protocol_version(char *ver_str);
void set_favored_system_id(int id);
BOOL is_session_completed(void);
int send_raw_packet(int fd, char *buf, int buf_size);
int raw_packet_processor(int fd, char *raw_packet, int packet_size);
int auto_write_session_data(void);

#endif
