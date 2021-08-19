/**
* @file cmtCore.c
* @date 2021-08-12
* @author Dexnab
*/
#include <cmtCore.h>

//Windows°æ±¾
#ifdef CMT_ENV_WINDOWS
cmtUint64 cmtCreateThread(cmtUint32(*proc)(void*), void* param, cmtThreadInfo* info)
{
	HANDLE hThread;

	if (!info)
		hThread = CreateThread(NULL, 0, proc, param, 0, NULL);
	else if (info->CreateSuspend)
		hThread = CreateThread(NULL, info->StackSize, proc, param, CREATE_SUSPENDED, NULL);
	else
		hThread = CreateThread(NULL, info->StackSize, proc, param, 0, NULL);

	return hThread;
}

cmtUint8 cmtTerminateThread(cmtUint64 handle)
{
	cmtBool err;

	err = TerminateThread(handle, 0);

	if (err) return 0;
	else return 0xff;
}

cmtUint8 cmtSuspendThread(cmtUint64 handle)
{
	cmtUint32 err;

	err = SuspendThread(handle);

	if (err == (cmtUint32)-1) return 0xff;
	else return 0;
}

cmtUint8 cmtResumeThread(cmtUint64 handle)
{
	cmtUint32 err;

	err = ResumeThread(handle);

	if (err == (cmtUint32)-1) return 0xff;
	else return 0;
}

cmtUint32 cmtWaitForThread(cmtUint64 handle, cmtUint64 time)
{
	cmtUint32 ret;

	WaitForSingleObject(handle, time);
	GetExitCodeThread(handle, &ret);

	return ret;
}

void cmtCloseHandle(cmtUint64 handle)
{
	CloseHandle(handle);
}

cmtUint64 cmtSysLockInit()
{
	HANDLE hSmp;

	hSmp = CreateSemaphoreW(NULL, 1, 1, NULL);

	return hSmp;
}

void cmtSysLockEnter(cmtUint64 handle)
{
	WaitForSingleObject(handle, INFINITE);
}

void cmtSysLockLeave(cmtUint64 handle)
{
	ReleaseSemaphore(handle, 1, NULL);
}

void cmtSysLockFree(cmtUint64 handle)
{
	CloseHandle(handle);
}

void cmtLockInit(cmtLock* lock, cmtUint64 MaxSpin)
{

}
#endif