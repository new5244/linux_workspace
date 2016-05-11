#ifndef _CDR_PACKET_H_
#define _CDR_PACKET_H_

#include "CDRType.h"

#define MSG_ACK 0x0001
#define MSG_NACK 0x0002
#define MSG_NAK MSG_NACK
#define MSG_BUSY 0x0009


struct cdr_packet {
	int magic;
	char*			m_pData ;
	int				m_nSize ;
	int				m_nFldPos ;
	char			m_sDesc[5] ;
	int				m_nSystemID ;
	int m_nProtocolVer;
	char			m_cSndSysID ;
	char			m_cSndNodeID ;
};

int cdr_packet_setup(struct cdr_packet *pkt, char *pData, int nSize);
void cdr_packet_free(struct cdr_packet *pkt);
int make_response_packet(struct cdr_packet *rcv_pkt, int type, char *buf, int buf_size);
int FetchField(struct cdr_packet *pkt, char *pField);
int FetchVA(struct cdr_packet *pkt, VA_DATA *va);
int FetchMain(struct cdr_packet *pkt, M_DATA *main);
int FetchNMain(struct cdr_packet *pkt, M_N_DATA *nmain);
int FetchRC(struct cdr_packet *pkt, RC_DATA *rc);
int FetchPrism(struct cdr_packet *pkt, P_DATA *prism);
int FetchBVF(struct cdr_packet *pkt, BVF_DATA *bvf, BOOL bFarPoint);
int FetchKR(struct cdr_packet *pkt, KR_DATA *kr);
int FetchZHead(struct cdr_packet *pkt, ZERNIKE_DATA *zdata);
int FetchZCoeff(struct cdr_packet *pkt, COEFF_DATA *coeff);


#endif

