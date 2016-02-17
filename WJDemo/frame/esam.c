#include "lib2hd.h"
#include "common.h"
#include "esam.h"
#include "psam.h"

 
/*
 * �������ƣ�	esamInitFrame
 * ����������
 * �����б�
 * ���ؽ����	0	--	�ɹ�
 *			��0	--	ʧ��
 */
void esamInitFrame(ST_TRANSFER_CHANNEL * transfer_rq)
{
    transfer_rq->channelid = 0x02;
    transfer_rq->apdulist = 0;
    transfer_rq->frame_len = 0;
	memset(transfer_rq->apdu,0,128);
}

/*
* �������ƣ�	esamReplaceKey
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*				��0	--	ʧ��
*/
void esamReplaceKey(ST_TRANSFER_CHANNEL *transfer_rq, uint8 flag)
{
	uint8 len = 0, j = 0;
	transfer_rq->apdulist++;	//������+1
	len = transfer_rq->frame_len;
	transfer_rq->apdu[len++] = 0x21;	//�����
	transfer_rq->apdu[len++] = 0x84;
	transfer_rq->apdu[len++] = 0xd4;
	transfer_rq->apdu[len++] = 0x01;
	if(flag == 0)
	{				
		transfer_rq->apdu[len++] = 0xff;
	}
	else
	{
		transfer_rq->apdu[len++] = 0x00;
	}
	transfer_rq->apdu[len++] = 0x1c;	//28 key
	for(j = 0; j < 28; j++)
	{
	    transfer_rq->apdu[len++] = psam_encrypted_key[j];
	}
	transfer_rq->frame_len = len;
}

/*
* �������ƣ�	esamGetRandFrame
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
void esamGetRandFrame(ST_TRANSFER_CHANNEL *transfer_rq)
{
	uint8 len = 0;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len; 
	//������ļ�
	transfer_rq->apdu[len++] = 0x05;	//�����
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x84;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x04;
	transfer_rq->frame_len = len;	//�����ܳ���
}

/*
* �������ƣ�	esamEnterDirFrame
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
void esamEnterDirFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint16 did)
{
	uint8 len = 0;
	transfer_rq->apdulist++;	//������+1
	len = transfer_rq->frame_len;
	transfer_rq->apdu[len++] = 0x07;	//�����
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0xa4;
	transfer_rq->apdu[len++] = 0x00;				
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x02;
	transfer_rq->apdu[len++] = (did >> 8) & 0xff;
	transfer_rq->apdu[len++] = did & 0xff;
	transfer_rq->frame_len = len;
}

/*
* �������ƣ�	esamWriteVehInfo
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int	esamWriteVehInfo(ST_TRANSFER_CHANNEL *transfer_rq, uint8 offset, uint8 length, uint8 *file)
{
	uint8 i = 0, j = 0, ret = 0;
	int psam_slot = g_issue_card;
	transfer_rq->apdulist++;	//��������+1
	i = transfer_rq->frame_len;
	//д�ļ�����
	transfer_rq->apdu[i++] = length + 9;	//�����files.Length + 9
	transfer_rq->apdu[i++] = 0x04;
	transfer_rq->apdu[i++] = 0xd6;
	transfer_rq->apdu[i++] = 0x81;
	transfer_rq->apdu[i++] = offset;	//ƫ����offset
	transfer_rq->apdu[i++] = length + 4;	//�����length + 4
	//д����
	for(j = 0; j < length; j++)
	{
	    transfer_rq->apdu[i++] = file[j];
	}
	//��д������Ϣ��������Ƶ��ϴ�������Ϣ֡��(д������Ϣ�����)  add by wll 2013-7-5 15:16:09
	memcpy(&g_frame_uploadtradeinfo_rq.WriteVehInfoRandom[0],&esam_random[0],sizeof(g_frame_uploadtradeinfo_rq.WriteVehInfoRandom));

	//����ESAM��ϵͳ��Ϣ�ļ�MAC 
	ret = psamCalcEsamMac(psam_slot, offset, length, file, 1);	//������Ϣ��־��1,��Ϊ���÷�ɢ���Ӳ�ͬ

	if(ret != 0)
	{
		return -1 + ret*10;
	}
	for(j = 0; j < 4; j++)
	{
		transfer_rq->apdu[i++] = esam_mac[j];	//MAC
	}
	//��д������Ϣָ��Ƶ��ϴ�������Ϣ֡��(д������Ϣָ��)  add by wll 2013-7-5 15:16:09
	memcpy(g_frame_uploadtradeinfo_rq.WriteVehInfoCommand,&transfer_rq->apdu[i - 4 - length - 5],5 + length + 4);
	//��0
	memset(&g_frame_uploadtradeinfo_rq.WriteVehInfoCommand[5 + length + 4],0x00,sizeof(g_frame_uploadtradeinfo_rq.WriteVehInfoCommand) - (5 + length + 4));

	//�����ܳ���
	transfer_rq->frame_len = i;
	return 0;
}

/*
* �������ƣ�	esamWriteSysInfo
* ����������	����ϵͳ��Ϣ�ļ�
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int esamWriteSysInfo(ST_TRANSFER_CHANNEL *transfer_rq, uint8 offset, uint8 length, uint8 *file)
{
	char InfoTmp[100];
	uint8 apdu_out[80];
	uint8 esam_random_tmp[8];	
// 	uint8 SourData_tmp[100];
	int SourData_length;
	uint8 i, j, ret = 0;
	int psam_slot = g_issue_card;
	transfer_rq->apdulist++;	//��������+1
	i = transfer_rq->frame_len;
	//д�ļ�����
	transfer_rq->apdu[i++] = length + 9;	//�����files.Length + 9
	transfer_rq->apdu[i++] = 0x04;
	transfer_rq->apdu[i++] = 0xd6;
	transfer_rq->apdu[i++] = 0x81;
	transfer_rq->apdu[i++] = offset;		//ƫ����offset
	transfer_rq->apdu[i++] = length + 4;	//�����length + 4
	//д����
	for(j = 0; j < length; j++)
	{
	    transfer_rq->apdu[i++] = file[j];
	}

// 	SourData_length = length+5;
// 	memcpy(SourData_tmp,&transfer_rq->apdu[i-SourData_length],SourData_length);
//	if (SourData_length%8 != 0)
//	{
//		SourData_tmp[SourData_length++] = 0x80;
//	}
//	while (SourData_length%8 != 0)
//	{
//		SourData_tmp[SourData_length++] = 0x00;
//	}
	//���˾���������ϰ�obu֮�⣬����obu����ʱ����дϵͳ��Ϣ����������ϴ�������Ϣ֡�е�WriteSysInfoRandom
	memcpy(&g_frame_uploadtradeinfo_rq.WriteSysInfoRandom[0],&esam_random[0],sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom));
	//����ESAM��ϵͳ��Ϣ�ļ�MAC
	if (g_OBU_ContractVersion == 0X16)//2��OBUֱ�Ӹ�psam����ȡMAC
	{
		ret = psamCalcEsamMac(psam_slot, offset, length, file, 0);
	}
	else	//1��OBU��PSAM����ȡ��Կʹ��FD_TripleMAC�㷨����MAC
	{
//		ret = psamGetKey(g_psam_slot,0x08);
//
//		sprintf(InfoTmp,"psamGetKey ERR, ret = %d\n", ret);
//		print_info_Info(InfoTmp,strlen(InfoTmp),COM);
//
//		memset(esam_random_tmp,0x00,sizeof(esam_random_tmp));
//		sprintf(InfoTmp,"memset, ret = %d\n", ret);
//		print_info_Info(InfoTmp,strlen(InfoTmp),COM);
//
//		memcpy(esam_random_tmp,esam_random,4);
//		sprintf(InfoTmp,"memcpy ERR, ret = %d\n", ret);
//		print_info_Info(InfoTmp,strlen(InfoTmp),COM);
//
//		FD_TripleMAC(cur_key,esam_random_tmp,SourData_length,SourData_tmp,esam_mac);
//		sprintf(InfoTmp,"FD_TripleMAC, MAC = %X %X %X %X \n", esam_mac[0], esam_mac[1], esam_mac[2], esam_mac[3]);
//		print_info_Info(InfoTmp,strlen(InfoTmp),COM);

		ret = psamCalcEsamMac(psam_slot, offset, length, file, 0);
	}
	if(ret != 0)
	{
		return  -1 + ret*10;
	}
	//ϵͳ��Ϣ��־��0,��Ϊ���÷�ɢ���Ӳ�ͬ
	for(j = 0; j < 4; j++)
	{
		transfer_rq->apdu[i++] = esam_mac[j];	//MAC
	}
	//���˾���������ϰ�obu֮�⣬����obu����ʱ��
	//��дϵͳ��Ϣָ��Ƶ��ϴ�������Ϣ֡��(дϵͳ��Ϣָ��)  add by wll 2013-7-5 15:16:09
	memcpy(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand,&transfer_rq->apdu[i - 4 - length - 5],5 + length + 4);
	//��0
	memset(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[5 + length + 4],0x00,sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand) - (5 + length + 4));

	transfer_rq->frame_len = i;	//�����ܳ���
	return SUCCESS;
}

/*
* �������ƣ�	esamWriteSysInfo
* ����������	����ϵͳ��Ϣ�ļ�
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int esamWriteSysInfo_JuLi(ST_TRANSFER_CHANNEL *transfer_rq, uint8 offset, uint8 length, uint8 *file)
{
	uint8 i, j, ret;
	int psam_slot = g_issue_card;
	uint8 lenpos = 0;
	transfer_rq->apdulist++;	//��������+1
	i = transfer_rq->frame_len;
	//д�ļ�����
	lenpos = i ++;
	transfer_rq->apdu[i++] = 0x86;
	transfer_rq->apdu[i++] = 0xA5;
	transfer_rq->apdu[i++] = 0xB6;
	transfer_rq->apdu[i++] = offset;
	transfer_rq->apdu[i++] = length;
//	transfer_rq->apdu[i++] = 0x80;
//	transfer_rq->apdu[i++] = 0x00;

//	�����ϰ�OBUʱ,��¼���׼�¼���дϵͳ��Ϣ����� add by wll 2013-7-8 11:19:26
	memcpy(&g_frame_uploadtradeinfo_rq.WriteSysInfoRandom[0],esam_random,sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom));
	//����ESAM��ϵͳ��Ϣ�ļ�MAC
	ret = psamCalcEsamMac_JuLi(psam_slot, offset, length, file, 0);
	if(ret != 0)
		return -1 + ret*10;
	//ϵͳ��Ϣ��־��0,��Ϊ���÷�ɢ���Ӳ�ͬ
	for(j = 0; j < 4; j++)
	{
		transfer_rq->apdu[i++] = esam_mac[j];	//MAC
	}

	//д����
	for(j = 0; j < length; j++)
	{
		transfer_rq->apdu[i++] = file[j];
	}
	if(length%8 != 0)
	{
		transfer_rq->apdu[i++] = 0x80;
		length++;
	}
	for(;length%8 != 0;)
	{
		transfer_rq->apdu[i++] = 0x00;
		length++;
	}
	transfer_rq->apdu[lenpos] = length + 5 + 4;
// 	transfer_rq->apdu[i++] = 0xB6;
// 	transfer_rq->apdu[i++] = length + 9;	//�����files.Length + 9
// 	transfer_rq->apdu[i++] = 0x04;
// 	transfer_rq->apdu[i++] = 0xd6;
// 	transfer_rq->apdu[i++] = 0x81;
// 	transfer_rq->apdu[i++] = offset;		//ƫ����offset
// 	transfer_rq->apdu[i++] = length + 4;	//�����length + 4

// 	//����ESAM��ϵͳ��Ϣ�ļ�MAC
// 	ret = psamCalcEsamMac(psam_slot, offset, length, file, 0);
// 	//ϵͳ��Ϣ��־��0,��Ϊ���÷�ɢ���Ӳ�ͬ
// 	for(j = 0; j < 4; j++)
// 	{
// 		transfer_rq->apdu[i++] = esam_mac[j];	//MAC
// 	}

//	�����ϰ�OBUʱ,��¼���׼�¼���дϵͳ��Ϣָ�� add by wll 2013-7-8 11:19:26
	memcpy(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[0], &transfer_rq->apdu[i - (length + 5 + 4)], length + 5 + 4);
//	��0	
	memset(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[length + 5 + 4], 0x00, sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand) - (length + 5 + 4));

	transfer_rq->frame_len = i;	//�����ܳ���
	return SUCCESS;
}


int esamWriteSysInfo_JuLi_online(ST_TRANSFER_CHANNEL *transfer_rq, uint8 offset, uint8 length, uint8 *file)
{
	uint8 i, j, ret;
	int psam_slot = g_issue_card;
	uint8 lenpos = 0;
	transfer_rq->apdulist++;	//��������+1
	i = transfer_rq->frame_len;
	//д�ļ�����
	lenpos = i ++;
	transfer_rq->apdu[i++] = 0x86;
	transfer_rq->apdu[i++] = 0xA5;
	transfer_rq->apdu[i++] = 0xB6;
	transfer_rq->apdu[i++] = offset;
	transfer_rq->apdu[i++] = length;
//	transfer_rq->apdu[i++] = 0x80;
//	transfer_rq->apdu[i++] = 0x00;

//	�����ϰ�OBUʱ,��¼���׼�¼���дϵͳ��Ϣ����� add by wll 2013-7-8 11:19:26
	memcpy(&g_frame_uploadtradeinfo_rq.WriteSysInfoRandom[0],esam_random,sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom));
	//����ESAM��ϵͳ��Ϣ�ļ�MAC
	//д����

	for(j = 0; j < 4; j++)
	{
		transfer_rq->apdu[i++] = file[j+9];
	}

	for(j = 0; j < length; j++)
	{
		transfer_rq->apdu[i++] = file[j];
	}
//	if(length%8 != 0)
//	{
		transfer_rq->apdu[i++] = 0x80;
		length++;
//	}
	for(;length%8 != 0;)
	{
		transfer_rq->apdu[i++] = 0x00;
		length++;
	}
	transfer_rq->apdu[lenpos] = length + 5 + 4;
// 	transfer_rq->apdu[i++] = 0xB6;
// 	transfer_rq->apdu[i++] = length + 9;	//�����files.Length + 9
// 	transfer_rq->apdu[i++] = 0x04;
// 	transfer_rq->apdu[i++] = 0xd6;
// 	transfer_rq->apdu[i++] = 0x81;
// 	transfer_rq->apdu[i++] = offset;		//ƫ����offset
// 	transfer_rq->apdu[i++] = length + 4;	//�����length + 4

// 	//����ESAM��ϵͳ��Ϣ�ļ�MAC
// 	ret = psamCalcEsamMac(psam_slot, offset, length, file, 0);
// 	//ϵͳ��Ϣ��־��0,��Ϊ���÷�ɢ���Ӳ�ͬ
// 	for(j = 0; j < 4; j++)
// 	{
// 		transfer_rq->apdu[i++] = esam_mac[j];	//MAC
// 	}

//	�����ϰ�OBUʱ,��¼���׼�¼���дϵͳ��Ϣָ�� add by wll 2013-7-8 11:19:26
	memcpy(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[0], &transfer_rq->apdu[i - (length + 5 + 4)], length + 5 + 4);
//	��0
	memset(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[length + 5 + 4], 0x00, sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand) - (length + 5 + 4));

	transfer_rq->frame_len = i;	//�����ܳ���
	return SUCCESS;
}

/*
* �������ƣ�	esamReturnMainDir
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
void esamReturnMainDir(ST_TRANSFER_CHANNEL *transfer_rq)
{
	uint8 i, j, ret;
	transfer_rq->apdulist++;	//��������+1
	i = transfer_rq->frame_len;
	transfer_rq->apdu[i++] = 0x05;    //�����
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0xa4;
	transfer_rq->apdu[i++] = 0x00;				
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->frame_len = i;
}

/*
* �������ƣ�	esamReadSysInfoFrame
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
void esamReadSysInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 offset,uint8 length)
{
	uint8 i;
	transfer_rq->apdulist++;	//��������+1
	i = transfer_rq->frame_len;
	//���ļ�����
	transfer_rq->apdu[i++] = 0x05;	//�����
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0xb0;
	transfer_rq->apdu[i++] = 0x81;
	transfer_rq->apdu[i++] = offset;
	transfer_rq->apdu[i++] = length;
	//�����ܳ���
	transfer_rq->frame_len = i;
}

/*
* �������ƣ�	esamGetSn
* ����������	��ȡOBE-SAMоƬ���к�
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
void esamGetSn(ST_TRANSFER_CHANNEL *transfer_rq)
{
	uint8 i;
	transfer_rq->apdulist++;	//��������+1
	i = transfer_rq->frame_len;
	transfer_rq->apdu[i++] = 0x05;	//�����
	transfer_rq->apdu[i++] = 0x80;
	transfer_rq->apdu[i++] = 0xf6;
	transfer_rq->apdu[i++] = 0x00;				
	transfer_rq->apdu[i++] = 0x03;
	transfer_rq->apdu[i++] = 0x04;
	//�����ܳ���
	transfer_rq->frame_len = i;
}

/*
* �������ƣ�	esamCheck
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int esamCheck(uint8 *data,uint8 curlist)
{
	//return 0;
	uint8 i, len;
	uint8 *pEsamRet;
	pEsamRet = data;
	len = 0;
	for(i = 0; i < curlist; i++)
	{
		len += pEsamRet[len] + 1;
	}
	pEsamRet += len;	//�õ���ʼλ��
	len = pEsamRet[0];	//�õ������ܳ���
	if((pEsamRet[len - 1] == 0x90) && (pEsamRet[len] == 0x00))
	{
		return 0;
	}
	else
	{
	 
		return -1;
	}
}
/*
* �������ƣ�	esamCheck_JuLi
* ����������	������OBU��
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int esamCheck_JuLi_Acv(uint8 *data,uint8 curlist)
{
	uint8 i, len;
	uint8 *pEsamRet;
	pEsamRet = data;
	len = 0;
	for(i = 0; i < curlist; i++)
	{
		len += pEsamRet[len] + 1;
	}
	pEsamRet += len;	//�õ���ʼλ��
	len = pEsamRet[0];	//�õ������ܳ���
	if((pEsamRet[len - 1] == 0x83) && (pEsamRet[len] == 0x00))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
/*
	 * �������ƣ�	esamCheck_JuLi
	 * ����������	������OBU��
	 * �����б�	
	 * ���ؽ����	0	--	�ɹ�
	 *			��0	--	ʧ��
*/
int esamCheck_JuLi_2Issue(uint8 *data,uint8 curlist)
{
	uint8 i, len;
	uint8 *pEsamRet;
	pEsamRet = data;
	len = 0;
	for(i = 0; i < curlist; i++)
	{
		len += pEsamRet[len] + 1;
	}
	pEsamRet += len;	//�õ���ʼλ��
	len = pEsamRet[0];	//�õ������ܳ���
	if((pEsamRet[len - 1] == 0x86) && (pEsamRet[len] == 0x00))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
/*
* �������ƣ�	esamCheckGetRand
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int esamCheckGetRand(uint8 *data,uint8 curlist)
{
	uint8 i = 0, j = 0, len = 0;
	uint8 *pEsamRet;
	pEsamRet = data;
	for(i = 0; i < curlist; i++)
	{
		len += pEsamRet[len] + 1;
	}
	pEsamRet += len;	//�õ���ʼλ��
	len = pEsamRet[0];	//�õ������ܳ���
	if((pEsamRet[len - 1] == 0x90) && (pEsamRet[len] == 0x00))
	{
		//��������
		for(j = 0; j < 4; j++)
		{
			esam_random[j] = pEsamRet[j + 1];    //�洢�����
		}
		return SUCCESS;
	}
	else
	{
		return -1;
	}
}

/*
* �������ƣ�	
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
// ��ȡϵͳ�ļ�����
int esamCheckReadSysInfo(uint8 * data,uint8 curlist) 						
{
	uint8 i,j, len;
	uint8 *pEsamRet;

	pEsamRet = data;
	
	len = 0;
	for(i=0; i<curlist; i++)
		len += pEsamRet[len] + 1;
	pEsamRet += len; // �õ���ʼλ��
	len = pEsamRet[0]; // �õ������ܳ���
	
	if(pEsamRet[len-1] == 0x90 && pEsamRet[len] == 0x00)
	{
		// ��������
		for (j=0; j<g_read_file.len[0]; j++)
			esam_read_sysinfo[j] =  pEsamRet[j+1];

		return 0;
	}
	else 
		return -1;
}

/*
* �������ƣ�	
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int esamCheckGetSn(uint8 * data,uint8 curlist)
{
	uint8 i,j, len;
	uint8 *pEsamRet;

	pEsamRet = data;
	
	len = 0;
	for(i=0; i<curlist; i++)
		len += pEsamRet[len] + 1;
	pEsamRet += len; // �õ���ʼλ��
	len = pEsamRet[0]; // �õ������ܳ���

	if(pEsamRet[len-1] == 0x90 && pEsamRet[len] == 0x00)
	{
		// ��������
		for (j=0; j<4; j++)
		{
			EsamSn[j] = pEsamRet[j+1];    //�洢�����
		}
	}
	else 
		return -1;
	return 0;
}

/*
* �������ƣ�	
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
// ��ȡ�ļ�����
int esamCheckReadVehInfo(uint8 * data,uint8 curlist) 						
{
	uint8 i,j, len;
	uint8 *pEsamRet;

	pEsamRet = data;
	
	len = 0;
	for(i=0; i<curlist; i++)
		len += pEsamRet[len] + 1;
	pEsamRet += len; // �õ���ʼλ��
	len = pEsamRet[0]; // �õ������ܳ���

	//g_esam_ret[0] = pEsamRet[len-1];  //�����人½��OBU���
	//g_esam_ret[1] = pEsamRet[len];
	
	if(pEsamRet[len-1] == 0x90 && pEsamRet[len] == 0x00)
	{
		// ��������
		for (j=0; j<len-2; j++)
			esam_read_vehicle[j] =  pEsamRet[j+1];
	}
	else
	{
		return -1;
	}
	return 0;
}

/*
* �������ƣ�	
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
//��ϵͳ��Ϣ��������Ϣ
void esamReadFileInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 offset,uint8 length)
{
	uint8 i;
	
	transfer_rq->apdulist++; // ��������+1
	
	i = transfer_rq->frame_len;
	// ���ļ�����
	transfer_rq->apdu[i++] = 0x05; // �����
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0xb0;
	transfer_rq->apdu[i++] = 0x81;
	transfer_rq->apdu[i++] = offset;
	transfer_rq->apdu[i++] = length;
	
	// �����ܳ���
	transfer_rq->frame_len = i;
}

/*
* �������ƣ�	esamReadRecordInfoFrame
* ����������	
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
void esamReadRecordInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 record_num,uint8 length)
{
	uint8 i;
	transfer_rq->apdulist++;	//��������+1
	i = transfer_rq->frame_len;
	//���ļ�����
	transfer_rq->apdu[i++] = 0x05;	//�����
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0xb2;
	transfer_rq->apdu[i++] = record_num;	//�ڼ�����¼
	transfer_rq->apdu[i++] = 0x14;	//��ʾEF02���׼�¼�ļ�
	transfer_rq->apdu[i++] = length;
	//�����ܳ���
	transfer_rq->frame_len = i;
}

/*
* �������ƣ�	esamCheckReadFileInfo
* ����������	��ȡ�ļ�����
* �����б�	
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int esamCheckReadFileInfo(uint8 * data,uint8 curlist) 						
{
	uint8 i,j, len;
	uint8 *pEsamRet;
	pEsamRet = data;
	len = 0;
	for(i = 0; i < curlist; i++)
	{
		len += pEsamRet[len] + 1;
	}
	pEsamRet += len;	//�õ���ʼλ��
	len = pEsamRet[0];	//�õ������ܳ���
	if((pEsamRet[len - 1] == 0x90) && (pEsamRet[len] == 0x00))
	{
		//��������
		for(j = 0; j < len - 2; j++)
		{
			esam_read_info[j] =  pEsamRet[j + 1];
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

// ���泵����Ϣ�ļ�
int esamSaveVehicle(int len_vehicleinfo, uint8 *randRSU, uint8 *File,uint8 *psam_decrypted_data,uint8 *psam_decrypted_mac,uint8 *esam_encrypted_data)
{
	uint8 i;
//	//������
// 	for(i=0;i<8;i++)
// 	{
// 		psam_random[i] = randRSU[i];
// 	}

	if((g_OBU_ContractVersion == 0x00)||(g_OBU_ContractVersion == 0x10)||(g_OBU_ContractVersion == 0x11))
	{
		// ������������
// 		psam_decrypted_data[0] = len_vehicleinfo + 2;
		psam_decrypted_data[0] = len_vehicleinfo;
		for (i=0; i<len_vehicleinfo; i++)
		{
			psam_decrypted_data[i+1] = File[i];     //�洢8�ֽ�MAC+Ӧ�ó�����Ϣ�������� ����Э����8�ֽ�MAC�ڳ�����Ϣ�������ݺ���
		}		
//		for (i=0; i<8; i++)
//		{
//			psam_decrypted_mac[i] = File[i+len_vehicleinfo];     //�洢8�ֽ�MAC+Ӧ�ó�����Ϣ�������� ����Э����8�ֽ�MAC�ڳ�����Ϣ�������ݺ���
//		}
	}
	else if(g_OBU_ContractVersion == 0x12)
	{
		esam_encrypted_data[0] = 0x48;//pGS_rs->length;
		for (i=0; i<esam_encrypted_data[0]; i++)
		{
			esam_encrypted_data[i+1] = File[i];     //�洢��������
		}
	}
	return 0;
	/*
	// ���������
	rsuMACInit(); // �õ������õ���Կ
	rsuMAC(pGS_rq->randRSU); // ���������
	// �Ƚ�MAC�������
	if ((pGS_rs->authenticator[0] == psam_mac[0]) && (pGS_rs->authenticator[1] == psam_mac[1]) &&
		(pGS_rs->authenticator[2] == psam_mac[2]) && (pGS_rs->authenticator[3] == psam_mac[3]) &&
		(pGS_rs->authenticator[4] == psam_mac[4]) && (pGS_rs->authenticator[5] == psam_mac[5]) &&
		(pGS_rs->authenticator[6] == psam_mac[6]) && (pGS_rs->authenticator[7] == psam_mac[7]))
		return 0;
	else
		return 1;
	*/
}

/*
* �������ƣ�	esamWriteRecordInfoFrame
* ����������
* �����б�
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
void esamWriteRecordInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 record_num,uint8 length)
{
	uint8 i;
	int j = 0;
	transfer_rq->apdulist++;	//��������+1
	i = transfer_rq->frame_len;
	//д��¼��Ϣ
	transfer_rq->apdu[i++] = 5 + length;	//�����
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0xDC;
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0x10 + 0x03;	//��ʾEF02���׼�¼�ļ�
	transfer_rq->apdu[i++] = length;
	for(j = 0; j < length; j ++)
	{
		transfer_rq->apdu[i++] = 0x05;
	}
	//�����ܳ���
	transfer_rq->frame_len = i;
}


/*
* �������ƣ�	esamInitFrame
* ����������
* �����б�
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
void ledInitFrame(ST_TRANSFER_CHANNEL * transfer_rq,char ledid)
{
	transfer_rq->channelid = ledid;
	transfer_rq->apdulist = 0;
	transfer_rq->frame_len = 0;
	memset(transfer_rq->apdu,0,128);
}

/*
* �������ƣ�	esamReplaceKey
* ����������
* �����б�
* ���ؽ����	0	--	�ɹ�
*				��0	--	ʧ��
*/
void ledflash(ST_TRANSFER_CHANNEL *transfer_rq,char flashCount)
{
	uint8 len = 0, j = 0;
	transfer_rq->apdulist++;	//������+1
	len = transfer_rq->frame_len;
	transfer_rq->apdu[len++] = 1;	//�����
	transfer_rq->apdu[len++] = flashCount;	//���ƴ���
	transfer_rq->frame_len = len;


}


/*
* �������ƣ�
* ����������
* �����б�
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int ledflashCheck(uint8 * data,uint8 curlist)
{
	uint8 i,j, len;
	uint8 *pEsamRet;

	pEsamRet = data;

	len = 0;
	for(i=0; i<curlist; i++)
		len += pEsamRet[len] + 1;
	pEsamRet += len; // �õ���ʼλ��
	len = pEsamRet[0]; // �õ������ܳ���

	/*
	if(pEsamRet[len-1] == 0x90 && pEsamRet[len] == 0x00)
	{
		// ��������

	}
	*/
	if(pEsamRet[len] == 0x00)
	{
		// ��������

	}
	else
		return -1;
	return 0;
}


