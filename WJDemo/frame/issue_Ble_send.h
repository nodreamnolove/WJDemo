#ifndef _ISSUE_BLE_SEND_H
#define _ISSUE_BLE_SEND_H
#include "lib2hd.h"
#include "lib2rsu.h"

//FRAME_SYSINFO_CHECK_RQ g_frame_syscheck_rq;
//
//FRAME_UPLOAD_TRADEINFO_RQ g_frame_uploadtradeinfo_rq;

void init_C4_ReadIccInfo_OC(byte nType,byte index,PROG_COMM_C4 *prog_c4);

int c4_init(PROG_COMM_C4 *prog_c4,byte nType);

int  c5_init(PROG_COMM_C5 *prog_c5);

int send_c1_Ble_OC(PROG_COMM_C1 prog_c1);

int send_c5_Ble_OC(PROG_COMM_C5 prog_c5);

int send_c9_Ble_OC(PROG_COMM_C4 prog_c4, int time_out);

int  send_c9_Ble1_OC(PROG_COMM_C4 prog_c4,int * needble2,int *icc_flag,int *icc_Length);

int  send_c9_Ble2_OC();

int TransferChannel_rq_OC(int DID,int ChannelID,int APDULIST,uint8 * APDU);

int INITIALISATION_rq_OC(int bst_type, char *beacon_id, char *unix_time, int profile, int obu_init_mode);

int SetMMI_rq_OC(int SetMMIPara);

int EVENT_REPORT_rq_OC(uint8 event_type, uint8 ant_id);

void save_Info_OC(PROG_COMM_C4 prog_c4,PROG_COMM_B3 prog_b3);

void save_CpuCardinfo_OC(PROG_COMM_B3 prog_b3);

float byteToFloat(byte* bytearr);

//int send_Quancun_Init_OnLine_To_Front_End_Processor(FRAME_QUANCUN_INIT_RQ frame_quancun_init_rq);
//int getQuanCunInit_Rq_Ble(FrameQuanCunInitRq *data_rq, uint8 *UserName,  uint8 Addmoney[], int miyaotype, int miyaoFlag, int TimeOut);
//int GetFingMarkFunction(uint8 *data,int datalen,uint8 *anTalkKey, int nTalkKeylen,uint8 *OutInfo,int Outlen);
#endif

