#include <cmtType.h>
#include <CommonCtil.h>
#include <cmtString.h>

#pragma once
#ifndef _INC_CMTBIGN
#define _INC_CMTBIGN

typedef struct _CMTBIGNUM
{
	cmtUint8* data;
	cmtUint64 size;
}cmtBigNum;

//配置器
extern void cmtBignConfInit(cmtConf* conf);

//操作
extern void (*cmtStr2Bign)(cmtU8str* in, cmtBigNum* out);
extern void cmtStr2BignSoft64(cmtU8str* in, cmtBigNum* out);

//算术运算
cmtUint64 cmtBignNeg(cmtBigNum* op1);
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