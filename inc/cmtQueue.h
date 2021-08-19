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
#include <cmtType.h>
#include <stdlib.h>

typedef struct _CMTQUEUE
{
	cmtUint8* base;
	cmtUint64 size;
	cmtUint64 bgn;
	cmtUint64 end;
}cmtQueue;

/**
* @brief 初始化一个队列
* @param[in] size 队列最大大小
* @return 队列指针
* @retval 0 失败
* @retval 非零 队列指针
* @attention 需要使用cmtQueueFree来释放
* @date 2021-07-30
* @author GogeBlue
* @author Dexnab
*/
extern cmtQueue* cmtQueueInit(cmtUint64 size);
extern void cmtQueueClean(cmtQueue* queue);
extern cmtUint8 cmtQueueAppend(cmtQueue* queue, cmtUint8* data, cmtUint64 size);
extern cmtUint8 cmtQueueGet(cmtQueue* queue, cmtUint8* buffer, cmtUint64 size);
extern void cmtQueueRemove(cmtQueue* q);
extern void* cmtQueueSearch(cmtQueue* q, cmtUint64 index);
extern void cmtFreeIndex(cmtQueue* q, cmtUint64 index);
extern int cmtScanAll(cmtQueue* q, cmtUint64 cmtScanHandler, void* udata);
#endif