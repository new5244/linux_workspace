#include <stdio.h>
#include <sys/inotify.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "inc/main.h"
#include "inc/inotify.h"
#include "inc/buf_list.h"
#include "inc/auto_send.h"

/* size of the event structure, not counting name */
#define EVENT_SIZE  (sizeof (struct inotify_event))

/* reasonable guess as to size of 1024 events */
#define BUF_LEN        (1024 * (EVENT_SIZE + 16))

/// 디렉토리 감시 초기화
/// \param path 감시할 디렉토리 
/// \return file_descriptor
int init_dir_monitor(char *path)
{
	int fd;
	int wd;

	fd = inotify_init();
	if ( fd < 0 ) {
		ERROR_LOG("inotify_init\n");
		return -ERR_IO;
	}

	wd = inotify_add_watch(fd, path, IN_CLOSE_WRITE);
	if ( wd < 0 ) {
		ERROR_LOG("inotify_add_watch\n\n");
		close(fd);
		return -ERR_INVAL;
	}

	return fd;
}

/// 감시 디렉토리에 생성된 파일을 얻어 transfer list 에 추가 
/// \param fd
/// \param plist transfer list
void handle_dir_monitor(int fd, BUF_LIST *plist)
{
	char buf[BUF_LEN];
	int len, i = 0;
	struct inotify_event *event;
	int type;

	len = read (fd, buf, BUF_LEN);
	if ( len > 0 ) {
		while (i < len) {
			event = (struct inotify_event *) &buf[i];

			DBG("wd=%d mask=%u cookie=%u len=%u\n",
				event->wd, event->mask,
				event->cookie, event->len);

			if (event->len) {
				DBG("name=%s\n", event->name);

				if ( ( type = check_file_ext(event->name)) >= 0 ) {
					if ( !(global_conf.flag & CONF_FLAG_MANUAL_TRANSFER) ) {
						add_buf_list(plist, event->name);
					}
					SMSG("SLF %s %s\n", event->name, type == 0 ? "CLM" : "MRK" );
				}
			}

			i += EVENT_SIZE + event->len;
		}
		i = 0;
	} else if (len < 0) {
		if (errno == EINTR) {
			/* need to reissue system call */
			ERROR_LOG("EINTR\n");
		} else {
			ERROR_LOG("read\n");
		}
	}
}

#if 0
/* size of the event structure, not counting name */
#define EVENT_SIZE  (sizeof (struct inotify_event))

/* reasonable guess as to size of 1024 events */
#define BUF_LEN        (1024 * (EVENT_SIZE + 16))


int main(int argc, char *argv[])
{
	int fd;
	int wd;
	char buf[BUF_LEN];
	int len, i = 0;
	int ret;

	fd = inotify_init ();
	if (fd < 0) { 
		perror ("inotify_init");
		exit(1);
	}

	wd = inotify_add_watch (fd, "/home/saibi/linux_work/cdrmate4linux/cdrmate_console/src", IN_CREATE | IN_MODIFY | IN_DELETE );
	if (wd < 0) {
		perror ("inotify_add_watch");
		exit(2);
	}

	len = read (fd, buf, BUF_LEN);
	if (len < 0) {
		if (errno == EINTR) {
			/* need to reissue system call */
			perror("EINTR");
		} else {
			perror ("read");
		}
	} else if (!len) {
		/* BUF_LEN too small? */
		perror("length");
	}

	while (i < len) {
		struct inotify_event *event;

		event = (struct inotify_event *) &buf[i];

		printf ("wd=%d mask=%u cookie=%u len=%u\n",
			event->wd, event->mask,
			event->cookie, event->len);

		if (event->len)
			printf ("name=%s\n", event->name);

		i += EVENT_SIZE + event->len;
	}

	ret = inotify_rm_watch (fd, wd);
	if (ret)
		perror ("inotify_rm_watch");

	ret = close (fd);
	if (ret)
		perror ("close");

	return 0;
}
#endif 
