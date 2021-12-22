/**
* @file cmtDemoString.h
* @brief cmtString库的测试程序
* @date 2021-12-04
* @author dexnab
*/
#pragma once
#ifndef _INC_CMTDEMOSTRING
#define _INC_CMTDEMOSTRING
#include <cmtString.h>

/**
* @brief 有关ANSI字符编码的一系列测试
* @see cmtANSIchSize(cmtChar* ch, cmtChar* locale)
* @see cmtANSIstrSize(cmtChar* str)
* @see cmtANSIlen(cmtANSIstr* str)
* @see cmtANSItoU8size(cmtANSIstr* ansi)
* @see cmtANSItoU8(cmtANSIstr* ansi, cmtU8str* u8)
* @see cmtANSItoU16size(cmtANSIstr* ansi)
* @see cmtANSItoU16(cmtANSIstr* ansi, cmtU16str* u16)
* @see cmtANSItoU32size(cmtANSIstr* ansi)
* @see cmtANSItoU32(cmtANSIstr* ansi, cmtU32str* u32)
* @date 2021-12-18
* @author dexnab
*/
extern void cmtDemoANSI();

/**
* @brief 有关UTF-8字符编码的一系列测试
* @see cmtU8chSize(cmtChar* ch)
* @see cmtU8strSize(cmtChar* str)
* @see cmtU8len(cmtU8str* str)
* @see cmtU8toANSIsize(cmtU8str* u8, cmtChar* locale, cmtBool* err)
* @see cmtU8toANSI(cmtU8str* u8, cmtANSIstr* ansi)
* @see cmtU8toU16size(cmtU8str* u8)
* @see cmtU8toU16(cmtU8str* u8, cmtU16str* u16)
* @see cmtU8toU32size(cmtU8str* u8)
* @see cmtU8toU32(cmtU8str* u8, cmtU32str* u32)
* @date 2021-12-18
* @author dexnab
*/
extern void cmtDemoU8();

/**
* @brief 有关UTF-16字符编码的一系列测试
* @see cmtU16chSize(cmtWchar* ch)
* @see cmtU16strSize(cmtWchar* str)
* @see cmtU16len(cmtU16str* str)
* @see cmtU16toANSIsize(cmtU16str* u16, cmtChar* locale, cmtBool* err)
* @see cmtU16toANSI(cmtU16str* u16, cmtANSIstr* ansi)
* @see cmtU16toU8size(cmtU16str* u16)
* @see cmtU16toU8(cmtU16str* u16, cmtU8str* u8)
* @see cmtU16toU32size(cmtU16str* u16)
* @see cmtU16toU32(cmtU16str* u16, cmtU32str* u32)
* @date 2021-12-18
* @author dexnab
*/
extern void cmtDemoU16();

/**
* @brief 有关UTF-32字符编码的一系列测试
* @see cmtU32strSize(cmtFchar* str)
* @see cmtU32toANSIsize(cmtU32str* u32, cmtChar* locale, cmtBool* err)
* @see cmtU32toANSI(cmtU32str* u32, cmtANSIstr* ansi)
* @see cmtU32toU8size(cmtU32str* u32)
* @see cmtU32toU8(cmtU32str* u32, cmtU8str* u8)
* @see cmtU32toU16size(cmtU32str* u32)
* @see cmtU32toU16(cmtU32str* u32, cmtU16str* u16)
* @date 2021-12-18
* @author dexnab
*/
extern void cmtDemoU32();

/**
* @brief 分析格式控制字符串
* @see cmtAnlyFmt(cmtU8str* fmt, cmtFmtInfo* info, cmtUint64* ArgList)
* @date 2021-12-19
* @author dexnab
*/
extern void cmtDemoAnlyFmt();

/**
* @brief 字符串转整数（二进制格式）
* @see CMT_CONSTSTR(str)
* @see cmtStrToBin(cmtU8str* in, cmtUint64* out)
* @date 2021-12-18
* @author dexnab
*/
extern void cmtDemoStrToBin();

/**
* @brief 字符串转整数（八进制格式）
* @see cmtStrToOct(cmtU8str* in, cmtUint64* out)
* @date 2021-12-18
* @author dexnab
*/
extern void cmtDemoStrToOct();

/**
* @brief 字符串转整数（十进制格式）
* @see cmtStrToDec(cmtU8str* in, cmtUint64* out)
* @date 2021-12-18
* @author dexnab
*/
extern void cmtDemoStrToDec();

/**
* @brief 字符串转整数（十六进制格式）
* @see cmtStrToHex(cmtU8str* in, cmtUint64* out)
* @date 2021-12-18
* @author dexnab
*/
extern void cmtDemoStrToHex();

/**
* @brief 字符串转32位浮点
* @see cmtStrToF32(cmtU8str*, float*)
* @date 2021-12-04
* @author dexnab
*/
extern void cmtDemoStrToF32();

/**
* @brief 字符串转32位浮点
* @see cmtStrToF64(cmtU8str*, float*)
* @date 2021-12-04
* @author dexnab
*/
extern void cmtDemoStrToF64();
extern void cmtDemoBinToStr();

/**
* @brief 整数转字符串（二进制模式）
* @see cmtSprintfBin(cmtU8str* out, cmtFmtInfo* info, cmtUint64 arg)
* @date 2021-12-22
* @author dexnab
*/
extern void cmtDemoSprintfBin();

/**
* @brief 整数转字符串（八进制模式）
* @see cmtSprintfOct(cmtU8str* out, cmtFmtInfo* info, cmtUint64 arg)
* @date 2021-12-21
* @author dexnab
*/
extern void cmtDemoSprintfOct();

/**
* @brief 整数转字符串（有符号十进制模式）
* @see cmtSprintfDec(cmtU8str* out, cmtFmtInfo* info, cmtInt64 arg)
* @date 2021-12-21
* @author dexnab
*/
extern void cmtDemoSprintfDec();

/**
* @brief 整数转字符串（无符号十进制模式）
* @see cmtSprintfUdec(cmtU8str* out, cmtFmtInfo* info, cmtUint64 arg)
* @date 2021-12-21
* @author dexnab
*/
extern void cmtDemoSprintfUdec();

/**
* @brief 整数转字符串（十六进制模式）
* @see cmtSprintfHex(cmtU8str* out, cmtFmtInfo* info, cmtUint64 arg)
* @date 2021-12-21
* @author dexnab
*/
extern void cmtDemoSprintfHex();
#endif