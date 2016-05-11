#ifndef _CONF_H_
#define _CONF_H_

struct conf_rec {
	char port[256];
	int speed;
	char protocol_ver[8];
	int system_id;

	char received[1024];
	char transfer[1024];
	char transferred[1024];
	char manual_path[1024];

	int flag;
};


void make_conf_path(char *path);
int load_conf(void);

#define CONF_FLAG_CONSOLE 0x01
#define CONF_FLAG_SYSLOG 0x02
#define CONF_FLAG_ERROR_LOG 0x04
#define CONF_FLAG_MANUAL_TRANSFER 0x08
#define CONF_FLAG_MOVE_FILE 0x10

extern struct conf_rec global_conf;

#endif


