#include <stdio.h>
#include <cmtString.h>

//123测试123
//ansi (locale=zh-cn):
//\x31\x32\x33\xb2\xe2\xca\xd4\x31\x32\x33
//u8:
//\x31\x32\x33\xe6\xb5\x8b\xe8\xaf\x95\x31\x32\x33
//u16:
//\x31\x00\x32\x00\x33\x00\x4b\x6d\xd5\x8b\x31\x00\x32\x00\x33\x00
//u32:
//\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00\x4b\x6d\x00\x00\xd5\x8b\x00\x00\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00
//123测试123𣱕a
//u8:
//\x31\x32\x33\xe6\xb5\x8b\xe8\xaf\x95\x31\x32\x33\xf0\xa3\xb1\x95\x61
//u16:
//\x31\x00\x32\x00\x33\x00\x4b\x6d\xd5\x8b\x31\x00\x32\x00\x33\x00\x4f\xd8\x55\xdc\x61\x00
//u32:
//\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00\x4b\x6d\x00\x00\xd5\x8b\x00\x00\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00\x55\x3c\x02\x00\x61\x00\x00\x00

void cmtDemoANSI()
{
	cmtANSIstr ansi = CMT_CONSTSTR("\x31\x32\x33\xb2\xe2\xca\xd4\x31\x32\x33");//123测试123
	ansi.locale = "zh-cn";
	cmtU8str u8;
	cmtU16str u16;
	cmtU32str u32;

	//测试1：ANSI转UTF-8
	u8.size = cmtANSItoU8size(&ansi);
	u8.data = malloc(u8.size);
	cmtANSItoU8(&ansi, &u8);
	//标答：u8->data="\x31\x32\x33\xe6\xb5\x8b\xe8\xaf\x95\x31\x32\x33"
	//u8->size=13

	//测试2：ANSI转UTF-16
	u16.size = cmtANSItoU16size(&ansi);
	u16.data = malloc(u16.size);
	cmtANSItoU16(&ansi, &u16);
	//标答：u16->data="\x31\x00\x32\x00\x33\x00\x4b\x6d\xd5\x8b\x31\x00\x32\x00\x33\x00"
	//u16->size=18

	//测试3：ANSI转UTF-32
	u32.size = cmtANSItoU32size(&ansi);
	u32.data = malloc(u32.size);
	cmtANSItoU32(&ansi, &u32);
	//标答：u16->data="\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00\x4b\x6d\x00\x00\xd5\x8b\x00\x00\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00"
	//u16->size=33

	free(u8.data);
	free(u16.data);
	free(u32.data);
}

void cmtDemoU8()
{
	cmtU8str u8a = CMT_CONSTSTR("\x31\x32\x33\xe6\xb5\x8b\xe8\xaf\x95\x31\x32\x33");//123测试123
	cmtU8str u8b = CMT_CONSTSTR("\x31\x32\x33\xe6\xb5\x8b\xe8\xaf\x95\x31\x32\x33\xf0\xa3\xb1\x95\x61");//123测试123𣱕a
	cmtANSIstr ansi;
	cmtU16str u16;
	cmtU32str u32;
	cmtBool err;

	ansi.size = cmtU8toANSIsize(&u8a, "zh-cn", &err);
	if (err)
	{
		printf("cmtDemoU8: ERROR: failed to convert into ANSI\n");
		return;
	}
	ansi.data = malloc(ansi.size);
	if (!ansi.data)
	{
		printf("cmtDemoU8: ERROR: memory not enough\n");
		return;
	}
	ansi.locale = "zh-cn";
	cmtU8toANSI(&u8a, &ansi);

	u16.size = cmtU8toU16size(&u8);
	u16.data = malloc(u16.size);
	if (!u16.data)
	{
		printf("cmtDemoU8: ERROR: memory not enough\n");
		free(ansi.data);
		return;
	}
	cmtU8toU16(&u8, &u16);

	u32.size = cmtU8toU32size(&u8);
	u32.data = malloc(u32.size);
	if (!u32.data)
	{
		printf("cmtDemoU8: ERROR: memory not enough\n");
		free(ansi.data);
		free(u16.data);
		return;
	}
	cmtU8toU32(&u8, &u32);

	free(ansi.data);
	free(u16.data);
	free(u32.data);
}

void cmtDemoU16()
{
	//UTF-16的字符串正常情况下应写成L"xxx"的形式，但由于这里是直接指定的编码值，所以写成"xxx"的形式避免把8位的数据扩展成16位
	//如："\x31"写成L"\x31"就会变成0x31 0x00
	//同时，由于"xxx"字符串结尾只会自动加1个'\0'，所以还要手动加一个（相比之下，L"xxx"的结尾会自动加两个'\0'，也就是一个L'\0'）
	cmtU8str u16a = CMT_CONSTSTR("\x31\x00\x32\x00\x33\x00\x4b\x6d\xd5\x8b\x31\x00\x32\x00\x33\x00\x00");
	cmtU8str u16 = CMT_CONSTSTR("\x31\x00\x32\x00\x33\x00\x4b\x6d\xd5\x8b\x31\x00\x32\x00\x33\x00\x4f\xd8\x55\xdc\x61\x00\x00");
	cmtANSIstr ansi;
	cmtU8str u8;
	cmtU32str u32;
	cmtBool err;

	ansi.size = cmtU16toANSIsize(&u16a, "zh-cn", &err);
	if (err)
	{
		printf("cmtDemoU16: ERROR: failed to convert into ANSI\n");
		return;
	}
	ansi.data = malloc(ansi.size);
	if (!ansi.data)
	{
		printf("cmtDemoU16: ERROR: memory not enough\n");
		return;
	}
	ansi.locale = "zh-cn";
	cmtU16toANSI(&u16a, &ansi);

	u8.size = cmtU16toU8size(&u16);
	u8.data = malloc(u8.size);
	if (!u8.data)
	{
		printf("cmtDemoU16: ERROR: memory not enough\n");
		free(ansi.data);
		return;
	}
	cmtU16toU8(&u16, &u8);

	u32.size = cmtU16toU32size(&u16);
	u32.data = malloc(u32.size);
	if (!u32.data)
	{
		printf("cmtDemoU16: ERROR: memory not enough\n");
		free(ansi.data);
		free(u8.data);
		return;
	}
	cmtU16toU32(&u16, &u32);

	free(ansi.data);
	free(u8.data);
	free(u32.data);
}

void cmtDemoU32()
{
	//同样，由于"xxx"只会自动在结尾加一个'\0'，所以还要手动再加三个
	cmtU8str u32a = CMT_CONSTSTR("\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00\x4b\x6d\x00\x00\xd5\x8b\x00\x00\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00\x00\x00\x00");
	cmtU8str u32 = CMT_CONSTSTR("\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00\x4b\x6d\x00\x00\xd5\x8b\x00\x00\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00\x55\x3c\x02\x00\x61\x00\x00\x00\x00\x00\x00");
	cmtANSIstr ansi;
	cmtU8str u8;
	cmtU16str u16;
	cmtBool err;

	ansi.size = cmtU32toANSIsize(&u32a, "zh-cn", &err);
	if (err)
	{
		printf("cmtDemoU32: ERROR: failed to convert into ANSI\n");
		return;
	}
	ansi.data = malloc(ansi.size);
	if (!ansi.data)
	{
		printf("cmtDemoU32: ERROR: memory not enough\n");
		return;
	}
	ansi.locale = "zh-cn";
	cmtU32toANSI(&u32a, &ansi);

	u8.size = cmtU32toU8size(&u32);
	u8.data = malloc(u8.size);
	if (!u8.data)
	{
		printf("cmtDemoU32: ERROR: memory not enough\n");
		free(ansi.data);
		return;
	}
	cmtU32toU8(&u32, &u8);

	u16.size = cmtU32toU16size(&u32);
	u16.data = malloc(u16.size);
	if (!u16.data)
	{
		printf("cmtDemoU32: ERROR: memory not enough\n");
		free(ansi.data);
		free(u8.data);
		return;
	}
	cmtU32toU16(&u32, &u16);

	free(ansi.data);
	free(u8.data);
	free(u16.data);
}

void cmtDemoAnlyFmt()
{
	cmtU8str u8 = CMT_CONSTSTR("+-010.=*r2-2-2f\n");
	cmtFmtInfo FmtInfo;
	cmtUint64 arglist[1];
	arglist[0] = 12;

	cmtAnlyFmt(&u8, &FmtInfo, arglist);
}

void cmtDemoStrToBin()
{
	cmtU8str str1 = CMT_CONSTSTR("101011011");//10101101b=173
	str1.size = 8;
	cmtU8str str2 = CMT_CONSTSTR("101011011abc");//101011011b=347
	str2.size = 12;
	cmtU8str str3 = CMT_CONSTSTR("abc10");
	str3.size = 5;
	cmtUint64 num, ret;

	//测试1：边界检测
	ret = cmtStrToBin(&str1, &num);
	//标答：num=173 ret=8

	//测试2：干扰字符检测1
	ret = cmtStrToBin(&str2, &num);
	//标答：num=347 ret=9

	//测试3：干扰字符检测2
	ret = cmtStrToBin(&str3, &num);
	//标答：num=0 ret=0
}

void cmtDemoStrToOct()
{
	cmtU8str str1 = CMT_CONSTSTR("123456");//12345o=5349
	str1.size = 5;
	cmtU8str str2 = CMT_CONSTSTR("123456abc");//123546o=42798
	str2.size = 9;
	cmtU8str str3 = CMT_CONSTSTR("abc123");
	str3.size = 6;
	cmtUint64 num, ret;

	//测试1：边界检测
	ret = cmtStrToOct(&str1, &num);
	//标答：num=5349 ret=5

	//测试2：干扰字符检测1
	ret = cmtStrToOct(&str2, &num);
	//标答：num=42798 ret=6

	//测试3：干扰字符检测2
	ret = cmtStrToOct(&str3, &num);
	//标答：num=0 ret=0
}

void cmtDemoStrToDec()
{
	cmtU8str str1 = CMT_CONSTSTR("12345678");
	str1.size = 5;
	cmtU8str str2 = CMT_CONSTSTR("+123456");
	str2.size = 7;
	cmtU8str str3 = CMT_CONSTSTR("-123456");
	str3.size = 7;
	cmtU8str str4 = CMT_CONSTSTR("1234abc");
	str4.size = 7;
	cmtU8str str5 = CMT_CONSTSTR("abc1234");
	str5.size = 7;
	cmtInt64 num, ret;

	//测试1：边界检测
	ret = cmtStrToDec(&str1, &num);
	//标答：num=12345 ret=5

	//测试2：正号检测
	ret = cmtStrToDec(&str2, &num);
	//标答：num=123456 ret=7

	//测试3：负号检测
	ret = cmtStrToDec(&str3, &num);
	//标答：num=-123456 ret=7

	//测试4：干扰字符检测1
	ret = cmtStrToDec(&str4, &num);
	//标答：num=1234 ret=4

	//测试5：干扰字符检测2
	ret = cmtStrToDec(&str5, &num);
	//标答：num=0 ret=0
}

void cmtDemoStrToHex()
{
	cmtU8str str1 = CMT_CONSTSTR("123456");
	str1.size = 5;
	cmtU8str str2 = CMT_CONSTSTR("123456aBc");
	str2.size = 9;
	cmtU8str str3 = CMT_CONSTSTR("123abch");
	str3.size = 7;
	cmtU8str str4 = CMT_CONSTSTR("x123");
	str4.size = 4;
	cmtUint64 num, ret;

	//测试1：边界检测
	ret = cmtStrToHex(&str1, &num);
	//标答：num=12345h ret=5

	//测试2：大小写a-f(A-F)检测
	ret = cmtStrToHex(&str2, &num);
	//标答：num=123456abch ret=9

	//测试3：干扰字符检测1
	ret = cmtStrToHex(&str3, &num);
	//标答：num=123abch ret=6

	//测试4：干扰字符检测2
	ret = cmtStrToHex(&str4, &num);
	//标答：num=0 ret=0
}

void cmtDemoStrToF32()
{
	cmtU8str str1 = CMT_CONSTSTR("1.4567");
	str1.size = 3;
	cmtU8str str2 = CMT_CONSTSTR("123.0007");
	str2.size = 8;
	cmtU8str str3 = CMT_CONSTSTR("1357000000000000000000000000000.000");
	str3.size = 35;
	cmtU8str str4 = CMT_CONSTSTR("0.0000000000000000000000000001357");
	str4.size = 33;
	cmtU8str str5 = CMT_CONSTSTR("123.543e+20");
	str5.size = 11;
	cmtU8str str6 = CMT_CONSTSTR("92.0003E+20");
	str6.size = 11;
	cmtU8str str7 = CMT_CONSTSTR("543.159abc");
	str7.size = 10;
	cmtU8str str8 = CMT_CONSTSTR("abc543.159");
	str8.size = 10;
	cmtU8str str9 = CMT_CONSTSTR("-164.327000");
	str9.size = 11;
	cmtU8str str10 = CMT_CONSTSTR("00580.13e-10");
	str10.size = 12;
	cmtU8str str11 = CMT_CONSTSTR("065300");
	str11.size = 6;
	cmtU8str str12 = CMT_CONSTSTR(".036600");
	str12.size = 7;
	float num;
	cmtUint64 ret;

	//测试1：边界检测
	ret = cmtStrToF32(&str1, &num);
	//标答：num=1.4（近似即可） ret=3
	
	//测试2：普通小数
	ret = cmtStrToF32(&str2, &num);
	//标答：num=123.0007（近似即可） ret=8
	
	//测试3：极大数
	ret = cmtStrToF32(&str3, &num);
	//标答：num=1.357e+30（近似即可） ret=35
	
	//测试4：极小数
	ret = cmtStrToF32(&str4, &num);
	//标答：num=1.357e-28（近似即可） ret=33
	
	//测试5：科学计数法（小写e）
	ret = cmtStrToF32(&str5, &num);
	//标答：num=1.23543e+22（近似即可） ret=11
	
	//测试6：科学计数法（大写E）
	ret = cmtStrToF32(&str6, &num);
	//标答：num=9.20003+21（近似即可） ret=11
	
	//测试7：干扰字符检测1
	ret = cmtStrToF32(&str7, &num);
	//标答：num=543.159（近似即可） ret=7
	
	//测试8：干扰字符检测2
	ret = cmtStrToF32(&str8, &num);
	//标答：num=0.0 ret=0

	//测试9：负数
	ret = cmtStrToF32(&str9, &num);
	//标答：num=-164.327（近似即可） ret=11

	//测试10：负指数
	ret = cmtStrToF32(&str10, &num);
	//标答：num=5.8013e-8（近似即可） ret=12

	//测试11：无小数
	ret = cmtStrToF32(&str11, &num);
	//标答：num=65300.0（近似即可） ret=6

	//测试12：无整数
	ret = cmtStrToF32(&str12, &num);
	//标答：num=0.0366 ret=7
}

void cmtDemoStrToF64()
{
	cmtU8str str1 = CMT_CONSTSTR("1.4567");
	str1.size = 3;
	cmtU8str str2 = CMT_CONSTSTR("123.0000007");
	str2.size = 11;
	cmtU8str str3 = CMT_CONSTSTR("135700000000000000000000000000000000000000000000000000000.000");
	str3.size = 61;
	cmtU8str str4 = CMT_CONSTSTR("0.00000000000000000000000000000000000000000000000000000000000000001357");
	str4.size = 70;
	cmtU8str str5 = CMT_CONSTSTR("123.543e+90");
	str5.size = 11;
	cmtU8str str6 = CMT_CONSTSTR("92.0003E+120");
	str6.size = 12;
	cmtU8str str7 = CMT_CONSTSTR("543.159abc");
	str7.size = 10;
	cmtU8str str8 = CMT_CONSTSTR("abc543.159");
	str8.size = 10;
	cmtU8str str9 = CMT_CONSTSTR("-1640000.327000");
	str9.size = 15;
	cmtU8str str10 = CMT_CONSTSTR("00580.13e-170");
	str10.size = 13;
	cmtU8str str11 = CMT_CONSTSTR("065300");
	str11.size = 6;
	cmtU8str str12 = CMT_CONSTSTR(".036600");
	str12.size = 7;
	double num;
	cmtUint64 ret;

	//测试1：边界检测
	ret = cmtStrToF64(&str1, &num);
	//标答：num=1.4（近似即可） ret=3

	//测试2：普通小数
	ret = cmtStrToF64(&str2, &num);
	//标答：num=123.0000007（近似即可） ret=11

	//测试3：极大数
	ret = cmtStrToF64(&str3, &num);
	//标答：num=1.357e+56（近似即可） ret=61

	//测试4：极小数
	ret = cmtStrToF64(&str4, &num);
	//标答：num=1.357e-65（近似即可） ret=70

	//测试5：科学计数法（小写e）
	ret = cmtStrToF64(&str5, &num);
	//标答：num=1.23543e+92（近似即可） ret=11

	//测试6：科学计数法（大写E）
	ret = cmtStrToF64(&str6, &num);
	//标答：num=9.20003+121（近似即可） ret=12

	//测试7：干扰字符检测1
	ret = cmtStrToF64(&str7, &num);
	//标答：num=543.159（近似即可） ret=7

	//测试8：干扰字符检测2
	ret = cmtStrToF64(&str8, &num);
	//标答：num=0.0 ret=0

	//测试9：负数
	ret = cmtStrToF64(&str9, &num);
	//标答：num=-1640000.327（近似即可） ret=15

	//测试10：负指数
	ret = cmtStrToF64(&str10, &num);
	//标答：num=5.8013e-168（近似即可） ret=13

	//测试11：无小数
	ret = cmtStrToF64(&str11, &num);
	//标答：num=65300.0（近似即可） ret=6

	//测试12：无整数
	ret = cmtStrToF64(&str12, &num);
	//标答：num=0.0366 ret=7
}

void cmtDemoBinToStr()
{

}

//void cmtDemoSprintf()
//{
//	cmtU8str str = CMT_CONSTSTR("输出二进制低8位（填0补齐12位）：%012.8%b；\n"
//		"输出八进制全部位数（填0补齐6位）：%06%o；\n"
//		"输出有符号十进制全部位数（填空格补齐10位，带正号）：%+10d；\n"
//		"输出无符号十进制全部位数（不补齐）：%u；\n"
//		"输出十六进制全部位数（填空格补齐6位，小写）：%6x；\n"
//		"输出十六进制全部位数（填空格补齐6位，左对齐，大写）：%-6X；\n"
//		"输出64位浮点数（有效位数10位，带正号）：%+.=10llf；\n"
//		"输出32位浮点数（默认）：%hhf；\n"
//		"输出64位浮点数（保留9位有效数字，指数模式，小写）：%.=9lle；\n"
//		"输出32位浮点数（保留3位有效数字，指数模式，小写）：%.=3hhe；\n"
//		"输出64位浮点数（保留9位有效数字，指数模式，大写）：%.=9llE；\n"
//		"输出32位浮点数（保留3位有效数字，指数模式，大写）：%.=3hhE；\n"
//		"输出64位浮点数（保留9位有效数字，自动模式，小写）：%.=9llg；\n"
//		"输出32位浮点数（自动模式，小写）：%hhg；\n"
//		"输出64位浮点数（保留2位有效数字，自动模式，大写）：%.=2llG；\n"
//		"输出32位浮点数（自动模式，大写）：%hhG；\n"
//		"输出UTF-16字符串（最多26字节）：%.26ls；\n");
//	cmtU8str out;
//	cmtU16str u16 = CMT_CONSTSTR(L"测试UTF-16字符串123456789");
//
//	setlocale(LC_ALL, ".utf-8");
//
//	out.data = malloc(4096);
//	out.size = 4096;
//
//	//0x76ad = 0111 0110 1010 1101 = 073 255 = 30381
//	cmtSprintf(&out, &str, 0x76ad, 0x76ad, 0x76ad, 0x76ad, 0x76ad, 0x76ad, 1234.12345678, -1234.12345678f, 1234.12345678, 0.01234f, -1234.12345678, 1234.0f, 1234.12345678, 0.0000123f, -1234.12345678, 1234.0f, &u16);
//	printf("%.*s", out.size, out.data);
//}

//int main(int argc, char** agrv)
//{
//	//ansi->?
//	cmtDemoANSI();
//
//	//utf-8->?
//	cmtDemoU8();
//
//	//utf-16->?
//	cmtDemoU16();
//
//	//utf-32->?
//	cmtDemoU32();
//
//	//cmtSprintf
//	cmtDemoSprintf();
//
//	return 0;
//}