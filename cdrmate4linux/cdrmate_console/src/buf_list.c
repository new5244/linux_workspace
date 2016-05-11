#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "inc/buf_list.h"
#include "inc/main.h"

/// BUF_LIST 초기화
/// \param p
void init_buf_list(BUF_LIST *p)
{
	pthread_mutex_init(&p->mutex, NULL);
	INIT_LIST_HEAD(&p->head);
	p->magic = 0x20118279;
}


/// BUF_LIST 메모리 해제
/// \param p
void free_buf_list(BUF_LIST *plist)
{
	struct list_head *p;
	BUF_LIST_ENTRY *entry = NULL;

	if (plist->magic != 0x20118279) {
		DBG("invalid BUF_LIST\n");
		return;
	}

	list_for_each(p, &plist->head) {
		if ( entry ) {
			free(entry);
		}
		entry = list_entry(p, BUF_LIST_ENTRY, list); 
	}

	if ( entry ) {
		free(entry);
	}

	init_buf_list(plist);
}


/// entry 메모리 할당
/// \param buf buf 은 null-terminated string 으로 간주함
/// \return entry
static BUF_LIST_ENTRY * alloc_entry(char *buf)
{
	BUF_LIST_ENTRY *entry;
	int buf_size;

	buf_size = strlen(buf);

	entry = (BUF_LIST_ENTRY *)malloc( sizeof(BUF_LIST_ENTRY) + buf_size + 1 );

	if ( entry ) {
		memset(entry, 0, sizeof(BUF_LIST_ENTRY) + buf_size + 1 );
		strcpy(entry->buf, buf);
	}
	return entry;
}

/// BUF_LIST 에 추가 
/// \param p
/// \param buf null-terminated string
/// \return length
int add_buf_list(BUF_LIST *p, char *buf)
{
	BUF_LIST_ENTRY *entry;

	entry = alloc_entry(buf);
	if ( entry == NULL ) {
		ERROR_LOG("insufficient memory\n");
		return -ERR_NOMEM;
	}

	pthread_mutex_lock(&p->mutex);
	list_add_tail(&entry->list, &p->head);
	pthread_mutex_unlock(&p->mutex);
	return 0;
}

/// BUF_LIST 에서 가장 먼저 추가된 entry 추출
/// list 에서 삭제됨 
/// \param p
/// \param buf (out)
/// \param buf_size (in)
/// \return 0 success
int get_buf_list(BUF_LIST *plist, char *buf, int buf_size)
{
	struct list_head *p;
	BUF_LIST_ENTRY *entry = NULL;

	pthread_mutex_lock(&plist->mutex);

	list_for_each(p, &plist->head) {
		entry = list_entry(p, BUF_LIST_ENTRY, list);
		break;
	}
	if ( entry ) {
		list_del(&entry->list);
		pthread_mutex_unlock(&plist->mutex);

		strncpy(buf, entry->buf, buf_size);
		buf[buf_size-1] = 0;
		free(entry);
		return 0;
	}
	pthread_mutex_unlock(&plist->mutex);
	return -ERR_NOENT;
}
