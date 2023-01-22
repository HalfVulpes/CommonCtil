/**
* @file cmtBign.h
* @brief 大数运算库
* @date 2022-02-22
* @author dexnab
*/
#include <cmtType.h>
#include <CommonCtil.h>
#include <cmtString.h>

#pragma once
#ifndef _INC_CMTBIGN
#define _INC_CMTBIGN

/**
* @typedef cmtBigNum
* @brief 大数
* @see _CMTBIGNUM
* @date 2022-02-22
* @author dexnab
*/
/**
* @brief 大数
* @date 2022-02-22
* @author dexnab
*/
typedef struct _CMTBIGNUM
{
	cmtUint64* data;
	cmtUint64 size;
}cmtBigNum;

//配置器
/**
* @brief cmtBign模块配置器
* @param[in] conf 库配置
* @test cmtConf() 状态：NT
* @date 2022-02-22
* @author dexnab
*/
extern void cmtBignConfInit(cmtConf* conf);

//操作
extern void (*cmtStr2Bign)(cmtU8str* in, cmtBigNum* out);
extern void cmtStr2BignSoft64(cmtU8str* in, cmtBigNum* out);

//算术运算
/**
* @brief 取相反数
* @param[in] in 输入
* @param[out] out 输出
* @test cmtDemoBignNeg() 状态：NT
* @date 2022-02-22
* @author dexnab
*/
extern void (*cmtBignNeg)(cmtBigNum* in, cmtBigNum* out);

/**
* @brief 取相反数-64位软件实现版本
* @param[in] in 输入
* @param[out] out 输出
* @test cmtDemoBignNegSoft64() 状态：NT
* @date 2022-02-22
* @author dexnab
*/
extern void cmtBignNegSoft64(cmtBigNum* in, cmtBigNum* out);
cmtUint64 cmtBignAdd(cmtBigNum* op1, cmtBigNum* op2);
cmtUint64 cmtBignSub(cmtBigNum* op1, cmtBigNum* op2);
cmtUint64 cmtBignMul(cmtBigNum* op1, cmtBigNum* op2);
cmtUint64 cmtBignImul(cmtBigNum* op1, cmtBigNum* op2);
cmtUint64 cmtBignDiv(cmtBigNum* op1, cmtBigNum* op2);
cmtUint64 cmtBignIdiv(cmtBigNum* op1, cmtBigNum* op2);

//逻辑运算
cmtUint64 cmtBignNot(cmtBigNum* op1);
cmtUint64 cmtBignAnd(cmtBigNum* op1, cmtBigNum* op2);
cmtUint64 cmtBignOr(cmtBigNum* op1, cmtBigNum* op2);
cmtUint64 cmtBignXor(cmtBigNum* op1, cmtBigNum* op2);

//移位运算
cmtUint64 cmtBignShl(cmtBigNum* op1, cmtUint64 op2);
cmtUint64 cmtBignShr(cmtBigNum* op1, cmtUint64 op2);
cmtUint64 cmtBignSal(cmtBigNum* op1, cmtUint64 op2);
cmtUint64 cmtBignSar(cmtBigNum* op1, cmtUint64 op2);
cmtUint64 cmtBignRol(cmtBigNum* op1, cmtUint64 op2);
cmtUint64 cmtBignRor(cmtBigNum* op1, cmtUint64 op2);
cmtUint64 cmtBignRcl(cmtBigNum* op1, cmtUint64 op2);
cmtUint64 cmtBignRcr(cmtBigNum* op1, cmtUint64 op2);

//比较
cmtUint64 cmtBignCmp(cmtBigNum* op1, cmtBigNum* op2);
cmtUint64 cmtBignTest(cmtBigNum* op1, cmtBigNum* op2);
#endif