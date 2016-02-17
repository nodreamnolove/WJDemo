#include "lib2hd.h"
#include "common.h"
#include "esam.h"
#include "psam.h"

 
/*
 * 函数名称：	esamInitFrame
 * 功能描述：
 * 参数列表：
 * 返回结果：	0	--	成功
 *			非0	--	失败
 */
void esamInitFrame(ST_TRANSFER_CHANNEL * transfer_rq)
{
    transfer_rq->channelid = 0x02;
    transfer_rq->apdulist = 0;
    transfer_rq->frame_len = 0;
	memset(transfer_rq->apdu,0,128);
}

/*
* 函数名称：	esamReplaceKey
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*				非0	--	失败
*/
void esamReplaceKey(ST_TRANSFER_CHANNEL *transfer_rq, uint8 flag)
{
	uint8 len = 0, j = 0;
	transfer_rq->apdulist++;	//命令数+1
	len = transfer_rq->frame_len;
	transfer_rq->apdu[len++] = 0x21;	//命令长度
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
* 函数名称：	esamGetRandFrame
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
void esamGetRandFrame(ST_TRANSFER_CHANNEL *transfer_rq)
{
	uint8 len = 0;
	transfer_rq->apdulist++;	//命令数量+1
	len = transfer_rq->frame_len; 
	//进入该文件
	transfer_rq->apdu[len++] = 0x05;	//命令长度
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x84;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x00;
	transfer_rq->apdu[len++] = 0x04;
	transfer_rq->frame_len = len;	//设置总长度
}

/*
* 函数名称：	esamEnterDirFrame
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
void esamEnterDirFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint16 did)
{
	uint8 len = 0;
	transfer_rq->apdulist++;	//命令数+1
	len = transfer_rq->frame_len;
	transfer_rq->apdu[len++] = 0x07;	//命令长度
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
* 函数名称：	esamWriteVehInfo
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int	esamWriteVehInfo(ST_TRANSFER_CHANNEL *transfer_rq, uint8 offset, uint8 length, uint8 *file)
{
	uint8 i = 0, j = 0, ret = 0;
	int psam_slot = g_issue_card;
	transfer_rq->apdulist++;	//命令数量+1
	i = transfer_rq->frame_len;
	//写文件内容
	transfer_rq->apdu[i++] = length + 9;	//命令长度files.Length + 9
	transfer_rq->apdu[i++] = 0x04;
	transfer_rq->apdu[i++] = 0xd6;
	transfer_rq->apdu[i++] = 0x81;
	transfer_rq->apdu[i++] = offset;	//偏移量offset
	transfer_rq->apdu[i++] = length + 4;	//命令长度length + 4
	//写数据
	for(j = 0; j < length; j++)
	{
	    transfer_rq->apdu[i++] = file[j];
	}
	//把写车辆信息随机数复制到上传交易信息帧中(写车辆信息随机数)  add by wll 2013-7-5 15:16:09
	memcpy(&g_frame_uploadtradeinfo_rq.WriteVehInfoRandom[0],&esam_random[0],sizeof(g_frame_uploadtradeinfo_rq.WriteVehInfoRandom));

	//计算ESAM卡系统信息文件MAC 
	ret = psamCalcEsamMac(psam_slot, offset, length, file, 1);	//车辆信息标志是1,因为所用分散因子不同

	if(ret != 0)
	{
		return -1 + ret*10;
	}
	for(j = 0; j < 4; j++)
	{
		transfer_rq->apdu[i++] = esam_mac[j];	//MAC
	}
	//把写车辆信息指令复制到上传交易信息帧中(写车辆信息指令)  add by wll 2013-7-5 15:16:09
	memcpy(g_frame_uploadtradeinfo_rq.WriteVehInfoCommand,&transfer_rq->apdu[i - 4 - length - 5],5 + length + 4);
	//补0
	memset(&g_frame_uploadtradeinfo_rq.WriteVehInfoCommand[5 + length + 4],0x00,sizeof(g_frame_uploadtradeinfo_rq.WriteVehInfoCommand) - (5 + length + 4));

	//设置总长度
	transfer_rq->frame_len = i;
	return 0;
}

/*
* 函数名称：	esamWriteSysInfo
* 功能描述：	更新系统信息文件
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
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
	transfer_rq->apdulist++;	//命令数量+1
	i = transfer_rq->frame_len;
	//写文件内容
	transfer_rq->apdu[i++] = length + 9;	//命令长度files.Length + 9
	transfer_rq->apdu[i++] = 0x04;
	transfer_rq->apdu[i++] = 0xd6;
	transfer_rq->apdu[i++] = 0x81;
	transfer_rq->apdu[i++] = offset;		//偏移量offset
	transfer_rq->apdu[i++] = length + 4;	//命令长度length + 4
	//写数据
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
	//除了聚利金溢的老版obu之外，其他obu交易时，把写系统信息随机数赋给上传交易信息帧中的WriteSysInfoRandom
	memcpy(&g_frame_uploadtradeinfo_rq.WriteSysInfoRandom[0],&esam_random[0],sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom));
	//计算ESAM卡系统信息文件MAC
	if (g_OBU_ContractVersion == 0X16)//2版OBU直接给psam数据取MAC
	{
		ret = psamCalcEsamMac(psam_slot, offset, length, file, 0);
	}
	else	//1版OBU从PSAM卡中取秘钥使用FD_TripleMAC算法计算MAC
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
	//系统信息标志是0,因为所用分散因子不同
	for(j = 0; j < 4; j++)
	{
		transfer_rq->apdu[i++] = esam_mac[j];	//MAC
	}
	//除了聚利金溢的老版obu之外，其他obu交易时，
	//把写系统信息指令复制到上传交易信息帧中(写系统信息指令)  add by wll 2013-7-5 15:16:09
	memcpy(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand,&transfer_rq->apdu[i - 4 - length - 5],5 + length + 4);
	//补0
	memset(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[5 + length + 4],0x00,sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand) - (5 + length + 4));

	transfer_rq->frame_len = i;	//设置总长度
	return SUCCESS;
}

/*
* 函数名称：	esamWriteSysInfo
* 功能描述：	更新系统信息文件
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int esamWriteSysInfo_JuLi(ST_TRANSFER_CHANNEL *transfer_rq, uint8 offset, uint8 length, uint8 *file)
{
	uint8 i, j, ret;
	int psam_slot = g_issue_card;
	uint8 lenpos = 0;
	transfer_rq->apdulist++;	//命令数量+1
	i = transfer_rq->frame_len;
	//写文件内容
	lenpos = i ++;
	transfer_rq->apdu[i++] = 0x86;
	transfer_rq->apdu[i++] = 0xA5;
	transfer_rq->apdu[i++] = 0xB6;
	transfer_rq->apdu[i++] = offset;
	transfer_rq->apdu[i++] = length;
//	transfer_rq->apdu[i++] = 0x80;
//	transfer_rq->apdu[i++] = 0x00;

//	聚利老版OBU时,记录交易记录里的写系统信息随机数 add by wll 2013-7-8 11:19:26
	memcpy(&g_frame_uploadtradeinfo_rq.WriteSysInfoRandom[0],esam_random,sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom));
	//计算ESAM卡系统信息文件MAC
	ret = psamCalcEsamMac_JuLi(psam_slot, offset, length, file, 0);
	if(ret != 0)
		return -1 + ret*10;
	//系统信息标志是0,因为所用分散因子不同
	for(j = 0; j < 4; j++)
	{
		transfer_rq->apdu[i++] = esam_mac[j];	//MAC
	}

	//写数据
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
// 	transfer_rq->apdu[i++] = length + 9;	//命令长度files.Length + 9
// 	transfer_rq->apdu[i++] = 0x04;
// 	transfer_rq->apdu[i++] = 0xd6;
// 	transfer_rq->apdu[i++] = 0x81;
// 	transfer_rq->apdu[i++] = offset;		//偏移量offset
// 	transfer_rq->apdu[i++] = length + 4;	//命令长度length + 4

// 	//计算ESAM卡系统信息文件MAC
// 	ret = psamCalcEsamMac(psam_slot, offset, length, file, 0);
// 	//系统信息标志是0,因为所用分散因子不同
// 	for(j = 0; j < 4; j++)
// 	{
// 		transfer_rq->apdu[i++] = esam_mac[j];	//MAC
// 	}

//	聚利老版OBU时,记录交易记录里的写系统信息指令 add by wll 2013-7-8 11:19:26
	memcpy(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[0], &transfer_rq->apdu[i - (length + 5 + 4)], length + 5 + 4);
//	补0	
	memset(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[length + 5 + 4], 0x00, sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand) - (length + 5 + 4));

	transfer_rq->frame_len = i;	//设置总长度
	return SUCCESS;
}


int esamWriteSysInfo_JuLi_online(ST_TRANSFER_CHANNEL *transfer_rq, uint8 offset, uint8 length, uint8 *file)
{
	uint8 i, j, ret;
	int psam_slot = g_issue_card;
	uint8 lenpos = 0;
	transfer_rq->apdulist++;	//命令数量+1
	i = transfer_rq->frame_len;
	//写文件内容
	lenpos = i ++;
	transfer_rq->apdu[i++] = 0x86;
	transfer_rq->apdu[i++] = 0xA5;
	transfer_rq->apdu[i++] = 0xB6;
	transfer_rq->apdu[i++] = offset;
	transfer_rq->apdu[i++] = length;
//	transfer_rq->apdu[i++] = 0x80;
//	transfer_rq->apdu[i++] = 0x00;

//	聚利老版OBU时,记录交易记录里的写系统信息随机数 add by wll 2013-7-8 11:19:26
	memcpy(&g_frame_uploadtradeinfo_rq.WriteSysInfoRandom[0],esam_random,sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom));
	//计算ESAM卡系统信息文件MAC
	//写数据

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
// 	transfer_rq->apdu[i++] = length + 9;	//命令长度files.Length + 9
// 	transfer_rq->apdu[i++] = 0x04;
// 	transfer_rq->apdu[i++] = 0xd6;
// 	transfer_rq->apdu[i++] = 0x81;
// 	transfer_rq->apdu[i++] = offset;		//偏移量offset
// 	transfer_rq->apdu[i++] = length + 4;	//命令长度length + 4

// 	//计算ESAM卡系统信息文件MAC
// 	ret = psamCalcEsamMac(psam_slot, offset, length, file, 0);
// 	//系统信息标志是0,因为所用分散因子不同
// 	for(j = 0; j < 4; j++)
// 	{
// 		transfer_rq->apdu[i++] = esam_mac[j];	//MAC
// 	}

//	聚利老版OBU时,记录交易记录里的写系统信息指令 add by wll 2013-7-8 11:19:26
	memcpy(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[0], &transfer_rq->apdu[i - (length + 5 + 4)], length + 5 + 4);
//	补0
	memset(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[length + 5 + 4], 0x00, sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand) - (length + 5 + 4));

	transfer_rq->frame_len = i;	//设置总长度
	return SUCCESS;
}

/*
* 函数名称：	esamReturnMainDir
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
void esamReturnMainDir(ST_TRANSFER_CHANNEL *transfer_rq)
{
	uint8 i, j, ret;
	transfer_rq->apdulist++;	//命令数量+1
	i = transfer_rq->frame_len;
	transfer_rq->apdu[i++] = 0x05;    //命令长度
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0xa4;
	transfer_rq->apdu[i++] = 0x00;				
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->frame_len = i;
}

/*
* 函数名称：	esamReadSysInfoFrame
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
void esamReadSysInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 offset,uint8 length)
{
	uint8 i;
	transfer_rq->apdulist++;	//命令数量+1
	i = transfer_rq->frame_len;
	//读文件内容
	transfer_rq->apdu[i++] = 0x05;	//命令长度
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0xb0;
	transfer_rq->apdu[i++] = 0x81;
	transfer_rq->apdu[i++] = offset;
	transfer_rq->apdu[i++] = length;
	//设置总长度
	transfer_rq->frame_len = i;
}

/*
* 函数名称：	esamGetSn
* 功能描述：	获取OBE-SAM芯片序列号
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
void esamGetSn(ST_TRANSFER_CHANNEL *transfer_rq)
{
	uint8 i;
	transfer_rq->apdulist++;	//命令数量+1
	i = transfer_rq->frame_len;
	transfer_rq->apdu[i++] = 0x05;	//命令长度
	transfer_rq->apdu[i++] = 0x80;
	transfer_rq->apdu[i++] = 0xf6;
	transfer_rq->apdu[i++] = 0x00;				
	transfer_rq->apdu[i++] = 0x03;
	transfer_rq->apdu[i++] = 0x04;
	//设置总长度
	transfer_rq->frame_len = i;
}

/*
* 函数名称：	esamCheck
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
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
	pEsamRet += len;	//得到开始位置
	len = pEsamRet[0];	//得到数据总长度
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
* 函数名称：	esamCheck_JuLi
* 功能描述：	检查聚利OBU是
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
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
	pEsamRet += len;	//得到开始位置
	len = pEsamRet[0];	//得到数据总长度
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
	 * 函数名称：	esamCheck_JuLi
	 * 功能描述：	检查聚利OBU是
	 * 参数列表：	
	 * 返回结果：	0	--	成功
	 *			非0	--	失败
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
	pEsamRet += len;	//得到开始位置
	len = pEsamRet[0];	//得到数据总长度
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
* 函数名称：	esamCheckGetRand
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
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
	pEsamRet += len;	//得到开始位置
	len = pEsamRet[0];	//得到数据总长度
	if((pEsamRet[len - 1] == 0x90) && (pEsamRet[len] == 0x00))
	{
		//保存数据
		for(j = 0; j < 4; j++)
		{
			esam_random[j] = pEsamRet[j + 1];    //存储随机数
		}
		return SUCCESS;
	}
	else
	{
		return -1;
	}
}

/*
* 函数名称：	
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
// 读取系统文件处理
int esamCheckReadSysInfo(uint8 * data,uint8 curlist) 						
{
	uint8 i,j, len;
	uint8 *pEsamRet;

	pEsamRet = data;
	
	len = 0;
	for(i=0; i<curlist; i++)
		len += pEsamRet[len] + 1;
	pEsamRet += len; // 得到开始位置
	len = pEsamRet[0]; // 得到数据总长度
	
	if(pEsamRet[len-1] == 0x90 && pEsamRet[len] == 0x00)
	{
		// 保存数据
		for (j=0; j<g_read_file.len[0]; j++)
			esam_read_sysinfo[j] =  pEsamRet[j+1];

		return 0;
	}
	else 
		return -1;
}

/*
* 函数名称：	
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int esamCheckGetSn(uint8 * data,uint8 curlist)
{
	uint8 i,j, len;
	uint8 *pEsamRet;

	pEsamRet = data;
	
	len = 0;
	for(i=0; i<curlist; i++)
		len += pEsamRet[len] + 1;
	pEsamRet += len; // 得到开始位置
	len = pEsamRet[0]; // 得到数据总长度

	if(pEsamRet[len-1] == 0x90 && pEsamRet[len] == 0x00)
	{
		// 保存数据
		for (j=0; j<4; j++)
		{
			EsamSn[j] = pEsamRet[j+1];    //存储随机数
		}
	}
	else 
		return -1;
	return 0;
}

/*
* 函数名称：	
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
// 读取文件处理
int esamCheckReadVehInfo(uint8 * data,uint8 curlist) 						
{
	uint8 i,j, len;
	uint8 *pEsamRet;

	pEsamRet = data;
	
	len = 0;
	for(i=0; i<curlist; i++)
		len += pEsamRet[len] + 1;
	pEsamRet += len; // 得到开始位置
	len = pEsamRet[0]; // 得到数据总长度

	//g_esam_ret[0] = pEsamRet[len-1];  //用于武汉陆桥OBU检测
	//g_esam_ret[1] = pEsamRet[len];
	
	if(pEsamRet[len-1] == 0x90 && pEsamRet[len] == 0x00)
	{
		// 保存数据
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
* 函数名称：	
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
//读系统信息、车辆信息
void esamReadFileInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 offset,uint8 length)
{
	uint8 i;
	
	transfer_rq->apdulist++; // 命令数量+1
	
	i = transfer_rq->frame_len;
	// 读文件内容
	transfer_rq->apdu[i++] = 0x05; // 命令长度
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0xb0;
	transfer_rq->apdu[i++] = 0x81;
	transfer_rq->apdu[i++] = offset;
	transfer_rq->apdu[i++] = length;
	
	// 设置总长度
	transfer_rq->frame_len = i;
}

/*
* 函数名称：	esamReadRecordInfoFrame
* 功能描述：	
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
*/
void esamReadRecordInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 record_num,uint8 length)
{
	uint8 i;
	transfer_rq->apdulist++;	//命令数量+1
	i = transfer_rq->frame_len;
	//读文件内容
	transfer_rq->apdu[i++] = 0x05;	//命令长度
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0xb2;
	transfer_rq->apdu[i++] = record_num;	//第几条记录
	transfer_rq->apdu[i++] = 0x14;	//表示EF02交易记录文件
	transfer_rq->apdu[i++] = length;
	//设置总长度
	transfer_rq->frame_len = i;
}

/*
* 函数名称：	esamCheckReadFileInfo
* 功能描述：	读取文件处理
* 参数列表：	
* 返回结果：	0	--	成功
*			非0	--	失败
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
	pEsamRet += len;	//得到开始位置
	len = pEsamRet[0];	//得到数据总长度
	if((pEsamRet[len - 1] == 0x90) && (pEsamRet[len] == 0x00))
	{
		//保存数据
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

// 保存车辆信息文件
int esamSaveVehicle(int len_vehicleinfo, uint8 *randRSU, uint8 *File,uint8 *psam_decrypted_data,uint8 *psam_decrypted_mac,uint8 *esam_encrypted_data)
{
	uint8 i;
//	//无意义
// 	for(i=0;i<8;i++)
// 	{
// 		psam_random[i] = randRSU[i];
// 	}

	if((g_OBU_ContractVersion == 0x00)||(g_OBU_ContractVersion == 0x10)||(g_OBU_ContractVersion == 0x11))
	{
		// 保存明文数据
// 		psam_decrypted_data[0] = len_vehicleinfo + 2;
		psam_decrypted_data[0] = len_vehicleinfo;
		for (i=0; i<len_vehicleinfo; i++)
		{
			psam_decrypted_data[i+1] = File[i];     //存储8字节MAC+应用车辆信息明文数据 北京协议中8字节MAC在车辆信息明文数据后面
		}		
//		for (i=0; i<8; i++)
//		{
//			psam_decrypted_mac[i] = File[i+len_vehicleinfo];     //存储8字节MAC+应用车辆信息明文数据 北京协议中8字节MAC在车辆信息明文数据后面
//		}
	}
	else if(g_OBU_ContractVersion == 0x12)
	{
		esam_encrypted_data[0] = 0x48;//pGS_rs->length;
		for (i=0; i<esam_encrypted_data[0]; i++)
		{
			esam_encrypted_data[i+1] = File[i];     //存储密文数据
		}
	}
	return 0;
	/*
	// 计算鉴别码
	rsuMACInit(); // 得到计算用的密钥
	rsuMAC(pGS_rq->randRSU); // 计算鉴别码
	// 比较MAC与鉴别码
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
* 函数名称：	esamWriteRecordInfoFrame
* 功能描述：
* 参数列表：
* 返回结果：	0	--	成功
*			非0	--	失败
*/
void esamWriteRecordInfoFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 record_num,uint8 length)
{
	uint8 i;
	int j = 0;
	transfer_rq->apdulist++;	//命令数量+1
	i = transfer_rq->frame_len;
	//写记录信息
	transfer_rq->apdu[i++] = 5 + length;	//命令长度
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0xDC;
	transfer_rq->apdu[i++] = 0x00;
	transfer_rq->apdu[i++] = 0x10 + 0x03;	//表示EF02交易记录文件
	transfer_rq->apdu[i++] = length;
	for(j = 0; j < length; j ++)
	{
		transfer_rq->apdu[i++] = 0x05;
	}
	//设置总长度
	transfer_rq->frame_len = i;
}


/*
* 函数名称：	esamInitFrame
* 功能描述：
* 参数列表：
* 返回结果：	0	--	成功
*			非0	--	失败
*/
void ledInitFrame(ST_TRANSFER_CHANNEL * transfer_rq,char ledid)
{
	transfer_rq->channelid = ledid;
	transfer_rq->apdulist = 0;
	transfer_rq->frame_len = 0;
	memset(transfer_rq->apdu,0,128);
}

/*
* 函数名称：	esamReplaceKey
* 功能描述：
* 参数列表：
* 返回结果：	0	--	成功
*				非0	--	失败
*/
void ledflash(ST_TRANSFER_CHANNEL *transfer_rq,char flashCount)
{
	uint8 len = 0, j = 0;
	transfer_rq->apdulist++;	//命令数+1
	len = transfer_rq->frame_len;
	transfer_rq->apdu[len++] = 1;	//命令长度
	transfer_rq->apdu[len++] = flashCount;	//闪灯次数
	transfer_rq->frame_len = len;


}


/*
* 函数名称：
* 功能描述：
* 参数列表：
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int ledflashCheck(uint8 * data,uint8 curlist)
{
	uint8 i,j, len;
	uint8 *pEsamRet;

	pEsamRet = data;

	len = 0;
	for(i=0; i<curlist; i++)
		len += pEsamRet[len] + 1;
	pEsamRet += len; // 得到开始位置
	len = pEsamRet[0]; // 得到数据总长度

	/*
	if(pEsamRet[len-1] == 0x90 && pEsamRet[len] == 0x00)
	{
		// 保存数据

	}
	*/
	if(pEsamRet[len] == 0x00)
	{
		// 保存数据

	}
	else
		return -1;
	return 0;
}


