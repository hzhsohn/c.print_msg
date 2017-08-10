/*

作者:韩智鸿

han.zhihong@qq.com

*/


#ifndef __CK_PRINTF_MSG_H_
#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include "print_msg.h"
#include <time.h>
#include "encoding.h"

#ifdef _WIN32
		#include <windows.h>
		#define PRINTMSG_LOCK_CS(p)					EnterCriticalSection(p)	
		#define PRINTMSG_UNLOCK_CS(p)				LeaveCriticalSection(p)
		#define PRINTMSG_INIT_CS(p)					InitializeCriticalSection(p)
		#define PRINTMSG_DELETE_CS(p)				DeleteCriticalSection(p)					
		#define PRINTMSG_TYPE_CS					CRITICAL_SECTION 
		#define VSNPRINTF(a,b,c,d) _vsnprintf(a,b,c,d)

#else
		#include <sys/time.h>
		#include <stddef.h>
		#include <unistd.h>
		#include <stdlib.h>
		#include <sys/wait.h>
		/* thread operate */
		#include <pthread.h>
		#include <semaphore.h>

		#define VSNPRINTF(a,b,c,d) vsnprintf(a,b,c,d)
	
		#define PRINTMSG_LOCK_CS(p)					sem_wait(p)
		#define PRINTMSG_UNLOCK_CS(p)				sem_post(p)
		#define PRINTMSG_INIT_CS(p)					sem_init(p,0,1)
		#define PRINTMSG_DELETE_CS(p)				sem_destroy(p)
		#define PRINTMSG_TYPE_CS					sem_t
#endif

//模块全局参数
int g_ckPrintMsg = 0;
int g_ckPrintErrCount=0;

//防止在WINDOWS平台下打印文字颜色在多线程里混乱
#ifdef Q_OS_WIN32
PRINTMSG_TYPE_CS g_ckPrintfMsgCSEndale;
#endif

void PrintMsgInit()
{	
	#ifdef Q_OS_WIN32
	PRINTMSG_INIT_CS(&g_ckPrintfMsgCSEndale);
	#endif
}

void PrintMsgEnable(int isEnable)
{
	g_ckPrintMsg=isEnable;
}

int getMsgErrCount()
{
	return g_ckPrintErrCount;
}

void SYS_PRINTF(char*format,...)
{
	char buf[512];
	va_list args;
	va_start(args, format);
	
	VSNPRINTF(buf,512,format,args);


	#ifdef Q_OS_WIN32
	PRINTMSG_LOCK_CS(&g_ckPrintfMsgCSEndale);
	#endif

   	printf("System-: %s\r\n",buf);

	#ifdef Q_OS_WIN32
	PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCSEndale);
	#endif

	va_end(args);
}
		
void ERR_PRINTF(char*format,...)
{
		char buf[512];
		va_list args;
		va_start(args, format);
	
		//收集错误消息数量
		g_ckPrintErrCount++;
		//----------------------
		//打印消息
		VSNPRINTF(buf,512,format,args);
		#ifdef Q_OS_WIN32
			PRINTMSG_LOCK_CS(&g_ckPrintfMsgCSEndale);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_INTENSITY);
		#endif
   			printf("Err-: %s\r\n",buf);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED );
			PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCSEndale);
		#endif
		va_end(args);
}

void ALERT_PRINTF_UTF8(char*format,...)
{
	if(g_ckPrintMsg)
	{
		char gbValue[1024]={0};
		char buf[512];
		va_list args;
		char tme_buf[30];
		time_t tt;		
		struct tm *ptm = NULL;

		//时间
		tt=time(NULL);
		ptm = localtime(&tt);
		if(ptm) 
		sprintf(tme_buf, "%02d:%02d:%02d",
				ptm->tm_hour,
				ptm->tm_min,
				ptm->tm_sec);

		//数据处理	
		va_start(args, format);
	
		VSNPRINTF(buf,512,format,args);
		#ifdef Q_OS_WIN32
			PRINTMSG_LOCK_CS(&g_ckPrintfMsgCSEndale);			
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
		#endif
			FROM_UTF8(buf,strlen(buf),gbValue);
   			printf("Alert-[%s]: %s\r\n",tme_buf,gbValue);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED );
			PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCSEndale);
		#endif
		va_end(args);
	}
}

/*****
*
* 打印黑底绿色字
*/
void DEBUG_PRINTF_UTF8(char*format,...)
{
	if(g_ckPrintMsg)
	{
		char gbValue[1024]={0};
		char buf[512];
		va_list args;
		char tme_buf[30];
		time_t tt;		
		struct tm *ptm = NULL;

		//时间
		tt=time(NULL);
		ptm = localtime(&tt);
		if(ptm)
		sprintf(tme_buf, "%02d:%02d:%02d",
				ptm->tm_hour,
				ptm->tm_min,
				ptm->tm_sec);

		va_start(args, format);
	
		VSNPRINTF(buf,512,format,args);
		#ifdef Q_OS_WIN32
			PRINTMSG_LOCK_CS(&g_ckPrintfMsgCSEndale);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		#endif
			
			FROM_UTF8(buf,strlen(buf),gbValue);
   			printf("Debug-[%s]: %s\r\n",tme_buf,gbValue);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED );
			PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCSEndale);
		#endif
		va_end(args);
	}

}

/*****
*
* 打印黑底黄色字
*/
void WARNING_PRINTF_UTF8(char*format,...)
{
	if(g_ckPrintMsg)
	{
		char gbValue[1024]={0};
		char buf[512];
		va_list args;
		char tme_buf[30];
		time_t tt;		
		struct tm *ptm = NULL;

		//时间
		tt=time(NULL);
		ptm = localtime(&tt);
		if(ptm)
		sprintf(tme_buf, "%02d:%02d:%02d",
				ptm->tm_hour,
				ptm->tm_min,
				ptm->tm_sec);

		//数据处理	
		va_start(args, format);
	
		VSNPRINTF(buf,512,format,args);
		#ifdef Q_OS_WIN32
			PRINTMSG_LOCK_CS(&g_ckPrintfMsgCSEndale);			
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		#endif
			FROM_UTF8(buf,strlen(buf),gbValue);
   			printf("Warning-[%s]: %s\r\n",tme_buf,gbValue);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED );
			PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCSEndale);
		#endif
		va_end(args);
	}
}

void DEBUG_PRINT_HEX16(unsigned char* data,int len,char*format,...)
{
	if(g_ckPrintMsg)
	{
		va_list args;
		char buf[512];
		int i;
		char data_buf[1024];
		char buf2[6];
		char tme_buf[30];
		time_t tt;		
		struct tm *ptm = NULL;

		//时间
		tt=time(NULL);
		ptm = localtime(&tt);
		if(ptm) 

		sprintf(tme_buf, "%02d:%02d:%02d",
				ptm->tm_hour,
				ptm->tm_min,
				ptm->tm_sec);

		//数据处理						
		data_buf[0]=0;
		for(i=0;i<len;i++)
		{
		sprintf(buf2,"0x%02X",(unsigned char)data[i]);
		sprintf(data_buf,"%s %s",data_buf,buf2);
		}

		va_start(args, format);

		VSNPRINTF(buf,512,format,args);
		#ifdef Q_OS_WIN32
			PRINTMSG_LOCK_CS(&g_ckPrintfMsgCSEndale);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		#endif
   			printf("Debug-[%s]: %s%s\r\n",tme_buf,buf,data_buf);
		#ifdef Q_OS_WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE |FOREGROUND_GREEN|FOREGROUND_RED );
			PRINTMSG_UNLOCK_CS(&g_ckPrintfMsgCSEndale);
		#endif

		va_end(args);
	}
}

#ifdef __cplusplus
}
#endif
#define __CK_PRINTF_MSG_H_
#endif
