/**
* @file cmtQueue.h
* @brief cmtQueue 库的头文件，实现了Queue队列的操作
* @par Windows 依赖库：
* @par Linux 依赖库：
* @data 2021-07-16
* @author GogeBlue
* @author Dexnab
*/

#pragma once
#ifndef _INC_CMTQUEUE
#define _INC_CMTQUEUE
#include "cmtType.h"
#include <stdlib.h>
#include <string.h>

typedef void (*cmtFree)(void*);
typedef int (*cmtScan)(void*, void*);

typedef struct {
	void** head;
	void** tail;
	void** queue;
	cmtU64            qlen;
	cmtU64            cmtElement;
	cmtFree        cmtFreeHandler;
} cmtQueue;

struct cmtFuncAttr {
	cmtU64            qlen;
	cmtFree        cmtFreeHandler;
};


#define CMT_QUEUE_EMPTY(q) (!((q)->cmtElement))
#define CMT_QUEUE_FULL(q) ((q)->cmtElement >= (q)->qlen)
#define CMT_QUEUE_LENGH(q) ((q)->qlen)
#define CMT_QUEUE_ELEMENT(q) ((q)->cmtElement)
extern cmtQueue* cmtQueueInit(struct cmtFuncAttr* attr);
extern void cmtQueueClean(cmtQueue* q);
extern int cmtQueueAppend(cmtQueue* q, void* data);
extern void* cmtQueueGet(cmtQueue* q);
extern void cmtQueueRemove(cmtQueue* q);
extern void* cmtQueueSearch(cmtQueue* q, cmtU64 index);
extern void cmtFreeIndex(cmtQueue* q, cmtU64 index);
extern int cmtScanAll(cmtQueue* q, cmtScan cmtScanHandler, void* udata);
#endif