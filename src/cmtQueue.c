/**
* @file cmtQueue.h
* @brief cmtQueue 库的数据类型标准定义文件，这个文件规定了大多数的数据类型的别名
* @par Windows 依赖库：
* @par Linux 依赖库：
* @data 2021-07-16
* @author GogeBlue
* @author Dexnab
*/
#include "..\inc\cmtQueue.h"

cmtQueue* cmtQueueInit(struct cmtFuncAttr* attr)
{
	cmtQueue* q = (cmtQueue*)malloc(sizeof(cmtQueue));
	if (q == NULL) return NULL;
	q->qlen = attr->qlen;
	q->cmtElement = 0;
	q->cmtFreeHandler = attr->cmtFreeHandler;
	q->queue = calloc(q->qlen, sizeof(void*));
	if (q->queue == NULL) {
		free(q);
		return NULL;
	}
	q->head = q->tail = q->queue;
	return q;
}

void cmtQueueClean(cmtQueue* q)
{
	if (q == NULL) return;
	if (q->cmtFreeHandler != NULL) {
		while (q->cmtElement) {
			q->cmtFreeHandler(*(q->head));
			if (++(q->head) >= q->queue + q->qlen)
				q->head = q->queue;
			--(q->cmtElement);
		}
	}
	if (q->queue != NULL)
		free(q->queue);
	free(q);
}

int cmtQueueAppend(cmtQueue* q, void* data)
{
	if (q->cmtElement >= q->qlen) return -1;
	*(q->tail) = data;
	if (++(q->tail) == q->queue + q->qlen)
		q->tail = q->queue;
	++(q->cmtElement);
	return 0;
}

void* cmtQueueGet(cmtQueue* q)
{
	if (!q->cmtElement) return NULL;
	return *(q->head);
}

void cmtQueueRemove(cmtQueue* q)
{
	if (!q->cmtElement) return;
	if (++(q->head) >= q->queue + q->qlen)
		q->head = q->queue;
	--(q->cmtElement);
}

void* cmtQueueSearch(cmtQueue* q, cmtU64 index)
{
	if (index >= q->cmtElement) return NULL;
	void** ptr = q->head + index;
	if (ptr >= q->queue + q->qlen)
		ptr = q->queue + (ptr - (q->queue + q->qlen));
	return *ptr;
}

int cmtScanAll(cmtQueue* q, cmtScan scan_handler, void* udata)
{
	void** scan = q->head;
	cmtU64 i = 0;
	for (; i < q->cmtElement; ++i) {
		if (scan_handler != NULL) {
			if (scan_handler(*scan, udata) < 0)
				return -1;
		}
		if (++scan >= q->queue + q->qlen)
			scan = q->queue;
	}
	return 0;
}

void cmtFreeIndex(cmtQueue* q, cmtU64 index)
{
	if (index >= q->cmtElement) return;
	void** pos = q->head + index;
	if (pos >= q->queue + q->qlen)
		pos = q->queue + (pos - (q->queue + q->qlen));
	void* save = *pos;
	void** next = pos;
	cmtU64 i, end = q->cmtElement - index;
	for (i = 0; i < end; ++i) {
		if (++next >= q->queue + q->qlen)
			next = q->queue;
		*pos = *next;
		if (++pos >= q->queue + q->qlen)
			pos = q->queue;
	}
	q->tail = pos;
	--(q->cmtElement);
	if (q->cmtFreeHandler != NULL)
		q->cmtFreeHandler(save);
}