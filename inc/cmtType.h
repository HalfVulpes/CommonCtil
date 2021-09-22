/**
* @file cmtType.h
* @brief cmtType 库的数据类型标准定义文件，这个文件规定了大多数的数据类型的别名
* @par Windows 依赖库：
* @par Linux 依赖库：
* @date 2021-09-14
* @author GogeBlue
* @author Dexnab
*/

#pragma once
#ifndef _INC_CMTTYPE
#define _INC_CMTTYPE

//操作系统环境
#if defined(__linux__)
#define CMT_ENV_LINUX
#elif defined(_WIN32)
#define CMT_ENV_WINDOWS
#elif defined(__APPLE__)
#define CMT_ENV_APPLE
#endif

//程序位数
#if defined(_WIN64)
#define CMT_ENV_x64
#elif defined(__x86_64__)
#define CMT_ENV_x64
#else
#define CMT_ENV_x86
#endif

//c/c++
#if defined(__cplusplus)
#define CMT_ENV_CPP
#endif

//调用约定
#if defined(CMT_ENV_x64)
#define CMT_CDECL
#define CMT_STDCALL
#define CMT_FASTCALL
#else
#if defined(CMT_ENV_WINDOWS)
#define CMT_CDECL __cdecl
#define CMT_STDCALL __stdcall
#define CMT_FASTCALL __fastcall
#elif defined(CMT_ENV_LINUX)
#define CMT_CDECL __attribute__((cdecl))
#define CMT_STDCALL __attribute__((stdcall))
#define CMT_FASTCALL __attribute__((fastcall))
#endif
#endif

//常用数据
#define CMT_FLOAT_TOL 0.00001
#define CMT_PI 3.141592653589793
#define TRUE 1
#define FALSE 0

//数据类型
typedef unsigned char cmtUint8;
typedef unsigned short cmtUint16;
typedef unsigned int cmtUint32;
typedef char cmtInt8;
typedef short cmtInt16;
typedef int cmtInt32;
#if defined(CMT_ENV_x64)
typedef unsigned long long cmtUint64;//注意：32位模式下该类型与cmtUint32相同
typedef long long cmtInt64;//注意：32位模式下该类型与cmtInt32相同
#else
typedef cmtUint32 cmtUint64;//注意：32位模式下该类型与cmtUint32相同
typedef cmtInt32 cmtInt64;//注意：32位模式下该类型与cmtInt32相同
#endif
typedef cmtUint8 cmtBool;

//字符类型
typedef cmtUint8 cmtChar;
typedef cmtUint16 cmtWchar;
typedef cmtUint32 cmtFchar;

#endif