#ifndef _MAIN_H_
#define _MAIN_H_

#include <syslog.h>
#include "buf_list.h"
#include "conf.h"


#ifndef NDEBUG

#define DBG(fmt, args...) fprintf(stderr, "DBG %s:%d:%s() " fmt, __FILE__,__LINE__, __func__, ## args )
//#define DBG(fmt, args...) syslog(LOG_ERR, "DBG %s:%d:%s() " fmt, __FILE__,__LINE__, __func__, ## args )
#define DCODE(x) x

#else

#define DBG(fmt, args...)
#define DCODE(x) 

#endif

#define PRINT(fmt, args...) do { fprintf(stdout, fmt, ##args); if ( global_conf.flag & CONF_FLAG_SYSLOG) { syslog(LOG_ERR, fmt, ##args);} } while(0)

#define PRINT_ERROR(err_code, fmt, args...) do { fprintf(stdout, "ERROR(%d):"fmt, err_code, ##args); if ( global_conf.flag & CONF_FLAG_SYSLOG) { syslog(LOG_ERR, "ERROR(%d):"fmt, err_code, ##args);} } while(0)

#define ERROR_LOG(fmt, args...) do { if ( global_conf.flag & CONF_FLAG_ERROR_LOG ) { fprintf(stdout, "%s():"fmt, __func__, ##args); if ( global_conf.flag & CONF_FLAG_SYSLOG) { syslog(LOG_ERR, fmt, ##args);} } } while(0)


#define SMSG(fmt, args...) do { if ( is_slave_mode() ) { fprintf(stderr, fmt, ##args); fflush(stdout); } } while (0)


enum error_list {

	ERR_PID = 1001,
	ERR_CONF,
	ERR_PORT,
	ERR_MONITOR,
	ERR_CLM_IMPORT,
	ERR_MRK_IMPORT,
	ERR_SEND_RAW_PACKET,
	ERR_INVALID_EXT,
	ERR_FILE_MOVE,
	ERR_ADD_PACKET,
	ERR_SIGNAL,




	ERR_IO = 1100,
	ERR_NOENT,
	ERR_NOT_FOUND,
	ERR_INVAL,
	ERR_STOP,
	ERR_NOMEM,
	ERR_DUP,
	ERR_BUSY,
	ERR_CONT,
	ERR_READ,
	ERR_WRITE,
	ERR_CREATE,
	ERR_DEL,






	ERR_END
	
};

#define MAX( a, b ) ( (a) > (b) ? (a) : (b))


struct serial_receiver_param {
	int fd_serial;
	int fd_monitor;

	BUF_LIST *transfer_list;
	BUF_LIST *raw_packet_list;
};

int is_slave_mode(void);

#endif
