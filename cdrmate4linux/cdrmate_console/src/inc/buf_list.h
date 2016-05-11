#ifndef _BUF_LIST_H_
#define _BUF_LIST_H_

#include <pthread.h>

#include "list.h"

typedef struct buf_list_rec {
	pthread_mutex_t mutex;
	struct list_head head;
	int magic;
} BUF_LIST;

typedef struct buf_list_entry_rec {
	struct list_head list;
	char buf[0];
} BUF_LIST_ENTRY;

void init_buf_list(BUF_LIST *p);
void free_buf_list(BUF_LIST *p);
int add_buf_list(BUF_LIST *p, char *buf);
int get_buf_list(BUF_LIST *p, char *buf, int buf_size);

#endif
