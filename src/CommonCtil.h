//（中文字符 卡utf-8编码用）
#include <cmtType.h>

typedef struct _CMTCONF
{
	cmtUint8 aes : 1;
	cmtUint8 avx : 1;
	cmtUint8 avx2 : 1;
	cmtUint8 fpu : 1;
	cmtUint8 mmx : 1;
	cmtUint8 mmxext : 1;
	cmtUint8 movbe : 1;
	cmtUint8 msr : 1;
	cmtUint8 rdpid : 1;
	cmtUint8 rdpru : 1;
	cmtUint8 rdrand : 1;
	cmtUint8 rdtscp : 1;
	cmtUint8 rdseed : 1;
	cmtUint8 sha : 1;
	cmtUint8 sse1 : 1;
	cmtUint8 sse2 : 1;
	cmtUint8 sse3 : 1;
	cmtUint8 ssse3 : 1;
	cmtUint8 sse4a : 1;
	cmtUint8 sse41 : 1;
	cmtUint8 sse42 : 1;
	cmtUint8 rdtsc : 1;
}cmtConf;

//初始配置器
extern void cmtConfInit();