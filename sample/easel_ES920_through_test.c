/******************************************************************************/
// easel_ES920_test.c - easel_ES920_test file
/******************************************************************************/

/*
-- Copyright (c) 2016 masayuki kubou

-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
-- THE SOFTWARE.
--
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <time.h>
#include <sys/time.h>
#include "libeasel_ES920.h"
#include "easel_ES920.h"

#define APP_VERSION "1.0.0"

int main(int argc, char **argv)
{
	int iRet;

	char DevName1[26]="/dev/ttyO3";
	char DevName2[26]="/dev/ttyO5";
	char cMsg[512]="1234567890abcdefghijklmnopqrstuvwxyz";
	unsigned char cRecv[50]={0};
	int i=2;
	int ret = 0;
	int sig = 0;
	int cnt = 1;
	int cntOK = 0, cntNG = 0;
	int Antenna = 0;
	long iCnt = 1;
	long Retrycnt = 20;
	int iWait = 700;
	int qnode=1;	
	char *qbw="125k";
	int qsf=7;
	int qch=1;
	unsigned int qpan=0x01;
	unsigned int qown=0x01;
	unsigned int qdst=0x00;
	int qack=1;
	int qret=3;
	int qbaud=115200;
	int qslep=1;
	int qsleptm=50;
	int qpwr=13;

	BYTE multi64bitAddr[8]={0};

	if( argc >= 3 ){

		strcpy( DevName1, argv[1] ); 
		strcpy( DevName2, argv[2] ); 

		if( argc >= 4 ){

			// 引数解析
			while(i < argc){
				printf("[%d] %s\n", i, argv[i]);

				//help
				if(strncmp(argv[i], "--help", strlen("--help")) == 0 ){
					printf("Version : %s \n", APP_VERSION );
					printf("easel_ES920_through test [device1] [device2] [options]\n");
					printf("options:\n");
					printf(" -b[baudrate]\n");
					printf(" -c[Count]\n");
					printf(" -r[Retry Count]\n");
					printf(" -d[Data]\n");
					printf(" -w[Wait(ms)]\n");
					printf(" -h[hop mode]\n");
					printf(" -q[920MHz Option]\n");
					printf("    -qa[node]\n");
					printf("    -qb[bandwidth]\n");
					printf("    -qc[sf]\n");
					printf("    -qd[channel]\n");
					printf("    -qe[panid]\n");
					printf("    -qf[ownid]\n");
					printf("    -qg[dstid]\n");
					printf("    -ql[ack]\n");
					printf("    -qm[retly]\n");
					printf("    -qr[baudrate]\n");
					printf("    -qs[sleep]\n");
					printf("    -qt[sleeptime]\n");
					printf("    -qu[power]\n");
					printf("    -qu[power]\n");
					printf("Usage:\n");
					printf("Antenna type internal [0] external [1]\n");
					printf("infinite loop [-1]\n");
					return 0;
				}

				// Cnt
				if(strncmp(argv[i], "-c", strlen("-c")) == 0){
					if(sscanf(argv[i], "-c%ld", &iCnt) != 1){
						ret = -1;
					}
				}

				// Retrycnt
				if(strncmp(argv[i], "-r", strlen("-r")) == 0){
					if(sscanf(argv[i], "-r%ld", &Retrycnt) != 1){
						ret = -1;
					}
				}

				// Wait
				if(strncmp(argv[i], "-w", strlen("-w")) == 0){
					if(sscanf(argv[i], "-w%d", &iWait) != 1){
						ret = -1;
					}
				}

				// Data
				if(strncmp(argv[i], "-d", strlen("-d")) == 0){
				 	if(strlen(argv[i]) > 2){
						strcpy(cMsg, &argv[i][2]);
					}
				}
				
				//920MHz Parameter settings 
				if(strncmp(argv[i], "-q", strlen("-q")) == 0){

					//node 
					if(strncmp(argv[i], "-qa", strlen("-qa")) == 0){
				 		if(sscanf(argv[i], "-qa%d", &qnode) != 1){
							ret = -1;
						}
					}
					
					//bandwidth
					if(strncmp(argv[i], "-qb", strlen("-qb")) == 0){
						if(sscanf(argv[i], "-qb%s", qbw) != 1){
							ret = -1;
						}
					}

					//sf
					if(strncmp(argv[i], "-qc", strlen("-qc")) == 0){
						if(sscanf(argv[i], "-qc%d", &qsf) != 1){
							ret = -1;
						}
					}
						
					if(strncmp(argv[i], "-qd", strlen("-qd")) == 0){
						if(sscanf(argv[i], "-qd%d", &qch) != 1){
							ret = -1;
						}

					}

					if(strncmp(argv[i], "-qe", strlen("-qe")) == 0){
						if(sscanf(argv[i], "-qe%d", &qpan) != 1){
							ret = -1;
						}

					}
					
					if(strncmp(argv[i], "-qf", strlen("-qf")) == 0){
						if(sscanf(argv[i], "-qf%d", &qown) != 1){
							ret = -1;
						}
					}
					
					if(strncmp(argv[i], "-qg", strlen("-qg")) == 0){
						if(sscanf(argv[i], "-qg%d", &qdst) != 1){
							ret = -1;
						}
					}
					
					if(strncmp(argv[i], "-ql", strlen("-ql")) == 0){
						if(sscanf(argv[i], "-ql%d", &qack) != 1){
							ret = -1;
						}
					}
					
					if(strncmp(argv[i], "-qm", strlen("-qm")) == 0){
						if(sscanf(argv[i], "-qi%d", &qret) != 1){
							ret = -1;
						}
					}
					
					if(strncmp(argv[i], "-qr", strlen("-qr")) == 0){
						if(sscanf(argv[i], "-qr%d", &qbaud) != 1){
							ret = -1;
						}
					}	

					if(strncmp(argv[i], "-qs", strlen("-qs")) == 0){
						if(sscanf(argv[i], "-qs%d", &qslep) != 1){
							ret = -1;
						}
					}
					
					if(strncmp(argv[i], "-qt", strlen("-qt")) == 0){
						if(sscanf(argv[i], "-qt%d", &qsleptm) != 1){
							ret = -1;
						}
					}

					if(strncmp(argv[i], "-qu", strlen("-qu")) == 0){
						if(sscanf(argv[i], "-qu%d", &qpwr) != 1){
							ret = -1;
						}
					}
				}

			}	

			i++;
		}
		if(ret){
			printf("param error\n");
			return ret;
		}
	}

	
	
	printf("Wait = %dms Cnt = %ld\n", iWait, iCnt);

	//Initialization 
	iRet = easel_ES920_init(DevName1,qbaud);
	if( iRet ){
		printf("DevName1: open error\n");
		return 1;
	}

	
	iRet = rs232c_init(DevName2);
	if( iRet ){
		printf("DevName2: open error\n");
		return 1;
	}
	
	// 空読み
	//(void)Serial_GetString(iPort, cRecv, sizeof(cRecv));


	// 設定モード要求
	iRet = SettingRequest(); 
	if(iRet){
		printf("CAN'T MOVE ON CONFIGURATION MODE\n");
		iRet = easel_ES920_exit();
		return 0;
	}

	// 無線設定要求(default)
	iRet = WirelessSettingRequest();
	if(iRet){
		printf("PARAMETER SETTING ERROR \n");
		iRet = easel_ES920_exit();
		return 0;
	}

/*	// Easel defaut Settings 
	easel_es920_set_wireless(
			EASEL_ES920_NODE_END, 	
			EASEL_ES920_BW_125K, 	
			EASEL_ES920_SF7,	
			1,			//Channel: 1
			1,			//Panid : 0x0001
			0,			//Ownid : 0x0001
			1,			//Dstid : 0x0000
			EASEL_ES920_ACK_ON,  	//1 
			10,			//Rettry: 3
			EASEL_ES920_TRASMOD_PAY,//
			EASEL_ES920_RCVID_ON,	
			EASEL_ES920_RSSI_ON,
			EASEL_ES920_MODE_OPE,	
			EASEL_ES920_BAUD_115200,
			EASEL_ES920_SLEEP_OFF, 
			50,			//Sleep time: 50
			13			//Power: 13dBm
	);
*/

/*

	// 応答待ちタイマ設定要求
//	(void)TimerRequest();
	conexio_cmm920_set_timer(100);

	if( iHop == CONEXIO_CMM920_HOP_MULTI )
	{
		conexio_cmm920_set_hop_mode(iHop);
		conexio_cmm920_set_address(myId, multi64bitAddr, myAddr);
	}

*/

	// 動作モード要求
	iRet = RunRequest();
	if(iRet){
		printf("CAN'T MOVE ON OPERATION MODE\n");
		iRet = easel_ES920_exit();
		return 0;
	}

	printf("----- OPERATION MODE ------\n");


	// 初回の受信側の準備待ち
//	usleep(iWait * 2 * 1000);
	int count = 0;
	while(1){
		
		//printf("1\n");
		//DATA FROM EASEL to 
		memset(cRecv,'\0',sizeof(cRecv));
		iRet = RecvTelegram(cRecv);
		if(iRet>0){
			usleep(100);
			SendRS232C(cRecv);
		}
		
		//printf("2\n");
		//usleep(100);
		memset(cRecv,'\0',sizeof(cRecv));


		//DATA FROM RS232C TO EASLE
		iRet = RecvRS232C(cRecv);
		if(iRet>0){
			usleep(100);
			iRet = SendTelegram(cRecv);
		}

		//printf("3\n");
		count++;	
		memset(cRecv,'\0',sizeof(cRecv));
		//usleep(100);
	}

	easel_ES920_exit();
	rs232c_exit();
	printf("Serial Port Closed...\n");

	return 0;
}
