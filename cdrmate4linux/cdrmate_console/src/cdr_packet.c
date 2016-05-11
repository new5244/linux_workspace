#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "inc/serial.h"
#include "inc/cdr_packet.h"
#include "inc/main.h"


/// raw packet string 으로 CDR packet 생성 
/// \param pkt (out)
/// \param pData raw packet data (malloc() 으로 할당한 데이터를 넘길것)
/// \param nSize
/// \return TRUE ok
/// \return FALSE error
int cdr_packet_setup(struct cdr_packet *pkt, char *pData, int nSize)
{
	char len;
	int pos;

	memset(pkt, 0, sizeof(struct cdr_packet));

	pkt->m_pData = pData;
	pkt->m_nSize = nSize;

	/*
	 *	Check the packet length with data size field in CAN header. 
	 */
	len = pData[6] - 128 ;

	if (len != nSize || nSize < MSG_LEN_MIN) { 
		DBG("invalid packet length - size:%d, data:%d\n", len, (unsigned int) pData[6]) ;
		return FALSE ;
	}

	/*
	 *	 Get the sender's system and node IDs in CAN protocol.  
	 */
	pkt->m_cSndSysID	= pData[3] ;
	pkt->m_cSndNodeID = pData[4] ;	


	/*
	 *	Compare the system id of the message with the favored one. 
	 */
	pkt->m_nSystemID = pData[3] - '0' ;
	
	if (pkt->m_nSystemID < SYS_ID_MIN && pkt->m_nSystemID > SYS_ID_MAX) { 
		DBG("invalid packet system id - sys_id:%d\n", pkt->m_nSystemID) ;
		return FALSE ;
	}

	
	/*
	 *	Get the data descriptor.  
	 */
	if (strncmp(pData + 14, "__TR", 4) == 0) {
		pos				= 14 ;
	}
	else { 
		pos				= 13 ;
	}

	strncpy(pkt->m_sDesc, pData + pos, 4) ;
	pkt->m_sDesc[4] = 0 ;


	DBG("packet setup - len:%d, sys_id:%d, desc:'%s'\n", len, pkt->m_nSystemID, pkt->m_sDesc) ;

	
	/*
	 *	Move the current pointer to a field delimeter after a descriptor. 
	 */
	pkt->m_nFldPos			= pos + 4 ;

	pkt->magic = 0x20118279;

	return TRUE ;
}

/// CDR packet 메모리 해재
/// \param pkt
void cdr_packet_free(struct cdr_packet *pkt)
{
	if ( pkt->magic != 0x20118279 ) {
		return ;
	}

	free(pkt->m_pData);

	memset(pkt, 0, sizeof(struct cdr_packet));
}


/// 응답 raw 패킷 생성 
/// \param rcv_pkt 직전 수신 패킷 
/// \param type response type
/// \param buf (out)
/// \param buf_size
/// \return packet_size 생성된 packet size
int make_response_packet(struct cdr_packet *rcv_pkt, int type, char *buf, int buf_size)
{
	int len ;

	snprintf(buf, buf_size, "#%c%ca0  @%1d@/%1d@%1d%1d%4s%s$", 
		rcv_pkt->m_cSndSysID,
		rcv_pkt->m_cSndNodeID,
		rcv_pkt->m_nSystemID,
		type,
		rcv_pkt->m_nProtocolVer, 
		0, // favoredsysid
		(type == MSG_ACK || type == MSG_BUSY) ? rcv_pkt->m_sDesc : "____", 
		get_connect_id_str()); 

	/*
	 *	 Set the packet length field. 
	 */
	len = strlen(buf);
	buf[6]= (char) (len + 128) ;

	return len;
}


static int GetAt(struct cdr_packet *pkt, int nPos)
{ 
	if (pkt->m_pData == NULL || nPos >= pkt->m_nSize) 
	{ 
		return -1 ;
	}
	else 
	{ 
		return pkt->m_pData[nPos] ;
	}
}

static BOOL GetField(struct cdr_packet *pkt, char *pField, int nBegPos, int nEndPos)
{
	int				flen ;


	flen			= nEndPos - nBegPos + 1 ;

	/*
	 *	Get a part of field in packet data. 
	 */
	if ((nBegPos > nEndPos) || flen >= FIELD_SIZE) 
	{
		pField[0]			= 0 ;
		return FALSE ;
	}
	else 
	{ 
		strncpy(pField, pkt->m_pData + nBegPos, flen) ;
		pField[flen]		= 0 ;

		return TRUE ;
	}
}

int FetchField(struct cdr_packet *pkt, char *pField)
{ 
	int pos, beg, end ;
	char ch ;

	pos = pkt->m_nFldPos ;

	/*
	 *	Skip a delimeter before starting of data field. 
	 */
	if ((ch = GetAt(pkt, pos)) < 0) { 
		return -1 ;
	}

	if (ch == PC_BD || ch == PC_RD || ch == PC_FD || ch == PC_SD) {
		pos++ ;
	}

	// eat spaces
	while ( GetAt(pkt, pos) == ' ') {
		pos++;
	}

	beg = pos ;
	end = 0 ;



	do 
	{ 
		/*
		 *	Find a delimeter after ending of data field and exit the loop. 
		 */
		if ((ch = GetAt(pkt, pos)) < 0) { 
			return -1 ;
		}

		if (ch == PC_ECP || ch == PC_BD || ch == PC_RD || ch == PC_FD || ch == PC_SD)
		{ 
			end = pos - 1 ;
			break ;
		}
		else 
		{ 
			pos++ ;
		}
	
	} while (1) ;

	
	/*
	 *	Copy the field's data except delimeters. 
	 */
	GetField(pkt, pField, beg, end) ;

	
	/*
	 *	Move the current position to a delimiter before a next field. 
	 */
	pkt->m_nFldPos			= pos ;
	return 1 ;
}

static void ReplaceChar(char *pField, char cBefore, char cAfter)
{
	int			i = 0 ;

	if ((pField[0] == 'C' && pField[1] == 'F') ||
		(pField[0] == 'H' && pField[1] == 'M')) {
	}
	else { 
		return ;
	}

	while (1) {
		if (pField[i] == cBefore) { 
			pField[i]	= cAfter ;
		}
		else if (pField[i] == 0) { 
			break ;
		}
		i++ ;
	}
	return ;
}

int FetchVA(struct cdr_packet *pkt, VA_DATA *va)
{
	if (FetchField(pkt, va->ou) < 0) 
		return -1 ;
	if (FetchField(pkt, va->r_va) < 0) 
		return -1 ;
	if (FetchField(pkt, va->l_va) < 0) 
		return -1 ;

	if (pkt->m_nProtocolVer >= CDR_PRTC_V120) {
		ReplaceChar(va->ou, '/', '@') ;
		ReplaceChar(va->r_va, '/', '@') ;
		ReplaceChar(va->l_va, '/', '@') ;
	}
	return 1 ;
}

int FetchMain(struct cdr_packet *pkt, M_DATA *main)
{
	if (FetchField(pkt, main->sph) < 0) 
		return -1 ;
	if (FetchField(pkt, main->cyl) < 0) 
		return -1 ;
	if (FetchField(pkt, main->axs) < 0) 
		return -1 ;
	if (FetchField(pkt, main->add) < 0)
		return -1 ;
	if (FetchField(pkt, main->p_x) < 0) 
		return -1 ;
	if (FetchField(pkt, main->p_y) < 0)
		return -1 ;	

	return 1 ;
}

int FetchNMain(struct cdr_packet *pkt, M_N_DATA *nmain)
{
	if (FetchField(pkt, nmain->sph) < 0) 
		return -1 ;
	if (FetchField(pkt, nmain->cyl) < 0) 
		return -1 ;
	if (FetchField(pkt, nmain->axs) < 0) 
		return -1 ;
	if (FetchField(pkt, nmain->p_x) < 0) 
		return -1 ;
	if (FetchField(pkt, nmain->p_y) < 0)
		return -1 ;	

	return 1 ;
}

int FetchRC(struct cdr_packet *pkt, RC_DATA *rc)
{
	if (FetchField(pkt, rc->blur) < 0) 
		return -1 ;
	if (FetchField(pkt, rc->_break) < 0) 
		return -1 ;
	if (FetchField(pkt, rc->recv) < 0) 
		return -1 ;

	return 1 ;
}

int FetchPrism(struct cdr_packet *pkt, P_DATA *prism)
{
	if (FetchField(pkt, prism->p_x) < 0) 
		return -1 ;
	if (FetchField(pkt, prism->p_y) < 0) 
		return -1 ;

	return 1 ;
}

int FetchBVF(struct cdr_packet *pkt, BVF_DATA *bvf, BOOL bFarPoint)
{
	if (FetchField(pkt, bvf->fusion) < 0) 
		return -1 ;
	
	/*
	 *	In protocol ver 1.1, the followings of near point are skipped.
	 */
	if (pkt->m_nProtocolVer >= CDR_PRTC_V110 && bFarPoint == FALSE) { 
		return 1 ;
	}

	if (FetchField(pkt, bvf->stereo) < 0) 
		return -1 ;
	if (FetchField(pkt, bvf->min_stereo) < 0)
		return -1 ;
	if (FetchField(pkt, bvf->horiz) < 0)		// horz. aniseikonia.
		return -1 ;

	if (pkt->m_nProtocolVer >= CDR_PRTC_V110)  
		return 1 ;
	
	if (FetchField(pkt, bvf->vert) < 0)		// vert. aniseikonia. 
		return -1 ;
	
	return 1 ;
}

int FetchKR(struct cdr_packet *pkt, KR_DATA *kr)
{
	if (FetchField(pkt, kr->mm) < 0) 
		return -1 ;
	if (FetchField(pkt, kr->d) < 0) 
		return -1 ;
	if (FetchField(pkt, kr->axs) < 0)
		return -1 ;

	return 1 ;
}

int FetchZHead(struct cdr_packet *pkt, ZERNIKE_DATA *zdata)
{
	if (FetchField(pkt, zdata->u_rat) < 0) 
		return -1 ;	
	if (FetchField(pkt, zdata->h_max) < 0) 
		return -1 ;
	if (FetchField(pkt, zdata->h_min) < 0) 
		return -1 ;
	if (FetchField(pkt, zdata->l_max) < 0) 
		return -1 ;
	if (FetchField(pkt, zdata->l_max) < 0) 
		return -1 ;

	return 1 ;
}

int FetchZCoeff(struct cdr_packet *pkt, COEFF_DATA *coeff)
{
	if (FetchField(pkt, coeff->sph) < 0) 
		return -1 ;	
	if (FetchField(pkt, coeff->cyl) < 0) 
		return -1 ;
	if (FetchField(pkt, coeff->axs) < 0) 
		return -1 ;
	if (FetchField(pkt, coeff->c_01) < 0) 
		return -1 ;	
	if (FetchField(pkt, coeff->c_02) < 0) 
		return -1 ;
	if (FetchField(pkt, coeff->c_03) < 0) 
		return -1 ;
	if (FetchField(pkt, coeff->c_04) < 0) 
		return -1 ;
	if (FetchField(pkt, coeff->c_05) < 0) 
		return -1 ;
	if (FetchField(pkt, coeff->c_06) < 0) 
		return -1 ;
	if (FetchField(pkt, coeff->c_07) < 0) 
		return -1 ;
	if (FetchField(pkt, coeff->c_08) < 0) 
		return -1 ;
	if (FetchField(pkt, coeff->c_09) < 0) 
		return -1 ;

	return 1 ;	
}
