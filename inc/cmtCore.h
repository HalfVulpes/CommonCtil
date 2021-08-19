/**
* @file cmtCore.h
* @brief 用于支持跨平台的核心库\n
* 仅提供基本功能，如需调用完整功能，请自行调用系统api
* @par Windows 依赖库：Kernel32.lib 
* @par Linux 依赖库：libpthread.a
* @data 2021-08-11
* @author Dexnab
*/
#include <cmtType.h>
#ifdef CMT_ENV_WINDOWS
#include <Windows.h>
#else
#include <pthread.h>
#endif

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
}cmtProcessInfo;

//进程操作
extern cmtUint64 cmtCreateProcess(cmtWchar* cmd, cmtProcessInfo* info);

//线程操作
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
extern cmtUint64 cmtCreateThread(cmtUint32(*proc)(void*), void* param, cmtThreadInfo* info);
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

//句柄操作
extern cmtUint8 cmtDupHandle(cmtUint64 dst, cmtUint64 src);
/**
* @brief 关闭句柄
* @param[in] handle 句柄
* @date 2021-08-12
* @author Dexnab
*/
extern void cmtCloseHandle(cmtUint64 handle);

//原子操作
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

//锁
/**
* @brief 尝试进入自旋锁
* @param[in] lock 锁
* @attention lock为0时才允许进入自旋锁，所以第一次调用前请将lock归零
* @date 2021-08-12
* @author Dexnab
*/
extern void __fastcall cmtSpinLockEnter(cmtUint8* lock);
/**
* @brief 退出自旋锁
* @param[in] lock 锁
* @date 2021-08-12
* @author Dexnab
*/
extern void __fastcall cmtSpinLockLeave(cmtUint8* lock);