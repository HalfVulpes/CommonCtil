/**
* @file cmtCore.h
* @brief 用于支持跨平台的核心库\n
* 仅提供基本功能，如需调用完整功能，请自行调用系统api
* @par Windows 依赖库：Kernel32.lib 
* @par Linux 依赖库：libpthread.a
* @date 2021-09-14
* @author Dexnab
*/

#pragma once
#ifndef _INC_CMTCORE
#define _INC_CMTCORE
#include <cmtType.h>
#if defined(CMT_ENV_WINDOWS)
#include <Windows.h>
#elif defined(CMT_ENV_LINUX)
#include <pthread.h>
#endif

/*--------------------------------结构体定义 开始--------------------------------*/

/**
* @struct cmtThreadInfo
* @brief 线程额外信息
* @date 2021-08-12
* @author Dexnab
*/
typedef struct _CMTTHREADINFO
{
	cmtBool CreateSuspend;//<创建后挂起线程
	cmtUint64 StackSize;//<初始栈大小
	cmtUint32 priority;//<优先级
	cmtBool inherit;//<是否继承句柄
}cmtThreadInfo;

/**
* @struct cmtProcessInfo
* @brief 进程额外信息
* @date 2021-08-11
* @author Dexnab
*/
typedef struct _CMTPROCESSINFO
{
	cmtUint32 priority;//<优先级
	cmtBool hide;//<是否隐藏窗口并禁用输出
	cmtBool inherit;//<是否继承句柄
}cmtProcessInfo;

/**
* @struct cmtLock
* @brief 自动锁结构体
* @date 2021-09-13
* @author Dexnab
*/
typedef struct _CMTLOCK
{
	cmtUint64 MaxSpin;//<最大自旋数
	cmtBool state;//<状态（0：自旋模式；1：信号量模式）
	cmtUint8 value;//<自旋模式下锁变量
	cmtUint64 handle;//<信号量模式下句柄
}cmtLock;
/*--------------------------------结构体定义 结束--------------------------------*/
/*--------------------------------线程/进程操作函数 开始--------------------------------*/

extern cmtUint64 cmtCreateProcess(cmtWchar* cmd, cmtProcessInfo* info);
extern cmtUint8 cmtTerminateProcess(cmtWchar* cmd);

/**
* @brief 创建线程
* @param[in] proc 线程函数\n
* 函数定义需要为：cmtUint32 __stdcall ThreadProc(void* param);
* @param[in] param 传入参数
* @param[in] info 线程额外信息，缺省NULL
* @return 线程句柄
* @retval 0 失败
* @retval 非零 线程句柄
* @date 2021-08-12
* @author Dexnab
*/
extern cmtUint64 cmtCreateThread(cmtUint32(__stdcall* proc)(void*), void* param, cmtThreadInfo* info);
/**
* @brief 强制终止线程
* @param[in] handle 线程句柄
* @return 错误码
* @exception 0 成功
* @exception 0xff 失败
* @date 2021-08-12
* @author Dexnab
*/
extern cmtUint8 cmtTerminateThread(cmtUint64 handle);
/**
* @brief 挂起线程
* @param[in] handle 线程句柄
* @return 错误码
* @exception 0 成功
* @exception 0xff 失败
* @date 2021-08-12
* @author Dexnab
*/
extern cmtUint8 cmtSuspendThread(cmtUint64 handle);
/**
* @brief 启动线程
* @param[in] handle 线程句柄
* @return 错误码
* @exception 0 成功
* @exception 0xff 失败
* @date 2021-08-12
* @author Dexnab
*/
extern cmtUint8 cmtResumeThread(cmtUint64 handle);
/**
* @brief 等待线程结束
* @param[in] handle 线程句柄
* @param[in] time 最长等待时间，-1为无限长
* @return 线程返回值
* @date 2021-08-12
* @author Dexnab
*/
extern cmtUint32 cmtWaitForThread(cmtUint64 handle, cmtUint64 time);
/*--------------------------------线程/进程操作函数 结束--------------------------------*/
/*--------------------------------句柄（文件描述符）操作函数 开始--------------------------------*/

extern cmtUint8 cmtDupHandle(cmtUint64 dst, cmtUint64 src);
/**
* @brief 关闭句柄
* @param[in] handle 句柄
* @date 2021-08-12
* @author Dexnab
*/
extern void cmtCloseHandle(cmtUint64 handle);
/*--------------------------------句柄（文件描述符）操作函数 结束--------------------------------*/
/*--------------------------------原子操作函数 开始--------------------------------*/

//INC
extern void cmtAtomInc8(cmtUint8* num);
extern void cmtAtomInc16(cmtUint16* num);
extern void cmtAtomInc32(cmtUint32* num);
extern void cmtAtomInc64(cmtUint64* num);
//DEC
extern void cmtAtomDec8(cmtUint8* num);
extern void cmtAtomDec16(cmtUint16* num);
extern void cmtAtomDec32(cmtUint32* num);
extern void cmtAtomDec64(cmtUint64* num);
/*--------------------------------原子操作函数 结束--------------------------------*/
/*--------------------------------锁操作函数 开始--------------------------------*/

/**
* @brief 进入自旋锁
* @param[in] value 锁变量地址
* @param[in] MaxSpin 最大自旋数（-1为不限）
* @return 返回原因
* @retval 0 锁被释放
* @retval 1 自旋数到达上限
* @attention value为0时才允许进入自旋锁，所以第一次调用前请将value归零
* @par 示例:
* @code
* cmtUint8 lock = 0;
* cmtSpinLockEnter(&lock, -1);
* //xxxxxx
* cmtSpinLockLeave(lock);
* @endcode
* @date 2021-09-13
* @author Dexnab
*/
extern BOOL CMT_FASTCALL cmtSpinLockEnter(cmtUint8* value, cmtUint64 MaxSpin);
/**
* @brief 离开自旋锁
* @param[in] value 锁变量
* @date 2021-09-13
* @author Dexnab
*/
#define cmtSpinLockLeave(value) value = 0
/**
* @brief 初始化重锁
* @return 锁句柄
* @retval 0 失败
* @retval 非零 锁句柄
* @date 2021-08-20
* @author Dexnab
*/
extern cmtUint64 cmtSysLockInit();
/**
* @brief 进入重锁
* @param[in] 锁句柄
* @date 2021-08-20
* @author Dexnab
*/
extern void cmtSysLockEnter(cmtUint64 handle);
/**
* @brief 离开重锁
* @param[in] 锁句柄
* @date 2021-08-20
* @author Dexnab
*/
extern void cmtSysLockLeave(cmtUint64 handle);
/**
* @brief 释放重锁
* @param[in] 锁句柄
* @date 2021-08-20
* @author Dexnab
*/
extern void cmtSysLockFree(cmtUint64 handle);
/**
* @brief 初始化自动锁
* @param[in] lock 自动锁结构体
* @param[in] MaxSpin 锁升级前最大自旋数（-1为不升级，始终为自旋锁）
* @date 2021-09-13
* @author Dexnab
*/
extern void cmtLockInit(cmtLock* lock, cmtUint64 MaxSpin);
/**
* @brief 进入自动锁
* @param[in] lock 自动锁结构体
* @date 2021-08-20
* @author Dexnab
*/
extern void cmtLockEnter(cmtLock* lock);
/**
* @brief 离开自动锁
* @param[in] lock 自动锁结构体
* @date 2021-08-20
* @author Dexnab
*/
extern void cmtLockLeave(cmtLock* lock);
/**
* @brief 释放自动锁
* @param[in] lock 自动锁结构体
* @date 2021-08-20
* @author Dexnab
*/
extern void cmtLockFree(cmtLock* lock);
/*--------------------------------锁操作函数 结束--------------------------------*/
/*--------------------------------内存操作函数 开始--------------------------------*/

extern cmtUint8* cmtReserveMem(cmtUint8* addr, cmtUint64 size);

extern cmtUint8 cmtCommitMem(cmtUint8* addr, cmtUint64 size);

extern void cmtFreeMem(cmtUint8* addr);

extern void cmtReleaseMem(cmtUint8* addr);
/*--------------------------------内存操作函数 结束--------------------------------*/
#endif