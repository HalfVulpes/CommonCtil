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

void cmtAESInitialVectorInit(cmtUint8* iv) 
{
	cmtUint64 random = 0x0, random2 = 0x0;
	cmtRealRand(&random, 1);
	cmtRealRand(&random2, 1);
	cmtUint8 seed[8] = { NULL };
	for (int i = 0; i < 4; i++)
	{
		seed[i] = (cmtUint8)(&random)[7 - i];
		seed[i + 4] = (cmtUint8)(&random2)[7 - i];
	}
	cmtMD5 ctx;
	cmtMD5Init(&ctx);
	cmtMD5Update(&ctx, seed, 8);
	cmtMD5Get(&ctx, iv);
}

void cmtXorBuffer(const cmtUint8 in[], cmtUint8 out[], cmtUint64 size)
{
	cmtUint64 idx;

	for (idx = 0; idx < size; idx++)
		out[idx] ^= in[idx];
}

/*******************
* AES - CBC 实现
*******************/
int cmtAESencCBC(const cmtUint8 in[], cmtUint64 in_len, cmtUint8 out[], const cmtUint32 key[], int keysize, const cmtUint8 iv[])
{
	cmtUint8 buf_in[CMT_AES_BLOCK_SIZE], buf_out[CMT_AES_BLOCK_SIZE], iv_buf[CMT_AES_BLOCK_SIZE];
	int blocks, idx;

	if (in_len % CMT_AES_BLOCK_SIZE != 0)
		return(FALSE);

	blocks = in_len / CMT_AES_BLOCK_SIZE;

	memcpy(iv_buf, iv, CMT_AES_BLOCK_SIZE);

	for (idx = 0; idx < blocks; idx++) {
		memcpy(buf_in, &in[idx * CMT_AES_BLOCK_SIZE], CMT_AES_BLOCK_SIZE);
		cmtXorBuffer(iv_buf, buf_in, CMT_AES_BLOCK_SIZE);
		cmtAESenc(buf_in, buf_out, key, keysize);
		memcpy(&out[idx * CMT_AES_BLOCK_SIZE], buf_out, CMT_AES_BLOCK_SIZE);
		memcpy(iv_buf, buf_out, CMT_AES_BLOCK_SIZE);
	}

	return(TRUE);
}

int cmtAESencCBCmac(const cmtUint8 in[], cmtUint64 in_len, cmtUint8 out[], const cmtUint32 key[], int keysize, const cmtUint8 iv[])
{
	cmtUint8 buf_in[CMT_AES_BLOCK_SIZE], buf_out[CMT_AES_BLOCK_SIZE], iv_buf[CMT_AES_BLOCK_SIZE];
	int blocks, idx;

	if (in_len % CMT_AES_BLOCK_SIZE != 0)
		return(FALSE);

	blocks = in_len / CMT_AES_BLOCK_SIZE;

	memcpy(iv_buf, iv, CMT_AES_BLOCK_SIZE);

	for (idx = 0; idx < blocks; idx++) {
		memcpy(buf_in, &in[idx * CMT_AES_BLOCK_SIZE], CMT_AES_BLOCK_SIZE);
		cmtXorBuffer(iv_buf, buf_in, CMT_AES_BLOCK_SIZE);
		cmtAESenc(buf_in, buf_out, key, keysize);
		memcpy(iv_buf, buf_out, CMT_AES_BLOCK_SIZE);
		// 输出所有的块可能会溢出
	}
	if (buf_out != NULL)
	{
		memcpy(out, buf_out, CMT_AES_BLOCK_SIZE);// 只输出最后的块
	}
	return(TRUE);
}

int cmtAESdecCBC(const cmtUint8 in[], cmtUint64 in_len, cmtUint8 out[], const cmtUint32 key[], int keysize, const cmtUint8 iv[])
{
	cmtUint8 buf_in[CMT_AES_BLOCK_SIZE], buf_out[CMT_AES_BLOCK_SIZE], iv_buf[CMT_AES_BLOCK_SIZE];
	int blocks, idx;

	if (in_len % CMT_AES_BLOCK_SIZE != 0)
		return(FALSE);

	blocks = in_len / CMT_AES_BLOCK_SIZE;

	memcpy(iv_buf, iv, CMT_AES_BLOCK_SIZE);

	for (idx = 0; idx < blocks; idx++) {
		memcpy(buf_in, &in[idx * CMT_AES_BLOCK_SIZE], CMT_AES_BLOCK_SIZE);
		cmtAESdec(buf_in, buf_out, key, keysize);
		cmtXorBuffer(iv_buf, buf_out, CMT_AES_BLOCK_SIZE);
		memcpy(&out[idx * CMT_AES_BLOCK_SIZE], buf_out, CMT_AES_BLOCK_SIZE);
		memcpy(iv_buf, buf_in, CMT_AES_BLOCK_SIZE);
	}

	return(TRUE);
}

/*******************
* AES - CTR 实现
*******************/
void cmtAESincrIV(cmtUint8 iv[], int counter_size)
{
	int idx;

	// 大端操作
	for (idx = CMT_AES_BLOCK_SIZE - 1; idx >= CMT_AES_BLOCK_SIZE - counter_size; idx--) {
		iv[idx]++;
		if (iv[idx] != 0 || idx == CMT_AES_BLOCK_SIZE - counter_size)
			break;
	}
}

// 开始加密
void cmtAESencCTR(const cmtUint8 in[], cmtUint64 in_len, cmtUint8 out[], const cmtUint32 key[], int keysize, const cmtUint8 iv[])
{
	cmtUint64 idx = 0, last_block_length;
	cmtUint8 iv_buf[CMT_AES_BLOCK_SIZE], out_buf[CMT_AES_BLOCK_SIZE];

	if (in != out)
		memcpy(out, in, in_len);

	memcpy(iv_buf, iv, CMT_AES_BLOCK_SIZE);
	last_block_length = in_len - CMT_AES_BLOCK_SIZE;

	if (in_len > CMT_AES_BLOCK_SIZE) {
		for (idx = 0; idx < last_block_length; idx += CMT_AES_BLOCK_SIZE) {
			cmtAESenc(iv_buf, out_buf, key, keysize);
			cmtXorBuffer(out_buf, &out[idx], CMT_AES_BLOCK_SIZE);
			cmtAESincrIV(iv_buf, CMT_AES_BLOCK_SIZE);
		}
	}

	cmtAESenc(iv_buf, out_buf, key, keysize);
	cmtXorBuffer(out_buf, &out[idx], in_len - idx);   // 最有效的字节
}

void cmtAESdecCTR(const cmtUint8 in[], cmtUint64 in_len, cmtUint8 out[], const cmtUint32 key[], int keysize, const cmtUint8 iv[])
{
	//CTR的加密就是他自己的反函数
	cmtAESencCTR(in, in_len, out, key, keysize, iv);
}

/*******************
* AES - CCM 加密
*******************/
// outLen = payloadLen + assocLen
int cmtAESencCCM(const cmtUint8 payload[], cmtUint32 payloadLen, const cmtUint8 assoc[], unsigned short assocLen,
	const cmtUint8 nonce[], unsigned short nonceLen, cmtUint8 out[], cmtUint32* out_len,
	cmtUint32 macLen, const cmtUint8 key_str[], int keysize)
{
	cmtUint8 temp_iv[CMT_AES_BLOCK_SIZE], counter[CMT_AES_BLOCK_SIZE], mac[16], * buf;
	int endOfBuffer, payloadLenStoreSize;
	cmtUint32 key[60];

	if (macLen != 4 && macLen != 6 && macLen != 8 && macLen != 10 &&
		macLen != 12 && macLen != 14 && macLen != 16)
		return(FALSE);

	if (nonceLen < 7 || nonceLen > 13)
		return(FALSE);

	if (assocLen > 32768 /* = 2^15 */)
		return(FALSE);

	buf = (cmtUint8*)malloc(payloadLen + assocLen + 48); //不会溢出
	if (!buf)
		return(FALSE);

	// 初始化密钥
	cmtAESkeyInit(key_str, key, keysize);

	// 格式化格式化数据的第一个块
	payloadLenStoreSize = CMT_AES_BLOCK_SIZE - 1 - nonceLen;
	cmtAESccmPreFirFormatBlock(buf, assocLen, payloadLen, payloadLenStoreSize, macLen, nonce, nonceLen);
	endOfBuffer = CMT_AES_BLOCK_SIZE;

	// 格式化:
	cmtCCMdataFormat(buf, &endOfBuffer, assoc, assocLen);
	cmtCCMdataPreload(buf, &endOfBuffer, payload, payloadLen);
	cmtAESpreFirCTRblock(counter, nonce, nonceLen, payloadLenStoreSize);
	memset(temp_iv, 0, CMT_AES_BLOCK_SIZE);
	cmtAESencCBCmac(buf, endOfBuffer, mac, key, keysize, temp_iv);
	memcpy(out, payload, payloadLen);
	memcpy(&out[payloadLen], mac, macLen);
	memcpy(temp_iv, counter, CMT_AES_BLOCK_SIZE);
	cmtAESincrIV(temp_iv, CMT_AES_BLOCK_SIZE - 1 - macLen);   // 可能有bug，但是还不是很清楚为什么
	cmtAESencCTR(out, payloadLen, out, key, keysize, temp_iv);
	cmtAESencCTR(&out[payloadLen], macLen, &out[payloadLen], key, keysize, counter);

	free(buf);
	*out_len = payloadLen + macLen;

	return(TRUE);
}


int cmtAESdecCCM(const cmtUint8 ciphertext[], cmtUint32 ciphertext_len, const cmtUint8 assoc[], unsigned short assocLen,
	const cmtUint8 nonce[], unsigned short nonceLen, cmtUint8 plaintext[], cmtUint32* plaintext_len,
	cmtUint32 macLen, int* mac_auth, const cmtUint8 key_str[], int keysize)
{
	cmtUint8 temp_iv[CMT_AES_BLOCK_SIZE], counter[CMT_AES_BLOCK_SIZE], mac[16], mac_buf[16], * buf;
	int endOfBuffer, plaintext_len_store_size;
	cmtUint32 key[60];

	if (ciphertext_len <= macLen)
		return(FALSE);

	buf = (cmtUint8*)malloc(assocLen + ciphertext_len + 48);
	if (!buf)
		return(FALSE);

	// 初始化密钥
	cmtAESkeyInit(key_str, key, keysize);

	// 拷贝明文
	*plaintext_len = ciphertext_len - macLen;
	plaintext_len_store_size = CMT_AES_BLOCK_SIZE - 1 - nonceLen;
	memcpy(plaintext, ciphertext, *plaintext_len);
	memcpy(mac, &ciphertext[*plaintext_len], macLen);

	// 拷贝第一个块入内存，用来解密
	cmtAESpreFirCTRblock(counter, nonce, nonceLen, plaintext_len_store_size);

	memcpy(temp_iv, counter, CMT_AES_BLOCK_SIZE);
	cmtAESincrIV(temp_iv, CMT_AES_BLOCK_SIZE - 1 - macLen);  
	cmtAESdecCTR(plaintext, *plaintext_len, plaintext, key, keysize, temp_iv);

	if (mac_auth != NULL) {
		cmtAESdecCTR(mac, macLen, mac, key, keysize, counter);

		plaintext_len_store_size = CMT_AES_BLOCK_SIZE - 1 - nonceLen;
		cmtAESccmPreFirFormatBlock(buf, assocLen, *plaintext_len, plaintext_len_store_size, macLen, nonce, nonceLen);
		endOfBuffer = CMT_AES_BLOCK_SIZE;

		cmtCCMdataFormat(buf, &endOfBuffer, assoc, assocLen);

		cmtCCMdataPreload(buf, &endOfBuffer, plaintext, *plaintext_len);

		memset(temp_iv, 0, CMT_AES_BLOCK_SIZE);
		cmtAESencCBCmac(buf, endOfBuffer, mac_buf, key, keysize, temp_iv);

		if (!memcmp(mac, mac_buf, macLen)) {
			*mac_auth = TRUE;
		}
		else {
			*mac_auth = FALSE;
			memset(plaintext, 0, *plaintext_len);
		}
	}

	free(buf);

	return(TRUE);
}

void cmtAESpreFirCTRblock(cmtUint8 counter[], const cmtUint8 nonce[], int nonceLen, int payloadLenStoreSize)
{
	memset(counter, 0, CMT_AES_BLOCK_SIZE);
	counter[0] = (payloadLenStoreSize - 1) & 0x07;
	memcpy(&counter[1], nonce, nonceLen);
}

void cmtAESccmPreFirFormatBlock(cmtUint8 buf[], int assocLen, int payloadLen, int payloadLenStoreSize, int macLen, const cmtUint8 nonce[], int nonceLen)
{
	buf[0] = ((((macLen - 2) / 2) & 0x07) << 3) | ((payloadLenStoreSize - 1) & 0x07);
	if (assocLen > 0)
		buf[0] += 0x40;
	memcpy(&buf[1], nonce, nonceLen);
	memset(&buf[1 + nonceLen], 0, CMT_AES_BLOCK_SIZE - 1 - nonceLen);
	buf[15] = payloadLen & 0x000000FF;
	buf[14] = (payloadLen >> 8) & 0x000000FF;
}

void cmtCCMdataFormat(cmtUint8 buf[], int* endOfBuffer, const cmtUint8 assoc[], int assocLen)
{
	int pad;

	buf[*endOfBuffer + 1] = assocLen & 0x00FF;
	buf[*endOfBuffer] = (assocLen >> 8) & 0x00FF;
	*endOfBuffer += 2;
	memcpy(&buf[*endOfBuffer], assoc, assocLen);
	*endOfBuffer += assocLen;
	pad = CMT_AES_BLOCK_SIZE - (*endOfBuffer % CMT_AES_BLOCK_SIZE); /*BUG?*/
	memset(&buf[*endOfBuffer], 0, pad);
	*endOfBuffer += pad;
}

void cmtCCMdataPreload(cmtUint8 buf[], int* endOfBuffer, const cmtUint8 payload[], int payloadLen)
{
	int pad;

	memcpy(&buf[*endOfBuffer], payload, payloadLen);
	*endOfBuffer += payloadLen;
	pad = *endOfBuffer % CMT_AES_BLOCK_SIZE;
	if (pad != 0)
		pad = CMT_AES_BLOCK_SIZE - pad;
	memset(&buf[*endOfBuffer], 0, pad);
	*endOfBuffer += pad;
}

/*******************
* AES
*******************/
/////////////////
// 密钥扩展
/////////////////

cmtUint32 cmtAESSubWord(cmtUint32 word)
{
	unsigned int result;

	result = (int)cmtAESsBox[(word >> 4) & 0x0000000F][word & 0x0000000F];
	result += (int)cmtAESsBox[(word >> 12) & 0x0000000F][(word >> 8) & 0x0000000F] << 8;
	result += (int)cmtAESsBox[(word >> 20) & 0x0000000F][(word >> 16) & 0x0000000F] << 16;
	result += (int)cmtAESsBox[(word >> 28) & 0x0000000F][(word >> 24) & 0x0000000F] << 24;
	return(result);
}


//生成密钥
void cmtAESkeyInit(const cmtUint8 keyChar[], cmtUint32 w[], int keysize)
{
	cmtUint8* key = NULL;
	if (keysize == 128)
	{
		key = (cmtUint8*)malloc(sizeof(cmtUint8) * 16);
		if (key == NULL) return;
		cmtMD5 ctx;
		cmtMD5Init(&ctx);
		cmtMD5Update(&ctx, keyChar, sizeof(keyChar) - 1);//排除结尾的'\0'
		cmtMD5Get(&ctx, key);
	} 
	else
	{
		cmtUint8 tempp[CMT_SHA256_BLOCK_SIZE];
		int i;
		cmtSHA256 ctx;
		cmtSHA256Init(&ctx);
		cmtSHA256Update(&ctx, keyChar, sizeof(keyChar) - 1);
		cmtSHA256Get(&ctx, tempp);
		if (keysize == 256)
		{
			key = (cmtUint8*)malloc(sizeof(cmtUint8) * 32);
			if (key == NULL) return;
			for (i = 0; i < 32; ++i)
			{
				key[i] = tempp[i];
			}
		} else if (keysize == 192)
		{
			key = (cmtUint8*)malloc(sizeof(cmtUint8) * 24);
			if (key == NULL) return;
			for (i = 0; i < 24; ++i)
			{
				key[i] = tempp[i];
			}
		}
	}
	int Nb = 4, Nr, Nk, idx;
	cmtUint32 temp, Rcon[] = { 0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,
					  0x40000000,0x80000000,0x1b000000,0x36000000,0x6c000000,0xd8000000,
					  0xab000000,0x4d000000,0x9a000000 };

	switch (keysize) {
	case 128: Nr = 10; Nk = 4; break;
	case 192: Nr = 12; Nk = 6; break;
	case 256: Nr = 14; Nk = 8; break;
	default: return;
	}

	if (key != NULL)
	{
		for (idx = 0; idx < Nk; ++idx) {
			w[idx] = ((key[4 * idx]) << 24) | ((key[4 * idx + 1]) << 16) |
				((key[4 * idx + 2]) << 8) | ((key[4 * idx + 3]));
		}
	}

	for (idx = Nk; idx < Nb * (Nr + 1); ++idx) {
		temp = w[idx - 1];
		if ((idx % Nk) == 0)
			temp = cmtAESSubWord(CMT_KE_ROTWORD(temp)) ^ Rcon[(idx - 1) / Nk];
		else if (Nk > 6 && (idx % Nk) == 4)
			temp = cmtAESSubWord(temp);
		w[idx] = w[idx - Nk] ^ temp;
	}
	free(key);
}

/////////////////
// 轮密钥扩展
/////////////////


void cmtAESRoundKeyInit(cmtUint8 state[][4], const cmtUint32 w[])
{
	cmtUint8 subkey[4];

	//subkey 1
	subkey[0] = w[0] >> 24;
	subkey[1] = w[0] >> 16;
	subkey[2] = w[0] >> 8;
	subkey[3] = w[0];
	state[0][0] ^= subkey[0];
	state[1][0] ^= subkey[1];
	state[2][0] ^= subkey[2];
	state[3][0] ^= subkey[3];
	// Subkey 2
	subkey[0] = w[1] >> 24;
	subkey[1] = w[1] >> 16;
	subkey[2] = w[1] >> 8;
	subkey[3] = w[1];
	state[0][1] ^= subkey[0];
	state[1][1] ^= subkey[1];
	state[2][1] ^= subkey[2];
	state[3][1] ^= subkey[3];
	// Subkey 3
	subkey[0] = w[2] >> 24;
	subkey[1] = w[2] >> 16;
	subkey[2] = w[2] >> 8;
	subkey[3] = w[2];
	state[0][2] ^= subkey[0];
	state[1][2] ^= subkey[1];
	state[2][2] ^= subkey[2];
	state[3][2] ^= subkey[3];
	// Subkey 4
	subkey[0] = w[3] >> 24;
	subkey[1] = w[3] >> 16;
	subkey[2] = w[3] >> 8;
	subkey[3] = w[3];
	state[0][3] ^= subkey[0];
	state[1][3] ^= subkey[1];
	state[2][3] ^= subkey[2];
	state[3][3] ^= subkey[3];
}

/////////////////
//cmtAESSubBytes
/////////////////

void cmtAESSubBytes(cmtUint8 state[][4])
{
	state[0][0] = cmtAESsBox[state[0][0] >> 4][state[0][0] & 0x0F];
	state[0][1] = cmtAESsBox[state[0][1] >> 4][state[0][1] & 0x0F];
	state[0][2] = cmtAESsBox[state[0][2] >> 4][state[0][2] & 0x0F];
	state[0][3] = cmtAESsBox[state[0][3] >> 4][state[0][3] & 0x0F];
	state[1][0] = cmtAESsBox[state[1][0] >> 4][state[1][0] & 0x0F];
	state[1][1] = cmtAESsBox[state[1][1] >> 4][state[1][1] & 0x0F];
	state[1][2] = cmtAESsBox[state[1][2] >> 4][state[1][2] & 0x0F];
	state[1][3] = cmtAESsBox[state[1][3] >> 4][state[1][3] & 0x0F];
	state[2][0] = cmtAESsBox[state[2][0] >> 4][state[2][0] & 0x0F];
	state[2][1] = cmtAESsBox[state[2][1] >> 4][state[2][1] & 0x0F];
	state[2][2] = cmtAESsBox[state[2][2] >> 4][state[2][2] & 0x0F];
	state[2][3] = cmtAESsBox[state[2][3] >> 4][state[2][3] & 0x0F];
	state[3][0] = cmtAESsBox[state[3][0] >> 4][state[3][0] & 0x0F];
	state[3][1] = cmtAESsBox[state[3][1] >> 4][state[3][1] & 0x0F];
	state[3][2] = cmtAESsBox[state[3][2] >> 4][state[3][2] & 0x0F];
	state[3][3] = cmtAESsBox[state[3][3] >> 4][state[3][3] & 0x0F];
}

void cmtInvSubBytes(cmtUint8 state[][4])
{
	state[0][0] = cmtAESinvsbox[state[0][0] >> 4][state[0][0] & 0x0F];
	state[0][1] = cmtAESinvsbox[state[0][1] >> 4][state[0][1] & 0x0F];
	state[0][2] = cmtAESinvsbox[state[0][2] >> 4][state[0][2] & 0x0F];
	state[0][3] = cmtAESinvsbox[state[0][3] >> 4][state[0][3] & 0x0F];
	state[1][0] = cmtAESinvsbox[state[1][0] >> 4][state[1][0] & 0x0F];
	state[1][1] = cmtAESinvsbox[state[1][1] >> 4][state[1][1] & 0x0F];
	state[1][2] = cmtAESinvsbox[state[1][2] >> 4][state[1][2] & 0x0F];
	state[1][3] = cmtAESinvsbox[state[1][3] >> 4][state[1][3] & 0x0F];
	state[2][0] = cmtAESinvsbox[state[2][0] >> 4][state[2][0] & 0x0F];
	state[2][1] = cmtAESinvsbox[state[2][1] >> 4][state[2][1] & 0x0F];
	state[2][2] = cmtAESinvsbox[state[2][2] >> 4][state[2][2] & 0x0F];
	state[2][3] = cmtAESinvsbox[state[2][3] >> 4][state[2][3] & 0x0F];
	state[3][0] = cmtAESinvsbox[state[3][0] >> 4][state[3][0] & 0x0F];
	state[3][1] = cmtAESinvsbox[state[3][1] >> 4][state[3][1] & 0x0F];
	state[3][2] = cmtAESinvsbox[state[3][2] >> 4][state[3][2] & 0x0F];
	state[3][3] = cmtAESinvsbox[state[3][3] >> 4][state[3][3] & 0x0F];
}

/////////////////
// (Inv)cmtShiftRows
/////////////////


void cmtShiftRows(cmtUint8 state[][4])
{
	int t;

	// Shift left by 1
	t = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = t;
	// Shift left by 2
	t = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = t;
	t = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = t;
	// Shift left by 3
	t = state[3][0];
	state[3][0] = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = t;
}


void cmtInvShiftRows(cmtUint8 state[][4])
{
	int t;

	// Shift right by 1
	t = state[1][3];
	state[1][3] = state[1][2];
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = t;
	// Shift right by 2
	t = state[2][3];
	state[2][3] = state[2][1];
	state[2][1] = t;
	t = state[2][2];
	state[2][2] = state[2][0];
	state[2][0] = t;
	// Shift right by 3
	t = state[3][3];
	state[3][3] = state[3][0];
	state[3][0] = state[3][1];
	state[3][1] = state[3][2];
	state[3][2] = t;
}

/////////////////
// (Inv)cmtMixColumns
/////////////////


void cmtMixColumns(cmtUint8 state[][4])
{
	cmtUint8 col[4];

	// Column 1
	col[0] = state[0][0];
	col[1] = state[1][0];
	col[2] = state[2][0];
	col[3] = state[3][0];
	state[0][0] = cmtGFMul[col[0]][0];
	state[0][0] ^= cmtGFMul[col[1]][1];
	state[0][0] ^= col[2];
	state[0][0] ^= col[3];
	state[1][0] = col[0];
	state[1][0] ^= cmtGFMul[col[1]][0];
	state[1][0] ^= cmtGFMul[col[2]][1];
	state[1][0] ^= col[3];
	state[2][0] = col[0];
	state[2][0] ^= col[1];
	state[2][0] ^= cmtGFMul[col[2]][0];
	state[2][0] ^= cmtGFMul[col[3]][1];
	state[3][0] = cmtGFMul[col[0]][1];
	state[3][0] ^= col[1];
	state[3][0] ^= col[2];
	state[3][0] ^= cmtGFMul[col[3]][0];
	// Column 2
	col[0] = state[0][1];
	col[1] = state[1][1];
	col[2] = state[2][1];
	col[3] = state[3][1];
	state[0][1] = cmtGFMul[col[0]][0];
	state[0][1] ^= cmtGFMul[col[1]][1];
	state[0][1] ^= col[2];
	state[0][1] ^= col[3];
	state[1][1] = col[0];
	state[1][1] ^= cmtGFMul[col[1]][0];
	state[1][1] ^= cmtGFMul[col[2]][1];
	state[1][1] ^= col[3];
	state[2][1] = col[0];
	state[2][1] ^= col[1];
	state[2][1] ^= cmtGFMul[col[2]][0];
	state[2][1] ^= cmtGFMul[col[3]][1];
	state[3][1] = cmtGFMul[col[0]][1];
	state[3][1] ^= col[1];
	state[3][1] ^= col[2];
	state[3][1] ^= cmtGFMul[col[3]][0];
	// Column 3
	col[0] = state[0][2];
	col[1] = state[1][2];
	col[2] = state[2][2];
	col[3] = state[3][2];
	state[0][2] = cmtGFMul[col[0]][0];
	state[0][2] ^= cmtGFMul[col[1]][1];
	state[0][2] ^= col[2];
	state[0][2] ^= col[3];
	state[1][2] = col[0];
	state[1][2] ^= cmtGFMul[col[1]][0];
	state[1][2] ^= cmtGFMul[col[2]][1];
	state[1][2] ^= col[3];
	state[2][2] = col[0];
	state[2][2] ^= col[1];
	state[2][2] ^= cmtGFMul[col[2]][0];
	state[2][2] ^= cmtGFMul[col[3]][1];
	state[3][2] = cmtGFMul[col[0]][1];
	state[3][2] ^= col[1];
	state[3][2] ^= col[2];
	state[3][2] ^= cmtGFMul[col[3]][0];
	// Column 4
	col[0] = state[0][3];
	col[1] = state[1][3];
	col[2] = state[2][3];
	col[3] = state[3][3];
	state[0][3] = cmtGFMul[col[0]][0];
	state[0][3] ^= cmtGFMul[col[1]][1];
	state[0][3] ^= col[2];
	state[0][3] ^= col[3];
	state[1][3] = col[0];
	state[1][3] ^= cmtGFMul[col[1]][0];
	state[1][3] ^= cmtGFMul[col[2]][1];
	state[1][3] ^= col[3];
	state[2][3] = col[0];
	state[2][3] ^= col[1];
	state[2][3] ^= cmtGFMul[col[2]][0];
	state[2][3] ^= cmtGFMul[col[3]][1];
	state[3][3] = cmtGFMul[col[0]][1];
	state[3][3] ^= col[1];
	state[3][3] ^= col[2];
	state[3][3] ^= cmtGFMul[col[3]][0];
}

void cmtInvMixColumns(cmtUint8 state[][4])
{
	cmtUint8 col[4];

	// Column 1
	col[0] = state[0][0];
	col[1] = state[1][0];
	col[2] = state[2][0];
	col[3] = state[3][0];
	state[0][0] = cmtGFMul[col[0]][5];
	state[0][0] ^= cmtGFMul[col[1]][3];
	state[0][0] ^= cmtGFMul[col[2]][4];
	state[0][0] ^= cmtGFMul[col[3]][2];
	state[1][0] = cmtGFMul[col[0]][2];
	state[1][0] ^= cmtGFMul[col[1]][5];
	state[1][0] ^= cmtGFMul[col[2]][3];
	state[1][0] ^= cmtGFMul[col[3]][4];
	state[2][0] = cmtGFMul[col[0]][4];
	state[2][0] ^= cmtGFMul[col[1]][2];
	state[2][0] ^= cmtGFMul[col[2]][5];
	state[2][0] ^= cmtGFMul[col[3]][3];
	state[3][0] = cmtGFMul[col[0]][3];
	state[3][0] ^= cmtGFMul[col[1]][4];
	state[3][0] ^= cmtGFMul[col[2]][2];
	state[3][0] ^= cmtGFMul[col[3]][5];
	// Column 2
	col[0] = state[0][1];
	col[1] = state[1][1];
	col[2] = state[2][1];
	col[3] = state[3][1];
	state[0][1] = cmtGFMul[col[0]][5];
	state[0][1] ^= cmtGFMul[col[1]][3];
	state[0][1] ^= cmtGFMul[col[2]][4];
	state[0][1] ^= cmtGFMul[col[3]][2];
	state[1][1] = cmtGFMul[col[0]][2];
	state[1][1] ^= cmtGFMul[col[1]][5];
	state[1][1] ^= cmtGFMul[col[2]][3];
	state[1][1] ^= cmtGFMul[col[3]][4];
	state[2][1] = cmtGFMul[col[0]][4];
	state[2][1] ^= cmtGFMul[col[1]][2];
	state[2][1] ^= cmtGFMul[col[2]][5];
	state[2][1] ^= cmtGFMul[col[3]][3];
	state[3][1] = cmtGFMul[col[0]][3];
	state[3][1] ^= cmtGFMul[col[1]][4];
	state[3][1] ^= cmtGFMul[col[2]][2];
	state[3][1] ^= cmtGFMul[col[3]][5];
	// Column 3
	col[0] = state[0][2];
	col[1] = state[1][2];
	col[2] = state[2][2];
	col[3] = state[3][2];
	state[0][2] = cmtGFMul[col[0]][5];
	state[0][2] ^= cmtGFMul[col[1]][3];
	state[0][2] ^= cmtGFMul[col[2]][4];
	state[0][2] ^= cmtGFMul[col[3]][2];
	state[1][2] = cmtGFMul[col[0]][2];
	state[1][2] ^= cmtGFMul[col[1]][5];
	state[1][2] ^= cmtGFMul[col[2]][3];
	state[1][2] ^= cmtGFMul[col[3]][4];
	state[2][2] = cmtGFMul[col[0]][4];
	state[2][2] ^= cmtGFMul[col[1]][2];
	state[2][2] ^= cmtGFMul[col[2]][5];
	state[2][2] ^= cmtGFMul[col[3]][3];
	state[3][2] = cmtGFMul[col[0]][3];
	state[3][2] ^= cmtGFMul[col[1]][4];
	state[3][2] ^= cmtGFMul[col[2]][2];
	state[3][2] ^= cmtGFMul[col[3]][5];
	// Column 4
	col[0] = state[0][3];
	col[1] = state[1][3];
	col[2] = state[2][3];
	col[3] = state[3][3];
	state[0][3] = cmtGFMul[col[0]][5];
	state[0][3] ^= cmtGFMul[col[1]][3];
	state[0][3] ^= cmtGFMul[col[2]][4];
	state[0][3] ^= cmtGFMul[col[3]][2];
	state[1][3] = cmtGFMul[col[0]][2];
	state[1][3] ^= cmtGFMul[col[1]][5];
	state[1][3] ^= cmtGFMul[col[2]][3];
	state[1][3] ^= cmtGFMul[col[3]][4];
	state[2][3] = cmtGFMul[col[0]][4];
	state[2][3] ^= cmtGFMul[col[1]][2];
	state[2][3] ^= cmtGFMul[col[2]][5];
	state[2][3] ^= cmtGFMul[col[3]][3];
	state[3][3] = cmtGFMul[col[0]][3];
	state[3][3] ^= cmtGFMul[col[1]][4];
	state[3][3] ^= cmtGFMul[col[2]][2];
	state[3][3] ^= cmtGFMul[col[3]][5];
}

/////////////////
// (En/De)Crypt
/////////////////

void cmtAESenc(const cmtUint8 in[], cmtUint8 out[], const cmtUint32 key[], int keysize)
{
	cmtUint8 state[4][4];

	state[0][0] = in[0];
	state[1][0] = in[1];
	state[2][0] = in[2];
	state[3][0] = in[3];
	state[0][1] = in[4];
	state[1][1] = in[5];
	state[2][1] = in[6];
	state[3][1] = in[7];
	state[0][2] = in[8];
	state[1][2] = in[9];
	state[2][2] = in[10];
	state[3][2] = in[11];
	state[0][3] = in[12];
	state[1][3] = in[13];
	state[2][3] = in[14];
	state[3][3] = in[15];

	cmtAESRoundKeyInit(state, &key[0]);
	cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[4]);
	cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[8]);
	cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[12]);
	cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[16]);
	cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[20]);
	cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[24]);
	cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[28]);
	cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[32]);
	cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[36]);
	if (keysize != 128) {
		cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[40]);
		cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[44]);
		if (keysize != 192) {
			cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[48]);
			cmtAESSubBytes(state); cmtShiftRows(state); cmtMixColumns(state); cmtAESRoundKeyInit(state, &key[52]);
			cmtAESSubBytes(state); cmtShiftRows(state); cmtAESRoundKeyInit(state, &key[56]);
		}
		else {
			cmtAESSubBytes(state); cmtShiftRows(state); cmtAESRoundKeyInit(state, &key[48]);
		}
	}
	else {
		cmtAESSubBytes(state); cmtShiftRows(state); cmtAESRoundKeyInit(state, &key[40]);
	}

	out[0] = state[0][0];
	out[1] = state[1][0];
	out[2] = state[2][0];
	out[3] = state[3][0];
	out[4] = state[0][1];
	out[5] = state[1][1];
	out[6] = state[2][1];
	out[7] = state[3][1];
	out[8] = state[0][2];
	out[9] = state[1][2];
	out[10] = state[2][2];
	out[11] = state[3][2];
	out[12] = state[0][3];
	out[13] = state[1][3];
	out[14] = state[2][3];
	out[15] = state[3][3];
}

void cmtAESdec(const cmtUint8 in[], cmtUint8 out[], const cmtUint32 key[], int keysize)
{
	cmtUint8 state[4][4];

	state[0][0] = in[0];
	state[1][0] = in[1];
	state[2][0] = in[2];
	state[3][0] = in[3];
	state[0][1] = in[4];
	state[1][1] = in[5];
	state[2][1] = in[6];
	state[3][1] = in[7];
	state[0][2] = in[8];
	state[1][2] = in[9];
	state[2][2] = in[10];
	state[3][2] = in[11];
	state[0][3] = in[12];
	state[1][3] = in[13];
	state[2][3] = in[14];
	state[3][3] = in[15];

	if (keysize > 128) {
		if (keysize > 192) {
			cmtAESRoundKeyInit(state, &key[56]);
			cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[52]); cmtInvMixColumns(state);
			cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[48]); cmtInvMixColumns(state);
		}
		else {
			cmtAESRoundKeyInit(state, &key[48]);
		}
		cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[44]); cmtInvMixColumns(state);
		cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[40]); cmtInvMixColumns(state);
	}
	else {
		cmtAESRoundKeyInit(state, &key[40]);
	}
	cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[36]); cmtInvMixColumns(state);
	cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[32]); cmtInvMixColumns(state);
	cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[28]); cmtInvMixColumns(state);
	cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[24]); cmtInvMixColumns(state);
	cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[20]); cmtInvMixColumns(state);
	cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[16]); cmtInvMixColumns(state);
	cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[12]); cmtInvMixColumns(state);
	cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[8]); cmtInvMixColumns(state);
	cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[4]); cmtInvMixColumns(state);
	cmtInvShiftRows(state); cmtInvSubBytes(state); cmtAESRoundKeyInit(state, &key[0]);

	out[0] = state[0][0];
	out[1] = state[1][0];
	out[2] = state[2][0];
	out[3] = state[3][0];
	out[4] = state[0][1];
	out[5] = state[1][1];
	out[6] = state[2][1];
	out[7] = state[3][1];
	out[8] = state[0][2];
	out[9] = state[1][2];
	out[10] = state[2][2];
	out[11] = state[3][2];
	out[12] = state[0][3];
	out[13] = state[1][3];
	out[14] = state[2][3];
	out[15] = state[3][3];
}