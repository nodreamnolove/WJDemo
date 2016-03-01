

#ifndef issue_Ble_recv_h
#define issue_Ble_recv_h

#include <stdio.h>
#include "lib2hd.h"

int recv_b1_Ble_OC(PROG_COMM_B1 *prog_b1, int time_out);

int recv_b4_Ble_OC(PROG_COMM_B4 *prog_b4, int time_out);

int recv_b9_Ble_OC(PROG_COMM_B3 *prog_b3, int time_out);

//解析transferchannel

int TransferChannel_rs_OC(int * DATALIST, char *Data, int time_out);
//解析

int INITIALISATION_rs_OC(char *obu_mac, char *sys_info, char *icc_info, char *rnd_info, char *reset_info, char *obu_status, int time_out);

int SetMMI_rs_OC();

int recv_b9_Blefile_OC(PROG_COMM_B3 *prog_b3, uint8 DidFid);
#endif
