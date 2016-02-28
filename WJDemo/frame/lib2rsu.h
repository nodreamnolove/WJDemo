#ifndef  _LIB2RSU_H
#define	 _LIB2RSU_H
#import "CToOcType.h"

#include "common.h"

#define	CMD_WRITESYSFILE_JINYI	0x08

/* **************************以下为INITIALISATION原语指令************************** */
int	INITIALISATION_rq(int bst_type, char *beacon_id, char *unix_time, int profile, int obu_init_mode);
int	INITIALISATION_rs(char *obu_mac, char *sys_info, char *icc_info, char * rnd_info, char * reset_info, char *obu_status, int time_out);

/* **************************以下为ACTION原语指令************************** */ 
int	GetSecure_rq(char *AccessCredentials, int offset, int length, char *RandRSU, int KeyIDforEncryptOp, int KeyIDforEncrypt);
int	GetSecure_rs(int *length, char *File, char *authenticator, int time_out);
int	TransferChannel_rq(int DID, int ChannelID, int APDULIST, char *APDU);
int	TransferChannel_rs(int * DATALIST, char *Data, int time_out);
int	SetMMI_rq_new(int SetMMIPara, uint8 *IdentInfo);
int	SetMMI_rq(int SetMMIPara);
int	SetMMI_rs(int time_out);

int PrWRQ_rs(uint8 *obu_mac, uint8 *ant_id, int time_out);
int PrWA_rq(uint8 *obu_mac, uint8 ant_id);
int SetSecure_rq(uint8 *accessCredentials, FILE_PIB *file, uint8 *randRsuForAuthen, uint8 randRsuForAuthenIndex, uint8 ant_id, uint8 OBUActions);
int QuickAck_rs(int time_out);
int GetAns_rq(uint8 *obu_mac, uint8 ant_id);
int SetSecure_rs(int time_out);

/* **************************以下为EVENT_REPORT原语指令************************** */ 
int	EVENT_REPORT_rq(uint8 event_type, uint8 ant_id);
int WriteVehFile_rq_Jinyi(int psam_slot,uint8 offset,uint8 length,uint8 * file);
int WriteVehFile_rq_Jinyi_online(int psam_slot,uint8 offset,uint8 length,uint8 * file);
int WriteVehFile_rs_JinYi(int TimeOut);
int WriteSysFile_rq_Jinyi(int psam_slot,uint8 offset,uint8 length,uint8 * file);
int WriteSysFile_rs_JinYi(int time_out);


int IsVst(uint8 *pkg);
uint8 IsTransfer_rs(uint8 *pkg);
uint8 IsSetMMI_rs(uint8 *pkg);
#endif


