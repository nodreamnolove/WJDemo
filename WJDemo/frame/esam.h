#ifndef _ESAM_H
#define	_ESAM_H


extern void esamInitFrame(ST_TRANSFER_CHANNEL * transfer_rq);
extern void esamReplaceKey(ST_TRANSFER_CHANNEL *transfer_rq,uint8 flag);
extern void esamGetRandFrame(ST_TRANSFER_CHANNEL *transfer_rq);
extern void esamEnterDirFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint16 did);
extern void esamReturnMainDir(ST_TRANSFER_CHANNEL *transfer_rq);
extern int esamWriteSysInfo(ST_TRANSFER_CHANNEL *transfer_rq,uint8 offset,uint8 length,uint8 * file);
extern int esamWriteVehInfo(ST_TRANSFER_CHANNEL *transfer_rq,uint8 offset,uint8 length,uint8 *file);
extern void esamReadSysInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 offset,uint8 length);
//获取OBE-SAM芯片序列号
extern void esamGetSn(ST_TRANSFER_CHANNEL *transfer_rq);

extern int esamCheck(uint8 *data,uint8 curlist);
extern int esamCheck_JuLi_Acv(uint8 *data,uint8 curlist);
extern int esamCheck_JuLi_2Issue(uint8 *data,uint8 curlist);
extern int esamCheckGetRand(uint8 * data,uint8 curlist);
extern int esamCheckReadSysInfo(uint8 * data,uint8 curlist); 						
extern int esamCheckGetSn(uint8 * data,uint8 curlist);
extern int esamCheckReadVehInfo(uint8 * data,uint8 curlist);

extern void esamReadFileInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 offset,uint8 length);
extern void esamReadRecordInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 record_num,uint8 length);
extern int esamCheckReadFileInfo(uint8 * data,uint8 curlist);
// 保存车辆信息文件
//extern uint8 esamSaveVehicle(int len_vehicleinfo, uint8 *randRSU, uint8 *File,uint8 *psam_decrypted_data,uint8 *esam_encrypted_data);
extern int esamSaveVehicle(int len_vehicleinfo, uint8 *randRSU, uint8 *File,uint8 *psam_decrypted_data,uint8 *psam_decrypted_mac,uint8 *esam_encrypted_data);
extern int esamWriteSysInfo_JuLi(ST_TRANSFER_CHANNEL *transfer_rq, uint8 offset, uint8 length, uint8 *file);

extern void esamWriteRecordInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 record_num,uint8 length);


void ledInitFrame(ST_TRANSFER_CHANNEL * transfer_rq,char ledid);
void ledflash(ST_TRANSFER_CHANNEL *transfer_rq,char flashCount);
int ledflashCheck(uint8 * data,uint8 curlist);
#endif

