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

void cmtDemoANSI();
void cmtDemoU8();
void cmtDemoU16();
void cmtDemoU32();
void cmtDemoAnlyFmt();
void cmtDemoStrToBin();
void cmtDemoStrToOct();
void cmtDemoStrToDec();
void cmtDemoStrToHex();

/**
* @brief 字符串转32位浮点
* @see cmtStrToF32(cmtU8str*, float*)
* @date 2021-12-04
* @author dexnab
*/
void cmtDemoStrToF32();
void cmtDemoStrToF64();
void cmtDemoBinToStr();
#endif