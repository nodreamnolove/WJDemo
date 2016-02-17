#include "lib2hd.h"
#include "common.h"
#include "icc.h"
#include "psam.h"


/*
* �������ƣ�iccInitFrame
* ������������ʼ��IC����
* �����б�*transfer_rq������ṹ
* ���ؽ����
*/
void iccInitFrame(ST_TRANSFER_CHANNEL *transfer_rq)
{
	transfer_rq->channelid = 0x01;
	transfer_rq->apdulist = 0;
	transfer_rq->frame_len = 0;
	memset(transfer_rq->apdu, 0, MAX_ICC_APDU_LEN);
}

/*
* �������ƣ�iccReadFileFrame
* ���������������IC���ļ�����֡
* �����б�
* ���ؽ����
*/
void iccReadFileFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 fid, uint8 offset, uint8 length)
{
	uint8 len = 0;
	uint8 i;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len;
	transfer_rq->apdu[len++] = 0x05;	//���ļ�����
	switch(fid)
	{
		case 0x02:
		{
			//��ȡǮ���ļ�
			transfer_rq->apdu[len++] = 0x80;
			transfer_rq->apdu[len++] = 0x5c;
			transfer_rq->apdu[len++] = 0x00;
		}
		break;
		case 0x12:
		{
			transfer_rq->apdu[len++] = 0x00;
			transfer_rq->apdu[len++] = 0xb0;
			transfer_rq->apdu[len++] = 0x92;
		}
		break;
		case 0x15:
		{
			transfer_rq->apdu[len++] = 0x00;
			transfer_rq->apdu[len++] = 0xb0;
			transfer_rq->apdu[len++] = 0x95;
		}
		break;
		case 0x18:
		{
			transfer_rq->apdu[len++] = 0x00;
			transfer_rq->apdu[len++] = 0xb2;
			transfer_rq->apdu[len++] = 0x01;
		}
		break;
		case 0x16:
		{
			transfer_rq->apdu[len++] = 0x00;
			transfer_rq->apdu[len++] = 0xb0;
			transfer_rq->apdu[len++] = 0x96;
		}
		break;
		case 0x19:
		{
			transfer_rq->apdu[len++] = 0x00;
			transfer_rq->apdu[len++] = 0xb2;
			transfer_rq->apdu[len++] = 0x01;
		}
		break;
		default:
		{
			/* do nothing */
		}
		break;
	}/* end switch(fid) */
	transfer_rq->apdu[len++] = offset;
	transfer_rq->apdu[len++] = length;
	transfer_rq->frame_len = len;	//�����ܳ���
	for(i = 0; i < len; i++){
		 
	}
}

/*
* �������ƣ�iccCheck
* ����������	
* �����б�
* ���ؽ����
*/
int iccCheck(uint8 *data,uint8 curlist)
{
	uint8 i = 0, len = 0;
	uint8 *pIccRet;
	pIccRet = data;
	for(i = 0; i < curlist; i++)
	{
		len += pIccRet[len] + 1;
	}
	pIccRet += len;	//�õ���ʼλ��
	len = pIccRet[0];	//�õ������ܳ���
	if((pIccRet[len - 1] == 0x90) && (pIccRet[len] == 0x00))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
/*
* �������ƣ�iccCheck
* ����������
* �����б�
* ���ؽ����
*/
int iccCheckandGetData(uint8 *data, uint8 curlist, uint8 *resultInfo)
{
	uint8 i = 0, len = 0;
	uint8 *pIccRet;
	pIccRet = data;
	for(i = 0; i < curlist; i++)
	{
		len += pIccRet[len] + 1;
	}
	pIccRet += len;	//�õ���ʼλ��
	len = pIccRet[0];	//�õ������ܳ���
	if((pIccRet[len - 1] == 0x90) && (pIccRet[len] == 0x00))
	{
		for(i = 0; i < 10; i ++)
		{
			resultInfo[i] = pIccRet[1+i];
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

/*
* �������ƣ�iccPayInitFrame
* �������������ѳ�ʼ��
* �����б�
* ���ؽ����
*/
int iccPayInitFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 trans_mode, uint8 *pay_val)
{
	uint8 i;
	transfer_rq->apdulist++;	//��������+1
	i = transfer_rq->frame_len;
	//��Խ��
	if((transfer_rq->frame_len + 0x10) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	transfer_rq->apdu[i++] = 0x10;	//�����
	transfer_rq->apdu[i++] = 0x80;
	transfer_rq->apdu[i++] = 0x50;
	if(trans_mode == 0x00)
	{
		transfer_rq->apdu[i++] = 0x01;	//��ͳ
	}
	if(trans_mode == 0x10)
		transfer_rq->apdu[i++] = 0x03;	//����
	transfer_rq->apdu[i++] = 0x02;
	transfer_rq->apdu[i++] = 0x0b;
	transfer_rq->apdu[i++] = 0x01;	//1�ֽ���Կ��ʶ
	//���ѽ��
	if(pay_val!=NULL)
	{
		transfer_rq->apdu[i++] = pay_val[0];
		transfer_rq->apdu[i++] = pay_val[1];
		transfer_rq->apdu[i++] = pay_val[2];
		transfer_rq->apdu[i++] = pay_val[3];
	}
	else
	{
		transfer_rq->apdu[i++] = 0x00;
		transfer_rq->apdu[i++] = 0x00;
		transfer_rq->apdu[i++] = 0x00;
		transfer_rq->apdu[i++] = 0x00;
	}
	//�ն��豸��
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[0];
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[1];
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[2];
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[3];
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[4];
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[5];
	transfer_rq->frame_len = i;	//�����ܳ���
	return SUCCESS;
}

/*
* �������ƣ�iccUpdateBufFrame
* �������������¸���Ӧ�����ݻ���
* �����б�
* ���ؽ����
*/
int iccUpdateBufFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 *station, uint8 *record0019)
{
	uint8 len = 0,j = 0;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len;
	//��Խ��
	if((transfer_rq->frame_len + 0x10) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	transfer_rq->apdu[len++] = record0019[1] + 2 + 5;	//ָ���
	transfer_rq->apdu[len++] = 0x80;
	transfer_rq->apdu[len++] = 0xdc;
	transfer_rq->apdu[len++] = record0019[0];
	transfer_rq->apdu[len++] = 0xc8;
	transfer_rq->apdu[len++] = record0019[1] + 2;
	//����������
	transfer_rq->apdu[len++] = record0019[0];
	transfer_rq->apdu[len++] = record0019[1];	//��¼����
	transfer_rq->apdu[len++] = record0019[2];	//Ӧ��������ʶ
	for(j = 0; j < record0019[1] - 1; j++)
	{
		transfer_rq->apdu[len++] = station[j];	//36+4
	}
	transfer_rq->frame_len = len;	//�����ܳ���
	return SUCCESS;
}

/*
* �������ƣ�iccPayFrame
* ��������������
* �����б�
* ���ؽ����
*/
int iccPayFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 *time)
{
	uint8 len;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len;
	//��Խ��
	if((transfer_rq->frame_len + 0x10) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	transfer_rq->apdu[len++] = 0x14;	//���ݳ���
	transfer_rq->apdu[len++] = 0x80;
	transfer_rq->apdu[len++] = 0x54;	
	transfer_rq->apdu[len++] = 0x01;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x0f;
	//4�ֽ��ն˽������
	transfer_rq->apdu[len++] = psam_pib.TransSerial[0];
	transfer_rq->apdu[len++] = psam_pib.TransSerial[1];
	transfer_rq->apdu[len++] = psam_pib.TransSerial[2];
	transfer_rq->apdu[len++] = psam_pib.TransSerial[3];

	transfer_rq->apdu[len++] = time[0];
	transfer_rq->apdu[len++] = time[1];
	transfer_rq->apdu[len++] = time[2];
	transfer_rq->apdu[len++] = time[3];
	transfer_rq->apdu[len++] = time[4];
	transfer_rq->apdu[len++] = time[5];
	transfer_rq->apdu[len++] = time[6];
	//4�ֽ�MAC1
	transfer_rq->apdu[len++] = psam_pib.MAC1[0];
	transfer_rq->apdu[len++] = psam_pib.MAC1[1];
	transfer_rq->apdu[len++] = psam_pib.MAC1[2];
	transfer_rq->apdu[len++] = psam_pib.MAC1[3];
	transfer_rq->frame_len = len;	//�����ܳ���
	return SUCCESS;
}

/*
* �������ƣ�iccReadMoneyFrame
* ������������ȡǮ�����
* �����б�
* ���ؽ����
*/
int iccReadMoneyFrame(ST_TRANSFER_CHANNEL *transfer_rq)
{
	uint8 len;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len;
	//��Խ��
	if((transfer_rq->frame_len + 0x05) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	//��ȡǮ���ļ�
	transfer_rq->apdu[len++] = 0x05;
	transfer_rq->apdu[len++] = 0x80;
	transfer_rq->apdu[len++] = 0x5c;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x02;
	transfer_rq->apdu[len++] = 0x04;
	transfer_rq->frame_len = len;	//�����ܳ���
	return SUCCESS;
}

/*
* �������ƣ�iccinit_for_loadFrame
* ����������Ȧ���ʼ��
* �����б�
* ���ؽ����
*/
int iccinit_for_loadFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 Addmoney[], uint8 zhongduanhao[], uint8 miyaotype, uint8 miyaoFlag)
{
	uint8 len;
	int i;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len;
	//��Խ��
	if((transfer_rq->frame_len + 0x10) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	//Ȧ���ʼ��
	transfer_rq->apdu[len++] = 0x10;
	//apduָ��
	transfer_rq->apdu[len++] = 0x80;
	transfer_rq->apdu[len++] = 0x50;
	transfer_rq->apdu[len++] = 0x00;
	//��������
	transfer_rq->apdu[len++] = miyaotype;
	//���ݳ���
	transfer_rq->apdu[len++] = 0x0B;
	//���ױ�ʶ
	transfer_rq->apdu[len++] = miyaoFlag;
	//���׽��
	for(i = 0; i < 4; i ++)
		transfer_rq->apdu[len++] = Addmoney[i];
	//�ն˻����
	for(i = 0; i < 6; i ++)
		transfer_rq->apdu[len++] = zhongduanhao[i];

	transfer_rq->frame_len = len;	//�����ܳ���
	return SUCCESS;
}

int icc_enter_dir(ST_TRANSFER_CHANNEL *transfer_rq, uint16 did)
{
	uint8 len;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len;
	//��Խ��
	if((transfer_rq->frame_len + 0x07) > MAX_ICC_APDU_LEN){
		return -1;
	}

	//��1001Ŀ¼ָ��
	transfer_rq->apdu[len++] = 0x07;//��һ��ָ���
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0xa4;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x02;
	transfer_rq->apdu[len++] = (did >> 8) & 0xff;
	transfer_rq->apdu[len++] = did & 0xff;

	transfer_rq->frame_len = len;	//�����ܳ���
	return SUCCESS;
}

int icc_check_Pin(ST_TRANSFER_CHANNEL *transfer_rq, uint16 datalen, uint8 data[])
{
	int i;
	uint8 len;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len;
	//��Խ��
	if((transfer_rq->frame_len + 0x07) > MAX_ICC_APDU_LEN){
		return -1;
	}
	//
	transfer_rq->apdu[len++] = 5 + datalen;//��һ��ָ���
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x20;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = datalen;
	for (i = 0; i < datalen; i ++)
	{
		transfer_rq->apdu[len++] = data[i];
	}
	transfer_rq->frame_len = len;	//�����ܳ���
	return SUCCESS;
}

int icc_select_file(ST_TRANSFER_CHANNEL *transfer_rq, uint8 data[], uint16 datalen)
{
	int i = 0;
	uint8 len;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len;
	//��Խ��
	if((transfer_rq->frame_len + 0x0E) > MAX_ICC_APDU_LEN){
		return -1;
	}

	uint8 aaa[100]= {0x00, 0xA4, 0x04, 0x00, 0x09,
				0xA0, 0x00, 0x00, 0x00, 0x03,
				0x86, 0x98, 0x07, 0x02};
	transfer_rq->apdu[len++] = 0x0E;
	for (i = 0; i < 14; i ++)
	{
		transfer_rq->apdu[len++] = aaa[i];
	}
//	//��1001Ŀ¼ָ��
//	transfer_rq->apdu[len++] = 0x07;//��һ��ָ���
//	transfer_rq->apdu[len++] = 0x00;
//	transfer_rq->apdu[len++] = 0xa4;
//	transfer_rq->apdu[len++] = 0x04;
//	transfer_rq->apdu[len++] = 0x00;
//	transfer_rq->apdu[len++] = 0x02;
//	transfer_rq->apdu[len++] = (did >> 8) & 0xff;
//	transfer_rq->apdu[len++] = did & 0xff;

	transfer_rq->frame_len = len;	//�����ܳ���
	return SUCCESS;
}

int icc_loadFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 dealtime[], uint8 mac2[])//Ȧ��
{
	uint8 len;
	int i;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len;
	//��Խ��
	if((transfer_rq->frame_len + 0x10) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	//Ȧ��
	transfer_rq->apdu[len++] = 0x10;
	//apduָ��
	transfer_rq->apdu[len++] = 0x80;
	transfer_rq->apdu[len++] = 0x52;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x00;
	//���ݳ���
	transfer_rq->apdu[len++] = 0x0B;
	//ʱ��
	for(i = 0; i < 7; i ++)
		transfer_rq->apdu[len++] = dealtime[i];
	//MAC2
	for(i = 0; i < 4; i ++)
		transfer_rq->apdu[len++] = mac2[i];
	transfer_rq->frame_len = len;	//�����ܳ���
	return SUCCESS;
}

int icc_getOneDispersed(ST_TRANSFER_CHANNEL *transfer_rq)
{
	int i = 0;
	uint8 len;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len;
	//��Խ��
	if((transfer_rq->frame_len + 0x05) > MAX_ICC_APDU_LEN){
		return -1;
	}
	transfer_rq->apdu[len++] = 0x05;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0xb0;
	transfer_rq->apdu[len++] = 0x95;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x08;

	transfer_rq->frame_len = len;	//�����ܳ���
	return SUCCESS;
}

int icc_getTwoDispersed(ST_TRANSFER_CHANNEL *transfer_rq)
{
	int i = 0;
	uint8 len;
	transfer_rq->apdulist++;	//��������+1
	len = transfer_rq->frame_len;
	//��Խ��
	if((transfer_rq->frame_len + 0x06) > MAX_ICC_APDU_LEN){
		return -1;
	}
	transfer_rq->apdu[len++] = 0x05;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0xb0;
	transfer_rq->apdu[len++] = 0x95;
	transfer_rq->apdu[len++] = 0x0C;
	transfer_rq->apdu[len++] = 0x08;

	transfer_rq->frame_len = len;	//�����ܳ���
	return SUCCESS;
}
