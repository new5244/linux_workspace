/*
##########################################################
##	CHAROPS CDR-3100 Program : comm_pc_def.h	##
##	-------------------------------------------	##
##	- written by CK Jeong, 				##
##		Huvitz Co., Ltd.			##
##	- creation date	: 2003-01-06 (Mon)		##
##	- last modified : 2003-01-07 (Tue)		##
##########################################################
*/

#ifndef __COMM_PC_DEF_H
#define __COMM_PC_DEF_H

/*
 * Delimiters
 */

#define		PC_SCP			'#'
#define		PC_ECP			'$'

#define		PC_BD			'@'
#define		PC_RD			'|'
#define		PC_FD			'^'
#define		PC_SD			'!'


/*
 * Modifiers
 */

#define		PC_DIST_MOD_FAR		'F'
#define		PC_DIST_MOD_NEAR	'N'
#define		PC_DIST_MOD_ALL		'A'
#define		PC_DIST_MOD_NONE	'_'

#define		PC_EYE_MOD_OU		'O'	/* OU(Binocular) */
#define		PC_EYE_MOD_RIGHT	'R'	/* Right */
#define		PC_EYE_MOD_LEFT		'L'	/* Left */
#define		PC_EYE_MOD_ADDVA	'D'	/* ADD VA */
#define		PC_EYE_MOD_VA		'V'	/* VA */
#define		PC_EYE_MOD_BOTH		'B'	/* R,L */
#define		PC_EYE_MOD_ALL		'A'	/* All(O,R,L,D,V) */
#define		PC_EYE_MOD_NONE		'_'


/*
 * Descriptors
 * -> "$$$$" {DISTANCE=F|N|A}{EYE=O|R|L|A}{Description=$$}
 *           A=ALL
 */

#define		PC_PDESC_HEADING	"HD"
#define		PC_PDESC_BASIC_DATA	"BD"
#define		PC_PDESC_UA_DATA	"UA"
#define		PC_PDESC_CLM_DATA	"LM"
#define		PC_PDESC_MRK_DATA	"RM"
#define		PC_PDESC_SUBJ_DATA	"SU"
#define		PC_PDESC_FIN_DATA	"FI"
#define		PC_PDESC_RA_DATA	"RA"	/* NP & RA Test */
#define		PC_PDESC_RC_DATA	"RC"	/* RC Test : NRC, PRC */
#define		PC_PDESC_PRISM1_DATA	"P1"
#define		PC_PDESC_PRISM2_DATA	"P2"
#define		PC_PDESC_PRISM3_DATA	"P3"
#define		PC_PDESC_BVF_DATA	"VF"
#define		PC_PDESC_KERATO_DATA	"KR"
#define		PC_PDESC_TERMINATOR	"TR"


#endif /* __COMM_PC_DEF_H */
