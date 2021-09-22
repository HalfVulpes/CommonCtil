/**
* @file cmtQueue.h
* @brief cmtQueue 库的头文件，实现了Queue队列的操作
* @par Windows 依赖库：
* @par Linux 依赖库：
* @date 2021-09-14
* @author GogeBlue
* @author Dexnab
*/

#pragma once
#ifndef _INC_CMTQUEUE
#define _INC_CMTQUEUE
#include <cmtType.h>
#include <stdlib.h>//malloc
#include <string.h>//memset

/**
* @struct cmtQueue
* @brief 队列结构体
* @date 2021-07-30
* @author GogeBlue
* @author Dexnab
*/
typedef struct _CMTQUEUE
{
	cmtUint8* base;//<基址
	cmtUint64 size;//<数据最大大小
	cmtUint64 bgn;//<数据环首字节偏移
	cmtUint64 end;//<数据环截止字节偏移（不包含最后一个字节）
	cmtBool empty;//<是否为空（TRUE：空）
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
/**
* @brief 清空队列
* @param[in] queue 队列指针
* @attention 此函数只清空队列，不释放
* @date 2021-07-30
* @author GogeBlue
* @author Dexnab
*/
#define cmtQueueClean(queue) {queue->bgn = queue->end = 0;}
/**
* @brief 释放队列
* @param[in] queue 队列指针
* @attention queue会被置零
* @date 2021-07-30
* @author GogeBlue
* @author Dexnab
*/
#define cmtQueueFree(queue) {free(queue->base);free(queue);}
/**
* @brief 队列插入
* @param[in] queue 队列指针
* @param[in] data 数据
* @param[in] size 数据大小
* @return 错误码
* @retval 0x00 正常
* @retval 0xff 数据过长
* @date 2021-07-30
* @author GogeBlue
* @author Dexnab
*/
extern cmtUint8 cmtQueueIn(cmtQueue* queue, cmtUint8* data, cmtUint64 size);
/**
* @brief 队列插入
* @param[in] queue 队列指针
* @param[in] data 数据
* @param[in] size 数据大小
* @return 错误码
* @retval 0x00 正常
* @retval 0xff 数据过长
* @date 2021-07-30
* @author GogeBlue
* @author Dexnab
*/
extern cmtUint8 cmtQueueOut(cmtQueue* queue, cmtUint8* buffer, cmtUint64 size);
//extern void cmtQueueRemove(cmtQueue* q);
//extern void* cmtQueueSearch(cmtQueue* q, cmtU64 index);
//extern void cmtFreeIndex(cmtQueue* q, cmtU64 index);
//extern int cmtScanAll(cmtQueue* q, cmtScan cmtScanHandler, void* udata);
#endif