/*

作者:韩智鸿

han.zhihong@qq.com

*/


#ifndef __CK_PRINTF_MSG_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "os_define.h"
	
//编译类型语言转换设置
#define TO_UTF8(src,src_len,dst)		Gb2312ToUtf8((const char*)src, (int)src_len,(char *)dst)
#define FROM_UTF8(src,src_len,dst)		Utf8ToGb2312((const char*)src, (int)src_len,(char *)dst)

/*****
*
* 启用或关闭打印信息
*/
void PrintMsgInit();
void PrintMsgEnable(int isEnable);
int getMsgErrCount(); //获取错误消息数量

/*****
*
* 打印
*/

void SYS_PRINTF(char*format,...);   //白字
void ERR_PRINTF(char*format,...);   //红色字

void ALERT_PRINTF_UTF8(char*format,...);  //粉红字
void DEBUG_PRINTF_UTF8(char*format,...);  //绿色字
void WARNING_PRINTF_UTF8(char*format,...);//黄色字
		
/*****
*
* DEBUG_PRINT_HEX16 打印16进制数据
*/
void DEBUG_PRINT_HEX16(unsigned char* data,int len,char*format,...);

#ifdef __cplusplus
}
#endif
#define __CK_PRINTF_MSG_H_
#endif
