/**
* @file cmtCryp.h
* @brief 用于支持跨平台的简化易用版本AES加密函数库 (to GogeBlue: 这个你得改一下,现在库名改成cmtCryp了,理论上是包含了所有密码学操作的(sha,md,aes,rsa等))
* @data 2021-09-13
* @author GogeBlue
*/

//防止重复包含
#pragma once
#ifndef _INC_CMTCRYP
#define _INC_CMTCRYP
#include <cmtType.h>
#include <cmtCore.h>

/*******************************SHA 256 宏定义开始*********************************/
#define SHA256_BLOCK_SIZE 32 //SHA256最终输出一个长度为32位的摘要，256Bits。

#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
/*******************************SHA 256 宏定义结束*********************************/

/*******************************SHA 256 变量定义开始*********************************/
static const cmtUint32 k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};
/*******************************SHA 256 变量定义结束*********************************/

/*******************************SHA 256 函数 结构体定义开始*********************************/
/**
* @struct cmtSHA256
* @brief SHA256结构体
* @date 2021-09-14
* @author GogeBlue
*/
typedef struct cmtSHA256_ {
	cmtUint8 data[64];
	cmtUint32 datalen;
	cmtUint64 bitlen;
	cmtUint32 state[8];
} cmtSHA256;
/*******************************SHA 256 函数 结构体定义结束*********************************/

/**
* @brief 生成真随机数字,参数buf传入一个64位无符号整形数组指针，len是偏移长度。在这个数组中将填满指定长度的随机数 (0~2^64-1)
* @date 2021-09-13
* @author Dexnab
*/
void cmtRealRand(cmtUint64* buf, cmtUint64 len);


/**
*   示例代码：
*   //字符串"abcabcabc" 的sha256值为：76b99ab4be8521d78b19bcff7d1078aabeb477bd134f404094c92cd39f051c3e
	cmtUint8 strTest[] = { "abcabcabc" };

	//用于接受sha256结果的buffer
	cmtUint8 rlt[SHA256_BLOCK_SIZE] = {NULL};

	//获取字符串的长度
	size_t len = strlen(strTest);

	//创建sha256结构体
	cmtSHA256* test = (cmtSHA256*)malloc(sizeof(cmtSHA256));

	//初始化结构体
	cmtSHA256Init(test);

	//开始更新sha256
	cmtSHA256Update(test, strTest, len);

	//获取sha256结果
	cmtSHA256Get(test, rlt);

	//查看sha256结果
	for (;i<32;i++)
	{
		printf_s("%x", rlt[i]);
	}
**/
/*******************************SHA 256 函数声明开始*********************************/
/**
* @brief 初始化cmtSHA256结构体
* @date 2021-09-14
* @author GogeBlue
*/
void cmtSHA256Init(cmtSHA256* ctx);

/**
* @brief 用data作为sha256函数的输入计算 data的值，len为data的长度。
* @date 2021-09-14
* @author GogeBlue
*/
void cmtSHA256Update(cmtSHA256* ctx, const cmtUint8 data[], size_t len);

/**
* @brief 将计算好的hash值保存到指定的数组中，这个数组的可用长度不能小于32个字节，即SHA256_BLOCK_SIZE。
* @date 2021-09-14
* @author GogeBlue
*/
void cmtSHA256Get(cmtSHA256* ctx, cmtUint8 hash[]);

/**
* @brief 矩阵函数，内部使用
* @date 2021-09-14
* @author GogeBlue
*/
void cmtSha256Transform_(cmtSHA256* ctx, const cmtUint8 data[]);
/*******************************SHA 256 函数声明结束*********************************/

#endif