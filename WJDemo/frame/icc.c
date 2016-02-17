#include "lib2hd.h"
#include "common.h"
#include "icc.h"
#include "psam.h"


/*
* 函数名称：iccInitFrame
* 功能描述：初始化IC命令
* 参数列表：*transfer_rq：命令结构
* 返回结果：
*/
void iccInitFrame(ST_TRANSFER_CHANNEL *transfer_rq)
{
	transfer_rq->channelid = 0x01;
	transfer_rq->apdulist = 0;
	transfer_rq->frame_len = 0;
	memset(transfer_rq->apdu, 0, MAX_ICC_APDU_LEN);
}

/*
* 函数名称：iccReadFileFrame
* 功能描述：构造读IC卡文件数据帧
* 参数列表：
* 返回结果：
*/
void iccReadFileFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 fid, uint8 offset, uint8 length)
{
	uint8 len = 0;
	uint8 i;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len;
	transfer_rq->apdu[len++] = 0x05;	//读文件内容
	switch(fid)
	{
		case 0x02:
		{
			//获取钱包文件
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
	transfer_rq->frame_len = len;	//设置总长度
	for(i = 0; i < len; i++){
		 
	}
}

/*
* 函数名称：iccCheck
* 功能描述：	
* 参数列表：
* 返回结果：
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
	pIccRet += len;	//得到开始位置
	len = pIccRet[0];	//得到数据总长度
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
* 函数名称：iccCheck
* 功能描述：
* 参数列表：
* 返回结果：
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
	pIccRet += len;	//得到开始位置
	len = pIccRet[0];	//得到数据总长度
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
* 函数名称：iccPayInitFrame
* 功能描述：消费初始化
* 参数列表：
* 返回结果：
*/
int iccPayInitFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 trans_mode, uint8 *pay_val)
{
	uint8 i;
	transfer_rq->apdulist++;	//命令数量+1
	i = transfer_rq->frame_len;
	//判越界
	if((transfer_rq->frame_len + 0x10) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	transfer_rq->apdu[i++] = 0x10;	//命令长度
	transfer_rq->apdu[i++] = 0x80;
	transfer_rq->apdu[i++] = 0x50;
	if(trans_mode == 0x00)
	{
		transfer_rq->apdu[i++] = 0x01;	//传统
	}
	if(trans_mode == 0x10)
		transfer_rq->apdu[i++] = 0x03;	//复合
	transfer_rq->apdu[i++] = 0x02;
	transfer_rq->apdu[i++] = 0x0b;
	transfer_rq->apdu[i++] = 0x01;	//1字节密钥标识
	//消费金额
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
	//终端设备号
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[0];
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[1];
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[2];
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[3];
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[4];
	transfer_rq->apdu[i++] = psam_pib.PSAMNO[5];
	transfer_rq->frame_len = i;	//设置总长度
	return SUCCESS;
}

/*
* 函数名称：iccUpdateBufFrame
* 功能描述：更新复合应用数据缓冲
* 参数列表：
* 返回结果：
*/
int iccUpdateBufFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 *station, uint8 *record0019)
{
	uint8 len = 0,j = 0;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len;
	//判越界
	if((transfer_rq->frame_len + 0x10) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	transfer_rq->apdu[len++] = record0019[1] + 2 + 5;	//指令长度
	transfer_rq->apdu[len++] = 0x80;
	transfer_rq->apdu[len++] = 0xdc;
	transfer_rq->apdu[len++] = record0019[0];
	transfer_rq->apdu[len++] = 0xc8;
	transfer_rq->apdu[len++] = record0019[1] + 2;
	//待更新内容
	transfer_rq->apdu[len++] = record0019[0];
	transfer_rq->apdu[len++] = record0019[1];	//记录长度
	transfer_rq->apdu[len++] = record0019[2];	//应用锁定标识
	for(j = 0; j < record0019[1] - 1; j++)
	{
		transfer_rq->apdu[len++] = station[j];	//36+4
	}
	transfer_rq->frame_len = len;	//设置总长度
	return SUCCESS;
}

/*
* 函数名称：iccPayFrame
* 功能描述：消费
* 参数列表：
* 返回结果：
*/
int iccPayFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 *time)
{
	uint8 len;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len;
	//判越界
	if((transfer_rq->frame_len + 0x10) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	transfer_rq->apdu[len++] = 0x14;	//数据长度
	transfer_rq->apdu[len++] = 0x80;
	transfer_rq->apdu[len++] = 0x54;	
	transfer_rq->apdu[len++] = 0x01;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x0f;
	//4字节终端交易序号
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
	//4字节MAC1
	transfer_rq->apdu[len++] = psam_pib.MAC1[0];
	transfer_rq->apdu[len++] = psam_pib.MAC1[1];
	transfer_rq->apdu[len++] = psam_pib.MAC1[2];
	transfer_rq->apdu[len++] = psam_pib.MAC1[3];
	transfer_rq->frame_len = len;	//设置总长度
	return SUCCESS;
}

/*
* 函数名称：iccReadMoneyFrame
* 功能描述：获取钱包余额
* 参数列表：
* 返回结果：
*/
int iccReadMoneyFrame(ST_TRANSFER_CHANNEL *transfer_rq)
{
	uint8 len;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len;
	//判越界
	if((transfer_rq->frame_len + 0x05) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	//获取钱包文件
	transfer_rq->apdu[len++] = 0x05;
	transfer_rq->apdu[len++] = 0x80;
	transfer_rq->apdu[len++] = 0x5c;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x02;
	transfer_rq->apdu[len++] = 0x04;
	transfer_rq->frame_len = len;	//设置总长度
	return SUCCESS;
}

/*
* 函数名称：iccinit_for_loadFrame
* 功能描述：圈存初始化
* 参数列表：
* 返回结果：
*/
int iccinit_for_loadFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 Addmoney[], uint8 zhongduanhao[], uint8 miyaotype, uint8 miyaoFlag)
{
	uint8 len;
	int i;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len;
	//判越界
	if((transfer_rq->frame_len + 0x10) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	//圈存初始化
	transfer_rq->apdu[len++] = 0x10;
	//apdu指令
	transfer_rq->apdu[len++] = 0x80;
	transfer_rq->apdu[len++] = 0x50;
	transfer_rq->apdu[len++] = 0x00;
	//交易类型
	transfer_rq->apdu[len++] = miyaotype;
	//内容长度
	transfer_rq->apdu[len++] = 0x0B;
	//交易标识
	transfer_rq->apdu[len++] = miyaoFlag;
	//交易金额
	for(i = 0; i < 4; i ++)
		transfer_rq->apdu[len++] = Addmoney[i];
	//终端机编号
	for(i = 0; i < 6; i ++)
		transfer_rq->apdu[len++] = zhongduanhao[i];

	transfer_rq->frame_len = len;	//设置总长度
	return SUCCESS;
}

int icc_enter_dir(ST_TRANSFER_CHANNEL *transfer_rq, uint16 did)
{
	uint8 len;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len;
	//判越界
	if((transfer_rq->frame_len + 0x07) > MAX_ICC_APDU_LEN){
		return -1;
	}

	//进1001目录指令
	transfer_rq->apdu[len++] = 0x07;//第一条指令长度
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0xa4;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x02;
	transfer_rq->apdu[len++] = (did >> 8) & 0xff;
	transfer_rq->apdu[len++] = did & 0xff;

	transfer_rq->frame_len = len;	//设置总长度
	return SUCCESS;
}

int icc_check_Pin(ST_TRANSFER_CHANNEL *transfer_rq, uint16 datalen, uint8 data[])
{
	int i;
	uint8 len;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len;
	//判越界
	if((transfer_rq->frame_len + 0x07) > MAX_ICC_APDU_LEN){
		return -1;
	}
	//
	transfer_rq->apdu[len++] = 5 + datalen;//第一条指令长度
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x20;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = datalen;
	for (i = 0; i < datalen; i ++)
	{
		transfer_rq->apdu[len++] = data[i];
	}
	transfer_rq->frame_len = len;	//设置总长度
	return SUCCESS;
}

int icc_select_file(ST_TRANSFER_CHANNEL *transfer_rq, uint8 data[], uint16 datalen)
{
	int i = 0;
	uint8 len;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len;
	//判越界
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
//	//进1001目录指令
//	transfer_rq->apdu[len++] = 0x07;//第一条指令长度
//	transfer_rq->apdu[len++] = 0x00;
//	transfer_rq->apdu[len++] = 0xa4;
//	transfer_rq->apdu[len++] = 0x04;
//	transfer_rq->apdu[len++] = 0x00;
//	transfer_rq->apdu[len++] = 0x02;
//	transfer_rq->apdu[len++] = (did >> 8) & 0xff;
//	transfer_rq->apdu[len++] = did & 0xff;

	transfer_rq->frame_len = len;	//设置总长度
	return SUCCESS;
}

int icc_loadFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 dealtime[], uint8 mac2[])//圈存
{
	uint8 len;
	int i;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len;
	//判越界
	if((transfer_rq->frame_len + 0x10) > MAX_ICC_APDU_LEN)
	{
		return -1;
	}
	//圈存
	transfer_rq->apdu[len++] = 0x10;
	//apdu指令
	transfer_rq->apdu[len++] = 0x80;
	transfer_rq->apdu[len++] = 0x52;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x00;
	//内容长度
	transfer_rq->apdu[len++] = 0x0B;
	//时间
	for(i = 0; i < 7; i ++)
		transfer_rq->apdu[len++] = dealtime[i];
	//MAC2
	for(i = 0; i < 4; i ++)
		transfer_rq->apdu[len++] = mac2[i];
	transfer_rq->frame_len = len;	//设置总长度
	return SUCCESS;
}

int icc_getOneDispersed(ST_TRANSFER_CHANNEL *transfer_rq)
{
	int i = 0;
	uint8 len;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len;
	//判越界
	if((transfer_rq->frame_len + 0x05) > MAX_ICC_APDU_LEN){
		return -1;
	}
	transfer_rq->apdu[len++] = 0x05;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0xb0;
	transfer_rq->apdu[len++] = 0x95;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x08;

	transfer_rq->frame_len = len;	//设置总长度
	return SUCCESS;
}

int icc_getTwoDispersed(ST_TRANSFER_CHANNEL *transfer_rq)
{
	int i = 0;
	uint8 len;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len;
	//判越界
	if((transfer_rq->frame_len + 0x06) > MAX_ICC_APDU_LEN){
		return -1;
	}
	transfer_rq->apdu[len++] = 0x05;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0xb0;
	transfer_rq->apdu[len++] = 0x95;
	transfer_rq->apdu[len++] = 0x0C;
	transfer_rq->apdu[len++] = 0x08;

	transfer_rq->frame_len = len;	//设置总长度
	return SUCCESS;
}
