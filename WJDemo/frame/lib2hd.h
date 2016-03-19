#ifndef _LIB2HD_H
#define	_LIB2HD_H

#include <stdio.h>
#include <stdlib.h>
#include "CToOcType.h"

//***************************** ˝æ›¿‡–Õ∂®“Â****************************//
//typedef unsigned char uint8;		//Œﬁ∑˚∫≈8Œª’˚–Õ±‰¡ø
//typedef signed char int8;		//”–∑˚∫≈8Œª’˚–Õ±‰¡ø
//typedef unsigned short uint16;		//Œﬁ∑˚∫≈16Œª’˚–Õ±‰¡ø
//typedef signed short int16;		//”–∑˚∫≈16Œª’˚–Õ±‰¡ø
//typedef unsigned int uint32;		//Œﬁ∑˚∫≈32Œª’˚–Õ±‰¡ø
//typedef signed int int32;		//”–∑˚∫≈32Œª’˚–Õ±‰¡ø
typedef float fp32;		//µ•æ´∂»∏°µ„ ˝£®32Œª≥§∂»£©
typedef double fp64;		//À´æ´∂»∏°µ„ ˝£®64Œª≥§∂»£©

#define PDEBUG

#ifdef PDEBUG
#define DEBUGP printf
#else
#define DEBUGP /\
/DEBUGP
#endif

#define SUCCESS							0	

#define BUF_LEN							512
#define	GET_OBU_INFO					1
#define	GET_IC_INFO						2

#define COM 							0	//¥Æø⁄
#define NET 							1	//Õ¯ø⁄
#define USBCOM 							2	//USB◊™¥Æø⁄
#define UDPNET 							3	//USB◊™¥Æø⁄
#define	RECV							0
#define	SEND							1
#define GPI0_NODE			    		"/dev/ctrl_io"	//I/OŒƒº˛√Ë ˆ∑˚

#define	PROCESS_0						0
#define	PROCESS_10						10
#define	PROCESS_20						20
#define	PROCESS_30						30
#define	PROCESS_40						40
#define	PROCESS_50						50
#define	PROCESS_60						60
#define	PROCESS_70						70
#define	PROCESS_80						80
#define	PROCESS_90						90
#define	PROCESS_100						100
//«¯”Ú∂®“Â
#define LIAO_NING_REGION				0x00
#define SHAN_DONG_REGION				0x01
#define XI_AN_REGION					0x02
#define TIAN_JIN_REGION					0x03
#define BEI_JING_REGION					0x04
#define HU_NAN_REGION					0x05
#define NEI_MENG_REGION					0x06
#define SI_CHUANG_REGION				0x07
#define HU_BEI_REGION					0x08
#define YUN_NAN_REGION					0x09
#define NING_XIA_REGION					0x0a
#define GUI_ZHOU_REGION					0x0b

#define MAXUDPDATALENGHT 1024
#define MAXCOMDATALENGHT 1024

#define Getbit(byt,num)					(((byt) >> (num))&1)
#define Setbit(str,num,val)				do{if((val) == 1) ((str) |= (1 << (num))); else ((str) &= ~(1 << (num))) ;}while(0)

extern uint8 g_u8LLCFlag;
 
extern int g_timeout_cnt;
extern int g_timeout_max;
inline int checktimeout();

extern char So_Version[30];

extern uint32 g_com_needrx_len;
extern uint32 g_com_rx_len;				//¥Æø⁄Ω” ’ ˝æ›≥§∂»
extern uint32 g_com_tx_len;				//¥Æø⁄∑¢ÀÕ ˝æ›≥§∂»
extern uint8 g_com_rx_buf[MAXCOMDATALENGHT];			//¥Æø⁄Ω” ’ ˝æ›
extern uint8 g_com_tx_buf[MAXCOMDATALENGHT];			//¥Æø⁄∑¢ÀÕ ˝æ›
extern uint32 g_com_tx_len_bst;			//¥Æø⁄∑¢ÀÕ ˝æ›≥§∂»
extern uint8 g_com_tx_buf_bst[MAXCOMDATALENGHT];		//¥Æø⁄∑¢ÀÕ ˝æ›

//UdpÕ¯ø⁄
extern uint32 g_udpnet_rx_len;			//UdpÕ¯ø⁄Ω” ’ ˝æ›≥§∂»
extern uint32 g_udpnet_tx_len;			//UdpÕ¯ø⁄∑¢ÀÕ ˝æ›≥§∂»
extern uint8 g_udpnet_rx_buf[MAXUDPDATALENGHT];		//UdpÕ¯ø⁄Ω” ’ ˝æ›
extern uint8 g_udpnet_tx_buf[MAXUDPDATALENGHT];		//UdpÕ¯ø⁄∑¢ÀÕ ˝æ›
extern uint8 g_udpnet_rx_buf_tmp[MAXUDPDATALENGHT];	//UdpÕ¯ø⁄Ω” ’ ˝æ›
extern uint8 g_udpnet_tx_buf_tmp[MAXUDPDATALENGHT];	//UdpÕ¯ø⁄∑¢ÀÕ ˝æ›

//*****************************OBU∑¢––÷∏¡Ó÷°∂®“Â***********************//
//C0 -- ≥ı ºªØ÷∏¡Ó£¨∂‘OBU≥ı ºªØ±‡≥Ã∆˜Ω¯––π§◊˜≤Œ ˝…Ë∂®
typedef struct _PROG_COMM_C0 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;			//÷∏¡Ó¥˙¬Î
    uint8 UnixTime[4]; 		//UNIX ±º‰
    uint8 BSTInterval; 		//BSTº‰∏Ù ±º‰(µ•Œª£∫∫¡√Î£¨Õ∆ºˆ»°÷µŒ™0x20)
    uint8 TxPower; 			//π¶¬ º∂ ˝
    uint8 PHYChannelID; 	//–≈µ¿∫≈
    uint8 CRCMode;			//CRCƒ£ Ω 00:π˙±Í£¨01£∫±±æ©
    uint8 RFPower;			//…‰∆µ∞ÂµÁ‘¥ø™πÿ
    uint8 PSAMPower;		//PSAMµÁ‘¥ø™πÿ
    uint8 run_mode;			//‘À––ƒ£ Ω,øÿ÷∆–≈œ¢µƒ¥Ú”°
    uint8 Reserved[2]; 		//±£¡Ù◊÷Ω⁄£¨ÃÓ≥‰0
} PROG_COMM_C0;
//C1 -- µ•∏ˆOBU≥ı ºªØ∑¢––≤Ÿ◊˜ø™ º÷∏¡Ó
typedef struct _PROG_COMM_C1 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;			//÷∏¡Ó¥˙¬Î
    uint8 ObuInitMode;		//÷∏ æOBU≥ı ºªØ∑¢––≤Ÿ◊˜µƒƒ£ Ω:0--OBU≥ı ºªØ; 1--OBUœµÕ≥–≈œ¢∏¸–¬(∫¨∑¿≤π¶ƒ‹…Ë÷√);
    //2--OBU≥µ¡æ–≈œ¢∏¸–¬;3--ESAM–≈œ¢∂¡»°
    uint8 Reserved[25];		//±£¡Ù◊÷Ω⁄
    //Reserved[0]  ÷–µƒƒ⁄»›¥˙±Ìµƒ∫¨“Â «£∫–≈µ¿∫≈
    //Reserved[10] ÷–µƒƒ⁄»›¥˙±Ìµƒ∫¨“Â «£∫0£∫Õ—ª˙ π”√÷∏Œ∆¬Î 1£∫Õ—ª˙ π”√ ˝◊÷«©√˚
} PROG_COMM_C1;
//C2 -- √‹‘øÃÊªª÷∏¡Ó÷°
typedef struct _PROG_COMM_C2 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;			//÷∏¡Ó¥˙¬Î
    uint8 KCType; 			//÷∏ æ√‹‘øƒ∏ø®¿‡–Õ
    uint8 TCType; 			//0-Œﬁ¥´ ‰ø®±£ª§£ª1-”–¥´ ‰ø®±£ª§£ª∆‰À˚-±£¡Ù
    uint8 Contractsn[8];  	//∫œÕ¨–Ú¡–∫≈
    uint8 Reserved[5]; 		//±£¡Ù◊÷Ω⁄,ÃÓ≥‰0
} PROG_COMM_C2;
//C3 -- Œƒº˛∏¸–¬÷∏¡Ó
typedef struct _PROG_COMM_C3 {
    uint8 PsamDisperse;			//PSAMø®√‹‘ø∑÷…¢¿‡–Õ£¨≤ªÕ¨µÿ«¯≤ªÕ¨
    uint8 RSCTL;				// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;				//÷∏¡Ó¥˙¬Î
    uint8 NumOfFiles; 			//–Ë“™∏¸–¬µƒŒƒº˛ ˝¡ø
    uint8 DIDnFID[10]; 			//÷∏ æ–Ë“™∏¸–¬µƒŒƒº˛µƒƒø¬º∫≈∫ÕŒƒº˛∫≈
    uint8 Offset[10]; 			//Œƒº˛µÿ÷∑∆´“∆¡ø
    uint8 Length[10]; 			//–Ë“™–¥»Îµƒ ˝æ›≥§∂»
    uint8 FileContent[10][100];	//–Ë“™–¥»Îµƒ ˝æ›ƒ⁄»›
} PROG_COMM_C3;
//C4 -- Œƒº˛∂¡»°÷∏¡Ó
typedef struct _PROG_COMM_C4 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;			//÷∏¡Ó¥˙¬Î
    uint8 NumOfFiles; 		//–Ë“™∂¡»°µƒŒƒº˛ ˝¡ø
    uint8 DIDnFID[10]; 		//÷∏ æ–Ë“™∂¡»°µƒŒƒº˛µƒƒø¬º∫≈∫ÕŒƒº˛∫≈
    uint8 Offset[10]; 		//Œƒº˛µÿ÷∑∆´“∆¡ø(–Ë“™∂¡»° ˝æ›µƒ∆ ºµÿ÷∑)
    uint8 Length[10]; 		//–Ë“™∂¡»°µƒ ˝æ›≥§∂»
    uint8 C4Flag;
} PROG_COMM_C4;
//C5 -- Ω· ¯Ωª“◊÷∏¡Ó
typedef struct _PROG_COMM_C5 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;			//÷∏¡Ó¥˙¬Î
    uint8 SetMMIMode;		//÷∏ æOBUµƒ”√ªßÃ· æ∑Ω Ω
    uint8 Reserved[5];		//±£¡Ù◊÷Ω⁄£¨ÃÓ≥‰0
} PROG_COMM_C5;
//C6 -- MACº∆À„÷∏¡Ó
typedef struct _PROG_COMM_C6 {
    uint8 Length;			//”√”⁄MACº∆À„µƒ ˝æ›≥§∂»
    uint8 Content[255];		//”√”⁄MACº∆À„µƒ ˝æ›ƒ⁄»›
} PROG_COMM_C6;

typedef struct _PROG_COMM_C8 {
    uint8 Datetime[4];		//≥ˆ»Îø⁄ ±º‰(UNIX ±º‰)(Ωª“◊ ±º‰)
    uint8 psamid[6];
    uint8 Tr_SN[4]; 		//÷’∂ÀΩª“◊¡˜ÀÆ∫≈
    uint8 station_code[2]; 	// ’∑—’æ±‡¬Î
    uint8 lane_code; 		// ’∑—≥µµ¿±‡¬Î
    uint8 balance[2]; 		//”‡∂Ó
    uint8 vehicle_state; 	//≥µ¡æ◊¥Ã¨–≈œ¢
    uint8 battery;			//obuµÁ¡ø–≈œ¢
    uint8 mac_check_info;	//obu MAC ID–£—È–≈œ¢£¨±»Ωœobu÷–¥Ê¥¢µƒmacid∫Õesam÷–¥Ê¥¢µƒmacid «∑Ò“ª÷¬
    uint8 Obu_Tr_SN[4];		//obuµƒ÷’∂ÀΩª“◊¡˜ÀÆ∫≈
    uint8 tac[8];
    uint8 key_version; 		//√‹‘ø∞Ê±æ∫≈
} PROG_COMM_C8;
// B0 -- …Ë±∏◊¥Ã¨–≈œ¢÷°
typedef struct _PROG_COMM_B0 {
    uint8 RSCTL;				// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;			//÷∏¡Ó¥˙¬Î
    uint8 RSUStatus; 			//OBU≥ı ºªØ±‡≥Ã∆˜÷˜◊¥Ã¨≤Œ ˝£¨00±Ì æ’˝≥££¨∑Ò‘Ú±Ì æ“Ï≥£
    uint8 RSUManuID[2]; 		//OBU≥ı ºªØ±‡≥Ã∆˜≥ß…Ã¥˙¬Î£¨16Ω¯÷∆±Ì æ
    uint8 RSUID[2]; 			//OBU≥ı ºªØ±‡≥Ã∆˜±‡∫≈
    uint8 RSUVersion[2]; 		//OBU≥ı ºªØ±‡≥Ã∆˜»Ìº˛∞Ê±æ∫≈
    uint8 Reserved[10]; 		//±£¡Ù◊÷Ω⁄£¨ÃÓ≥‰0
} PROG_COMM_B0;
//B1
typedef struct _PROG_COMM_B1 {
    uint8 RSCTL;					// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;				//÷∏¡Ó¥˙¬Î
    uint8 ErrorCode; 				//÷¥––◊¥Ã¨¥˙¬Î
    uint8 ContractProvider[8]; 		//∑˛ŒÒÃ·π©…Ã√˚≥∆
    uint8 ContractType; 			//–≠‘º¿‡–Õ
    uint8 ContractVersion; 			//∫œÕ¨∞Ê±æ
    uint8 ContractSerialNumber[8];	//∫œÕ¨–Ú¡–∫≈
    uint8 ContractSignedDate[4]; 	//∫œÕ¨«© »’∆⁄
    uint8 ContractExpiredDate[4];	//∫œÕ¨π˝∆⁄»’∆⁄
    uint8 Reserved[6]; 				//±£¡Ù◊÷Ω⁄£¨ÃÓ≥‰0
    uint8 FileContent_ICCInfo[100];		//0015Œƒº˛–≈œ¢
} PROG_COMM_B1;
//B2 -- ∏¸–¬√¸¡Ó÷¥––œÏ”¶÷°
typedef struct _PROG_COMM_B2 {
    uint8 RSCTL;				// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;			//÷∏¡Ó¥˙¬Î
    uint8 ErrorCode;			//÷¥––◊¥Ã¨¥˙¬Î£¨0x00--≥…π¶£¨∆‰À˚-- ß∞‹
    uint8 Reserved[5]; 			//±£¡Ù◊÷Ω⁄£¨ÃÓ≥‰0
} PROG_COMM_B2;
// B3
typedef struct _PROG_COMM_B3 {
    uint8 RSCTL;				// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;			//÷∏¡Ó¥˙¬Î
    uint8 ErrorCode; 			//÷¥––◊¥Ã¨¥˙¬Î,0x00--≥…π¶£¨∆‰À˚-- ß∞‹
    uint8 NumOfFiles; 			//∑µªÿµƒŒƒº˛µƒ ˝¡ø
    uint8 Length[10]; 			//∑µªÿµƒ ˝æ›µƒ≥§∂»
    uint8 FileContent[10][100];	//∑µªÿµƒ ˝æ›µƒƒ⁄»›
    
} PROG_COMM_B3;
typedef struct _PROG_COMM_DATA {
    
    uint8 len0; // PSAMø®√‹‘ø∑÷…¢¿‡–Õ£¨≤ªÕ¨µÿ«¯≤ªÕ¨
    uint8 sendbuf[100]; // PSAMø®√‹‘ø∑÷…¢¿‡–Õ£¨≤ªÕ¨µÿ«¯≤ªÕ¨
    uint8 len; // PSAMø®√‹‘ø∑÷…¢¿‡–Õ£¨≤ªÕ¨µÿ«¯≤ªÕ¨
    uint8 FileContent[100];
    // –Ë“™–¥»Îµƒ ˝æ›ƒ⁄»›
}PROG_COMM_DATA;
//typedef struct _PROG_COMM_B3LIST {
//	uint8 RSCTL;				// ˝æ›÷°–Ú¡–∫≈
//	uint8 FrameType;			//÷∏¡Ó¥˙¬Î
//	uint8 ErrorCode; 			//÷¥––◊¥Ã¨¥˙¬Î,0x00--≥…π¶£¨∆‰À˚-- ß∞‹
//	uint8 NumOfFiles; 			//∑µªÿµƒŒƒº˛µƒ ˝¡ø
//	uint8 Length[10]; 			//∑µªÿµƒ ˝æ›µƒ≥§∂»
//	uint8 FileContent[50][100];	//∑µªÿµƒ ˝æ›µƒƒ⁄»›
//
//} PROG_COMM_B3LIST;
//B4 -- Ωª“◊Ω· ¯œÏ”¶÷°
typedef struct _PROG_COMM_B4 {
    uint8 RSCTL;				// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;			//÷∏¡Ó¥˙¬Î
    uint8 ErrorCode; 			//÷¥––◊¥Ã¨¥˙¬Î£¨0x00--≥…π¶;∆‰À˚-- ß∞‹
} PROG_COMM_B4;
//B5 -- MAC ˝æ›÷°
typedef struct _PROG_COMM_B5 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;		//÷∏¡Ó¥˙¬Î
    uint8 ErrorCode; 		//÷¥––◊¥Ã¨¥˙¬Î£¨0x00--≥…π¶£ª∆‰À˚-- ß∞‹
    uint8 Length; 			//÷∏ æMACµƒ≥§∂»£¨Õ®≥£Œ™0x04ªÚ0x08
    uint8 MAC[8]; 			//PSAMº∆À„µ√µΩµƒMAC÷µ
} PROG_COMM_B5;
//BA -- STR715≥Ã–Ú∞Ê±æ ˝æ›÷°
typedef struct _PROG_COMM_BA {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;		//÷∏¡Ó¥˙¬Î
    uint8 ErrorCode; 		//÷¥––◊¥Ã¨¥˙¬Î£¨0x00--≥…π¶£ª∆‰À˚-- ß∞‹
    uint8 Version[32]; 		//STR715≥Ã–Ú∞Ê±æ
} PROG_COMM_BA;
//*****************************OBUΩª“◊÷∏¡Ó÷°∂®“Â***********************//
//C0 -- ≥ı ºªØ÷∏¡Ó
typedef struct _PROG_TRADE_C0 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;			//÷∏¡Ó¥˙¬Î
    uint8 UnixTime[4]; 		//UNIX ±º‰
    uint8 BSTInterval; 		//BSTº‰∏Ù ±º‰(µ•Œª£∫∫¡√Î£¨Õ∆ºˆ»°÷µŒ™0x20)
    uint8 TxPower; 			//π¶¬ º∂ ˝
    uint8 PHYChannelID; 	//–≈µ¿∫≈
    uint8 CRCMode;			//CRCƒ£ Ω 00:π˙±Í£¨01£∫±±æ©
    uint8 RFPower;			//…‰∆µ∞ÂµÁ‘¥ø™πÿ
    uint8 PSAMPower;		//PSAMµÁ‘¥ø™πÿ
    uint8 run_mode;			//‘À––ƒ£ Ω,øÿ÷∆–≈œ¢µƒ¥Ú”°
    uint8 LaneMode;			//≥µµ¿ƒ£ Ω£¨»°÷µ∑÷±Œ™3/4/6/7/8
    uint8 WaitTime;			//◊Ó–°÷ÿ∂¡ ±º‰
    uint8 TransClass;		//0: º«’Àø®∫Õ¥¢÷µø®∂º «¥´Õ≥Ωª“◊
    //1: º«’Àø®∫Õ¥¢÷µø®∂º «∏¥∫œΩª“◊
    //2: º«’Àø® «¥´Õ≥Ωª“◊£¨¥¢÷µø® «∏¥∫œΩª“◊
    uint8 Reserved[2]; 		//±£¡Ù◊÷Ω⁄£¨ÃÓ≥‰0
    //◊‘∂®“Â◊÷Ω⁄
    uint8 EsamType;			//esam¿‡–Õ,0:»Ìsam, 1:”≤sam
} PROG_TRADE_C0;
// C1 -- ºÃ–¯Ωª“◊÷∏¡Ó
typedef struct _PROG_TRADE_C1 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;			//÷∏¡Ó¥˙¬Î
    uint8 OBUID[4];			//OBU∫≈£®‘⁄Ãÿ∂®«Èøˆœ¬ø…ÃÓ≥‰0£©
    uint8 OBUDivFactor[8];	//OBU“ªº∂∑÷…¢“Ú◊”
} PROG_TRADE_C1;
//C2 -- Õ£÷πΩª“◊÷∏¡Ó
typedef struct _PROG_TRADE_C2 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;			//÷∏¡Ó¥˙¬Î
    uint8 OBUID[4];			//OBU∫≈£®‘⁄Ãÿ∂®«Èøˆœ¬ø…ÃÓ≥‰0£©
    uint8 StopType;			//1£∫÷ÿ–¬À—À˜OBU£¨≤ª≈–∂œOBU∫≈°£
    //2£∫÷ÿ–¬∑¢ÀÕµ±«∞÷°£¨÷ª∂‘B3°¢B4”––ß°£
} PROG_TRADE_C2;
//C3 -- –¥’æ–≈œ¢÷∏¡Ó
typedef struct _PROG_TRADE_C3 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;			//÷∏¡Ó¥˙¬Î
    uint8 OBUID[4];			//OBU∫≈£®‘⁄Ãÿ∂®«Èøˆœ¬ø…ÃÓ≥‰0£©
    uint8 CardDivFactor[8];	//ø®“ªº∂∑÷…¢“Ú◊”
    uint8 TransSerial[4];	//Ωª“◊À≥–Ú∫≈°£¥À÷µ‘⁄B5◊˜Œ™PSAMø®Ωª“◊À≥–Ú∫≈∑µªÿ
    uint8 PurchaseTime[7];	//YYYYYMMDDHHmmSS£¨”√¥À ±º‰»•º∆À„TAC¬Î
    uint8 Station[36];		//π˝’æ–≈œ¢£®0012Œƒº˛£©
} PROG_TRADE_C3;
//C6 -- œ˚∑—≤¢–¥’æ–≈œ¢÷∏¡Ó
typedef struct _PROG_TRADE_C6 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;			//÷∏¡Ó¥˙¬Î
    uint8 OBUID[4];			//OBU∫≈£®‘⁄Ãÿ∂®«Èøˆœ¬ø…ÃÓ≥‰0£©
    uint8 CardDivFactor[8];	//ø®“ªº∂∑÷…¢“Ú◊”
    uint8 TransSerial[4];	//Ωª“◊À≥–Ú∫≈°£¥À÷µ‘⁄B5◊˜Œ™PSAMø®Ωª“◊À≥–Ú∫≈∑µªÿ
    uint8 ConsumeMoney[4];	//ø€øÓ∂Ó£¨∏ﬂŒª‘⁄«∞
    uint8 PurchaseTime[7];	//YYYYYMMDDHHmmSS£¨”√¥À ±º‰»•º∆À„TAC¬Î
    uint8 Station[36];		//π˝’æ–≈œ¢£®π˙±Íø®£∫0012Œƒº˛ªÚ’ﬂ0019∫Û36∏ˆ◊÷Ω⁄£©
} PROG_TRADE_C6;
//C7 -- “Ï≥£¥¶¿Ì÷∏¡Ó
typedef struct _PROG_TRADE_C7 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 CMDType;			//÷∏¡Ó¥˙¬Î
    uint8 OBUID[4];			//OBU∫≈£®‘⁄Ãÿ∂®«Èøˆœ¬ø…ÃÓ≥‰0£©
    uint8 CardDivFactor[8];	//ø®“ªº∂∑÷…¢“Ú◊”
    uint8 TransSerial[4];	//Ωª“◊À≥–Ú∫≈°£¥À÷µ‘⁄B5◊˜Œ™PSAMø®Ωª“◊À≥–Ú∫≈∑µªÿ
    uint8 PurchaseTime[7];	//YYYYYMMDDHHmmSS£¨”√¥À ±º‰»•º∆À„TAC¬Î
    uint8 Station[36];		//π˝’æ–≈œ¢£®π˙±Íø®£∫0012Œƒº˛£©
} PROG_TRADE_C7;
//B0 -- RSU…Ë±∏◊¥Ã¨–≈œ¢÷°
typedef struct _PROG_TRADE_B0 {
    uint8 RSCTL;				// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;			//÷∏¡Ó¥˙¬Î
    uint8 RSUStatus; 			//RSU÷˜◊¥Ã¨≤Œ ˝£ª0x00±Ì æ’˝≥££¨∑Ò‘Ú±Ì æ“Ï≥£
    uint8 PSAMNum; 				//PSAMø®∏ˆ ˝
    uint8 RSUTerminalId1[6]; 	//PSAMø®÷’∂Àª˙±‡∫≈£¨π˙±ÍPSAMø®∫≈
    uint8 RSUTerminalId2[6]; 	//PSAMø®÷’∂À±‡∫≈£¨“ªø®Õ®PSAMø®∫≈
    uint8 RSUAlgId; 			//À„∑®±Í ∂
    uint8 RSUManuID[2]; 		//RSU≥ß…Ã¥˙¬Î£¨16Ω¯÷∆±Ì æ
    uint8 RSUID[2]; 			//RSU±‡∫≈£¨16Ω¯÷∆±Ì æ
    uint8 RSUVersion[2]; 		//RSU»Ìº˛∞Ê±æ∫≈£¨16Ω¯÷∆±Ì æ
    uint8 Reserved[5]; 			//±£¡Ù◊÷Ω⁄
} PROG_TRADE_B0;
//B1 -- µÿ∏–◊¥Ã¨–≈œ¢÷°
typedef struct _PROG_TRADE_B1 {
    uint8 RSCTL;			// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;		//÷∏¡Ó¥˙¬Î
    uint8 RsuIoStatus; 		//µÿ∏–◊¥Ã¨
    uint8 RsuIoChgSum; 		//µÿ∏–◊¥Ã¨±‰ªØº∆ ˝
} PROG_TRADE_B1;
//B2 -- OBU∫≈–≈œ¢÷°
typedef struct _PROG_TRADE_B2 {
    uint8 RSCTL;				//  ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;			// ÷∏¡Ó¥˙¬Î
    uint8 OBUID[4]; 			//OBU∫≈
    uint8 ErrorCode; 			//÷¥––◊¥Ã¨¥˙¬Î£¨»°÷µŒ™00H£¨≤≈”–∫Û–¯ ˝æ›ƒ⁄»›°£
    uint8 IssuerIdentifier[8];	//∑¢––…Ã¥˙¬Î
    uint8 SerialNumber[8];		//”¶”√–Ú¡–∫≈
    uint8 DateofIssue[4];		//∆Ù”√»’∆⁄
    uint8 DateofExpire[4];		//π˝∆⁄»’∆⁄
    uint8 EquipmentStatus;		//…Ë±∏¿‡–Õ
    uint8 OBUStatus[2];			//OBU◊¥Ã¨
    //◊‘∂®“Â◊÷Ω⁄
    uint8 CardType;			//¥”‘§∂¡µƒ0015Œƒº˛µ√µΩ
} PROG_TRADE_B2;
//B3 --OBU Ù–‘÷°
typedef struct _PROG_TRADE_B3 {
    uint8 RSCTL;				//  ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;			// ÷∏¡Ó¥˙¬Î
    uint8 OBUID[4]; 			//OBU∫≈
    uint8 ErrorCode; 			//÷¥––◊¥Ã¨¥˙¬Î,¥À¥¶»°÷µ0x00
    uint8 VehicleLicencePlateNumber[12]; 	//OBUº«‘ÿµƒ≥µ≈∆∫≈
    uint8 VehicleLicencePlateColor[2]; 		//≥µ≈∆—’…´
    uint8 VehicleClass; 		//≥µ¡æ¿‡–Õ
    uint8 VehicleUserType; 		//≥µ¡æ”√ªß¿‡–Õ
} PROG_TRADE_B3;
//B4 -- ICø®–≈œ¢÷°
typedef struct _PROG_TRADE_B4 {
    uint8 RSCTL;				// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;			//÷∏¡Ó¥˙¬Î
    uint8 OBUID[4]; 			//OBU∫≈
    uint8 ErrorCode; 			//÷¥––◊¥Ã¨¥˙¬Î£¨¥À¥¶»°÷µ0x00
    uint8 CardType; 			//ø®¿‡–Õ£¨00-π˙±ÍCPUø®£¨01-“ªø®Õ®ø®
    uint8 PhysicalCardType; //ŒÔ¿Ìø®¿‡–Õ£¨0x00:π˙±ÍCPUø®;0x01:MIFARE 1(S50)ø®;0x02:MIFARE PRO(ªÚºÊ»›)ø®;0x03:Ultra Light;0x05:MIFARE 1(S70)ø®
    uint8 TransType; 			//Ωª“◊¿‡–Õ£¨00-¥´Õ≥Ωª“◊£¨0x10-∏¥∫œΩª“◊°£÷ª”√”⁄π˙±ÍCPUø®£¨“ªø®Õ®Œ™00.
    uint8 CardRestMoney[4]; 	//ø®”‡∂Ó£¨∏ﬂŒª‘⁄«∞
    uint8 CardID[4]; 			//“ªø®Õ®∫ÕCPUø®µƒŒÔ¿Ìø®∫≈(CPUø®ø…“‘√ª”–£¨‘›ÃÓ0)
    uint8 IssuerInfo[43]; 		//ø®∑¢–––≈œ¢
    uint8 LastStation[39];		//…œ¥Œπ˝’æ–≈œ¢
} PROG_TRADE_B4;
//B5 -- ≥…π¶Ωª“◊Ω· ¯÷°
typedef struct _PROG_TRADE_B5 {
    uint8 RSCTL;				// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;			//÷∏¡Ó¥˙¬Î
    uint8 OBUID[4]; 			//OBU∫≈
    uint8 ErrorCode; 			//÷¥––◊¥Ã¨¥˙¬Î
    uint8 WrFileTime[4]; 		//–¥Œƒº˛ ±º‰£¨UNIX∏Ò Ω
    uint8 PSAMNo[6]; 			//PSAMø®÷’∂Àª˙±‡∫≈
    uint8 TransTime[7]; 		//Ωª“◊ ±º‰£¨∏Ò Ω:YYYYMMDDHHMMSS
    uint8 TransType; 			//Ωª“◊¿‡–Õ
    uint8 TAC[4]; 				//TAC¬Î
    uint8 ICCPayserial[2]; 		//CPUø®Ωª“◊–Ú∫≈
    uint8 PSAMTransSerial[4]; 	//PSAMø®Ωª“◊–Ú∫≈£¨º«’Àø®ÃÓ≥‰0
    uint8 CardBalance[4]; 		//Ωª“◊∫Û”‡∂Ó£¨∏ﬂŒª‘⁄«∞£¨∂¡»°ø®”‡∂Ó ß∞‹£¨∑µªÿ0xFFFFFFFF=65535
} PROG_TRADE_B5;
typedef struct _PROG_COMM_ICC {
    uint8 RSCTL;				// ˝æ›÷°–Ú¡–∫≈
    uint8 FrameType;			//÷∏¡Ó¥˙¬Î
    uint8 Operate; 				//≤Ÿ◊˜¿‡–Õ
    uint8 ErrorCode; 			//¥ÌŒÛ¬Î
    uint8 TotalLength; 			//÷∏¡Ó◊‹≥§∂»
    uint8 NumOfCmds; 			//Œƒº˛ ˝
    uint8 Length[10]; 			//Œƒº˛≥§∂»
    uint8 Content[10][128]; 	//Œƒº˛ƒ⁄»›
    uint8 Status[10][2]; 		//÷∏¡Ó÷¥––◊¥Ã¨¬Î
} PROG_COMM_ICC;

/********** –¬ÃÌº”‘⁄œﬂ∑¢––÷° add by wll 2013-3-25 10:28:08****************************/

//ÃÙ’Ω÷°
typedef struct _FRAME_CHALLENGE {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈	1	0x00£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞ÃÙ’Ω÷°°±°£
    uint8 FrameLen;					//÷°≥§∂»	1	0x20£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 FillBytes[2];				//ÃÓ≥‰◊÷Ω⁄	2	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 RandomNumber[4];//ÀÊª˙ ˝	4	∑¢∆ÃÙ’Ωµƒ“ª∑Ω…˙≥…£ª∆‰÷–∂‘”⁄÷’∂À£¨±ÿ–Î «¥”ICø®Õ®π˝Get Challenge÷∏¡ÓªÒµ√°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	∑¢∆ÃÙ’Ωµƒ ±øÃ£¨”…∑¢∆ÃÙ’Ωµƒ“ª∑Ω…˙≥…£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 ExtendInformation[16];	//¿©’π–≈œ¢	16	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_CHALLENGE;

// »œ÷§–≈œ¢÷°
typedef struct _FRAME_VALIDATE_RQ {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈	1	0x01£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞»œ÷§–≈œ¢÷°°±°£
    uint8 FrameLen;					//÷°≥§∂»	1	0x50£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 AdminCardIssueID[2];//π‹¿Ì‘±ø®∑¢––Õ¯¬Á±‡∫≈	2	—πÀı ÆΩ¯÷∆±Ì æµƒ4Œª∑¢––…ÃÕ¯¬Á±‡∫≈£¨”√”⁄∂‘±»ºÏ≤Èπ‹¿Ì‘±ø® «∑Ò Ù”⁄œµÕ≥ƒ⁄°£
    uint8 AdminCardSerialID[8];		//π‹¿Ì‘±ø®–Ú¡–∫≈	8	÷∏π‹¿Ì‘±ø®µƒ8◊÷Ω⁄ƒ⁄≤ø±‡∫≈£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 TerminalPhysicsID[40];//÷’∂ÀŒÔ¿Ì¥Æ∫≈	40	≥ß…ÃÃ·π©µƒ”≤º˛Õ≥“ª±‡∫≈(∫Û≤π°Ø\x0°Ø)£¨≤ª◊„40◊÷Ω⁄∫Û≤π°∞0x00°±°£ASCII±‡¬Î°£
    uint8 IdentityValidate[16];		//…Ì∑›»œ÷§¬Î	16	…˙≥…À„∑®º˚6.3.3Ω⁄°£
    uint8 ExtendInformation[12];	//¿©≥‰–≈œ¢	12	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰
} FRAME_VALIDATE_RQ;

//œÏ”¶»œ÷§–≈œ¢÷°
typedef struct _FRAME_VALIDATE_RS {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈	1	0x02£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞œÏ”¶»œ÷§–≈œ¢÷°°±°£
    uint8 FrameLen;					//÷°≥§∂»÷°≥§∂»	1	0x30£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 TerminalNO[6];	//÷’∂Àª˙±‡∫≈	6	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «÷’∂ÀµƒŒÔ¿Ì¥Æ∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨∂˛Ω¯÷∆◊÷Ω⁄¡˜°£
    uint8 AdminNO[4];//π‹¿Ì‘±±‡∫≈	4	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «π‹¿Ì‘±ø®∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨±‡¬Î∑Ω Ω£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 UnityTimeOutDuration[2]; /*ª·ª∞√‹‘ø’˚ÃÂ≥¨ ± ±≥§	2	÷∏ª·ª∞√‹‘ø…˙≥…∫Ûµƒ”––ß ±≥§,≥¨π˝∏√ ±≥§ª·ª∞√‹‘ø◊‘∂Ø ß–ß°£
                                    //“‘∑÷÷”Œ™µ•Œª£¨’˚ ˝ ˝÷µ–Õ°£*/
    uint8 FreeTimeOutDuration[2]; /*ª·ª∞√‹‘øø’œ–≥¨ ± ±≥§	2	÷∏ª·ª∞√‹‘ø…˙≥…∫Û£¨ ÷≥÷ª˙√ª”–÷¥––Ωª“◊,ø’œ–µƒ ±º‰≥§∂»≥¨π˝∏√ ±≥§ª·ª∞√‹‘ø◊‘∂Ø ß–ß°£
                                   //“‘∑÷÷”Œ™µ•Œª£¨’˚ ˝ ˝÷µ–Õ°£*/
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 IdentityValidate[16];		//…Ì∑›»œ÷§¬Î	16	…˙≥…À„∑®º˚6.4.3Ω⁄°£
    uint8 ExtendInformation[8];		//¿©≥‰–≈œ¢	8	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_VALIDATE_RS;

//OBU–¥≥µ¡æ–≈œ¢«Î«Û÷°(÷’∂À->«∞÷√ª˙)
typedef struct _FRAME_WRITE_VEHINFO_RQ {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈	1	0x13£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBU–¥≥µ¡æ–≈œ¢«Î«Û÷°°±°£
    uint8 FrameLen;					//÷°≥§∂»	1	0x50£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 TerminalNO[6];	//÷’∂Àª˙±‡∫≈	6	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «÷’∂ÀµƒŒÔ¿Ì¥Æ∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨∂˛Ω¯÷∆◊÷Ω⁄¡˜°£
    uint8 AdminNO[4];//π‹¿Ì‘±±‡∫≈	4	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «π‹¿Ì‘±ø®∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨±‡¬Î∑Ω Ω£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 TerminalTradeNO[4];//÷’∂ÀΩª“◊–Ú∫≈		4	÷’∂ÀΩª“◊»’÷æ÷–µ±«∞µƒ¡˜ÀÆ∫≈£¨”…÷’∂À…˙≥…º∞Œ¨ª§£¨µ±»’≤ªø…÷ÿ∏¥£¨∂‘”⁄Õ¨“ª± Ωª“◊£¨∏˜÷°µƒΩª“◊–Ú∫≈±ÿ–Î“ª÷¬£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 FillBytes[3];				//ÃÓ≥‰◊÷Ω⁄	3	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 OBUContractVersion;		//OBU ∫œÕ¨∞Ê±æ	1	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨∞Ê±æ°±£¨’˚ ˝ ˝÷µ–Õ°£
    uint8 OBUContractSerialNO[8];	//OBU∫œÕ¨–Ú¡–∫≈	8	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨–Ú¡–∫≈°±£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 GBCPUCardID[8];//π˙±ÍCPUø®–Ú¡–∫≈	8	÷∏≤Â»ÎOBUƒ⁄≤øµƒπ˙±ÍCPUø®µƒ8◊÷Ω⁄ø®∫≈£¨»ÁŒ¥≤Â»Îø®∆¨‘Ú“‘°∞0xFF°±ÃÓ≥‰£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 RandomNumber[8];			//ÀÊª˙ ˝	8		‘⁄OBU π”√Get Challenge÷∏¡ÓªÒµ√ÀÊª˙ ˝°£
    // 	°∞OBU∫œÕ¨∞Ê±æ°±Œ™°∞0x00°±°¢°∞0x10°±ªÚ°∞0x16°± ±£¨∏√ÀÊª˙ ˝”√”⁄–¥OBU≥µ¡æ–≈œ¢Œƒº˛ ±º∆À„MAC£¨«∞4◊÷Ω⁄”––ß£¨∫Û≤π4◊÷Ω⁄0°£
    // 	°∞OBU∫œÕ¨∞Ê±æ°±Œ™0x11 ±£¨∏√ÀÊª˙ ˝”√”⁄÷Æ∫ÛµƒÀ´œÚ»œ÷§«∞÷√ª˙º∆À„»œ÷§–≈
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏÷’∂À ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];			//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[12];	//¿©≥‰–≈œ¢	12	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_WRITE_VEHINFO_RQ;

//OBU–¥≥µ¡æ–≈œ¢”¶¥÷°(«∞÷√ª˙->÷’∂À)
typedef struct _FRAME_WRITE_VEHINFO_RS {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈	1	0x14£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBU–¥≥µ¡æ–≈œ¢”¶¥÷°°±°£
    uint8 FrameLen;					//÷°≥§∂»	1	0x70£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;				//”¶¥∑µªÿ¬Î	1	0x00±Ì æ°∞÷∏¡Óø…”√”⁄–¥OBUŒƒº˛°±£¨∆‰À¸÷µº˚6.19Ω⁄°£
    uint8 FillByte;					//ÃÓ≥‰◊÷Ω⁄	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TerminalTradeNO[4];	//÷’∂ÀΩª“◊–Ú∫≈	4	”Î÷’∂À…œÀÕµƒ°∞OBU–¥≥µ¡æ–≈œ¢«Î«Û÷°°±÷–µƒ°∞÷’∂ÀΩª“◊–Ú∫≈°±œ‡Õ¨°£
    uint8 WriteVehInfo[68];	//–¥≥µ¡æ–≈œ¢÷∏¡Ó	68		 π”√ESAMµƒ°∞UpdateBinary°±÷∏¡Ó“‘∆´“∆∏≤∏«µƒ∑Ω Ω£¨ π”√√˜Œƒº”MAC∏¸–¬OBU≥µ¡æ–≈œ¢Œƒº˛µƒ«∞59∏ˆ◊÷Ω⁄£¨”…«∞÷√ª˙Ã·π©°£æﬂÃÂ÷∏¡Ó≤Œøº°∂ ’∑—π´¬∑¡™Õ¯µÁ◊”≤ªÕ£≥µ ’∑—ºº ı“™«Û°∑÷–OBE-SAMµƒœ‡πÿπÊ∂®°£÷ª”–°∞”¶¥∑µªÿ¬Î°±∑µªÿ°∞0x00°± ±∏√◊÷∂Œ≤≈”––ß£¨∑Ò‘Ú∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£
    // 	°∞OBU–¥≥µ¡æ–≈œ¢«Î«Û÷°°±÷–°∞OBU∫œÕ¨∞Ê±æ°±◊÷∂ŒŒ™°∞0x11°± ±£¨∏√◊÷∂Œ”√”⁄«∞÷√ª˙∑µªÿÀ´œÚ»œ÷§ ±º∆À„µƒ»œ÷§–≈œ¢£¨«∞8◊÷Ω⁄”––ß£¨÷Æ∫Û≤π0°£
    uint8 TradeRandomNumber[8];	//Ωª“◊ÀÊª˙ ˝	8		 ««∞÷√ª˙∂‘Ωª“◊µƒŒ®“ª–‘…˙≥…µƒ±Í ∂£¨÷’∂À‘⁄÷Æ∫Û…œÀÕµƒ ˝æ›÷°÷–∂º”¶∏√–Ø¥¯∏√ÀÊª˙ ˝£¨±Ì√˜Œ™Õ¨“ª± Ωª“◊°£Õ¨“ª± Ωª“◊∏√ÀÊª˙ ˝œ‡Õ¨£¨≤ªÕ¨µƒΩª“◊∏√ÀÊª˙ ˝≤ªÕ¨°£
    // 	∏√ÀÊª˙ ˝“≤‘⁄÷’∂À∂¡»°≥µ¡æ–≈œ¢ ±≤Œ”Î π”√£¨”√”⁄…˙≥…≥µ¡æ–≈œ¢µƒ√‹Œƒ°£
    // 	°∞OBU–¥≥µ¡æ–≈œ¢«Î«Û÷°°±÷–°∞OBU∫œÕ¨∞Ê±æ°±◊÷∂ŒŒ™°∞0x11°± ±£¨∏√◊÷∂Œ”√”⁄À´œÚ»œ÷§ ±«∞÷√ª˙∑µªÿÀÊª˙ ˝£¨π©OBUº∆À„»œ÷§–≈
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];			//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[4];		//¿©≥‰–≈œ¢	4	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_WRITE_VEHINFO_RS;

//OBU∂¡≥µ¡æ–≈œ¢«Î«Û÷°(÷’∂À->«∞÷√ª˙)
typedef struct _FRAME_READ_VEHINFO_RQ {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈	1	0x15£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBU∂¡≥µ¡æ–≈œ¢«Î«Û÷°°±°£
    uint8 FrameLen;					//÷°≥§∂»	1	0x40£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»
    uint8 TerminalNO[6];	//÷’∂Àª˙±‡∫≈	6	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «÷’∂ÀµƒŒÔ¿Ì¥Æ∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨∂˛Ω¯÷∆◊÷Ω⁄¡˜°£
    uint8 AdminNO[4];//π‹¿Ì‘±±‡∫≈	4	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «π‹¿Ì‘±ø®∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨±‡¬Î∑Ω Ω£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 TerminalTradeNO[4];//÷’∂ÀΩª“◊–Ú∫≈	4	÷’∂ÀΩª“◊»’÷æ÷–µ±«∞µƒ¡˜ÀÆ∫≈£¨”…÷’∂À…˙≥…º∞Œ¨ª§£¨µ±»’≤ªø…÷ÿ∏¥£¨∂‘”⁄Õ¨“ª± Ωª“◊£¨∏˜÷°µƒΩª“◊–Ú∫≈±ÿ–Î“ª÷¬£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 FillBytes[3];				//ÃÓ≥‰◊÷Ω⁄	3	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 OBUContractVersion;		//OBU ∫œÕ¨∞Ê±æ	1	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨∞Ê±æ°±£¨’˚ ˝ ˝÷µ–Õ°£
    uint8 OBUContractSerialNO[8];	//OBU∫œÕ¨–Ú¡–∫≈	8	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨–Ú¡–∫≈°±£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏÷’∂À ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];			//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[12];	//¿©≥‰–≈œ¢	12	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_READ_VEHINFO_RQ;

//OBU∂¡≥µ¡æ–≈œ¢”¶¥÷°(«∞÷√ª˙->÷’∂À)
typedef struct _FRAME_READ_VEHINFO_RS {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈	1	0x16£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBU∂¡≥µ¡æ–≈œ¢”¶¥÷°°±°£
    uint8 FrameLen;					//÷°≥§∂»	1	0x30£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;				//”¶¥∑µªÿ¬Î	1	0x00±Ì æ°∞≥…π¶£¨ø…∂¡»°≥µ¡æ–≈œ¢°±£¨∆‰À¸÷µº˚6.19Ω⁄°£
    uint8 FillByte;					//ÃÓ≥‰◊÷Ω⁄	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TerminalTradeNO[4];	//÷’∂ÀΩª“◊–Ú∫≈	4	”Î÷’∂À…œÀÕµƒ°∞OBU∂¡≥µ¡æ–≈œ¢”¶¥÷°°±÷–µƒ°∞÷’∂ÀΩª“◊–Ú∫≈°±œ‡Õ¨°£
    uint8 TradeRandomNumber[8];	//Ωª“◊ÀÊª˙ ˝	8	 ««∞÷√ª˙∂‘Ωª“◊µƒŒ®“ª–‘…˙≥…µƒ±Í ∂£¨÷’∂À‘⁄÷Æ∫Û…œÀÕµƒ ˝æ›÷°÷–∂º”¶∏√–Ø¥¯∏√ÀÊª˙ ˝£¨±Ì√˜Œ™Õ¨“ª± Ωª“◊°£Õ¨“ª± Ωª“◊∏√ÀÊª˙ ˝œ‡Õ¨£¨≤ªÕ¨µƒΩª“◊∏√ÀÊª˙ ˝≤ªÕ¨°£Õ¨ ±£¨∏√ÀÊª˙ ˝“≤‘⁄÷’∂À∂¡»°≥µ¡æ–≈œ¢ ±≤Œ”Î π”√£¨”√”⁄…˙≥…≥µ¡æ–≈œ¢µƒ√‹Œƒ°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];			//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[8];		//¿©≥‰–≈œ¢	8	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_READ_VEHINFO_RS;

//–¥œµÕ≥–≈œ¢«Î«Û÷°
typedef struct _FRAME_WRITE_SYSINFO_RQ {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x17£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBU–¥œµÕ≥–≈œ¢«Î«Û÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x90£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 TerminalNO[6];	//÷’∂Àª˙±‡∫≈	6	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «÷’∂ÀµƒŒÔ¿Ì¥Æ∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨∂˛Ω¯÷∆◊÷Ω⁄¡˜°£
    uint8 AdminNO[4];//π‹¿Ì‘±±‡∫≈	4	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «π‹¿Ì‘±ø®∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨±‡¬Î∑Ω Ω£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 TerminalTradeNO[4];//÷’∂ÀΩª“◊–Ú∫≈	4	÷’∂ÀΩª“◊»’÷æ÷–µ±«∞µƒ¡˜ÀÆ∫≈£¨”…÷’∂À…˙≥…º∞Œ¨ª§£¨µ±»’≤ªø…÷ÿ∏¥£¨∂‘”⁄Õ¨“ª± Ωª“◊£¨∏˜÷°µƒΩª“◊–Ú∫≈±ÿ–Î“ª÷¬£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 TradeRandomNumber[8];	//Ωª“◊ÀÊª˙ ˝	8	”…«∞÷√ª˙…˙≥…£¨◊˜Œ™√ø± Ωª“◊µƒŒ®“ª±Íº«£¨÷’∂À‘⁄÷Æ∫Û…œÀÕµƒ ˝æ›÷°÷–∂º”¶∏√–Ø¥¯∏√ÀÊª˙ ˝£¨±Ì√˜Œ™Õ¨“ª± Ωª“◊°£Õ¨“ª± Ωª“◊∏√ÀÊª˙ ˝œ‡Õ¨£¨≤ªÕ¨µƒΩª“◊∏√ÀÊª˙ ˝≤ªÕ¨°£
    uint8 CopherDecentGene[8]; /*√‹‘ø∑÷…¢“Ú◊”	8	÷∏≤˙…˙OBU∏˘ƒø¬ºœ¬œµÕ≥Œ¨ª§√‹‘ø∫Õ”¶”√º”√‹√‹‘ø π”√µƒ∂˛º∂∑÷…¢“Ú◊”£ª
                                // 	°∞OBU∫œÕ¨∞Ê±æ°±Œ™°∞0x11°±∫Õ°∞0x12°± ±£¨∏√◊÷∂ŒÃÓ–¥OBU∏¥Œª–≈œ¢£¨º¥2◊÷Ω⁄–æ∆¨≥ß…Ã±Í ∂ + 2◊÷Ω⁄ITSC∑÷≈‰µƒID∫≈ + 4◊÷Ω⁄ESAM–æ∆¨–Ú¡–∫≈°£
                                // 	°∞OBU∫œÕ¨∞Ê±æ°±Œ™°∞0x00°±ªÚ°∞0x10°± ±£¨«“°∞√‹‘ø—°‘Ò±Í ∂°±◊÷∂ŒŒ™°∞0x0A°± ±£¨∏√◊÷∂ŒÃÓ–¥OBU MACµÿ÷∑£®4∏ˆ◊÷Ω⁄£©+ OBU MACµÿ÷∑∞¥Œª»°∑¥£®4∏ˆ◊÷Ω⁄£©°£
                                // 	∆‰À¸∞Ê±æÃÓ–¥OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨–Ú¡–∫≈°±£¨—πÀı ÆΩ¯÷∆±‡¬Î*/
    uint8 EncryptedVehInfo[72]; /*≥µ¡æ–≈œ¢Œƒº˛√‹Œƒ	72	∂¡»°OBU≥µ¡æ–≈œ¢∫Û∑µªÿµƒ√‹Œƒ°£ESAM∑µªÿµƒ√‹Œƒ ˝æ›≥§∂»Œ™8µƒ±∂ ˝£¨ µº ”––ß–≈œ¢Œ™≥µ¡æ–≈œ¢Œƒº˛µƒ«∞59◊÷Ω⁄µƒƒ⁄»›°£72◊÷Ω⁄ ˝æ›÷–∞¸¿®1◊÷Ω⁄≥§∂»,8◊÷Ω⁄–£—È ˝æ›,59◊÷Ω⁄≥µ¡æ–≈œ¢“‘º∞5◊÷Ω⁄ÃÓ≥‰ ˝æ›°£
                                 // 	»Áπ˚OBU∑µªÿ√˜Œƒ–≈œ¢£¨‘Ú÷±Ω”…œÀÕ59◊÷Ω⁄√˜Œƒ≥µ¡æ–≈œ¢£¨≤ª◊„72◊÷Ω⁄∫Û≤π0°£
                                 // 	»ÁŒ™À´œÚ»œ÷§ªÒ»°µƒOBU≥µ¡æ–≈œ¢£¨‘Ú≥˝¡À…œÀÕ≥µ¡æ√˜Œƒ–≈œ¢Õ‚£¨ªπ–Ë…œÀÕ8◊÷Ω⁄–≈œ¢º¯±¬Î£¨≤ª◊„72◊÷Ω⁄∫Û≤π0°£*/
    uint8 RandomNumber[4]; /*ÀÊª˙ ˝	4		‘⁄OBUµƒ∏˘ƒø¬ºœ¬≤˙…˙£¨”√”⁄–¥OBUœµÕ≥–≈œ¢Œƒº˛ ±º∆À„MAC°£
                            //	µ±∏√◊÷∂ŒŒ™»´0 ±£¨∑µªÿµƒ°∞–¥œµÕ≥–≈œ¢”¶¥÷°°±≤ª∑µªÿ”––ßµƒ°∞–¥œµÕ≥–≈œ¢÷∏¡Ó°±°£*/
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏÷’∂À ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    //	uint8 SecretSelctFlag;				/*√‹‘ø—°‘Ò±Í ∂  1‘⁄°∞OBU∫œÕ¨∞Ê±æ°±Œ™°∞0x00°±∫Õ°∞0x10°± ±£¨∏√◊÷∂Œ”––ß£¨
    //			//÷∏º∆À„–¥OBUœµÕ≥–≈œ¢ ± π”√µƒ√‹‘ø£¨∆‰ π”√µƒ√‹‘ø±Í ∂ø…≤Œøº6.9.3Ω⁄°£
    //			//∏√◊÷∂ŒŒ™°∞0x0A°± ±£¨±Ì æ π”√°∞”¶”√Œ¨ª§√‹‘ø°±º∆À„–¥OBUœµÕ≥–≈œ¢∞≤»´±®Œƒ°£
    //			//∏√◊÷∂ŒŒ™ƒ¨»œ÷µ°∞0xA5°± ±£¨±Ì æ π”√ƒ¨»œµƒ°∞œµÕ≥Œ¨ª§√‹‘ø°±º∆À„–¥OBUœµÕ≥–≈œ¢∞≤»´±®Œƒ°£
    //			 * ◊¢£∫æ€¿˚µƒ¿œ∞ÊOBU  π”√0x0A£¨Ω“Áµƒ¿œ∞ÊOBU  π”√0x08 */
    uint8 ExtendInformation[12];		//¿©’π–≈œ¢	11	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_WRITE_SYSINFO_RQ;

//–¥œµÕ≥–≈œ¢”¶¥÷°
typedef struct _FRAME_WRITE_SYSINFO_RS {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x18£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBU–¥œµÕ≥–≈œ¢”¶¥÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x80£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;				//”¶¥∑µªÿ¬Î	1	0x00±Ì æ°∞÷∏¡Óø…”√”⁄–¥OBUŒƒº˛°±£¨∆‰À¸÷µº˚6.19Ω⁄°£
    uint8 FillByte;						//ÃÓ≥‰◊÷Ω⁄	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£;
    uint8 TerminalTradeNO[4];	//÷’∂ÀΩª“◊–Ú∫≈	4	”Î÷’∂À…œÀÕµƒ°∞OBU–¥œµÕ≥–≈œ¢«Î«Û÷°°±÷–µƒ°∞÷’∂ÀΩª“◊–Ú∫≈°±œ‡Õ¨°£
    uint8 WriteSysInfoCommand[18];//–¥œµÕ≥–≈œ¢÷∏¡Ó	18		 π”√ESAMµƒ°∞UpdateBinary°±÷∏¡Ó“‘∆´“∆∏≤∏«µƒ∑Ω Ω£¨ π”√√˜Œƒº”MAC∏¸–¬OBUœµÕ≥–≈œ¢Œƒº˛÷–µƒ°∞∫œÕ¨«© »’∆⁄°±°¢°∞∫œÕ¨π˝∆⁄»’∆⁄°±°¢°∞≤–∂◊¥Ã¨°±£¨”…«∞÷√ª˙Ã·π©°£æﬂÃÂ÷∏¡Ó≤Œøº°∂ ’∑—π´¬∑¡™Õ¯µÁ◊”≤ªÕ£≥µ ’∑—ºº ı“™«Û°∑÷–OBE-SAMµƒœ‡πÿπÊ∂®°£÷ª”–°∞”¶¥∑µªÿ¬Î°±∑µªÿ°∞0x00°± ±∏√◊÷∂Œ≤≈”––ß£¨∑Ò‘Ú∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£
    //	µ±°∞–¥œµÕ≥–≈œ¢«Î«Û÷°°±µƒÀÊª˙ ˝◊÷∂ŒŒ™»´0 ±£¨∏√◊÷∂Œ≤ª∑µªÿ–¥–≈œ¢µƒCOS÷∏¡Ó£¨“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£
    uint8 FillBytes[2];					//ÃÓ≥‰◊÷Ω⁄	2	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 DecryptionVehInfo[59];//≥µ¡æ–≈œ¢Œƒº˛√˜Œƒ	59	 «∂‘÷’∂À…œÀÕµƒ≥µ¡æ–≈œ¢√‹ŒƒΩ‚Œˆ∫Ûµƒ≥µ¡æ–≈œ¢µƒ√˜Œƒƒ⁄»›£¨ø…”√”⁄÷’∂À’π æ°£ƒ⁄»›Àµ√˜º∞±‡¬Î∑Ω Ω≤Œøº°∂ ’∑—π´¬∑¡™Õ¯µÁ◊”≤ªÕ£≥µ ’∑—ºº ı“™«Û°∑÷–±Ì4-4µƒƒ⁄»›£¨”ÎOBUƒ⁄µƒ≥µ¡æ–≈œ¢Œƒº˛ƒ⁄»›∂‘”¶°£÷ª”–°∞”¶¥∑µªÿ¬Î°±∑µªÿ°∞0x00°±∫Õ°∞0x03°± ±∏√◊÷∂Œ≤≈”––ß£¨∑Ò‘Ú∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£√˜º∞±‡¬Î∑Ω Ω≤Œøº°∂ ’∑—π´¬∑¡™Õ¯µÁ◊”≤ªÕ£≥µ ’∑—ºº ı“™«Û°∑÷–±Ì4-4µƒƒ⁄»›£¨”ÎOBUƒ⁄µƒ≥µ¡æ–≈œ¢Œƒº˛ƒ⁄»›∂‘”¶°£÷ª”–°∞”¶¥∑µªÿ¬Î°±∑µªÿ°∞0x00°± ±∏√◊÷∂Œ≤≈”––ß£¨∑Ò‘Ú∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£
    uint8 FillByte1;					//ÃÓ≥‰◊÷Ω⁄;	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[16];		//¿©’π–≈œ¢	16	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_WRITE_SYSINFO_RS;

//œµÕ≥–≈œ¢±»∂‘«Î«Û÷°
typedef struct _FRAME_SYSINFO_CHECK_RQ {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x19£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBUœµÕ≥–≈œ¢±»∂‘«Î«Û÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x50£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 TerminalNO[6];	//÷’∂Àª˙±‡∫≈	6	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «÷’∂ÀµƒŒÔ¿Ì¥Æ∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨∂˛Ω¯÷∆◊÷Ω⁄¡˜°£
    uint8 AdminNO[4];//π‹¿Ì‘±±‡∫≈	4	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «π‹¿Ì‘±ø®∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨±‡¬Î∑Ω Ω£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 TerminalTradeNO[4];//÷’∂ÀΩª“◊–Ú∫≈	4	÷’∂ÀΩª“◊»’÷æ÷–µ±«∞µƒ¡˜ÀÆ∫≈£¨”…÷’∂À…˙≥…º∞Œ¨ª§£¨µ±»’≤ªø…÷ÿ∏¥£¨∂‘”⁄Õ¨“ª± Ωª“◊£¨∏˜÷°µƒΩª“◊–Ú∫≈±ÿ–Î“ª÷¬£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 TradeRandomNumber[8];	//Ωª“◊ÀÊª˙ ˝	8	”…«∞÷√ª˙…˙≥…£¨◊˜Œ™√ø± Ωª“◊µƒŒ®“ª±Íº«£¨÷’∂À‘⁄÷Æ∫Û…œÀÕµƒ ˝æ›÷°÷–∂º”¶∏√–Ø¥¯∏√ÀÊª˙ ˝£¨±Ì√˜Œ™Õ¨“ª± Ωª“◊°£Õ¨“ª± Ωª“◊∏√ÀÊª˙ ˝œ‡Õ¨£¨≤ªÕ¨µƒΩª“◊∏√ÀÊª˙ ˝≤ªÕ¨°£
    uint8 OBUContractSerialNO[8];	//OBU ∫œÕ¨–Ú¡–∫≈	8	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨–Ú¡–∫≈°±£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 OBUContractSignedDate[4];	//OBU ∫œÕ¨«© »’∆⁄	4	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨«© »’∆⁄°±£¨∏Ò ΩŒ™°∞yyyyMMdd°±£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 OBUContractExpiredDate[4];//OBU ∫œÕ¨π˝∆⁄»’∆⁄	4	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨π˝∆⁄»’∆⁄°±£¨∏Ò ΩŒ™°∞yyyyMMdd°±£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 DisassembleState;				//≤–∂◊¥Ã¨	1	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞≤–∂◊¥Ã¨°±°£
    uint8 FillBytes[3];					//ÃÓ≥‰◊÷Ω⁄	3	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏÷’∂À ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[12];		//¿©’π–≈œ¢	12	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_SYSINFO_CHECK_RQ;

//œµÕ≥–≈œ¢±»∂‘”¶¥÷°
typedef struct _FRAME_SYSINFO_CHECK_RS {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x1A£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBUœµÕ≥–≈œ¢±»∂‘”¶¥÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x30£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;					//”¶¥∑µªÿ¬Î	1	0x00±Ì æ°∞±»∂‘≥…π¶°±£¨∆‰À¸÷µº˚6.19Ω⁄°£
    uint8 FillByte;						//ÃÓ≥‰◊÷Ω⁄	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TerminalTradeNO[4];	//÷’∂ÀΩª“◊–Ú∫≈	4	”Î÷’∂À…œÀÕµƒ°∞OBU–¥œµÕ≥–≈œ¢«Î«Û÷°°±÷–µƒ°∞÷’∂ÀΩª“◊–Ú∫≈°±œ‡Õ¨°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[16];		//¿©’π–≈œ¢	16	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_SYSINFO_CHECK_RS;

//∫ÛÃ®–≈œ¢≤È—Ø«Î«Û÷°
typedef struct _FRAME_QUERY_BACKSTAGEINFO_RQ {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x1B£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞∫ÛÃ®–≈œ¢≤È—Ø«Î«Û÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x40£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 TerminalNO[6];	//÷’∂Àª˙±‡∫≈	6	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «÷’∂ÀµƒŒÔ¿Ì¥Æ∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨∂˛Ω¯÷∆◊÷Ω⁄¡˜°£
    uint8 AdminNO[4];//π‹¿Ì‘±±‡∫≈	4	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «π‹¿Ì‘±ø®∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨±‡¬Î∑Ω Ω£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 TerminalTradeNO[4];//÷’∂ÀΩª“◊–Ú∫≈	4	÷’∂ÀΩª“◊»’÷æ÷–µ±«∞µƒ¡˜ÀÆ∫≈£¨”…÷’∂À…˙≥…º∞Œ¨ª§£¨µ±»’≤ªø…÷ÿ∏¥£¨∂‘”⁄Õ¨“ª± Ωª“◊£¨∏˜÷°µƒΩª“◊–Ú∫≈±ÿ–Î“ª÷¬£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 FillBytes[3];					//ÃÓ≥‰◊÷Ω⁄	3	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 OBUContractVersion;		//OBU ∫œÕ¨∞Ê±æ	1	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨∞Ê±æ°±£¨’˚ ˝ ˝÷µ–Õ°£
    uint8 OBUContractSerialNO[8];	//OBU ∫œÕ¨–Ú¡–∫≈	8	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨–Ú¡–∫≈°±£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 GBCPUCardID[8];//π˙±ÍCPUø®–Ú¡–∫≈	8	÷∏≤Â»ÎOBUƒ⁄≤øµƒπ˙±ÍCPUø®µƒ8◊÷Ω⁄ø®∫≈£¨»ÁŒ¥≤Â»Îø®∆¨‘Ú“‘°∞0xFF°±ÃÓ≥‰£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏÷’∂À ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[4];			//¿©’π–≈œ¢	4	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_QUERY_BACKSTAGEINFO_RQ;

//∫ÛÃ®–≈œ¢≤È—Ø”¶¥÷°
typedef struct _FRAME_QUERY_BACKSTAGEINFO_RS {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈	1	0x1C£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞∫ÛÃ®–≈œ¢≤È—Ø”¶¥÷°°±°£
    uint8 FrameLen;					//÷°≥§∂»	1	0x70£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;	//”¶¥∑µªÿ¬Î	1	0x00±Ì æ°∞∫ÛÃ®–≈œ¢≤È—Ø≥…π¶°±£¨∆‰À¸÷µº˚6.13Ω⁄°£ø®∆¨◊¥Ã¨£¨OBU◊¥Ã¨µ»–≈œ¢“≤Õ®π˝∏√◊÷∂Œ∑µªÿ°£
    uint8 FillByte;					//ÃÓ≥‰◊÷Ω⁄	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TerminalTradeNO[4];	//÷’∂ÀΩª“◊–Ú∫≈	4	”Î÷’∂À…œÀÕµƒ°∞∫ÛÃ®–≈œ¢≤È—Ø«Î«Û÷°°±÷–µƒ°∞÷’∂ÀΩª“◊–Ú∫≈°±œ‡Õ¨°£
    uint8 OBUOriginDate[4];	//OBU ∆ º»’∆⁄	4	÷∏ ˝æ›ø‚÷–º«¬ºµƒOBU∫œÕ¨«© »’∆⁄£¨”ÎOBUƒ⁄¥Ê∑≈»’∆⁄µƒ∏Ò Ωœ‡Õ¨£¨—πÀı ÆΩ¯÷∆±Ì æ°£÷ª”–°∞”¶¥∑µªÿ¬Î°±∑µªÿ°∞0x00°± ±∏√◊÷∂Œ≤≈”––ß£¨∑Ò‘Ú∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£
    uint8 OBUExpiredDate[4];//OBU Ωÿ÷π»’∆⁄	4	÷µ ˝æ›ø‚÷–º«¬ºµƒOBU∫œÕ¨π˝∆⁄»’∆⁄£¨”ÎOBUƒ⁄¥Ê∑≈»’∆⁄µƒ∏Ò Ωœ‡Õ¨£¨—πÀı ÆΩ¯÷∆±Ì æ°£÷ª”–°∞”¶¥∑µªÿ¬Î°±∑µªÿ°∞0x00°± ±∏√◊÷∂Œ≤≈”––ß£¨∑Ò‘Ú∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£
    uint8 DecryptionVehInfo[59];//≥µ¡æ–≈œ¢Œƒº˛√˜Œƒ	59	ƒ⁄»›Àµ√˜º∞±‡¬Î∑Ω Ω≤Œøº°∂ ’∑—π´¬∑¡™Õ¯µÁ◊”≤ªÕ£≥µ ’∑—ºº ı“™«Û°∑÷–±Ì4-4µƒƒ⁄»›£¨”ÎOBUƒ⁄µƒ≥µ¡æ–≈œ¢Œƒº˛ƒ⁄»›∂‘”¶°£÷ª”–°∞”¶¥∑µªÿ¬Î°±∑µªÿ°∞0x00°± ±∏√◊÷∂Œ≤≈”––ß£¨∑Ò‘Ú∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£
    uint8 FillByte1;				//ÃÓ≥‰◊÷Ω⁄	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];			//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[12];	//¿©’π–≈œ¢	12	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_QUERY_BACKSTAGEINFO_RS;

//Ωª“◊–≈œ¢…œ¥´«Î«Û÷°
typedef struct _FRAME_UPLOAD_TRADEINFO_RQ {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x1D£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞Ωª“◊–≈œ¢…œ¥´÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0xE0£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 OnLineDenoteByte; /*¡™ª˙÷∏ æ◊÷Ω⁄	1		»Áπ˚ «“‘°∞0xA5°±ÃÓ≥‰£¨±Ì æ…œ¥´µƒ «ƒ¨»œµƒ¡™ª˙Ωª“◊º«¬º,«“”√÷∏Œ∆¬Î◊ˆ±®Œƒ—È÷§£ª
                             »Áπ˚Œ™0£¨±Ì æ…œ¥´µƒ «Õ—ª˙Ωª“◊º«¬º,«“”√÷∏Œ∆¬Î◊ˆ±®Œƒ—È÷§£ª
                             »Áπ˚Œ™1£¨±Ì æ…œ¥´µƒ «Õ—ª˙Ωª“◊º«¬º,«“”√ ˝◊÷«©√˚◊ˆ±®Œƒ—È÷§°£*/
    uint8 OBUContractVersion;		//OBU ∫œÕ¨∞Ê±æ	1	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨∞Ê±æ°±£¨’˚ ˝ ˝÷µ–Õ°£
    uint8 TerminalPhysicsID[40];//÷’∂ÀŒÔ¿Ì¥Æ∫≈	40	≥ß…ÃÃ·π©µƒ”≤º˛Õ≥“ª±‡∫≈(∫Û≤π°Ø\x0°Ø)£¨≤ª◊„40◊÷Ω⁄∫Û≤π°∞0x00°±°£ASCII±‡¬Î°£
    //Õ—ª˙Œ¥∏≥÷µ waiting by wll 2013-7-8 10:20:12
    uint8 AdminCardSerialID[8];	//π‹¿Ì‘±ø®–Ú¡–∫≈	8	÷∏≤Ÿ◊˜“µŒÒ ± π”√µƒπ‹¿Ì‘±ø®µƒ8◊÷Ω⁄ƒ⁄≤ø±‡∫≈£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 TerminalTradeNO[4];	//÷’∂ÀΩª“◊–Ú∫≈	4	÷∏Ωª“◊ ±÷’∂À∏¯∏√± Ωª“◊∑÷≈‰µƒ¡˜ÀÆ∫≈£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ°£
    uint8 OBUContractSerialNO[8];	//OBU ∫œÕ¨–Ú¡–∫≈	8	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨–Ú¡–∫≈°±£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 GBCPUCardID[8]; /*π˙±ÍCPUø®–Ú¡–∫≈	8		÷∏≤Â»ÎOBUƒ⁄≤øµƒπ˙±ÍCPUø®µƒ8◊÷Ω⁄ø®∫≈°£
                           »ÁŒ¥≤Â»Îø®∆¨‘Ú“‘°∞0xFF°±ÃÓ≥‰£¨—πÀı ÆΩ¯÷∆±‡¬Î°£*/
    uint8 WriteVehInfoRandom[4]; /*–¥≥µ¡æ–≈œ¢ÀÊª˙ ˝	4		‘⁄OBUµƒDF01ƒø¬ºœ¬ π”√Get Challenge÷∏¡ÓªÒµ√£¨”√”⁄–¥OBU≥µ¡æ–≈œ¢Œƒº˛ ±º∆À„MAC°£
                                  »Áπ˚Ωª“◊¡˜≥Ã÷–Œ¥∏¸–¬≥µ¡æ–≈œ¢£¨∏√◊÷∂Œ“‘»´0Ω¯––ÃÓ≥‰°£*/
    uint8 WriteVehInfoCommand[68]; /*–¥≥µ¡æ–≈œ¢÷∏¡Ó	68		 π”√ESAMµƒ°∞UpdateBinary°±÷∏¡Ó“‘∆´“∆∏≤∏«µƒ∑Ω Ω£¨ π”√√˜Œƒº”MAC∏¸–¬OBU≥µ¡æ–≈œ¢Œƒº˛µƒ«∞59∏ˆ◊÷Ω⁄°£
                                    »Áπ˚÷∏¡Ó≥§∂»≤ª◊„68◊÷Ω⁄£¨‘Ú∫Û≤π°∞0x00°±°£
                                    »Áπ˚Ωª“◊¡˜≥Ã÷–Œ¥∏¸–¬≥µ¡æ–≈œ¢£¨∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£*/
    uint8 CopherDecentGene[8]; /*√ÿ‘ø∑÷…¢“Ú◊”	8		÷∏≤˙…˙OBU∏˘ƒø¬ºœ¬œµÕ≥Œ¨ª§√‹‘ø π”√µƒ∂˛º∂∑÷…¢“Ú◊”£ª
                                °∞OBU∫œÕ¨∞Ê±æ°±Œ™°∞0x11°±∫Õ°∞0x12°± ±£¨∏√◊÷∂ŒÃÓ–¥OBU∏¥Œª–≈œ¢£¨º¥2◊÷Ω⁄–æ∆¨≥ß…Ã±Í ∂ + 2◊÷Ω⁄ITSC∑÷≈‰µƒID∫≈ + 4◊÷Ω⁄ESAM–æ∆¨–Ú¡–∫≈°£
                                °∞OBU∫œÕ¨∞Ê±æ°±Œ™°∞0x00°±∫Õ°∞0x10°± ±£¨∏√◊÷∂ŒÃÓ–¥OBUµƒMACµÿ÷∑º∞µÿ÷∑»°∑¥∫Ûµƒ8◊÷Ω⁄–≈œ¢°£
                                ∆‰À¸∞Ê±æ∏√◊÷∂ŒÃÓ–¥OBU∫œÕ¨–Ú¡–∫≈°£*/
    uint8 WriteSysInfoRandom[4]; /*–¥œµÕ≥–≈œ¢ÀÊª˙ ˝	4	‘⁄OBUµƒ∏˘ƒø¬ºœ¬≤˙…˙£¨”√”⁄–¥OBUœµÕ≥–≈œ¢Œƒº˛ ±º∆À„MAC°£
                                  »Áπ˚Ωª“◊¡˜≥Ã÷–Œ¥∏¸–¬œµÕ≥–≈œ¢£¨∏√◊÷∂Œ“‘»´0Ω¯––ÃÓ≥‰°£*/
    uint8 WriteSysInfoCommand[20]; /*–¥œµÕ≥–≈œ¢÷∏¡Ó	20		 π”√ESAMµƒ°∞UpdateBinary°±÷∏¡Ó“‘∆´“∆∏≤∏«µƒ∑Ω Ω£¨ π”√√˜Œƒº”MAC∏¸–¬OBUœµÕ≥–≈œ¢Œƒº˛÷–µƒ°∞∫œÕ¨«© »’∆⁄°±°¢°∞∫œÕ¨π˝∆⁄»’∆⁄°±°¢°∞≤–∂◊¥Ã¨°±»˝∏ˆ◊÷∂Œ°£
                                    »Áπ˚÷∏¡Ó≥§∂»≤ª◊„20◊÷Ω⁄£¨‘Ú∫Û≤π°∞0x00°±°£
                                    »Áπ˚Ωª“◊¡˜≥Ã÷–Œ¥∏¸–¬œµÕ≥–≈œ¢£¨∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£*/
    uint8 TradeTimeStamp[8];//Ωª“◊ ±º‰	8	÷∏÷’∂Àª˙º«¬ºµƒΩª“◊∑¢…˙µƒ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 TimeStamp[8]; /* ±º‰¥¡	8	÷∏…œ¥´Ωª“◊–≈œ¢ ±µƒ÷’∂À ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
                         ‘⁄º∆À„∏√±®Œƒµƒ ˝◊÷«©√˚ ±,∏√◊÷∂Œƒ⁄»›”Î°∞Ωª“◊ ±º‰°±±£≥÷“ª÷¬£ª
                         …œ¥´Ωª“◊ ±∏√◊÷∂ŒÃÓ–¥Ωª“◊…œ¥´µƒ ±º‰µ„°£*/
    uint8 FingerMark[16];		//÷∏Œ∆¬Î/ ˝◊÷«©√˚	16	…˙≥…À„∑®º˚3.3.2Ω⁄£®¡™ª˙Ωª“◊£©ªÚµ⁄5.4.8Ω⁄£®Õ—ª˙Ωª“◊£©°£
    uint8 ExtendInformation[16];		//¿©’π–≈œ¢	16	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_UPLOAD_TRADEINFO_RQ;

//Ωª“◊–≈œ¢…œ¥´”¶¥÷°
typedef struct _FRAME_UPLOAD_TRADEINFO_RS {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈	1	0x1E£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞Ωª“◊–≈œ¢…œ¥´”¶¥÷°°±°£
    uint8 FrameLen;					//÷°≥§∂»	1	0x30£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;				//”¶¥∑µªÿ¬Î	1	0x00±Ì æ°∞…œ¥´≥…π¶°±£¨∆‰À¸÷µº˚6.19Ω⁄°£
    uint8 FillByte;					//ÃÓ≥‰◊÷Ω⁄	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TerminalTradeNO[4];		//÷’∂ÀΩª“◊–Ú∫≈	4	”Î÷’∂À…œÀÕµƒ°∞Ωª“◊–≈œ¢…œ¥´÷°°±÷–µƒ°∞÷’∂ÀΩª“◊–Ú∫≈°±œ‡Õ¨°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];			//÷∏Œ∆¬Î	16	 π”√∏√÷’∂Àµ±«∞”––ßµƒª·ª∞√‹‘øº∆À„÷∏Œ∆¬Î£¨…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[16];	//¿©’π–≈œ¢	16	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_UPLOAD_TRADEINFO_RS;

//◊¢œ˙»œ÷§–≈œ¢«Î«Û÷°
typedef struct _FRAME_LOGOUT_VALIDATE_RQ {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x1F£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞◊¢œ˙»œ÷§–≈œ¢«Î«Û÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x40£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 TerminalNO[6];	//÷’∂Àª˙±‡∫≈	6	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «÷’∂ÀµƒŒÔ¿Ì¥Æ∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨∂˛Ω¯÷∆◊÷Ω⁄¡˜°£
    uint8 AdminNO[4];//π‹¿Ì‘±±‡∫≈	4	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «π‹¿Ì‘±ø®∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨±‡¬Î∑Ω Ω£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 TerminalTradeNO[4];//÷’∂ÀΩª“◊–Ú∫≈	4	÷∏÷’∂Àº«¬ºµƒ◊Ó∫Û“ªÃıΩª“◊º«¬ºµƒ÷’∂Àª˙Ωª“◊–Ú∫≈£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    //»Áπ˚√ª”–≤˙…˙Ωª“◊º«¬º‘ÚÃÓ–¥0°£
    uint8 TradeTimeStamp[8];//Ωª“◊ ±º‰	8	÷∏÷’∂Àº«¬ºµƒ◊Ó∫Û“ª± Ωª“◊º«¬ºµƒ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    //»Áπ˚√ª”–≤˙…˙Ωª“◊º«¬º‘Ú”¶∏√∏Ê÷™«∞÷√ª˙ «ƒƒ“ª∏ˆΩª“◊»’√ª”–≤˙…˙Ωª“◊º«¬º°£‘ÚΩª“◊ ±º‰ÕÀªØŒ™Ωª“◊»’,°∞ ±∑÷√Î°±≤ø∑÷ø…ÃÓ–¥»´0°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏…œ¥´Ωª“◊–≈œ¢ ±µƒ÷’∂À ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[16];		//¿©’π–≈œ¢	16	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_LOGOUT_VALIDATE_RQ;

//◊¢œ˙»œ÷§–≈œ¢”¶¥÷°
typedef struct _FRAME_LOGOUT_VALIDATE_RS {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈		-0x20£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™"◊¢œ˙»œ÷§–≈œ¢”¶¥÷°"°£
    uint8 FrameLen;					//÷°≥§∂»			-0x30£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;				//”¶¥∑µªÿ¬Î		-0x00±Ì æ"◊¢œ˙≥…π¶"£¨∆‰À¸÷µº˚6.19Ω⁄°£
    uint8 FillByte;					//ÃÓ≥‰◊÷Ω⁄			-“‘"0xA5"◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TerminalTradeNO[4];	//÷’∂ÀΩª“◊–Ú∫≈		-”Î÷’∂À…œÀÕµƒ"◊¢œ˙»œ÷§–≈œ¢«Î«Û÷°"÷–µƒ"÷’∂ÀΩª“◊–Ú∫≈"œ‡Õ¨°£
    uint8 UNUpLoadTradeNO[4];//Œ¥…œ¥´Ωª“◊–Ú∫≈	-÷∏‘⁄Ωª“◊»’∆⁄ƒ⁄Œ¥…œ¥´Ωª“◊–≈œ¢µƒ◊Ó–°µƒ÷’∂ÀΩª“◊–Ú∫≈£¨»Áπ˚“—æ≠»´≤ø…œ¥´‘ÚÃÓ–¥0°£
    uint8 TimeStamp[8];	// ±º‰¥¡			-Œ™√˜»∑∏Ê÷™÷’∂À°∞Œ¥…œ¥´µƒΩª“◊–Ú∫≈°±¡• ÙµƒΩª“◊»’∆⁄£¨∏√◊÷∂Œ”Î°∞◊¢œ˙»œ÷§–≈œ¢«Î«Û÷°°±÷–µƒ°∞ ±º‰¥¡°±◊÷∂Œœ‡Õ¨°£
    uint8 FingerMark[16];			//÷∏Œ∆¬Î			-…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[12];	//¿©’π–≈œ¢		-±∏”√£¨‘› ±“‘"0xA5"◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_LOGOUT_VALIDATE_RS;

//»¶¥Ê≥ı ºªØ«Î«Û÷°
typedef struct _FRAME_QUANCUN_INIT_RQ {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈
    uint8 FrameLen;						//÷°≥§∂»
    uint8 UserName[22];					//”√ªß√˚;
    uint8 FillBytes[4];					//ÃÓ≥‰◊÷Ω⁄;
    uint8 Quancun_Money[4];				//»¶¥ÊΩ∂Ó
    uint8 TimeStamp[8];					// ±º‰¥¡
    uint8 FingerMark[16];				//÷∏Œ∆¬Î
    uint8 ExtendInformation[8];			//¿©’π–≈œ¢
} FRAME_QUANCUN_INIT_RQ;

//»¶¥Ê≥ı ºªØ”¶¥÷°
typedef struct _FRAME_QUANCUN_INIT_RS {
    uint8 FrameType;						//÷°¿‡–Õ±‡∫≈
    uint8 FrameLen;							//÷°≥§∂»
    uint8 ResponseCode;						//”¶¥∑µªÿ¬Î
    uint8 FillBytes[5];						//ÃÓ≥‰◊÷Ω⁄;
    uint8 TerminalNO[8];					//÷’∂Àª˙±‡∫≈
    uint8 TimeStamp[8];						// ±º‰¥¡
    uint8 FingerMark[16];					//÷∏Œ∆¬Î
    uint8 ExtendInformation[12];			//¿©’π–≈œ¢
} FRAME_QUANCUN_INIT_RS;

//»¶¥Ê«Î«Û÷°
typedef struct _FRAME_QUANCUN_RQ {
    uint8 FrameType;					    //÷°¿‡–Õ±‡∫≈	1	0x23£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™"»¶¥Ê«Î«Û÷°"°£
    uint8 FrameLen;							//÷°≥§∂»	1	0x28£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 QuancunTradeNo[2];		//¡™ª˙Ωª“◊–Ú∫≈	2	CPUø®»¶¥Ê≥ı ºªØ”¶¥÷°÷–µƒ ˝æ›÷–ªÒ»°£¨”√”⁄º∆À„π˝≥Ã√ÿ‘ø
    uint8 BogusRandNumber[4];			//Œ±ÀÊª˙ ˝	4	CPUø®»¶¥Ê≥ı ºªØ”¶¥÷°÷–µƒ ˝æ›÷–ªÒ»°£¨”√”⁄º∆À„π˝≥Ã√ÿ‘ø
    uint8 OneDispersed[8];				//“ªº∂∑÷…¢“Ú◊”	8	‘⁄CPUø®÷–∂¡»°µƒ“ªº∂∑÷…¢“Ú◊”£¨”√”⁄º∆À„π˝≥Ã√ÿ‘ø
    uint8 TwoDispersed[8];				//∂˛º∂∑÷…¢“Ú◊”	8	‘⁄CPUø®÷–∂¡»°µƒ∂˛º∂∑÷…¢“Ú◊”£¨”√”⁄º∆À„π˝≥Ã√ÿ‘ø
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏Ωª“◊ ±º‰£¨∏Ò ΩŒ™"ƒÍ‘¬»’÷‹ ±∑÷√Î"£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];					//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[8];				//¿©’π–≈œ¢	8	±∏”√£¨‘› ±“‘"0xA5"◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_QUANCUN_RQ;

//»¶¥Ê”¶¥÷°
typedef struct _FRAME_QUANCUN_RS {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈
    uint8 FrameLen;						//÷°≥§∂»
    uint8 ResponseCode;					//”¶¥∑µªÿ¬Î
    uint8 FillBytes[5];					//ÃÓ≥‰◊÷Ω⁄;
    uint8 PassKey[8];				//π˝≥Ã√ÿ‘ø 8	∫ÛÃ®Õ®π˝»¶¥ÊPSAMø®º∆À„ªÒ»°
    uint8 TimeStamp[8];					// ±º‰¥¡
    uint8 FingerMark[16];				//÷∏Œ∆¬Î
    uint8 ExtendInformation[8];			//¿©’π–≈œ¢
} FRAME_QUANCUN_RS;

//≥‰÷µ«Î«Û÷°
typedef struct _FRAME_RECHARGE_RQ {
    uint8 FrameType;					    //÷°¿‡–Õ±‡∫≈	1	0x25£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞ICø®≥‰÷µ«Î«Û÷°°±°£
    uint8 FrameLen;							//÷°≥§∂»	1	0x50£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 Net_UserName[22];				//∫ÛÃ®’À∫≈	22	±Í ∂∫ÛÃ®µƒ’À∫≈£¨◊Ó∂‡22Œª, …Ÿ”⁄22Œªµƒ∫Û√Ê≤π0x00
    uint8 BAND_ICCardNum[20];				//Ω∂Ó¿¥‘¥’À∫≈	20	“¯––ø®∫≈£¨…Ÿ”⁄20Œªµƒ∫Û√Ê≤π0x00
    uint8 ICCard_RechargeMoney[4];		//ICø®≥‰÷µΩ∂Ó	4	≥‰÷µΩ∂Ó£¨4◊÷Ω⁄±Ì æ£¨∏Ò Ω∞¥2.1÷–πÊ∂®µƒ¿¥
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏Ωª“◊ ±º‰£¨∏Ò ΩŒ™"ƒÍ‘¬»’÷‹ ±∑÷√Î"£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];					//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[8];				//¿©’π–≈œ¢	8	±∏”√£¨‘› ±“‘"0xA5"◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_RECHARGE_RQ;

//≥‰÷µ”¶¥÷°
typedef struct _FRAME_RECHARGE_RS {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x26£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞ICø®≥‰÷µ”¶¥÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x28£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;					//”¶¥∑µªÿ¬Î	1	0x00±Ì æ°∞ICø®≥‰÷µ”¶¥≥…π¶°±£¨∆‰À¸÷µº˚6.19Ω⁄°£
    uint8 FillByte;						//ÃÓ≥‰◊÷Ω⁄	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 User_curAccount[4];		//µ±«∞’Àªß”‡∂Ó	4	µ±«∞’Àªß £”‡Ω∂Ó£¨±Ì æ∑Ω Ω π”√2.1÷–µƒπÊ∑∂Ω¯––±Ì æ
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙Ωª“◊ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[8];			//¿©’π–≈œ¢	8	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_RECHARGE_RS;

//”√ªßµ«¬Ω«Î«Û÷°(÷’∂À->«∞÷√ª˙)
typedef struct _FRAME_NETLOGIN_RQ {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x27£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞”√ªßµ«¬º«Î«Û÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x48£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 Net_UserName[22];				//∫ÛÃ®’À∫≈	22	±Í ∂∫ÛÃ®µƒ’À∫≈£¨◊Ó∂‡22Œª, …Ÿ”⁄22Œªµƒ∫Û√Ê≤π0x00
    uint8 Net_UserPsaaword[16];			//∫ÛÃ®√‹¬Î	16	∫ÛÃ®√‹¬Î£¨◊Ó∂‘16Œª£¨…Ÿ”⁄16Œªµƒ∫Û√Ê≤π0x00
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏÷’∂À»¶¥Ê ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[8];			//¿©’π–≈œ¢	8	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_NETLOGIN_RQ;

//”√ªßµ«¬Ω”¶¥÷°(«∞÷√ª˙->÷’∂À)
typedef struct _FRAME_NETLOGIN_RS {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x28£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞”√ªßµ«¬º”¶¥÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x28£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;					//”¶¥∑µªÿ¬Î	1	0x00±Ì æ°∞”√ªßµ«¬Ω”¶¥≥…π¶°±£¨∆‰À¸÷µº˚6.19Ω⁄°£
    uint8 FillBytes[5];					//ÃÓ≥‰◊÷∂Œ	5	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙Ωª“◊ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[8];			//¿©’π–≈œ¢	8	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_NETLOGIN_RS;

//ºÏ∂®º§ªÓ∫œ∑®–‘«Î«Û÷°(÷’∂À->«∞÷√ª˙)
typedef struct _FRAME_CHECKISSUEVALIDITY_RQ {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x29£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞ºÏ∂®º§ªÓ∫œ∑®–‘«Î«Û÷°°±°£
    uint8 FrameLen;						//÷°≥§∂» 1	0xFC£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 FrameSerialNum[2];			//÷°–Ú∫≈	2	µ⁄“ª÷°Œ™0x0000£ª
    //µ±÷°–Ú∫≈Œ™0xFFFF ±£¨Œ™◊Ó∫Û“ª÷°°£
    uint8 FrameFileContent[0xD8];//÷°ƒ⁄»›	0xD8	µ±÷°–Ú∫≈Œ™0x0000 ±£¨«∞0x10∏ˆ◊÷Ω⁄Œ™µ⁄“ª—È÷§–≈œ¢£¨ £”‡Œ™µ⁄∂˛—È÷§–≈œ¢£®º¥Õº∆¨–≈œ¢£©
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏÷’∂À»¶¥Ê ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[8];			//¿©’π–≈œ¢	8	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_CHECKISSUEVALIDITY_RQ;

//”√ªßµ«¬Ω”¶¥÷°(«∞÷√ª˙->÷’∂À)
typedef struct _FRAME_CHECKISSUEVALIDITY_RS {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x2A£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞ºÏ∂®º§ªÓ∫œ∑®–‘”¶¥÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x28£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;				//”¶¥∑µªÿ¬Î	1	0x00±Ì æ°∞ºÏ∂®º§ªÓ∫œ∑®–‘”¶¥≥…π¶°±£¨∆‰À¸÷µº˚6.19Ω⁄°£
    uint8 ValdityResponseCode;	//∫œ∑®–‘”¶¥∑µªÿ¬Î	1	÷°–Ú∫≈Œ™0xFFFF ±£¨0£∫ºÏ∂®≥…π¶£ª1£∫ºÏ∂® ß∞‹£¨∆‰À˚£∫ ß∞‹
    //∆‰À˚÷°–Ú∫≈£∫ÃÓ≥‰0xA5
    uint8 FrameSerialNum[2];			//÷°–Ú∫≈	2	µ⁄“ª÷°Œ™0x0000£ª
    //µ±÷°–Ú∫≈Œ™0xFFFF ±£¨Œ™◊Ó∫Û“ª÷°°£
    uint8 FillBytes[2];					//ÃÓ≥‰◊÷∂Œ	2	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙Ωª“◊ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[8];			//¿©’π–≈œ¢	8	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FRAME_CHECKISSUEVALIDITY_RS;

//ª˙ø®¡™∫œ»œ÷§∑µªÿΩ·ππÃÂ
typedef struct _Certify_Result {
    uint8 TerminalNO[6];	//÷’∂Àª˙±‡∫≈
    uint8 AdminNO[4];		//π‹¿Ì‘±±‡∫≈
    uint8 TalkKey[20];		//ª·ª∞√ÿ‘ø
    uint8 TimeStamp[20];	//«∞÷√ª˙ ±º‰¥¡
    uint8 UnityTimeOutDuration[2]; /*ª·ª∞√‹‘ø’˚ÃÂ≥¨ ± ±≥§	2	÷∏ª·ª∞√‹‘ø…˙≥…∫Ûµƒ”––ß ±≥§,≥¨π˝∏√ ±≥§ª·ª∞√‹‘ø◊‘∂Ø ß–ß°£
                                    “‘∑÷÷”Œ™µ•Œª£¨’˚ ˝ ˝÷µ–Õ°£*/
    uint8 FreeTimeOutDuration[2]; /*ª·ª∞√‹‘øø’œ–≥¨ ± ±≥§	2	÷∏ª·ª∞√‹‘ø…˙≥…∫Û£¨ ÷≥÷ª˙√ª”–÷¥––Ωª“◊,ø’œ–µƒ ±º‰≥§∂»≥¨π˝∏√ ±≥§ª·ª∞√‹‘ø◊‘∂Ø ß–ß°£
                                   “‘∑÷÷”Œ™µ•Œª£¨’˚ ˝ ˝÷µ–Õ°£*/
    uint8 Reserved[100];	//‘§¡Ù◊÷Ω⁄
} CertifyResult;

//ª˙ø®¡™∫œ»œ÷§∑µªÿΩ·ππÃÂ
typedef struct _FrameQuanCunInitRq {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈
    uint8 FrameLen;						//÷°≥§∂»
    uint8 UserName[22];					//”√ªß√˚;
    uint8 FillBytes[4];					//ÃÓ≥‰◊÷Ω⁄;
    uint8 Quancun_Money[4];				//»¶¥ÊΩ∂Ó
    uint8 TimeStamp[8];					// ±º‰¥¡
    uint8 FingerMark[16];				//÷∏Œ∆¬Î
    uint8 ExtendInformation[8];			//¿©’π–≈œ¢
} FrameQuanCunInitRq;

//ª˙ø®¡™∫œ»œ÷§∑µªÿΩ·ππÃÂ
typedef struct _FrameQuanCunInitRs {
    uint8 FrameType;						//÷°¿‡–Õ±‡∫≈
    uint8 FrameLen;							//÷°≥§∂»
    uint8 ResponseCode;						//”¶¥∑µªÿ¬Î
    uint8 FillBytes[5];						//ÃÓ≥‰◊÷Ω⁄;
    uint8 TerminalNO[8];					//÷’∂Àª˙±‡∫≈
    uint8 TimeStamp[8];						// ±º‰¥¡
    uint8 FingerMark[16];					//÷∏Œ∆¬Î
    uint8 ExtendInformation[12];			//¿©’π–≈œ¢
} FrameQuanCunInitRs;
//»¶¥Ê«Î«Û÷°
typedef struct _FrameQuanCunRq {
    uint8 FrameType;					    //÷°¿‡–Õ±‡∫≈	1	0x23£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™"»¶¥Ê«Î«Û÷°"°£
    uint8 FrameLen;							//÷°≥§∂»	1	0x28£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 QuancunTradeNo[2];		//¡™ª˙Ωª“◊–Ú∫≈	2	CPUø®»¶¥Ê≥ı ºªØ”¶¥÷°÷–µƒ ˝æ›÷–ªÒ»°£¨”√”⁄º∆À„π˝≥Ã√ÿ‘ø
    uint8 BogusRandNumber[4];			//Œ±ÀÊª˙ ˝	4	CPUø®»¶¥Ê≥ı ºªØ”¶¥÷°÷–µƒ ˝æ›÷–ªÒ»°£¨”√”⁄º∆À„π˝≥Ã√ÿ‘ø
    uint8 OneDispersed[8];				//“ªº∂∑÷…¢“Ú◊”	8	‘⁄CPUø®÷–∂¡»°µƒ“ªº∂∑÷…¢“Ú◊”£¨”√”⁄º∆À„π˝≥Ã√ÿ‘ø
    uint8 TwoDispersed[8];				//∂˛º∂∑÷…¢“Ú◊”	8	‘⁄CPUø®÷–∂¡»°µƒ∂˛º∂∑÷…¢“Ú◊”£¨”√”⁄º∆À„π˝≥Ã√ÿ‘ø
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏Ωª“◊ ±º‰£¨∏Ò ΩŒ™"ƒÍ‘¬»’÷‹ ±∑÷√Î"£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];					//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[8];				//¿©’π–≈œ¢	8	±∏”√£¨‘› ±“‘"0xA5"◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FrameQuanCunRq;

typedef struct _CardConsumeRecordList {
    uint8 m_num;				//∑µªÿµƒŒƒº˛ ˝ƒø
    uint8 m_FileContent[50][43];
} CardConsumeRecordList;
//»¶¥Ê”¶¥÷°
typedef struct _FrameQuanCunRs {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈
    uint8 FrameLen;						//÷°≥§∂»
    uint8 ResponseCode;					//”¶¥∑µªÿ¬Î
    uint8 FillBytes[5];					//ÃÓ≥‰◊÷Ω⁄;
    uint8 PassKey[8];				//π˝≥Ã√ÿ‘ø 8	∫ÛÃ®Õ®π˝»¶¥ÊPSAMø®º∆À„ªÒ»°
    uint8 TimeStamp[8];					// ±º‰¥¡
    uint8 FingerMark[16];				//÷∏Œ∆¬Î
    uint8 ExtendInformation[8];			//¿©’π–≈œ¢
} FrameQuanCunRs;

//OBU∂¡≥µ¡æ–≈œ¢«Î«Û÷°(÷’∂À->«∞÷√ª˙)
typedef struct _FrameReadVehInfoRq {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈	1	0x15£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBU∂¡≥µ¡æ–≈œ¢«Î«Û÷°°±°£
    uint8 FrameLen;					//÷°≥§∂»	1	0x40£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»
    uint8 TerminalNO[6];	//÷’∂Àª˙±‡∫≈	6	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «÷’∂ÀµƒŒÔ¿Ì¥Æ∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨∂˛Ω¯÷∆◊÷Ω⁄¡˜°£
    uint8 AdminNO[4];//π‹¿Ì‘±±‡∫≈	4	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «π‹¿Ì‘±ø®∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨±‡¬Î∑Ω Ω£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 TerminalTradeNO[4];//÷’∂ÀΩª“◊–Ú∫≈	4	÷’∂ÀΩª“◊»’÷æ÷–µ±«∞µƒ¡˜ÀÆ∫≈£¨”…÷’∂À…˙≥…º∞Œ¨ª§£¨µ±»’≤ªø…÷ÿ∏¥£¨∂‘”⁄Õ¨“ª± Ωª“◊£¨∏˜÷°µƒΩª“◊–Ú∫≈±ÿ–Î“ª÷¬£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 FillBytes[3];				//ÃÓ≥‰◊÷Ω⁄	3	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 OBUContractVersion;		//OBU ∫œÕ¨∞Ê±æ	1	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨∞Ê±æ°±£¨’˚ ˝ ˝÷µ–Õ°£
    uint8 OBUContractSerialNO[8];	//OBU∫œÕ¨–Ú¡–∫≈	8	÷∏OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨–Ú¡–∫≈°±£¨—πÀı ÆΩ¯÷∆±‡¬Î°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏÷’∂À ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];			//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[12];	//¿©≥‰–≈œ¢	12	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FrameReadVehInfoRq;

//OBU∂¡≥µ¡æ–≈œ¢”¶¥÷°(«∞÷√ª˙->÷’∂À)
typedef struct _FrameReadVehInfoRs {
    uint8 FrameType;				//÷°¿‡–Õ±‡∫≈	1	0x16£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBU∂¡≥µ¡æ–≈œ¢”¶¥÷°°±°£
    uint8 FrameLen;					//÷°≥§∂»	1	0x30£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;				//”¶¥∑µªÿ¬Î	1	0x00±Ì æ°∞≥…π¶£¨ø…∂¡»°≥µ¡æ–≈œ¢°±£¨∆‰À¸÷µº˚6.19Ω⁄°£
    uint8 FillByte;					//ÃÓ≥‰◊÷Ω⁄	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TerminalTradeNO[4];	//÷’∂ÀΩª“◊–Ú∫≈	4	”Î÷’∂À…œÀÕµƒ°∞OBU∂¡≥µ¡æ–≈œ¢”¶¥÷°°±÷–µƒ°∞÷’∂ÀΩª“◊–Ú∫≈°±œ‡Õ¨°£
    uint8 TradeRandomNumber[8];	//Ωª“◊ÀÊª˙ ˝	8	 ««∞÷√ª˙∂‘Ωª“◊µƒŒ®“ª–‘…˙≥…µƒ±Í ∂£¨÷’∂À‘⁄÷Æ∫Û…œÀÕµƒ ˝æ›÷°÷–∂º”¶∏√–Ø¥¯∏√ÀÊª˙ ˝£¨±Ì√˜Œ™Õ¨“ª± Ωª“◊°£Õ¨“ª± Ωª“◊∏√ÀÊª˙ ˝œ‡Õ¨£¨≤ªÕ¨µƒΩª“◊∏√ÀÊª˙ ˝≤ªÕ¨°£Õ¨ ±£¨∏√ÀÊª˙ ˝“≤‘⁄÷’∂À∂¡»°≥µ¡æ–≈œ¢ ±≤Œ”Î π”√£¨”√”⁄…˙≥…≥µ¡æ–≈œ¢µƒ√‹Œƒ°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];			//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[8];		//¿©≥‰–≈œ¢	8	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FrameReadVehInfoRs;

//–¥œµÕ≥–≈œ¢«Î«Û÷°
typedef struct _FrameWriteSysInfoRq {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x17£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBU–¥œµÕ≥–≈œ¢«Î«Û÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x90£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 TerminalNO[6];	//÷’∂Àª˙±‡∫≈	6	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «÷’∂ÀµƒŒÔ¿Ì¥Æ∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨∂˛Ω¯÷∆◊÷Ω⁄¡˜°£
    uint8 AdminNO[4];//π‹¿Ì‘±±‡∫≈	4	π‹¿Ì±‡∫≈£¨”…«∞÷√ª˙Ã·π©£¨ «π‹¿Ì‘±ø®∫≈‘⁄œµÕ≥÷–”≥…‰µƒŒ®“ª±‡∫≈£¨±‡¬Î∑Ω Ω£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 TerminalTradeNO[4];//÷’∂ÀΩª“◊–Ú∫≈	4	÷’∂ÀΩª“◊»’÷æ÷–µ±«∞µƒ¡˜ÀÆ∫≈£¨”…÷’∂À…˙≥…º∞Œ¨ª§£¨µ±»’≤ªø…÷ÿ∏¥£¨∂‘”⁄Õ¨“ª± Ωª“◊£¨∏˜÷°µƒΩª“◊–Ú∫≈±ÿ–Î“ª÷¬£¨±‡¬Î¿‡–ÕŒ™’˚ ˝ ˝÷µ–Õ£¨Õ¯¬Á◊÷Ω⁄–Ú°£
    uint8 TradeRandomNumber[8];	//Ωª“◊ÀÊª˙ ˝	8	”…«∞÷√ª˙…˙≥…£¨◊˜Œ™√ø± Ωª“◊µƒŒ®“ª±Íº«£¨÷’∂À‘⁄÷Æ∫Û…œÀÕµƒ ˝æ›÷°÷–∂º”¶∏√–Ø¥¯∏√ÀÊª˙ ˝£¨±Ì√˜Œ™Õ¨“ª± Ωª“◊°£Õ¨“ª± Ωª“◊∏√ÀÊª˙ ˝œ‡Õ¨£¨≤ªÕ¨µƒΩª“◊∏√ÀÊª˙ ˝≤ªÕ¨°£
    uint8 CopherDecentGene[8]; /*√‹‘ø∑÷…¢“Ú◊”	8	÷∏≤˙…˙OBU∏˘ƒø¬ºœ¬œµÕ≥Œ¨ª§√‹‘ø∫Õ”¶”√º”√‹√‹‘ø π”√µƒ∂˛º∂∑÷…¢“Ú◊”£ª
                                // 	°∞OBU∫œÕ¨∞Ê±æ°±Œ™°∞0x11°±∫Õ°∞0x12°± ±£¨∏√◊÷∂ŒÃÓ–¥OBU∏¥Œª–≈œ¢£¨º¥2◊÷Ω⁄–æ∆¨≥ß…Ã±Í ∂ + 2◊÷Ω⁄ITSC∑÷≈‰µƒID∫≈ + 4◊÷Ω⁄ESAM–æ∆¨–Ú¡–∫≈°£
                                // 	°∞OBU∫œÕ¨∞Ê±æ°±Œ™°∞0x00°±ªÚ°∞0x10°± ±£¨«“°∞√‹‘ø—°‘Ò±Í ∂°±◊÷∂ŒŒ™°∞0x0A°± ±£¨∏√◊÷∂ŒÃÓ–¥OBU MACµÿ÷∑£®4∏ˆ◊÷Ω⁄£©+ OBU MACµÿ÷∑∞¥Œª»°∑¥£®4∏ˆ◊÷Ω⁄£©°£
                                // 	∆‰À¸∞Ê±æÃÓ–¥OBU÷–œµÕ≥–≈œ¢Œƒº˛µƒ°∞∫œÕ¨–Ú¡–∫≈°±£¨—πÀı ÆΩ¯÷∆±‡¬Î*/
    uint8 EncryptedVehInfo[72]; /*≥µ¡æ–≈œ¢Œƒº˛√‹Œƒ	72	∂¡»°OBU≥µ¡æ–≈œ¢∫Û∑µªÿµƒ√‹Œƒ°£ESAM∑µªÿµƒ√‹Œƒ ˝æ›≥§∂»Œ™8µƒ±∂ ˝£¨ µº ”––ß–≈œ¢Œ™≥µ¡æ–≈œ¢Œƒº˛µƒ«∞59◊÷Ω⁄µƒƒ⁄»›°£72◊÷Ω⁄ ˝æ›÷–∞¸¿®1◊÷Ω⁄≥§∂»,8◊÷Ω⁄–£—È ˝æ›,59◊÷Ω⁄≥µ¡æ–≈œ¢“‘º∞5◊÷Ω⁄ÃÓ≥‰ ˝æ›°£
                                 // 	»Áπ˚OBU∑µªÿ√˜Œƒ–≈œ¢£¨‘Ú÷±Ω”…œÀÕ59◊÷Ω⁄√˜Œƒ≥µ¡æ–≈œ¢£¨≤ª◊„72◊÷Ω⁄∫Û≤π0°£
                                 // 	»ÁŒ™À´œÚ»œ÷§ªÒ»°µƒOBU≥µ¡æ–≈œ¢£¨‘Ú≥˝¡À…œÀÕ≥µ¡æ√˜Œƒ–≈œ¢Õ‚£¨ªπ–Ë…œÀÕ8◊÷Ω⁄–≈œ¢º¯±¬Î£¨≤ª◊„72◊÷Ω⁄∫Û≤π0°£*/
    uint8 RandomNumber[4]; /*ÀÊª˙ ˝	4		‘⁄OBUµƒ∏˘ƒø¬ºœ¬≤˙…˙£¨”√”⁄–¥OBUœµÕ≥–≈œ¢Œƒº˛ ±º∆À„MAC°£
                            //	µ±∏√◊÷∂ŒŒ™»´0 ±£¨∑µªÿµƒ°∞–¥œµÕ≥–≈œ¢”¶¥÷°°±≤ª∑µªÿ”––ßµƒ°∞–¥œµÕ≥–≈œ¢÷∏¡Ó°±°£*/
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏÷’∂À ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    //	uint8 SecretSelctFlag;				/*√‹‘ø—°‘Ò±Í ∂  1‘⁄°∞OBU∫œÕ¨∞Ê±æ°±Œ™°∞0x00°±∫Õ°∞0x10°± ±£¨∏√◊÷∂Œ”––ß£¨
    //			//÷∏º∆À„–¥OBUœµÕ≥–≈œ¢ ± π”√µƒ√‹‘ø£¨∆‰ π”√µƒ√‹‘ø±Í ∂ø…≤Œøº6.9.3Ω⁄°£
    //			//∏√◊÷∂ŒŒ™°∞0x0A°± ±£¨±Ì æ π”√°∞”¶”√Œ¨ª§√‹‘ø°±º∆À„–¥OBUœµÕ≥–≈œ¢∞≤»´±®Œƒ°£
    //			//∏√◊÷∂ŒŒ™ƒ¨»œ÷µ°∞0xA5°± ±£¨±Ì æ π”√ƒ¨»œµƒ°∞œµÕ≥Œ¨ª§√‹‘ø°±º∆À„–¥OBUœµÕ≥–≈œ¢∞≤»´±®Œƒ°£
    //			 * ◊¢£∫æ€¿˚µƒ¿œ∞ÊOBU  π”√0x0A£¨Ω“Áµƒ¿œ∞ÊOBU  π”√0x08 */
    uint8 ExtendInformation[12];		//¿©’π–≈œ¢	11	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FrameWriteSysInfoRq;

//–¥œµÕ≥–≈œ¢”¶¥÷°
typedef struct _FrameWriteSysInfoRs {
    uint8 FrameType;					//÷°¿‡–Õ±‡∫≈	1	0x18£¨±Ì æ∏√÷°µƒ¿‡–ÕŒ™°∞OBU–¥œµÕ≥–≈œ¢”¶¥÷°°±°£
    uint8 FrameLen;						//÷°≥§∂»	1	0x80£¨±Ì æ’˚∏ˆ÷°µƒ≥§∂»°£
    uint8 ResponseCode;				//”¶¥∑µªÿ¬Î	1	0x00±Ì æ°∞÷∏¡Óø…”√”⁄–¥OBUŒƒº˛°±£¨∆‰À¸÷µº˚6.19Ω⁄°£
    uint8 FillByte;						//ÃÓ≥‰◊÷Ω⁄	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£;
    uint8 TerminalTradeNO[4];	//÷’∂ÀΩª“◊–Ú∫≈	4	”Î÷’∂À…œÀÕµƒ°∞OBU–¥œµÕ≥–≈œ¢«Î«Û÷°°±÷–µƒ°∞÷’∂ÀΩª“◊–Ú∫≈°±œ‡Õ¨°£
    uint8 WriteSysInfoCommand[18];//–¥œµÕ≥–≈œ¢÷∏¡Ó	18		 π”√ESAMµƒ°∞UpdateBinary°±÷∏¡Ó“‘∆´“∆∏≤∏«µƒ∑Ω Ω£¨ π”√√˜Œƒº”MAC∏¸–¬OBUœµÕ≥–≈œ¢Œƒº˛÷–µƒ°∞∫œÕ¨«© »’∆⁄°±°¢°∞∫œÕ¨π˝∆⁄»’∆⁄°±°¢°∞≤–∂◊¥Ã¨°±£¨”…«∞÷√ª˙Ã·π©°£æﬂÃÂ÷∏¡Ó≤Œøº°∂ ’∑—π´¬∑¡™Õ¯µÁ◊”≤ªÕ£≥µ ’∑—ºº ı“™«Û°∑÷–OBE-SAMµƒœ‡πÿπÊ∂®°£÷ª”–°∞”¶¥∑µªÿ¬Î°±∑µªÿ°∞0x00°± ±∏√◊÷∂Œ≤≈”––ß£¨∑Ò‘Ú∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£
    //	µ±°∞–¥œµÕ≥–≈œ¢«Î«Û÷°°±µƒÀÊª˙ ˝◊÷∂ŒŒ™»´0 ±£¨∏√◊÷∂Œ≤ª∑µªÿ–¥–≈œ¢µƒCOS÷∏¡Ó£¨“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£
    uint8 FillBytes[2];					//ÃÓ≥‰◊÷Ω⁄	2	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 DecryptionVehInfo[59];//≥µ¡æ–≈œ¢Œƒº˛√˜Œƒ	59	 «∂‘÷’∂À…œÀÕµƒ≥µ¡æ–≈œ¢√‹ŒƒΩ‚Œˆ∫Ûµƒ≥µ¡æ–≈œ¢µƒ√˜Œƒƒ⁄»›£¨ø…”√”⁄÷’∂À’π æ°£ƒ⁄»›Àµ√˜º∞±‡¬Î∑Ω Ω≤Œøº°∂ ’∑—π´¬∑¡™Õ¯µÁ◊”≤ªÕ£≥µ ’∑—ºº ı“™«Û°∑÷–±Ì4-4µƒƒ⁄»›£¨”ÎOBUƒ⁄µƒ≥µ¡æ–≈œ¢Œƒº˛ƒ⁄»›∂‘”¶°£÷ª”–°∞”¶¥∑µªÿ¬Î°±∑µªÿ°∞0x00°±∫Õ°∞0x03°± ±∏√◊÷∂Œ≤≈”––ß£¨∑Ò‘Ú∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£√˜º∞±‡¬Î∑Ω Ω≤Œøº°∂ ’∑—π´¬∑¡™Õ¯µÁ◊”≤ªÕ£≥µ ’∑—ºº ı“™«Û°∑÷–±Ì4-4µƒƒ⁄»›£¨”ÎOBUƒ⁄µƒ≥µ¡æ–≈œ¢Œƒº˛ƒ⁄»›∂‘”¶°£÷ª”–°∞”¶¥∑µªÿ¬Î°±∑µªÿ°∞0x00°± ±∏√◊÷∂Œ≤≈”––ß£¨∑Ò‘Ú∏√◊÷∂Œ“‘°∞0xA5°±Ω¯––ÃÓ≥‰°£
    uint8 FillByte1;					//ÃÓ≥‰◊÷Ω⁄;	1	“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
    uint8 TimeStamp[8];	// ±º‰¥¡	8	÷∏«∞÷√ª˙ ±º‰£¨∏Ò ΩŒ™°∞ƒÍ‘¬»’÷‹ ±∑÷√Î°±£¨24–° ±÷∆£¨±‡¬Î∏Ò Ω≤Œº˚2.3Ω⁄ ±º‰¥¡≤ø∑÷°£
    uint8 FingerMark[16];				//÷∏Œ∆¬Î	16	…˙≥…À„∑®º˚3.3.2Ω⁄°£
    uint8 ExtendInformation[16];		//¿©’π–≈œ¢	16	±∏”√£¨‘› ±“‘°∞0xA5°±◊÷Ω⁄Ω¯––ÃÓ≥‰°£
} FrameWriteSysInfoRs;

typedef struct _loadCreditGetMac1Ret {
    uint8 a_cid[10];		//ø®∫≈£®∫Û16Œª£©
    uint8 a_pt[4];		//≥‰÷µΩ∂Ó£®µ•Œª∑÷£©
    uint8 a_rnd[8];		//Œ±ÀÊª˙ ˝
    uint8 a_cbb[4];		//≥‰÷µ«∞”‡∂Ó£®µ•Œª∑÷£©
    uint8 a_m1[10];		//MAC1¬Î
    uint8 a_on[2];		//¡™ª˙Ωª“◊–Ú∫≈
} loadCreditGetMac1Ret;
//»œ÷§÷°Ω·π˚
extern CertifyResult g_certifyResult;

//ÃÙ’Ω÷°
extern FRAME_CHALLENGE g_frame_challenge_rq;	//£®÷’∂À£©
extern FRAME_CHALLENGE g_frame_challenge_rs;	//£®«∞÷√ª˙£©

//»œ÷§–≈œ¢÷°
extern FRAME_VALIDATE_RQ g_frame_validate_rq;	//£®÷’∂À£©
extern FRAME_VALIDATE_RS g_frame_validate_rs;	//£®«∞÷√ª˙£©

//–¥≥µ¡æ–≈œ¢÷°
extern FRAME_WRITE_VEHINFO_RQ g_frame_writeveh_rq;	//£®÷’∂À£©
extern FRAME_WRITE_VEHINFO_RS g_frame_writeveh_rs;	//£®«∞÷√ª˙£©

//∂¡≥µ¡æ–≈œ¢÷°
extern FRAME_READ_VEHINFO_RQ g_frame_readveh_rq;	//£®÷’∂À£©
extern FRAME_READ_VEHINFO_RS g_frame_readveh_rs;	//£®«∞÷√ª˙£©

//–¥œµÕ≥–≈œ¢÷°
extern FRAME_WRITE_SYSINFO_RQ g_frame_writesys_rq;	//£®÷’∂À£©
extern FRAME_WRITE_SYSINFO_RS g_frame_writesys_rs;	//£®«∞÷√ª˙£©

//œµÕ≥–≈œ¢∂‘±»÷°
extern FRAME_SYSINFO_CHECK_RQ g_frame_syscheck_rq;	//£®÷’∂À£©
extern FRAME_SYSINFO_CHECK_RS g_frame_syscheck_rs;	//£®«∞÷√ª˙£©

//∫ÛÃ®–≈œ¢≤È—Ø÷°
extern FRAME_QUERY_BACKSTAGEINFO_RQ g_frame_querybackInfo_rq;	//£®÷’∂À£©
extern FRAME_QUERY_BACKSTAGEINFO_RS g_frame_querybackInfo_rs;	//£®«∞÷√ª˙£©

//…œ¥´Ωª“◊–≈œ¢÷°
extern FRAME_UPLOAD_TRADEINFO_RQ g_frame_uploadtradeinfo_rq;	//£®÷’∂À£©
extern FRAME_UPLOAD_TRADEINFO_RS g_frame_uploadtradeinfo_rs;	//£®«∞÷√ª˙£©

//◊¢œ˙»œ÷§–≈œ¢÷°
extern FRAME_LOGOUT_VALIDATE_RQ g_frame_logoutvalidate_rq;	//£®÷’∂À£©
extern FRAME_LOGOUT_VALIDATE_RS g_frame_logoutvalidate_rs;	//£®«∞÷√ª˙£©

//»¶¥Ê≥ı ºªØ÷∏¡Ó
extern FRAME_QUANCUN_INIT_RQ g_frame_quancun_init_rq;	//£®÷’∂À£©
extern FRAME_QUANCUN_INIT_RS g_frame_quancun_init_rs;	//£®«∞÷√ª˙£©

//»¶¥Ê÷∏¡Ó
extern FRAME_QUANCUN_RQ g_frame_quancun_rq;	//£®÷’∂À£©
extern FRAME_QUANCUN_RS g_frame_quancun_rs;	//£®«∞÷√ª˙£©

//≥‰÷µ÷∏¡Ó
extern FRAME_RECHARGE_RQ g_frame_Recharge_rq;	//£®÷’∂À£©;
extern FRAME_RECHARGE_RS g_frame_Recharge_rs;	//£®«∞÷√ª˙£©;

//”√ªßµ«¬Ω÷∏¡Ó
extern FRAME_NETLOGIN_RQ g_frame_NetLogin_rq;	//£®÷’∂À£©;
extern FRAME_NETLOGIN_RS g_frame_NetLogin_rs;	//£®«∞÷√ª˙£©;

//ºÏ∂®∑¢––∫œ∑®–‘÷∏¡Ó
extern FRAME_CHECKISSUEVALIDITY_RQ g_frame_ChkIssVal_rq;
extern FRAME_CHECKISSUEVALIDITY_RS g_frame_ChkIssVal_rs;

extern loadCreditGetMac1Ret g_loadCreditGetMac1_Ret;

/********** End –¬ÃÌº”‘⁄œﬂ∑¢––÷° ************************************************/

//π´”√∫Ø ˝common.c
extern uint8 CheckSafe(uint8 *num);
extern uint32 pkt_code(uint8 *src_buf, uint8 *target_buf, uint32 len);	// ˝æ›◊™“Â
extern uint32 pkt_code_udpnet(uint8 *src_buf, uint8 *target_buf, uint32 len);// ˝æ›◊™“Â

//*****************************¥Æø⁄≤Ÿ◊˜Ω”ø⁄***********************//
extern int serial_open();
extern int serial_close();
extern int creat_serial_thread();
extern int serial_send();
extern int serial_recv(int time_out);

//**********UDPÕ¯ø⁄≤Ÿ◊˜Ω”ø⁄  Add by wll2013-3-29 10:37:48 ***********************//
extern int init_Net(char* DSET_IP_ADDRESS, int DEST_PORT);
extern int Net_close();
extern int creat_udpnet_thread();

//*****************************U◊™¥Æø⁄≤Ÿ◊˜Ω”ø⁄********************//
extern int usbcom_open();
extern int usbcom_close();
extern int creat_usbcom_thread();
//*****************************Õ¯ø⁄≤Ÿ◊˜Ω”ø⁄***********************//
extern int net_server_start();
//*****************************STR715≤Ÿ◊˜Ω”ø⁄********************//
extern int str715_poweron();		//STR715…œµÁ
extern int str715_poweroff();		//STR715∂œµÁ
//*****************************Õ¯ø®≤Ÿ◊˜Ω”ø⁄***********************//
extern int eth0_suspend();			//¡ÓÕ¯ø®Ω¯»Î–›√ﬂ◊¥Ã¨
extern int eth0_resume();			//ªΩ–—Õ¯ø®
//*****************************»’÷æ/≈‰÷√Œƒº˛≤Ÿ◊˜Ω”ø⁄****************//
extern int set_config_key(const char *CFG_file, const char *section,
                          const char *key, char *buf);
extern int get_config_key(const char *CFG_file, const char *section,
                          const char *key, char *buf);
extern int write_log(const char *log_file, const char *msg);
extern int recover_hdrsu_conf(const char *conf_file);
//*****************************OBU∑¢––≤Ÿ◊˜Ω”ø⁄********************//
//RSU≥ı ºªØ
extern int send_c0(PROG_COMM_C0 prog_c0, int TerminalTradeNO);
extern int recv_b0(PROG_COMM_B0 *prog_b0,/* int psam_slot, uint16 dir, */
                   int time_out);
//BST VST
extern int send_c1(PROG_COMM_C1 prog_c1);
extern int recv_b1(PROG_COMM_B1 *prog_b1, int time_out);
//√‹‘øÃÊªª
extern int send_c2(PROG_COMM_C2 prog_c2, int time_out);
extern int recv_b2(PROG_COMM_B2 * prog_b2);
//Œƒº˛∏¸–¬
extern int send_c3(PROG_COMM_C3 prog_c3, int time_out);
extern int send_c5(PROG_COMM_C5 prog_c5);
extern int recv_b4(PROG_COMM_B4 *prog_b4, int time_out);
extern int send_c4(PROG_COMM_C4 prog_c4, int time_out);	//∂¡»°Œƒº˛
extern int recv_b3(PROG_COMM_B3 *prog_b3);				//∂¡»°Œƒº˛µƒ∑µªÿ
//∂¡»°OBU SN∫≈
extern int send_c6(int TimeOut);
extern int recv_b6(uint8 *EsamSerialNum);
//∂¡»°icø®–≈œ¢
extern int recv_iccInfo(PROG_COMM_B3 *prog_b3, int time_out);
//∏¥Œªpsamø®,Ω¯ƒø¬º
extern int ResetPsam_EnterDir(uint8 *psam_num, int psam_slot, uint16 dir,
                              int time_out);
//≥ı ºªØUDPÕ¯¬Á≤Œ ˝÷°
extern int InitUDPNetFrame(uint8 InitData[20]);

//*****************************OBU‘⁄œﬂ∑¢––≤Ÿ◊˜Ω”ø⁄********************//add by wll 2013-3-25 9:22:54
//RSU≥ı ºªØ
extern int send_c0_OnLine(PROG_COMM_C0 prog_c0, int TerminalTradeNO);
extern int recv_b0_OnLine(PROG_COMM_B0 *prog_b0, int time_out);
//BST VST
extern int send_c1_OnLine(PROG_COMM_C1 prog_c1);
extern int recv_b1_OnLine(PROG_COMM_B1 *prog_b1, int time_out);
//√‹‘øÃÊªª
extern int send_c2_OnLine(PROG_COMM_C2 prog_c2, int time_out);
extern int recv_b2_OnLine(PROG_COMM_B2 *prog_b2);
//Œƒº˛∏¸–¬
extern int send_c3_OnLine(PROG_COMM_C3 prog_c3, int time_out);
extern int send_c5_OnLine(PROG_COMM_C5 prog_c5);
extern int recv_b4_OnLine(PROG_COMM_B4 *prog_b4, int time_out);
extern int send_c4_OnLine(PROG_COMM_C4 prog_c4, int time_out);	//∂¡»°Œƒº˛
extern int recv_b3_OnLine(PROG_COMM_B3 *prog_b3);				//∂¡»°Œƒº˛µƒ∑µªÿ
//∂¡»°OBU SN∫≈
extern int send_c6_OnLine(int TimeOut);
extern int recv_b6_OnLine(uint8 *EsamSerialNum);

//–¥≥µ¡æ–≈œ¢µƒ«Î«Û”Î”¶¥£®÷’∂À”Îobu£©
extern int send_c7_OnLine(PROG_COMM_C3 prog_c3, int time_out);
extern int recv_b7_OnLine(PROG_COMM_B2 *prog_b2);

//∂¡≥µ¡æ–≈œ¢µƒ«Î«Û”Î”¶¥£®÷’∂À”Îobu£©
//int send_c8_OnLine(PROG_COMM_C4 prog_c4, int time_out);
extern int send_c8_OnLine(PROG_COMM_C4 prog_c4, int c8_type, int time_out);
extern int recv_b8_OnLine(PROG_COMM_B3 *prog_b3);

//∂¡œµÕ≥–≈œ¢µƒ«Î«Û”Î”¶¥£®÷’∂À”Îobu£©
extern int send_c9_OnLine(PROG_COMM_C4 prog_c4, int time_out);
extern int recv_b9_OnLine(PROG_COMM_B3 *prog_b3);

//ª˙ø®¡™∫œ»œ÷§Ω”ø⁄
extern int CardCertify(uint8* terminalPhysicsID, CertifyResult *certifyResult,
                       int time_out);
//∫ÛÃ®–≈œ¢≤È—ØΩ”ø⁄
extern int recv_b3s_OnLine(PROG_COMM_B3 *prog_b3, int time_out);
//◊¢œ˙ª˙ø®¡™∫œ»œ÷§Ω”ø⁄
//extern int LogOut_CardCertify(int TerminalTradeNO, uint8 TradeTimeStamp[10], int time_out);
extern int LogOut_CardCertify(int TerminalTradeNO, uint8 TradeTimeStamp[10],
                              int *Reserved, int time_out);

//…œ¥´Ωª“◊–≈œ¢Ω”ø⁄
extern int UpLoad_TradeInfo(int DBNUM,
                            FRAME_UPLOAD_TRADEINFO_RQ frame_uploadtradeinfo_rq, uint8 *anTalkKey,
                            int nTalkKeylen, int time_out);
//÷ÿ–¬∑¢ÀÕœµÕ≥–≈œ¢∂‘±»÷°∫Ø ˝Ω”ø⁄
extern int ReSendCompareSysInfo(uint8 *rq, uint8 *rs, int time_out);
//±£¥ÊΩª“◊–≈œ¢µΩJAVA≤„µƒ ˝æ›ø‚÷–
extern int SavaTradeInfoToJavaDB(
                                 FRAME_UPLOAD_TRADEINFO_RQ *frame_uploadtradeinfo_rq, int time_out);
//…Ë÷√ª·ª∞√ÿ‘ø
extern int SetTalkKey(uint8 *au8TalkKey, int length);
//*****************************ICø®≤Ÿ◊˜Ω”ø⁄**********************//

extern int send_init(PROG_COMM_C0 prog_c0);
extern int recv_init(PROG_COMM_B0 *prog_b0, int time_out);
extern int send_icc_reset(uint8 channel_id);
extern int recv_icc_reset(int time_out);
extern int send_enter_dir(uint16 did);
extern int recv_enter_dir(int time_out);
extern int send_read_file(uint16 fid, uint8 offset, uint8 length);
extern int recv_read_file(PROG_COMM_ICC *prog_icc, int time_out);
//*****************************OBUπ˙±ÍΩª“◊≤Ÿ◊˜Ω”ø⁄*******************//
//∑¢ÀÕBST
extern int send_bst();
//≥ı ºªØ÷∏¡Ó
extern int send_trade_c0(PROG_TRADE_C0 prog_trade_c0);
//ºÃ–¯Ωª“◊÷∏¡Ó
extern int send_trade_c1(PROG_TRADE_C1 prog_trade_c1, int step, int time_out);
//Õ£÷πΩª“◊÷∏¡Ó
extern int send_trade_c2(PROG_TRADE_C2 prog_trade_c2);
//–¥’æ–≈œ¢÷∏¡Ó
extern int send_trade_c3(PROG_TRADE_C3 prog_trade_c3, int time_out);
//œ˚∑—≤¢–¥’æ–≈œ¢÷∏¡Ó
extern int send_trade_c6(PROG_TRADE_C6 prog_trade_c6, int time_out);
//“Ï≥£¥¶¿Ì÷∏¡Ó
extern int send_trade_c7(PROG_TRADE_C7 prog_trade_c7, int time_out);
//RSU…Ë±∏◊¥Ã¨–≈œ¢÷°
extern int recv_trade_b0(PROG_TRADE_B0 *prog_trade_b0, int psam_slot,
                         uint16 dir, int time_out);
//µÿ∏–◊¥Ã¨–≈œ¢÷°
extern int recv_trade_b1(PROG_TRADE_B1 *prog_trade_b1, int time_out);
//OBU∫≈–≈œ¢÷°
extern int recv_trade_b2(PROG_TRADE_B2 *prog_trade_b2, int time_out);
//OBU Ù–‘÷°
extern int recv_trade_b3(PROG_TRADE_B3 *prog_trade_b3);
//ICø®–≈œ¢÷°
extern int recv_trade_b4(PROG_TRADE_B4 *prog_trade_b4);
//≥…π¶Ωª“◊Ω· ¯÷°
extern int recv_trade_b5(PROG_TRADE_B5 *prog_trade_b5);
//*****************************OBU◊‘”…¡˜Ωª“◊≤Ÿ◊˜Ω”ø⁄*******************//
extern int send_BST1();								//∑¢ÀÕBST-Ωª“◊ƒ£ Ω1
extern int recv_PrWRq(uint8 *obu_mac, int time_out);				//Ω” ’PrWR÷°
extern int send_PrWA(void);							//∑¢ÀÕPrWA÷°
extern int recv_VST1(uint8 *veh_plate_number, uint8 *battery, int time_out);//Ω” ’VST-Ωª“◊ƒ£ Ω1
extern int send_SetSecureRq(void);					//∑¢ÀÕΩª“◊–≈œ¢÷°
//int recv_QuickAck(int time_out);					//Ω” ’øÏÀŸœÏ”¶÷°
//int send_GetAns(void);							//∑¢ÀÕ»°Ω·π˚÷°
extern int recv_SetSecureRs(int time_out);			//Ω” ’»∑»œ–≈œ¢÷°
extern int send_EventReport(uint8 event_type);		//∑¢ÀÕEventReport
//********************************************************************//
extern int8 psamEnterDir(int psam_slot, uint16 dir);
//*****************************◊‘”…¡˜≤Ÿ◊˜Ω”ø⁄*******************//
extern int send_zyl_c1(PROG_COMM_C1 prog_c1);
extern int send_c8(PROG_COMM_C8 *prog_c8, int record_num, int TimeOut);
//*****************************str715Ω”ø⁄*******************//
extern int send_ca();
extern int recv_ba(PROG_COMM_BA *prog_ba, int TimeOut);
extern void intToBytes2(int value, uint8* src);
extern CardConsumeRecordList g_Files_0019;
#define WJOBUTEST
extern loadCreditGetMac1Ret g_sret;
#if 0
//*****************************¡™ª˙≤Ÿ◊˜Ω”ø⁄**********************//
extern int net_to_serial(uint8 *cmd_type, int *step_num, int *fd, int time_out);//Õ¯ø⁄ ’µΩ∑¢Õ˘¥Æø⁄
extern int serial_to_net(int time_out);//¥Æø⁄ ’µΩ∑¢Õ˘Õ¯ø⁄
extern int usbcom_to_serial(uint8 *cmd_type, int *step_num, int *fd, int time_out);//U◊™¥Æø⁄ ’µΩ∑¢Õ˘¥Æø⁄
extern int serial_to_usbcom(int time_out);//¥Æø⁄ ’µΩ∑¢Õ˘U◊™¥Æø⁄
#endif

#endif

