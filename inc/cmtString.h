/**
* @file cmtString.h
* @brief 字符串处理库
* @par Windows 依赖库：
* @par Linux 依赖库：
* @note 本库中所有的UTF-16和UTF-32均为小端模式存储
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
#include <math.h>

/*--------------------------------结构体定义 开始--------------------------------*/

/**
* @struct cmtANSIstr
* @brief ANSI字符串结构体
* @date 2021-9-23
* @author dexnab
*/
typedef struct _CMTANSISTR
{
	cmtChar* data;//<字符数组
	cmtUint64 size;//<总字节数
	cmtChar* locale;//使用的代码页（locale字符串注意以\\0结尾）
}cmtANSIstr;

/**
* @struct cmtU8str
* @brief UTF-8字符串结构体
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
* @brief UTF-16字符串结构体
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
* @brief UTF-32字符串结构体
* @date 2021-9-23
* @author dexnab
*/
typedef struct _CMTU32STR
{
	cmtFchar* data;//<字符数组
	cmtUint64 size;//<总字节数
}cmtU32str;

typedef union _CMTCOMVAL
{
	cmtUint64 u64;
	cmtInt64 i64;
	double f64;
	float f32;
}cmtComVal;

typedef struct _CMTFMTPADINFO
{
	cmtBool align;
	cmtBool content;
	cmtUint64 length;
}cmtFmtPadInfo;

typedef struct _CMTFMTPRECINFO
{
	cmtBool enabled;
	cmtBool flag;
	cmtUint64 value;
}cmtFmtPrecInfo;

typedef struct _CMTFMTITRINFO
{
	cmtBool enabled;
	cmtUint64 length;
	cmtUint64 GroupSize;
	cmtUint64 RowSize;
}cmtFmtItrInfo;

typedef struct _CMTFMTINFO
{
	cmtBool sign;
	cmtFmtPadInfo padding;
	cmtFmtPrecInfo precision;
	cmtFmtItrInfo iteration;
	cmtUint8 size;
	cmtChar type;
}cmtFmtInfo;
/*--------------------------------结构体定义 结束--------------------------------*/

/*--------------------------------宏定义 开始--------------------------------*/

#define CMT_LOCALE_MAX 64
#define CMT_UNICODE_RSV_START 0xd800
#define CMT_UNICODE_RSV_END 0xdfff

#define CMT_FMT_SIZE_DEFAULT 0
#define CMT_FMT_SIZE_HH 1
#define CMT_FMT_SIZE_H 2
#define CMT_FMT_SIZE_L 3
#define CMT_FMT_SIZE_LL 4

//10^22
#define CMT_DOUBLE_MAX_10E 10000000000000000000000.0
//10^9
#define CMT_FLOAT_MAX_10E 1000000000.0

#define CMT_CONSTSTR(str) {str,sizeof(str)}
/*--------------------------------宏定义 结束--------------------------------*/

/*--------------------------------字符编码处理函数 开始--------------------------------*/

/**
* @brief 测量ANSI字符字节数
* @param[in] ch 字符首地址
* @param[in] locale 使用的代码页（locale字符串注意以\\0结尾）
* @return 字符字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint8 cmtANSIchSize(cmtChar* ch, cmtChar* locale);

/**
* @brief 测量ANSI字符串总字节数
* @param[in] str 字符串首地址
* @return 字符串总字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtANSIstrSize(cmtChar* str);

/**
* @brief 测量ANSI字符串字符数
* @param[in] str 字符串结构体
* @return 字符串字符数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtANSIlen(cmtANSIstr* str);

/**
* @brief 计算ANSI转UTF-8后，UTF-8字符串的字节数
* @param[in] ansi ANSI字符串结构体
* @return UTF-8字符串的字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtANSItoU8size(cmtANSIstr* ansi);

/**
* @brief ANSI转UTF-8
* @param[in] ansi ANSI字符串结构体
* @param[out] u8 UTF-8字符串结构体
* @date 2021-09-23
* @author dexnab
*/
extern void cmtANSItoU8(cmtANSIstr* ansi, cmtU8str* u8);

/**
* @brief 计算ANSI转UTF-16后，UTF-16字符串的字节数
* @param[in] ansi ANSI字符串结构体
* @return UTF-16字符串的字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtANSItoU16size(cmtANSIstr* ansi);

/**
* @brief ANSI转UTF-16
* @param[in] ansi ANSI字符串结构体
* @param[out] u16 UTF-16字符串结构体
* @date 2021-09-23
* @author dexnab
*/
extern void cmtANSItoU16(cmtANSIstr* ansi, cmtU16str* u16);

/**
* @brief 计算ANSI转UTF-32后，UTF-32字符串的字节数
* @param[in] ansi ANSI字符串结构体
* @return UTF-32字符串的字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtANSItoU32size(cmtANSIstr* ansi);

/**
* @brief ANSI转UTF-32
* @param[in] ansi ANSI字符串结构体
* @param[out] u32 UTF-32字符串结构体
* @date 2021-09-23
* @author dexnab
*/
extern void cmtANSItoU32(cmtANSIstr* ansi, cmtU32str* u32);

/**
* @brief 测量UTF-8字符字节数
* @param[in] ch 字符首地址
* @return 字符字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint8 cmtU8chSize(cmtChar* ch);

/**
* @brief 测量UTF-8字符串总字节数
* @param[in] str 字符串首地址
* @return 字符串总字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU8strSize(cmtChar* str);

/**
* @brief 测量UTF-8字符串字符数
* @param[in] str 字符串结构体
* @return 字符串字符数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU8len(cmtU8str* str);

/**
* @brief 计算UTF-8转ANSI后，ANSI字符串的字节数
* @param[in] u8 UTF-8字符串结构体
* @param[in] locale 使用的代码页（locale字符串注意以\\0结尾）
* @param[out] err 是否发生错误
* @return ANSI字符串的字节数
* @exception FALSE 正常
* @exception TRUE 发生错误
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU8toANSIsize(cmtU8str* u8, cmtChar* locale, cmtBool* err);

/**
* @brief UTF-8转ANSI
* @param[in] u8 UTF-8字符串结构体
* @param[out] ansi ANSI字符串结构体
* @return 是否出错
* @retval FALSE 正常
* @retval TRUE 转换失败
* @date 2021-09-23
* @author dexnab
*/
extern cmtBool cmtU8toANSI(cmtU8str* u8, cmtANSIstr* ansi);

/**
* @brief 计算UTF-8转UTF-16后，UTF-16字符串的字节数
* @param[in] u8 UTF-8字符串结构体
* @return UTF-16字符串的字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU8toU16size(cmtU8str* u8);

/**
* @brief UTF-8转UTF-16
* @param[in] u8 UTF-8字符串结构体
* @param[out] u16 UTF-16字符串结构体
* @date 2021-09-23
* @author dexnab
*/
extern void cmtU8toU16(cmtU8str* u8, cmtU16str* u16);

/**
* @brief 计算UTF-8转UTF-32后，UTF-32字符串的字节数
* @param[in] u8 UTF-8字符串结构体
* @return UTF-32字符串的字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU8toU32size(cmtU8str* u8);

/**
* @brief UTF-8转UTF-32
* @param[in] u8 UTF-8字符串结构体
* @param[out] u32 UTF-32字符串结构体
* @date 2021-09-23
* @author dexnab
*/
extern void cmtU8toU32(cmtU8str* u8, cmtU32str* u32);

/**
* @brief 测量UTF-16字符字节数
* @param[in] ch 字符首地址
* @return 字符字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint8 cmtU16chSize(cmtWchar* ch);

/**
* @brief 测量UTF-16字符串总字节数
* @param[in] str 字符串首地址
* @return 字符串总字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU16strSize(cmtWchar* str);

/**
* @brief 测量UTF-16字符串字符数
* @param[in] str 字符串结构体
* @return 字符串字符数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU16len(cmtU16str* str);

/**
* @brief 计算UTF-16转ANSI后，ANSI字符串的字节数
* @param[in] u16 UTF-16字符串结构体
* @param[in] locale 使用的代码页（locale字符串注意以\\0结尾）
* @param[out] err 是否发生错误
* @return ANSI字符串的字节数
* @exception FALSE 正常
* @exception TRUE 发生错误
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU16toANSIsize(cmtU16str* u16, cmtChar* locale, cmtBool* err);

/**
* @brief UTF-16转ANSI
* @param[in] u16 UTF-16字符串结构体
* @param[out] ansi ANSI字符串结构体
* @return 是否出错
* @retval FALSE 正常
* @retval TRUE 转换失败
* @date 2021-09-23
* @author dexnab
*/
extern cmtBool cmtU16toANSI(cmtU16str* u16, cmtANSIstr* ansi);

/**
* @brief 计算UTF-16转UTF-8后，UTF-8字符串的字节数
* @param[in] u16 UTF-16字符串结构体
* @return UTF-8字符串的字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU16toU8size(cmtU16str* u16);

/**
* @brief UTF-16转UTF-8
* @param[in] u16 UTF-16字符串结构体
* @param[out] u8 UTF-8字符串结构体
* @date 2021-09-23
* @author dexnab
*/
extern void cmtU16toU8(cmtU16str* u16, cmtU8str* u8);

/**
* @brief 计算UTF-16转UTF-32后，UTF-32字符串的字节数
* @param[in] u16 UTF-16字符串结构体
* @return UTF-32字符串的字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU16toU32size(cmtU16str* u16);

/**
* @brief UTF-16转UTF-32
* @param[in] u16 UTF-16字符串结构体
* @param[out] u32 UTF-32字符串结构体
* @date 2021-09-23
* @author dexnab
*/
extern void cmtU16toU32(cmtU16str* u16, cmtU32str* u32);

/**
* @brief 测量UTF-32字符串总字节数
* @param[in] str 字符串首地址
* @return 字符串总字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU32strSize(cmtFchar* str);

/**
* @brief 计算UTF-32转ANSI后，ANSI字符串的字节数
* @param[in] u32 UTF-32字符串结构体
* @param[in] locale 使用的代码页（locale字符串注意以\\0结尾）
* @param[out] err 是否发生错误
* @return ANSI字符串的字节数
* @exception FALSE 正常
* @exception TRUE 发生错误
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU32toANSIsize(cmtU32str* u32, cmtChar* locale, cmtBool* err);

/**
* @brief UTF-32转ANSI
* @param[in] u32 UTF-32字符串结构体
* @param[out] ansi ANSI字符串结构体
* @return 是否出错
* @retval FALSE 正常
* @retval TRUE 转换失败
* @date 2021-09-23
* @author dexnab
*/
extern cmtBool cmtU32toANSI(cmtU32str* u32, cmtANSIstr* ansi);

/**
* @brief 计算UTF-32转UTF-8后，UTF-8字符串的字节数
* @param[in] u32 UTF-32字符串结构体
* @return UTF-8字符串的字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU32toU8size(cmtU32str* u32);

/**
* @brief UTF-32转UTF-8
* @param[in] u32 UTF-32字符串结构体
* @param[out] u8 UTF-8字符串结构体
* @date 2021-09-23
* @author dexnab
*/
extern void cmtU32toU8(cmtU32str* u32, cmtU8str* u8);

/**
* @brief 计算UTF-32转UTF-16后，UTF-16字符串的字节数
* @param[in] u32 UTF-32字符串结构体
* @return UTF-16字符串的字节数
* @date 2021-09-23
* @author dexnab
*/
extern cmtUint64 cmtU32toU16size(cmtU32str* u32);

/**
* @brief UTF-32转UTF-16
* @param[in] u32 UTF-32字符串结构体
* @param[out] u16 UTF-16字符串结构体
* @date 2021-09-23
* @author dexnab
*/
extern void cmtU32toU16(cmtU32str* u32, cmtU16str* u16);
/*--------------------------------字符编码处理函数 结束--------------------------------*/

/*--------------------------------字符串操作函数 开始--------------------------------*/

/**
* @brief 字符串转整数（二进制）
* @param[in] in 字符串
* @param[out] out 整数
* @return 转换了多少字节的字符串
* @attention 数字必须从第一个字符起始
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtStrtoBin(cmtU8str* in, cmtUint64* out);

/**
* @brief 字符串转整数（八进制）
* @param[in] in 字符串
* @param[out] out 整数
* @return 转换了多少字节的字符串
* @attention 数字必须从第一个字符起始
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtStrtoOct(cmtU8str* in, cmtUint64* out);

/**
* @brief 字符串转整数（十进制）
* @param[in] in 字符串
* @param[out] out 整数
* @return 转换了多少字节的字符串
* @attention 数字必须从第一个字符起始（前面不能有符号）
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtStrtoDec(cmtU8str* in, cmtUint64* out);

/**
* @brief 字符串转整数（十六进制）
* @param[in] in 字符串
* @param[out] out 整数
* @return 转换了多少字节的字符串
* @attention 数字必须从第一个字符起始（前面不能有前缀）
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtStrtoHex(cmtU8str* in, cmtUint64* out);

/**
* @brief 字符串转浮点数（32位）
* @param[in] in 字符串
* @param[out] out 浮点数
* @return 转换了多少字节的字符串
* @attention 数字必须从第一个字符起始（前面不能有符号）
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtStrtoF32(cmtU8str* in, float* out);

/**
* @brief 字符串转浮点数（64位）
* @param[in] in 字符串
* @param[out] out 浮点数
* @return 转换了多少字节的字符串
* @attention 数字必须从第一个字符起始（前面不能有符号）
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtStrtoF64(cmtU8str* in, double* out);

/**
* @brief 计算由整数转换而来的字符串的字节数（二进制）
* @param[in] in 整数
* @return 字节数
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtBintoStrSize(cmtUint64 in);

/**
* @brief 整数转字符串（二进制）
* @param[in] in 整数
* @param[in] out 输出字符串
* @date 2021-10-14
* @author dexnab
*/
extern void cmtBintoStr(cmtUint64 in, cmtU8str* out);

/**
* @brief 计算由整数转换而来的字符串的字节数（八进制）
* @param[in] in 整数
* @return 字节数
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtOcttoStrSize(cmtUint64 in);

/**
* @brief 整数转字符串（八进制）
* @param[in] in 整数
* @param[in] out 输出字符串
* @date 2021-10-14
* @author dexnab
*/
extern void cmtOcttoStr(cmtUint64 in, cmtU8str* out);

/**
* @brief 计算由整数转换而来的字符串的字节数（十进制）
* @param[in] in 整数
* @return 字节数
* @attention 不包含符号
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtDectoStrSize(cmtUint64 in);

/**
* @brief 整数转字符串（十进制）
* @param[in] in 整数
* @param[in] out 输出字符串
* @attention 不输出符号
* @date 2021-10-14
* @author dexnab
*/
extern void cmtDectoStr(cmtUint64 in, cmtU8str* out);

/**
* @brief 计算由整数转换而来的字符串的字节数（十六进制）
* @param[in] in 整数
* @return 字节数
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtHextoStrSize(cmtUint64 in);

/**
* @brief 整数转字符串（十六进制）
* @param[in] in 整数
* @param[in] out 输出字符串
* @date 2021-10-14
* @author dexnab
*/
extern void cmtHextoStr(cmtUint64 in, cmtU8str* out, cmtBool cap);

/**
* @brief 计算pofd（32位浮点）
* @param[in] in 数字
* @return pofd
* @note pofd的解释详见 doc/格式字符串.md
* @date 2021-10-12
* @author dexnab
*/
extern cmtInt64 cmtCalcPofdF32(float in);

/**
* @brief 计算pofd（64位浮点）
* @param[in] in 数字
* @return pofd
* @note pofd的解释详见 doc/格式字符串.md
* @date 2021-10-12
* @author dexnab
*/
extern cmtInt64 cmtCalcPofdF64(double in);

/**
* @brief 计算由浮点数转换而来的普通小数字符串的字节数
* @param[in] in 数字
* @param[in] pofd pofd
* @param[in] sigf 保留有效数字位数
* @return 字节数
* @attention 不包含符号
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtFltoStrSize(cmtInt64 pofd, cmtUint64 sigf);
/**
* @brief 浮点数转普通小数字符串（32位浮点）
* @param[in] in 数字
* @param[in] out 输出字符串
* @param[in] pofd pofd
* @param[in] sigf 保留有效数字位数
* @attention 不输出符号
* @date 2021-10-12
* @author dexnab
*/
extern void cmtF32toStr(float in, cmtU8str* out, cmtInt64 pofd, cmtUint64 sigf);

/**
* @brief 浮点数转普通小数字符串（64位浮点）
* @param[in] in 数字
* @param[in] out 输出字符串
* @param[in] pofd pofd
* @param[in] sigf 保留有效数字位数
* @attention 不输出符号
* @date 2021-10-12
* @author dexnab
*/
extern void cmtF64toStr(double in, cmtU8str* out, cmtInt64 pofd, cmtUint64 sigf);

/**
* @brief 计算有浮点数转换而来的科学计数法模式字符串的字节数
* @param[in] in 数字
* @param[in] pofd pofd
* @param[in] sigf 保留有效数字位数
* @return 字节数
* @attention 不包含符号
* @date 2021-10-14
* @author dexnab
*/
extern cmtUint64 cmtFltoStrExSize(cmtInt64 pofd, cmtUint64 sigf);

/**
* @brief 浮点数转科学计数法模式字符串（32位浮点）
* @param[in] in 数字
* @param[in] out 输出字符串
* @param[in] pofd pofd
* @param[in] sigf 保留有效数字位数
* @param[in] cap 指数部分的‘e’是否大写
* @attention 不输出符号
* @date 2021-10-14
* @author dexnab
*/
extern void cmtF32toStrEx(float in, cmtU8str* out, cmtInt64 pofd, cmtUint64 sigf, cmtBool cap);

/**
* @brief 浮点数转科学计数法模式字符串（64位浮点）
* @param[in] in 数字
* @param[in] out 输出字符串
* @param[in] pofd pofd
* @param[in] sigf 保留有效数字位数
* @param[in] cap 指数部分的‘e’是否大写
* @attention 不输出符号
* @date 2021-10-14
* @author dexnab
*/
extern void cmtF64toStrEx(double in, cmtU8str* out, cmtInt64 pofd, cmtUint64 sigf, cmtBool cap);

extern cmtUint64 cmtSprintfSize(cmtU8str* format, ...);

/*
* TODO:
* 浮点数还没有处理错误数据（NAN，0，INFINITY）
* 输出字符串还要增加控制字数模式（由precision给定）
* 遍历输出实现
*/
/**
* @brief 根据格式字符串生成字符串
* @param[out] out 生成的字符串
* @param[in] format 格式字符串
* @param[in] ... 参数列表（与格式字符串匹配）
* @attention 格式字符串和生成的字符串都是UTF-8编码的
* @attention 格式字符串具体要求见 doc/格式字符串.md
* @date 2021-09-23
* @author dexnab
*/
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