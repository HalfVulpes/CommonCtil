/**
* @file cmtDemoBign.h
* @brief cmtBign库的测试程序
* @date 2022-02-22
* @author dexnab
*/
#pragma once
#ifndef _INC_CMTDEMOBIGN
#define _INC_CMTDEMOBIGN
#include <cmtBign.h>

/**
* @brief 取相反数
* @see cmtBignNeg
* @date 2022-02-22
* @author dexnab
*/
extern void cmtDemoBignNeg();

/**
* @brief 取相反数-64位软件实现版本
* @see cmtANSIchSize(cmtChar* ch, cmtChar* locale)
* @date 2022-02-22
* @author dexnab
*/
extern void cmtDemoBignNegSoft64();

#endif