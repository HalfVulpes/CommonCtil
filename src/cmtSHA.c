#include <cmtCryp.h>

void cmtSHA256Transform(cmtSHA256* ctx, cmtUint8* data)
{
	cmtUint32 a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	for (; i < 64; ++i)
		m[i] = CMT_SIG1(m[i - 2]) + m[i - 7] + CMT_SIG0(m[i - 15]) + m[i - 16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + CMT_EP1(e) + CMT_CH(e, f, g) + k[i] + m[i];
		t2 = CMT_EP0(a) + CMT_MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void cmtSHA256Init(cmtSHA256* ctx) 
{
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
};

void cmtSHA256Update(cmtSHA256* ctx, cmtUint8* data, cmtUint64 size)
{
	cmtUint32 i;

	for (i = 0; i < size; ++i)
	{
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64)
		{
			cmtSHA256Transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void cmtSHA256Get(cmtSHA256* ctx, cmtUint8* hash)
{
	cmtUint32 i;

	i = ctx->datalen;

	if (ctx->datalen < 56)
	{
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else
	{
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;
		cmtSHA256Transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}

	ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	cmtSHA256Transform(ctx, ctx->data);

	// 翻转比特
	for (i = 0; i < 4; ++i)
	{
		hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

void cmtSHA1Init(cmtSHA1* ctx) {
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xEFCDAB89;
	ctx->state[2] = 0x98BADCFE;
	ctx->state[3] = 0x10325476;
	ctx->state[4] = 0xc3d2e1f0;
	ctx->k[0] = 0x5a827999;
	ctx->k[1] = 0x6ed9eba1;
	ctx->k[2] = 0x8f1bbcdc;
	ctx->k[3] = 0xca62c1d6;
}

void cmtSHA1Update(cmtSHA1* ctx, cmtUint8* data, cmtUint64 size) {
	cmtUint64 i;

	for (i = 0; i < size; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			cmtSHA1Transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void cmtSHA1Get(cmtSHA1* ctx, cmtUint8* hash) {
	cmtUint32 i;

	i = ctx->datalen;

	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else {
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;
		cmtSHA1Transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}

	ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	cmtSHA1Transform(ctx, ctx->data);

	// 在标准手册中的实现是针对大端机的，但是我们民用基本上不太可能使用大端，所以这里改为小段机模式
	// 将最后的结果拷贝出来的时候对比特进行翻转
	for (i = 0; i < 4; ++i) {
		hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
	}
}

void cmtSHA1Transform(cmtSHA1* ctx, cmtUint8* data) {
	cmtUint32 a, b, c, d, e, i, j, t, m[80];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) + (data[j + 1] << 16) + (data[j + 2] << 8) + (data[j + 3]);
	for (; i < 80; ++i) {
		m[i] = (m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16]);
		m[i] = (m[i] << 1) | (m[i] >> 31);
	}

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];

	for (i = 0; i < 20; ++i) {
		t = CMT_ROTLEFT(a, 5) + ((b & c) ^ (~b & d)) + e + ctx->k[0] + m[i];
		e = d;
		d = c;
		c = CMT_ROTLEFT(b, 30);
		b = a;
		a = t;
	}
	for (; i < 40; ++i) {
		t = CMT_ROTLEFT(a, 5) + (b ^ c ^ d) + e + ctx->k[1] + m[i];
		e = d;
		d = c;
		c = CMT_ROTLEFT(b, 30);
		b = a;
		a = t;
	}
	for (; i < 60; ++i) {
		t = CMT_ROTLEFT(a, 5) + ((b & c) ^ (b & d) ^ (c & d)) + e + ctx->k[2] + m[i];
		e = d;
		d = c;
		c = CMT_ROTLEFT(b, 30);
		b = a;
		a = t;
	}
	for (; i < 80; ++i) {
		t = CMT_ROTLEFT(a, 5) + (b ^ c ^ d) + e + ctx->k[3] + m[i];
		e = d;
		d = c;
		c = CMT_ROTLEFT(b, 30);
		b = a;
		a = t;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
}

void cmtMD5Init(cmtMD5* ctx) 
{
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xEFCDAB89;
	ctx->state[2] = 0x98BADCFE;
	ctx->state[3] = 0x10325476;
}

void cmtMD5Update(cmtMD5* ctx, cmtUint8* data, cmtUint64 size) {
	cmtUint64 i;

	for (i = 0; i < size; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			cmtMD5Transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void cmtMD5Get(cmtMD5* ctx, cmtUint8* hash) 
{
	cmtUint64 i;

	i = ctx->datalen;

	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else if (ctx->datalen >= 56) {
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;
		cmtMD5Transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}

	ctx->bitlen += ctx->datalen * 8;
	ctx->data[56] = ctx->bitlen;
	ctx->data[57] = ctx->bitlen >> 8;
	ctx->data[58] = ctx->bitlen >> 16;
	ctx->data[59] = ctx->bitlen >> 24;
	ctx->data[60] = ctx->bitlen >> 32;
	ctx->data[61] = ctx->bitlen >> 40;
	ctx->data[62] = ctx->bitlen >> 48;
	ctx->data[63] = ctx->bitlen >> 56;
	cmtMD5Transform(ctx, ctx->data);

	// 在标准手册中的实现是针对大端机的，但是我们民用基本上不太可能使用大端，所以这里改为小段机模式
	// 将最后的结果拷贝出来的时候对比特进行翻转
	for (i = 0; i < 4; ++i) {
		hash[i] = (ctx->state[0] >> (i * 8)) & 0x000000ff;
		hash[i + 4] = (ctx->state[1] >> (i * 8)) & 0x000000ff;
		hash[i + 8] = (ctx->state[2] >> (i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (i * 8)) & 0x000000ff;
	}
}

void cmtMD5Transform(cmtMD5* ctx, cmtUint8* data) 
{
	cmtUint32 a, b, c, d, m[16], i, j;

	// MD5的标准手册中的例子仍然是大端机，这里直接重新实现成小端机版本
	// 翻转比特
	// 在最后输出时从大端机结果翻转到小端机结果
	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j]) + (data[j + 1] << 8) + (data[j + 2] << 16) + (data[j + 3] << 24);

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];

	CMT_FF(a, b, c, d, m[0], 7, 0xd76aa478);
	CMT_FF(d, a, b, c, m[1], 12, 0xe8c7b756);
	CMT_FF(c, d, a, b, m[2], 17, 0x242070db);
	CMT_FF(b, c, d, a, m[3], 22, 0xc1bdceee);
	CMT_FF(a, b, c, d, m[4], 7, 0xf57c0faf);
	CMT_FF(d, a, b, c, m[5], 12, 0x4787c62a);
	CMT_FF(c, d, a, b, m[6], 17, 0xa8304613);
	CMT_FF(b, c, d, a, m[7], 22, 0xfd469501);
	CMT_FF(a, b, c, d, m[8], 7, 0x698098d8);
	CMT_FF(d, a, b, c, m[9], 12, 0x8b44f7af);
	CMT_FF(c, d, a, b, m[10], 17, 0xffff5bb1);
	CMT_FF(b, c, d, a, m[11], 22, 0x895cd7be);
	CMT_FF(a, b, c, d, m[12], 7, 0x6b901122);
	CMT_FF(d, a, b, c, m[13], 12, 0xfd987193);
	CMT_FF(c, d, a, b, m[14], 17, 0xa679438e);
	CMT_FF(b, c, d, a, m[15], 22, 0x49b40821);

	CMT_GG(a, b, c, d, m[1], 5, 0xf61e2562);
	CMT_GG(d, a, b, c, m[6], 9, 0xc040b340);
	CMT_GG(c, d, a, b, m[11], 14, 0x265e5a51);
	CMT_GG(b, c, d, a, m[0], 20, 0xe9b6c7aa);
	CMT_GG(a, b, c, d, m[5], 5, 0xd62f105d);
	CMT_GG(d, a, b, c, m[10], 9, 0x02441453);
	CMT_GG(c, d, a, b, m[15], 14, 0xd8a1e681);
	CMT_GG(b, c, d, a, m[4], 20, 0xe7d3fbc8);
	CMT_GG(a, b, c, d, m[9], 5, 0x21e1cde6);
	CMT_GG(d, a, b, c, m[14], 9, 0xc33707d6);
	CMT_GG(c, d, a, b, m[3], 14, 0xf4d50d87);
	CMT_GG(b, c, d, a, m[8], 20, 0x455a14ed);
	CMT_GG(a, b, c, d, m[13], 5, 0xa9e3e905);
	CMT_GG(d, a, b, c, m[2], 9, 0xfcefa3f8);
	CMT_GG(c, d, a, b, m[7], 14, 0x676f02d9);
	CMT_GG(b, c, d, a, m[12], 20, 0x8d2a4c8a);

	CMT_HH(a, b, c, d, m[5], 4, 0xfffa3942);
	CMT_HH(d, a, b, c, m[8], 11, 0x8771f681);
	CMT_HH(c, d, a, b, m[11], 16, 0x6d9d6122);
	CMT_HH(b, c, d, a, m[14], 23, 0xfde5380c);
	CMT_HH(a, b, c, d, m[1], 4, 0xa4beea44);
	CMT_HH(d, a, b, c, m[4], 11, 0x4bdecfa9);
	CMT_HH(c, d, a, b, m[7], 16, 0xf6bb4b60);
	CMT_HH(b, c, d, a, m[10], 23, 0xbebfbc70);
	CMT_HH(a, b, c, d, m[13], 4, 0x289b7ec6);
	CMT_HH(d, a, b, c, m[0], 11, 0xeaa127fa);
	CMT_HH(c, d, a, b, m[3], 16, 0xd4ef3085);
	CMT_HH(b, c, d, a, m[6], 23, 0x04881d05);
	CMT_HH(a, b, c, d, m[9], 4, 0xd9d4d039);
	CMT_HH(d, a, b, c, m[12], 11, 0xe6db99e5);
	CMT_HH(c, d, a, b, m[15], 16, 0x1fa27cf8);
	CMT_HH(b, c, d, a, m[2], 23, 0xc4ac5665);

	CMT_II(a, b, c, d, m[0], 6, 0xf4292244);
	CMT_II(d, a, b, c, m[7], 10, 0x432aff97);
	CMT_II(c, d, a, b, m[14], 15, 0xab9423a7);
	CMT_II(b, c, d, a, m[5], 21, 0xfc93a039);
	CMT_II(a, b, c, d, m[12], 6, 0x655b59c3);
	CMT_II(d, a, b, c, m[3], 10, 0x8f0ccc92);
	CMT_II(c, d, a, b, m[10], 15, 0xffeff47d);
	CMT_II(b, c, d, a, m[1], 21, 0x85845dd1);
	CMT_II(a, b, c, d, m[8], 6, 0x6fa87e4f);
	CMT_II(d, a, b, c, m[15], 10, 0xfe2ce6e0);
	CMT_II(c, d, a, b, m[6], 15, 0xa3014314);
	CMT_II(b, c, d, a, m[13], 21, 0x4e0811a1);
	CMT_II(a, b, c, d, m[4], 6, 0xf7537e82);
	CMT_II(d, a, b, c, m[11], 10, 0xbd3af235);
	CMT_II(c, d, a, b, m[2], 15, 0x2ad7d2bb);
	CMT_II(b, c, d, a, m[9], 21, 0xeb86d391);

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
}