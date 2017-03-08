/***

	libeasel_ES920_func.c - EASEL_ES920_functions library

	Copyright (C) 2016  masayuki kubou, Syunsuke Okamoto.<okamoto@contec.jp>

***/

#ifndef __EASEL_ES920__
#define __EASEL_ES920__

// BOOL define
typedef unsigned int BOOL;
#define FALSE 0
#define TRUE 1

// BYTE define n
typedef unsigned char BYTE,*LPBYTE;
typedef unsigned short WORD,*LPWORD;
typedef unsigned long DWORD,*LPDWORD;  

// EASEL
#define EASEL_ES920_MODE_CONFIG 2	//Mode:Processor 

#define EASEL_ES920_NODE_COORD 1	//node:Coordinator 
#define EASEL_ES920_NODE_END 2		//node:EndDevice

#define EASEL_ES920_BW_31K 1		//BandWidth: 31.25kHz  
#define EASEL_ES920_BW_42K 2		//BandWidth: 41.7kHz  
#define EASEL_ES920_BW_63K 3		//BandWidth: 62.5kHz  
#define EASEL_ES920_BW_125K 4		//BandWidth: 125kHz  
#define EASEL_ES920_BW_250K 5		//BandWidth: 250kHz  
#define EASEL_ES920_BW_500K 6		//BandWidth: 500kHz  

#define EASEL_ES920_SF7 7		//Sprerading Factor: 7  
#define EASEL_ES920_SF8 8		//Sprerading Factor: 8  
#define EASEL_ES920_SF9 9		//Sprerading Factor: 9  
#define EASEL_ES920_SF10 10		//Sprerading Factor: 10  
#define EASEL_ES920_SF11 11		//Sprerading Factor: 11  
#define EASEL_ES920_SF12 12   		//Sprerading Factor: 12  

#define EASEL_ES920_ACK_ON  1		//ACK : ON  
#define EASEL_ES920_ACK_OFF  2		//ACK : OFF  

#define EASEL_ES920_TRASMOD_PAY 1	//Transmode : Payload       
#define EASEL_ES920_TRASMOD_FRA 2	//Transmode : Frame       

#define EASEL_ES920_RCVID_ON 1		//Node ID information:	ON       
#define EASEL_ES920_RCVID_OFF 2		//Node ID information:	OFF   

#define EASEL_ES920_RSSI_ON 1 		//RSSI information:	ON   
#define EASEL_ES920_RSSI_OFF 2 		//RSSI information:	OFF   

#define EASEL_ES920_MODE_CONF 1 	//Configuration Mode   
#define EASEL_ES920_MODE_OPE 2	 	//Operation Mode 

#define EASEL_ES920_BAUD_9600	 1	//Baudrate:	9600
#define EASEL_ES920_BAUD_19200   2	//Baudrate:	19200
#define EASEL_ES920_BAUD_38400   3	//Baudrate:	38400
#define EASEL_ES920_BAUD_57600   4	//Baudrate:	57600
#define EASEL_ES920_BAUD_115200  5	//Baudrate:	115200
#define EASEL_ES920_BAUD_230400  6	//Baudrate:	230400

#define EASEL_ES920_SLEEP_OFF  1 	//Sleep Mode:	OFF 
#define EASEL_ES920_SLEEP_TIME 2 	//Sleep Mode:	Timer Wakeup 
#define EASEL_ES920_SLEEP_INT  3	//Sleep Mode:	INT Wakeup 

/*
typedef struct __conexioCMM920_packet{
	BYTE dle;		///< DLE
	BYTE stx;		///< STX
	BYTE size[2];	///< packet size
	BYTE command[2];	///< command
	BYTE result;	///< result
	BYTE resultCode;	///< result code
	BYTE *data;	///< data
	BYTE sum;	///< check sum
	BYTE etx;	///< ETX
} CONEXIO920PACKET, *PCONEXIO920PACKET;
*/

typedef struct __easelES920_param{
	int node;	///< 
	int bw;	///< band witdh
	int sf;	///< 
	int ch;	///< channel
	int panid;	///< PAN ID
	int ownid;	///< Own ID
	int dstid;	///< Distination ID
	int ack;	///< Acknowlge 
	int retry;	///< Retry
	int transmod;	///< Transfer Mode
	int rcvid;		///< receive ID
	int rssi;		///< RSSI
	int baudrate;	///< BaudRate
	int sleep;		///< sleep
	long int sleeptime;	///< sleep time
	int power;
	int SerialPort;	///< Serial Port
	int SerialWait;	///< Serial Port Wait
} EASEL920PARAM, *PEASEL920PARAM;

//Function
extern int _easel_es920_send_recv(char buf[],int command);
extern int easel_ES920_init(char* PortName);
extern int SendCommand(char buf[], int command );
extern int RecvCommandAck(void);
extern int easel_ES920_SetProcessorMode(char buf[], int command);
extern void easel_ES920_GetErrorString(int iRet,char *msg);
extern int easel_ES920_exit(void);

//RS232C
extern int rs232c_init(char* PortName);
extern int rs232c_exit(void);

//ES920 command list

extern int easel_ES920_set_mode_config(int command);
extern int easel_ES920_save_param();
extern int easel_ES920_save_param();
extern int easel_ES920_set_node(int node);
extern int easel_ES920_set_bw(int bw);
extern int easel_ES920_set_sf(int sf);
extern int easel_ES920_set_ch(int ch);
extern int easel_ES920_set_panid(int panid);
extern int easel_ES920_set_ownid(int ownid);
extern int easel_ES920_set_dstid(int dstid);
extern int easel_ES920_set_acksw(int acksw);
extern int easel_ES920_set_ackRetryNum(int ackRetryNum);
extern int easel_ES920_set_trmode(int trmode);
extern int easel_ES920_set_rcvsw(int rcvsw);
extern int easel_ES920_set_rssisw(int rssisw);
extern int easel_ES920_set_opmode(int opmode);
extern int easel_ES920_set_brate(int brate);
extern int easel_ES920_set_slepsw(int slepsw);
extern int easel_ES920_set_slept(int slepNum);
extern int easel_ES920_set_outpw(int outpw);

#define easel_ES920_set_processor_mode()	easel_ES920_set_mode_config( EASEL_ES920_MODE_CONFIG )
#define easel_ES920_set_mode_operation()	_easel_es920_send_recv("z", 0);  // any value is ok 
#define easel_ES920_save_param()	_easel_es920_send_recv("w",0)
#define easel_ES920_load_param()	_easel_es920_send_recv("x",0)


BYTE _calc_Hex2Bcd( BYTE hex );

int SendTelegram(unsigned char *buf);
int RecvTelegram(unsigned char *buf);
int RecvRS232C(unsigned char *buf);
int SendRS232C(unsigned char *buf);

// wrapper function
extern int easel_ES920_set_wireless_default();
extern int easel_ES920_set_wireless( int node, int bw, int sf, int ch, int panid, int ownid, int dstid, int acksw, int ackRetryNum, int trmode, int rcvsw, int rssisw, int opmode, int brate, int slepsw, int slepNum, int outpw);

#endif
