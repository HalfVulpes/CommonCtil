//（写几个中文在这控制字符集）
#include <cmtBign.h>

void cmtBignConfInit(cmtConf* conf)
{
	//cmtStr2Bign
	cmtStr2Bign = cmtStr2BignSoft64;

	//cmtBignNeg
	cmtBignNeg = cmtBignNegSoft64;
}

void (*cmtStr2Bign)(cmtU8str* in, cmtBigNum* out);

void cmtStr2BignSoft64(cmtU8str* in, cmtBigNum* out)
{

}

void (*cmtBignNeg)(cmtBigNum* in, cmtBigNum* out);