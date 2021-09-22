/**
* @file cmtQueue.c
* @date 2021-07-16
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
	queue->empty = TRUE;

	return queue;
}

cmtUint8 cmtQueueIn(cmtQueue* queue, cmtUint8* data, cmtUint64 size)
{
	cmtUint64 rData;

	//正序
	if (queue->end > queue->bgn || queue->empty)
	{
		//剩余空间
		if (queue->end - queue->bgn + size > queue->size) return 0xff;

		//不需要反转
		if (size <= queue->size - queue->end)
			for (rData = 0; rData < size; rData++, queue->end++)
				queue->base[queue->end] = data[rData];
		//需要反转
		else
		{
			for (rData = 0; queue->end <= queue->size; rData++, queue->end++)
				queue->base[queue->end] = data[rData];
			for (queue->end = 0; rData < size; rData++, queue->end++)
				queue->base[queue->end] = data[rData];
		}
	}
	//逆序
	else
	{
		//剩余空间
		if (size > queue->bgn - queue->end) return 0xff;

		//绝对不可能反转
		for (rData = 0; rData < size; rData++, queue->end++)
			queue->base[queue->end] = data[rData];
	}

	queue->empty = FALSE;
	return 0x00;
}