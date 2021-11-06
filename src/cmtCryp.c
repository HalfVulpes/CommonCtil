#include <cmtCryp.h>

/*--------------------------------常量量定义 开始-------------------------------*/

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

static const cmtUint8 cmtAESsBox[16][16] = {
	{0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76},
	{0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0},
	{0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15},
	{0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75},
	{0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84},
	{0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF},
	{0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8},
	{0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2},
	{0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73},
	{0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB},
	{0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79},
	{0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08},
	{0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A},
	{0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E},
	{0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF},
	{0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16}
};

static const cmtUint8 cmtAESinvsbox[16][16] = {
	{0x52,0x09,0x6A,0xD5,0x30,0x36,0xA5,0x38,0xBF,0x40,0xA3,0x9E,0x81,0xF3,0xD7,0xFB},
	{0x7C,0xE3,0x39,0x82,0x9B,0x2F,0xFF,0x87,0x34,0x8E,0x43,0x44,0xC4,0xDE,0xE9,0xCB},
	{0x54,0x7B,0x94,0x32,0xA6,0xC2,0x23,0x3D,0xEE,0x4C,0x95,0x0B,0x42,0xFA,0xC3,0x4E},
	{0x08,0x2E,0xA1,0x66,0x28,0xD9,0x24,0xB2,0x76,0x5B,0xA2,0x49,0x6D,0x8B,0xD1,0x25},
	{0x72,0xF8,0xF6,0x64,0x86,0x68,0x98,0x16,0xD4,0xA4,0x5C,0xCC,0x5D,0x65,0xB6,0x92},
	{0x6C,0x70,0x48,0x50,0xFD,0xED,0xB9,0xDA,0x5E,0x15,0x46,0x57,0xA7,0x8D,0x9D,0x84},
	{0x90,0xD8,0xAB,0x00,0x8C,0xBC,0xD3,0x0A,0xF7,0xE4,0x58,0x05,0xB8,0xB3,0x45,0x06},
	{0xD0,0x2C,0x1E,0x8F,0xCA,0x3F,0x0F,0x02,0xC1,0xAF,0xBD,0x03,0x01,0x13,0x8A,0x6B},
	{0x3A,0x91,0x11,0x41,0x4F,0x67,0xDC,0xEA,0x97,0xF2,0xCF,0xCE,0xF0,0xB4,0xE6,0x73},
	{0x96,0xAC,0x74,0x22,0xE7,0xAD,0x35,0x85,0xE2,0xF9,0x37,0xE8,0x1C,0x75,0xDF,0x6E},
	{0x47,0xF1,0x1A,0x71,0x1D,0x29,0xC5,0x89,0x6F,0xB7,0x62,0x0E,0xAA,0x18,0xBE,0x1B},
	{0xFC,0x56,0x3E,0x4B,0xC6,0xD2,0x79,0x20,0x9A,0xDB,0xC0,0xFE,0x78,0xCD,0x5A,0xF4},
	{0x1F,0xDD,0xA8,0x33,0x88,0x07,0xC7,0x31,0xB1,0x12,0x10,0x59,0x27,0x80,0xEC,0x5F},
	{0x60,0x51,0x7F,0xA9,0x19,0xB5,0x4A,0x0D,0x2D,0xE5,0x7A,0x9F,0x93,0xC9,0x9C,0xEF},
	{0xA0,0xE0,0x3B,0x4D,0xAE,0x2A,0xF5,0xB0,0xC8,0xEB,0xBB,0x3C,0x83,0x53,0x99,0x61},
	{0x17,0x2B,0x04,0x7E,0xBA,0x77,0xD6,0x26,0xE1,0x69,0x14,0x63,0x55,0x21,0x0C,0x7D}
};

// 这个矩阵用于储存所有 GF(2^8) 的计算结果。
// 只在 cmtMixColumns 中使用
static const cmtUint8 cmtGFMul[256][6] = {
	{0x00,0x00,0x00,0x00,0x00,0x00},{0x02,0x03,0x09,0x0b,0x0d,0x0e},
	{0x04,0x06,0x12,0x16,0x1a,0x1c},{0x06,0x05,0x1b,0x1d,0x17,0x12},
	{0x08,0x0c,0x24,0x2c,0x34,0x38},{0x0a,0x0f,0x2d,0x27,0x39,0x36},
	{0x0c,0x0a,0x36,0x3a,0x2e,0x24},{0x0e,0x09,0x3f,0x31,0x23,0x2a},
	{0x10,0x18,0x48,0x58,0x68,0x70},{0x12,0x1b,0x41,0x53,0x65,0x7e},
	{0x14,0x1e,0x5a,0x4e,0x72,0x6c},{0x16,0x1d,0x53,0x45,0x7f,0x62},
	{0x18,0x14,0x6c,0x74,0x5c,0x48},{0x1a,0x17,0x65,0x7f,0x51,0x46},
	{0x1c,0x12,0x7e,0x62,0x46,0x54},{0x1e,0x11,0x77,0x69,0x4b,0x5a},
	{0x20,0x30,0x90,0xb0,0xd0,0xe0},{0x22,0x33,0x99,0xbb,0xdd,0xee},
	{0x24,0x36,0x82,0xa6,0xca,0xfc},{0x26,0x35,0x8b,0xad,0xc7,0xf2},
	{0x28,0x3c,0xb4,0x9c,0xe4,0xd8},{0x2a,0x3f,0xbd,0x97,0xe9,0xd6},
	{0x2c,0x3a,0xa6,0x8a,0xfe,0xc4},{0x2e,0x39,0xaf,0x81,0xf3,0xca},
	{0x30,0x28,0xd8,0xe8,0xb8,0x90},{0x32,0x2b,0xd1,0xe3,0xb5,0x9e},
	{0x34,0x2e,0xca,0xfe,0xa2,0x8c},{0x36,0x2d,0xc3,0xf5,0xaf,0x82},
	{0x38,0x24,0xfc,0xc4,0x8c,0xa8},{0x3a,0x27,0xf5,0xcf,0x81,0xa6},
	{0x3c,0x22,0xee,0xd2,0x96,0xb4},{0x3e,0x21,0xe7,0xd9,0x9b,0xba},
	{0x40,0x60,0x3b,0x7b,0xbb,0xdb},{0x42,0x63,0x32,0x70,0xb6,0xd5},
	{0x44,0x66,0x29,0x6d,0xa1,0xc7},{0x46,0x65,0x20,0x66,0xac,0xc9},
	{0x48,0x6c,0x1f,0x57,0x8f,0xe3},{0x4a,0x6f,0x16,0x5c,0x82,0xed},
	{0x4c,0x6a,0x0d,0x41,0x95,0xff},{0x4e,0x69,0x04,0x4a,0x98,0xf1},
	{0x50,0x78,0x73,0x23,0xd3,0xab},{0x52,0x7b,0x7a,0x28,0xde,0xa5},
	{0x54,0x7e,0x61,0x35,0xc9,0xb7},{0x56,0x7d,0x68,0x3e,0xc4,0xb9},
	{0x58,0x74,0x57,0x0f,0xe7,0x93},{0x5a,0x77,0x5e,0x04,0xea,0x9d},
	{0x5c,0x72,0x45,0x19,0xfd,0x8f},{0x5e,0x71,0x4c,0x12,0xf0,0x81},
	{0x60,0x50,0xab,0xcb,0x6b,0x3b},{0x62,0x53,0xa2,0xc0,0x66,0x35},
	{0x64,0x56,0xb9,0xdd,0x71,0x27},{0x66,0x55,0xb0,0xd6,0x7c,0x29},
	{0x68,0x5c,0x8f,0xe7,0x5f,0x03},{0x6a,0x5f,0x86,0xec,0x52,0x0d},
	{0x6c,0x5a,0x9d,0xf1,0x45,0x1f},{0x6e,0x59,0x94,0xfa,0x48,0x11},
	{0x70,0x48,0xe3,0x93,0x03,0x4b},{0x72,0x4b,0xea,0x98,0x0e,0x45},
	{0x74,0x4e,0xf1,0x85,0x19,0x57},{0x76,0x4d,0xf8,0x8e,0x14,0x59},
	{0x78,0x44,0xc7,0xbf,0x37,0x73},{0x7a,0x47,0xce,0xb4,0x3a,0x7d},
	{0x7c,0x42,0xd5,0xa9,0x2d,0x6f},{0x7e,0x41,0xdc,0xa2,0x20,0x61},
	{0x80,0xc0,0x76,0xf6,0x6d,0xad},{0x82,0xc3,0x7f,0xfd,0x60,0xa3},
	{0x84,0xc6,0x64,0xe0,0x77,0xb1},{0x86,0xc5,0x6d,0xeb,0x7a,0xbf},
	{0x88,0xcc,0x52,0xda,0x59,0x95},{0x8a,0xcf,0x5b,0xd1,0x54,0x9b},
	{0x8c,0xca,0x40,0xcc,0x43,0x89},{0x8e,0xc9,0x49,0xc7,0x4e,0x87},
	{0x90,0xd8,0x3e,0xae,0x05,0xdd},{0x92,0xdb,0x37,0xa5,0x08,0xd3},
	{0x94,0xde,0x2c,0xb8,0x1f,0xc1},{0x96,0xdd,0x25,0xb3,0x12,0xcf},
	{0x98,0xd4,0x1a,0x82,0x31,0xe5},{0x9a,0xd7,0x13,0x89,0x3c,0xeb},
	{0x9c,0xd2,0x08,0x94,0x2b,0xf9},{0x9e,0xd1,0x01,0x9f,0x26,0xf7},
	{0xa0,0xf0,0xe6,0x46,0xbd,0x4d},{0xa2,0xf3,0xef,0x4d,0xb0,0x43},
	{0xa4,0xf6,0xf4,0x50,0xa7,0x51},{0xa6,0xf5,0xfd,0x5b,0xaa,0x5f},
	{0xa8,0xfc,0xc2,0x6a,0x89,0x75},{0xaa,0xff,0xcb,0x61,0x84,0x7b},
	{0xac,0xfa,0xd0,0x7c,0x93,0x69},{0xae,0xf9,0xd9,0x77,0x9e,0x67},
	{0xb0,0xe8,0xae,0x1e,0xd5,0x3d},{0xb2,0xeb,0xa7,0x15,0xd8,0x33},
	{0xb4,0xee,0xbc,0x08,0xcf,0x21},{0xb6,0xed,0xb5,0x03,0xc2,0x2f},
	{0xb8,0xe4,0x8a,0x32,0xe1,0x05},{0xba,0xe7,0x83,0x39,0xec,0x0b},
	{0xbc,0xe2,0x98,0x24,0xfb,0x19},{0xbe,0xe1,0x91,0x2f,0xf6,0x17},
	{0xc0,0xa0,0x4d,0x8d,0xd6,0x76},{0xc2,0xa3,0x44,0x86,0xdb,0x78},
	{0xc4,0xa6,0x5f,0x9b,0xcc,0x6a},{0xc6,0xa5,0x56,0x90,0xc1,0x64},
	{0xc8,0xac,0x69,0xa1,0xe2,0x4e},{0xca,0xaf,0x60,0xaa,0xef,0x40},
	{0xcc,0xaa,0x7b,0xb7,0xf8,0x52},{0xce,0xa9,0x72,0xbc,0xf5,0x5c},
	{0xd0,0xb8,0x05,0xd5,0xbe,0x06},{0xd2,0xbb,0x0c,0xde,0xb3,0x08},
	{0xd4,0xbe,0x17,0xc3,0xa4,0x1a},{0xd6,0xbd,0x1e,0xc8,0xa9,0x14},
	{0xd8,0xb4,0x21,0xf9,0x8a,0x3e},{0xda,0xb7,0x28,0xf2,0x87,0x30},
	{0xdc,0xb2,0x33,0xef,0x90,0x22},{0xde,0xb1,0x3a,0xe4,0x9d,0x2c},
	{0xe0,0x90,0xdd,0x3d,0x06,0x96},{0xe2,0x93,0xd4,0x36,0x0b,0x98},
	{0xe4,0x96,0xcf,0x2b,0x1c,0x8a},{0xe6,0x95,0xc6,0x20,0x11,0x84},
	{0xe8,0x9c,0xf9,0x11,0x32,0xae},{0xea,0x9f,0xf0,0x1a,0x3f,0xa0},
	{0xec,0x9a,0xeb,0x07,0x28,0xb2},{0xee,0x99,0xe2,0x0c,0x25,0xbc},
	{0xf0,0x88,0x95,0x65,0x6e,0xe6},{0xf2,0x8b,0x9c,0x6e,0x63,0xe8},
	{0xf4,0x8e,0x87,0x73,0x74,0xfa},{0xf6,0x8d,0x8e,0x78,0x79,0xf4},
	{0xf8,0x84,0xb1,0x49,0x5a,0xde},{0xfa,0x87,0xb8,0x42,0x57,0xd0},
	{0xfc,0x82,0xa3,0x5f,0x40,0xc2},{0xfe,0x81,0xaa,0x54,0x4d,0xcc},
	{0x1b,0x9b,0xec,0xf7,0xda,0x41},{0x19,0x98,0xe5,0xfc,0xd7,0x4f},
	{0x1f,0x9d,0xfe,0xe1,0xc0,0x5d},{0x1d,0x9e,0xf7,0xea,0xcd,0x53},
	{0x13,0x97,0xc8,0xdb,0xee,0x79},{0x11,0x94,0xc1,0xd0,0xe3,0x77},
	{0x17,0x91,0xda,0xcd,0xf4,0x65},{0x15,0x92,0xd3,0xc6,0xf9,0x6b},
	{0x0b,0x83,0xa4,0xaf,0xb2,0x31},{0x09,0x80,0xad,0xa4,0xbf,0x3f},
	{0x0f,0x85,0xb6,0xb9,0xa8,0x2d},{0x0d,0x86,0xbf,0xb2,0xa5,0x23},
	{0x03,0x8f,0x80,0x83,0x86,0x09},{0x01,0x8c,0x89,0x88,0x8b,0x07},
	{0x07,0x89,0x92,0x95,0x9c,0x15},{0x05,0x8a,0x9b,0x9e,0x91,0x1b},
	{0x3b,0xab,0x7c,0x47,0x0a,0xa1},{0x39,0xa8,0x75,0x4c,0x07,0xaf},
	{0x3f,0xad,0x6e,0x51,0x10,0xbd},{0x3d,0xae,0x67,0x5a,0x1d,0xb3},
	{0x33,0xa7,0x58,0x6b,0x3e,0x99},{0x31,0xa4,0x51,0x60,0x33,0x97},
	{0x37,0xa1,0x4a,0x7d,0x24,0x85},{0x35,0xa2,0x43,0x76,0x29,0x8b},
	{0x2b,0xb3,0x34,0x1f,0x62,0xd1},{0x29,0xb0,0x3d,0x14,0x6f,0xdf},
	{0x2f,0xb5,0x26,0x09,0x78,0xcd},{0x2d,0xb6,0x2f,0x02,0x75,0xc3},
	{0x23,0xbf,0x10,0x33,0x56,0xe9},{0x21,0xbc,0x19,0x38,0x5b,0xe7},
	{0x27,0xb9,0x02,0x25,0x4c,0xf5},{0x25,0xba,0x0b,0x2e,0x41,0xfb},
	{0x5b,0xfb,0xd7,0x8c,0x61,0x9a},{0x59,0xf8,0xde,0x87,0x6c,0x94},
	{0x5f,0xfd,0xc5,0x9a,0x7b,0x86},{0x5d,0xfe,0xcc,0x91,0x76,0x88},
	{0x53,0xf7,0xf3,0xa0,0x55,0xa2},{0x51,0xf4,0xfa,0xab,0x58,0xac},
	{0x57,0xf1,0xe1,0xb6,0x4f,0xbe},{0x55,0xf2,0xe8,0xbd,0x42,0xb0},
	{0x4b,0xe3,0x9f,0xd4,0x09,0xea},{0x49,0xe0,0x96,0xdf,0x04,0xe4},
	{0x4f,0xe5,0x8d,0xc2,0x13,0xf6},{0x4d,0xe6,0x84,0xc9,0x1e,0xf8},
	{0x43,0xef,0xbb,0xf8,0x3d,0xd2},{0x41,0xec,0xb2,0xf3,0x30,0xdc},
	{0x47,0xe9,0xa9,0xee,0x27,0xce},{0x45,0xea,0xa0,0xe5,0x2a,0xc0},
	{0x7b,0xcb,0x47,0x3c,0xb1,0x7a},{0x79,0xc8,0x4e,0x37,0xbc,0x74},
	{0x7f,0xcd,0x55,0x2a,0xab,0x66},{0x7d,0xce,0x5c,0x21,0xa6,0x68},
	{0x73,0xc7,0x63,0x10,0x85,0x42},{0x71,0xc4,0x6a,0x1b,0x88,0x4c},
	{0x77,0xc1,0x71,0x06,0x9f,0x5e},{0x75,0xc2,0x78,0x0d,0x92,0x50},
	{0x6b,0xd3,0x0f,0x64,0xd9,0x0a},{0x69,0xd0,0x06,0x6f,0xd4,0x04},
	{0x6f,0xd5,0x1d,0x72,0xc3,0x16},{0x6d,0xd6,0x14,0x79,0xce,0x18},
	{0x63,0xdf,0x2b,0x48,0xed,0x32},{0x61,0xdc,0x22,0x43,0xe0,0x3c},
	{0x67,0xd9,0x39,0x5e,0xf7,0x2e},{0x65,0xda,0x30,0x55,0xfa,0x20},
	{0x9b,0x5b,0x9a,0x01,0xb7,0xec},{0x99,0x58,0x93,0x0a,0xba,0xe2},
	{0x9f,0x5d,0x88,0x17,0xad,0xf0},{0x9d,0x5e,0x81,0x1c,0xa0,0xfe},
	{0x93,0x57,0xbe,0x2d,0x83,0xd4},{0x91,0x54,0xb7,0x26,0x8e,0xda},
	{0x97,0x51,0xac,0x3b,0x99,0xc8},{0x95,0x52,0xa5,0x30,0x94,0xc6},
	{0x8b,0x43,0xd2,0x59,0xdf,0x9c},{0x89,0x40,0xdb,0x52,0xd2,0x92},
	{0x8f,0x45,0xc0,0x4f,0xc5,0x80},{0x8d,0x46,0xc9,0x44,0xc8,0x8e},
	{0x83,0x4f,0xf6,0x75,0xeb,0xa4},{0x81,0x4c,0xff,0x7e,0xe6,0xaa},
	{0x87,0x49,0xe4,0x63,0xf1,0xb8},{0x85,0x4a,0xed,0x68,0xfc,0xb6},
	{0xbb,0x6b,0x0a,0xb1,0x67,0x0c},{0xb9,0x68,0x03,0xba,0x6a,0x02},
	{0xbf,0x6d,0x18,0xa7,0x7d,0x10},{0xbd,0x6e,0x11,0xac,0x70,0x1e},
	{0xb3,0x67,0x2e,0x9d,0x53,0x34},{0xb1,0x64,0x27,0x96,0x5e,0x3a},
	{0xb7,0x61,0x3c,0x8b,0x49,0x28},{0xb5,0x62,0x35,0x80,0x44,0x26},
	{0xab,0x73,0x42,0xe9,0x0f,0x7c},{0xa9,0x70,0x4b,0xe2,0x02,0x72},
	{0xaf,0x75,0x50,0xff,0x15,0x60},{0xad,0x76,0x59,0xf4,0x18,0x6e},
	{0xa3,0x7f,0x66,0xc5,0x3b,0x44},{0xa1,0x7c,0x6f,0xce,0x36,0x4a},
	{0xa7,0x79,0x74,0xd3,0x21,0x58},{0xa5,0x7a,0x7d,0xd8,0x2c,0x56},
	{0xdb,0x3b,0xa1,0x7a,0x0c,0x37},{0xd9,0x38,0xa8,0x71,0x01,0x39},
	{0xdf,0x3d,0xb3,0x6c,0x16,0x2b},{0xdd,0x3e,0xba,0x67,0x1b,0x25},
	{0xd3,0x37,0x85,0x56,0x38,0x0f},{0xd1,0x34,0x8c,0x5d,0x35,0x01},
	{0xd7,0x31,0x97,0x40,0x22,0x13},{0xd5,0x32,0x9e,0x4b,0x2f,0x1d},
	{0xcb,0x23,0xe9,0x22,0x64,0x47},{0xc9,0x20,0xe0,0x29,0x69,0x49},
	{0xcf,0x25,0xfb,0x34,0x7e,0x5b},{0xcd,0x26,0xf2,0x3f,0x73,0x55},
	{0xc3,0x2f,0xcd,0x0e,0x50,0x7f},{0xc1,0x2c,0xc4,0x05,0x5d,0x71},
	{0xc7,0x29,0xdf,0x18,0x4a,0x63},{0xc5,0x2a,0xd6,0x13,0x47,0x6d},
	{0xfb,0x0b,0x31,0xca,0xdc,0xd7},{0xf9,0x08,0x38,0xc1,0xd1,0xd9},
	{0xff,0x0d,0x23,0xdc,0xc6,0xcb},{0xfd,0x0e,0x2a,0xd7,0xcb,0xc5},
	{0xf3,0x07,0x15,0xe6,0xe8,0xef},{0xf1,0x04,0x1c,0xed,0xe5,0xe1},
	{0xf7,0x01,0x07,0xf0,0xf2,0xf3},{0xf5,0x02,0x0e,0xfb,0xff,0xfd},
	{0xeb,0x13,0x79,0x92,0xb4,0xa7},{0xe9,0x10,0x70,0x99,0xb9,0xa9},
	{0xef,0x15,0x6b,0x84,0xae,0xbb},{0xed,0x16,0x62,0x8f,0xa3,0xb5},
	{0xe3,0x1f,0x5d,0xbe,0x80,0x9f},{0xe1,0x1c,0x54,0xb5,0x8d,0x91},
	{0xe7,0x19,0x4f,0xa8,0x9a,0x83},{0xe5,0x1a,0x46,0xa3,0x97,0x8d}
};

//用于 URL 的 Base64 非标准 Base64，是一种变种，因此如果你的base64要用于URL编码，请将该对照数组后面的'+' 和 '/' 替换为 '*' 和 '-'来满足URL标准的编码
static const cmtUint8 cmtCharSet[64] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };
/*--------------------------------常量量定义 结束--------------------------------*/

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

	for (i = 0; i < size; i++) {
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

void cmtAESkeyInit(cmtUint8* keystr, cmtUint8* sKeystr, cmtUint32* w, cmtUint16 keysize)
{
	cmtUint8 key[256];

	//字符串标准化为定长密钥数组
	if (keysize == 128)
	{
		cmtMD5 ctx;
		cmtMD5Init(&ctx);
		cmtMD5Update(&ctx, keystr, sKeystr);
		cmtMD5Get(&ctx, key);
	}
	else
	{
		cmtSHA256 ctx;
		cmtSHA256Init(&ctx);
		cmtSHA256Update(&ctx, keystr, sKeystr);
		cmtSHA256Get(&ctx, key);
	}

	//计算密钥组矩阵
	cmtAESRestrictkeyInit(key, w, keysize);
}

void cmtAESRestrictkeyInit(cmtUint8* key, cmtUint32* w, cmtUint16 keysize)
{
	cmtUint32 temp, Rcon[] = { 0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,
						   0x40000000,0x80000000,0x1b000000,0x36000000,0x6c000000,0xd8000000,
						   0xab000000,0x4d000000,0x9a000000 };
	cmtInt32 Nb = 4, Nr, Nk, idx;

	switch (keysize) {
	case 128: Nr = 10; Nk = 4; break;
	case 192: Nr = 12; Nk = 6; break;
	case 256: Nr = 14; Nk = 8; break;
	default: return;
	}

	for (idx = 0; idx < Nk; ++idx) {
		w[idx] = ((key[4 * idx]) << 24) | ((key[4 * idx + 1]) << 16) | ((key[4 * idx + 2]) << 8) | ((key[4 * idx + 3]));
	}

	for (idx = Nk; idx < Nb * (Nr + 1); ++idx) {
		temp = w[idx - 1];
		if ((idx % Nk) == 0)
			temp = cmtAESSubWord(CMT_KE_ROTWORD(temp)) ^ Rcon[(idx - 1) / Nk];
		else if (Nk > 6 && (idx % Nk) == 4)
			temp = cmtAESSubWord(temp);
		w[idx] = w[idx - Nk] ^ temp;
	}
}

void cmtAESInitialVectorInit(cmtUint8* iv)
{
	cmtUint8 random[8], random2[8];
	cmtUint8 seed[8];
	cmtMD5 ctx;

	cmtRealRand(&random, 1);
	cmtRealRand(&random2, 1);

	for (int i = 0; i < 4; i++)
	{
		seed[i] = random[7 - i];
		seed[i + 4] = random2[7 - i];
	}

	cmtMD5Init(&ctx);
	cmtMD5Update(&ctx, seed, 8);
	cmtMD5Get(&ctx, iv);
}

void cmtAESecbEnc(cmtUint8* in, cmtUint8* out, cmtUint32* key, cmtUint16 keysize)
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

void cmtAESecbDec(cmtUint8* in, cmtUint8* out, cmtUint32* key, cmtUint16 keysize)
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

void cmtAESecbEncEx(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize)
{
	cmtUint64 ExBlockOffset;//不完整块（如果存在）首偏移
	cmtUint8 ExBlockTemp[CMT_AES_BLOCK_SIZE];//填充完整的不完整块
	cmtUint64 r;

	ExBlockOffset = size - size % CMT_AES_BLOCK_SIZE;

	//完整块
	for (r = 0; r < ExBlockOffset; r += CMT_AES_BLOCK_SIZE)
		cmtAESecbEnc(in + r, out + r, key, keysize);

	//不完整块
	if (ExBlockOffset != size)
	{
		//低字节复制
		for (; r < size; r++)
			ExBlockTemp[r - ExBlockOffset] = in[r];
		//高字节填0
		for (r = size - ExBlockOffset; r < CMT_AES_BLOCK_SIZE; r++)
			ExBlockTemp[r] = 0;
		//加密
		cmtAESecbEnc(ExBlockTemp, out + ExBlockOffset, key, keysize);
	}
}

void cmtAESecbDecEx(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize)
{
	cmtUint64 r;

	for (r = 0; r < size; r += CMT_AES_BLOCK_SIZE)
		cmtAESecbDec(in + r, out + r, key, keysize);
}

void cmtAEScbcEnc(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize, cmtUint8* iv)
{
	cmtUint8 buf_in[CMT_AES_BLOCK_SIZE], buf_out[CMT_AES_BLOCK_SIZE], iv_buf[CMT_AES_BLOCK_SIZE];
	cmtUint64 blocks, idx;

	blocks = size / CMT_AES_BLOCK_SIZE;

	memcpy(iv_buf, iv, CMT_AES_BLOCK_SIZE);

	for (idx = 0; idx < blocks; idx++) {
		memcpy(buf_in, &in[idx * CMT_AES_BLOCK_SIZE], CMT_AES_BLOCK_SIZE);
		cmtXorBuffer(iv_buf, buf_in, CMT_AES_BLOCK_SIZE);
		cmtAESecbEnc(buf_in, buf_out, key, keysize);
		memcpy(&out[idx * CMT_AES_BLOCK_SIZE], buf_out, CMT_AES_BLOCK_SIZE);
		memcpy(iv_buf, buf_out, CMT_AES_BLOCK_SIZE);
	}
}

void cmtAEScbcEncMac(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize, cmtUint8* iv)
{
	cmtUint8 buf_in[CMT_AES_BLOCK_SIZE], buf_out[CMT_AES_BLOCK_SIZE], iv_buf[CMT_AES_BLOCK_SIZE];
	cmtUint64 blocks, idx;

	blocks = size / CMT_AES_BLOCK_SIZE;

	memcpy(iv_buf, iv, CMT_AES_BLOCK_SIZE);

	for (idx = 0; idx < blocks; idx++) {
		memcpy(buf_in, &in[idx * CMT_AES_BLOCK_SIZE], CMT_AES_BLOCK_SIZE);
		cmtXorBuffer(iv_buf, buf_in, CMT_AES_BLOCK_SIZE);
		cmtAESecbEnc(buf_in, buf_out, key, keysize);
		memcpy(iv_buf, buf_out, CMT_AES_BLOCK_SIZE);
		// 输出所有的块可能会溢出
	}

	memcpy(out, buf_out, CMT_AES_BLOCK_SIZE);// 只输出最后的块
}

void cmtAEScbcDec(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize, cmtUint8* iv)
{
	cmtUint8 buf_in[CMT_AES_BLOCK_SIZE], buf_out[CMT_AES_BLOCK_SIZE], iv_buf[CMT_AES_BLOCK_SIZE];
	int blocks, idx;

	blocks = size / CMT_AES_BLOCK_SIZE;

	memcpy(iv_buf, iv, CMT_AES_BLOCK_SIZE);

	for (idx = 0; idx < blocks; idx++) {
		memcpy(buf_in, &in[idx * CMT_AES_BLOCK_SIZE], CMT_AES_BLOCK_SIZE);
		cmtAESecbDec(buf_in, buf_out, key, keysize);
		cmtXorBuffer(iv_buf, buf_out, CMT_AES_BLOCK_SIZE);
		memcpy(&out[idx * CMT_AES_BLOCK_SIZE], buf_out, CMT_AES_BLOCK_SIZE);
		memcpy(iv_buf, buf_in, CMT_AES_BLOCK_SIZE);
	}
}

void cmtAESctrEnc(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize, cmtUint8* iv)
{
	cmtUint64 idx = 0, last_block_length;
	cmtUint8 iv_buf[CMT_AES_BLOCK_SIZE], out_buf[CMT_AES_BLOCK_SIZE];

	if (in != out)
		memcpy(out, in, size);

	memcpy(iv_buf, iv, CMT_AES_BLOCK_SIZE);
	last_block_length = size - CMT_AES_BLOCK_SIZE;

	if (size > CMT_AES_BLOCK_SIZE) {
		for (idx = 0; idx < last_block_length; idx += CMT_AES_BLOCK_SIZE) {
			cmtAESecbEnc(iv_buf, out_buf, key, keysize);
			cmtXorBuffer(out_buf, &out[idx], CMT_AES_BLOCK_SIZE);
			cmtAESincrIV(iv_buf, CMT_AES_BLOCK_SIZE);
		}
	}

	cmtAESecbEnc(iv_buf, out_buf, key, keysize);
	cmtXorBuffer(out_buf, &out[idx], size - idx);   // 最有效的字节
}

void cmtAESctrDec(cmtUint8* in, cmtUint64 size, cmtUint8* out, cmtUint32* key, cmtUint16 keysize, cmtUint8* iv)
{
	//CTR的加密就是他自己的反函数
	cmtAESctrEnc(in, size, out, key, keysize, iv);
}

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

void cmtXorBuffer(const cmtUint8 in[], cmtUint8 out[], cmtUint64 size)
{
	cmtUint64 idx;

	for (idx = 0; idx < size; idx++)
		out[idx] ^= in[idx];
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

cmtUint32 cmtAESSubWord(cmtUint32 word)
{
	unsigned int result;

	result = (int)cmtAESsBox[(word >> 4) & 0x0000000F][word & 0x0000000F];
	result += (int)cmtAESsBox[(word >> 12) & 0x0000000F][(word >> 8) & 0x0000000F] << 8;
	result += (int)cmtAESsBox[(word >> 20) & 0x0000000F][(word >> 16) & 0x0000000F] << 16;
	result += (int)cmtAESsBox[(word >> 28) & 0x0000000F][(word >> 24) & 0x0000000F] << 24;
	return(result);
}

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

cmtUint8 cmtRevChar(char ch)
{
	if (ch >= 'A' && ch <= 'Z')
		ch -= 'A';
	else if (ch >= 'a' && ch <= 'z')
		ch = ch - 'a' + 26;
	else if (ch >= '0' && ch <= '9')
		ch = ch - '0' + 52;
	else if (ch == '+')
		ch = 62;
	else if (ch == '/')
		ch = 63;

	return(ch);
}

cmtInt64 cmtBase64Encode(const cmtUint8 in[], cmtUint8 out[], cmtInt64 size, int newLineFlag)
{
	cmtInt64 idx, idx2, blks, blkTop, leftOver, newLineCount = 0;

	blks = (size / 3);
	leftOver = size % 3;

	if (out == NULL) {
		idx2 = blks * 4;
		if (leftOver)
			idx2 += 4;
		if (newLineFlag)
			idx2 += size / 57;   // (CMT_NEWLINE_INVL / 4) * 3 = 57. 一行大概57个in[] bytes
	}
	else {
		// 将3个字节的数据编码成4个字节的数据 数据膨胀率33.3% 数据大小约等于原来的133.3%
		blkTop = blks * 3;
		for (idx = 0, idx2 = 0; idx < blkTop; idx += 3, idx2 += 4) {
			out[idx2] = cmtCharSet[in[idx] >> 2];
			out[idx2 + 1] = cmtCharSet[((in[idx] & 0x03) << 4) | (in[idx + 1] >> 4)];
			out[idx2 + 2] = cmtCharSet[((in[idx + 1] & 0x0f) << 2) | (in[idx + 2] >> 6)];
			out[idx2 + 3] = cmtCharSet[in[idx + 2] & 0x3F];
			// RFC 822 规定每76字节换行一次，如果NewLineFlag为1则换行来满足此规定
			// 第一行是77字节
			if (((idx2 - newLineCount + 4) % CMT_NEWLINE_INVL == 0) && newLineFlag) {
				out[idx2 + 4] = '\n';
				idx2++;
				newLineCount++;
			}
		}

		if (leftOver == 1) {
			out[idx2] = cmtCharSet[in[idx] >> 2];
			out[idx2 + 1] = cmtCharSet[(in[idx] & 0x03) << 4];
			out[idx2 + 2] = '=';
			out[idx2 + 3] = '=';
			idx2 += 4;
		}
		else if (leftOver == 2) {
			out[idx2] = cmtCharSet[in[idx] >> 2];
			out[idx2 + 1] = cmtCharSet[((in[idx] & 0x03) << 4) | (in[idx + 1] >> 4)];
			out[idx2 + 2] = cmtCharSet[(in[idx + 1] & 0x0F) << 2];
			out[idx2 + 3] = '=';
			idx2 += 4;
		}
	}

	return(idx2);
}

cmtInt64 cmtBase64Decode(const cmtUint8 in[], cmtUint8 out[], cmtInt64 size)
{
	cmtUint8 ch;
	cmtInt64 idx, idx2, blks, blkTop, leftOver;

	if (in[size - 1] == '=')
		size--;
	if (in[size - 1] == '=')
		size--;

	blks = size / 4;
	leftOver = size % 4;

	if (out == NULL) {
		if (size >= 77 && in[CMT_NEWLINE_INVL] == '\n')   //校验每行
			size -= size / (CMT_NEWLINE_INVL + 1);
		blks = size / 4;
		leftOver = size % 4;

		idx = blks * 3;
		if (leftOver == 2)
			idx++;
		else if (leftOver == 3)
			idx += 2;
	}
	else {
		blkTop = blks * 4;
		for (idx = 0, idx2 = 0; idx2 < blkTop; idx += 3, idx2 += 4) {
			if (in[idx2] == '\n')
				idx2++;
			out[idx] = (cmtRevChar(in[idx2]) << 2) | ((cmtRevChar(in[idx2 + 1]) & 0x30) >> 4);
			out[idx + 1] = (cmtRevChar(in[idx2 + 1]) << 4) | (cmtRevChar(in[idx2 + 2]) >> 2);
			out[idx + 2] = (cmtRevChar(in[idx2 + 2]) << 6) | cmtRevChar(in[idx2 + 3]);
		}

		if (leftOver == 2) {
			out[idx] = (cmtRevChar(in[idx2]) << 2) | ((cmtRevChar(in[idx2 + 1]) & 0x30) >> 4);
			idx++;
		}
		else if (leftOver == 3) {
			out[idx] = (cmtRevChar(in[idx2]) << 2) | ((cmtRevChar(in[idx2 + 1]) & 0x30) >> 4);
			out[idx + 1] = (cmtRevChar(in[idx2 + 1]) << 4) | (cmtRevChar(in[idx2 + 2]) >> 2);
			idx += 2;
		}
	}

	return(idx);
}