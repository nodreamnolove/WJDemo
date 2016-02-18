

#ifndef issue_Ble_recv_h
#define issue_Ble_recv_h

#include <stdio.h>
#include "lib2hd.h"

int recv_b1_Ble_OC(PROG_COMM_B1 *prog_b1, int time_out);

int recv_b4_Ble_OC(PROG_COMM_C5 prog_c5);

int recv_b9_Ble_OC(PROG_COMM_C4 prog_c4, int time_out);


int TransferChannel_rs_OC(int DID,int ChannelID,int APDULIST,uint8 * APDU);

int INITIALISATION_rs_OC(char *obu_mac, char *sys_info, char *icc_info, char *rnd_info, char *reset_info, char *obu_status, int time_out);

#endif
