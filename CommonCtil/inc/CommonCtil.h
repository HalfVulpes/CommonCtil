/**
* @file CommonCtil.h
* @brief ComonCtil 库的头文件，通过在你的项目中包含此头文件来使用这个工具库
* @par Windows 依赖库：
* @par Linux 依赖库：
* @data 2021-07-15
* @author GogeBlue <mail>
* @author Dexnab
*/

//防止多重包含
#pragma once
#ifndef _INC_COMMONCTIL
#define _INC_COMMONCTIL

//操作系统环境
#ifdef __linux__
#define CMT_ENV_LINUX
#elif _WIN32
#define CMT_ENV_WINDOWS
#elif __APPLE__
#define CMT_ENV_APPLE
#elif __arm__
#define CMT_ENV_ARM
#elif i386
#define CMT_ENV_I386
#endif

//程序位数
#ifdef _WIN64
#define CMT_ENV_x64
#elif __x86_64__
#define CMT_ENV_x64
#else
#define CMT_ENV_x86
#endif

//c/c++
#ifdef __cplusplus
#define CMT_ENV_CPP
#endif

//常用数据
#define CMT_FLOAT_TOL				0.00001
#define CMT_PI						3.141592653589793
#define INIT_BUFFER_SIZE			512
#define INIT_STACK_BUFFER_SIZE		128
#define TRUE						1
#define FALSE						0

//标准库调用
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//包含自定义库
#include "cmtQueue.h"
#endif