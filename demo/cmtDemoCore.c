/**
* @file cmtDemoCore.c
* @date 2021-12-22
* @author dexnab
*/
#include <cmtDemoCore.h>

void cmtDemoBSR()
{
	cmtUint64 num = 0x01040224;//0001 0000 0100 0000 0010 0010 0100
	cmtUint8 ret;

	//测试1：非零数
	ret = cmtBSR(num);
	//标答：ret=24

	//测试2：零
	ret = cmtBSR(0);
	//标答：ret=0
}