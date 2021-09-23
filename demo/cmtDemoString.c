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
	cmtANSIstr ansi = CMT_CONSTSTR("\x31\x32\x33\xb2\xe2\xca\xd4\x31\x32\x33");
	ansi.locale = "zh-cn";
	cmtU8str u8;
	cmtU16str u16;
	cmtU32str u32;

	u8.size = cmtANSItoU8size(&ansi);
	u8.data = malloc(u8.size);
	if (!u8.data)
	{
		printf("cmtDemoANSI: ERROR: memory not enough\n");
		return;
	}
	cmtANSItoU8(&ansi, &u8);

	u16.size = cmtANSItoU16size(&ansi);
	u16.data = malloc(u16.size);
	if (!u16.data)
	{
		printf("cmtDemoANSI: ERROR: memory not enough\n");
		free(u8.data);
		return;
	}
	cmtANSItoU16(&ansi, &u16);

	u32.size = cmtANSItoU32size(&ansi);
	u32.data = malloc(u32.size);
	if (!u32.data)
	{
		printf("cmtDemoANSI: ERROR: memory not enough\n");
		free(u8.data);
		free(u16.data);
		return;
	}
	cmtANSItoU32(&ansi, &u32);

	free(u8.data);
	free(u16.data);
	free(u32.data);
}

void cmtDemoU8()
{
	cmtU8str u8a = CMT_CONSTSTR("\x31\x32\x33\xe6\xb5\x8b\xe8\xaf\x95\x31\x32\x33");
	cmtU8str u8 = CMT_CONSTSTR("\x31\x32\x33\xe6\xb5\x8b\xe8\xaf\x95\x31\x32\x33\xf0\xa3\xb1\x95\x61");
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

int main(int argc, char** agrv)
{
	cmtU8str u8 = CMT_CONSTSTR("\x31\x32\x33\xe6\xb5\x8b\xe8\xaf\x95\x31\x32\x33\xf0\xa3\xb1\x95\x61");
	cmtU16str u16 = CMT_CONSTSTR("\x31\x00\x32\x00\x33\x00\x4b\x6d\xd5\x8b\x31\x00\x32\x00\x33\x00\x4f\xd8\x55\xdc\x61\x00");
	cmtU32str u32 = CMT_CONSTSTR("\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00\x4b\x6d\x00\x00\xd5\x8b\x00\x00\x31\x00\x00\x00\x32\x00\x00\x00\x33\x00\x00\x00\x55\x3c\x02\x00\x61\x00\x00\x00");

	//ansi->?
	cmtDemoANSI();

	//utf-8->?
	cmtDemoU8();

	//utf-16->?
	cmtDemoU16();

	//utf-32->?
	cmtDemoU32();

	return 0;
}