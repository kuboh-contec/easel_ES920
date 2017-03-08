/***

	libeasel_ES920_test_rs232c.c - easel_ES920_functions library
	
	Copyright (C) 2016 masayuki kubou
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, see
   <http://www.gnu.org/licenses/>.  

***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>
#include "libeasel_ES920.h"
#include "serialfunc.h"

/*!
 @~English
 @name DebugPrint macro
 @~Japanese
 @name デバッグ用表示マクロ
*/
/// @{

#if 1
#define DbgPrint(fmt...)	printf(fmt)
#else
#define DbgPrint(fmt...)	do { } while (0)
#endif

/// @}

static int iPort2;
struct timeval myTime;
struct tm *time_st;


int rs232c_init(char* PortName){

	int iBaudrate = 19200;
	int iLength = 8;
	int iStop = 1;
	int iParity = 2;
	int iFlow=0;

	iPort2 = Serial_PortOpen_Full(
		PortName,
		iBaudrate,
		iLength,
		iStop,
		iParity,
		10,
		iFlow,
		0
	);

	if(iPort2 < 0){
		return 1;
	}

	return 0;
}

int rs232c_exit(void)
{
	if(iPort2 == 0) return 0;

	Serial_PortClose(iPort2);

	return 0;
}

int RecvRS232C(unsigned char *buf)
{
	int iRet = 0;
	int length;
	int bef_readlen=0;
	//unsigned char Data[50]={0};
	int readlen = 0;
	int count;

	//get the data length(headder + response data) 
	ioctl( iPort2, FIONREAD, &readlen );
	if(readlen==0) return 0;
	
	while(1){
		bef_readlen=readlen;
		usleep(100*1000);
		ioctl(iPort2,FIONREAD,&readlen);
		if(bef_readlen==readlen) break;
	}

	for(count=0;count<readlen;count++){
		buf[count] = Serial_GetChar(iPort2);
	}
	
	printf("=====RecvRS232C==================\n");
	int i;
	for(i=0;i<readlen;i++) printf("%02x ",buf[i]);
	printf("\n");

	/*
	gettimeofday(&myTime,NULL);
	time_st = localtime(&myTime.tv_sec);
	printf("TIME:%02d:%02d:%02d.%06d\n",
				time_st->tm_hour,
				time_st->tm_min,
				time_st->tm_sec,
				myTime.tv_usec
	      );
	*/ 

	//memcpy(buf, buf, (strlen(buf)-2) * sizeof(unsigned char));
	DbgPrint("<RecvRS232C> Port %d buf %s res %d\n",iPort2,buf,iRet);
/*
	length = stRLen(buf);

	iRet = Serial_GetString(iPort2,buf,sizeof(buf));
	DbgPrint("<RecvRS232C> Port %d buf %s res %d\n",iPort2,buf,iRet);
	if(iRet<=0) return -1;
*/


	return readlen;
}

int SendRS232C(unsigned char *buf)
{
	int iRet = 0;
	int length;	
	//unsigned char Data[50]={0};
	
	length = strlen(buf);

	iRet = Serial_PutString(iPort2,buf, length * sizeof(unsigned char));
	if(iRet<0) return -1;

	DbgPrint("==========SendRS232C========================\n");
	int i;
	for(i=0;i<strlen(buf);i++) printf("%02x ",buf[i]);
	printf("\n");
		
	/*
	gettimeofday(&myTime,NULL);
	time_st = localtime(&myTime.tv_sec);
	printf("TIME:%02d:%02d:%02d.%06d\n",
				time_st->tm_hour,
				time_st->tm_min,
				time_st->tm_sec,
				myTime.tv_usec
	      );
	*/

	DbgPrint("<SendRS232C> Port %d  buf %s res %d\n",iPort2,buf,iRet);

	return 0;
}

