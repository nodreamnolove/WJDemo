#ifndef _PSAM_H
#define	_PSAM_H
#import "CToOcType.h"

//PSAMĸ������ETCӦ��Ŀ¼ 
extern int8 psamEnterETCDir(int psam_slot);
//PSAM������ETCӦ��Ŀ¼
extern int8 psamEnterDir(int psam_slot, uint16 dir);
/* ************************** һ�η�������********************************* */
//Ҫ��ԭ��Կ��ɢ���ӵı����£���ȡ�·�ɢ�����µ���Կ
//ȡ��MFϵͳά����Կ 
extern int8 psamGetDAMK_MF(int psam_slot);
//ȡ��DF01Ӧ��ά����Կ
extern int8 psamGetDAMK_DF01(int psam_slot);
//ȡ��DF01Ӧ����֤��Կ 
extern int8 psamGetRK1_DF01(int psam_slot);
//ȡ��DF01Ӧ�ü�����Կ���汾1�� 
extern int8 psamGetRK2_DF01_V1(int psam_slot);
//ȡ��DF01Ӧ�ü�����Կ���汾2�� 
extern int8 psamGetRK2_DF01_V2(int psam_slot);
//ȡ��DF01Ӧ�ü�����Կ���汾3�� 
extern int8 psamGetRK2_DF01_V3(int psam_slot);
/* **************************** ���η������� ****************************** */
//����ϵͳ�ļ����߳����ļ�ʱ��Ҫ��psam��һ��mac����У��ͨ������д��Ȩ�� 
extern int8 psamCalcEsamMac(int psam_slot,uint8 offset,uint8 length,uint8 *file,uint8 type);
/* *****************************��ǩ�������******************************* */
//psam��ȡ�������obu��������mac������psam��obu�ļ���Ϣ����֤�ͽ���
extern int psamGetRandom(int psam_slot);
//��Կ����,����psam
extern int psamGetKey(int psam_slot,uint8 P2);
//���ܴ�ESAM����ȡ��Ӧ�ó�����Ϣ 
extern int8 psamDecryptEsamVehInfo(int psam_slot,uint8 *src,uint8 src_len,uint8 *dst,uint8 *mac);
//У���ESAM����ȡ��Ӧ�ó�����Ϣ 	   ����Э��	 ��ȡ����ĸ����Կ���ģ��������MAC
extern int8 psamVerifyEsamVehInfo(int psam_slot,uint8 *src,uint8 src_len,uint8 *dst_mac);
/* ***********************���º���Ӧ����Ӳsam������******************************** */
//���ܳ�ʼ��
extern int8 psamDESInit(int psam_slot);
extern int8 psamDES(int psam_slot,uint8 *src_file,int src_len,uint8 *dst_file,uint8 *dst_mac);
//���ܺ����MAC
extern int8 psamMAC(int psam_slot,uint8 *rand,uint8 *file,int file_len);

extern int psamCheckMAC1(uint8 psam_slot, uint8* payserial, uint8 *rnd, uint8 trans_mode, uint8 *pay_val, uint8 *time);
extern int psamCheckMAC2(uint8 psam_slot, uint8* mac);
//��ȡ�ļ�
extern int psamReadBinarayFile(uint8 psam_slot, uint8 fid,uint8 offset,uint8 length);
//������Կ�汾��
extern int psamSetKeyindex(uint8 psam_slot, uint8 keyindex);
//���ɷ������֤��ר��ָ��3�������շѽ���ģʽһ�Ͷ���
extern int psamAccesscredentials(uint8 psam_slot, const uint8 *contractserialnum, uint8 contractserialnumlen, uint8 keyversionid, const uint8 *Rand, uint8 Randlen);
//��չ��������ר��ָ��1��
extern int psamGetChallenge(uint8 psam_slot);

extern int	PSAM_RESET_rq(int PSAMSlot, int Baud);	//psam��λָ��
extern int	PSAM_RESET_rs(int time_out, char* PSAMNO);
extern int	PSAM_CHANNEL_rq(int PSAMSlot, int APDUList, char *APDU);	//PSAMͨ��ָ�� 
extern int	PSAM_CHANNEL_rs(int *DATALIST, char *Data, int time_out);	//PSAMͨ��ָ���
extern int8 psamCalcEsamMac_JuLi(int psam_slot,uint8 offset,uint8 length,uint8 *file,uint8 type);

#endif

