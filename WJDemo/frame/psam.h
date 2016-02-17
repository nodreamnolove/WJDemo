#ifndef _PSAM_H
#define	_PSAM_H
#import "CToOcType.h"

//PSAM母卡进入ETC应用目录 
extern int8 psamEnterETCDir(int psam_slot);
//PSAM卡进入ETC应用目录
extern int8 psamEnterDir(int psam_slot, uint16 dir);
/* ************************** 一次发行所用********************************* */
//要在原密钥分散因子的保护下，获取新分散因子下的密钥
//取得MF系统维护密钥 
extern int8 psamGetDAMK_MF(int psam_slot);
//取得DF01应用维护密钥
extern int8 psamGetDAMK_DF01(int psam_slot);
//取得DF01应用认证密钥 
extern int8 psamGetRK1_DF01(int psam_slot);
//取得DF01应用加密密钥（版本1） 
extern int8 psamGetRK2_DF01_V1(int psam_slot);
//取得DF01应用加密密钥（版本2） 
extern int8 psamGetRK2_DF01_V2(int psam_slot);
//取得DF01应用加密密钥（版本3） 
extern int8 psamGetRK2_DF01_V3(int psam_slot);
/* **************************** 二次发行所用 ****************************** */
//更新系统文件或者车辆文件时需要用psam算一个mac，卡校验通过才有写的权限 
extern int8 psamCalcEsamMac(int psam_slot,uint8 offset,uint8 length,uint8 *file,uint8 type);
/* *****************************标签检测所用******************************* */
//psam获取随机数，obu用它计算mac，用于psam对obu文件信息的认证和解密
extern int psamGetRandom(int psam_slot);
//密钥导出,北京psam
extern int psamGetKey(int psam_slot,uint8 P2);
//解密从ESAM卡读取的应用车辆信息 
extern int8 psamDecryptEsamVehInfo(int psam_slot,uint8 *src,uint8 src_len,uint8 *dst,uint8 *mac);
//校验从ESAM卡读取的应用车辆信息 	   北京协议	 读取握奇母卡密钥明文，软件计算MAC
extern int8 psamVerifyEsamVehInfo(int psam_slot,uint8 *src,uint8 src_len,uint8 *dst_mac);
/* ***********************如下函数应用在硬sam解密中******************************** */
//解密初始化
extern int8 psamDESInit(int psam_slot);
extern int8 psamDES(int psam_slot,uint8 *src_file,int src_len,uint8 *dst_file,uint8 *dst_mac);
//解密后计算MAC
extern int8 psamMAC(int psam_slot,uint8 *rand,uint8 *file,int file_len);

extern int psamCheckMAC1(uint8 psam_slot, uint8* payserial, uint8 *rnd, uint8 trans_mode, uint8 *pay_val, uint8 *time);
extern int psamCheckMAC2(uint8 psam_slot, uint8* mac);
//读取文件
extern int psamReadBinarayFile(uint8 psam_slot, uint8 fid,uint8 offset,uint8 length);
//设置密钥版本号
extern int psamSetKeyindex(uint8 psam_slot, uint8 keyindex);
//生成访问许可证（专有指令3：用于收费交易模式一和二）
extern int psamAccesscredentials(uint8 psam_slot, const uint8 *contractserialnum, uint8 contractserialnumlen, uint8 keyversionid, const uint8 *Rand, uint8 Randlen);
//扩展随机数命令（专有指令1）
extern int psamGetChallenge(uint8 psam_slot);

extern int	PSAM_RESET_rq(int PSAMSlot, int Baud);	//psam复位指令
extern int	PSAM_RESET_rs(int time_out, char* PSAMNO);
extern int	PSAM_CHANNEL_rq(int PSAMSlot, int APDUList, char *APDU);	//PSAM通道指令 
extern int	PSAM_CHANNEL_rs(int *DATALIST, char *Data, int time_out);	//PSAM通道指令返回
extern int8 psamCalcEsamMac_JuLi(int psam_slot,uint8 offset,uint8 length,uint8 *file,uint8 type);

#endif

