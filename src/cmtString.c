/**
* @file cmtCore.c
* @date 2021-09-23
* @author Dexnab
*/

#include <cmtString.h>

cmtUint8 cmtANSIchSize(cmtChar* ch, cmtChar* locale)
{
	cmtUint8 chsize;
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	/*调试笔记:
	* 之后setlocale会改变此处返回值指向的内存,所以需要复制一遍
	*/
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, locale);

	chsize = mblen(ch, MB_CUR_MAX);

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);

	return chsize;
}

cmtUint64 cmtANSIstrSize(cmtChar* str)
{
	cmtUint64 size = 0;

	while (str[size]) size++;

	return size;
}

cmtUint64 cmtANSIlen(cmtANSIstr* str)
{
	cmtUint64 len = 0, r = 0;
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, str->locale);

	while (r < str->size)
	{
		//mblen不会把'\0'算作字符，所以需要单独来处理
		if (!str->data[r]) r++;
		else r += mblen(str->data + r, str->size - r);
		len++;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);

	return len;
}

cmtUint64 cmtANSItoU8size(cmtANSIstr* ansi)
{
	cmtUint64 rAs = 0, u8size = 0;
	cmtUint8 chsize;
	cmtWchar u16temp[2];
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, ansi->locale);

	while (rAs < ansi->size)
	{
		//'\0'
		if (!ansi->data[rAs])
		{
			rAs++;
			u8size++;
			continue;
		}

		chsize = mblen(ansi->data + rAs, ansi->size - rAs);
		mbtowc(u16temp, ansi->data + rAs, chsize);
		//如果首字在保留区间外，那么绝对只有一个字
		if (u16temp[0] < CMT_UNICODE_RSV_START || u16temp[0] > CMT_UNICODE_RSV_END)
		{
			//[0,0x7f]
			if (u16temp[0] < 0x80) u8size++;
			//[0x80,0x07ff]
			else if (u16temp[0] < 0x800) u8size += 2;
			//[0x0800,0xffff]
			else u8size += 3;
		}
		//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
		else u8size += 4;
		rAs += chsize;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);

	return u8size;
}

void cmtANSItoU8(cmtANSIstr* ansi, cmtU8str* u8)
{
	cmtUint64 rAs = 0, rU8 = 0;
	cmtUint8 chsize;
	cmtWchar u16temp[2];
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, ansi->locale);

	while (rAs < ansi->size && rU8 < u8->size)
	{
		//'\0'
		if (!ansi->data[rAs])
		{
			u8->data[rU8] = 0;
			rAs++;
			rU8++;
			continue;
		}

		chsize = mblen(ansi->data + rAs, ansi->size - rAs);
		mbtowc(u16temp, ansi->data + rAs, chsize);
		//如果首字在保留区间外，那么绝对只有一个字
		if (u16temp[0] < CMT_UNICODE_RSV_START || u16temp[0] > CMT_UNICODE_RSV_END)
		{
			//[0,0x7f]
			if (u16temp[0] < 0x80)
			{
				u8->data[rU8] = (cmtUint8)u16temp[0];
				rU8++;
			}
			//[0x80,0x07ff]
			else if (u16temp[0] < 0x800)
			{
				//u8第二字节
				u8->data[rU8 + 1] = 0x80 + ((cmtUint8)u16temp[0] & 0x3f);
				//u8第一字节
				u16temp[0] >>= 6;
				u8->data[rU8] = 0xc0 + (cmtUint8)u16temp[0];
				rU8 += 2;
			}
			//[0x0800,0xffff]
			else
			{
				//u8第三字节
				u8->data[rU8 + 2] = 0x80 + ((cmtUint8)u16temp[0] & 0x3f);
				//u8第二字节
				u16temp[0] >>= 6;
				u8->data[rU8 + 1] = 0x80 + ((cmtUint8)u16temp[0] & 0x3f);
				//u8第三字节
				u16temp[0] >>= 6;
				u8->data[rU8] = 0xe0 + (cmtUint8)u16temp[0];
				rU8 += 3;
			}
		}
		//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
		else
		{
			u16temp[0] = u16temp[0] - 0xd800 + 0x40;
			u16temp[1] -= 0xdc00;
			//u8第四字节
			u8->data[rU8 + 3] = 0x80 + ((cmtUint8)u16temp[1] & 0x3f);
			//u8第三字节
			u16temp[1] >>= 6;
			u8->data[rU8 + 2] = 0x80 + (cmtUint8)u16temp[1] + (((cmtUint8)u16temp[0] & 0x3) << 4);
			//u8第二字节
			u16temp[0] >>= 2;
			u8->data[rU8 + 1] = 0x80 + ((cmtUint8)u16temp[0] & 0x3f);
			//u8第一字节
			u16temp[0] >>= 6;
			u8->data[rU8] = 0xf0 + (cmtUint8)u16temp[0];
			rU8 += 4;
		}
		rAs += chsize;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);
}

cmtUint64 cmtANSItoU16size(cmtANSIstr* ansi)
{
	cmtUint64 rAs = 0, u16size = 0;
	cmtUint8 chsize;
	cmtWchar u16temp[2];
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, ansi->locale);

	while (rAs < ansi->size)
	{
		//'\0'
		if (!ansi->data[rAs])
		{
			rAs++;
			u16size += 2;
			continue;
		}

		chsize = mblen(ansi->data + rAs, ansi->size - rAs);
		mbtowc(u16temp, ansi->data + rAs, chsize);
		//如果首字在保留区间外，那么绝对只有一个字
		if (u16temp[0] < CMT_UNICODE_RSV_START || u16temp[0] > CMT_UNICODE_RSV_END) u16size += 2;
		//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
		else u16size += 4;
		rAs += chsize;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);

	return u16size;
}

void cmtANSItoU16(cmtANSIstr* ansi, cmtU16str* u16)
{
	cmtUint64 rAs = 0, rU16 = 0;
	cmtUint64 rU16max;
	cmtUint8 chsize;
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, ansi->locale);

	rU16max = u16->size / 2;
	while (rAs < ansi->size && rU16 < rU16max)
	{
		//'\0'
		if (!ansi->data[rAs])
		{
			u16->data[rU16] = 0;
			rAs++;
			rU16++;
			continue;
		}

		chsize = mblen(ansi->data + rAs, ansi->size - rAs);
		mbtowc(u16->data + rU16, ansi->data + rAs, chsize);
		//如果首字在保留区间外，那么绝对只有一个字
		if (u16->data[rU16] < CMT_UNICODE_RSV_START || u16->data[rU16] > CMT_UNICODE_RSV_END) rU16++;
		//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
		else rU16 += 2;
		rAs += chsize;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);
}

cmtUint64 cmtANSItoU32size(cmtANSIstr* ansi)
{
	return cmtANSIlen(ansi) * 4;
}

void cmtANSItoU32(cmtANSIstr* ansi, cmtU32str* u32)
{
	cmtUint64 rAs = 0, rU32 = 0;
	cmtUint64 rU32max;
	cmtUint8 chsize;
	cmtWchar u16temp[2];
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, ansi->locale);

	rU32max = u32->size / 4;
	while (rAs < ansi->size && rU32 < rU32max)
	{
		//'\0'
		if (!ansi->data[rAs])
		{
			u32->data[rU32] = 0;
			rAs++;
			rU32++;
			continue;
		}

		chsize = mblen(ansi->data + rAs, ansi->size - rAs);
		mbtowc(u16temp, ansi->data + rAs, chsize);
		//如果首字在保留区间外，那么绝对只有一个字
		if (u16temp[0] < CMT_UNICODE_RSV_START || u16temp[0] > CMT_UNICODE_RSV_END)
		{
			u32->data[rU32] = u16temp[0];
		}
		//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
		else
		{
			u16temp[0] = u16temp[0] - 0xd800 + 0x40;
			u16temp[1] -= 0xdc00;
			u32->data[rU32] = (u16temp[0] << 16) + u16temp[1];
		}
		rAs += chsize;
		rU32++;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);
}

cmtUint8 cmtU8chSize(cmtChar* ch)
{
	if (*ch < 0x80) return 1;
	if (*ch < 0xe0) return 2;
	if (*ch < 0xf0) return 3;
	return 4;
}

cmtUint64 cmtU8strSize(cmtChar* str)
{
	cmtUint64 size = 0;

	while (str[size]) size++;

	return size;
}

cmtUint64 cmtU8len(cmtU8str* str)
{
	cmtUint64 r = 0, len = 0;

	while (r < str->size)
	{
		if (str->data[r] < 0x80 || str->data[r] >= 0xc0) len++;
		r++;
	}

	return len;
}

cmtUint64 cmtU8toANSIsize(cmtU8str* u8, cmtChar* locale, cmtBool* err)
{
	cmtUint64 rU8 = 0, ASsize = 0;
	cmtInt8 chsize;
	cmtWchar u16temp[2];
	cmtChar AStemp[MB_LEN_MAX];
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, locale);

	while (rU8 < u8->size)
	{
		//'\0'
		if (!u8->data[rU8])
		{
			rU8++;
			ASsize++;
			continue;
		}

		if (u8->data[rU8] < 0x80)
		{
			u16temp[0] = u8->data[rU8];
			rU8++;
		}
		else if (u8->data[rU8] < 0xe0)
		{
			u16temp[0] = u8->data[rU8] & 0x1f;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 1] & 0x3f;
			rU8 += 2;
		}
		else if (u8->data[rU8] < 0xf0)
		{
			u16temp[0] = u8->data[rU8] & 0xf;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 1] & 0x3f;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 2] & 0x3f;
			rU8 += 3;
		}
		else
		{
			u16temp[0] = u8->data[rU8] & 0x7;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 1] & 0x3f;
			u16temp[0] <<= 2;
			u16temp[0] += u8->data[rU8 + 2] >> 4 & 0x3;
			u16temp[0] = u16temp[0] - 0x40 + 0xd800;
			u16temp[1] = u8->data[rU8 + 2] & 0xf;
			u16temp[1] <<= 6;
			u16temp[1] += u8->data[rU8 + 3] & 0x3f;
			u16temp[1] += 0xdc00;
			rU8 += 4;
		}

		chsize = wctomb(AStemp, *u16temp);
		if (chsize == -1)
		{
			//恢复locale
			setlocale(LC_ALL, CurLocaleCp);

			if (err) *err = TRUE;
			return 0;
		}

		ASsize += chsize;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);

	if (err) *err = FALSE;
	return ASsize;
}

cmtBool cmtU8toANSI(cmtU8str* u8, cmtANSIstr* ansi)
{
	cmtUint64 rU8 = 0, rAs = 0;
	cmtInt8 chsize;
	cmtWchar u16temp[2];
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, ansi->locale);

	while (rU8 < u8->size && rAs < ansi->size)
	{
		//'\0'
		if (!u8->data[rU8])
		{
			ansi->data[rAs] = 0;
			rU8++;
			rAs++;
			continue;
		}

		if (u8->data[rU8] < 0x80)
		{
			u16temp[0] = u8->data[rU8];
			rU8++;
		}
		else if (u8->data[rU8] < 0xe0)
		{
			u16temp[0] = u8->data[rU8] & 0x1f;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 1] & 0x3f;
			rU8 += 2;
		}
		else if (u8->data[rU8] < 0xf0)
		{
			u16temp[0] = u8->data[rU8] & 0xf;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 1] & 0x3f;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 2] & 0x3f;
			rU8 += 3;
		}
		else
		{
			u16temp[0] = u8->data[rU8] & 0x7;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 1] & 0x3f;
			u16temp[0] <<= 2;
			u16temp[0] += u8->data[rU8 + 2] >> 4 & 0x3;
			u16temp[0] = u16temp[0] - 0x40 + 0xd800;
			u16temp[1] = u8->data[rU8 + 2] & 0xf;
			u16temp[1] <<= 6;
			u16temp[1] += u8->data[rU8 + 3] & 0x3f;
			u16temp[1] += 0xdc00;
			rU8 += 4;
		}

		chsize = wctomb(ansi->data + rAs, *u16temp);
		if (chsize == -1)
		{
			//恢复locale
			setlocale(LC_ALL, CurLocaleCp);

			return TRUE;
		}

		rAs += chsize;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);

	return FALSE;
}

cmtUint64 cmtU8toU16size(cmtU8str* u8)
{
	cmtUint64 rU8 = 0, u16size = 0;

	while (rU8 < u8->size)
	{
		//'\0'
		if (!u8->data[rU8])
		{
			rU8++;
			u16size += 2;
			continue;
		}

		if (u8->data[rU8] < 0x80)
		{
			u16size += 2;
			rU8++;
		}
		else if (u8->data[rU8] < 0xe0)
		{
			u16size += 2;
			rU8 += 2;
		}
		else if (u8->data[rU8] < 0xf0)
		{
			u16size += 2;
			rU8 += 3;
		}
		else
		{
			u16size += 4;
			rU8 += 4;
		}
	}

	return u16size;
}

void cmtU8toU16(cmtU8str* u8, cmtU16str* u16)
{
	cmtUint64 rU8 = 0, rU16 = 0;
	cmtUint64 rU16max;
	cmtWchar u16temp[2];

	rU16max = u16->size / 2;
	while (rU8 < u8->size && rU16 < rU16max)
	{
		//'\0'
		if (!u8->data[rU8])
		{
			u16->data[rU16] = 0;
			rU8++;
			rU16++;
			continue;
		}

		if (u8->data[rU8] < 0x80)
		{
			u16->data[rU16] = u8->data[rU8];
			rU8++;
			rU16++;
		}
		else if (u8->data[rU8] < 0xe0)
		{
			u16temp[0] = u8->data[rU8] & 0x1f;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 1] & 0x3f;
			u16->data[rU16] = u16temp[0];
			rU8 += 2;
			rU16++;
		}
		else if (u8->data[rU8] < 0xf0)
		{
			u16temp[0] = u8->data[rU8] & 0xf;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 1] & 0x3f;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 2] & 0x3f;
			u16->data[rU16] = u16temp[0];
			rU8 += 3;
			rU16++;
		}
		else
		{
			u16temp[0] = u8->data[rU8] & 0x7;
			u16temp[0] <<= 6;
			u16temp[0] += u8->data[rU8 + 1] & 0x3f;
			u16temp[0] <<= 2;
			u16temp[0] += u8->data[rU8 + 2] >> 4 & 0x3;
			u16temp[0] = u16temp[0] - 0x40 + 0xd800;
			u16temp[1] = u8->data[rU8 + 2] & 0xf;
			u16temp[1] <<= 6;
			u16temp[1] += u8->data[rU8 + 3] & 0x3f;
			u16temp[1] += 0xdc00;
			u16->data[rU16] = u16temp[0];
			u16->data[rU16 + 1] = u16temp[1];
			rU8 += 4;
			rU16 += 2;
		}
	}
}

cmtUint64 cmtU8toU32size(cmtU8str* u8)
{
	return cmtU8len(u8) * 4;
}

void cmtU8toU32(cmtU8str* u8, cmtU32str* u32)
{
	cmtUint64 rU8 = 0, rU32 = 0;
	cmtUint64 rU32max;
	cmtFchar u32temp;

	rU32max = u32->size / 4;
	while (rU8 < u8->size && rU32 < rU32max)
	{
		//'\0'
		if (!u8->data[rU8])
		{
			u32->data[rU32] = 0;
			rU8++;
			rU32++;
			continue;
		}

		if (u8->data[rU8] < 0x80)
		{
			u32->data[rU32] = u8->data[rU8];
			rU8++;
		}
		else if (u8->data[rU8] < 0xe0)
		{
			u32temp = u8->data[rU8] & 0x1f;
			u32temp <<= 6;
			u32temp += u8->data[rU8 + 1] & 0x3f;
			u32->data[rU32] = u32temp;
			rU8 += 2;
		}
		else if (u8->data[rU8] < 0xf0)
		{
			u32temp = u8->data[rU8] & 0xf;
			u32temp <<= 6;
			u32temp += u8->data[rU8 + 1] & 0x3f;
			u32temp <<= 6;
			u32temp += u8->data[rU8 + 2] & 0x3f;
			u32->data[rU32] = u32temp;
			rU8 += 3;
		}
		else
		{
			u32temp = u8->data[rU8] & 0x7;
			u32temp <<= 6;
			u32temp += u8->data[rU8 + 1] & 0x3f;
			u32temp <<= 6;
			u32temp += u8->data[rU8 + 2] & 0x3f;
			u32temp <<= 6;
			u32temp += u8->data[rU8 + 3] & 0x3f;
			u32->data[rU32] = u32temp;
			rU8 += 4;
		}
		rU32++;
	}
}

cmtUint8 cmtU16chSize(cmtWchar* ch)
{
	//如果首字在保留区间外，那么绝对只有一个字
	if (*ch < CMT_UNICODE_RSV_START || *ch > CMT_UNICODE_RSV_END) return 2;
	//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
	else return 4;
}

cmtUint64 cmtU16strSize(cmtWchar* str)
{
	cmtUint64 r = 0;

	while (str[r]) r++;

	return r * 2;
}

cmtUint64 cmtU16len(cmtU16str* str)
{
	cmtUint64 r = 0, len = 0;
	cmtUint64 maxr;

	maxr = str->size / 2;
	while (r < maxr)
	{
		//如果首字在保留区间外，那么绝对只有一个字
		if (str->data[r] < CMT_UNICODE_RSV_START || str->data[r] > CMT_UNICODE_RSV_END) r++;
		//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
		else r += 2;
		len++;
	}

	return len;
}

cmtUint64 cmtU16toANSIsize(cmtU16str* u16, cmtChar* locale, cmtBool* err)
{
	cmtUint64 rU16 = 0, ASsize = 0;
	cmtUint64 rU16max;
	cmtChar AStemp[MB_LEN_MAX];
	cmtInt8 chsize;
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, locale);

	rU16max = u16->size / 2;
	while (rU16 < rU16max)
	{
		//'\0'
		if (!u16->data[rU16])
		{
			rU16++;
			ASsize++;
			continue;
		}

		chsize = wctomb(AStemp, u16->data[rU16]);
		if (chsize == -1)
		{
			//恢复locale
			setlocale(LC_ALL, CurLocaleCp);
			if (err) *err = TRUE;
			return 0;
		}

		//如果首字在保留区间外，那么绝对只有一个字
		if (u16->data[rU16] < CMT_UNICODE_RSV_START || u16->data[rU16] > CMT_UNICODE_RSV_END) rU16++;
		//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
		else return rU16 += 2;
		ASsize += chsize;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);
	if (err) *err = FALSE;
	return ASsize;
}

cmtBool cmtU16toANSI(cmtU16str* u16, cmtANSIstr* ansi)
{
	cmtUint64 rU16 = 0, rAs = 0;
	cmtUint64 rU16max;
	cmtInt8 chsize;
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, ansi->locale);

	rU16max = u16->size / 2;
	while (rU16 < rU16max && rAs < ansi->size)
	{
		//'\0'
		if (!u16->data[rU16])
		{
			ansi->data[rAs] = 0;
			rU16++;
			rAs++;
			continue;
		}

		chsize = wctomb(ansi->data + rAs, u16->data[rU16]);
		if (chsize == -1)
		{
			//恢复locale
			setlocale(LC_ALL, CurLocaleCp);
			return TRUE;
		}

		//如果首字在保留区间外，那么绝对只有一个字
		if (u16->data[rU16] < CMT_UNICODE_RSV_START || u16->data[rU16] > CMT_UNICODE_RSV_END) rU16++;
		//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
		else return rU16 += 2;
		rAs += chsize;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);
	return FALSE;
}

cmtUint64 cmtU16toU8size(cmtU16str* u16)
{
	cmtUint64 rU16 = 0, u8size = 0;
	cmtUint64 rU16max;

	rU16max = u16->size / 2;
	while (rU16 < rU16max)
	{
		//'\0'
		if (!u16->data[rU16])
		{
			rU16++;
			u8size++;
			continue;
		}

		//如果首字在保留区间外，那么绝对只有一个字
		if (u16->data[rU16] < CMT_UNICODE_RSV_START || u16->data[rU16] > CMT_UNICODE_RSV_END)
		{
			//[0,0x7f]
			if (u16->data[rU16] < 0x80) u8size++;
			//[0x80,0x07ff]
			else if (u16->data[rU16] < 0x800) u8size += 2;
			//[0x0800,0xffff]
			else u8size += 3;
			rU16++;
		}
		//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
		else
		{
			u8size += 4;
			rU16 += 2;
		}
	}

	return u8size;
}

void cmtU16toU8(cmtU16str* u16, cmtU8str* u8)
{
	cmtUint64 rU16 = 0, rU8 = 0;
	cmtUint64 rU16max;
	cmtWchar u16temp[2];

	rU16max = u16->size / 2;
	while (rU16 < rU16max && rU8 < u8->size)
	{
		//'\0'
		if (!u16->data[rU16])
		{
			u8->data[rU8] = 0;
			rU16++;
			rU8++;
			continue;
		}

		//如果首字在保留区间外，那么绝对只有一个字
		if (u16->data[rU16] < CMT_UNICODE_RSV_START || u16->data[rU16] > CMT_UNICODE_RSV_END)
		{
			u16temp[0] = u16->data[rU16];
			//[0,0x7f]
			if (u16temp[0] < 0x80)
			{
				u8->data[rU8] = (cmtUint8)u16temp[0];
				rU8++;
			}
			//[0x80,0x07ff]
			else if (u16temp[0] < 0x800)
			{
				//u8第二字节
				u8->data[rU8 + 1] = 0x80 + ((cmtUint8)u16temp[0] & 0x3f);
				//u8第一字节
				u16temp[0] >>= 6;
				u8->data[rU8] = 0xc0 + (cmtUint8)u16temp[0];
				rU8 += 2;
			}
			//[0x0800,0xffff]
			else
			{
				//u8第三字节
				u8->data[rU8 + 2] = 0x80 + ((cmtUint8)u16temp[0] & 0x3f);
				//u8第二字节
				u16temp[0] >>= 6;
				u8->data[rU8 + 1] = 0x80 + ((cmtUint8)u16temp[0] & 0x3f);
				//u8第一字节
				u16temp[0] >>= 6;
				u8->data[rU8] = 0xe0 + (cmtUint8)u16temp[0];
				rU8 += 3;
			}
			rU16++;
		}
		//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
		else
		{
			u16temp[0] = u16->data[rU16];
			u16temp[1] = u16->data[rU16 + 1];

			u16temp[0] = u16temp[0] - 0xd800 + 0x40;
			u16temp[1] -= 0xdc00;
			//u8第四字节
			u8->data[rU8 + 3] = 0x80 + ((cmtUint8)u16temp[1] & 0x3f);
			//u8第三字节
			u16temp[1] >>= 6;
			u8->data[rU8 + 2] = 0x80 + (cmtUint8)u16temp[1] + (((cmtUint8)u16temp[0] & 0x3) << 4);
			//u8第二字节
			u16temp[0] >>= 2;
			u8->data[rU8 + 1] = 0x80 + ((cmtUint8)u16temp[0] & 0x3f);
			//u8第一字节
			u16temp[0] >>= 6;
			u8->data[rU8] = 0xf0 + (cmtUint8)u16temp[0];
			rU8 += 4;

			rU16 += 2;
		}
	}
}

cmtUint64 cmtU16toU32size(cmtU16str* u16)
{
	return cmtU16len(u16) * 4;
}

void cmtU16toU32(cmtU16str* u16, cmtU32str* u32)
{
	cmtUint64 rU16 = 0, rU32 = 0;
	cmtUint64 rU16max, rU32max;
	cmtWchar u16temp[2];

	rU16max = u16->size / 2;
	rU32max = u32->size / 4;
	while (rU16 < rU16max && rU32 < rU32max)
	{
		//如果首字在保留区间外，那么绝对只有一个字
		if (u16->data[rU16] < CMT_UNICODE_RSV_START || u16->data[rU16] > CMT_UNICODE_RSV_END)
		{
			u32->data[rU32] = u16->data[rU16];
			rU16++;
		}
		//如果首字在保留区间内，那么绝对有两个字，范围为[0x010000,0x10ffff]
		else
		{
			u16temp[0] = u16->data[rU16];
			u16temp[1] = u16->data[rU16 + 1];

			u16temp[0] = u16temp[0] - 0xd800 + 0x40;
			u16temp[1] -= 0xdc00;
			u32->data[rU32] = ((cmtUint32)u16temp[0] << 10) + u16temp[1];
			rU16 += 2;
		}
		rU32++;
	}
}

cmtUint64 cmtU32strSize(cmtFchar* str)
{
	cmtUint64 r = 0;

	while (str[r]) r++;

	return r * 4;
}

cmtUint64 cmtU32toANSIsize(cmtU32str* u32, cmtChar* locale, cmtBool* err)
{
	cmtUint64 rU32 = 0, ASsize = 0;
	cmtUint64 rU32max;
	cmtInt8 chsize;
	cmtWchar u16temp[2];
	cmtChar AStemp[MB_LEN_MAX];
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, locale);

	rU32max = u32->size / 4;
	while (rU32 < rU32max)
	{
		//'\0'
		if (!u32->data[rU32])
		{
			rU32++;
			ASsize++;
			continue;
		}

		//基本平面，单字utf-16
		if (u32->data[rU32] < 0x10000)
		{
			u16temp[0] = (cmtUint16)u32->data[rU32];
			u16temp[1] = 0;
		}
		else
		{
			u16temp[1] = (cmtUint16)u32->data[rU32] & 0x3ff;
			u16temp[1] += 0xdc00;
			u16temp[0] = (cmtUint16)(u32->data[rU32] >> 10 & 0x3ff);
			u16temp[0] = u16temp[0] - 0x40 + 0xd800;
		}

		chsize = wctomb(AStemp, *u16temp);
		if (chsize == -1)
		{
			//恢复locale
			setlocale(LC_ALL, CurLocaleCp);

			if (err) *err = TRUE;
			return 0;
		}

		rU32++;
		ASsize += chsize;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);

	if (err) *err = FALSE;
	return ASsize;
}

cmtBool cmtU32toANSI(cmtU32str* u32, cmtANSIstr* ansi)
{
	cmtUint64 rU32 = 0, rAs = 0;
	cmtUint64 rU32max;
	cmtInt8 chsize;
	cmtWchar u16temp[2];
	cmtChar CurLocaleCp[CMT_LOCALE_MAX], * CurLocale;

	//保存当前locale
	memset(CurLocaleCp, 0, sizeof(CurLocaleCp));
	CurLocale = setlocale(LC_ALL, NULL);
	strncpy(CurLocaleCp, CurLocale, sizeof(CurLocaleCp) - 1);
	//设置locale
	setlocale(LC_ALL, ansi->locale);

	rU32max = u32->size / 4;
	while (rU32 < rU32max && rAs < ansi->size)
	{
		//'\0'
		if (!u32->data[rU32])
		{
			ansi->data[rAs] = 0;
			rU32++;
			rAs++;
			continue;
		}

		//基本平面，单字utf-16
		if (u32->data[rU32] < 0x10000)
		{
			u16temp[0] = (cmtUint16)u32->data[rU32];
			u16temp[1] = 0;
		}
		else
		{
			u16temp[1] = (cmtUint16)u32->data[rU32] & 0x3ff;
			u16temp[1] += 0xdc00;
			u16temp[0] = (cmtUint16)(u32->data[rU32] >> 10 & 0x3ff);
			u16temp[0] = u16temp[0] - 0x40 + 0xd800;
		}

		chsize = wctomb(ansi->data + rAs, *u16temp);
		if (chsize == -1)
		{
			//恢复locale
			setlocale(LC_ALL, CurLocaleCp);

			return TRUE;
		}

		rU32++;
		rAs += chsize;
	}

	//恢复locale
	setlocale(LC_ALL, CurLocaleCp);

	return FALSE;
}

cmtUint64 cmtU32toU8size(cmtU32str* u32)
{
	cmtUint64 rU32 = 0, u8size = 0;
	cmtUint64 rU32max;

	rU32max = u32->size / 4;
	while (rU32 < rU32max)
	{
		//[0,0x7f]
		if (u32->data[rU32] < 0x80) u8size++;
		//[0x80,0x07ff]
		else if (u32->data[rU32] < 0x800) u8size += 2;
		//[0x0800,0xffff]
		else if (u32->data[rU32] < 0x10000) u8size += 3;
		//[0x010000,0x10ffff]
		else u8size += 4;
		rU32++;
	}

	return u8size;
}

void cmtU32toU8(cmtU32str* u32, cmtU8str* u8)
{
	cmtUint64 rU32 = 0, rU8 = 0;
	cmtUint64 rU32max;
	cmtFchar u32temp;

	rU32max = u32->size / 4;
	while (rU32 < rU32max && rU8 < u8->size)
	{
		u32temp = u32->data[rU32];

		//[0,0x7f]
		if (u32temp < 0x80)
		{
			u8->data[rU8] = (cmtUint8)u32temp;
			rU8++;
		}
		//[0x80,0x07ff]
		else if (u32->data[rU32] < 0x800)
		{
			//u8第二字节
			u8->data[rU8 + 1] = 0x80 + ((cmtUint8)u32temp & 0x3f);
			//u8第一字节
			u32temp >>= 6;
			u8->data[rU8] = 0xc0 + (cmtUint8)u32temp;
			rU8 += 2;
		}
		//[0x0800,0xffff]
		else if (u32->data[rU32] < 0x10000)
		{
			//u8第三字节
			u8->data[rU8 + 2] = 0x80 + ((cmtUint8)u32temp & 0x3f);
			//u8第二字节
			u32temp >>= 6;
			u8->data[rU8 + 1] = 0x80 + ((cmtUint8)u32temp & 0x3f);
			//u8第一字节
			u32temp >>= 6;
			u8->data[rU8] = 0xe0 + (cmtUint8)u32temp;
			rU8 += 3;
		}
		//[0x010000,0x10ffff]
		else
		{
			//u8第四字节
			u8->data[rU8 + 3] = 0x80 + ((cmtUint8)u32temp & 0x3f);
			//u8第三字节
			u32temp >>= 6;
			u8->data[rU8 + 2] = 0x80 + ((cmtUint8)u32temp & 0x3f);
			//u8第二字节
			u32temp >>= 6;
			u8->data[rU8 + 1] = 0x80 + ((cmtUint8)u32temp & 0x3f);
			//u8第一字节
			u32temp >>= 6;
			u8->data[rU8] = 0xf0 + (cmtUint8)u32temp;
			rU8 += 4;
		}
		rU32++;
	}
}

cmtUint64 cmtU32toU16size(cmtU32str* u32)
{
	cmtUint64 rU32 = 0, u16size = 0;
	cmtUint64 rU32max;

	rU32max = u32->size / 4;
	while (rU32 < rU32max)
	{
		if (u32->data[rU32] < 0x10000) u16size += 2;
		else u16size += 4;
		rU32++;
	}

	return u16size;
}

void cmtU32toU16(cmtU32str* u32, cmtU16str* u16)
{
	cmtUint64 rU32 = 0, rU16 = 0;
	cmtUint64 rU32max, rU16max;
	cmtFchar u32temp;

	rU32max = u32->size / 4;
	rU16max = u16->size / 2;
	while (rU32 < rU32max && rU16 < rU16max)
	{
		u32temp = u32->data[rU32];
		if (u32temp < 0x10000)
		{
			u16->data[rU16] = (cmtUint16)u32temp;
			rU16++;
		}
		else
		{
			u16->data[rU16 + 1] = (cmtUint16)u32temp & 0x3ff;
			u16->data[rU16 + 1] += 0xdc00;
			u16->data[rU16] = (cmtUint16)(u32temp >> 10 & 0x3ff);
			u16->data[rU16] = u16->data[rU16] - 0x40 + 0xd800;
			rU16 += 2;
		}
		rU32++;
	}
}

void cmtAnlyFmt(cmtU8str* fmt, cmtFmtInfo* info, cmtUint64* ArgList)
{
	cmtUint64 rFmt = 0, rArg = 0;
	cmtU8str TempStr;

	//一、sign字段
	if (fmt->data[rFmt] == '+')
	{
		info->sign = TRUE;
		rFmt++;
	}
	else
		info->sign = FALSE;

	//二、padding字段
	//（一）align
	if (fmt->data[rFmt] == '-')
	{
		info->padding.align = TRUE;
		rFmt++;
	}
	else
		info->padding.align = FALSE;
	//（二）content
	if (fmt->data[rFmt] == '0')
	{
		info->padding.content = TRUE;
		rFmt++;
	}
	else
		info->padding.content = FALSE;
	//（三）length
	if (fmt->data[rFmt] == '*')
	{
		info->padding.length = ArgList[rArg];
		rArg++;
		rFmt++;
	}
	else
	{
		TempStr.data = fmt->data + rFmt;
		TempStr.size = fmt->size - rFmt;
		rFmt += cmtStrToDec(&TempStr, &info->padding.length);
	}

	//三、precision字段
	if (fmt->data[rFmt] == '.')
	{
		info->precision.enabled = TRUE;
		rFmt++;
		//（一）flag
		if (fmt->data[rFmt] == '=')
		{
			info->precision.flag = TRUE;
			rFmt++;
		}
		else
			info->precision.flag = FALSE;
		//（二）value
		if (fmt->data[rFmt] == '*')
		{
			info->precision.value = ArgList[rArg];
			rArg++;
			rFmt++;
		}
		else
		{
			TempStr.data = fmt->data + rFmt;
			TempStr.size = fmt->size - rFmt;
			rFmt += cmtStrToDec(&TempStr, &info->precision.value);
		}
	}
	else
		info->precision.enabled = FALSE;

	//四、iteration字段
	if (fmt->data[rFmt] == 'r')
	{
		info->iteration.enabled = TRUE;
		rFmt++;
		//（一）length
		if (fmt->data[rFmt] == '*')
		{
			info->iteration.length = ArgList[rArg];
			rArg++;
			rFmt++;
		}
		else
		{
			TempStr.data = fmt->data + rFmt;
			TempStr.size = fmt->size - rFmt;
			rFmt += cmtStrToDec(&TempStr, &info->iteration.length);
		}
		//（二）group size
		if (fmt->data[rFmt] == '-')
		{
			rFmt++;
			if (fmt->data[rFmt] == '*')
			{
				info->iteration.GroupSize = ArgList[rArg];
				rArg++;
				rFmt++;
			}
			else
			{
				TempStr.data = fmt->data + rFmt;
				TempStr.size = fmt->size - rFmt;
				rFmt += cmtStrToDec(&TempStr, &info->iteration.GroupSize);
			}
		}
		//（三）row size
		if (fmt->data[rFmt] == '-')
		{
			rFmt++;
			if (fmt->data[rFmt] == '*')
			{
				info->iteration.RowSize = ArgList[rArg];
				rArg++;
				rFmt++;
			}
			else
			{
				TempStr.data = fmt->data + rFmt;
				TempStr.size = fmt->size - rFmt;
				rFmt += cmtStrToDec(&TempStr, &info->iteration.RowSize);
			}
		}
	}
	else
		info->iteration.enabled = FALSE;

	//五、size字段
	if (fmt->data[rFmt] == 'h')
	{
		rFmt++;
		if (fmt->data[rFmt] == 'h') info->size = CMT_FMT_SIZE_HH;
		else info->size = CMT_FMT_SIZE_H;
	}
	else if (fmt->data[rFmt] == 'l')
	{
		rFmt++;
		if (fmt->data[rFmt] == 'l') info->size = CMT_FMT_SIZE_LL;
		else info->size = CMT_FMT_SIZE_L;
	}
	else
		info->size = CMT_FMT_SIZE_DEFAULT;

	//六、type字段
	info->type = fmt->data[fmt->size - 1];
}

cmtUint64 cmtStrToBin(cmtU8str* in, cmtUint64* out)
{
	cmtUint64 r = 0;

	*out = 0;
	while (r < in->size && in->data[r] >= '0' && in->data[r] <= '1')
	{
		*out *= 2;
		*out += in->data[r] - '0';
		r++;
	}

	return r;
}

cmtUint64 cmtStrToOct(cmtU8str* in, cmtUint64* out)
{
	cmtUint64 r = 0;

	*out = 0;
	while (r < in->size && in->data[r] >= '0' && in->data[r] <= '7')
	{
		*out *= 8;
		*out += in->data[r] - '0';
		r++;
	}

	return r;
}

cmtUint64 cmtStrToDec(cmtU8str* in, cmtUint64* out)
{
	cmtUint64 r = 0;

	*out = 0;
	if (in->size && in->data[0] == '-')
	{
		r++;
		while (r < in->size && in->data[r] >= '0' && in->data[r] <= '9')
		{
			*out *= 10;
			*out -= in->data[r] - '0';
			r++;
		}
	}
	else
	{
		if (in->size && in->data[0] == '+') r++;
		while (r < in->size && in->data[r] >= '0' && in->data[r] <= '9')
		{
			*out *= 10;
			*out += in->data[r] - '0';
			r++;
		}
	}

	return r;
}

cmtUint64 cmtStrToHex(cmtU8str* in, cmtUint64* out)
{
	cmtUint64 r = 0;

	*out = 0;
	while (r < in->size && (in->data[r] >= '0' && in->data[r] <= '9' ||
		in->data[r] >= 'a' && in->data[r] <= 'f' || in->data[r] >= 'A' && in->data[r] <= 'F'))
	{
		*out *= 16;
		if (in->data[r] <= '9') *out += in->data[r] - '0';
		else if (in->data[r] <= 'F') *out += in->data[r] - 'A' + 10;
		else *out += in->data[r] - 'a' + 10;
		r++;
	}

	return r;
}

cmtUint64 cmtStrToF32(cmtU8str* in, float* out)
{
	//num=sigf*mag
	float sigf = 0.0f;//有效数字部分（尾数）
	float mag = 0.0f;//倍数
	cmtUint64 r = 0;

	
}

//cmtUint64 cmtStrtoF64(cmtU8str* in, double* out)
//{
//	double integer = 0.0f, decimal = 0.0f;
//	cmtU8str DecStr;
//	cmtUint64 r = 0;
//
//	//整数部分
//	while (r < in->size && in->data[r] >= '0' && in->data[r] <= '9')
//	{
//		integer *= 10.0;
//		integer += in->data[r] - '0';
//		r++;
//	}
//	//小数部分
//	if (in->data[r] == '.')
//	{
//		//计算小数数据字符串大小
//		DecStr.data = in->data + r;
//		r++;
//		while (r < in->size && in->data[r] >= '0' && in->data[r] <= '9') r++;
//		DecStr.size = in->data + r - DecStr.data;
//		//转换
//		r = DecStr.size;
//		while (r > 0)
//		{
//			decimal += in->data[r] - '0';
//			decimal /= 10.0;
//			r--;
//		}
//		//使用的大小
//		r = DecStr.data + DecStr.size - in->data;
//	}
//
//	*out = integer + decimal;
//	return r;
//}
//
//cmtUint64 cmtBintoStrSize(cmtUint64 in)
//{
//	cmtUint64 size = 0;
//
//	while (in > 0)
//	{
//		in /= 2;
//		size++;
//	}
//
//	return size;
//}
//
//void cmtBintoStr(cmtUint64 in, cmtU8str* out, cmtUint64 digit)
//{
//	cmtUint64 r;
//
//	r = digit;
//	while (r > 0)
//	{
//		if (r <= out->size)
//			out->data[r - 1] = in % 2;
//		in /= 2;
//		r--;
//	}
//}
//
//cmtUint64 cmtOcttoStrSize(cmtUint64 in)
//{
//	cmtUint64 size = 0;
//
//	while (in > 0)
//	{
//		in /= 8;
//		size++;
//	}
//
//	return size;
//}
//
//void cmtOcttoStr(cmtUint64 in, cmtU8str* out, cmtUint64 digit)
//{
//	cmtUint64 r;
//
//	r = digit;
//	while (r > 0)
//	{
//		if (r <= out->size)
//			out->data[r - 1] = in % 8;
//		in /= 8;
//		r--;
//	}
//}
//
//cmtUint64 cmtDectoStrSize(cmtUint64 in)
//{
//	cmtUint64 size = 0;
//
//	while (in > 0)
//	{
//		in /= 10;
//		size++;
//	}
//
//	return size;
//}
//
//void cmtDectoStr(cmtUint64 in, cmtU8str* out, cmtUint64 digit)
//{
//	cmtUint64 r;
//
//	r = digit;
//	while (r > 0)
//	{
//		if (r <= out->size)
//			out->data[r - 1] = in % 10;
//		in /= 10;
//		r--;
//	}
//}
//
//cmtUint64 cmtHextoStrSize(cmtUint64 in)
//{
//	cmtUint64 size = 0;
//
//	while (in > 0)
//	{
//		in /= 16;
//		size++;
//	}
//
//	return size;
//}
//
//void cmtHextoStr(cmtUint64 in, cmtU8str* out, cmtUint64 digit, cmtBool cap)
//{
//	cmtUint64 r;
//	cmtChar ch;
//
//	r = digit;
//	while (r > 0)
//	{
//		if (r <= out->size)
//		{
//			ch = '0' + in % 16;
//			//大于9的字符转a-f
//			if (ch > '9')
//			{
//				if (cap) ch += 'A' - '9' - 1;
//				else ch += 'a' - '9' - 1;
//			}
//			out->data[r - 1] = ch;
//		}
//		in /= 16;
//		r--;
//	}
//}
//
//cmtInt64 cmtCalcPofdF32(float in)
//{
//	cmtInt64 pofd = 0;
//
//	if (in >= 1)
//	{
//		while (in >= 1)
//		{
//			in /= 10.0f;
//			pofd++;
//		}
//	}
//	else
//	{
//		while (in < 1)
//		{
//			in *= 10.0f;
//			pofd--;
//		}
//	}
//
//	return pofd;
//}
//
//cmtInt64 cmtCalcPofdF64(double in)
//{
//	cmtInt64 pofd = 0;
//
//	if (in >= 1)
//	{
//		while (in >= 1)
//		{
//			in /= 10.0;
//			pofd++;
//		}
//	}
//	else
//	{
//		while (in < 1)
//		{
//			in *= 10.0;
//			pofd--;
//		}
//	}
//
//	return pofd;
//}
//
//cmtUint64 cmtFltoStrSize(cmtInt64 pofd, cmtUint64 sigf)
//{
//	//结构：(整数数据)[.(小数数据)]
//	cmtU8str integer, decimal;
//
//	//一、计算各字串大小
//	//（一）整数数据字符串
//	if (pofd > 0) integer.size = pofd;
//	else integer.size = 1;
//	//（二）小数数据字符串
//	if (pofd > 0)
//	{
//		if (sigf > pofd) decimal.size = sigf - pofd + 1;
//		else decimal.size = 0;
//	}
//	else
//		decimal.size = sigf - pofd;
//
//	return integer.size + decimal.size;
//}
//
//void cmtF32toStr(float in, cmtU8str* out, cmtInt64 pofd, cmtUint64 sigf)
//{
//	//结构：(整数数据)[.(小数数据)]
//	cmtU8str integer, decimal;
//	cmtUint64 IntSize, DecSize;
//	cmtUint64 r;
//	float InCopy;
//	cmtUint64 TotalSize = 0;
//
//	//一、计算各部分数字个数
//	//（一）整数
//	if (pofd > 0) IntSize = pofd;
//	else IntSize = 1;
//	//大小限制
//	TotalSize += integer.size;
//	if (TotalSize > out->size)
//	{
//		integer.size = IntSize - (TotalSize - out->size);
//		decimal.size = 0;
//		goto T_1end;
//	}
//	else
//		integer.size = IntSize;
//	//（二）小数
//	if (pofd > 0)
//	{
//		if (sigf > pofd) DecSize = sigf - pofd + 1;
//		else DecSize = 0;
//	}
//	else
//		DecSize = sigf - pofd;
//	//大小限制
//	TotalSize += DecSize;
//	if (TotalSize > out->size)
//		decimal.size = DecSize - (TotalSize - out->size);
//	else
//		decimal.size = DecSize;
//
//	//二、计算各子字符串地址
//	//（一）计算大小
//	//1. 整数
//	TotalSize += IntSize;
//	if (TotalSize > out->size)
//	{
//		integer.size = IntSize - (TotalSize - out->size);
//		decimal.size = 0;
//		goto T_2_1end;
//	}
//	else
//		integer.size = IntSize;
//T_2_1end:
//	//（二）计算地址
//	integer.data = out->data;
//	decimal.data = integer.data + integer.size;
//
//	//三、构建字符串
//	//（一）整数数据字符串
//	InCopy = in;
//	r = integer.size;
//	while (r > 0)
//	{
//		//保留范围之外，填0
//		if (r > sigf)
//			integer.data[r - 1] = '0';
//		//最后一位保留的有效数字，四舍五入
//		else if (r == sigf)
//			integer.data[r - 1] = '0' + (cmtUint64)(InCopy + 0.5f) % 10;
//		//其他情况正常转换
//		else
//			integer.data[r - 1] = '0' + (cmtUint64)InCopy % 10;
//
//		InCopy /= 10.0f;
//		r--;
//	}
//	if (decimal.size)
//	{
//		decimal.data[0] = '.';
//		//剪掉整数部分以防指数上溢
//		in -= (cmtUint64)in;
//		//正常转换
//		in *= 10.0f;
//		r = 1;
//		while (r < decimal.size - 1)
//		{
//			decimal.data[r] = '0' + (cmtUint64)in % 10;
//			in *= 10.0f;
//			r++;
//		}
//		//最后一位需要四舍五入
//		if (r < decimal.size)
//			decimal.data[r] = '0' + (cmtUint64)(in + 0.5f) % 10;
//	}
//}
//
//void cmtF64toStr(double in, cmtU8str* out, cmtInt64 pofd, cmtUint64 sigf)
//{
//	//结构：(整数数据)[.(小数数据)]
//	cmtU8str integer, decimal;
//	cmtUint64 r;
//	cmtUint64 MaxAddr;//最大地址+1
//	double InCopy;
//
//	MaxAddr = out->data + out->size;
//
//	//一、计算各字串大小
//	//（一）整数数据字符串
//	if (pofd > 0) integer.size = pofd;
//	else integer.size = 1;
//	//（二）小数数据字符串
//	if (pofd > 0)
//	{
//		if (sigf > pofd) decimal.size = sigf - pofd + 1;
//		else decimal.size = 0;
//	}
//	else
//		decimal.size = sigf - pofd;
//
//	//二、计算各子字符串地址
//	integer.data = out->data;
//	decimal.data = integer.data + integer.size;
//
//	//三、构建字符串
//	//（一）整数数据字符串
//	InCopy = in;
//	r = integer.size;
//	while (r > 0)
//	{
//		//写入地址限制
//		if (integer.data + r <= MaxAddr)
//		{
//			//保留范围之外，填0
//			if (r > sigf)
//				integer.data[r - 1] = '0';
//			//最后一位保留的有效数字，四舍五入
//			else if (r == sigf)
//				integer.data[r - 1] = '0' + (cmtUint64)(InCopy + 0.5) % 10;
//			//其他情况正常转换
//			else
//				integer.data[r - 1] = '0' + (cmtUint64)InCopy % 10;
//		}
//		InCopy /= 10.0;
//		r--;
//	}
//	if (decimal.size)
//	{
//		if (decimal.data < MaxAddr)
//			decimal.data[0] = '.';
//		//剪掉整数部分以防指数上溢
//		in -= (cmtUint64)in;
//		//正常转换
//		in *= 10.0;
//		r = 1;
//		while (r < decimal.size - 1)
//		{
//			if (decimal.data + r < MaxAddr)
//				decimal.data[r] = '0' + (cmtUint64)in % 10;
//			in *= 10.0;
//			r++;
//		}
//		//最后一位需要四舍五入
//		if (decimal.data + r < MaxAddr)
//			decimal.data[r] = '0' + (cmtUint64)(in + 0.5) % 10;
//	}
//}
//
//cmtUint64 cmtFltoStrExSize(cmtInt64 pofd, cmtUint64 sigf)
//{
//	//结构：(整数数据)[.(小数数据)](e(指数符号)(指数数据))
//	cmtU8str decimal, exponent;
//
//	//一、计算各字串大小
//	//（一）小数数据字符串
//	if (sigf == 1) decimal.size = 0;
//	else decimal.size = sigf;
//	//（二）指数数据字符串
//	if (pofd > 0) exponent.size = 2 + cmtDectoStrSize(pofd - 1);
//	else exponent.size = 2 + cmtDectoStrSize(-pofd);
//
//	return 1 + decimal.size + exponent.size;
//}
//
//void cmtF32toStrEx(float in, cmtU8str* out, cmtInt64 pofd, cmtUint64 sigf, cmtBool cap)
//{
//	//结构：(整数数据)[.(小数数据)](e(指数符号)(指数数据))
//	cmtU8str decimal, exponent;
//	cmtChar integer, ExpSign;
//	cmtUint64 r;
//	cmtUint64 MaxAddr;//最大地址+1
//	cmtU8str TempStr;
//	float InCopy;
//
//	MaxAddr = out->data + out->size;
//
//	//一、计算各字串大小
//	//（一）小数数据字符串
//	if (sigf == 1) decimal.size = 0;
//	else decimal.size = sigf;
//	//（二）指数数据字符串
//	if (pofd > 0) exponent.size = 2 + cmtDectoStrSize(pofd - 1);
//	else exponent.size = 2 + cmtDectoStrSize(-pofd);
//
//	//二、计算各子字符串地址
//	decimal.data = out->data + 1;
//	exponent.data = decimal.data + decimal.size;
//
//	//三、构建字符串
//	//（一）通用
//	if (exponent.data < MaxAddr)
//	{
//		if (cap) exponent.data[0] = 'E';
//		else exponent.data[0] = 'e';
//	}
//	//（二）pofd < 0
//	if (pofd < 0)
//	{
//		//1. 指数数据字符串
//		if (exponent.data + 1 < MaxAddr) exponent.data[1] = '-';
//		TempStr.data = exponent.data + 2;
//		if (exponent.data + exponent.size > MaxAddr)
//			TempStr.size = MaxAddr - (cmtUint64)TempStr.data;
//		else
//			TempStr.size = exponent.size;
//		cmtDectoStr(-pofd, &TempStr);
//		//2. 整数数据字符串
//		//左移
//		while (pofd < 0)
//		{
//			in *= 10.0f;
//			pofd++;
//		}
//		//转换
//		if (out->size)
//		{
//			if (decimal.size) out->data[0] = '0' + (cmtUint64)in;
//			//没有小数数据，整数数据需要四舍五入
//			else out->data[0] = '0' + (cmtUint64)(in + 0.5f);
//		}
//		//3. 小数数据字符串
//		if (decimal.size)
//		{
//			in *= 10.0f;
//			if (decimal.data < MaxAddr) decimal.data[0] = '.';
//			r = 1;
//			while (r < sigf - 1)
//			{
//				if (decimal.data + r < MaxAddr)
//					decimal.data[r] = '0' + (cmtUint64)in % 10;
//				in *= 10.0f;
//				r++;
//			}
//			//最后一位需要四舍五入
//			if (decimal.data + r < MaxAddr)
//				decimal.data[r] = '0' + (cmtUint64)(in + 0.5f) % 10;
//		}
//	}
//	//（三）0 < pofd < sigf
//	else if (pofd < sigf)
//	{
//		//1. 指数数据字符串
//		if (exponent.data + 1 < MaxAddr) exponent.data[1] = '+';
//		TempStr.data = exponent.data + 2;
//		if (exponent.data + exponent.size > MaxAddr)
//			TempStr.size = MaxAddr - (cmtUint64)TempStr.data;
//		else
//			TempStr.size = exponent.size;
//		cmtDectoStr(pofd - 1, &TempStr);
//		//2. 左侧
//		InCopy = in;
//		//（1）小数数据字符串
//		if (decimal.data < MaxAddr) decimal.data[0] = '.';
//		r = pofd - 1;
//		while (r > 0)
//		{
//			if (decimal.data + r < MaxAddr)
//				decimal.data[r] = '0' + (cmtUint64)InCopy % 10;
//			InCopy /= 10.0f;
//			r--;
//		}
//		//（2）整数数据字符串
//		if (out->size) out->data[0] = '0' + (cmtUint64)InCopy % 10;
//		//3. 右侧
//		//剪掉整数部分以防指数上溢
//		in -= (cmtUint64)in;
//		//小数数据字符串
//		in *= 10.0f;
//		r = pofd;
//		while (r < sigf - 1)
//		{
//			if (decimal.data + r < MaxAddr)
//				decimal.data[r] = '0' + (cmtUint64)in % 10;
//			in *= 10.0f;
//			r++;
//		}
//		//最后一位需要四舍五入
//		if (decimal.data + r < MaxAddr)
//			decimal.data[r] = '0' + (cmtUint64)(in + 0.5f) % 10;
//	}
//	//（四）pofd >= sigf
//	else
//	{
//		//1. 指数数据字符串
//		if (exponent.data + 1 < MaxAddr) exponent.data[1] = '+';
//		TempStr.data = exponent.data + 2;
//		if (exponent.data + exponent.size > MaxAddr)
//			TempStr.size = MaxAddr - (cmtUint64)TempStr.data;
//		else
//			TempStr.size = exponent.size;
//		cmtDectoStr(pofd - 1, &TempStr);
//		//2. 小数数据字符串
//		//移位
//		while (pofd - sigf > 0)
//		{
//			in /= 10.0f;
//			pofd--;
//		}
//		//转换
//		if (decimal.data < MaxAddr) decimal.data[0] = '.';
//		//末位需要四舍五入
//		if (decimal.data + decimal.size <= MaxAddr)
//			decimal.data[decimal.size - 1] = '0' + (cmtUint64)(in + 0.5f) % 10;
//		in /= 10.0f;
//		r = decimal.size - 2;
//		while (r > 0)
//		{
//			if (decimal.data + r < MaxAddr)
//				decimal.data[r] = '0' + (cmtUint64)in % 10;
//			in /= 10.0f;
//			r--;
//		}
//		//3. 整数数据字符串
//		if (out->size) out->data[0] = '0' + (cmtUint64)in;
//	}
//}
//
//void cmtF64toStrEx(double in, cmtU8str* out, cmtInt64 pofd, cmtUint64 sigf, cmtBool cap)
//{
//	//结构：(整数数据)[.(小数数据)](e(指数符号)(指数数据))
//	cmtU8str decimal, exponent;
//	cmtChar integer, ExpSign;
//	cmtUint64 r;
//	cmtUint64 MaxAddr;//最大地址+1
//	cmtU8str TempStr;
//	double InCopy;
//
//	MaxAddr = out->data + out->size;
//
//	//一、计算各字串大小
//	//（一）小数数据字符串
//	if (sigf == 1) decimal.size = 0;
//	else decimal.size = sigf;
//	//（二）指数数据字符串
//	if (pofd > 0) exponent.size = 2 + cmtDectoStrSize(pofd - 1);
//	else exponent.size = 2 + cmtDectoStrSize(-pofd);
//
//	//二、计算各子字符串地址
//	decimal.data = out->data + 1;
//	exponent.data = decimal.data + decimal.size;
//
//	//三、构建字符串
//	//（一）通用
//	if (exponent.data < MaxAddr)
//	{
//		if (cap) exponent.data[0] = 'E';
//		else exponent.data[0] = 'e';
//	}
//	//（二）pofd < 0
//	if (pofd < 0)
//	{
//		//1. 指数数据字符串
//		if (exponent.data + 1 < MaxAddr) exponent.data[1] = '-';
//		TempStr.data = exponent.data + 2;
//		if (exponent.data + exponent.size > MaxAddr)
//			TempStr.size = MaxAddr - (cmtUint64)TempStr.data;
//		else
//			TempStr.size = exponent.size;
//		cmtDectoStr(-pofd, &TempStr);
//		//2. 整数数据字符串
//		//左移
//		while (pofd < 0)
//		{
//			in *= 10.0;
//			pofd++;
//		}
//		//转换
//		if (out->size)
//		{
//			if (decimal.size) out->data[0] = '0' + (cmtUint64)in;
//			//没有小数数据，整数数据需要四舍五入
//			else out->data[0] = '0' + (cmtUint64)(in + 0.5);
//		}
//		//3. 小数数据字符串
//		if (decimal.size)
//		{
//			in *= 10.0;
//			if (decimal.data < MaxAddr) decimal.data[0] = '.';
//			r = 1;
//			while (r < sigf - 1)
//			{
//				if (decimal.data + r < MaxAddr)
//					decimal.data[r] = '0' + (cmtUint64)in % 10;
//				in *= 10.0;
//				r++;
//			}
//			//最后一位需要四舍五入
//			if (decimal.data + r < MaxAddr)
//				decimal.data[r] = '0' + (cmtUint64)(in + 0.5) % 10;
//		}
//	}
//	//（三）0 < pofd < sigf
//	else if (pofd < sigf)
//	{
//		//1. 指数数据字符串
//		if (exponent.data + 1 < MaxAddr) exponent.data[1] = '+';
//		TempStr.data = exponent.data + 2;
//		if (exponent.data + exponent.size > MaxAddr)
//			TempStr.size = MaxAddr - (cmtUint64)TempStr.data;
//		else
//			TempStr.size = exponent.size;
//		cmtDectoStr(pofd - 1, &TempStr);
//		//2. 左侧
//		InCopy = in;
//		//（1）小数数据字符串
//		if (decimal.data < MaxAddr) decimal.data[0] = '.';
//		r = pofd - 1;
//		while (r > 0)
//		{
//			if (decimal.data + r < MaxAddr)
//				decimal.data[r] = '0' + (cmtUint64)InCopy % 10;
//			InCopy /= 10.0;
//			r--;
//		}
//		//（2）整数数据字符串
//		if (out->size) out->data[0] = '0' + (cmtUint64)InCopy % 10;
//		//3. 右侧
//		//剪掉整数部分以防指数上溢
//		in -= (cmtUint64)in;
//		//小数数据字符串
//		in *= 10.0;
//		r = pofd;
//		while (r < sigf - 1)
//		{
//			if (decimal.data + r < MaxAddr)
//				decimal.data[r] = '0' + (cmtUint64)in % 10;
//			in *= 10.0;
//			r++;
//		}
//		//最后一位需要四舍五入
//		if (decimal.data + r < MaxAddr)
//			decimal.data[r] = '0' + (cmtUint64)(in + 0.5) % 10;
//	}
//	//（四）pofd >= sigf
//	else
//	{
//		//1. 指数数据字符串
//		if (exponent.data + 1 < MaxAddr) exponent.data[1] = '+';
//		TempStr.data = exponent.data + 2;
//		if (exponent.data + exponent.size > MaxAddr)
//			TempStr.size = MaxAddr - (cmtUint64)TempStr.data;
//		else
//			TempStr.size = exponent.size;
//		cmtDectoStr(pofd - 1, &TempStr);
//		//2. 小数数据字符串
//		//移位
//		while (pofd - sigf > 0)
//		{
//			in /= 10.0;
//			pofd--;
//		}
//		//转换
//		if (decimal.data < MaxAddr) decimal.data[0] = '.';
//		//末位需要四舍五入
//		if (decimal.data + decimal.size <= MaxAddr)
//			decimal.data[decimal.size - 1] = '0' + (cmtUint64)(in + 0.5) % 10;
//		in /= 10.0;
//		r = decimal.size - 2;
//		while (r > 0)
//		{
//			if (decimal.data + r < MaxAddr)
//				decimal.data[r] = '0' + (cmtUint64)in % 10;
//			in /= 10.0;
//			r--;
//		}
//		//3. 整数数据字符串
//		if (out->size) out->data[0] = '0' + (cmtUint64)in;
//	}
//}

//void cmtSprintf(cmtU8str* out, cmtU8str* format, ...)
//{
//	//通用
//	cmtUint64* ArgList;//参数栈是会64位/32位对齐的
//	cmtUint64 rArg = 0;
//	cmtUint64 rFmt = 0, rOut = 0;
//	//格式控制字符串分析使用
//	cmtU8str FmtStr;
//	cmtFmtInfo FmtInfo;
//	//构建输出字符串使用
//	cmtU8str FrontPad, DataStr, BackPad;
//	cmtUint64 rOutStr;
//	cmtComVal value1, value2;
//	cmtChar SignChar;
//	//浮点类型使用
//	//position of first digit（第一位十进制数字位置）
//	//小数点左边第一位为1，小数点右边第一位为-1，左增右减
//	cmtInt64 pofd;
//
//	ArgList = &format + 1;
//
//	while (rFmt < format->size && rOut < out->size)
//	{
//		//可能是格式控制字符串
//		if (format->data[rFmt] == '%')
//		{
//			//是个百分号而已 %%
//			if (format->data[rFmt + 1] == '%')
//			{
//				out->data[rOut++] = '%';
//				rFmt++;
//			}
//			//真正的格式控制字符串
//			else
//			{
//				//一、提取格式控制字符串
//				rFmt++;
//				FmtStr.data = format->data + rFmt;
//				FmtStr.size = rFmt;
//				//找type字段
//				while (rFmt < format->size && format->data[rFmt] != 'b' && format->data[rFmt] != 'B' &&
//					format->data[rFmt] != 'o' && format->data[rFmt] != 'O' && format->data[rFmt] != 'd' && format->data[rFmt] != 'D' &&
//					format->data[rFmt] != 'u' && format->data[rFmt] != 'U' && format->data[rFmt] != 'x' && format->data[rFmt] != 'X' &&
//					format->data[rFmt] != 'f' && format->data[rFmt] != 'F' && format->data[rFmt] != 'e' && format->data[rFmt] != 'E' &&
//					format->data[rFmt] != 'g' && format->data[rFmt] != 'G' && format->data[rFmt] != 'c' && format->data[rFmt] != 'C' &&
//					format->data[rFmt] != 's' && format->data[rFmt] != 'S') rFmt++;
//				if (rFmt == format->size) break;
//				rFmt++;
//				FmtStr.size = rFmt - FmtStr.size;
//
//				//二、分析格式控制字符串
//				cmtAnlyFmt(&FmtStr, &FmtInfo, ArgList);
//
//				//三、根据分析结果构建输出字符串
//				FrontPad.data = out->data + rOut;
//				if (FmtInfo.type == 'b' || FmtInfo.type == 'B')
//				{
//					//结构：(前置padding)(数据)(后置padding)
//					//一、数据处理
//					//从参数中载入指定长度的数据
//					value1.u64 = 0;
//					if (FmtInfo.size == CMT_FMT_SIZE_HH) value1.u64 = (cmtUint8)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_H) value1.u64 = (cmtUint16)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_LL) value1.u64 = (cmtUint64)ArgList[rArg];
//					else value1.u64 = (cmtUint32)ArgList[rArg];
//
//					//二、计算数据字符串大小
//					//没有指定precision：输出所有数字
//					if (!FmtInfo.precision.enabled)
//						DataStr.size = cmtBintoStrSize(value1.u64);
//					DataStr.size = FmtInfo.precision.value;
//
//					//三、各子字符串定位
//					//（一）计算各子字符串大小
//					//需要padding
//					if (FmtInfo.padding.length > DataStr.size)
//					{
//						//左对齐
//						if (FmtInfo.padding.align)
//						{
//							FrontPad.size = 0;
//							BackPad.size = FmtInfo.padding.length - DataStr.size;
//						}
//						//右对齐
//						else
//						{
//							FrontPad.size = FmtInfo.padding.length - DataStr.size;
//							BackPad.size = 0;
//						}
//					}
//					//不需要padding
//					else
//					{
//						FrontPad.size = 0;
//						BackPad.size = 0;
//					}
//					//（二）计算各子字符串地址
//					DataStr.data = FrontPad.data + FrontPad.size;
//					BackPad.data = DataStr.data + DataStr.size;
//
//					//四、构建字符串
//					//（一）前置padding
//					if (FrontPad.size)
//					{
//						rOutStr = 0;
//						//填0
//						if (FmtInfo.padding.content)
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = '0';
//								rOutStr++;
//							}
//						}
//						//填空格
//						else
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//					}
//					//（二）数据字符串
//					rOutStr = 0;
//					while (rOutStr < DataStr.size)
//					{
//						//倒着写入（最低位在最右边）
//						DataStr.data[DataStr.size - rOutStr - 1] = value1.u64 % 2 + '0';
//						value1.u64 /= 2;
//						rOutStr++;
//					}
//					//（三）后置padding
//					if (BackPad.size)
//					{
//						rOutStr = 0;
//						while (rOutStr < BackPad.size)
//						{
//							BackPad.data[rOutStr] = ' ';
//							rOutStr++;
//						}
//					}
//
//					//五、rOut增量
//					rOut += FrontPad.size + DataStr.size + BackPad.size;
//				}
//				else if (FmtInfo.type == 'o' || FmtInfo.type == 'O')
//				{
//					//结构：(前置padding)(数据)(后置padding)
//					//一、数据处理
//					//从参数中载入指定长度的数据
//					value1.u64 = 0;
//					if (FmtInfo.size == CMT_FMT_SIZE_HH) value1.u64 = (cmtUint8)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_H) value1.u64 = (cmtUint16)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_LL) value1.u64 = (cmtUint64)ArgList[rArg];
//					else value1.u64 = (cmtUint32)ArgList[rArg];
//
//					//二、计算数据字符串大小
//					//没有指定precision：计算能输出所有数字的precision
//					if (!FmtInfo.precision.enabled)
//					{
//						value2 = value1;
//						FmtInfo.precision.value = 0;
//						while (value2.u64)
//						{
//							FmtInfo.precision.value++;
//							value2.u64 /= 8;
//						}
//					}
//					DataStr.size = FmtInfo.precision.value;
//
//					//三、各子字符串定位
//					//（一）计算各子字符串大小
//					//需要padding
//					if (FmtInfo.padding.length > DataStr.size)
//					{
//						//左对齐
//						if (FmtInfo.padding.align)
//						{
//							FrontPad.size = 0;
//							BackPad.size = FmtInfo.padding.length - DataStr.size;
//						}
//						//右对齐
//						else
//						{
//							FrontPad.size = FmtInfo.padding.length - DataStr.size;
//							BackPad.size = 0;
//						}
//					}
//					//不需要padding
//					else
//					{
//						FrontPad.size = 0;
//						BackPad.size = 0;
//					}
//					//（二）计算各子字符串地址
//					DataStr.data = FrontPad.data + FrontPad.size;
//					BackPad.data = DataStr.data + DataStr.size;
//
//					//四、构建字符串
//					//（一）前置padding
//					if (FrontPad.size)
//					{
//						rOutStr = 0;
//						//填0
//						if (FmtInfo.padding.content)
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = '0';
//								rOutStr++;
//							}
//						}
//						//填空格
//						else
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//					}
//					//（二）数据字符串
//					rOutStr = 0;
//					while (rOutStr < DataStr.size)
//					{
//						//倒着写入（最低位在最右边）
//						DataStr.data[DataStr.size - rOutStr - 1] = value1.u64 % 8 + '0';
//						value1.u64 /= 8;
//						rOutStr++;
//					}
//					//（三）后置padding
//					if (BackPad.size)
//					{
//						rOutStr = 0;
//						while (rOutStr < BackPad.size)
//						{
//							BackPad.data[rOutStr] = ' ';
//							rOutStr++;
//						}
//					}
//
//					//五、rOut增量
//					rOut += FrontPad.size + DataStr.size + BackPad.size;
//				}
//				else if (FmtInfo.type == 'd' || FmtInfo.type == 'D')
//				{
//					//结构：(偏左符号)(前置padding)(偏右符号)(数据)(后置padding)
//					//一、数据处理
//					//（一）从参数中载入指定长度的数据
//					value1.i64 = 0;
//					if (FmtInfo.size == CMT_FMT_SIZE_HH) value1.i64 = (cmtInt8)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_H) value1.i64 = (cmtInt16)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_LL) value1.i64 = (cmtInt64)ArgList[rArg];
//					else value1.i64 = (cmtInt32)ArgList[rArg];
//					//（二）负数记录并取绝对值
//					if (value1.i64 < 0)
//					{
//						SignChar = '-';
//						value1.i64 = -value1.i64;
//						FmtInfo.sign = TRUE;//打开符号显示
//					}
//					else
//						SignChar = '+';
//
//					//二、计算数据字符串大小
//					//没有指定precision：计算能输出所有数字的precision
//					if (!FmtInfo.precision.enabled)
//					{
//						value2 = value1;
//						FmtInfo.precision.value = 0;
//						while (value2.u64)
//						{
//							FmtInfo.precision.value++;
//							value2.u64 /= 10;
//						}
//					}
//					DataStr.size = FmtInfo.precision.value;
//
//					//三、各子字符串定位
//					//（一）计算各子字符串大小
//					//需要padding
//					if (FmtInfo.padding.length > FmtInfo.sign + DataStr.size)
//					{
//						//左对齐
//						if (FmtInfo.padding.align)
//						{
//							FrontPad.size = 0;
//							BackPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size;
//						}
//						//右对齐
//						else
//						{
//							FrontPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size;
//							BackPad.size = 0;
//						}
//					}
//					//不需要padding
//					else
//					{
//						FrontPad.size = 0;
//						BackPad.size = 0;
//					}
//					//（二）计算各子字符串地址
//					DataStr.data = FrontPad.data + FrontPad.size + FmtInfo.sign;
//					BackPad.data = DataStr.data + DataStr.size;
//					//（三）根据符号位置调整前置padding位置，并预填符号
//					if (FmtInfo.sign)
//					{
//						//符号偏左
//						if (FrontPad.size && FmtInfo.padding.content)
//						{
//							FrontPad.data++;
//							*(FrontPad.data - 1) = SignChar;
//						}
//						//符号偏右
//						else
//							*(DataStr.data - 1) = SignChar;
//					}
//
//					//四、构建字符串
//					//（一）前置padding
//					if (FrontPad.size)
//					{
//						rOutStr = 0;
//						//填0
//						if (FmtInfo.padding.content)
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = '0';
//								rOutStr++;
//							}
//						}
//						//填空格
//						else
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//					}
//					//（二）数据字符串
//					rOutStr = 0;
//					while (rOutStr < DataStr.size)
//					{
//						//倒着写入（最低位在最右边）
//						DataStr.data[DataStr.size - rOutStr - 1] = value1.u64 % 10 + '0';
//						value1.u64 /= 10;
//						rOutStr++;
//					}
//					//（三）后置padding
//					if (BackPad.size)
//					{
//						rOutStr = 0;
//						while (rOutStr < BackPad.size)
//						{
//							BackPad.data[rOutStr] = ' ';
//							rOutStr++;
//						}
//					}
//
//					//五、rOut增量
//					rOut += FrontPad.size + FmtInfo.sign + DataStr.size + BackPad.size;
//				}
//				else if (FmtInfo.type == 'u' || FmtInfo.type == 'U')
//				{
//					//结构：(前置padding)(数据)(后置padding)
//					//一、数据处理
//					//从参数中载入指定长度的数据
//					value1.u64 = 0;
//					if (FmtInfo.size == CMT_FMT_SIZE_HH) value1.u64 = (cmtUint8)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_H) value1.u64 = (cmtUint16)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_LL) value1.u64 = (cmtUint64)ArgList[rArg];
//					else value1.u64 = (cmtUint32)ArgList[rArg];
//
//					//二、计算数据字符串大小
//					//没有指定precision：计算能输出所有数字的precision
//					if (!FmtInfo.precision.enabled)
//					{
//						value2 = value1;
//						FmtInfo.precision.value = 0;
//						while (value2.u64)
//						{
//							FmtInfo.precision.value++;
//							value2.u64 /= 10;
//						}
//					}
//					DataStr.size = FmtInfo.precision.value;
//
//					//三、各子字符串定位
//					//（一）计算各子字符串大小
//					//需要padding
//					if (FmtInfo.padding.length > DataStr.size)
//					{
//						//左对齐
//						if (FmtInfo.padding.align)
//						{
//							FrontPad.size = 0;
//							BackPad.size = FmtInfo.padding.length - DataStr.size;
//						}
//						//右对齐
//						else
//						{
//							FrontPad.size = FmtInfo.padding.length - DataStr.size;
//							BackPad.size = 0;
//						}
//					}
//					//不需要padding
//					else
//					{
//						FrontPad.size = 0;
//						BackPad.size = 0;
//					}
//					//（二）计算各子字符串地址
//					DataStr.data = FrontPad.data + FrontPad.size;
//					BackPad.data = DataStr.data + DataStr.size;
//
//					//四、构建字符串
//					//（一）前置padding
//					if (FrontPad.size)
//					{
//						rOutStr = 0;
//						//填0
//						if (FmtInfo.padding.content)
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = '0';
//								rOutStr++;
//							}
//						}
//						//填空格
//						else
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//					}
//					//（二）数据字符串
//					rOutStr = 0;
//					while (rOutStr < DataStr.size)
//					{
//						//倒着写入（最低位在最右边）
//						DataStr.data[DataStr.size - rOutStr - 1] = value1.u64 % 10 + '0';
//						value1.u64 /= 10;
//						rOutStr++;
//					}
//					//（三）后置padding
//					if (BackPad.size)
//					{
//						rOutStr = 0;
//						while (rOutStr < BackPad.size)
//						{
//							BackPad.data[rOutStr] = ' ';
//							rOutStr++;
//						}
//					}
//
//					//五、rOut增量
//					rOut += FrontPad.size + DataStr.size + BackPad.size;
//				}
//				else if (FmtInfo.type == 'x')
//				{
//					//结构：(前置padding)(数据)(后置padding)
//					//一、数据处理
//					//从参数中载入指定长度的数据
//					value1.u64 = 0;
//					if (FmtInfo.size == CMT_FMT_SIZE_HH) value1.u64 = (cmtUint8)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_H) value1.u64 = (cmtUint16)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_LL) value1.u64 = (cmtUint64)ArgList[rArg];
//					else value1.u64 = (cmtUint32)ArgList[rArg];
//
//					//二、计算数据字符串大小
//					//没有指定precision：计算能输出所有数字的precision
//					if (!FmtInfo.precision.enabled)
//					{
//						value2 = value1;
//						FmtInfo.precision.value = 0;
//						while (value2.u64)
//						{
//							FmtInfo.precision.value++;
//							value2.u64 /= 16;
//						}
//					}
//					DataStr.size = FmtInfo.precision.value;
//
//					//三、各子字符串定位
//					//（一）计算各子字符串大小
//					//需要padding
//					if (FmtInfo.padding.length > DataStr.size)
//					{
//						//左对齐
//						if (FmtInfo.padding.align)
//						{
//							FrontPad.size = 0;
//							BackPad.size = FmtInfo.padding.length - DataStr.size;
//						}
//						//右对齐
//						else
//						{
//							FrontPad.size = FmtInfo.padding.length - DataStr.size;
//							BackPad.size = 0;
//						}
//					}
//					//不需要padding
//					else
//					{
//						FrontPad.size = 0;
//						BackPad.size = 0;
//					}
//					//（二）计算各子字符串地址
//					DataStr.data = FrontPad.data + FrontPad.size;
//					BackPad.data = DataStr.data + DataStr.size;
//
//					//四、构建字符串
//					//（一）前置padding
//					if (FrontPad.size)
//					{
//						rOutStr = 0;
//						//填0
//						if (FmtInfo.padding.content)
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = '0';
//								rOutStr++;
//							}
//						}
//						//填空格
//						else
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//					}
//					//（二）数据字符串
//					rOutStr = 0;
//					while (rOutStr < DataStr.size)
//					{
//						//倒着写入（最低位在最右边）
//						DataStr.data[DataStr.size - rOutStr - 1] = value1.u64 % 16 + '0';
//						//大于9的字符转a-f
//						if (DataStr.data[DataStr.size - rOutStr - 1] > '9')
//							DataStr.data[DataStr.size - rOutStr - 1] += 'a' - '9' - 1;
//						value1.u64 /= 16;
//						rOutStr++;
//					}
//					//（三）后置padding
//					if (BackPad.size)
//					{
//						rOutStr = 0;
//						while (rOutStr < BackPad.size)
//						{
//							BackPad.data[rOutStr] = ' ';
//							rOutStr++;
//						}
//					}
//
//					//五、rOut增量
//					rOut += FrontPad.size + DataStr.size + BackPad.size;
//				}
//				else if (FmtInfo.type == 'X')
//				{
//					//结构：(前置padding)(数据)(后置padding)
//					//一、数据处理
//					//从参数中载入指定长度的数据
//					value1.u64 = 0;
//					if (FmtInfo.size == CMT_FMT_SIZE_HH) value1.u64 = (cmtUint8)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_H) value1.u64 = (cmtUint16)ArgList[rArg];
//					else if (FmtInfo.size == CMT_FMT_SIZE_LL) value1.u64 = (cmtUint64)ArgList[rArg];
//					else value1.u64 = (cmtUint32)ArgList[rArg];
//
//					//二、计算数据字符串大小
//					//没有指定precision：计算能输出所有数字的precision
//					if (!FmtInfo.precision.enabled)
//					{
//						value2 = value1;
//						FmtInfo.precision.value = 0;
//						while (value2.u64)
//						{
//							FmtInfo.precision.value++;
//							value2.u64 /= 16;
//						}
//					}
//					DataStr.size = FmtInfo.precision.value;
//
//					//三、各子字符串定位
//					//（一）计算各子字符串大小
//					//需要padding
//					if (FmtInfo.padding.length > DataStr.size)
//					{
//						//左对齐
//						if (FmtInfo.padding.align)
//						{
//							FrontPad.size = 0;
//							BackPad.size = FmtInfo.padding.length - DataStr.size;
//						}
//						//右对齐
//						else
//						{
//							FrontPad.size = FmtInfo.padding.length - DataStr.size;
//							BackPad.size = 0;
//						}
//					}
//					//不需要padding
//					else
//					{
//						FrontPad.size = 0;
//						BackPad.size = 0;
//					}
//					//（二）计算各子字符串地址
//					DataStr.data = FrontPad.data + FrontPad.size;
//					BackPad.data = DataStr.data + DataStr.size;
//
//					//四、构建字符串
//					//（一）前置padding
//					if (FrontPad.size)
//					{
//						rOutStr = 0;
//						//填0
//						if (FmtInfo.padding.content)
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = '0';
//								rOutStr++;
//							}
//						}
//						//填空格
//						else
//						{
//							while (rOutStr < FrontPad.size)
//							{
//								FrontPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//					}
//					//（二）数据字符串
//					rOutStr = 0;
//					while (rOutStr < DataStr.size)
//					{
//						//倒着写入（最低位在最右边）
//						DataStr.data[DataStr.size - rOutStr - 1] = value1.u64 % 16 + '0';
//						//大于9的字符转A-F
//						if (DataStr.data[DataStr.size - rOutStr - 1] > '9')
//							DataStr.data[DataStr.size - rOutStr - 1] += 'A' - '9' - 1;
//						value1.u64 /= 16;
//						rOutStr++;
//					}
//					//（三）后置padding
//					if (BackPad.size)
//					{
//						rOutStr = 0;
//						while (rOutStr < BackPad.size)
//						{
//							BackPad.data[rOutStr] = ' ';
//							rOutStr++;
//						}
//					}
//
//					//五、rOut增量
//					rOut += FrontPad.size + DataStr.size + BackPad.size;
//				}
//				else if (FmtInfo.type == 'f' || FmtInfo.type == 'F')
//				{
//					//结构：(偏左符号)(前置padding)(偏右符号)(整数数据)(.(小数数据))(后置padding)
//					//单双精度要分开
//					if (FmtInfo.size == CMT_FMT_SIZE_L || FmtInfo.size == CMT_FMT_SIZE_LL)
//					{
//						//一、数据处理
//						//（一）从参数中载入指定长度的数据
//						value1.f64 = *((double*)ArgList + rArg);
//						//负数记录并取绝对值
//						if (value1.f64 < 0)
//						{
//							SignChar = '-';
//							value1.f64 = -value1.f64;
//							FmtInfo.sign = TRUE;//打开符号显示
//						}
//						else
//							SignChar = '+';
//						//（二）计算pofd（第一位十进制数字位置）
//						value2 = value1;
//						pofd = 0;
//						if (value2.f64 >= 1)
//						{
//							while (value2.f64 >= 1)
//							{
//								value2.f64 /= 10.0;
//								pofd++;
//							}
//						}
//						else
//						{
//							while (value2.f64 < 1)
//							{
//								value2.f64 *= 10.0;
//								pofd--;
//							}
//						}
//
//						//二、计算数据字符串大小
//					cSp_llf_step2:
//						//（一）计算整数数据字符串大小
//						//没有指定precision：默认保留6位小数
//						if (!FmtInfo.precision.enabled)
//						{
//							FmtInfo.precision.flag = FALSE;
//							FmtInfo.precision.value = 6;
//						}
//						//计算整数数据字符串大小
//						if (pofd > 0) DataStr.size = pofd;
//						else DataStr.size = 1;
//						//（二）计算小数数据字符串大小
//						//有效数字模式
//						if (FmtInfo.precision.flag)
//						{
//							if (pofd > 0)
//							{
//								if (FmtInfo.precision.value > pofd) DecDataStr.size = FmtInfo.precision.value - pofd + 1;
//								else DecDataStr.size = 0;
//							}
//							else
//								DecDataStr.size = FmtInfo.precision.value - pofd;
//						}
//						//小数位数模式
//						else
//							DecDataStr.size = FmtInfo.precision.value + 1;
//
//						//三、各子字符串定位
//						//（一）计算各子字符串大小
//						//需要padding
//						if (FmtInfo.padding.length > FmtInfo.sign + DataStr.size + DecDataStr.size)
//						{
//							//左对齐
//							if (FmtInfo.padding.align)
//							{
//								FrontPad.size = 0;
//								BackPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size;
//							}
//							//右对齐
//							else
//							{
//								FrontPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size;
//								BackPad.size = 0;
//							}
//						}
//						//不需要padding
//						else
//						{
//							FrontPad.size = 0;
//							BackPad.size = 0;
//						}
//						//（二）计算各子字符串地址
//						DataStr.data = FrontPad.data + FrontPad.size + FmtInfo.sign;
//						DecDataStr.data = DataStr.data + DataStr.size;
//						BackPad.data = DecDataStr.data + DecDataStr.size;
//						//（三）根据符号位置调整前置padding位置，并预填符号
//						if (FmtInfo.sign)
//						{
//							//符号偏左
//							if (FrontPad.size && FmtInfo.padding.content)
//							{
//								FrontPad.data++;
//								*(FrontPad.data - 1) = SignChar;
//							}
//							//符号偏右
//							else
//								*(DataStr.data - 1) = SignChar;
//						}
//
//						//四、构建字符串
//						//（一）前置padding
//						if (FrontPad.size)
//						{
//							rOutStr = 0;
//							//填0
//							if (FmtInfo.padding.content)
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = '0';
//									rOutStr++;
//								}
//							}
//							//填空格
//							else
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = ' ';
//									rOutStr++;
//								}
//							}
//						}
//						//（二）整数数据字符串
//						value2 = value1;
//						rOutStr = 0;
//						while (rOutStr < DataStr.size)
//						{
//							//倒着写入（最低位在最右边）
//							//有效数字模式
//							if (FmtInfo.precision.flag)
//							{
//								//保留范围之外，填0
//								if (DataStr.size - rOutStr > FmtInfo.precision.value)
//									DataStr.data[DataStr.size - rOutStr - 1] = '0';
//								//最后一位保留的有效数字，四舍五入
//								else if (DataStr.size - rOutStr == FmtInfo.precision.value)
//									DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)(value2.f64 + 0.5) % 10;
//								//其他情况正常转换
//								else
//									DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)value2.f64 % 10;
//							}
//							//小数位数模式
//							else
//							{
//								//如果个位就是最后一位（不保留小数位），个位需要四舍五入
//								if (!rOutStr && !FmtInfo.precision.value)
//									DataStr.data[DataStr.size - 1] = '0' + (cmtUint64)(value2.f64 + 0.5) % 10;
//								//其他情况正常转换
//								else
//									DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)value2.f64 % 10;
//							}
//							value2.f64 /= 10;
//							rOutStr++;
//						}
//						//（三）小数数据字符串
//						if (DecDataStr.size)
//						{
//							DecDataStr.data[0] = '.';
//							//剪掉整数部分以防指数上溢
//							value1.f64 -= (cmtUint64)value1.f64;
//							rOutStr = 1;
//							while (rOutStr < DecDataStr.size - 1)
//							{
//								value1.f64 *= 10;
//								//正着写入
//								DecDataStr.data[rOutStr] = '0' + (cmtUint64)value1.f64 % 10;
//								rOutStr++;
//							}
//							//最后一位需要带四舍五入
//							DecDataStr.data[rOutStr] = '0' + (cmtUint64)(value1.f64 + 0.5) % 10;
//						}
//						//（四）后置padding
//						if (BackPad.size)
//						{
//							rOutStr = 0;
//							while (rOutStr < BackPad.size)
//							{
//								BackPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//
//						//五、rOut增量
//						rOut += FrontPad.size + FmtInfo.sign + DataStr.size + DecDataStr.size + BackPad.size;
//					}
//					else
//					{
//						//一、数据处理
//						//（一）从参数中载入指定长度的数据
//						//全都升级成double了
//						value1.f32 = (float)*((double*)ArgList + rArg);
//						//负数记录并取绝对值
//						if (value1.f32 < 0)
//						{
//							SignChar = '-';
//							value1.f32 = -value1.f32;
//							FmtInfo.sign = TRUE;//打开符号显示
//						}
//						else
//							SignChar = '+';
//						//（二）计算pofd（第一位十进制数字位置）
//						value2 = value1;
//						pofd = 0;
//						if (value2.f32 >= 1)
//						{
//							while (value2.f32 >= 1)
//							{
//								value2.f32 /= 10.0;
//								pofd++;
//							}
//						}
//						else
//						{
//							while (value2.f32 < 1)
//							{
//								value2.f32 *= 10.0;
//								pofd--;
//							}
//						}
//
//						//二、计算数据字符串大小
//					cSp_hhf_step2:
//						//（一）计算整数数据字符串大小
//						//没有指定precision：默认保留6位小数
//						if (!FmtInfo.precision.enabled)
//						{
//							FmtInfo.precision.flag = FALSE;
//							FmtInfo.precision.value = 6;
//						}
//						//计算整数数据字符串大小
//						if (pofd > 0) DataStr.size = pofd;
//						else DataStr.size = 1;
//						//（二）计算小数数据字符串大小
//						//有效数字模式
//						if (FmtInfo.precision.flag)
//						{
//							if (pofd > 0)
//							{
//								if (FmtInfo.precision.value > pofd) DecDataStr.size = FmtInfo.precision.value - pofd + 1;
//								else DecDataStr.size = 0;
//							}
//							else
//								DecDataStr.size = FmtInfo.precision.value - pofd;
//						}
//						//小数位数模式
//						else
//							DecDataStr.size = FmtInfo.precision.value + 1;
//
//						//三、各子字符串定位
//						//（一）计算各子字符串大小
//						//需要padding
//						if (FmtInfo.padding.length > FmtInfo.sign + DataStr.size + DecDataStr.size)
//						{
//							//左对齐
//							if (FmtInfo.padding.align)
//							{
//								FrontPad.size = 0;
//								BackPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size;
//							}
//							//右对齐
//							else
//							{
//								FrontPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size;
//								BackPad.size = 0;
//							}
//						}
//						//不需要padding
//						else
//						{
//							FrontPad.size = 0;
//							BackPad.size = 0;
//						}
//						//（二）计算各子字符串地址
//						DataStr.data = FrontPad.data + FrontPad.size + FmtInfo.sign;
//						DecDataStr.data = DataStr.data + DataStr.size;
//						BackPad.data = DecDataStr.data + DecDataStr.size;
//						//（三）根据符号位置调整前置padding位置，并预填符号
//						if (FmtInfo.sign)
//						{
//							//符号偏左
//							if (FrontPad.size && FmtInfo.padding.content)
//							{
//								FrontPad.data++;
//								*(FrontPad.data - 1) = SignChar;
//							}
//							//符号偏右
//							else
//								*(DataStr.data - 1) = SignChar;
//						}
//
//						//四、构建字符串
//						//（一）前置padding
//						if (FrontPad.size)
//						{
//							rOutStr = 0;
//							//填0
//							if (FmtInfo.padding.content)
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = '0';
//									rOutStr++;
//								}
//							}
//							//填空格
//							else
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = ' ';
//									rOutStr++;
//								}
//							}
//						}
//						//（二）整数数据字符串
//						value2 = value1;
//						rOutStr = 0;
//						while (rOutStr < DataStr.size)
//						{
//							//倒着写入（最低位在最右边）
//							//有效数字模式
//							if (FmtInfo.precision.flag)
//							{
//								//保留范围之外，填0
//								if (DataStr.size - rOutStr > FmtInfo.precision.value)
//									DataStr.data[DataStr.size - rOutStr - 1] = '0';
//								//最后一位保留的有效数字，四舍五入
//								else if (DataStr.size - rOutStr == FmtInfo.precision.value)
//									DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)(value2.f32 + 0.5) % 10;
//								//其他情况正常转换
//								else
//									DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)value2.f32 % 10;
//							}
//							//小数位数模式
//							else
//							{
//								//如果个位就是最后一位（不保留小数位），个位需要四舍五入
//								if (!rOutStr && !FmtInfo.precision.value)
//									DataStr.data[DataStr.size - 1] = '0' + (cmtUint64)(value2.f32 + 0.5) % 10;
//								//其他情况正常转换
//								else
//									DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)value2.f32 % 10;
//							}
//							value2.f32 /= 10;
//							rOutStr++;
//						}
//						//（三）小数数据字符串
//						if (DecDataStr.size)
//						{
//							DecDataStr.data[0] = '.';
//							//剪掉整数部分以防指数上溢
//							value1.f32 -= (cmtUint64)value1.f32;
//							rOutStr = 1;
//							while (rOutStr < DecDataStr.size - 1)
//							{
//								value1.f32 *= 10;
//								//正着写入
//								DecDataStr.data[rOutStr] = '0' + (cmtUint64)value1.f32 % 10;
//								rOutStr++;
//							}
//							//最后一位需要带四舍五入
//							DecDataStr.data[rOutStr] = '0' + (cmtUint64)(value1.f32 + 0.5) % 10;
//						}
//						//（四）后置padding
//						if (BackPad.size)
//						{
//							rOutStr = 0;
//							while (rOutStr < BackPad.size)
//							{
//								BackPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//
//						//五、rOut增量
//						rOut += FrontPad.size + FmtInfo.sign + DataStr.size + DecDataStr.size + BackPad.size;
//					}
//				}
//				else if (FmtInfo.type == 'e')
//				{
//					//结构：(偏左符号)(前置padding)(偏右符号)(整数数据)(.(小数数据))(e(指数符号)(指数数据))(后置padding)
//					//单双精度要分开
//					if (FmtInfo.size == CMT_FMT_SIZE_L || FmtInfo.size == CMT_FMT_SIZE_LL)
//					{
//						//一、数据处理
//						//（一）从参数中载入指定长度的数据
//						value1.f64 = *((double*)ArgList + rArg);
//						//负数记录并取绝对值
//						if (value1.f64 < 0)
//						{
//							SignChar = '-';
//							value1.f64 = -value1.f64;
//							FmtInfo.sign = TRUE;//打开符号显示
//						}
//						else
//							SignChar = '+';
//						//（二）计算pofd（第一位十进制数字位置）
//						value2 = value1;
//						pofd = 0;
//						if (value2.f64 >= 1)
//						{
//							while (value2.f64 >= 1)
//							{
//								value2.f64 /= 10.0;
//								pofd++;
//							}
//						}
//						else
//						{
//							while (value2.f64 < 1)
//							{
//								value2.f64 *= 10.0;
//								pofd--;
//							}
//						}
//						//（三）计算指数数据字符串大小
//						exponent = pofd - 1;
//						value2.i64 = exponent;
//						if (value2.i64) ExpStr.size = 2;
//						else ExpStr.size = 3;
//						while (value2.i64)
//						{
//							ExpStr.size++;
//							value2.i64 /= 10;
//						}
//
//						//二、计算数据字符串大小
//					cSp_lle_step2:
//						//（一）计算整数数据字符串大小
//						//没有指定precision：默认保留6位小数
//						if (!FmtInfo.precision.enabled)
//						{
//							FmtInfo.precision.flag = FALSE;
//							FmtInfo.precision.value = 6;
//						}
//						//有效数字转小数点
//						if (FmtInfo.precision.flag)
//							FmtInfo.precision.value--;
//						//整数数据字符串大小始终为1
//						DataStr.size = 1;
//						//（二）计算小数数据字符串大小
//						if (FmtInfo.precision.value)
//							DecDataStr.size = FmtInfo.precision.value + 1;
//						else
//							DecDataStr.size = 0;
//
//						//三、各子字符串定位
//						//（一）计算各子字符串大小
//						//需要padding
//						if (FmtInfo.padding.length > FmtInfo.sign + DataStr.size + DecDataStr.size + ExpStr.size)
//						{
//							//左对齐
//							if (FmtInfo.padding.align)
//							{
//								FrontPad.size = 0;
//								BackPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size - ExpStr.size;
//							}
//							//右对齐
//							else
//							{
//								FrontPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size - ExpStr.size;
//								BackPad.size = 0;
//							}
//						}
//						//不需要padding
//						else
//						{
//							FrontPad.size = 0;
//							BackPad.size = 0;
//						}
//						//（二）计算各子字符串地址
//						DataStr.data = FrontPad.data + FrontPad.size + FmtInfo.sign;
//						DecDataStr.data = DataStr.data + DataStr.size;
//						ExpStr.data = DecDataStr.data + DecDataStr.size;
//						BackPad.data = DecDataStr.data + ExpStr.size;
//						//（三）根据符号位置调整前置padding位置，并预填符号
//						if (FmtInfo.sign)
//						{
//							//符号偏左
//							if (FrontPad.size && FmtInfo.padding.content)
//							{
//								FrontPad.data++;
//								*(FrontPad.data - 1) = SignChar;
//							}
//							//符号偏右
//							else
//								*(DataStr.data - 1) = SignChar;
//						}
//
//						//四、构建字符串
//						//（一）前置padding
//						if (FrontPad.size)
//						{
//							rOutStr = 0;
//							//填0
//							if (FmtInfo.padding.content)
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = '0';
//									rOutStr++;
//								}
//							}
//							//填空格
//							else
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = ' ';
//									rOutStr++;
//								}
//							}
//						}
//						//（二）数据字符串
//						//1. 原始数据首位数字在小数点右边
//						if (pofd < 0)
//						{
//							//一、过位（去掉前面的0，让首位数字移到个位）
//							while (pofd < 0)
//							{
//								value1.f64 *= 10;
//								pofd++;
//							}
//							//二、整数数据字符串
//							if (DecDataStr.size)
//								DataStr.data[0] = '0' + (cmtUint64)value1.f64 % 10;
//							//如果没有小数部分，整数位需要四舍五入
//							else
//								DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)(value1.f64 + 0.5) % 10;
//							value1.f64 *= 10;
//							//三、小数数据字符串
//							if (DecDataStr.size)
//							{
//								DecDataStr.data[0] = '.';
//								rOutStr = 1;
//								while (rOutStr < DecDataStr.size - 1)
//								{
//									//正着写入（从左往右）
//									DecDataStr.data[rOutStr] = '0' + (cmtUint64)value1.f64 % 10;
//									value1.f64 *= 10;
//									rOutStr++;
//								}
//								//最后一位需要四舍五入
//								DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f64 + 0.5) % 10;
//							}
//						}
//						//2. 原始数据末位有效数字在小数点左边
//						else if (pofd > FmtInfo.precision.value)
//						{
//							//一、过位（去掉无效数字）
//							while (pofd > 1 + FmtInfo.precision.value)
//							{
//								value1.f64 /= 10;
//								pofd--;
//							}
//							//二、小数数据字符串
//							if (DecDataStr.size)
//							{
//								DecDataStr.data[0] = '.';
//								//最后一位需要四舍五入
//								DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f64 + 0.5) % 10;
//								value1.f64 /= 10;
//								rOutStr = 1;
//								while (rOutStr < DecDataStr.size - 1)
//								{
//									//倒着写入（从右往左）
//									DecDataStr.data[DecDataStr.size - rOutStr - 1] = '0' + (cmtUint64)value1.f64 % 10;
//									value1.f64 /= 10;
//									rOutStr++;
//								}
//							}
//							//三、整数数据字符串
//							if (DecDataStr.size)
//								DataStr.data[0] = '0' + (cmtUint64)value1.f64 % 10;
//							//如果没有小数部分，整数位需要四舍五入
//							else
//								DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)(value1.f64 + 0.5) % 10;
//						}
//						//3. 原始数据有效数字横跨小数点
//						else
//						{
//							//一、原始数据整数部分
//							//（一）小数数据字符串
//							DecDataStr.data[0] = '.';
//							value2 = value1;
//							rOutStr = 1;
//							while (rOutStr < pofd)
//							{
//								//倒着写入（从右往左）
//								DecDataStr.data[pofd - rOutStr] = '0' + (cmtUint64)value2.f64 % 10;
//								value2.f64 /= 10;
//								rOutStr++;
//							}
//							//（二）整数数据字符串
//							DataStr.data[0] = '0' + (cmtUint64)value2.f64 % 10;
//							//二、原始数据小数部分
//							//（一）小数数据字符串
//							//剪掉整数部分以防指数上溢
//							value1.f64 -= (cmtUint64)value1.f64;
//							value1.f64 *= 10;
//							while (rOutStr < DecDataStr.size - 1)
//							{
//								//正着写入（从左往右）
//								DecDataStr.data[rOutStr] = '0' + (cmtUint64)value1.f64 % 10;
//								value1.f64 *= 10;
//								rOutStr++;
//							}
//							//最后一位需要四舍五入
//							DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f64 + 0.5) % 10;
//						}
//						//（四）指数数据字符串
//						ExpStr.data[0] = 'e';
//						if (exponent >= 0) ExpStr.data[1] = '+';
//						else
//						{
//							ExpStr.data[1] = '-';
//							exponent = -exponent;
//						}
//						rOutStr = 0;
//						while (rOutStr < ExpStr.size - 2)
//						{
//							//倒着写入（从右往左）
//							ExpStr.data[ExpStr.size - rOutStr - 1] = '0' + exponent % 10;
//							exponent /= 10;
//							rOutStr++;
//						}
//						//（五）后置padding
//						if (BackPad.size)
//						{
//							rOutStr = 0;
//							while (rOutStr < BackPad.size)
//							{
//								BackPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//					}
//					else
//					{
//						//一、数据处理
//						//（一）从参数中载入指定长度的数据
//						value1.f32 = (float)*((double*)ArgList + rArg);
//						//负数记录并取绝对值
//						if (value1.f32 < 0)
//						{
//							SignChar = '-';
//							value1.f32 = -value1.f32;
//							FmtInfo.sign = TRUE;//打开符号显示
//						}
//						else
//							SignChar = '+';
//						//（二）计算pofd（第一位十进制数字位置）
//						value2 = value1;
//						pofd = 0;
//						if (value2.f32 >= 1)
//						{
//							while (value2.f32 >= 1)
//							{
//								value2.f32 /= 10.0;
//								pofd++;
//							}
//						}
//						else
//						{
//							while (value2.f32 < 1)
//							{
//								value2.f32 *= 10.0;
//								pofd--;
//							}
//						}
//						//（三）计算指数数据字符串大小
//						exponent = pofd - 1;
//						value2.i64 = exponent;
//						if (value2.i64) ExpStr.size = 2;
//						else ExpStr.size = 3;
//						while (value2.i64)
//						{
//							ExpStr.size++;
//							value2.i64 /= 10;
//						}
//
//						//二、计算数据字符串大小
//					cSp_hhe_step2:
//						//（一）计算整数数据字符串大小
//						//没有指定precision：默认保留6位小数
//						if (!FmtInfo.precision.enabled)
//						{
//							FmtInfo.precision.flag = FALSE;
//							FmtInfo.precision.value = 6;
//						}
//						//有效数字转小数点
//						if (FmtInfo.precision.flag)
//							FmtInfo.precision.value--;
//						//整数数据字符串大小始终为1
//						DataStr.size = 1;
//						//（二）计算小数数据字符串大小
//						if (FmtInfo.precision.value)
//							DecDataStr.size = FmtInfo.precision.value + 1;
//						else
//							DecDataStr.size = 0;
//
//						//三、各子字符串定位
//						//（一）计算各子字符串大小
//						//需要padding
//						if (FmtInfo.padding.length > FmtInfo.sign + DataStr.size + DecDataStr.size + ExpStr.size)
//						{
//							//左对齐
//							if (FmtInfo.padding.align)
//							{
//								FrontPad.size = 0;
//								BackPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size - ExpStr.size;
//							}
//							//右对齐
//							else
//							{
//								FrontPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size - ExpStr.size;
//								BackPad.size = 0;
//							}
//						}
//						//不需要padding
//						else
//						{
//							FrontPad.size = 0;
//							BackPad.size = 0;
//						}
//						//（二）计算各子字符串地址
//						DataStr.data = FrontPad.data + FrontPad.size + FmtInfo.sign;
//						DecDataStr.data = DataStr.data + DataStr.size;
//						ExpStr.data = DecDataStr.data + DecDataStr.size;
//						BackPad.data = DecDataStr.data + ExpStr.size;
//						//（三）根据符号位置调整前置padding位置，并预填符号
//						if (FmtInfo.sign)
//						{
//							//符号偏左
//							if (FrontPad.size && FmtInfo.padding.content)
//							{
//								FrontPad.data++;
//								*(FrontPad.data - 1) = SignChar;
//							}
//							//符号偏右
//							else
//								*(DataStr.data - 1) = SignChar;
//						}
//
//						//四、构建字符串
//						//（一）前置padding
//						if (FrontPad.size)
//						{
//							rOutStr = 0;
//							//填0
//							if (FmtInfo.padding.content)
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = '0';
//									rOutStr++;
//								}
//							}
//							//填空格
//							else
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = ' ';
//									rOutStr++;
//								}
//							}
//						}
//						//（二）数据字符串
//						//1. 原始数据首位数字在小数点右边
//						if (pofd < 0)
//						{
//							//一、过位（去掉前面的0，让首位数字移到个位）
//							while (pofd < 0)
//							{
//								value1.f32 *= 10;
//								pofd++;
//							}
//							//二、整数数据字符串
//							if (DecDataStr.size)
//								DataStr.data[0] = '0' + (cmtUint64)value1.f32 % 10;
//							//如果没有小数部分，整数位需要四舍五入
//							else
//								DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)(value1.f32 + 0.5) % 10;
//							value1.f32 *= 10;
//							//三、小数数据字符串
//							if (DecDataStr.size)
//							{
//								DecDataStr.data[0] = '.';
//								rOutStr = 1;
//								while (rOutStr < DecDataStr.size - 1)
//								{
//									//正着写入（从左往右）
//									DecDataStr.data[rOutStr] = '0' + (cmtUint64)value1.f32 % 10;
//									value1.f32 *= 10;
//									rOutStr++;
//								}
//								//最后一位需要四舍五入
//								DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f32 + 0.5) % 10;
//							}
//						}
//						//2. 原始数据末位有效数字在小数点左边
//						else if (pofd > FmtInfo.precision.value)
//						{
//							//一、过位（去掉无效数字）
//							while (pofd > 1 + FmtInfo.precision.value)
//							{
//								value1.f32 /= 10;
//								pofd--;
//							}
//							//二、小数数据字符串
//							if (DecDataStr.size)
//							{
//								DecDataStr.data[0] = '.';
//								//最后一位需要四舍五入
//								DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f32 + 0.5) % 10;
//								value1.f32 /= 10;
//								rOutStr = 1;
//								while (rOutStr < DecDataStr.size - 1)
//								{
//									//倒着写入（从右往左）
//									DecDataStr.data[DecDataStr.size - rOutStr - 1] = '0' + (cmtUint64)value1.f32 % 10;
//									value1.f32 /= 10;
//									rOutStr++;
//								}
//							}
//							//三、整数数据字符串
//							if (DecDataStr.size)
//								DataStr.data[0] = '0' + (cmtUint64)value1.f32 % 10;
//							//如果没有小数部分，整数位需要四舍五入
//							else
//								DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)(value1.f32 + 0.5) % 10;
//						}
//						//3. 原始数据有效数字横跨小数点
//						else
//						{
//							//一、原始数据整数部分
//							//（一）小数数据字符串
//							DecDataStr.data[0] = '.';
//							value2 = value1;
//							rOutStr = 1;
//							while (rOutStr < pofd)
//							{
//								//倒着写入（从右往左）
//								DecDataStr.data[pofd - rOutStr] = '0' + (cmtUint64)value2.f32 % 10;
//								value2.f32 /= 10;
//								rOutStr++;
//							}
//							//（二）整数数据字符串
//							DataStr.data[0] = '0' + (cmtUint64)value2.f32 % 10;
//							//二、原始数据小数部分
//							//（一）小数数据字符串
//							//剪掉整数部分以防指数上溢
//							value1.f32 -= (cmtUint64)value1.f32;
//							value1.f32 *= 10;
//							while (rOutStr < DecDataStr.size - 1)
//							{
//								//正着写入（从左往右）
//								DecDataStr.data[rOutStr] = '0' + (cmtUint64)value1.f32 % 10;
//								value1.f32 *= 10;
//								rOutStr++;
//							}
//							//最后一位需要四舍五入
//							DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f32 + 0.5) % 10;
//						}
//						//（四）指数数据字符串
//						ExpStr.data[0] = 'e';
//						if (exponent >= 0) ExpStr.data[1] = '+';
//						else
//						{
//							ExpStr.data[1] = '-';
//							exponent = -exponent;
//						}
//						rOutStr = 0;
//						while (rOutStr < ExpStr.size - 2)
//						{
//							//倒着写入（从右往左）
//							ExpStr.data[ExpStr.size - rOutStr - 1] = '0' + exponent % 10;
//							exponent /= 10;
//							rOutStr++;
//						}
//						//（五）后置padding
//						if (BackPad.size)
//						{
//							rOutStr = 0;
//							while (rOutStr < BackPad.size)
//							{
//								BackPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//					}
//
//					//五、rOut增量
//					rOut += FrontPad.size + FmtInfo.sign + DataStr.size + DecDataStr.size + ExpStr.size + BackPad.size;
//				}
//				else if (FmtInfo.type == 'E')
//				{
//					//单双精度要分开
//					if (FmtInfo.size == CMT_FMT_SIZE_L || FmtInfo.size == CMT_FMT_SIZE_LL)
//					{
//						//一、数据处理
//						//（一）从参数中载入指定长度的数据
//						value1.f64 = *((double*)ArgList + rArg);
//						//负数记录并取绝对值
//						if (value1.f64 < 0)
//						{
//							SignChar = '-';
//							value1.f64 = -value1.f64;
//							FmtInfo.sign = TRUE;//打开符号显示
//						}
//						else
//							SignChar = '+';
//						//（二）计算pofd（第一位十进制数字位置）
//						value2 = value1;
//						pofd = 0;
//						if (value2.f64 >= 1)
//						{
//							while (value2.f64 >= 1)
//							{
//								value2.f64 /= 10.0;
//								pofd++;
//							}
//						}
//						else
//						{
//							while (value2.f64 < 1)
//							{
//								value2.f64 *= 10.0;
//								pofd--;
//							}
//						}
//						//（三）计算指数数据字符串大小
//						exponent = pofd - 1;
//						value2.i64 = exponent;
//						if (value2.i64) ExpStr.size = 2;
//						else ExpStr.size = 3;
//						while (value2.i64)
//						{
//							ExpStr.size++;
//							value2.i64 /= 10;
//						}
//
//						//二、计算数据字符串大小
//					cSp_llE_step2:
//						//（一）计算整数数据字符串大小
//						//没有指定precision：默认保留6位小数
//						if (!FmtInfo.precision.enabled)
//						{
//							FmtInfo.precision.flag = FALSE;
//							FmtInfo.precision.value = 6;
//						}
//						//有效数字转小数点
//						if (FmtInfo.precision.flag)
//							FmtInfo.precision.value--;
//						//整数数据字符串大小始终为1
//						DataStr.size = 1;
//						//（二）计算小数数据字符串大小
//						if (FmtInfo.precision.value)
//							DecDataStr.size = FmtInfo.precision.value + 1;
//						else
//							DecDataStr.size = 0;
//
//						//三、各子字符串定位
//						//（一）计算各子字符串大小
//						//需要padding
//						if (FmtInfo.padding.length > FmtInfo.sign + DataStr.size + DecDataStr.size + ExpStr.size)
//						{
//							//左对齐
//							if (FmtInfo.padding.align)
//							{
//								FrontPad.size = 0;
//								BackPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size - ExpStr.size;
//							}
//							//右对齐
//							else
//							{
//								FrontPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size - ExpStr.size;
//								BackPad.size = 0;
//							}
//						}
//						//不需要padding
//						else
//						{
//							FrontPad.size = 0;
//							BackPad.size = 0;
//						}
//						//（二）计算各子字符串地址
//						DataStr.data = FrontPad.data + FrontPad.size + FmtInfo.sign;
//						DecDataStr.data = DataStr.data + DataStr.size;
//						ExpStr.data = DecDataStr.data + DecDataStr.size;
//						BackPad.data = DecDataStr.data + ExpStr.size;
//						//（三）根据符号位置调整前置padding位置，并预填符号
//						if (FmtInfo.sign)
//						{
//							//符号偏左
//							if (FrontPad.size && FmtInfo.padding.content)
//							{
//								FrontPad.data++;
//								*(FrontPad.data - 1) = SignChar;
//							}
//							//符号偏右
//							else
//								*(DataStr.data - 1) = SignChar;
//						}
//
//						//四、构建字符串
//						//（一）前置padding
//						if (FrontPad.size)
//						{
//							rOutStr = 0;
//							//填0
//							if (FmtInfo.padding.content)
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = '0';
//									rOutStr++;
//								}
//							}
//							//填空格
//							else
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = ' ';
//									rOutStr++;
//								}
//							}
//						}
//						//（二）数据字符串
//						//1. 原始数据首位数字在小数点右边
//						if (pofd < 0)
//						{
//							//一、过位（去掉前面的0，让首位数字移到个位）
//							while (pofd < 0)
//							{
//								value1.f64 *= 10;
//								pofd++;
//							}
//							//二、整数数据字符串
//							if (DecDataStr.size)
//								DataStr.data[0] = '0' + (cmtUint64)value1.f64 % 10;
//							//如果没有小数部分，整数位需要四舍五入
//							else
//								DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)(value1.f64 + 0.5) % 10;
//							value1.f64 *= 10;
//							//三、小数数据字符串
//							if (DecDataStr.size)
//							{
//								DecDataStr.data[0] = '.';
//								rOutStr = 1;
//								while (rOutStr < DecDataStr.size - 1)
//								{
//									//正着写入（从左往右）
//									DecDataStr.data[rOutStr] = '0' + (cmtUint64)value1.f64 % 10;
//									value1.f64 *= 10;
//									rOutStr++;
//								}
//								//最后一位需要四舍五入
//								DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f64 + 0.5) % 10;
//							}
//						}
//						//2. 原始数据末位有效数字在小数点左边
//						else if (pofd > FmtInfo.precision.value)
//						{
//							//一、过位（去掉无效数字）
//							while (pofd > 1 + FmtInfo.precision.value)
//							{
//								value1.f64 /= 10;
//								pofd--;
//							}
//							//二、小数数据字符串
//							if (DecDataStr.size)
//							{
//								DecDataStr.data[0] = '.';
//								//最后一位需要四舍五入
//								DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f64 + 0.5) % 10;
//								value1.f64 /= 10;
//								rOutStr = 1;
//								while (rOutStr < DecDataStr.size - 1)
//								{
//									//倒着写入（从右往左）
//									DecDataStr.data[DecDataStr.size - rOutStr - 1] = '0' + (cmtUint64)value1.f64 % 10;
//									value1.f64 /= 10;
//									rOutStr++;
//								}
//							}
//							//三、整数数据字符串
//							if (DecDataStr.size)
//								DataStr.data[0] = '0' + (cmtUint64)value1.f64 % 10;
//							//如果没有小数部分，整数位需要四舍五入
//							else
//								DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)(value1.f64 + 0.5) % 10;
//						}
//						//3. 原始数据有效数字横跨小数点
//						else
//						{
//							//一、原始数据整数部分
//							//（一）小数数据字符串
//							DecDataStr.data[0] = '.';
//							value2 = value1;
//							rOutStr = 1;
//							while (rOutStr < pofd)
//							{
//								//倒着写入（从右往左）
//								DecDataStr.data[pofd - rOutStr] = '0' + (cmtUint64)value2.f64 % 10;
//								value2.f64 /= 10;
//								rOutStr++;
//							}
//							//（二）整数数据字符串
//							DataStr.data[0] = '0' + (cmtUint64)value2.f64 % 10;
//							//二、原始数据小数部分
//							//（一）小数数据字符串
//							//剪掉整数部分以防指数上溢
//							value1.f64 -= (cmtUint64)value1.f64;
//							value1.f64 *= 10;
//							while (rOutStr < DecDataStr.size - 1)
//							{
//								//正着写入（从左往右）
//								DecDataStr.data[rOutStr] = '0' + (cmtUint64)value1.f64 % 10;
//								value1.f64 *= 10;
//								rOutStr++;
//							}
//							//最后一位需要四舍五入
//							DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f64 + 0.5) % 10;
//						}
//						//（四）指数数据字符串
//						ExpStr.data[0] = 'E';
//						if (exponent >= 0) ExpStr.data[1] = '+';
//						else
//						{
//							ExpStr.data[1] = '-';
//							exponent = -exponent;
//						}
//						rOutStr = 0;
//						while (rOutStr < ExpStr.size - 2)
//						{
//							//倒着写入（从右往左）
//							ExpStr.data[ExpStr.size - rOutStr - 1] = '0' + exponent % 10;
//							exponent /= 10;
//							rOutStr++;
//						}
//						//（五）后置padding
//						if (BackPad.size)
//						{
//							rOutStr = 0;
//							while (rOutStr < BackPad.size)
//							{
//								BackPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//					}
//					else
//					{
//						//一、数据处理
//						//（一）从参数中载入指定长度的数据
//						value1.f32 = (float)*((double*)ArgList + rArg);
//						//负数记录并取绝对值
//						if (value1.f32 < 0)
//						{
//							SignChar = '-';
//							value1.f32 = -value1.f32;
//							FmtInfo.sign = TRUE;//打开符号显示
//						}
//						else
//							SignChar = '+';
//						//（二）计算pofd（第一位十进制数字位置）
//						value2 = value1;
//						pofd = 0;
//						if (value2.f32 >= 1)
//						{
//							while (value2.f32 >= 1)
//							{
//								value2.f32 /= 10.0;
//								pofd++;
//							}
//						}
//						else
//						{
//							while (value2.f32 < 1)
//							{
//								value2.f32 *= 10.0;
//								pofd--;
//							}
//						}
//						//（三）计算指数数据字符串大小
//						exponent = pofd - 1;
//						value2.i64 = exponent;
//						if (value2.i64) ExpStr.size = 2;
//						else ExpStr.size = 3;
//						while (value2.i64)
//						{
//							ExpStr.size++;
//							value2.i64 /= 10;
//						}
//
//						//二、计算数据字符串大小
//					cSp_hhE_step2:
//						//（一）计算整数数据字符串大小
//						//没有指定precision：默认保留6位小数
//						if (!FmtInfo.precision.enabled)
//						{
//							FmtInfo.precision.flag = FALSE;
//							FmtInfo.precision.value = 6;
//						}
//						//有效数字转小数点
//						if (FmtInfo.precision.flag)
//							FmtInfo.precision.value--;
//						//整数数据字符串大小始终为1
//						DataStr.size = 1;
//						//（二）计算小数数据字符串大小
//						if (FmtInfo.precision.value)
//							DecDataStr.size = FmtInfo.precision.value + 1;
//						else
//							DecDataStr.size = 0;
//
//						//三、各子字符串定位
//						//（一）计算各子字符串大小
//						//需要padding
//						if (FmtInfo.padding.length > FmtInfo.sign + DataStr.size + DecDataStr.size + ExpStr.size)
//						{
//							//左对齐
//							if (FmtInfo.padding.align)
//							{
//								FrontPad.size = 0;
//								BackPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size - ExpStr.size;
//							}
//							//右对齐
//							else
//							{
//								FrontPad.size = FmtInfo.padding.length - FmtInfo.sign - DataStr.size - DecDataStr.size - ExpStr.size;
//								BackPad.size = 0;
//							}
//						}
//						//不需要padding
//						else
//						{
//							FrontPad.size = 0;
//							BackPad.size = 0;
//						}
//						//（二）计算各子字符串地址
//						DataStr.data = FrontPad.data + FrontPad.size + FmtInfo.sign;
//						DecDataStr.data = DataStr.data + DataStr.size;
//						ExpStr.data = DecDataStr.data + DecDataStr.size;
//						BackPad.data = DecDataStr.data + ExpStr.size;
//						//（三）根据符号位置调整前置padding位置，并预填符号
//						if (FmtInfo.sign)
//						{
//							//符号偏左
//							if (FrontPad.size && FmtInfo.padding.content)
//							{
//								FrontPad.data++;
//								*(FrontPad.data - 1) = SignChar;
//							}
//							//符号偏右
//							else
//								*(DataStr.data - 1) = SignChar;
//						}
//
//						//四、构建字符串
//						//（一）前置padding
//						if (FrontPad.size)
//						{
//							rOutStr = 0;
//							//填0
//							if (FmtInfo.padding.content)
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = '0';
//									rOutStr++;
//								}
//							}
//							//填空格
//							else
//							{
//								while (rOutStr < FrontPad.size)
//								{
//									FrontPad.data[rOutStr] = ' ';
//									rOutStr++;
//								}
//							}
//						}
//						//（二）数据字符串
//						//1. 原始数据首位数字在小数点右边
//						if (pofd < 0)
//						{
//							//一、过位（去掉前面的0，让首位数字移到个位）
//							while (pofd < 0)
//							{
//								value1.f32 *= 10;
//								pofd++;
//							}
//							//二、整数数据字符串
//							if (DecDataStr.size)
//								DataStr.data[0] = '0' + (cmtUint64)value1.f32 % 10;
//							//如果没有小数部分，整数位需要四舍五入
//							else
//								DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)(value1.f32 + 0.5) % 10;
//							value1.f32 *= 10;
//							//三、小数数据字符串
//							if (DecDataStr.size)
//							{
//								DecDataStr.data[0] = '.';
//								rOutStr = 1;
//								while (rOutStr < DecDataStr.size - 1)
//								{
//									//正着写入（从左往右）
//									DecDataStr.data[rOutStr] = '0' + (cmtUint64)value1.f32 % 10;
//									value1.f32 *= 10;
//									rOutStr++;
//								}
//								//最后一位需要四舍五入
//								DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f32 + 0.5) % 10;
//							}
//						}
//						//2. 原始数据末位有效数字在小数点左边
//						else if (pofd > FmtInfo.precision.value)
//						{
//							//一、过位（去掉无效数字）
//							while (pofd > 1 + FmtInfo.precision.value)
//							{
//								value1.f32 /= 10;
//								pofd--;
//							}
//							//二、小数数据字符串
//							if (DecDataStr.size)
//							{
//								DecDataStr.data[0] = '.';
//								//最后一位需要四舍五入
//								DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f32 + 0.5) % 10;
//								value1.f32 /= 10;
//								rOutStr = 1;
//								while (rOutStr < DecDataStr.size - 1)
//								{
//									//倒着写入（从右往左）
//									DecDataStr.data[DecDataStr.size - rOutStr - 1] = '0' + (cmtUint64)value1.f32 % 10;
//									value1.f32 /= 10;
//									rOutStr++;
//								}
//							}
//							//三、整数数据字符串
//							if (DecDataStr.size)
//								DataStr.data[0] = '0' + (cmtUint64)value1.f32 % 10;
//							//如果没有小数部分，整数位需要四舍五入
//							else
//								DataStr.data[DataStr.size - rOutStr - 1] = '0' + (cmtUint64)(value1.f32 + 0.5) % 10;
//						}
//						//3. 原始数据有效数字横跨小数点
//						else
//						{
//							//一、原始数据整数部分
//							//（一）小数数据字符串
//							DecDataStr.data[0] = '.';
//							value2 = value1;
//							rOutStr = 1;
//							while (rOutStr < pofd)
//							{
//								//倒着写入（从右往左）
//								DecDataStr.data[pofd - rOutStr] = '0' + (cmtUint64)value2.f32 % 10;
//								value2.f32 /= 10;
//								rOutStr++;
//							}
//							//（二）整数数据字符串
//							DataStr.data[0] = '0' + (cmtUint64)value2.f32 % 10;
//							//二、原始数据小数部分
//							//（一）小数数据字符串
//							//剪掉整数部分以防指数上溢
//							value1.f32 -= (cmtUint64)value1.f32;
//							value1.f32 *= 10;
//							while (rOutStr < DecDataStr.size - 1)
//							{
//								//正着写入（从左往右）
//								DecDataStr.data[rOutStr] = '0' + (cmtUint64)value1.f32 % 10;
//								value1.f32 *= 10;
//								rOutStr++;
//							}
//							//最后一位需要四舍五入
//							DecDataStr.data[DecDataStr.size - 1] = '0' + (cmtUint64)(value1.f32 + 0.5) % 10;
//						}
//						//（四）指数数据字符串
//						ExpStr.data[0] = 'E';
//						if (exponent >= 0) ExpStr.data[1] = '+';
//						else
//						{
//							ExpStr.data[1] = '-';
//							exponent = -exponent;
//						}
//						rOutStr = 0;
//						while (rOutStr < ExpStr.size - 2)
//						{
//							//倒着写入（从右往左）
//							ExpStr.data[ExpStr.size - rOutStr - 1] = '0' + exponent % 10;
//							exponent /= 10;
//							rOutStr++;
//						}
//						//（五）后置padding
//						if (BackPad.size)
//						{
//							rOutStr = 0;
//							while (rOutStr < BackPad.size)
//							{
//								BackPad.data[rOutStr] = ' ';
//								rOutStr++;
//							}
//						}
//					}
//
//					//五、rOut增量
//					rOut += FrontPad.size + FmtInfo.sign + DataStr.size + DecDataStr.size + ExpStr.size + BackPad.size;
//				}
//				else if (FmtInfo.type == 'g')
//				{
//					if (FmtInfo.size == CMT_FMT_SIZE_L || FmtInfo.size == CMT_FMT_SIZE_LL)
//					{
//						//一、数据处理
//						//（一）从参数中载入指定长度的数据
//						value1.f64 = *((double*)ArgList + rArg);
//						//负数记录并取绝对值
//						if (value1.f64 < 0)
//						{
//							SignChar = '-';
//							value1.f64 = -value1.f64;
//							FmtInfo.sign = TRUE;//打开符号显示
//						}
//						else
//							SignChar = '+';
//						//（二）计算pofd（第一位十进制数字位置）
//						value2 = value1;
//						pofd = 0;
//						if (value2.f64 >= 1)
//						{
//							while (value2.f64 >= 1)
//							{
//								value2.f64 /= 10.0;
//								pofd++;
//							}
//						}
//						else
//						{
//							while (value2.f64 < 1)
//							{
//								value2.f64 *= 10.0;
//								pofd--;
//							}
//						}
//						//（三）计算指数数据字符串大小
//						exponent = pofd - 1;
//						value2.i64 = exponent;
//						if (value2.i64) ExpStr.size = 2;
//						else ExpStr.size = 3;
//						while (value2.i64)
//						{
//							ExpStr.size++;
//							value2.i64 /= 10;
//						}
//
//						//二、选择输出模式
//						//使用指数模式
//						if (FmtInfo.precision.enabled && exponent > FmtInfo.precision.value || exponent < -4)
//							goto cSp_lle_step2;
//						//使用正常模式
//						else
//							goto cSp_llf_step2;
//					}
//					else
//					{
//						//一、数据处理
//						//（一）从参数中载入指定长度的数据
//						value1.f32 = (float)*((double*)ArgList + rArg);
//						//负数记录并取绝对值
//						if (value1.f32 < 0)
//						{
//							SignChar = '-';
//							value1.f32 = -value1.f32;
//							FmtInfo.sign = TRUE;//打开符号显示
//						}
//						else
//							SignChar = '+';
//						//（二）计算pofd（第一位十进制数字位置）
//						value2 = value1;
//						pofd = 0;
//						if (value2.f32 >= 1)
//						{
//							while (value2.f32 >= 1)
//							{
//								value2.f32 /= 10.0;
//								pofd++;
//							}
//						}
//						else
//						{
//							while (value2.f32 < 1)
//							{
//								value2.f32 *= 10.0;
//								pofd--;
//							}
//						}
//						//（三）计算指数数据字符串大小
//						exponent = pofd - 1;
//						value2.i64 = exponent;
//						if (value2.i64) ExpStr.size = 2;
//						else ExpStr.size = 3;
//						while (value2.i64)
//						{
//							ExpStr.size++;
//							value2.i64 /= 10;
//						}
//
//						//二、选择输出模式
//						//使用指数模式
//						if (FmtInfo.precision.enabled && exponent > FmtInfo.precision.value || exponent < -4)
//							goto cSp_hhe_step2;
//						//使用正常模式
//						else
//							goto cSp_hhf_step2;
//					}
//				}
//				else if (FmtInfo.type == 'G')
//				{
//					if (FmtInfo.size == CMT_FMT_SIZE_L || FmtInfo.size == CMT_FMT_SIZE_LL)
//					{
//						//一、数据处理
//						//（一）从参数中载入指定长度的数据
//						value1.f64 = *((double*)ArgList + rArg);
//						//负数记录并取绝对值
//						if (value1.f64 < 0)
//						{
//							SignChar = '-';
//							value1.f64 = -value1.f64;
//							FmtInfo.sign = TRUE;//打开符号显示
//						}
//						else
//							SignChar = '+';
//						//（二）计算pofd（第一位十进制数字位置）
//						value2 = value1;
//						pofd = 0;
//						if (value2.f64 >= 1)
//						{
//							while (value2.f64 >= 1)
//							{
//								value2.f64 /= 10.0;
//								pofd++;
//							}
//						}
//						else
//						{
//							while (value2.f64 < 1)
//							{
//								value2.f64 *= 10.0;
//								pofd--;
//							}
//						}
//						//（三）计算指数数据字符串大小
//						exponent = pofd - 1;
//						value2.i64 = exponent;
//						if (value2.i64) ExpStr.size = 2;
//						else ExpStr.size = 3;
//						while (value2.i64)
//						{
//							ExpStr.size++;
//							value2.i64 /= 10;
//						}
//
//						//二、选择输出模式
//						//使用指数模式
//						if (FmtInfo.precision.enabled && exponent > FmtInfo.precision.value || exponent < -4)
//							goto cSp_llE_step2;
//						//使用正常模式
//						else
//							goto cSp_llf_step2;
//					}
//					else
//					{
//						//一、数据处理
//						//（一）从参数中载入指定长度的数据
//						value1.f32 = (float)*((double*)ArgList + rArg);
//						//负数记录并取绝对值
//						if (value1.f32 < 0)
//						{
//							SignChar = '-';
//							value1.f32 = -value1.f32;
//							FmtInfo.sign = TRUE;//打开符号显示
//						}
//						else
//							SignChar = '+';
//						//（二）计算pofd（第一位十进制数字位置）
//						value2 = value1;
//						pofd = 0;
//						if (value2.f32 >= 1)
//						{
//							while (value2.f32 >= 1)
//							{
//								value2.f32 /= 10.0;
//								pofd++;
//							}
//						}
//						else
//						{
//							while (value2.f32 < 1)
//							{
//								value2.f32 *= 10.0;
//								pofd--;
//							}
//						}
//						//（三）计算指数数据字符串大小
//						exponent = pofd - 1;
//						value2.i64 = exponent;
//						if (value2.i64) ExpStr.size = 2;
//						else ExpStr.size = 3;
//						while (value2.i64)
//						{
//							ExpStr.size++;
//							value2.i64 /= 10;
//						}
//
//						//二、选择输出模式
//						//使用指数模式
//						if (FmtInfo.precision.enabled && exponent > FmtInfo.precision.value || exponent < -4)
//							goto cSp_hhE_step2;
//						//使用正常模式
//						else
//							goto cSp_hhf_step2;
//					}
//				}
//				else
//				{
//					//结构：(前置padding)(字符串)(后置padding)
//					//一、计算字符串大小
//					if (FmtInfo.size == CMT_FMT_SIZE_HH) DataStr.size = cmtANSItoU8size(ArgList[rArg]);
//					else if (FmtInfo.size == CMT_FMT_SIZE_L) DataStr.size = cmtU16toU8size(ArgList[rArg]);
//					else if (FmtInfo.size == CMT_FMT_SIZE_LL) DataStr.size = cmtU32toU8size(ArgList[rArg]);
//					else DataStr.size = ((cmtU8str*)ArgList[rArg])->size;
//					if (FmtInfo.precision.enabled && DataStr.size > FmtInfo.precision.value)
//						DataStr.size = FmtInfo.precision.value;
//
//					//二、各子字符串定位
//					//（一）计算各子字符串大小
//					//需要padding
//					if (FmtInfo.padding.length > DataStr.size)
//					{
//						//左对齐
//						if (FmtInfo.padding.align)
//						{
//							FrontPad.size = 0;
//							BackPad.size = FmtInfo.padding.length - DataStr.size;
//						}
//						//右对齐
//						else
//						{
//							FrontPad.size = FmtInfo.padding.length - DataStr.size;
//							BackPad.size = 0;
//						}
//					}
//					//不需要padding
//					else
//					{
//						FrontPad.size = 0;
//						BackPad.size = 0;
//					}
//					//（二）计算各子字符串地址
//					DataStr.data = FrontPad.data + FrontPad.size;
//					BackPad.data = DataStr.data + DataStr.size;
//
//					//三、构建字符串
//					//（一）前置padding
//					if (FrontPad.size)
//					{
//						rOutStr = 0;
//						while (rOutStr < FrontPad.size)
//						{
//							FrontPad.data[rOutStr] = ' ';
//							rOutStr++;
//						}
//					}
//					//（二）字符串
//					if (FmtInfo.size == CMT_FMT_SIZE_HH) cmtANSItoU8(ArgList[rArg], &DataStr);
//					else if (FmtInfo.size == CMT_FMT_SIZE_L) cmtU16toU8(ArgList[rArg], &DataStr);
//					else if (FmtInfo.size == CMT_FMT_SIZE_LL) cmtU32toU8(ArgList[rArg], &DataStr);
//					else
//					{
//						rOutStr = 0;
//						while (rOutStr < DataStr.size)
//							DataStr.data[rOutStr] = ((cmtU8str*)ArgList[rArg])->data[rOutStr];
//					}
//					//（三）后置padding
//					if (BackPad.size)
//					{
//						rOutStr = 0;
//						while (rOutStr < BackPad.size)
//						{
//							BackPad.data[rOutStr] = ' ';
//							rOutStr++;
//						}
//					}
//
//					//四、rOut增量
//					rOut += FrontPad.size + DataStr.size + BackPad.size;
//				}
//				rArg++;
//			}
//		}
//		else
//		{
//			//不是格式控制字符串
//			//复制
//			if (format->data[rFmt] < 0x80)
//				out->data[rOut++] = format->data[rFmt++];
//			else if (format->data[rFmt] < 0xe0)
//			{
//				out->data[rOut++] = format->data[rFmt++];
//				out->data[rOut++] = format->data[rFmt++];
//			}
//			else if (format->data[rFmt] < 0xf0)
//			{
//				out->data[rOut++] = format->data[rFmt++];
//				out->data[rOut++] = format->data[rFmt++];
//				out->data[rOut++] = format->data[rFmt++];
//			}
//			else
//			{
//				out->data[rOut++] = format->data[rFmt++];
//				out->data[rOut++] = format->data[rFmt++];
//				out->data[rOut++] = format->data[rFmt++];
//				out->data[rOut++] = format->data[rFmt++];
//			}
//		}
//	}
//}