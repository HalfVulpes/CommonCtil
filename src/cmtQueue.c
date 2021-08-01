/**
* @file cmtQueue.c
* @data 2021-07-16
* @author GogeBlue
* @author Dexnab
*/

#include <cmtQueue.h>

cmtQueue* cmtQueueInit(cmtUint64 size)
{
	cmtQueue* queue;

	//结构体
	queue = malloc(sizeof(cmtQueue));
	if (!queue) return 0;

	//数据空间
	queue->base = malloc(size);
	if (!queue->base)
	{
		free(queue);
		return 0;
	}

	//初始化值
	queue->size = size;
	queue->bgn = 0;
	queue->end = 0;

	return queue;
}