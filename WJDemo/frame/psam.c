
#include "lib2hd.h"
#include "common.h"
#include "psam.h"
#include "lib2rsu.h"
#include "crc.h"


static const uint8 psam_password[8] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38};	//pin码校验



/*
* 函数名称：	psamEnterDir()
* 功能描述：	 
* 参数列表：	无
* 返回结果：	
*/
int8 psamEnterDir(int psam_slot, uint16 dir)
{
	uint8 apdulist = 1;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	int ret, len, i;
	apdu[0] = 0x07;				//7	 进入ETC应用目录
	apdu[1] = 0x00;
	apdu[2] = 0xa4;
	apdu[3] = 0x00;
	apdu[4] = 0x00;
	apdu[5] = 0x02;		
	apdu[6] = (dir >> 8) & 0xff;		  
	apdu[7] = dir & 0xff;
	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu);
	if(ret != SUCCESS)
	{
		return -1 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0]; 
		//注意psam操作的长度不包括9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			return SUCCESS;
		}
	}
	return -2 + ret*10;
}

/*
* 函数名称：	psamEnterETCDir()
* 功能描述：	PSAM母卡口令验证并进入ETC应用目录 
* 参数列表：	无
* 返回结果：	
*/
int8 psamEnterETCDir(int psam_slot)
{
	uint8 apdulist = 2;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	int ret, len, i;
	apdu[0] = 0x0d;	//13,口令验证
	apdu[1] = 0x00;
	apdu[2] = 0x20;
	apdu[3] = 0x00;
	apdu[4] = 0x01;
	apdu[5] = 0x08;	//8
	for(i = 0;i < 8;i++)
	{
	    apdu[i + 6] = psam_password[i];
	}
	apdu[14] = 0x07;	//7,进入ETC应用目录
	apdu[15] = 0x00;
	apdu[16] = 0xa4;
	apdu[17] = 0x00;
	apdu[18] = 0x00;
	apdu[19] = 0x02;		
	apdu[20] = 0xa0;			  
	apdu[21] = 0x01;
	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu);
	if(ret != SUCCESS)
	{
		return -1 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[3];	//第二条命令的返回数据长度
		//注意psam操作的长度不包括9000
		if((data[len + 4] == 0x90) && (data[len + 5] == 0x00))
		{
			return SUCCESS;
		}
	}
	return -2 + ret*10;
}

/*
* 函数名称：	psamGetMK_MF()
* 功能描述：	取得系统主控密钥
* 参数列表：	无
* 返回结果：	
*/
int psamGetMK_MF(int psam_slot,uint8 P1,uint8 P2)
{
	return 1; 
}

/*
* 函数名称：	psamGetDAMK_MF()
* 功能描述：	取得MF系统维护密钥
* 参数列表：	无
* 返回结果：	
*/
int8 psamGetDAMK_MF(int psam_slot)
{
	uint8 apdulist = 2;	//待发命令
	uint8 apdu[128];
	int datalist;//返回数据
	uint8 data[128];
	int ret, len, i;
	//指令一，用新的分散因子进行密钥分散
	apdu[0] = 0x13;	//19,二级密钥分散
	apdu[1] = 0x80;
	apdu[2] = 0x40;
	apdu[3] = 0x00;
	apdu[4] = 0x01;
	apdu[5] = 0x0e;	//14
	apdu[6] = 0x11;
	apdu[7] = 0x01;
	apdu[8] = 0xfe;
	apdu[9] = 0x01;
	apdu[10] = 0x01;
	apdu[11] = 0x00;
	for(i = 0;i < 8;i++)
	{
		apdu[i + 12] = g_contract_sn[i]; //合同序列号
	}
	//指令二，在旧的密钥保护下导出新的密文密钥
	apdu[20] = 0x19;	//25,导出密钥
	apdu[21] = 0x80;
	apdu[22] = 0x42;
	apdu[23] = 0x01;
	apdu[24] = 0x00;
	apdu[25] = 0x14;	//20
	apdu[26] = 0x84;			  
	apdu[27] = 0xd4;	
	apdu[28] = 0x01;			  
	apdu[29] = 0xff;
	//芯片序列号作为保护密钥分散因子,因为主控密钥是用芯片序列号分散的
	for(i = 0; i < 4; i++)
	{
	    apdu[i + 30] = esam_random[i];
		apdu[i + 34] = 0x00;
		apdu[i + 38] = EsamSn[i]; 
		apdu[i + 42] = EsamSn[i];
	}
	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
	{
		return -1 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[3]; //第二条命令的返回数据长度
		//注意psam操作的长度不包括9000
		if((data[len + 4] == 0x90) && (data[len + 5] == 0x00))
		{ 
		    for(i = 0; i < 28; i++)
			{
		        psam_encrypted_key[i] = data[4 + i];
			}
			return SUCCESS; 
		} 
	}
	return -2 + ret*10;
}

/*
* 函数名称：	psamGetMK_DF01()
* 功能描述：	取得DF01主控密钥
* 参数列表：	无
* 返回结果：	
*/
uint8 psamGetMK_DF01(int psam_slot,uint8 P1,uint8 P2)
{
	return 1;
}

/*
* 函数名称：	psamGetDAMK_DF01()
* 功能描述：	取得DF01应用维护密钥 
* 参数列表：	无
* 返回结果：	
*/
int8 psamGetDAMK_DF01(int psam_slot)
{
	uint8 apdulist = 2;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	int ret, len, i;
	//第一条指令，密钥分散
	apdu[0] = 0x13;	//19,一级密钥分散
	apdu[1] = 0x80;
	apdu[2] = 0x40;
	apdu[3] = 0x00;
	apdu[4] = 0x01;
	apdu[5] = 0x0e;	//14
	apdu[6] = 0x21;
	apdu[7] = 0x01;
	apdu[8] = 0xfe;
	apdu[9] = 0x01;
	apdu[10] = 0x01;
	apdu[11] = 0x00;
	for(i = 0; i < 8; i++)
	{
	    apdu[i + 12] = g_contract_sn[i];
	}
	//指令二，密钥导出
	apdu[20] = 0x1a;	//25,二级密钥分散
	apdu[21] = 0x80;
	apdu[22] = 0x42;
	apdu[23] = 0x01;
	apdu[24] = 0x00;
	apdu[25] = 0x14;	//20
	apdu[26] = 0x84;			  
	apdu[27] = 0xd4;	
	apdu[28] = 0x01;			  
	apdu[29] = 0xff;
	for(i = 0;i < 4;i++)
	{
	    apdu[i + 30] = esam_random[i];
		apdu[i + 34] = 0x00;
		apdu[i + 38] = EsamSn[i];
		apdu[i + 42] = EsamSn[i];
	}
	ret = PSAM_CHANNEL_rq(psam_slot, apdulist, apdu) ;
	if(ret != SUCCESS)
	{
// 		return ret;
		return -1 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[3]; //第二条命令的返回数据长度
		//注意psam操作的长度不包括9000
		if((data[len + 4] == 0x90) && (data[len + 5] == 0x00))
		{ 
		    for(i = 0; i < 28; i++)
			{
		        psam_encrypted_key[i] = data[4 + i];
			}
			return SUCCESS; 
		} 
		return -2;
	}
	return -3 + ret*10;
}

/*
* 函数名称：	psamGetKey()
* 功能描述：	 
* 参数列表：	无
* 返回结果：	
*/
// 取得DF01应用认证密钥 
int8 psamGetRK1_DF01(int psam_slot)
{
	uint8 apdulist;//待发命令
	uint8 apdu[128];
	int datalist;//返回数据
	uint8 data[128];
	int ret;
	int len;
	int i;

	apdulist = 2;

	//第一条指令，密钥分散
	apdu[0] = 0x13;			    //19  一级密钥分散
	apdu[1] = 0x80;
	apdu[2] = 0x40;
	apdu[3] = 0x00;
	apdu[4] = 0x01;
	apdu[5] = 0x0e;			    //14

	apdu[6] = 0x22;
	apdu[7] = 0x01;
	apdu[8] = 0xfe;
	apdu[9] = 0x01;
	apdu[10] = 0x02;
	apdu[11] = 0x00;
	for(i=0;i<8;i++)
	{
	    apdu[i+12] = g_contract_sn[i];
	}

	//第二条指令，密钥导出
	apdu[20] = 0x1a;				//25   二级密钥分散
	apdu[21] = 0x80;
	apdu[22] = 0x42;
	apdu[23] = 0x01;
	apdu[24] = 0x00;
	apdu[25] = 0x14;	            //20   
		
	apdu[26] = 0x84;			  
	apdu[27] = 0xd4;	
	apdu[28] = 0x01;			  
	apdu[29] = 0xff;
	for(i=0;i<4;i++)
	{
	    apdu[i + 30] = esam_random[i];
		apdu[i + 34] = 0x00;
		apdu[i + 38] = EsamSn[i];
		apdu[i + 42] = EsamSn[i];
	}

	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
		return -1 + ret*10;

	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[3]; //第二条命令的返回数据长度
		//注意psam操作的长度不包括9000
		if(data[len + 4] == 0x90 && data[len+5] == 0x00) 
		{ 
		    for(i=0;i<28;i++)
			{
		        psam_encrypted_key[i] = data[4+i];
			}
			return SUCCESS; 
		}
		else 
			return -2;
	}
	return -3 + ret*10;
}

/*
* 函数名称：	psamGetKey()
* 功能描述：	 
* 参数列表：	无
* 返回结果：	
*/
// 取得DF01应用加密密钥（版本1） 
int8 psamGetRK2_DF01_V1(int psam_slot)
{
	uint8 apdulist;//待发命令
	uint8 apdu[128];
	int datalist;//返回数据
	uint8 data[128];
	int ret, len, i;

	apdulist = 2;

	//第一条指令，密钥分散
	apdu[0] = 0x13;			    //19  一级密钥分散
	apdu[1] = 0x80;
	apdu[2] = 0x40;
	apdu[3] = 0x00;
	apdu[4] = 0x01;
	apdu[5] = 0x0e;			    //14

	apdu[6] = 0x23;
	apdu[7] = 0x01;
	apdu[8] = 0xfe;
	apdu[9] = 0x01;
	apdu[10] = 0x03;
	apdu[11] = 0x00;
	for(i=0;i<8;i++)
	{
	    apdu[i+12] = g_contract_sn[i];
	}

	//第二条指令，密钥导出
	apdu[20] = 0x1a;				//25   二级密钥分散
	apdu[21] = 0x80;
	apdu[22] = 0x42;
	apdu[23] = 0x01;
	apdu[24] = 0x00;
	apdu[25] = 0x14;	            //20   
		
	apdu[26] = 0x84;			  
	apdu[27] = 0xd4;	
	apdu[28] = 0x01;			  
	apdu[29] = 0xff;
	for(i=0;i<4;i++)
	{
	    apdu[i + 30] = esam_random[i];
		apdu[i + 34] = 0x00;
		apdu[i + 38] = EsamSn[i];
		apdu[i + 42] = EsamSn[i];
	}

	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
		return -1 + ret*10;


	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[3]; //第二条命令的返回数据长度
		//注意psam操作的长度不包括9000
		if(data[len + 4] == 0x90 && data[len+5] == 0x00) 
		{ 
		    for(i=0;i<28;i++)
			{
		        psam_encrypted_key[i] = data[4+i];
			}

			return SUCCESS; 
		}
		else
			return -2;

	}
	return -3 + ret*10;
}

/*
* 函数名称：	psamGetKey()
* 功能描述：	 
* 参数列表：	无
* 返回结果：	
*/
// 取得DF01应用加密密钥（版本2） 
int8 psamGetRK2_DF01_V2(int psam_slot)
{
	uint8 apdulist;//待发命令
	uint8 apdu[128];
	int datalist;//返回数据
	uint8 data[128];
	int ret;
	int len;
	int i;

	apdulist = 2;

	//第一条指令，密钥分散
	apdu[0] = 0x13;			    //19  一级密钥分散
	apdu[1] = 0x80;
	apdu[2] = 0x40;
	apdu[3] = 0x00;
	apdu[4] = 0x01;
	apdu[5] = 0x0e;			    //14

	apdu[6] = 0x23;
	apdu[7] = 0x02;
	apdu[8] = 0xfe;
	apdu[9] = 0x01;
	apdu[10] = 0x03;
	apdu[11] = 0x01;
	for(i=0;i<8;i++)
	{
	    apdu[i+12] = g_contract_sn[i];
	}

	//第二条指令，密钥导出
	apdu[20] = 0x1a;				//25   二级密钥分散
	apdu[21] = 0x80;
	apdu[22] = 0x42;
	apdu[23] = 0x01;
	apdu[24] = 0x00;
	apdu[25] = 0x14;	            //20   
		
	apdu[26] = 0x84;			  
	apdu[27] = 0xd4;	
	apdu[28] = 0x01;			  
	apdu[29] = 0xff;
	for(i=0;i<4;i++)
	{
	    apdu[i + 30] = esam_random[i];
		apdu[i + 34] = 0x00;
		apdu[i + 38] = EsamSn[i];
		apdu[i + 42] = EsamSn[i];
	}

	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
		return -1 + ret*10;


	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[3]; //第二条命令的返回数据长度
		//注意psam操作的长度不包括9000
		if(data[len + 4] == 0x90 && data[len+5] == 0x00) 
		{ 
		    for(i=0;i<28;i++)
			{
		        psam_encrypted_key[i] = data[4+i];
			}

			return SUCCESS; 
		}
		else
		{
			return -2;
		}

	}
	return -3 + ret*10;
}

/*
* 函数名称：	psamGetKey()
* 功能描述：	 
* 参数列表：	无
* 返回结果：	
*/
// 取得DF01应用加密密钥（版本3） 
int8 psamGetRK2_DF01_V3(int psam_slot)
{
	uint8 apdulist;//待发命令
	uint8 apdu[128];
	int datalist;//返回数据
	uint8 data[128];
	int ret;
	int len;
	int i;

	apdulist = 2;

	//第一条指令，密钥分散
	apdu[0] = 0x13;			    //19  一级密钥分散
	apdu[1] = 0x80;
	apdu[2] = 0x40;
	apdu[3] = 0x00;
	apdu[4] = 0x01;
	apdu[5] = 0x0e;			    //14

	apdu[6] = 0x23;
	apdu[7] = 0x03;
	apdu[8] = 0xfe;
	apdu[9] = 0x01;
	apdu[10] = 0x03;
	apdu[11] = 0x02;
	for(i=0;i<8;i++)
	{
	    apdu[i+12] = g_contract_sn[i];
	}

	//第二条指令，密钥导出
	apdu[20] = 0x1a;				//25   二级密钥分散
	apdu[21] = 0x80;
	apdu[22] = 0x42;
	apdu[23] = 0x01;
	apdu[24] = 0x00;
	apdu[25] = 0x14;	            //20   
		
	apdu[26] = 0x84;			  
	apdu[27] = 0xd4;	
	apdu[28] = 0x01;			  
	apdu[29] = 0xff;
	for(i=0;i<4;i++)
	{
	    apdu[i + 30] = esam_random[i];
		apdu[i + 34] = 0x00;
		apdu[i + 38] = EsamSn[i];
		apdu[i + 42] = EsamSn[i];
	}

	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
		return -1 + ret*10;

	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[3]; //第二条命令的返回数据长度
		//注意psam操作的长度不包括9000
		if(data[len + 4] == 0x90 && data[len+5] == 0x00) 
		{ 
		    for(i=0;i<28;i++)
			{
		        psam_encrypted_key[i] = data[4+i];
			}

			return SUCCESS; 
		}
		else
			return -2;
 
	}
	return -3 + ret*10;
	
}

/*
* 函数名称：	psamCalcEsamMac()
* 功能描述：	计算ESAM卡文件MAC,握奇PSAM卡
* 参数列表：	
* 返回结果：	
*/
int8 psamCalcEsamMac(int psam_slot,uint8 offset,uint8 length,uint8 *file,uint8 type)
{
//	psamGetKey(1,8);
//	int ii;
//	for(ii = 0;ii<16;ii++)
//	{
//		g_com_rx_buf[ii] = cur_key[ii];
//	}
//
//	g_com_rx_len  = 16;
//	print_info(COM,RECV);
	uint8 apdulist;	//待发命令
	uint8 apdu[128] = {0};
	int datalist;	//返回数据
	uint8 data[128] = {0};
	int ret, len, temp, i;
	apdulist = 2;
	if(g_psam_disperse == 1)
	{
		//只进行一次分散
		//第一条指令，密钥分散  （二级密钥分散）
		apdu[0] = 0x0d;	//13
		apdu[1] = 0x80;
		apdu[2] = 0x1a;
		//apdu[3] = 0x26;//国标
		apdu[3] = 0x28;//北京协议 change by wll	2013-5-6 12:40:48

		if (g_OBU_ContractVersion == 0x16)//二版OBU合同版本
		{
			if(type == 0)
			{
				apdu[4] = 0x01;	//更新系统信息文件
			}
			else
			{
				apdu[4] = 0x02;	//更新应用车辆信息文件
			}
			for(i = 0; i < 8; i++)
			{
				apdu[i + 6] = g_contract_sn[i];   //合同序列号
			}
		}
		else//1版OBU合同版本
		{
			if(type == 0)//系统信息
			{
				apdu[4] = 0x08;	//更新系统信息文件
				if((vst.macid[0] == 0x02)&&((vst.sysinfo[9] == 0x00)||(vst.sysinfo[9] == 0x10)))
				{
					for(i = 0; i < 8; i++)
					{
						apdu[i + 6] = g_contract_sn[i];   //合同序列号
	//							apdu[i + 6] = EsamDv[i];   //复位信息
					}
				}
				else
				{
					for(i = 0; i < 8; i++)
					{
						apdu[i + 6] = EsamDv[i];   //复位信息
					}
				}

			}
			else//车辆信息
			{
				apdu[4] = 0x0a;	//更新应用车辆信息文件
				for(i = 0; i < 8; i++)
				{
					apdu[i + 6] = g_contract_sn[i];   //合同序列号
				}
			}

		}

		apdu[5] = 0x08;		//16
		//第二条指令，计算mac
		apdu[15] = 0x80;
		apdu[16] = 0xfa;
		apdu[17] = 0x05;
		apdu[18] = 0x00;
		//计算mac所用的所有内容，包括随机数+esam命令内容
		for(i = 0; i < 4; i++)
		{
			apdu[i + 20] = esam_random[i];
			apdu[i + 24] = 0x00;
		}
		//psam计算所用的内容要与esam命令内容一致
		apdu[28] = 0x04;
		apdu[29] = 0xd6;
		apdu[30] = 0x81;
		apdu[31] = offset;
		apdu[32] = length + 4;	//length+4,加4表示4个字节mac
		for(i = 0;i < length;i++)
		{
			apdu[i + 33] = file[i];
		}
		temp = (length + 13) % 8;	//计算mac所用内容长度模8
		//甭管能否被8整除，都应该补齐一个0x80+0x00,补到8的倍数
		apdu[length + 33] = 0x80;
		for(i = 0;i < (7 - temp);i++)
		{
			apdu[i +length + 34] = 0x00;
		}
		apdu[14] = length + 26 - temp;	//length+5+5+8+8-temp
		apdu[19] = length + 21 - temp;	//8+5+length+8-temp，计算mac所用内容的长度
	}/* end if(g_psam_disperse == 1) */
	else if(g_psam_disperse == 2)
	{
		//进行两次分散
		//第一条指令，密钥分散
		apdu[0] = 0x15;			    
		apdu[1] = 0x80;
		apdu[2] = 0x1a;
		apdu[3] = 0x46;
		if(type == 0)
		{
			apdu[4] = 0x01;	//更新系统信息文件
			if (g_OBU_ContractVersion == 0x16)
			{
				for(i = 0; i < 8; i++)
				{
					apdu[i + 6] = g_contract_sn[i];
				}
			}
			else	//一版OBU写系统信息分散因子时，是OBU复位信息
			{
				for(i = 0; i < 8; i++)
				{
					apdu[i + 6] = EsamDv[i];
				}
			}
		}
		else
		{
			apdu[4] = 0x02;	//更新应用车辆信息文件
			for(i = 0; i < 8; i++)
			{
				apdu[i + 6] = g_contract_sn[i];
			}
		}
		apdu[5] = 0x10;			    

		for(i = 0; i < 4; i++)
		{
			apdu[i+14] = vst.sysinfo[i];
			apdu[i+18] = vst.sysinfo[i];
		}
		//第二条指令，计算mac
		apdu[23] = 0x80;
		apdu[24] = 0xfa;
		apdu[25] = 0x05;
		apdu[26] = 0x00;
		//计算mac所用的所有内容，包括随机数+esam命令内容
		for(i = 0; i < 4; i++)
		{
			apdu[i + 28] = esam_random[i];
			apdu[i + 32] = 0x00;
		}
		//psam计算所用的内容要与esam命令内容一致
		apdu[36] = 0x04;
		apdu[37] = 0xd6;
		apdu[38] = 0x81;
		apdu[39] = offset;
		apdu[40] = length + 4;	//length+4,加4表示4个字节mac
		for(i = 0;i < length;i++)
		{
			apdu[i + 41] = file[i];
		}
		temp = (length + 13) % 8;	//计算mac所用内容长度模8
		//甭管能否被8整除，都应该补齐一个0x80+0x00,补到8的倍数
		apdu[length + 41] = 0x80;
		for(i = 0; i < (7 - temp); i++)
		{
			apdu[i + length + 42] = 0x00;
		}
		apdu[22] = length + 26 - temp;	//length+5+5+8+8-temp
		apdu[27] = length + 21 - temp;	
	}/* end else if(g_psam_disperse == 2) */
	ret = PSAM_CHANNEL_rq(psam_slot, apdulist, apdu) ;
	if(ret != SUCCESS)
	{
		//DEBUGP("psamCalcEsamMac PSAM_CHANNEL_rq ERR, ret = %d\n", ret);
		return -1 + ret*10;

	}
	ret = PSAM_CHANNEL_rs(&datalist, data, 1000);
	if(ret == SUCCESS)  
	{ 
		len = data[3]; //第二条命令的返回数据长度
		//注意psam操作的长度不包括9000
		if((data[len + 4] == 0x90) && (data[len + 5] == 0x00))
		{ 
			esam_mac[0] = data[4];
			esam_mac[1] = data[5];
			esam_mac[2] = data[6];
			esam_mac[3] = data[7];
			return SUCCESS; 
		} 
		else
			return -2;
	}
	return -3 + ret*10;
}

/*
* 函数名称：	psamCalcEsamMac_JuLi()
* 功能描述：	计算ESAM卡文件MAC,握奇PSAM卡
* 参数列表：	
* 返回结果：	
*/
int8 psamCalcEsamMac_JuLi(int psam_slot,uint8 offset,uint8 length,uint8 *file,uint8 type)
{
	uint8 apdulist;	//待发命令
	uint8 apdu[128] = {0};
	int datalist;	//返回数据
	uint8 data[128] = {0};
	int ret, len, temp, i;
	int pos = 0;
	int syslen = 0;   //补齐8000之后的长度
	apdulist = 2;
	if(g_psam_disperse == 1)
	{

//		apdu[pos++]= 0x07;//第一条指令长度
//		apdu[pos++]= 0x00;//第一条指令
//		apdu[pos++]= 0xA4;
//		apdu[pos++]= 0x00;
//		apdu[pos++]= 0x00;
//		apdu[pos++]= 0x02;
//		apdu[pos++]= 0x10;
//		apdu[pos++]= 0x01;

		apdu[pos++]= 0x0d;//第二条指令长度
		apdu[pos++]= 0x80;//第二条指令	
		apdu[pos++]= 0x1A;	
		apdu[pos++]= 0x28;	
		apdu[pos++]= 0x0a;//此位  系统信息是08
		apdu[pos++]= 0x08;
		apdu[pos++]= vst.macid[0];
		apdu[pos++]= vst.macid[1];
		apdu[pos++]= vst.macid[2];
		apdu[pos++]= vst.macid[3];
		apdu[pos++]= ~(vst.macid[0]);
		apdu[pos++]= ~(vst.macid[1]);
		apdu[pos++]= ~(vst.macid[2]);
		apdu[pos++]= ~(vst.macid[3]);

		if(length%8 == 0)
			syslen= length;
		else
			syslen = ((length/8)+1)*8;
		apdu[pos++]= syslen+13;//第三条指令长度
		apdu[pos++]= 0x80;//第三条指令
		apdu[pos++]= 0xFA;	
		apdu[pos++]= 0x05;	
		apdu[pos++]= 0x00;
		apdu[pos++]= 8+syslen;
		apdu[pos++]= vst.rndinfo[0];
		apdu[pos++]= vst.rndinfo[1];
		apdu[pos++]= vst.rndinfo[2];
		apdu[pos++]= vst.rndinfo[3];
		apdu[pos++]= 0x00;	
		apdu[pos++]= 0x00;
		apdu[pos++]= 0x00;	
		apdu[pos++]= 0x00;
		for (i= 0;i<length;i++)
		{
			apdu[pos++]= file[i];
		}

		if(length%8 != 0)
		{
			apdu[pos++]= 0x80;
			length++;
		}
		while(length%8 != 0)
		{
			apdu[pos++]= 0x00;
			length++;
		}
	}/* end if(g_psam_disperse == 1) */
	else if(g_psam_disperse == 2)
	{
		//进行两次分散
		//第一条指令，密钥分散
		apdu[0] = 0x15;			    
		apdu[1] = 0x80;
		apdu[2] = 0x1a;
		apdu[3] = 0x46;
		if(type == 0)
		{
			apdu[4] = 0x01;	//更新系统信息文件
			if (g_OBU_ContractVersion == 0x16)
			{
				for(i = 0; i < 8; i++)
				{
					apdu[i + 6] = g_contract_sn[i];
				}
			}
			else	//一版OBU写系统信息分散因子时，是OBU复位信息
			{
				for(i = 0; i < 8; i++)
				{
					apdu[i + 6] = EsamDv[i];
				}
			}
		}
		else
		{
			apdu[4] = 0x02;	//更新应用车辆信息文件
			for(i = 0; i < 8; i++)
			{
				apdu[i + 6] = g_contract_sn[i];
			}
		}
		apdu[5] = 0x10;			    

		for(i = 0; i < 4; i++)
		{
			apdu[i+14] = vst.sysinfo[i];
			apdu[i+18] = vst.sysinfo[i];
		}
		//第二条指令，计算mac
		apdu[23] = 0x80;
		apdu[24] = 0xfa;
		apdu[25] = 0x05;
		apdu[26] = 0x00;
		//计算mac所用的所有内容，包括随机数+esam命令内容
		for(i = 0; i < 4; i++)
		{
			apdu[i + 28] = esam_random[i];
			apdu[i + 32] = 0x00;
		}
		//psam计算所用的内容要与esam命令内容一致
		apdu[36] = 0x04;
		apdu[37] = 0xd6;
		apdu[38] = 0x81;
		apdu[39] = offset;
		apdu[40] = length + 4;	//length+4,加4表示4个字节mac
		for(i = 0;i < length;i++)
		{
			apdu[i + 41] = file[i];
		}
		temp = (length + 13) % 8;	//计算mac所用内容长度模8
		//甭管能否被8整除，都应该补齐一个0x80+0x00,补到8的倍数
		apdu[length + 41] = 0x80;
		for(i = 0; i < (7 - temp); i++)
		{
			apdu[i + length + 42] = 0x00;
		}
		apdu[22] = length + 26 - temp;	//length+5+5+8+8-temp
		apdu[27] = length + 21 - temp;	
	}/* end else if(g_psam_disperse == 2) */
	ret = PSAM_CHANNEL_rq(psam_slot, apdulist, apdu) ;
	if(ret != SUCCESS)
	{
		//DEBUGP("psamCalcEsamMac PSAM_CHANNEL_rq ERR, ret = %d\n", ret);
// 		return ret;
		return -1 + ret*10;

	}
	ret = PSAM_CHANNEL_rs(&datalist, data, 1000);
	print_info_Info(data,20,UDPNET);
	if(ret == SUCCESS)  
	{ 
		len = data[3]; //第二条命令的返回数据长度
		//注意psam操作的长度不包括9000
		if((data[len + 4] == 0x90) && (data[len + 5] == 0x00))
		{ 
			esam_mac[0] = data[4];
			esam_mac[1] = data[5];
			esam_mac[2] = data[6];
			esam_mac[3] = data[7];
			return SUCCESS; 
		} 
		else
			return -2;
	}
	//DEBUGP("psamCalcEsamMac PSAM_CHANNEL_rs ERR, ret = %d\n", ret);
	return -3 + ret*10;
}

/*
* 函数名称：	psamGetKey()
* 功能描述：	 
* 参数列表：	无
* 返回结果：	
*/
int psamGetKey(int psam_slot,uint8 P2)
{
	uint8 apdulist;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	int ret, len, i;
	apdulist = 1;
	apdu[0] = 0x0d;			  //导出DF01应用加密密钥（版本1）
	apdu[1] = 0x80;
	apdu[2] = 0xf6;
	apdu[3] = 0x80;
	apdu[4] = P2;
	apdu[5] = 0x08;
	for(i=0;i<8;i++)
	{
//		if((vst.macid[0] == 0x02)&&((vst.sysinfo[9] == 0x00)||(vst.sysinfo[9] == 0x10)))
//		{
//			apdu[i + 6] = EsamDv[i];   //复位信息
//		}
//		else
//		{
//			apdu[i+6] = g_contract_sn[i];
//		}
	    apdu[i+6] = g_contract_sn[i];
	}

	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)	
		return -1 + ret*10;


	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0]; //返回数据的长度
		//注意psam操作的长度不包括9000
		if(data[len + 1] == 0x90 && data[len+2] == 0x00) 
		{ 
		    for(i=0;i<16;i++) 
			{ 
		        cur_key[i] = data[1+i]; 
			} 
			return SUCCESS; 
		} 
		else
			return -2;
	}
	return -3 + ret*10;
// 
// 	return 1;
}

/*
* 函数名称：	psamGetRandom()
* 功能描述：	取8字节随机数 
* 参数列表：	无
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int psamGetRandom(int psam_slot)
{
	uint8 apdulist;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	int ret;
	int len, i;
	apdulist = 1;
	apdu[0] = 0x05;
	apdu[1] = 0x00;
	apdu[2] = 0x84;
	apdu[3] = 0x00;
	apdu[4] = 0x00;
	apdu[5] = 0x08;
	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
	{
		return -1 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0];	//返回数据的长度
		//注意psam操作的长度不包括9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			for(i = 0;i < 8;i++)
			{
			    psam_random[i] = data[1 + i];
			} 
			return SUCCESS; 
		}
		return -2;
	}	
	return -3 + ret*10;
}

// 校验从ESAM卡读取的应用车辆信息 	   北京协议	 读取握奇母卡密钥明文，软件计算MAC
int8 psamVerifyEsamVehInfo(int psam_slot,uint8 *src,uint8 src_len,uint8 *dst_mac)
{
	uint8 i,len;
	uint16 crc_value;
	uint8 cur_crc[8];

//	src_len = src[1] - 8;  //去掉8字节MAC
	
	crc_value = do_crc(0xffff, src, src_len);
	cur_crc[0] = crc_value&0x00ff;
	cur_crc[1] = crc_value>>8;
	//对车辆信息文件计算crc，然后用所算的crc替换getsecure_rq中发送随机数的前两个字节
	for(i=0;i<6;i++)
	    cur_crc[i+2] = psam_random[i+2];

//	FD_3DesEncodeByDes(cur_crc,cur_key,psam_mac);hmh
//	g_com_tx_len = 8;
//	for(i = 0;i < g_com_rx_len;i++)
//	{
//		g_com_rx_buf[i] = cur_crc[i];
//	}
//	print_info(COM, RECV);
//
//	g_com_tx_len = 0x08;
//	for(i = 0;i < g_com_rx_len;i++)
//	{
//		g_com_rx_buf[i] = dst_mac[i];
//	}
//	print_info(COM, RECV);

//	if((vst.macid[0] == 0x03)&&(g_OBU_ContractVersion == 0x10))
//	{
//		for (i=0; i<8; i++)
//		{
//			psam_mac[i] = psam_random[i];     //聚利10版本OBU直接返回PSAM随即数
//		}
//	}

	//00和10版OBU不校验车辆信息mac
    if(((vst.macid[0] == 0x02)&&(g_OBU_ContractVersion == 0x00||g_OBU_ContractVersion == 0x10))
    		||((vst.macid[0] == 0x03)&&(g_OBU_ContractVersion == 0x00||g_OBU_ContractVersion == 0x10)))
    {
    	for(i=0;i<g_read_file.len[1];i++)
    		esam_read_vehicle[i] = src[i];

    	return SUCCESS;
    }
	//校验
	if((psam_mac[0]==dst_mac[0])&&(psam_mac[1]==dst_mac[1])&&(psam_mac[2]==dst_mac[2])&&(psam_mac[3]==dst_mac[3]))
	{
		if((psam_mac[4]==dst_mac[4])&&(psam_mac[5]==dst_mac[5])&&(psam_mac[6]==dst_mac[6])&&(psam_mac[7]==dst_mac[7]))
		{				
			for(i=0;i<g_read_file.len[1];i++)
				esam_read_vehicle[i] = src[i]; 
			
			return SUCCESS;
		}
	}

	return -1;
}
void FD_3DesDecode(uint8 src_len,uint8 src,uint8 *cur_key,uint8 *apdu_out)
{
    
}
// 解密从ESAM卡读取的应用车辆信息 
int8 psamDecryptEsamVehInfo(int psam_slot,uint8 *src,uint8 src_len,uint8 *dst,uint8 *mac)
{   
	uint8 apdu_out[80];
	uint8 i;

	FD_3DesDecode(src_len,src,cur_key,apdu_out);

	dst[0] = g_read_file.len[1];

	for(i = 0; i < g_read_file.len[1]; i ++)
	{
		dst[i+1] = apdu_out[9+i];
	}
	for(i=0;i<8;i++)
	{
		mac[i] = apdu_out[1+i];
	}

	return SUCCESS;
}

/* ***********************如下函数应用在硬sam解密中******************************** */

/*
* 函数名称：	psamDESInit()
* 功能描述：	解密初始化
* 参数列表：	无
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int8 psamDESInit(int psam_slot)
{
	int ret;
	uint8 apdulist;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	uint16 len = 0, i = 0;
	apdulist = 1;
//	apdu[len++] = 21;
//	apdu[len++] = 0x80;
//	apdu[len++] = 0x1a;
//	apdu[len++] = 0x59;
//	apdu[len++] = 0x03;
//	apdu[len++] = 0x10;
	apdu[len++] = 13;
	apdu[len++] = 0x80;
	apdu[len++] = 0x1a;
	apdu[len++] = 0x39;
	apdu[len++] = 0x03;
	apdu[len++] = 0x08;
	//从VST中得到系统信息文件	
	for(i = 0;i < 8;i++)
	{
		apdu[len++] = g_contract_sn[i];		 // 8字节合同序列号
	}
	//区域分散代码
//	apdu[len++] = vst.sysinfo[0];
//	apdu[len++] = vst.sysinfo[1];
//	apdu[len++] = vst.sysinfo[2];
//	apdu[len++] = vst.sysinfo[3];
//	apdu[len++] = vst.sysinfo[0];
//	apdu[len++] = vst.sysinfo[1];
//	apdu[len++] = vst.sysinfo[2];
//	apdu[len++] = vst.sysinfo[3];
	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
	{
		return -1 + ret*10;
/*		return ret;*/
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0];	//返回数据的长度
		//注意psam操作的长度不包括9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{ 
			return SUCCESS;
		} 
		return -2;
	}
	return -3 + ret*10;
// 
// 	return 1;
}

/*
* 函数名称：	psamDES()
* 功能描述：	解密
* 参数列表：	无
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int8 psamDES(int psam_slot, uint8 *src_file, int src_len, uint8 *dst_file, uint8 *dst_mac)
{
	int ret = SUCCESS;
	uint8 apdulist;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	uint16 len = 0, i = 0;
	apdulist = 1;
	apdu[len++] = 6 + src_len;
	apdu[len++] = 0x80;
	apdu[len++] = 0xfa;
	apdu[len++] = 0x80;
	apdu[len++] = 0x00;
	apdu[len++] = src_len;	//发行为0x48,国标交易为0x20
	//加密文件长度
	for(i = 0; i < src_len; i++)
	{
		apdu[len++] = src_file[i];
	}
	apdu[len++] = src_len;
	ret = PSAM_CHANNEL_rq(psam_slot, apdulist, apdu) ;
	if(ret != SUCCESS)
	{
		return -1 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0];	//PSAM返回数据的长度
		//注意psam操作的长度不包括9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			len = data[1];	//返回实际明文长度,MAC+车辆信息
			for(i = 0; i < 8; i++)
			{
				dst_mac[i] = data[2 + i];
			}
			for(i = 0; i < len - 8; i++)
			{
				dst_file[i] = data[10 + i];
			}
			return SUCCESS; 
		}
		return -2;

	}
	return -3 + ret*10;
}

/*
* 函数名称：	psamMAC()
* 功能描述：	解密后计算MAC
* 参数列表：	无
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int8 psamMAC(int psam_slot,uint8 *rand,uint8 *file,int file_len)
{
	int ret;
	uint8 apdulist = 1;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	uint16 len = 0, i = 0;
	apdu[len++] = 14 + file_len;
	apdu[len++] = 0x80;
	apdu[len++] = 0xfa;
	apdu[len++] = 0x08;
	apdu[len++] = 0x00;
	apdu[len++] = 8 + file_len;
	for(i = 0; i < 8; i++)
	{
		apdu[len++] = rand[i];	//随机数
	}
	for(i = 0; i < file_len; i++)
	{
		apdu[len++] = file[i];
	}
	apdu[len++] = 0x08;
	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
	{
		return -1 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0]; //返回数据的长度
		//注意psam操作的长度不包括9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			for(i = 0 ;i < 8;i++)
			{
				psam_mac[i] = data[1 + i];
			}
		}
		return SUCCESS; 
	}
	return -3 + ret*10;
}

/*
* 函数名称：	psamCheckMAC1()
* 功能描述：	校验MAC1C
* 参数列表：	无
* 返回结果：	
*/
int psamCheckMAC1(uint8 psam_slot, uint8* payserial, uint8 *rnd, uint8 trans_mode, uint8 *pay_val, uint8 *time)
{
	uint8 apdulist;//待发命令
	uint8 apdu[128];
	int datalist;//返回数据
	uint8 data[128];
	int ret;
	int len;
	int i;
	apdulist = 1;
	len = 0;
	apdu[len++] = 41;
	apdu[len++] = 0x80;
	apdu[len++] = 0x70;
	apdu[len++] = 0x00;
	apdu[len++] = 0x00;
	apdu[len++] = 0x24;
	apdu[len++] = rnd[0]; // 四字节随机数
	apdu[len++] = rnd[1];
	apdu[len++] = rnd[2];
	apdu[len++] = rnd[3];
	apdu[len++] = payserial[0]; // 2字节用户卡交易序号
	apdu[len++] = payserial[1];
	if(pay_val!=NULL)
	{
		apdu[len++] = pay_val[0]; // 消费金额
		apdu[len++] = pay_val[1];
		apdu[len++] = pay_val[2];
		apdu[len++] = pay_val[3];
	} 
	else
	{
		apdu[len++] = 0; // 消费金额
		apdu[len++] = 0;
		apdu[len++] = 0;
		apdu[len++] = 0;
	}
	if(trans_mode==0)
	{
		apdu[len++] = 0x06;	//标识交易类型，传统
	}
	else
	{
		apdu[len++] = 0x09; // 标识交易类型，复合
	}
	//交易日期和时间
	apdu[len++] = time[0];
	apdu[len++] = time[1];
	apdu[len++] = time[2];
	apdu[len++] = time[3];
	apdu[len++] = time[4];
	apdu[len++] = time[5];
	apdu[len++] = time[6];
	//卡片内部编号 分散因子
	apdu[len++] = 0x01;
	apdu[len++] = 0x00;
	apdu[len++] = icc_pib.icc0015[12]; // 8字节用户卡应用序列号
	apdu[len++] = icc_pib.icc0015[13];
	apdu[len++] = icc_pib.icc0015[14];
	apdu[len++] = icc_pib.icc0015[15];
	apdu[len++] = icc_pib.icc0015[16];
	apdu[len++] = icc_pib.icc0015[17];
	apdu[len++] = icc_pib.icc0015[18];
	apdu[len++] = icc_pib.icc0015[19];
	apdu[len++] = icc_pib.icc0015[0];  // 8字节区域分散代码
	apdu[len++] = icc_pib.icc0015[1];
	apdu[len++] = icc_pib.icc0015[2];
	apdu[len++] = icc_pib.icc0015[3];
	apdu[len++] = icc_pib.icc0015[0];
	apdu[len++] = icc_pib.icc0015[1];
	apdu[len++] = icc_pib.icc0015[2];
	apdu[len++] = icc_pib.icc0015[3];
	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
	{
		return -1 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0];	//返回数据的长度
		//注意psam操作的长度不包括9000
		if(data[len + 1] == 0x90 && data[len+2] == 0x00) 
		{
			for(i = 0 ;i < 4;i++)
			{
				psam_pib.TransSerial[i] = data[1 + i];
			}
			for(i = 0 ;i < 4;i++)
			{
				psam_pib.MAC1[i] = data[5 + i];
			}
		}
		return SUCCESS;
	}	
	return -3 + ret*10;
}

/*
* 函数名称：	psamCheckMAC2()
* 功能描述：	校验MAC2
* 输入参数：	
* 输出参数：  无
* 返回值：	0--成功，非0--失败
*/
int psamCheckMAC2(uint8 psam_slot, uint8* mac)
{
	int ret = SUCCESS;
	uint8 apdulist = 1;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	uint16 len = 0, i = 0;
	apdu[len++] = 9;
	apdu[len++] = 0x80;
	apdu[len++] = 0x72;
	apdu[len++] = 0x00;
	apdu[len++] = 0x00;
	apdu[len++] = 0x04;
	apdu[len++] = mac[0];	//四字节随机数
	apdu[len++] = mac[1];
	apdu[len++] = mac[2];
	apdu[len++] = mac[3];
	ret = PSAM_CHANNEL_rq(psam_slot, apdulist, apdu) ;
	if(ret != SUCCESS)
	{
		return -1 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist, data, 1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0];	//返回数据的长度
		//注意psam操作的长度不包括9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			return SUCCESS;
		} 
		return -2;
	}
	return -3 + ret*10;
}

/*
* 函数名称：	psamSetKeyindex()
* 功能描述：	设置密钥版本号
* 输入参数：	
* 输出参数：  无
* 返回值：	
*/
int psamSetKeyindex(uint8 psam_slot, uint8 keyindex)
{
	uint8 apdulist;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	int ret, len;
	apdulist = 1;
	apdu[0] = 0x06;	
	apdu[1] = 0xd0;				
	apdu[2] = 0xda;
	apdu[3] = 0x00;
	apdu[4] = 0x00;
	apdu[5] = 0x01;
	apdu[6] = keyindex;
	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
	{
		return -1 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0]; 
		//注意psam操作的长度不包括9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			return SUCCESS;
		}	
		return -2;
	}
	return -3 + ret*10;
}

/*
* 函数名称：	psamAccesscredentials()
* 功能描述：	
* 输入参数：	
* 输出参数：  无
* 返回值：	
*/
int psamAccesscredentials(uint8 psam_slot, const uint8 *contractserialnum, uint8 contractserialnumlen, uint8 keyversionid, const uint8 *Rand, uint8 Randlen)
{
	int i;
	uint8 apdulist;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	int ret, len, pos, accessCredentialsLen = 17;
	apdulist = 1;
	//生成指令
	if(Randlen != 8)
	{
		return -1;
	}
	if(contractserialnumlen != 8)
	{
		return -2;
	}
	pos = 0;
	apdu[pos++] = 6 + accessCredentialsLen;	
	apdu[pos++] = 0xd0;				
	apdu[pos++] = 0x80;
	apdu[pos++] = 0x00;
	apdu[pos++] = 0x00;
	apdu[pos++] = accessCredentialsLen;
	for(i = 0; i < contractserialnumlen; i++)	//8字节合同序列号
	{
		apdu[pos++] = contractserialnum[i];
	}
	apdu[pos++] = keyversionid;	//密钥版本号
	for(i = 0; i < Randlen; i++)	//RandOBU
	{
		apdu[pos++] = Rand[i];
	}
	apdu[pos++] = 0x0e;
	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
	{	
		return -3 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0]; 
		//注意psam操作的长度不包括9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00)) 
		{
			for(i = 0; i < 8; i++)
			{
				psam_pib.accessCredentials[i] = data[i + 1];
			}
			for(i = 0; i < 6; i++)
			{
				psam_pib.PSAMNO[i] = data[i + 9];
			}
			return SUCCESS;
		} 
		return -4;
	}
	return -5 + ret*10;

}

/*
* 函数名称：	psamGetChallenge()
* 功能描述：	
* 输入参数：	
* 输出参数：  无
* 返回值：	
*/
int psamGetChallenge(uint8 psam_slot)
{
	int i, ret, len;
	int datalist;	//返回数据
	uint8 apdulist;	//待发命令
	uint8 apdu[128];
	uint8 data[128];
	apdulist = 1;
	apdu[0] = 5;	
	apdu[1] = 0xd0;				
	apdu[2] = 0x84;
	apdu[3] = 0x00;
	apdu[4] = 0x00;
	apdu[5] = 0x09;
	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
	{	
		return -1 + ret*10;
	}
	ret = PSAM_CHANNEL_rs(&datalist,data,1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0]; 
		//注意psam操作的长度不包括9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			for(i = 0; i < 8; i++)
			{
				psam_pib.randRsuForAuthen[i] = data[i+1];	//用于信息鉴别的随机数
			}
			psam_pib.randRsuForAuthenIndex = data[9];	//随机数序列号
			return SUCCESS;
		} 
		return -2;

	}
	return -3 + ret*10;
}

/*
* 函数名称：	psamReadBinarayFile()
* 功能描述：	
* 参数列表：	无
* 返回结果：	
*/
int psamReadBinarayFile(uint8 psam_slot, uint8 fid,uint8 offset,uint8 length)
{
	uint8 apdulist;	//待发命令
	uint8 apdu[128];
	int datalist;	//返回数据
	uint8 data[128];
	int ret, len, i,pos;
	apdulist = 1;
	pos = 0;
	//读文件内容
	apdu[pos++] = 0x05;	//7,进入ETC应用目录
	apdu[pos++] = 0x00;
	apdu[pos++] = 0xb0;
	if(fid == 0x16)
	{
		apdu[pos++] = 0x96;
	}
	else if(fid = 0x1a)
	{
		apdu[pos++] = 0x9a;
	}
	apdu[pos++] = offset;
	apdu[pos++] = length;
	ret = PSAM_CHANNEL_rq(psam_slot,apdulist,apdu) ;
	if(ret != SUCCESS)
	{
		return -1 + ret*10;

		return ret;
	}
	ret = PSAM_CHANNEL_rs(&datalist, data, 1000);
	if(ret == SUCCESS)  
	{ 
		len = data[0]; 
		//注意psam操作的长度不包括9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			for(i = 0; i < len; i++)
			{
				psam_read_info[i] = data[i + 1];
			}
			return SUCCESS; 
		}
		return -2;
	}
	return -3 + ret*10;
// 
// 	return 1;
}

/*
* 函数名称：	PSAM_RESET_rq()
* 功能描述：	psam复位
* 参数列表：	无
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int PSAM_RESET_rq(int PSAMSlot,int Baud)
{
	int ret = 0;    
	uint8 chk = 0;
	int cmdtype = 0xf0; //根据psam槽号的不同而不同
	uint32 slen = 0, i = 0;
	uint8 sbuf[BUF_LEN] = {0};
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl; //帧序号 
	cmdtype |= PSAMSlot;	//PSAMSlot = 0x01--槽号1,PSAMSlot=0x02--槽号2，PSAMSlot = 0x03--槽号3
	sbuf[slen++] = cmdtype; //0xf1、0xf2、0xf3
	sbuf[slen++] = 0x00;	//psam复位0x00
	sbuf[slen++] = (Baud >> 24) & 0xff;
	sbuf[slen++] = (Baud >> 16) & 0xff;
	sbuf[slen++] = (Baud >> 8) & 0xff;
	sbuf[slen++] = Baud & 0xff;
	chk = sbuf[2];
	for(i = 3; i < slen; i++)
	{
		chk = chk ^ sbuf[i];
	}
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;
	g_com_tx_len = pkt_code(&sbuf[0], &g_com_tx_buf[0], slen);
	ret = serial_send();	//调用底层发送函数
	sem_init(&g_sem_psamreset, 0, 0);
	is_psamreset_recved = 0;	//收到或者超时退出都不再收数
	return ret;
}

/*
* 函数名称：	PSAM_RESET_rs()
* 功能描述：	
* 参数列表：	无
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int PSAM_RESET_rs(int time_out, char* PSAMNO)
{
	int ret = 0;
	int pos = 0, i = 0, data_len = 0;
    for(i = 0; i < time_out; i++)
	{
		ret = sem_trywait(&g_sem_psamreset);
		if(ret == 0)
		{
			break;
		}
		else
		{
			if(i == time_out - 1)
			{
				is_psamreset_recved = 1;	//收到或者超时退出都不再收数
				sem_destroy(&g_sem_psamreset);
				return -1;	//超时，没有收到PSAM回复帧
			}
			usleep(20000);
		}
	}
	is_psamreset_recved = 1;	//收到或者超时退出都不再收数
	sem_destroy(&g_sem_psamreset);
	pos = 3;	//去掉帧序号、命令号0xe3、原语类型0x01
	//解析psamchannel_rs
	if(g_com_rx_buf[pos++] == 1) //后面数据的有效性，0x00--有效，0x01--无效,防止无psam卡的情况
	{
		return -2;
	}
	for(i = 0; i < 6; i++)
	{
		PSAMNO[i] = g_com_rx_buf[pos++];	//保存PSAM终端编号,6个字节
	}
	return SUCCESS; 
}

/*
* 函数名称：	PSAM_CHANNEL_rq()
* 功能描述：	PSAM通道指令 
* 参数列表：	无
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int PSAM_CHANNEL_rq(int PSAMSlot, int APDUList, char *APDU) 
{
	int ret = 0;
	uint8 chk = 0;
	uint8 sbuf[BUF_LEN] = {0}; 
	uint16 slen = 0, len_pos = 0, i = 0;
	int cmdtype = 0xf0; //根据psam槽号的不同而不同
	int apdulen = 0;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl;	//帧序号  
	cmdtype |= PSAMSlot;	//PSAMSlot = 0x01--槽号1,PSAMSlot=0x02--槽号2，PSAMSlot = 0x03--槽号3
	sbuf[slen++] = cmdtype; //0xf1、0xf2、0xf3
	sbuf[slen++] = 0x01;	//psam非复位都是0x01
	len_pos = slen++; 		//留出总长度位置 
	sbuf[slen++] = APDUList;
	//计算APDU总长度 
	apdulen = 0; 
	for(i = 0; i < APDUList; i++)
	{
		apdulen += APDU[apdulen] + 1;
	} 
	for(i = 0; i < apdulen; i++)
	{ 
		sbuf[slen++] = APDU[i]; 
	}
	sbuf[len_pos] = slen - len_pos - 1;
	chk = sbuf[2];
	for(i = 3; i < slen; i++)
	{
		chk = chk ^ sbuf[i];
	}
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;
	g_com_tx_len = pkt_code(&sbuf[0], &g_com_tx_buf[0], slen);
	ret = serial_send();	//调用底层发送函数
	sem_init(&g_sem_psamchannel, 0, 0);
	is_psamchannel_recved = 0;	//收到或者超时退出都不再收数
	return ret;
} 

/*
* 函数名称：	PSAM_CHANNEL_rs()
* 功能描述：	PSAM通道指令返回 
* 参数列表：	无
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int PSAM_CHANNEL_rs(int *DATALIST, char *Data, int time_out) 
{ 
	int ret = 0; 
	uint8 psamchannel_rs_len = 0;
	uint16 data_len = 0, pos = 0, i = 0;
    for(i = 0; i < time_out; i++)
	{
		ret = sem_trywait(&g_sem_psamchannel);
		if(ret == SUCCESS)
		{
			break;
		}
		else
		{
			if(i == time_out - 1)
			{
				is_psamchannel_recved = 1;	//收到或者超时退出都不再收数
				sem_destroy(&g_sem_psamchannel);
				//DEBUGP("PSAM_CHANNEL_rs TIME OUT ERR, ret = %d\n", ret);
				return -1;	//超时，没有收到vst
			}
			usleep(20000);
		}
	}
	is_psamchannel_recved = 1;	//收到或者超时退出都不再收数
	sem_destroy(&g_sem_psamchannel);
	pos = 3;	//去掉帧序号、命令号0xe3、原语类型0x01
	//解析psamchannel_rs
	if(g_com_rx_buf[pos++] == 1)	//后面数据的有效性，0x00--有效，0x01--无效,防止无psam卡的情况
	{
		return -2;
	}
	psamchannel_rs_len = g_com_rx_buf[pos++];
	*DATALIST = g_com_rx_buf[pos++];
	data_len = psamchannel_rs_len + 5 - pos;
	for(i = 0; i < data_len; i++)
	{
		memcpy(Data, &g_com_rx_buf[pos], data_len);
	}
	pos += data_len;
	return SUCCESS; 
}


