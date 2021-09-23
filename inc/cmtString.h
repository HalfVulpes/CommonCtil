/**
* @file cmtString.h
* @brief 字符串处理库
* @par Windows 依赖库：
* @par Linux 依赖库：
* @date 2021-09-23
* @author Dexnab
*/

#pragma once
#ifndef _INC_CMTSTRING
#define _INC_CMTSTRING
#define _CRT_SECURE_NO_WARNINGS
#include <cmtType.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

/*--------------------------------结构体定义 开始--------------------------------*/

/**
* @struct cmtANSIstr
* @brief ANSI字符串
* @date 2021-9-23
* @author dexnab
*/
typedef struct _CMTANSISTR
{
	cmtChar* data;//<字符数组
	cmtUint64 size;//<总字节数
	cmtChar* locale;//locale字符串（注意以\\0结尾）
}cmtANSIstr;

/**
* @struct cmtU8str
* @brief UTF-8字符串
* @date 2021-9-23
* @author dexnab
*/
typedef struct _CMTU8STR
{
	cmtChar* data;//<字符数组
	cmtUint64 size;//<总字节数
}cmtU8str;

/**
* @struct cmtU16str
* @brief UTF-16字符串
* @date 2021-9-23
* @author dexnab
*/
typedef struct _CMTU16STR
{
	cmtWchar* data;//<字符数组
	cmtUint64 size;//<总字节数
}cmtU16str;

/**
* @struct cmtU32str
* @brief UTF-32字符串
* @date 2021-9-23
* @author dexnab
*/
typedef struct _CMTU32STR
{
	cmtFchar* data;//<字符数组
	cmtUint64 size;//<总字节数
}cmtU32str;
/*--------------------------------结构体定义 结束--------------------------------*/

/*--------------------------------宏定义 开始--------------------------------*/

#define CMT_LOCALE_MAX 64
#define CMT_UNICODE_RSV_START 0xd800
#define CMT_UNICODE_RSV_END 0xdfff

#define CMT_CONSTSTR(str) {str,sizeof(str)}
/*--------------------------------宏定义 结束--------------------------------*/

/*--------------------------------字符编码处理函数 开始--------------------------------*/

/**
* @brief 测量ANSI字符字节数
* @param[in] ch 字符首地址
* @param[in] locale locale字符串（注意以\\0结尾）
* @return 字符字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint8 cmtANSIchSize(cmtChar* ch, cmtChar* locale);

extern cmtUint64 cmtANSIstrSize(cmtChar* str);

extern cmtUint64 cmtANSIlen(cmtANSIstr* str);

extern cmtUint64 cmtANSItoU8size(cmtANSIstr* ansi);

extern void cmtANSItoU8(cmtANSIstr* ansi, cmtU8str* u8);

extern cmtUint64 cmtANSItoU16size(cmtANSIstr* ansi);

extern void cmtANSItoU16(cmtANSIstr* ansi, cmtU16str* u16);

extern cmtUint64 cmtANSItoU32size(cmtANSIstr* ansi);

extern void cmtANSItoU32(cmtANSIstr* ansi, cmtU32str* u32);

extern cmtUint8 cmtU8chSize(cmtChar* ch);

extern cmtUint64 cmtU8strSize(cmtChar* str);

extern cmtUint64 cmtU8len(cmtU8str* str);

extern cmtUint64 cmtU8toANSIsize(cmtU8str* u8, cmtChar* locale, cmtBool* err);

extern cmtBool cmtU8toANSI(cmtU8str* u8, cmtANSIstr* ansi);

extern cmtUint64 cmtU8toU16size(cmtU8str* u8);

extern void cmtU8toU16(cmtU8str* u8, cmtU16str* u16);

extern cmtUint64 cmtU8toU32size(cmtU8str* u8);

extern void cmtU8toU32(cmtU8str* u8, cmtU32str* u32);

extern cmtUint8 cmtU16chSize(cmtWchar* ch);

extern cmtUint64 cmtU16strSize(cmtWchar* str);

extern cmtUint64 cmtU16len(cmtU16str* str);

extern cmtUint64 cmtU16toANSIsize(cmtU16str* u16, cmtChar* locale, cmtBool* err);

extern cmtBool cmtU16toANSI(cmtU16str* u16, cmtANSIstr* ansi);

extern cmtUint64 cmtU16toU8size(cmtU16str* u16);

extern void cmtU16toU8(cmtU16str* u16, cmtU8str* u8);

extern cmtUint64 cmtU16toU32size(cmtU16str* u16);

extern void cmtU16toU32(cmtU16str* u16, cmtU32str* u32);

extern cmtUint64 cmtU32strSize(cmtFchar* str);

extern cmtUint64 cmtU32toANSIsize(cmtU32str* u32, cmtChar* locale, cmtBool* err);

extern cmtBool cmtU32toANSI(cmtU32str* u32, cmtANSIstr* ansi);

extern cmtUint64 cmtU32toU8size(cmtU32str* u32);

extern void cmtU32toU8(cmtU32str* u32, cmtU8str* u8);

extern cmtUint64 cmtU32toU16size(cmtU32str* u32);

extern void cmtU32toU16(cmtU32str* u32, cmtU16str* u16);
/*--------------------------------字符编码处理函数 结束--------------------------------*/

/*--------------------------------字符串操作函数 开始--------------------------------*/

extern void cmtSprintf(cmtU8str* out, cmtU8str* format, ...);

extern cmtBool cmtStrCmp(cmtU8str* str1, cmtU8str* str2);

extern cmtUint64 cmtStrFind(cmtU8str* src, cmtU8str* tar);

extern cmtUint64 cmtRegexMatch(cmtU8str* src, cmtU8str* regex);
/*--------------------------------字符串操作函数 结束--------------------------------*/

/*--------------------------------json函数 开始--------------------------------*/

extern cmtBool cmtJsonGetBool(cmtU8str* json, cmtU8str* name, cmtBool* ret);

extern cmtBool cmtJsonGetInt(cmtU8str* json, cmtU8str* name, cmtInt64* ret);

extern cmtBool cmtJsonGetDouble(cmtU8str* json, cmtU8str* name, double* ret);

extern cmtBool cmtJsonGetStrSize(cmtU8str* json, cmtU8str* name, cmtUint64* ret);

extern cmtBool cmtJsonGetStr(cmtU8str* json, cmtU8str* name, cmtU8str* ret);

extern cmtBool cmtJsonGetJsonSize(cmtU8str* json, cmtU8str* name, cmtUint64* ret);

extern cmtBool cmtJsonGetJson(cmtU8str* json, cmtU8str* name, cmtU8str* ret);

extern cmtBool cmtJsonGetArrSize(cmtU8str* json, cmtU8str* name, cmtUint64* ret);

extern cmtBool cmtJsonGetArr(cmtU8str* json, cmtU8str* name, cmtU8str* ret);

extern cmtBool cmtJsonArrGetBool(cmtU8str* arr, cmtU8str* name, cmtBool* ret);

extern cmtBool cmtJsonArrGetInt(cmtU8str* arr, cmtU8str* name, cmtInt64* ret);

extern cmtBool cmtJsonArrGetDouble(cmtU8str* arr, cmtU8str* name, double* ret);

extern cmtBool cmtJsonArrGetStrSize(cmtU8str* arr, cmtU8str* name, cmtUint64* ret);

extern cmtBool cmtJsonArrGetStr(cmtU8str* arr, cmtU8str* name, cmtU8str* ret);

extern cmtBool cmtJsonArrGetJsonSize(cmtU8str* arr, cmtU8str* name, cmtUint64* ret);

extern cmtBool cmtJsonArrGetJson(cmtU8str* arr, cmtU8str* name, cmtU8str* ret);

extern cmtBool cmtJsonArrGetArrSize(cmtU8str* arr, cmtU8str* name, cmtUint64* ret);

extern cmtBool cmtJsonArrGetArr(cmtU8str* arr, cmtU8str* name, cmtU8str* ret);
/*--------------------------------json函数 结束--------------------------------*/
#endif