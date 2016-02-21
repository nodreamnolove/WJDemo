#ifndef _ISSUE_BLE_SEND_H
#define _ISSUE_BLE_SEND_H
#include "lib2hd.h"
#include "lib2rsu.h"

//FRAME_SYSINFO_CHECK_RQ g_frame_syscheck_rq;
//
//FRAME_UPLOAD_TRADEINFO_RQ g_frame_uploadtradeinfo_rq;


int send_c1_Ble_OC(PROG_COMM_C1 prog_c1);

int send_c5_Ble_OC(PROG_COMM_C5 prog_c5);

int send_c9_Ble_OC(PROG_COMM_C4 prog_c4, int time_out);

int TransferChannel_rq_OC(int DID,int ChannelID,int APDULIST,uint8 * APDU);

int INITIALISATION_rq_OC(int bst_type, char *beacon_id, char *unix_time, int profile, int obu_init_mode);

int SetMMI_rq_OC(int SetMMIPara);

#endif

