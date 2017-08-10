/*

����:���Ǻ�

han.zhihong@qq.com

*/


#ifndef __CK_PRINTF_MSG_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "os_define.h"
	
//������������ת������
#define TO_UTF8(src,src_len,dst)		Gb2312ToUtf8((const char*)src, (int)src_len,(char *)dst)
#define FROM_UTF8(src,src_len,dst)		Utf8ToGb2312((const char*)src, (int)src_len,(char *)dst)

/*****
*
* ���û�رմ�ӡ��Ϣ
*/
void PrintMsgInit();
void PrintMsgEnable(int isEnable);
int getMsgErrCount(); //��ȡ������Ϣ����

/*****
*
* ��ӡ
*/

void SYS_PRINTF(char*format,...);   //����
void ERR_PRINTF(char*format,...);   //��ɫ��

void ALERT_PRINTF_UTF8(char*format,...);  //�ۺ���
void DEBUG_PRINTF_UTF8(char*format,...);  //��ɫ��
void WARNING_PRINTF_UTF8(char*format,...);//��ɫ��
		
/*****
*
* DEBUG_PRINT_HEX16 ��ӡ16��������
*/
void DEBUG_PRINT_HEX16(unsigned char* data,int len,char*format,...);

#ifdef __cplusplus
}
#endif
#define __CK_PRINTF_MSG_H_
#endif
