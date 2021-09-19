#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <cmtCryp.h>

void cmtDemoAESctr()
{
	//AES-CTR测试代码
	cmtChar keystr[] = "abc123abc999!";//字符串口令密钥
	//明文（摘自某一段ACT文法文章 2019-C03 P5）
	cmtChar data[cmtAEScipherSize(sizeof("Separating Oregon from Washington, the Columbia River Gorge is eighty-five miles of flowing water, there are tree-covered bluffs, and roaring waterfalls. These striking features daunted would-be road builders until 1913. That year, Samuel Hill and Samuel Lancaster, a businessman and an engineer, respectively, began constructing a road through the gorge to connect the towns along the river. Their design went beyond practicalities it showcased the scenic grandeur of the gorge where the Columbia River is located."))] =
		"Separating Oregon from Washington, the Columbia River Gorge is eighty-five miles of flowing water, there are tree-covered bluffs, and roaring waterfalls. These striking features daunted would-be road builders until 1913. That year, Samuel Hill and Samuel Lancaster, a businessman and an engineer, respectively, began constructing a road through the gorge to connect the towns along the river. Their design went beyond practicalities it showcased the scenic grandeur of the gorge where the Columbia River is located.";
	cmtUint32 key[60];//计算密钥组矩阵
	cmtUint8 enced[cmtAEScipherSize(sizeof(data))];//密文
	cmtChar deced[cmtAEScipherSize(sizeof(data))];//还原明文
	cmtUint8 iv[CMT_AES_BLOCK_SIZE];

	//生成 计算密钥组矩阵
	cmtAESkeyInit(keystr, sizeof(keystr), key, 256);

	//iv
	cmtAESInitialVectorInit(iv);

	//加密
	cmtAESctrEnc(data, sizeof(data), enced, key, 256, iv);

	//解密
	cmtAESctrDec(enced, sizeof(enced), deced, key, 256, iv);

	printf("AES-CTR:\nplain:\n%hs\ncipher:\n%hs\n", data, deced);
}

void cmtDemoAEScbc()
{
	//AES-CBC测试代码
	cmtChar keystr[] = "abc123abc999!";//字符串口令密钥
	//明文（摘自某一段ACT文法文章 2019-C03 P5）
	cmtChar data[cmtAEScipherSize(sizeof("Separating Oregon from Washington, the Columbia River Gorge is eighty-five miles of flowing water, there are tree-covered bluffs, and roaring waterfalls. These striking features daunted would-be road builders until 1913. That year, Samuel Hill and Samuel Lancaster, a businessman and an engineer, respectively, began constructing a road through the gorge to connect the towns along the river. Their design went beyond practicalities it showcased the scenic grandeur of the gorge where the Columbia River is located."))] =
		"Separating Oregon from Washington, the Columbia River Gorge is eighty-five miles of flowing water, there are tree-covered bluffs, and roaring waterfalls. These striking features daunted would-be road builders until 1913. That year, Samuel Hill and Samuel Lancaster, a businessman and an engineer, respectively, began constructing a road through the gorge to connect the towns along the river. Their design went beyond practicalities it showcased the scenic grandeur of the gorge where the Columbia River is located.";
	cmtUint32 key[60];//计算密钥组矩阵
	cmtUint8 enced[cmtAEScipherSize(sizeof(data))];//密文
	cmtChar deced[cmtAEScipherSize(sizeof(data))];//还原明文
	cmtUint8 iv[CMT_AES_BLOCK_SIZE];

	//生成 计算密钥组矩阵
	cmtAESkeyInit(keystr, sizeof(keystr), key, 256);

	//iv
	cmtAESInitialVectorInit(iv);

	//加密
	cmtAEScbcEnc(data, sizeof(data), enced, key, 256, iv);

	//解密
	cmtAEScbcDec(enced, sizeof(enced), deced, key, 256, iv);

	printf("AES-CBC:\nplain:\n%hs\ncipher:\n%hs\n", data, deced);
}

void cmtDemoAESecb()
{
	//AES-ECB测试代码
	cmtChar keystr[] = "abc123abc999!";//字符串口令密钥
	//明文（摘自某一段ACT文法文章 2019-C03 P5）
	cmtChar data[] = "Separating Oregon from Washington, the Columbia River Gorge is eighty-five miles of flowing water, there are tree-covered bluffs, and roaring waterfalls. These striking features daunted would-be road builders until 1913. That year, Samuel Hill and Samuel Lancaster, a businessman and an engineer, respectively, began constructing a road through the gorge to connect the towns along the river. Their design went beyond practicalities it showcased the scenic grandeur of the gorge where the Columbia River is located.";
	cmtUint32 key[60];//计算密钥组矩阵
	cmtUint8 enced[cmtAEScipherSize(sizeof(data))];//密文
	cmtChar deced[cmtAEScipherSize(sizeof(data))];//还原明文

	//生成 计算密钥组矩阵
	cmtAESkeyInit(keystr, sizeof(keystr), key, 256);

	//加密
	cmtAESecbEncEx(data, sizeof(data), enced, key, 256);

	//解密
	cmtAESecbDecEx(enced, sizeof(enced), deced, key, 256);

	printf("AES-ECB:\nplain:\n%hs\ncipher:\n%hs\n", data, deced);
}

int main(int argc, char** argv)
{
	cmtDemoAESecb();
	cmtDemoAEScbc();
	cmtDemoAESctr();
	
	return 0;
}