#ifndef _AUTO_SEND_H_
#define _AUTO_SEND_H_

int check_file_ext(char *path);
int send_clm_mrk_file(int fd, int type, char *path);
int send_data_to_cdr(int fd, char *path);
int auto_send_data(int fd, char *name);

#endif
