/***

	libeasel_ES920_func.c - easel_ES920_functions library
	
	Copyright (C) 2016 masayuki kubou, Syunsuke Okamoto.<okamoto@contec.jp>
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

#if 0
#define DbgAllocFreeCheck(fmt...)	printf(fmt)
#else
#define DbgAllocFreeCheck(fmt...)	do { } while (0)
#endif

#if 0
#define DbgDataLength(fmt...)	printf(fmt)
#else
#define DbgDataLength(fmt...)	do { } while (0)
#endif

#if 1
#define DbgPrintRecvTelegram(fmt...)	printf(fmt)
#else
#define DbgPrintRecvTelegram(fmt...)	do { } while (0)
#endif

/// @}

static EASEL920PARAM param;

struct timeval myTime;
struct tm *time_st;


/**
	@~English
	@brief Change Hex to Bcd Dec
	@param hex : value
	@return Success: Bcd Dec-
	@~Japanese
	@brief 16進数から 10進のBCDコードに変換する関数
	@param hex : 16進数
	@return 成功:  10進数BCDコード
**/
/*
BYTE _calc_Hex2Bcd( BYTE hex ){

	return ( ( hex / 16 ) * 10 + ( hex % 16 ) );

}
*/

/**
	@~English
	@brief Change Bcd Decimal to Hex
	@param bcd : bcd Decimal
	@return Success: Hex
	@~Japanese
	@brief 10進のBCDコードから16進数に変換する関数
	@param bcd :　10進数BCDコード
	@return 成功:  16進数
**/
/*
BYTE _calc_Bcd2Hex( BYTE bcd ){

	return ( ( bcd / 10 ) * 16 + ( bcd % 10 ) );

}
*/

/**
	@~English
	@brief Change dBm to Hex
	@param dBm : decibel per milisecond
	@return Success: Hex
	@~Japanese
	@brief dBmから16進数に変換する関数
	@param dBm :　1ミリ秒中のデシベル
	@return 成功:  16進数
**/
/*
BYTE _conexio_cmm920_dBm2Hex( int dBm ){
	dBm = dBm * (-1);
	if( dBm >= 100 ){
		dBm -= 100;
	}
	return _calc_Bcd2Hex( (BYTE)dBm );
}
*/

/**
	@~English
	@brief Change Hex　to dBm
	@param hex : value
	@return Success: dbm
	@~Japanese
	@brief 16進数からdBmに変換する関数
	@param hex :　16進数
	@return 成功:  dBm
**/
/*
int _conexio_cmm920_Hex2dBm( BYTE hex ){
	int dBm;
	dBm = (int) _calc_Hex2Bcd( hex );
	
	if( dBm <= 10 ){
		dBm += 100;
	}
	dBm = dBm * (-1);
	return dBm;
}
*/

/**
	@~English
	@brief EASEL 920 Module send and Ack receive
	@param Data : Send Data
	@param size : Send Size
	@param mode : CMM920  mode
	@param command : CMM920 Command
	@return Success : 0 , Failed : From -16 to -31 Send Error, less than -32 : Receive Error
	@~Japanese
	@brief EASEL 920MHzモジュールに送信とAck受信を行う関数
	@param Data : 送信データ
	@param size : 送信サイズ
	@param mode : CMM920 モード
	@param command : CMM920 コマンド
	@return 成功:  0 失敗 :  送信 エラー: -16～-31,　受信エラー : -32～
**/
/*
int _conexio_cmm920_send_recv( BYTE Data[], int size ,int mode, int command ){

	int iRet, send_size = size;
	int offset = 0; // 2016.01.15 (1) add

	// 2016.01.15 (1) start
	if( mode == CONEXIO_CMM920_MODE_COMMON && 
		command == CONEXIO_CMM920_SET_LSI 
	){
		offset = 3;
	}
	// 2016.01.15 (1) end

	if( Data[offset] == CONEXIO_CMM920_SET_READING_READ ){
		send_size = 1;
	}

	iRet = SendCommand(	Data, send_size,	mode, command );

	if( iRet < 0 )	return (-16 * 1) + iRet;

	usleep(iWait);

	//iRet = RecvCommandAck(Data, &send_size,	mode, command );
	iRet = RecvCommandAck(Data, &size,	mode, command ); // 2016.01.15 (4)
	if( iRet < 0 )	return (-16 * 2) + iRet;
	
	return iRet;
}
*/

void easel_ES920_GetErrorString(int iRet,char *msg)
{

	switch(iRet){

		case 1:	sprintf(msg,"PARAMETER SETTING NG: %s","UNDEFINED COMMAND");
			break;
		
		case 2: sprintf(msg,"PARAMETER SETTING NG: %s","UNEXPECTED OPTION VALUE");
			break;

		case 3: sprintf(msg,"PARAMETER SETTING NG: %s","FlashROM: DELETE ERROR"); 
			break;

		case 4:	sprintf(msg,"PARAMETER SETTING NG: %s","FlashROM: SAVE ERROR");
			break;

		case 5: sprintf(msg,"PARAMETER SETTING NG: %s","FlashROM: LOAD ERROR");
			break;

		case 6: sprintf(msg,"PARAMETER SETTING NG: %s","FlashROM: LOAD ERROR");
			break;

		case 100: sprintf(msg,"PARAMETER SETTING NG: %s", "UNEXPECTE SIZE OF DATA");
			  break;

		case 101: sprintf(msg,"PARAMETER SETTING NG: %s","SENDING ERROR");
			  break;

		case 102: sprintf(msg,"PARAMETER SETTING NG: %s","SENDING ERROR");
			  break;

		case 103: sprintf(msg,"PARAMETER SETTING NG: %s","UNRECEIVING ERROR");
			  break;

		default: sprintf(msg,"PARAMETER SETTING NG: %s","UNKNOWN ERROR");
			 break;
	}

}	
 
 
int _easel_es920_send_recv(char buf[], int command )
{
	int iRet;
	char msg[128]={0};
	 
	iRet = SendCommand(buf,command);
	if( iRet < 0 )	return iRet;

	usleep(param.SerialWait);

	iRet = RecvCommandAck(); // 2016.01.15 (4)
	if( iRet < 0 )	return iRet;

	else if(iRet > 0 ){
		easel_ES920_GetErrorString(iRet,msg);
		DbgPrint("%s\n", msg);
	}


	return iRet;
}

int easel_ES920_set_mode_config( int command ){

	int iRet  = 0;

	iRet = _easel_es920_send_recv("", command);
	
	if( iRet == 0 ){

	}

	return iRet;

}


int easel_ES920_set_node( int command ){

	int iRet  = 0;

	iRet = _easel_es920_send_recv("node", command);
	
	if( iRet == 0 ){

	}

	return iRet;

}

int easel_ES920_set_bw(int command)
{
	return _easel_es920_send_recv("bw", command);
}

int easel_ES920_set_sf(int command)
{
	return _easel_es920_send_recv("sf", command);
}

int easel_ES920_set_ch(int command)
{
	return _easel_es920_send_recv("channel", command);
}

int easel_ES920_set_panid(int command)
{
	return _easel_es920_send_recv("panid", command);
}

int easel_ES920_set_ownid(int command)
{
	return _easel_es920_send_recv("ownid", command);
}

int easel_ES920_set_dstid(int command)
{
	return _easel_es920_send_recv("dstid", command);
}

int easel_ES920_set_acksw(int command)
{
	return _easel_es920_send_recv("ack", command);
}

int easel_ES920_set_ackRetryNum(int command)
{
	return _easel_es920_send_recv("retry", command);
}

int easel_ES920_set_trmode(int command)
{
	return _easel_es920_send_recv("transmode", command);
}

int easel_ES920_set_rcvsw(int command)
{
	return _easel_es920_send_recv("rcvid", command);
}

int easel_ES920_set_rssisw(int command)
{
	return _easel_es920_send_recv("rssi", command);
}

int easel_ES920_set_opmode(int command)
{
	return _easel_es920_send_recv("operation", command);
}

int easel_ES920_set_brate(int command)
{
	return _easel_es920_send_recv("baudrate", command);
}

int easel_ES920_set_slepsw(int command)
{
	return _easel_es920_send_recv("sleep", command);
}

int easel_ES920_set_slept(int command)
{
	return _easel_es920_send_recv("sleeptime", command);
}

int easel_ES920_set_outpw(int command)
{
	return _easel_es920_send_recv("power", command);
}


/**
	@~English
	@brief EASEL 920MHz Initialize
	@param PortName : Serial port name
	@return Success : 0 , Failed : otherwise 0
	@~Japanese
	@brief EASEL 920MHzモジュールの初期化
	@param PortName : シリアル通信ポート名
	@return 成功:  0 失敗 :  0以外
**/

int easel_ES920_init(char* PortName){

	int iBaudrate = 115200;
	int iLength = 8;
	int iStop = 1;
	int iParity = 0;
	int iFlow=0;
	int iPort;

	iPort = Serial_PortOpen_Full(
		PortName,
		iBaudrate,
		iLength,
		iStop,
		iParity,
		10,
		iFlow,
		0
	);

	if(iPort < 0){
		return 1;
	}

	param.SerialPort = iPort;
	param.SerialWait = 50000;

	return 0;
}




/*
int bandwidth_channel_GetErrorString(int ch,int bw)
{
	swich(bw){
		case 1:
		case 2:
		case 3: 
		case 4: if( ch < 1 && 37 < ch) printf("Note channel became 1ch");
			
		case 5: if( ch < 1 && 18 < ch) printf("Note channel became 1ch");

		case 6: if( ch < 1 && 12 < ch) printf("Note channel became 1ch");
	}

}

int channel_bandwidth_GetErrorString(int bw, int ch)
{
	swich(bw){
		case 1:
		case 2:
		case 3: 
		case 4: if( ch < 1 && 37 < ch){
				printf("Invalid Value of ch");
				retrun -1;
			}
		case 5: if( ch < 1 && 18 < ch){
				printf("Invalid Value of ch");
				retrun -1;
			}
		case 6: if( ch < 1 && 12 < ch){
				printf("Invalid Value of ch");
				retrun -1;
			}
	}
	
	return 0;
}
*/


// mode
/**
	@~English
	@brief CONEXIO 920MHz Module Mode function
	@param isWrite : CONEXIO_CMM920_SET_READING_READ or CONEXIO_CMM920_SET_READING_WRITE
	@param code : mode
	@return Success : 0 , Failed : From -1 to -15 : Parameter Error, From -16 to -31 Send Error, less than -32 : Receive Error
	@~Japanese
	@brief CONEXIO 920MHz　Module の モード関数
	@param isWrite : CONEXIO_CMM920_SET_READING_READ or CONEXIO_CMM920_SET_READING_WRITE
	@param code : mode
	@return 成功:  0 失敗 :  送信 エラー:  -1～-15 -16～-31,　受信エラー : -32～
**/


// Address
/**
	@~English
	@brief CONEXIO 920MHz Module Address function
	@param isWrite : CONEXIO_CMM920_SET_READING_READ or CONEXIO_CMM920_SET_READING_WRITE
	@param panId : PAN ID
	@param Addr : Long Address
	@param shortAddr : Short address
	@return Success : 0 , Failed : From -1 to -15 : Parameter Error, From -16 to -31 Send Error, less than -32 : Receive Error
	@~Japanese
	@brief CONEXIO 920MHz　Module の アドレス関数
	@param isWrite : CONEXIO_CMM920_SET_READING_READ or CONEXIO_CMM920_SET_READING_WRITE
	@param panId : PAN ID
	@param Addr : ロングアドレス
	@param shortAddr : ショートアドレス
	@return 成功:  0 失敗 :  送信 エラー:  -1～-15 -16～-31,　受信エラー : -32～
**/


// Version Read
/**
	@~English
	@brief CONEXIO 920MHz Module Version Read function
	@param ver : module version
	@param rev : module revision
	@return Success : 0 , Failed : From -1 to -15 : Parameter Error, From -16 to -31 Send Error, less than -32 : Receive Error
	@~Japanese
	@brief CONEXIO 920MHz　Module の バージョン読み込み 関数
	@param ver : モジュールのバージョン
	@param rev : モジュールのリビジョン
	@return 成功:  0 失敗 :  送信 エラー:  -1～-15 -16～-31,　受信エラー : -32～
**/
/*
int conexio_cmm920_version(int *ver, int *rev)
*/


/**
	@~English
	@brief EASEL 920MHz Module data send function
	@param buf : send data buffer
	@param size : send data size
	@param send_mode : send mode
	@param r_buf : receive buffer (option)
	@return Success : 0 , Failed : From -1 to -15 : Parameter Error, From -16 to -31 Send Error, less than -32 : Receive Error
	@~Japanese
	@brief EASEL 920MHz　Module のデータ送信 関数
	@param buf : 送信データバッファ
	@param size : 送信データサイズ
	@param send_mode : 送信モード
	@param r_buf : 受信バッファ (オプション)
	@return 成功:  0 失敗 :  送信 エラー:  -1～-15 -16～-31,　受信エラー : -32～
**/
int SendTelegram(unsigned char *buf)
{
	int iRet=0;
	char Data[50]={0};
	int length;
	int count;

	//make the formart
	for( count = 0;count < 50; count ++ ){
		Data[count] = buf[count];	
	}

	length = strlen(Data);

	//send the format
	//tcflush( param.SerialPort, TCIFLUSH );
	length = Serial_PutString(param.SerialPort, Data, length * sizeof(char));
	if(iRet<0) return -1;

	printf("===========SendTele=======================\n");
	printf("Data\n");
	for( count = 0; count < length; count++ ) printf("%02x ",Data[count]);
	printf("\n");
	
	gettimeofday(&myTime,NULL);
	time_st = localtime(&myTime.tv_sec);
	printf("TIME:%02d:%02d:%02d.%06d\n",
				time_st->tm_hour,
				time_st->tm_min,
				time_st->tm_sec,
				myTime.tv_usec
	      );


	DbgPrint("<SendTelegram> Port %x, ret %d, Data %s\n",param.SerialPort,iRet,Data);

	return iRet;
}

/**
	@~English
	@brief Change Hex to Bcd Dec
	@param hex : value
	@return Success: Bcd Dec-
	@~Japanese
	@brief 16進数から 10進のBCDコードに変換する関数
	@param hex : 16進数
	@return 成功:  10進数BCDコード
**/
BYTE _calc_Hex2Bcd( BYTE hex ){

	return ( ( hex / 16 ) * 10 + ( hex % 16 ) );

}


/**
	@~English
	@brief EASEL 920MHz Module data receive function
	@param buf : receive data buffer
	@param size : receive data size
	@param hop : hop mode
	@param r_channel : receive channel
	@param rx_pwr : receive power
	@param crc: crc value
	@param dest_id : dest PAN ID
	@param src_id : source PAN ID
	@param dest_addr : Dest Address
	@param src_addr : Source Addres
	@return Success : 0 , Failed : From -1 to -15 : Parameter Error, From -16 to -31 Send Error, less than -32 : Receive Error
	@~Japanese
	@brief EASEL 920MHz　Module のデータ受信 関数
	@param buf : 受信データバッファ
	@param size : 受信データサイズ
	@param hop : ホップ・モード
	@param r_channel : 受信チャネル
	@param rx_pwr : 受信強度
	@param crc: CRC値
	@param dest_id : 送信先PAN　ID
	@param src_id : 送信元 PAN ID
	@param dest_addr : 送信先アドレス
	@param src_addr : 送信元アドレス
	@return 成功:  0 失敗 :  送信 エラー:  -1～-15 -16～-31,　受信エラー : -32～
**/
int RecvTelegram(unsigned char *buf)
//int RecvTelegram(unsinged char *buf, unsigned int rx_pwr, int src_id, int src_addr )
{
	int iRet=0;
	char Data[62]={0};
	int readlen = 0;
	int count;
	int bef_readlen = 0;

	//get the data length(header + response data) 
	memset(&Data[0],0x00, 62);

	ioctl( param.SerialPort, FIONREAD, &readlen );
	if(readlen == 0) return 0;

	while(1){
		bef_readlen=readlen;
		usleep(100*1000);
		ioctl(param.SerialPort,FIONREAD,&readlen);
		if(bef_readlen==readlen) break;
	}	
	//if(readlen==0) return 0;

	for(count=0;count < readlen;count++){
		Data[count] = Serial_GetChar(param.SerialPort);
	}

	DbgPrintRecvTelegram("===========RecvTele=======================\n");
	DbgPrintRecvTelegram("Data\n");
	for(count=0;count < readlen;count++)
		DbgPrintRecvTelegram("%02x ",Data[count]);
	
	DbgPrintRecvTelegram("\n");

	gettimeofday(&myTime,NULL);
	time_st = localtime(&myTime.tv_sec);
	printf("TIME:%02d:%02d:%02d.%06d\n",
				time_st->tm_hour,
				time_st->tm_min,
				time_st->tm_sec,
				myTime.tv_usec
	      );


	//Serial_GetChar(param.SerialPort);

	if(!strcmp(Data,"OK\r\n")) return 0;

	DbgPrintRecvTelegram("DATA:");

	//for(i=0;i<=readlen ;i++) ;
	
/*		
	if( readlen >= 2 ){
		if( Data[readlen - 2] == '\r' &&
			Data[readlen -1] == '\n' ){
				memset(&buf[readlen-2],'\0', 2);
		}
	}
*/

	for( count = 0; count < readlen; count ++ ){

		if( count < 4 ){
			//rx_pwr
		}else if( count < 8 ){
			// id
		}else if( count < 12){ 
			// addr
		}else{
			buf[count-12] = Data[count];
		}
	}
	//memcpy(buf, buf, (readlen-13) * sizeof(unsigned char) );


	//printf("\n");

	//buf = strtok(Data,"\r");
	
	DbgPrintRecvTelegram("<RecvTelegram>Data%s buf %s\n",Data,buf);
	
	//make the area of data length   
	//length = (int)_calc_Hex2Bcd(head[0]);
	
	//DbgPrint("length:%d,panid:%x%x,ownid:%x%x,dstid:%x%x\n",
	//		length,head[5],head[6],head[7],head[8],head[9],head[10]);

	return readlen;
}

// Send Command ES920
/**
	@~English
	@brief EASEL 920MHz Module command send function
	@param buf : send data buffer
	@param command : command
	@return Success : 0 , Failed : Packet memory allocation error : -1, Data memory allocation error : -2
	@~Japanese
	@brief EASEL 920MHz　Module のコマンドデータ受信 関数
	@param buf : 送信データバッファ
	@param command : コマンド
	@return 成功:  0 失敗 :  パケットメモリ確保エラー:  -1, データメモリ確保エラー : -2
**/
int SendCommand(char buf[], int command )
{
	int iRet=0;
	char Data[15] = {0};
	int length;


	//make the format	
	//Data = (char *)malloc( sizeof(char) * 15 );
	//if( Data == (char*) NULL ) return -1;

	if( strlen( buf ) == 0 ){
		DbgPrint("--- Processoer Mode ----\n");
		sprintf(Data,"%d\r\n",command);
	}
	else{
		switch( buf[0] ){
			case 'x':
			case 'w':
			case 'z':
				sprintf((char *)Data,"%s\r\n",buf);
				DbgPrint("start or save or load\n");
				break;
			default:
				sprintf((char *)Data,"%s %d\r\n",buf,command);
				DbgPrint("setting parameter\n");
				break;
		}
	}

	length = strlen(Data);

	//send the format
	tcflush( param.SerialPort, TCIFLUSH );
	if(length != Serial_PutString(param.SerialPort, Data, length * sizeof(char)))
		iRet=-1;

	DbgPrint("<SendCommand> Port %x, buf %s, command %d, ret %d\n",param.SerialPort, buf, command,iRet);

	//free(Data);
	return iRet;
}


/**
	@~English
	@brief EASEL 920MHz Module Address function
	@return Success : 0 , Failed : Error
	@~Japanese
	@brief CONEXIO 920MHz　Module の アドレス関数
	@return 成功:  0 , 失敗：それ以外
**/

int RecvCommandAck()
{

	char res[8]={0};
	int iRet = 0;
	char *code;	

	//size get
	Serial_GetString(param.SerialPort, res, sizeof(res));

	//divide response  
	code = strtok(res," \r");
	if(strcmp(code,"OK")){
		// get error code 
		code = strtok(NULL," \r");
		sscanf(code,"%d",&iRet);
	}

	DbgPrint("<RecvCommandAck> Port %x, res %s, ret %d\n",param.SerialPort,res,iRet);
	return iRet;
}

/*

int get_param(char *param){

 	PEASEL920PARAM param;

	if(!strcmp(param,"panid")){

		return param->panid;

	}

	if(!strcmp(param,"dstid")){

		return param->dstid;
	}
}	
*/

/**
	@~English
	@brief EASEL 920MHz Exit
	@return Success : 0 , Failed : otherwise 0
	@~Japanese
	@brief EASEL 920MHzモジュールの終了
	@return 成功:  0 失敗 :  0以外
**/
int easel_ES920_exit(void)
{
	if(param.SerialPort == 0) return 0;

	Serial_PortClose(param.SerialPort);

	return 0;
}



