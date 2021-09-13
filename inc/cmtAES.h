/**
* @file cmtAES.h
* @brief 用于支持跨平台的简化易用版本AES加密函数库\n
* @data 2021-09-13
* @author GogeBlue
*/

#include <cmtType.h>
#include "cmtCore.h"
#if defined(CMT_ENV_WINDOWS)
#include <Windows.h>
#elif defined(CMT_ENV_LINUX)
#include <pthread.h>
#endif

#pragma once
#ifndef _INC_CMTAES
#define _INC_CMTAES

#endif