/**
* @file cmtType.h
* @brief cmtType 库的数据类型标准定义文件，这个文件规定了大多数的数据类型的别名
* @par Windows 依赖库：
* @par Linux 依赖库：
* @data 2021-07-16
* @author GogeBlue
* @author Dexnab
*/

#pragma once
#ifndef _INC_CMTTYPE
#define _INC_CMTTYPE
#include "CommonCtil.h"

//定义CommonCtil数据类型

//标准整形
typedef unsigned char         cmtU8;
typedef unsigned short        cmtU16;
typedef unsigned int          cmtU32;
typedef char                  cmtS8;
typedef short                 cmtS16;
typedef int                   cmtS32;

//指针
typedef unsigned char*		  cmtU8ptr;
typedef unsigned short*       cmtU16ptr;
typedef unsigned int*         cmtU32ptr;
typedef char*                 cmtS8ptr;
typedef short*                cmtS16ptr;
typedef int*                  cmtS32ptr;

//浮点
typedef float				  cmt32f;
typedef double		          cmt64f;

//64位
typedef unsigned long long    cmtU64;
typedef long long             cmtS64;

//64位指针
typedef long long*            cmtS64ptr;
typedef unsigned long long*   cmtU64ptr;
#endif