#ifndef _SERIAL_H_
#define _SERIAL_H_

int open_serial_port(char *port, int baud);
int close_serial_port(void);
char * get_connect_id_str(void);

#endif

