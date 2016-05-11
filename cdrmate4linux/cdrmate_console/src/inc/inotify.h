#ifndef _INOTIFY_H_
#define _INOTIFY_H_

#include "buf_list.h"

int init_dir_monitor(char *path);
void handle_dir_monitor(int fd, BUF_LIST *plist);

#endif
