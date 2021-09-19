/**
* @file cmtCryp.h
* @brief 用于支持跨平台的简化易用版本密码学库
* @data 2021-09-13
* @author Brad Conte
* @author GogeBlue
* @disclaimer: This code is presented "as is" without any guarantees.
*
* Details:
*
			  Implementation of the SHA-256 hashing algorithm.
			  SHA-256 is one of the three algorithms in the SHA2
			  specification. The others, SHA-384 and SHA-512, are not
			  offered in this implementation.
			  For SHA-384 and SHA-512, they require a x64 environment
			  Algorithm specification can be found here:
			   * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
			  This implementation uses little endian byte order.

			  Implementation of the SHA1 hashing algorithm.
			  Algorithm specification can be found here:
			   * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
			  This implementation uses little endian byte order.

			  Implementation of the MD5 hashing algorithm.
			  Algorithm specification can be found here:
			   * http://tools.ietf.org/html/rfc1321
			  This implementation uses little endian byte order.

			  The implementation of the AES algorithm and the CTR, CBC, and
			  CCM modes of operation it can be used in.
			   AES is, specified by the NIST in in publication FIPS PUB 197,
			  availible at:
			   * http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf .
			  The CBC and CTR modes of operation are specified by
			  NIST SP 800-38 A, available at:
			   * http://csrc.nist.gov/publications/nistpubs/800-38a/sp800-38a.pdf .
			  The CCM mode of operation is specified by NIST SP80-38 C, available at:
			   * http://csrc.nist.gov/publications/nistpubs/800-38C/SP800-38C_updated-July20_2007.pdf
*/

//防止重复包含
#pragma once
#ifndef _INC_CMTCRYP
#define _INC_CMTCRYP
#include <cmtType.h>
#include <cmtCore.h>

/*--------------------------------宏定义 开始--------------------------------*/

//SHA256输出摘要长度，32字节（256位）
#define CMT_SHA256_BLOCK_SIZE 32

//SHA1输出的摘要长度，20字节（160位）
#define CMT_SHA1_BLOCK_SIZE 20

//MD5输出的摘要长度，16字节（128位）
#define CMT_MD5_BLOCK_SIZE 16 

#define CMT_ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define CMT_ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))
#define CMT_KE_ROTWORD(x) (((x) << 8) | ((x) >> 24))

#define CMT_CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define CMT_MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define CMT_EP0(x) (CMT_ROTRIGHT(x,2) ^ CMT_ROTRIGHT(x,13) ^ CMT_ROTRIGHT(x,22))
#define CMT_EP1(x) (CMT_ROTRIGHT(x,6) ^ CMT_ROTRIGHT(x,11) ^ CMT_ROTRIGHT(x,25))
#define CMT_SIG0(x) (CMT_ROTRIGHT(x,7) ^ CMT_ROTRIGHT(x,18) ^ ((x) >> 3))
#define CMT_SIG1(x) (CMT_ROTRIGHT(x,17) ^ CMT_ROTRIGHT(x,19) ^ ((x) >> 10))

#define CMT_F(x,y,z) ((x & y) | (~x & z))
#define CMT_G(x,y,z) ((x & z) | (y & ~z))
#define CMT_H(x,y,z) (x ^ y ^ z)
#define CMT_I(x,y,z) (y ^ (x | ~z))

#define CMT_FF(a,b,c,d,m,s,t) { a += CMT_F(b,c,d) + m + t; \
                            a = b + CMT_ROTLEFT(a,s); }
#define CMT_GG(a,b,c,d,m,s,t) { a += CMT_G(b,c,d) + m + t; \
                            a = b + CMT_ROTLEFT(a,s); }
#define CMT_HH(a,b,c,d,m,s,t) { a += CMT_H(b,c,d) + m + t; \
                            a = b + CMT_ROTLEFT(a,s); }
#define CMT_II(a,b,c,d,m,s,t) { a += CMT_I(b,c,d) + m + t; \
                            a = b + CMT_ROTLEFT(a,s); }

//AES加密一次操作16Bytes
#define CMT_AES_BLOCK_SIZE 16

#define CMT_AES_128_ROUNDS 10
#define CMT_AES_192_ROUNDS 12
#define CMT_AES_256_ROUNDS 14

//根据明文大小计算补齐并加密后的密文大小
#define cmtAEScipherSize(PlainSize) ((PlainSize) % CMT_AES_BLOCK_SIZE ? (PlainSize) + CMT_AES_BLOCK_SIZE - (PlainSize) % CMT_AES_BLOCK_SIZE : (PlainSize))
/*--------------------------------宏定义 结束--------------------------------*/

/*--------------------------------结构体定义 开始--------------------------------*/

/**
* @struct cmtSHA256
* @brief SHA256结构体
* @date 2021-09-14
* @author Brad Conte
* @author GogeBlue
*/
typedef struct _CMTSHA256
{
	cmtUint8 data[64];
	cmtUint32 datalen;
	cmtUint64 bitlen;
	cmtUint32 state[8];
}cmtSHA256;

/**
* @struct cmtSHA1
* @brief SHA1结构体
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
typedef struct _CMTSHA1
{
	cmtUint8 data[64];
	cmtUint32 datalen;
	cmtUint64 bitlen;
	cmtUint32 state[5];
	cmtUint32 k[4];
}cmtSHA1;

/**
* @struct cmtMD5
* @brief MD5结构体
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
typedef struct _CMTMD5
{
	cmtUint8 data[64];
	cmtUint32 datalen;
	cmtUint64 bitlen;
	cmtUint32 state[4];
}cmtMD5;
/*--------------------------------结构体定义 结束--------------------------------*/

/*--------------------------------随机数函数 开始--------------------------------*/

/**
* @brief 生成真随机数
* @param[out] 输出缓冲区
* @param[in] 缓冲区长度（生成随机数的数量）
* @attention 32位模式下，每个随机数为32字节；64位模式下，每个随机数为64字节
* @date 2021-09-19
* @author Dexnab
*/
void CMT_FASTCALL cmtRealRand(cmtUint64* buf, cmtUint64 len);
/*--------------------------------随机数函数 结束--------------------------------*/

/*--------------------------------散列函数 开始--------------------------------*/

/**
* @brief 初始化cmtSHA256结构体
* @param[out] 输出结构体
* @par 示例:
* @code
	//对照：字符串"abcabcabc" 的sha256值为：76b99ab4be8521d78b19bcff7d1078aabeb477bd134f404094c92cd39f051c3e
	cmtChar str[] = "abcabcabc";
	cmtUint8 result[CMT_SHA256_BLOCK_SIZE];
	cmtSHA256 ctx;

	//初始化sha256结构体
	cmtSHA256Init(&ctx);

	//写入数据，更新sha256结构体
	cmtSHA256Update(&ctx, str, sizeof(str) - 1);//排除结尾的'\0'

	//获取结果
	cmtSHA256Get(&ctx, result);

	//输出
	for (int i = 0; i < 32; i++)
	{
		printf("%hhx", result[i]);
	}
* @endcode
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtSHA256Init(cmtSHA256* ctx);

/**
* @brief 写入数据，更新sha256结构体
* @param[in] sha256结构体
* @param[in] 数据
* @param[in] 数据字节数
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtSHA256Update(cmtSHA256* ctx, cmtUint8* data, cmtUint64 size);

/**
* @brief 计算并取值
* @param[in] sha256结构体
* @param[out] 接受缓冲区
* @attention 接受缓冲区长度不能小于CMT_SHA256_BLOCK_SIZE, 如果第一次的update是abc，第二次的update是123，最后在get则相当于计算abc123的hash256
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtSHA256Get(cmtSHA256* ctx, cmtUint8* hash);

/**
* @brief 矩阵函数，内部使用
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtSHA256Transform(cmtSHA256* ctx, cmtUint8* data);

/**
* @brief 初始化cmtSHA1结构体
* @param[out] 输出结构体
* @par 示例:
* @code
	//对照：字符串"abcabcabc" 的sha1值为：0b6f5dae7f8d68348f7d56ac05ea20a55f652d91
	cmtChar str[] = "abcabcabc";
	cmtUint8 result[CMT_SHA1_BLOCK_SIZE];
	cmtSHA1 ctx;

	//初始化sha256结构体
	cmtSHA1Init(&ctx);

	//写入数据，更新sha256结构体
	cmtSHA1Update(&ctx, str, sizeof(str) - 1);//排除结尾的'\0'

	//获取结果
	cmtSHA1Get(&ctx, result);

	//输出
	for (int i = 0; i < 20; i++)
	{
		printf("%hhx", result[i]);
	}
* @endcode
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtSHA1Init(cmtSHA1* ctx);

/**
* @brief 写入数据，更新sha1结构体
* @param[in] sha1结构体
* @param[in] 数据
* @param[in] 数据字节数
* @date 2021-09-15
* @author GogeBlue
*/
void cmtSHA1Update(cmtSHA1* ctx, cmtUint8* data, cmtUint64 size);

/**
* @brief 计算并取值
* @param[in] sha1结构体
* @param[out] 接受缓冲区
* @attention 接受缓冲区长度不能小于CMT_SHA1_BLOCK_SIZE, 如果第一次的update是abc，第二次的update是123，最后在get则相当于计算abc123的hash1
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtSHA1Get(cmtSHA1* ctx, cmtUint8* hash);

/**
* @brief 矩阵函数，内部使用
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtSHA1Transform(cmtSHA1* ctx, cmtUint8* data);

/**
* @brief 初始化cmtMD5结构体
* @param[out] 输出结构体
* @par 示例:
* @code
	//对照：字符串"abcabcabc" 的MD5值为：97ac82a5b825239e782d0339e2d7b910
	cmtChar str[] = "abcabcabc";
	cmtUint8 result[CMT_MD5_BLOCK_SIZE];
	cmtMD5 ctx;

	//初始化MD5结构体
	cmtMD5Init(&ctx);

	//写入数据，更新MD5结构体
	cmtMD5Update(&ctx, str, sizeof(str) - 1);//排除结尾的'\0'

	//获取结果
	cmtMD5Get(&ctx, result);

	//输出
	for (int i = 0; i < 16; i++)
	{
		printf("%hhx", result[i]);
	}
* @endcode
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtMD5Init(cmtMD5* ctx);

/**
* @brief 写入数据，更新MD5结构体
* @param[in] MD5结构体
* @param[in] 数据
* @param[in] 数据字节数
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtMD5Update(cmtMD5* ctx, cmtUint8* data, cmtUint64 size);

/**
* @brief 计算并取值
* @param[in] MD5结构体
* @param[out] 接受缓冲区
* @attention 接受缓冲区长度不能小于CMT_MD5_BLOCK_SIZE, 如果第一次的update是abc，第二次的update是123，最后在get则相当于计算abc123的MD5
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtMD5Get(cmtMD5* ctx, cmtUint8* hash);

/**
* @brief 矩阵函数，内部使用
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtMD5Transform(cmtMD5* ctx, cmtUint8* data);
/*--------------------------------散列函数 结束--------------------------------*/


/*--------------------------------对称加密函数 开始--------------------------------*/
/*
* 注意事项
1，对于所有的AES函数，其密钥设置函数，必须得在加密解密前执行
2，密钥的位数只能是128位（cmtUint8 key[16]），192位（cmtUint8 key[24]），或者256位（cmtUint8 key[32]）。
3，对于控制密钥位数，最方便的方法就是对用户的输入进行hash运算，如：keyInit(sha256(userInput)) //生成一个256位AES密钥。
4，默认的cmtAESKeyInit是最朴素的AES操作方式：ECB模式，这种模式并不是非常的安全，明文中的重复部分在密文中也会重复。推荐使用CBC模式
5，ECB模式下不需要使用初始化向量（IV），因为ECB模式下明文是以密码本的形式加密的，但是呢，其他模式下IV就变得重要起来了，它可以给文件增加一个随机偏移量，让相同的文件加密得到不同的密文。
*/

/**
* @brief AES密钥初始化函数（使用字符串）
* @param[in] keystr 用于生成密钥的字符串
* @param[in] sKeystr keystr字节数
* @param[out] w 计算密钥组矩阵，大小为60个32位int（240字节）
* @param[in] keysize 生成的密钥长度（如AES-256就填写256）
* @attention keystr处理过程中不会识别任何终止符
* @date 2021-09-19
* @author Brad Conte
* @author GogeBlue
*/
void cmtAESkeyInit(cmtUint8* keystr, cmtUint8* sKeystr, cmtUint32* w, cmtUint16 keysize);

/**
* @brief AES密钥初始化函数（使用定长密钥数组）
* @param[in] key 定长密钥数组，大小为keysize
* @param[out] w 计算密钥组矩阵，大小为60个32位int（240字节）
* @param[in] keysize 密钥长度（如AES-256就填写256）
* @date 2021-09-19
* @author Brad Conte
* @author GogeBlue
*/
void cmtAESRestrictkeyInit(cmtUint8* key, cmtUint32* w, cmtUint16 keysize);

/**
* @brief AES偏移向量生成函数
* @param[out] iv 偏移向量，大小为CMT_AES_BLOCK_SIZE字节
* @attention 请务必保存偏移向量，和密钥一样重要，因为在解密的时候需要偏移向量和密钥共同解密。
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtAESInitialVectorInit(cmtUint8* iv);

/**
* @brief AES-ECB模式加密函数，每次可以处理16个Bytes
* @param[in] in 明文
* @param[out] out 密文，大小为CMT_AES_BLOCK_SIZE字节
* @param[in] key 计算密钥组矩阵
* @param[in] keysize 密钥长度
* @attention 明文一次只处理CMT_AES_BLOCK_SIZE字节
* @date 2021-09-19
* @author Brad Conte
* @author GogeBlue
*/
void cmtAESecbEnc(cmtUint8* in, cmtUint8* out, cmtUint32* key, cmtUint16 keysize);

/**
* @brief AES-ECB模式解密函数，每次可以处理16个Bytes
* @param[in] in 密文
* @param[out] out 明文，大小为CMT_AES_BLOCK_SIZE字节
* @param[in] key 计算密钥组矩阵
* @param[in] keysize 密钥长度
* @attention 密文一次只处理CMT_AES_BLOCK_SIZE字节
* @date 2021-09-19
* @author Brad Conte
* @author GogeBlue
*/
void cmtAESecbDec(cmtUint8* in, cmtUint8* out, cmtUint32* key, cmtUint16 keysize);

/**
* @brief AES-ECB模式加密函数，每次可以处理任意字节
* @param[in] in 明文
* @param[out] out 密文
* @param[in] size 密文大小
* @param[in] key 计算密钥组矩阵
* @param[in] keysize 密钥长度
* @date 2021-09-19
* @arthur dexnab
*/
void cmtAESecbEncEx(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize);

/**
* @brief AES-ECB模式解密函数，每次可以处理任意字节
* @param[in] in 密文
* @param[out] out 明文
* @param[in] size 密文大小
* @param[in] key 计算密钥组矩阵
* @param[in] keysize 密钥长度
* @date 2021-09-19
* @arthur dexnab
*/
void cmtAESecbDecEx(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize);

/**
* @brief AES-CBC模式加密函数，每次可以处理n * CMT_AES_BLOCK_SIZE个字节的数据
* @param[in] in 明文
* @param[in] size 明文字节数
* @param[out] out 密文，大小为size
* @param[in] key 计算密钥组矩阵
* @param[in] keysize 密钥长度
* @param[in] iv 偏移向量
* @attention size必须为CMT_AES_BLOCK_SIZE的整数倍
* @date 2021-09-19
* @author Brad Conte
* @author GogeBlue
*/
void cmtAEScbcEnc(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize, cmtUint8* iv);

/**
* @brief AES-CBC-输出MAC模式加密函数，每次可以处理n * CMT_AES_BLOCK_SIZE个字节的数据
* @param[in] in 明文
* @param[in] size 明文字节数
* @param[out] out 输出MAC，大小为CMT_AES_BLOCK_SIZE
* @param[in] key 计算密钥组矩阵
* @param[in] keysize 密钥长度
* @param[in] iv 偏移向量
* @date 2021-09-19
* @author Brad Conte
* @author GogeBlue
*/
void cmtAEScbcEncMac(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize, cmtUint8* iv);

/**
* @brief AES-CBC模式解密函数，每次可以处理n * CMT_AES_BLOCK_SIZE字节的数据
* @param[in] in 密文
* @param[in] size 密文字节数
* @param[out] out 明文，大小为size
* @param[in] key 计算密钥组矩阵
* @param[in] keysize 密钥长度
* @param[in] iv 偏移向量
* @attention size必须为CMT_AES_BLOCK_SIZE的整数倍
* @date 2021-09-19
* @author Brad Conte
* @author GogeBlue
*/
void cmtAEScbcDec(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize, cmtUint8* iv);

/**
* @brief AES-CTR模式加密函数，每次可以处理n * CMT_AES_BLOCK_SIZE个字节的数据
* @param[in] in 明文
* @param[in] size 明文字节数
* @param[out] out 密文，大小为size
* @param[in] key 计算密钥组矩阵
* @param[in] keysize 密钥长度
* @param[in] iv 偏移向量
* @attention size必须为CMT_AES_BLOCK_SIZE的整数倍
* @date 2021-09-19
* @author Brad Conte
* @author GogeBlue
*/
void cmtAESctrEnc(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize, cmtUint8* iv);

/**
* @brief AES-CBC模式解密函数，每次可以处理n * CMT_AES_BLOCK_SIZE字节的数据
* @param[in] in 密文
* @param[in] size 密文字节数
* @param[out] out 明文，大小为size
* @param[in] key 计算密钥组矩阵
* @param[in] keysize 密钥长度
* @param[in] iv 偏移向量
* @attention size必须为CMT_AES_BLOCK_SIZE的整数倍
* @date 2021-09-19
* @author Brad Conte
* @author GogeBlue
*/
void cmtAESctrDec(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize, cmtUint8* iv);

/**
* @brief AES相关函数，仅供内部使用，懒得写注释了，const别去掉
* @date 2021-09-15
* @author Brad Conte
* @author GogeBlue
*/
void cmtAESincrIV(cmtUint8 iv[], int counter_size);
void cmtAESpreFirCTRblock(cmtUint8 counter[], const cmtUint8 nonce[], int nonceLen, int payloadLenStoreSize);
void cmtAESccmPreFirFormatBlock(cmtUint8 buf[], int assocLen, int payloadLen, int payloadLenStoreSize, int macLen, const cmtUint8 nonce[], int nonceLen);
void cmtCCMdataFormat(cmtUint8 buf[], int* endOfBuffer, const cmtUint8 assoc[], int assocLen);
void cmtCCMdataPreload(cmtUint8 buf[], int* endOfBuffer, const cmtUint8 payload[], int payloadLen);
void cmtXorBuffer(const cmtUint8 in[], cmtUint8 out[], cmtUint64 size);
void cmtAESpreFirCTRblock(cmtUint8 counter[], const cmtUint8 nonce[], int nonceLen, int payloadLenStoreSize);
void cmtAESccmPreFirFormatBlock(cmtUint8 buf[], int assocLen, int payloadLen, int payloadLenStoreSize, int macLen, const cmtUint8 nonce[], int nonceLen);
void cmtCCMdataFormat(cmtUint8 buf[], int* endOfBuffer, const cmtUint8 assoc[], int assocLen);
void cmtCCMdataPreload(cmtUint8 buf[], int* endOfBuffer, const cmtUint8 payload[], int payloadLen);
cmtUint32 cmtAESSubWord(cmtUint32 word);
void cmtAESRoundKeyInit(cmtUint8 state[][4], const cmtUint32 w[]);
void cmtAESSubBytes(cmtUint8 state[][4]);
void cmtInvSubBytes(cmtUint8 state[][4]);
void cmtShiftRows(cmtUint8 state[][4]);
void cmtInvShiftRows(cmtUint8 state[][4]);
void cmtMixColumns(cmtUint8 state[][4]);
void cmtInvMixColumns(cmtUint8 state[][4]);
/*--------------------------------对称加密函数 结束--------------------------------*/


#endif