#ifndef _COMMON_H
#define _COMMON_H
#import "CToOcType.h"
//#ifdef	__cplusplus
//extern "C" {
//#endif

#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
//#include <iconv.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <termios.h>

#include<stddef.h>
//#include <sys/signal.h>
#include <sys/ioctl.h>

//TCP/IP
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include "lib2hd.h"
#include "saveStruct.h"

//***************************** ÷≥÷…Ë±∏∫Í∂®“Â*****************************//
#define 	BACKLOG 				10				// Õ¯¬Á◊Ó¥ÛÕ¨ ±¡¨Ω”«Î«Û ˝
#define		ICC_0002_LEN			4
#define		ICC_0015_LEN			43
#define		ICC_0016_LEN			55
#define		ICC_0012_LEN			43
#define		ICC_0019_LEN			43
#define		ICC_0018_LEN			23

//*****************************∂®“Â»´æ÷¥ÌŒÛ≥£ ˝*****************************//
#define 	ERR_NO					0				// ÷¥––≥…π¶
#define 	ERR_DATA_WRONG			10				// Ω” ’µΩµƒ ˝æ›¥ÌŒÛ
#define 	ERR_CHECK_WRONG			11				// –£—È¥ÌŒÛ
#define 	ERR_SERIAL_SEND			12				// ¥Æø⁄∑¢ÀÕ ß∞‹
#define 	ERR_NET_SEND			13				// Õ¯ø⁄∑¢ÀÕ ß∞‹
#define 	ERR_OPEN_FILE			14				// ¥Úø™Œƒº˛ ß∞‹
#define 	ERR_UDPNET_SEND			15				// ¥Æø⁄∑¢ÀÕ ß∞‹
#define 	ERR_USBCOM_SEND			20				// USB◊™¥Æø⁄∑¢ÀÕ ß∞‹

//*****************************∂‘RSU ˝æ›Ω·ππ*********************************//
enum
{
    BST_ISSUE_NOPARAM  = 0,
    BST_ISSUE_NOREAD   = 1,
    BST_ISSUE_PREREAD  = 2,
    BST_TRADE_GB = 3,
    BST_TRADE_ZYL = 4,
    BST_ISSUE_ZYL = 5,
    BST_ISSUE_BJ = 6,
};

typedef struct _BST {
    uint8 profile;
} ST_BST;

typedef struct _VST {
    uint8 macid[4];
    uint8 sysinfo[128];
    uint8 iccinfo[128];
    uint8 rndinfo[128];
    uint8 resetinfo[128];
    uint8 RandOBE[8];
    uint8 vehicleLicensePlateNumber[12];
    uint8 vehicleType;
    uint8 vehicleUserType;
    uint8 ContractSerialNumber[8];
    uint8 ObuConfigurationMacID[8];
    uint8 obustatus[3];
} ST_VST;

typedef struct _TRANSFER_CHANNEL_RQ {
    uint8 channelid;
    uint8 apdulist;
    uint8 apdu[256];
    uint8 frame_len;
} ST_TRANSFER_CHANNEL;

//esam–≈œ¢∂¡»°À˘”√µΩµƒΩ·ππ
typedef struct _ESAM_READ_FILE {
    uint8 NumOfFiles; //∂¡»°Œƒº˛µƒ ˝¡ø
    uint8 DIDnFID[10];
    uint8 offset[10];
    uint8 len[10];
} ST_READ_FILE;

typedef struct _ICC_PIB {
    unsigned short iccRet;        //ICC∑µªÿ◊¥Ã¨
    uint8 Balance[4];
    uint8 icc0002[ICC_0002_LEN];  //0015Œƒº˛
    uint8 icc0015[ICC_0015_LEN];  //0015Œƒº˛
    uint8 icc0016[ICC_0016_LEN];  //0015Œƒº˛
    uint8 icc0012[ICC_0012_LEN];  //0012Œƒº˛
    uint8 icc0018[ICC_0018_LEN];  //0012Œƒº˛
    uint8 icc0019[ICC_0019_LEN];  //0019Œƒº˛
    signed long money;            //Ω´”‡∂ÓªªÀ„≥… ÆΩ¯÷∆
    
    // Ωª“◊œ‡πÿ–≈œ¢
    uint8 TAC[4];                 //TAC¬Î
    uint8 Payserial[2];           //CPUø®Ωª“◊–Ú∫≈
    uint8 frame_size;             //µ±«∞÷°≥§∂»
} ICC_PIB;

typedef struct _PSAM_PIB {
    uint8 accessCredentials[8];
    uint8 TransSerial[4];         //PSAMø®Ωª“◊–Ú∫≈
    uint8 PSAMNO[6];
    uint8 MAC1[4];
    uint8 MAC2[4];
    uint8 randRsuForAuthen[8];
    uint8 randRsuForAuthenIndex;
    uint8 psamSlot;
} PSAM_PIB;

typedef struct _PrWRq_PIB {
    uint8 OBUMAC[4];
    uint8 AntID;
} PrWRq_PIB;

typedef struct _FILE_PIB {
    uint8 exchangeTime[4];
    uint8 PSAMID[6];
    uint8 Tr_SN[4];
    uint8 exchangeStation[2];
    uint8 exchangeRoadway;
    uint8 balance[2];
    uint8 vehicleStatus;
} FILE_PIB;


uint8 pkt_decode(uint8 *buf, uint32 len, uint8 port);				//Ω¯–– ˝æ›∞¸µƒΩ‚¬Îπ§◊˜
uint8 pkt_decode_udp(uint8 *buf, uint32 len, uint8 port);
void print_info(int,int);
void print_info_Info(char Info[],int Len,int dev_type);
int DtoHex(int data);



extern  PKG_DATA  g_pkg_data;
extern  PKG_ICCINFO_DATA g_pkg_iccinfo_data;
//extern  PROG_COMM_C4   g_c4_data;
//extern  PROG_COMM_B3   g_b3_data;


//*****************************RSU ÷≥÷…Ë±∏»´æ÷±‰¡ø…˘√˜*****************************//


extern char gtf_veh_file[128];//≥µ¡æ–≈œ¢buf£¨Ω“Ê¿œ∞ÊOBU–Ë“™Õ®π˝–¥≥µ¡æ–≈œ¢ÕÍ≥…º§ªÓ£¨œ»Ω´±Í«©÷–≥µ¡æ–≈œ¢∂¡≥ˆ£¨‘Ÿ–¥»Î°£2013-8-20 by gaotf
extern uint8 g_OBU_ContractVersion;	// OBU∫œÕ¨∞Ê±æ
extern uint8 g_psam_slot;			//	psamø®≤€
extern uint8 g_bst_type;
extern int g_rsctl;				//÷°–Ú∫≈
extern uint8 g_master_card;		//∑¢––ƒ∏ø®ø®≤€∫≈
extern uint8 g_issue_card;		//∂˛¥Œ∑¢––PSAMø®ø®≤€∫≈
//extern uint8 g_psam_slot;	//≥µ¡æΩ‚√‹PSAMø®ø®≤€∫≈
extern uint8 g_trade_card;		//Ωª“◊PSAMø®ø®≤€∫≈

extern uint8 run_mode;
extern char g_obu_mac[4];
extern uint8 g_psam_disperse;
//¥Æø⁄
extern int32 g_serial_fd;		//¥Æø⁄Œƒº˛√Ë ˆ∑˚
extern uint32 g_com_len;
extern uint32 g_com_Framelen;
extern uint8 g_com_buf[MAXCOMDATALENGHT];
extern uint16 g_start_flag;
extern uint8 g_frame_flag;

//UdpÕ¯ø⁄
extern int g_udpnet_fd ;		    //UdpÕ¯ø⁄Œƒº˛√Ë ˆ∑˚
extern uint32 g_udpnet_len ;
extern uint8 g_udpnet_buf[MAXUDPDATALENGHT];
extern uint16 g_udpnet_start_flag;
extern uint8 g_udpnet_frame_flag;
extern uint8 g_IsRecvUdpNetData ;//ºÏ≤ÈUDP «∑Ò ’µΩ ˝æ› add by wll 2013-4-20 10:17:59
//0£∫Œ¥ ’µΩ
//1£∫ ’µΩ ˝æ›£¨≤ª±£÷§ ˝æ› «∑Ò’˝»∑
extern uint8 g_IscanSend;
extern uint8 g_IsRecvUdpNetData_tmp ;
extern int is_udpnetinit_recved;
extern int is_rsuinit_recved;
extern int is_vst_recved;
extern int is_getsecure_recved;
extern int is_transfer_recved;
extern int is_setmmi_recved;
extern int is_psamchannel_recved;
extern int is_psamreset_recved;
extern int is_iccreset_recved;
extern int is_enterdir_recved;
extern int is_readfile_recved;
extern int is_prwrq_recved;
extern int is_quickack_recved;
extern int is_setsecure_recved;
extern int is_str715ver_recved;

extern sem_t g_sem_udpnet_init_00;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-ÃÙ’Ω÷°
extern sem_t g_sem_udpnet_init_02;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-»œ÷§”¶¥÷°
extern sem_t g_sem_udpnet_init_14;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-–¥≥µ¡æ–≈œ¢”¶¥÷°
extern sem_t g_sem_udpnet_init_16;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-∂¡≥µ¡æ–≈œ¢”¶¥÷°
extern sem_t g_sem_udpnet_init_18;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-–¥œµÕ≥–≈œ¢”¶¥÷°
extern sem_t g_sem_udpnet_init_1A;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-œµÕ≥–≈œ¢∂‘±»”¶¥÷°
extern sem_t g_sem_udpnet_init_1C;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-∫ÛÃ®–≈œ¢≤È—Ø
extern sem_t g_sem_udpnet_init_1E;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-…œ¥´Ωª“◊–≈œ¢”¶¥÷°
extern sem_t g_sem_udpnet_init_20;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-◊¢œ˙»œ÷§”¶¥÷°
extern sem_t g_sem_udpnet_init_22;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-»¶¥Ê≥ı ºªØ”¶¥÷°
extern sem_t g_sem_udpnet_init_24;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-»¶¥Ê”¶¥÷°
extern sem_t g_sem_udpnet_init_26;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-≥‰÷µ”¶¥÷°
extern sem_t g_sem_udpnet_init_28;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-∫ÛÃ®’À∫≈µ«¬Ω”¶¥÷°
extern sem_t g_sem_udpnet_init_2A;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-ºÏ∂®∑¢––∫œ∑®–‘”¶¥÷°

extern sem_t g_sem_rsuinit;
extern sem_t g_sem_vst;			//vst–≈∫≈¡ø
extern sem_t g_sem_getsecure;	//getsecure–≈∫≈¡ø
extern sem_t g_sem_transfer;	//transfer–≈∫≈¡ø
extern sem_t g_sem_setmmi;		//setmmi÷°–≈∫≈¡ø
extern sem_t g_sem_psamchannel;
extern sem_t g_sem_psamreset;
extern sem_t g_sem_iccreset;
extern sem_t g_sem_enterdir;
extern sem_t g_sem_readfile;
extern sem_t g_sem_prwrq;
extern sem_t g_sem_quickack;
extern sem_t g_sem_setsecure;
extern sem_t g_sem_str715ver;

//µ±«∞obu ˝æ›–≈œ¢
extern ST_VST vst;
extern ST_BST bst;
extern ICC_PIB icc_pib;
extern PSAM_PIB psam_pib;
extern PrWRq_PIB prwrq_pib;
extern FILE_PIB file_pib;
extern uint8 esam_random[8];			//esam÷–µƒÀÊª˙ ˝
extern uint8 EsamDv[8];					//÷˜øÿ√‹‘øÀ˘”√µƒ∑÷…¢“Ú◊”
extern uint8 EsamSn[8];					//–æ∆¨–Ú¡–∫≈,œµÕ≥÷˜øÿ√‹‘ø∫Õ”¶”√÷˜øÿ√‹‘øÀ˘”√µƒ∑÷…¢“Ú◊”
extern uint8 g_contract_sn[8];			//∫œÕ¨–Ú¡–∫≈
extern uint8 psam_encrypted_key[28];	//¥˝ÃÊªªµƒ√‹‘ø
extern uint8 esam_mac[4];				//–¥Œƒº˛ ±–Ë”√psamÀ„µƒ∞≤»´±®Œƒmac
extern uint8 cur_key[16];				//Õ®π˝psamµº≥ˆµƒ∏˜¿‡√‹‘ø
extern uint8 g_veh_file_sign;			//0-√˜Œƒ,1-√‹Œƒ
//esam–≈œ¢∂¡»°À˘”√µΩµƒ±‰¡ø
extern ST_READ_FILE g_read_file;
extern uint8 esam_read_sysinfo[128];	//œµÕ≥–≈œ¢
extern uint8 esam_read_vehicle[128];	//≥µ¡æ–≈œ¢
extern uint8 esam_read_info[128];		//00 b0√¸¡Ó∂¡»°µƒ–≈œ¢
extern uint8 psam_read_info[128];		//00 b0√¸¡Ó∂¡»°µƒ–≈œ¢
extern uint8 psam_mac[8];				//psamÀ„µƒº¯±¬Î£¨getsecure_rq÷–À˘”√
extern uint8 psam_random[8];			//psam÷–ªÒµ√µƒÀÊª˙ ˝
extern uint8 contract_version;			//±±æ©∏˘æ›≤ªÕ¨µƒ∞Ê±æ∫≈£¨«¯∑÷”≤samªπ «»Ìsam
extern uint8 g_esam_type;
extern uint8 once_sign;
extern uint8 g_ObuInitMode;

/* ********************* ¡™ª˙ƒ£ Ωœ¬œ‡πÿΩ”ø⁄ ******************* */
extern int online_sign;					// «∑Ò¡™ª˙≤Ÿ◊˜
extern int is_net_recved;
extern int is_udpnet_recved;
extern int is_serial_recved;
extern int is_usbcom_recved;
extern sem_t g_sem_serialrecv;			//¥Æø⁄ ’µΩ ˝
extern sem_t g_sem_udpnetrecv;			//Õ¯ø⁄ ’µΩ ˝ add by wll 2013-3-22 13:53:53

char InfoTmp[100];				//¥Ú”°–≈œ¢ ˝◊È

#if 0
//USB2COM
extern int		g_usbcom_fd;				// USB◊™¥Æø⁄Œƒº˛√Ë ˆ∑˚
extern int		g_usbcom_rx_len;			// USB◊™¥Æø⁄Ω” ’ ˝æ›≥§∂»
extern int		g_usbcom_tx_len;			// USB◊™¥Æø⁄∑¢ÀÕ ˝æ›≥§∂»
extern uint8 	g_usbcom_rx_buf[512];		// USB◊™¥Æø⁄Ω” ’ ˝æ›
extern uint8 	g_usbcom_tx_buf[512];		// USB◊™¥Æø⁄∑¢ÀÕ ˝æ›
extern int		g_usbcom_len;
extern uint8 	g_usbcom_buf[512];

extern int 		g_usbcom_flag;
extern unsigned short g_usbcom_start;

// Õ¯¬Á
extern int32	g_server_fd;			// ∑˛ŒÒ∆˜Œƒº˛√Ë ˆ∑˚
extern int32	g_client_fd;			// øÕªß∂ÀŒƒº˛√Ë ˆ∑˚
extern uint32  g_net_rx_len;			// Õ¯ø⁄Ω” ’ ˝æ›≥§∂»
extern uint32  g_net_tx_len;			// Õ¯ø⁄∑¢ÀÕ ˝æ›≥§∂»
extern uint8   g_net_rx_buf[512];		// Õ¯ø⁄Ω” ’ ˝æ›
extern uint8   g_net_tx_buf[512];		// Õ¯ø⁄∑¢ÀÕ ˝æ›
extern char   server_ip[100];
extern int    server_port;

extern sem_t	g_sem_netrecv;//Õ¯¬Á ’µΩ ˝
extern sem_t	g_sem_usbcomrecv;//U◊™¥Æ ’µΩ ˝



#endif

extern int CreateFilePath1(char* filename);

extern int GetTimebufFunction(uint8 *TimeInfo);
extern int getFileSize(char strFileName[]);
//Œƒº˛÷∏’Î
extern FILE *udpnetfp;//
extern FILE *comfp;
extern FILE *netfp;
extern FILE *usbcomfp;

extern char filename_udpnet[100];
extern char filename_com[100];
extern char filename_net[100];
extern char filename_usbcom[100];
//add by wll 2014-12-11
//extern JNIEnv *g_pobjenv;

//#ifdef __cplusplus
//}
//#endif

#endif



