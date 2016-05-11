#ifndef _RAW_PACKET_H_
#define _RAW_PACKET_H_

#include "buf_list.h"

int handle_serial_data(int fd, BUF_LIST *plist);


// old code
#if 0
void init_raw_buffer(void);
int copy_serial_data_to_raw_buffer(int fd);
int get_raw_packet(char **pbuf, int *psize);
#endif 

#endif
