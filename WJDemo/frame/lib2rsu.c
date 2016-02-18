#import "CToOcType.h"
#include "lib2rsu.h"
#include "common.h"
#include "lib2hd.h"
#include "psam.h"


int len_vehicleinfo;

int IsVst(uint8 *pkg);
/* 
BST 
功能描述： RSU广播BST给OBU 
输入参数： bst_type--BST类型
		 beacon_id--由1字节manufacturerID和3字节individualID组成 
		 unix_time--UNIX时间，长度为4字节，从2000年1月1日00:00起，以秒计算的累计 
		 profile--配置选项，长度为1字节，配置值如下： 00H--配置0(A类)的信道1	01H--配置0(A类)的信道2 
											    10H--配置1(B类)的信道1	11H--配置1(B类)的信道2 
		 obu_init_mode--OBU发行操作的模式
输出参数： 无 
返回值： 0--成功, 非0--失败  
*/ 
int INITIALISATION_rq(int bst_type, char *beacon_id, char *unix_time, int profile, int obu_init_mode)
{ 
	int i;
	int slen;
	int len_pos; 
	int ret;    
	uint8 chk = 0;
	uint8 sbuf[512]; 
	memset(sbuf,0,512);
	slen = 0; 
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl+1)%16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl; // 帧序号  
	sbuf[slen++] = 0xf0; // 命令号,bst以及action原语 
	sbuf[slen++] = 0x00; //bst
	len_pos = slen++;    //留出总长度位置 

	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0x50;
	sbuf[slen++] = 0x03;
	sbuf[slen++] = 0x91; //段字头 
	sbuf[slen++] = 0xc0; //bst标识 
	sbuf[slen++] = beacon_id[0]; 
	sbuf[slen++] = beacon_id[1]; 
	sbuf[slen++] = beacon_id[2]; 
	sbuf[slen++] = beacon_id[3]; 
	sbuf[slen++] = unix_time[0]; 
	sbuf[slen++] = unix_time[1]; 
	sbuf[slen++] = unix_time[2]; 
	sbuf[slen++] = unix_time[3]; 
	sbuf[slen++] = profile; 
	sbuf[slen++] = 0x01;	//MandApplications 无扩展，1个应用，取值1
//	sbuf[slen++] = 0x02;	//北京在线发行协议要求
#if 1//北京协议
	if(bst_type == BST_ISSUE_BJ)
	{
		if(obu_init_mode == 0x2f)
			sbuf[slen++] = 0x41; //paramter存在,AID = 0北京标示发行
		else
			sbuf[slen++] = 0x40; //paramter存在,AID = 0北京标示发行
		sbuf[slen++] = obu_init_mode; 
		sbuf[slen++] = 0x00; //profilelist,无扩展
	}

#endif
#if 1
	if(bst_type == BST_TRADE_GB)
	{
		sbuf[slen++] = 0x41; //paramter存在
		/* 交易类型说明
		 icctransmode：0x87--储值卡和记账卡支持传统消费、复合消费，优先采用复合消费交易模式，支持OBU预处理快速交易模式
		               0xaf--储值卡和记账卡都仅支持复合消费交易模式，优先采用复合消费交易模式，支持OBU预处理快速交易模式
		               0xd1--储值卡和记账卡都仅支持传统消费交易模式，优先采用传统消费交易模式，支持OBU预处理快速交易模式
		               0xcb--储值卡支持传统消费交易模式，记账卡支持复合消费交易模式，优先采用复合消费交易模式，支持OBU预处理快速交易模式
		               0xb5--储值卡支持复合消费交易模式，记账卡支持传统消费交易模式，优先采用复合消费交易模式，支持OBU预处理快速交易模式*/
		sbuf[slen++] = 0xaf; 
		sbuf[slen++] = 0x29;
		/*是否读取0002文件:1
		  是否读取0012文件:0
		  是否读取0015文件:1
		  是否读取0019文件:0
		  fill:0000*/
		sbuf[slen++] = 0xa0;
		sbuf[slen++] = 0x1a;
		sbuf[slen++] = 0x00; //0002文件偏移量
		sbuf[slen++] = ICC_0002_LEN; //0002文件长度
		sbuf[slen++] = 0x00; //0015文件偏移量
		sbuf[slen++] = ICC_0015_LEN; //0015文件长度
	}
	else if(bst_type == BST_ISSUE_NOPARAM)
	{
		sbuf[slen++] = 0x01;//paramter不存在
	}
	else if(bst_type == BST_ISSUE_NOREAD)
	{
		sbuf[slen++] = 0x41;//paramter存在
		sbuf[slen++] = 0x87; 
		sbuf[slen++] = 0x29;
		sbuf[slen++] = 0x00;
		sbuf[slen++] = 0x1a;
	}
	else if(bst_type == BST_ISSUE_PREREAD)
	{
		sbuf[slen++] = 0x41;//paramter存在
		sbuf[slen++] = 0x87; 
		sbuf[slen++] = 0x29;
		sbuf[slen++] = 0xb0;
		sbuf[slen++] = 0x1a;
		sbuf[slen++] = 0x00;
		sbuf[slen++] = 0x00;
		sbuf[slen++] = 0x00;
		sbuf[slen++] = 0x00;
		sbuf[slen++] = 0x00;
		sbuf[slen++] = 0x00;
	}
	else if(bst_type == BST_TRADE_ZYL)
	{
		sbuf[slen++] = 0x51;	//Dsrc-did不显示，Parameter存在，AID=17（武汉路桥ETC应用）
		sbuf[slen++] = 0x01;	//A类交易模式，A型单片有源电子标签（含ESAM）
		sbuf[slen++] = 0x00; 	//KeyIDforEncrypt
		sbuf[slen++] = 0x06; 	//CodeInx
	}
	else if(bst_type == BST_ISSUE_ZYL)
	{
		sbuf[slen++] = 0x11;
	}

	sbuf[slen++] = 0x00;
#endif

	sbuf[len_pos] = slen - len_pos - 1;

	chk = sbuf[2];
	for (i = 3; i < slen; i++) {
		chk = chk ^ sbuf[i];
	}
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;
	memset(&g_com_rx_buf,0,sizeof(g_com_rx_buf));
	g_com_tx_len = pkt_code(&sbuf[0], &g_com_tx_buf[0], slen);
	if (g_bst_type == BST_TRADE_ZYL) {
		sem_init(&g_sem_prwrq, 0, 0);
		is_prwrq_recved = 0;	//初始没有收到
	} else {
		sem_init(&g_sem_vst, 0, 0);
		is_vst_recved = 0;	//初始没有收到
	}
	ret = serial_send();	//调用底层发送函数





	int re_sendB1_count=g_timeout_max;
	while(re_sendB1_count>0)
	{
		 
//		ret=serial_recv_sem(&g_sem_vst,3);hmh
		if(ret != SUCCESS)
		{
			if((0 != checktimeout()))
				break;
			ret=serial_send();
			re_sendB1_count--;
		}else
		{
			break;
		}
	}



	return ret; 
} 
 
/* 
VST 
功能描述： 接收OBU返回的VST 
输入参数： time_out--超时时间计数，以20ms为单位
输出参数： obu_mac--OBU的MACID
	     sys_info--OBU的系统信息，26字节
   		 icc_info--预读的IC卡信息
		 rnd_info--随机数
		 reset_info--复位信息
		 obu_status--OBU状态
返回值： 0--成功, 非0--失败  
*/ 
int INITIALISATION_rs(char *obu_mac, char *sys_info, char *icc_info, char *rnd_info, char *reset_info, char *obu_status, int time_out) 
{ 
	int len_icc_info = 0;
	int pos,i,ret; 
	uint8 vst_len = 0;
	uint8 mac_control = 0;
	uint8 dsrc_op = 0;
	uint8 container_ops = 0;
	char InfoTmp[100];
	memset(icc_info,0x00,sizeof(icc_info));//add by wll 2013-5-14 9:40:13
//    for(i = 0; i < time_out; i++)
//	{
//		ret = sem_trywait(&g_sem_vst);
//		if (ret == 0)
//		{
//			break;
//		}
//		else
//		{
//			if(i == time_out - 1)
//			{
//				is_vst_recved = 1;	//收到或者超时退出都不再收数
//				sem_destroy(&g_sem_vst);
//				return -1;	//超时，没有收到vst
//			}
//			usleep(20000);
//		}
//	}
//	is_vst_recved = 1;	//收到或者超时退出都不再收数
//	sem_destroy(&g_sem_vst);
	if(IsVst(g_com_rx_buf) != 0)	//判断是否是VST
	{
		return -2;
	}
	pos = 3;	//去掉帧序号、命令号0xe0、原语类型0x00
	vst_len = g_com_rx_buf[pos++];
	//解析vst
	if(g_bst_type == BST_TRADE_ZYL)
	{
		for(i = 0;i < 4;i++)
		{
			obu_mac[i] = g_com_rx_buf[pos++];
		}
		mac_control = g_com_rx_buf[pos++];	//mac控制域0xc0
		pos++;	//llc控制域0x03
		pos++;	//字段字头0x91
		pos++;	//vst标识0xd0
		pos++;	//信道号
		pos++;	//0xd1,aid的填充字节
		pos++;	//did,0x03
		for(i = 0;i < 8;i++)
		{
			vst.RandOBE[i] = g_com_rx_buf[pos++];	//8字节的随机数
		}
		for(i = 0;i < 12;i++)
		{
			vst.vehicleLicensePlateNumber[i] = g_com_rx_buf[pos++];	//12字节的车牌号
		}
		vst.vehicleType = g_com_rx_buf[pos++];	//车型
		vst.vehicleUserType = g_com_rx_buf[pos++];	//车辆用户类型
		for(i = 0; i < 8; i++)
		{
			vst.ContractSerialNumber[i] = g_com_rx_buf[pos++];	//合同序列号
		}
		for(i = 0; i < 4; i++)
		{
			vst.ObuConfigurationMacID[i] = g_com_rx_buf[pos++];	//OBU MACID
		}
		for(i = 0; i < 3; i++)
		{
			vst.obustatus[i] = g_com_rx_buf[pos++];	//OBU状态信息
		}
	
	}/* end if(g_bst_type == BST_TRADE_ZYL) */
	else if(g_bst_type == BST_ISSUE_ZYL)
	{
		for(i = 0;i < 4;i++)
		{
			obu_mac[i] = g_com_rx_buf[pos++];
		}
		mac_control = g_com_rx_buf[pos++];	//mac控制域0xc0
		pos++;	//llc控制域0x03
		pos++;	//字段字头0x91
		pos++;	//vst标识0xd0
		pos++;	//信道号
		pos++;	//0xd1,aid的填充字节
		pos++;	//did,0x03
		//8字节的合同序列号
		for(i = 0; i < 8; i++)
		{
			g_contract_sn[i] = g_com_rx_buf[pos++];
		}
		pos += 4;	//4字节的obu macid,3字节的obustatus
		for(i = 0; i < 3; i++)
		{
			obu_status[i] = g_com_rx_buf[pos++];	//obu状态信息
		}
	}/* end else if(g_bst_type == BST_ISSUE_ZYL) */
	else
	{
		for(i = 0; i < 4; i++)
		{
			obu_mac[i] = g_com_rx_buf[pos++];
		}
		mac_control = g_com_rx_buf[pos++]; //mac控制域0xc0
		pos++;	//llc控制域0x03
		if(mac_control == 0xe0)
		{
			pos++;	//状态子域
		}
		pos++;	//字段字头0x91
		pos++;	//vst标识0xd0
		pos++;	//信道号
		pos++;	//应用列表数0x01
		dsrc_op = g_com_rx_buf[pos++];	//从高到低,分别标识did显示、parameter存在、aid
		if(Getbit(dsrc_op,7))
		{
			pos++;	//did
		}
		if(Getbit(dsrc_op,6))
		{
			container_ops = g_com_rx_buf[pos++];	//从高到抵，rndOBE_op、privateinfo_op、gbiccinfo_op、reset_info_op
			pos++;	//容器类型0x27或者0x20
			//系统信息容器
			for(i = 0; i < 26; i++)
			{
				sys_info[i] = g_com_rx_buf[pos++]; 
			}
			//随机数容器
			if(Getbit(container_ops,7))
			{
				pos++;	//容器类型0x1d
				for(i = 0; i < 8; i++)
				{
					rnd_info[i] = g_com_rx_buf[pos++];
				}
			}
			//privateinfo,目前不存在
			if(Getbit(container_ops,6))
			{
				/* do nothing */
			}
			//gbiccinfo_op
			if(Getbit(container_ops,5))
			{
				if(g_bst_type == BST_TRADE_GB)	//交易带预读
				{
					len_icc_info = ICC_0002_LEN + ICC_0015_LEN;
					pos++;	//容器类型0x28
					icc_info[0] = len_icc_info;
					for(i = 1; i <= len_icc_info; i ++)
					{
						icc_info[i] = g_com_rx_buf[pos++];
					}
					print_info_Info(icc_info,len_icc_info + 1,COM);
				}
				else if(g_bst_type == BST_ISSUE_PREREAD)	//发行带预读
				{	
					if (g_com_rx_buf[pos] == 0x28)
					{
						pos++;
					}
				}
				else if (g_bst_type == BST_ISSUE_BJ)	//北京发行
				{
					icc_info[0] = 62;
					for(i = 0;i < 62;i++)
					{
						icc_info[i+1] = g_com_rx_buf[pos++];
					}
				}
			}
			//复位信息容器,北京发行专用
			//sprintf(InfoTmp,"fu wei xin xi, container_ops = %X\n", container_ops);
			//print_info_Info(InfoTmp,strlen(InfoTmp),COM);
			//复位信息容器,北京发行专用
			if(Getbit(container_ops,4))
			{
				pos++;	//容器类型0x50
				for(i = 0; i < 15; i++)
				{
					reset_info[i] = g_com_rx_buf[pos++];
				}
				sprintf(InfoTmp,"fu wei xin xi, reset_info = %X %X ..... %X %X\n", reset_info[0],reset_info[1], reset_info[13],reset_info[14]);
				print_info_Info(InfoTmp,strlen(InfoTmp),COM);

			}

		}/* end if(Getbit(dsrc_op,6)) */
		pos += 4;//obu_mac
		pos++; //硬件版本号
		*obu_status = g_com_rx_buf[pos++];//obu状态信息
		pos++; //obu_status中的保留字节
	}	
 
	if(vst_len != pos - 4)
	{
		 
		return -3;
	}
	return 0;
}

/*
* 函数名称：	GetSecure_rq()
* 功能描述：	GetSecure_Request
* 参数列表：	无
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int GetSecure_rq(char *AccessCredentials, int offset, int length, char *RandRSU, int KeyIDforEncryptOp, int KeyIDforEncrypt) 
{
	int ret = 0;
	uint8 chk = 0;
	uint8 sbuf[BUF_LEN] = {0}; 
	uint16 slen = 0, len_pos = 0, i = 0;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl;	//帧序号  
	sbuf[slen++] = 0xf0;	//命令号,bst以及action原语 
	sbuf[slen++] = 0x01;	//getsecure_rq
	len_pos = slen++;		//留出总长度位置
	//组包getsecure_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;	//mac控制域
	sbuf[slen++] = 0x77 | g_u8LLCFlag ;	//llc控制域,0xf7
	if(g_u8LLCFlag == 0x80)
		g_u8LLCFlag = 0x00;
	else
		g_u8LLCFlag = 0x80;

 

	sbuf[slen++] = 0x91;	//段字头
//	if(vst.sysinfo[9] == 0x16||vst.sysinfo[9] == 0x12)
	{
		sbuf[slen++] = 0x05;
	}
//	else
//	{
//		sbuf[slen++] = 0x0d;
//	}
//	sbuf[slen++] = 0x05;	//action标识0x05,若加上发行accessCredentialsOp存在,则为0x0d
//	sbuf[slen++] = 0x0d;
	sbuf[slen++] = 0x01;	//DID
	sbuf[slen++] = 0x00;	//actiontype
//	if(vst.sysinfo[9] == 0x16||vst.sysinfo[9] == 0x12)
//	{
//
//	}
//	else
//	{
//		for(i = 0;i < 8;i++)
//		{
//			sbuf[slen++] = AccessCredentials[i];
//		}
//	}
	sbuf[slen++] = 0x14;	//container标识
	sbuf[slen++] = KeyIDforEncryptOp;	//0x80-加密密钥标识存在，0x00-不存在
	sbuf[slen++] = 0x01;	//FID
	sbuf[slen++] = (offset >> 8) & 0xff;
	sbuf[slen++] = offset & 0xff;
	sbuf[slen++] = length;
	for(i = 0;i < 8;i++)
	{
		sbuf[slen++] = RandRSU[i];
	}
	if(KeyIDforEncryptOp == 0)
	{
		sbuf[slen++] = 0x12;
		g_veh_file_sign = 0;//明文方式,
	}else
	{
		sbuf[slen++] = 0x00;	//keyIDforAuthen
		sbuf[slen++] = KeyIDforEncrypt;
		g_veh_file_sign = 1;	//密文方式,北京协议是按着协议类型分的
	}
//	sbuf[slen++] = 0x00;	//keyIDforAuthen
//	if(KeyIDforEncryptOp == 0x80)
//	{
//		sbuf[slen++] = KeyIDforEncrypt;
//		g_veh_file_sign = 1;	//密文方式,北京协议是按着协议类型分的
//	}
	sbuf[len_pos] = slen - len_pos - 1;
	chk = sbuf[2];
	for(i = 3;i < slen;i++)
	{
		chk = chk ^ sbuf[i];
	}
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;
	len_vehicleinfo = length;	//用于解析getsecure_rs
	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);
	ret = serial_send();	//调用底层发送函数
	sem_init(&g_sem_getsecure, 0, 0);
	is_getsecure_recved = 0;	//初始没有收到
	return ret; 
}

static uint8 IsGetseq_rs(uint8 *pkg)
{
	if(!Getbit(pkg[12], 7) && !Getbit(pkg[12], 6) && !Getbit(pkg[12], 5) && Getbit(pkg[12], 4))
	{
		if(Getbit(pkg[12],3))
		{
			if(pkg[14] == 0x15)
			{
				return 0;
			}
			else
			{
				return -3;
			}
		}
		else
		{
			return -2;
		}
	}
	else
	{
		return -1;
	}
}

/*
* 函数名称：	GetSecure_rs()
* 功能描述：	GetSecure_Response
* 参数列表：	无
* 返回结果：	0	--	成功
*			非0	--	失败
*/
int GetSecure_rs(int *length, char *File, char *authenticator, int time_out) 
{
	int ret = SUCCESS;
	uint16 pos = 0, i = 0; 
	uint8 getsecure_rs_len = 0, action_para;
    for(i = 0; i < time_out; i++)
	{
		ret = sem_trywait(&g_sem_getsecure);
		if(ret == SUCCESS)
		{
			break;
		}
		else
		{
			if(i == time_out - 1)
			{
				is_getsecure_recved = 1;	//收到或者超时退出都不再收数
				sem_destroy(&g_sem_getsecure);
				return -1;	//超时，没有收到vst
			}
			usleep(20000);
		}
	}
	is_getsecure_recved = 1;	//收到或者超时退出都不再收数
	sem_destroy(&g_sem_getsecure);
	if(IsGetseq_rs(g_com_rx_buf) != SUCCESS)	//判断是否是GetSecure_rs
	{
		return -2;
	}
	pos = 3;	//去掉帧序号、命令号0xe0、原语类型0x01
	getsecure_rs_len = g_com_rx_buf[pos++];
	//解析getsecure_rs
	pos += 4;	//跳过macid
	pos++;		//mac控制域0xe0
	pos++;		//llc控制域
	pos++;		//状态子域
	pos++;		//段子头
	action_para = g_com_rx_buf[pos++];	//action标识0x18
	pos++;		//DID 0x01
	if(Getbit(action_para,3))
	{
		pos++;	//容器类型0x15
		pos++;	// FID 0x01
		//车辆密文
//		if(g_veh_file_sign)
//		{
			len_vehicleinfo = len_vehicleinfo + 8 + 1;	//明文长度加8个字节鉴别码，1个字节长度信息
			if(len_vehicleinfo % 8)
			{
				len_vehicleinfo = 8 *(len_vehicleinfo / 8 + 1);
			}
//		}
//		else
//		{
//
//		}

		memcpy(File, &g_com_rx_buf[pos], len_vehicleinfo);
		pos += len_vehicleinfo;
		*length = len_vehicleinfo;
		if((vst.macid[0] == 0x03)&&(g_OBU_ContractVersion == 0x00))
		{}
		else
		{
			for(i = 0;i < 8;i++)
			{
				authenticator[i] = g_com_rx_buf[pos++];
			}
		}
//		char InfoTmp[100];
//		sprintf(InfoTmp,"5555555555555555after_decrypt_mac,pos = %d\n",pos);
//		print_info_Info(InfoTmp,strlen(InfoTmp),COM);
////		g_com_rx_len = 128;
//		g_com_rx_len = 8;
//		for(i = 0;i<g_com_rx_len;i++)
//		{
//			g_com_rx_buf[i] = authenticator[i];
//		}
//		print_info(COM, RECV);
	}/* end if(Getbit(action_para,3)) */
	pos++;	//ReturnStatus
	//如果是明文则不判长度

//	if(g_esam_type == 1)
//	{
//		if(getsecure_rs_len != pos - 4)
//		{
//			return -3;
//		}
//	}
	return SUCCESS; 
}

/* 
TransferChannel_Request 
功能描述：RSU通过透明通道对OBU进行操作 
输入参数：DID--要读取的OBU应用号，ETC应用为1，标识站应用为2 
		ChannelID--通道ID号 
		APDULIST--对通道操作的APDU命令数 
		APDU--对通道操作的APDU命令，按顺序为指令1长度(1字节)、指令1、指令2长度(1字节)、指令2…… 
输出参数：	无 
返回值：	0--命令执行成功 
		-100--超时，RSU设备无响应 
		-1000--输入参数错误 
		-1001--设备未打开 
		-2000--其他错误 
*/ 
int TransferChannel_rq(int DID,int ChannelID,int APDULIST,char *APDU)
{
	int ret = 0;
	uint8 chk = 0;
	uint8 sbuf[BUF_LEN] = {0}; 
	uint16 slen = 0, len_pos = 0, i = 0, apdulen = 0;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl; //帧序号  
	sbuf[slen++] = 0xf0; 	//命令号,bst以及action原语 
	sbuf[slen++] = 0x02;	//transferchannel_rq
	len_pos = slen++;		//留出总长度位置
	//组包transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;	//mac控制域
//	sbuf[slen++] = 0x77;	//llc控制域  0xf7
	sbuf[slen++] = 0x77 | g_u8LLCFlag ;	//llc控制域,0xf7
	if(g_u8LLCFlag == 0x80)
		g_u8LLCFlag = 0x00;
	else
		g_u8LLCFlag = 0x80;
	sbuf[slen++] = 0x91;	//段字头 
	sbuf[slen++] = 0x05;	//action标识0x05 
	sbuf[slen++] = DID;		//DID,根目录0x00,应用主目录0x01
	sbuf[slen++] = 0x03;	//actiontype
	sbuf[slen++] = 0x18;	//容器类型
	sbuf[slen++] = ChannelID;	//icc-0x01,esam-0x02
	sbuf[slen++] = APDULIST; 
	//计算APDU总长度
	apdulen = 0; 
	for(i = 0; i < APDULIST; i++)
	{
		apdulen += APDU[apdulen] + 1;
	}
	for(i = 0; i < apdulen; i++)
	{
		sbuf[slen++] = APDU[i]; 
		 
	}
	sbuf[len_pos] = slen - len_pos - 1;
	chk = sbuf[2];
	for(i = 3;i < slen;i++)
	{
		chk = chk  ^ sbuf[i];
	}
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;
	g_com_tx_len = pkt_code(&sbuf[0], &g_com_tx_buf[0], slen);
//	print_info(COM,SEND);
//	if(1 == 1)
//		return -1;
	while(1)
	{
		if(g_IscanSend != 0)//是否需要等待发送串口数据，0：等待；1：直接发送; 2:定义初始值
			break;
		usleep(1000*100);
	}

	memset(&g_com_rx_buf,0,sizeof(g_com_rx_buf));



	is_transfer_recved = 0;	//收到或者超时退出都不再收数

	int re_serial_send_count=g_timeout_max;
	while(re_serial_send_count>0)
	{

			sem_init(&g_sem_transfer,0,0);
			ret = serial_send();	//调用底层发送函数
		 
			if(ChannelID == 0x05 || ChannelID == 0x06){
				 
//				ret=serial_recv_sem(&g_sem_transfer,3);hmh
			}
			else{
			 
//				ret=serial_recv_sem(&g_sem_transfer,3);hmh
			}

 

		if(ret != SUCCESS)
		{
			if((0 != checktimeout()))
				break;
			re_serial_send_count--;
		}else
		{

			break;
		}
		if(re_serial_send_count == 0){
			 
		}
	}
	 
	return ret; 
}
/*
TransferChannel_rq_JinYi_0010_GetRand
功能描述：金溢的00版及10版OBU取随机数函数
输入参数：无
输出参数：无
返回值：	 0--命令执行成功
		非0--数据发送失败
*/
int TransferChannel_rq_JinYi_0010_GetRand()
{
	int ret;
	uint8 chk = 0;
	uint8 sbuf[BUF_LEN] = {0};
	uint16 slen = 0, len_pos = 0, i = 0, apdulen = 0;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl; //帧序号
	sbuf[slen++] = 0xf0; 	//命令号,bst以及action原语
	sbuf[slen++] = 0x02;	//transferchannel_rq
	len_pos = slen++;		//留出总长度位置
	//组包transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	//50 F7 91 04 01 02 04
	sbuf[slen++] = 0x50;
	sbuf[slen++] = 0xF7;
	sbuf[slen++] = 0x91;
	sbuf[slen++] = 0x04;
	sbuf[slen++] = 0x01;//原为0x01
	sbuf[slen++] = 0x02;
	sbuf[slen++] = 0x04;
	sbuf[len_pos] = slen - len_pos - 1;
	chk = sbuf[2];
	for(i = 3;i < slen;i++)
	{
		chk = chk  ^ sbuf[i];
	}
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;
	g_com_tx_len = pkt_code(&sbuf[0], &g_com_tx_buf[0], slen);
	while(1)
	{
		if(g_IscanSend != 0)//是否需要等待发送串口数据，0：等待；1：直接发送; 2:定义初始值
			break;
		usleep(1000*100);
	}
	ret = serial_send();	//调用底层发送函数
	sem_init(&g_sem_transfer,0,0);
	is_transfer_recved = 0;	//收到或者超时退出都不再收数
	return ret;
}
/*
* 函数名称：	IsTransfer_rs()
* 功能描述：	根据目前协议，没有与GetSeq_rs的拼包
* 参数列表：	无
* 返回结果：	0	--	成功
*			非0	--	失败
*/
static uint8 IsTransfer_rs(uint8 *pkg)
{
	if(((vst.macid[0] == 0x02)&&((vst.sysinfo[9] == 0x00)||(vst.sysinfo[9] == 0x10))))
	//金溢1版OBU(00、10版)
	{
		if(pkg[11] == 0x91)
			return 0;
	}
	if(!Getbit(pkg[12], 7) && !Getbit(pkg[12], 6) && !Getbit(pkg[12], 5) && Getbit(pkg[12], 4))
	{
		if(Getbit(pkg[12],3))
		{
			if(pkg[14] == 0x19)
			{
				return 0;
			}
			else
			{
				 
				return -3;
			}
		}
		else
		{
			 
			return -2;
		}
	}
	else
	{
		 
		return -1;
	}
}
/* 
TransferChannel_Response 
功能描述：透明通道操作的响应 
输入参数：	fd--设备句柄号 
		time_out--超时时间，范围1-1000ms 
输出参数：	DID--OBU应用号，ETC应用为1，标识站应用为2 
		ChannelID--通道ID号 
		APDULIST--对通道操作的APDU命令数 
		Data--对通道操作的APDU命令返回数据，按顺序为数据1长度(1字节)、数据1、数据2长度(1字节)、数据2…… 
		ReturnStatus--OBU处理状态 
返回值：	0--命令执行成功 
		-1--超时，RSU设备无响应
		其他--回复帧不是transfer帧
*/ 
int TransferChannel_rs(int *DATALIST, char *Data, int time_out) 
{
	//return 0;//lzb test
	int ret = SUCCESS;
	uint16 pos = 0, i = 0, data_len = 0;
	uint8 transfer_rs_len = 0, action_para;
//    for(i = 0; i < time_out; i++)
//	{
//		ret = sem_trywait(&g_sem_transfer);
//		if(ret == SUCCESS)
//		{
//			break;
//		}
//		else
//		{
//			if(i == time_out - 1)
//			{
//				is_transfer_recved = 1;		//收到或者超时退出都不再收数
//				sem_destroy(&g_sem_transfer);
//				return -1;		//超时，没有收到vst
//			}
//			usleep(20000);
//		}
//	}
//	is_transfer_recved = 1;	//收到或者超时退出都不再收数
//	sem_destroy(&g_sem_transfer);
	ret = IsTransfer_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//判断是否时TransferChannel_rs
	{
		return -2 + ret*10;
	}
	pos = 3;	//去掉帧序号、命令号0xe0、原语类型0x02
	transfer_rs_len = g_com_rx_buf[pos++];
	//解析transfer_rs
	pos += 4;	//跳过macid
	pos++;		//mac控制域0xe0
	pos++;		//llc控制域
	pos++;		//状态子域
	pos++;		//段子头
	action_para = g_com_rx_buf[pos++];	//action标识0x18
	pos++;		//DID 0x01
	//判断容器是否存在
	if(Getbit(action_para,3))
	{
		pos++;	//容器类型0x19
		pos++;	//通道号
		*DATALIST = g_com_rx_buf[pos++];
		data_len = transfer_rs_len + 4 - pos - 1;

		if(data_len < 128)
		{
			print_info_Info(g_com_rx_buf,99,UDPNET);
			memcpy(Data,&g_com_rx_buf[pos],data_len);
		}
		pos += data_len;
	}
	pos++;	//ReturnStatus

	 

 	return SUCCESS;
} 

/*
TransferChannel_Response
功能描述：透明通道操作的响应
输入参数：	fd--设备句柄号
		time_out--超时时间，范围1-1000ms
输出参数：	DID--OBU应用号，ETC应用为1，标识站应用为2
		ChannelID--通道ID号
		APDULIST--对通道操作的APDU命令数
		Data--对通道操作的APDU命令返回数据，按顺序为数据1长度(1字节)、数据1、数据2长度(1字节)、数据2……
		ReturnStatus--OBU处理状态
返回值：	0--命令执行成功
		-1--超时，RSU设备无响应
		其他--回复帧不是transfer帧
*/
int TransferChannel_rs_channelID(uint8 *channelID,int *DATALIST, char *Data, int time_out)
{
	int ret = SUCCESS;
	struct timespec ts;
		struct timeval tt;
	uint16 pos = 0, i = 0, data_len = 0;
	uint8 transfer_rs_len = 0, action_para;
 
    for(i = 0; i < time_out; i++)
	{
    	gettimeofday(&tt,NULL);
    	ts.tv_sec = tt.tv_sec+2;
    	ts.tv_nsec = tt.tv_usec*1000;
//		ret = sem_timedwait(&g_sem_transfer,&ts);hmh
		if(ret == SUCCESS)
		{
			break;
		}
		else
		{
			//if(i == time_out - 1)
			if((0 != checktimeout())|| (i == time_out - 1))
			{
				is_transfer_recved = 1;		//收到或者超时退出都不再收数
				//sem_destroy(&g_sem_transfer);
				return -1;		//超时，没有收到vst
			}
			//usleep(20000);
		}
	}
	is_transfer_recved = 1;	//收到或者超时退出都不再收数
	//sem_destroy(&g_sem_transfer);
	ret = IsTransfer_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//判断是否时TransferChannel_rs
	{
		return -2 + ret*10;
	}
	pos = 3;	//去掉帧序号、命令号0xe0、原语类型0x02
	transfer_rs_len = g_com_rx_buf[pos++];
	//解析transfer_rs
	pos += 4;	//跳过macid
	pos++;		//mac控制域0xe0
	pos++;		//llc控制域
	pos++;		//状态子域
	pos++;		//段子头
	action_para = g_com_rx_buf[pos++];	//action标识0x18
	pos++;		//DID 0x01
	//判断容器是否存在
	if(Getbit(action_para,3))
	{
		pos++;	//容器类型0x19
		//pos++;	//通道号
		*channelID = g_com_rx_buf[pos++];	//通道号
		*DATALIST = g_com_rx_buf[pos++];
		data_len = transfer_rs_len + 4 - pos - 1;

		if(data_len < 128)
		{
			print_info_Info(g_com_rx_buf,99,UDPNET);
			memcpy(Data,&g_com_rx_buf[pos],data_len);
		}
		pos += data_len;
	}
	pos++;	//ReturnStatus
 	return SUCCESS;
}
/* 
TransferChannel_Response
功能描述：透明通道操作的响应
输入参数：	fd--设备句柄号
		time_out--超时时间，范围1-1000ms
输出参数：	DID--OBU应用号，ETC应用为1，标识站应用为2
		ChannelID--通道ID号
		APDULIST--对通道操作的APDU命令数
		Data--对通道操作的APDU命令返回数据，按顺序为数据1长度(1字节)、数据1、数据2长度(1字节)、数据2……
		ReturnStatus--OBU处理状态
返回值：	0--命令执行成功
		-100--超时，RSU设备无响应
		-101--超时，OBU无响应
		-103--超时，通道设备(IC卡、ESAM等)无响应
		-1000--输入参数错误
		-1001--设备未打开
		-2000--其他错误
*/
int TransferChannel_rs_JinYi_0010_GetRand(int *DATALIST, char *Data, int time_out)
{
	int ret = SUCCESS;
	uint16 pos = 0, i = 0, data_len = 0;
	uint8 transfer_rs_len = 0, action_para;
    for(i = 0; i < time_out; i++)
	{
		ret = sem_trywait(&g_sem_transfer);
		if(ret == SUCCESS)
		{
			break;
		}
		else
		{
			if(i == time_out - 1)
			{
				is_transfer_recved = 1;		//收到或者超时退出都不再收数
				sem_destroy(&g_sem_transfer);
				return -1;		//超时
			}
			usleep(20000);
		}
	}
	is_transfer_recved = 1;	//收到或者超时退出都不再收数
	sem_destroy(&g_sem_transfer);
	ret = IsTransfer_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//判断是否时TransferChannel_rs
	{
		return -2 + ret*10;
	}
	pos = 3;	//去掉帧序号、命令号0xe0、原语类型0x02
	transfer_rs_len = g_com_rx_buf[pos++];
	//解析transfer_rs
	pos += 4;	//跳过macid
	pos++;		//mac控制域0xe0
	pos++;		//llc控制域
	pos++;		//状态子域
	pos++;		//段子头
	action_para = g_com_rx_buf[pos++];	//action标识0x18
	pos++;		//DID 0x01
	//判断容器是否存在
	if(Getbit(action_para,3))
	{
		pos++;	//容器类型0x19
		*DATALIST = 1;
		data_len = 4;
		memcpy(Data,&g_com_rx_buf[pos],data_len);
		pos += data_len;
	}
	pos++;	//ReturnStatus
 	return SUCCESS;
}
/*
SetMMI_Request 
功能描述：对OBU进行界面提示 
输入参数：fd--设备句柄号 
		mode--确认模式，1：需应答，0：无需应答 
		DID--OBU应用号，ETC应用为1，标识站应用为2 
		SetMMIPara--人机界面参数 
		time_out--超时时间，范围1-1000ms 
输出参数：	无 
返回值：	0-命令执行成功 
		-100--超时，RSU设备无响应 
		-1000--输入参数错误 
		-1001--设备未打开 
		-2000--其他错误 
*/ 
int SetMMI_rq(int SetMMIPara) 
{ 
	int ret = SUCCESS;
	uint8 chk = 0;
	uint8 sbuf[BUF_LEN] = {0};
	uint16 slen = 0, len_pos = 0, i = 0; 
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl; //帧序号  
	sbuf[slen++] = 0xf0; 	//命令号,bst以及action原语 
	sbuf[slen++] = 0x03;	//Setmmi_rq
	len_pos = slen++; 		//留出总长度位置
	//组包transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;	//mac控制域
//	sbuf[slen++] = 0xf7;	//llc控制域
	sbuf[slen++] = 0x77 | g_u8LLCFlag ;	//llc控制域,0xf7
	if(g_u8LLCFlag == 0x80)
		g_u8LLCFlag = 0x00;
	else
		g_u8LLCFlag = 0x80;
	sbuf[slen++] = 0x91;	//段字头 
	sbuf[slen++] = 0x05;	//action标识0x05 
	sbuf[slen++] = 0x01;	//DID
	sbuf[slen++] = 0x04;	//actiontype
	sbuf[slen++] = 0x1A;	//容器类型
	sbuf[slen++] = SetMMIPara;
 	sbuf[len_pos] = slen - len_pos - 1;
	chk = sbuf[2];
	for(i = 3;i < slen;i++)
	{
		chk = chk  ^ sbuf[i];
	}
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;
	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);
	ret = serial_send();	//调用底层发送函数
	sem_init(&g_sem_setmmi,0,0);
	is_setmmi_recved = 0;	//收到或者超时退出都不再收数
	return ret; 
}

/*
SetMMI_Request
功能描述：对OBU进行界面提示
输入参数：fd--设备句柄号
		mode--确认模式，1：需应答，0：无需应答
		DID--OBU应用号，ETC应用为1，标识站应用为2
		SetMMIPara--人机界面参数
		time_out--超时时间，范围1-1000ms
输出参数：	无
返回值：	0-命令执行成功
		-100--超时，RSU设备无响应
		-1000--输入参数错误
		-1001--设备未打开
		-2000--其他错误
*/
int SetMMI_rq_new(int SetMMIPara , uint8 *IdentInfo)
{
	int ret = SUCCESS;
	uint8 chk = 0;
	uint8 sbuf[BUF_LEN] = {0};
	uint16 slen = 0, len_pos = 0, i = 0;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl; //帧序号
	sbuf[slen++] = 0xf0; 	//命令号,bst以及action原语
	sbuf[slen++] = 0x03;	//Setmmi_rq
	len_pos = slen++; 		//留出总长度位置
	//组包transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;	//mac控制域
//	sbuf[slen++] = 0xf7;	//llc控制域
	sbuf[slen++] = 0x77 | g_u8LLCFlag ;	//llc控制域,0xf7
	if(g_u8LLCFlag == 0x80)
		g_u8LLCFlag = 0x00;
	else
		g_u8LLCFlag = 0x80;
	sbuf[slen++] = 0x91;	//段字头
	sbuf[slen++] = 0x05;	//action标识0x05
	sbuf[slen++] = 0x01;	//DID
	sbuf[slen++] = 0x04;	//actiontype
	sbuf[slen++] = 0x1A;	//容器类型
	sbuf[slen++] = SetMMIPara;
	for(i = 0;i < 20;i++)
	{
		sbuf[slen++] = IdentInfo[i];
	}
	sbuf[len_pos] = slen - len_pos - 1;

	chk = sbuf[2];
	for(i = 3;i < slen;i++)
	{
		chk = chk  ^ sbuf[i];
	}
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;
	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);
	ret = serial_send();	//调用底层发送函数
	sem_init(&g_sem_setmmi,0,0);
	is_setmmi_recved = 0;	//收到或者超时退出都不再收数
	return ret;
}

/*
* 函数名称：	IsSetMMI_rs()
* 功能描述：	目前协议不支持拼包
* 输入参数：	无
* 输出参数：  prog_trade_b5--成功交易结束帧
* 返回值：	0--成功，非0--失败
*/
static uint8 IsSetMMI_rs(uint8 *pkg)
{
	if(!Getbit(pkg[12], 7) && !Getbit(pkg[12], 6) && !Getbit(pkg[12], 5) && Getbit(pkg[12], 4))
	{
		if(Getbit(pkg[12], 3))
		{
			if(pkg[14] == 0x1B)
			{
				return 0;
			}
			else
			{
				return -3;
			}
		}
		else
		{
			return -2;
		}
	}
	else
	{
		return -1;
	}
}
 
/* 
SetMMI_Response 
功能描述：对OBU进行界面提示的响应 
输入参数：	fd--设备句柄号 
		time_out--超时时间，范围1-1000ms 
输出参数：	DID--OBU应用号，ETC应用为1，标识站应用为2 
		ReturnStatus--OBU处理状态 
返回值：	0--命令执行成功 
		-100--超时，RSU设备无响应 
		-101--超时，OBU无响应 
		-1000--输入参数错误 
		-1001--设备未打开 
		-2000--其他错误 
*/ 
int SetMMI_rs(int time_out) 
{
	int ret = SUCCESS;
	struct timespec ts;
	struct timeval tt;
	time_out = 1;
	clock_t tick1,tick2;

	uint16 pos = 0, i = 0;
	uint8 setmmi_rs_len = 0, action_para;
    for(i = 0; i < time_out; i++)
	{
    	gettimeofday(&tt,NULL);
    	ts.tv_sec = tt.tv_sec+2;
    	ts.tv_nsec = tt.tv_usec*1000;
//		ret = sem_timedwait(&g_sem_setmmi,&ts);hmh
		 
		if(ret == SUCCESS)
		{
			break;
		}
		else
		{
			//if(i == time_out - 1)
			if((0 != checktimeout())|| (i == time_out - 1))
			{
				is_setmmi_recved = 1;	//收到或者超时退出都不再收数
				//sem_destroy(&g_sem_setmmi);
				return -1;	//超时，没有收到vst
			}
			//usleep(20000);
		}
	}
	is_setmmi_recved = 1;	//收到或者超时退出都不再收数
	//sem_destroy(&g_sem_setmmi);
	ret = IsSetMMI_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//判断是否是SetMMI_rs
	{
		return -2 + ret*10;
	}
	pos = 3;	//去掉帧序号、命令号0xe0、原语类型0x03
	setmmi_rs_len = g_com_rx_buf[pos++];
	//解析setmmi_rs
	pos += 4;//跳过macid
	pos++;//mac控制域0xe0
	pos++;//llc控制域
	pos++;//状态子域
	pos++;//段子头
	action_para = g_com_rx_buf[pos++];//action标识0x18
	pos++;//DID 0x01
	//容器是否存在
	if(Getbit(action_para,3))
	{
		pos++; //容器类型0x1b
	}
	pos++;	//ReturnStatus
	if(setmmi_rs_len != pos - 4)
	{
		return -3;
	}
	return SUCCESS;
}
 
/* 
EVENT_REPORT_Request 
功能描述：释放OBU 
输入参数：	fd--设备句柄号 
		mode--确认模式，1：需应答，0：无需应答 
		DID--DSRC-DID，目录号 
		EventType--Release=0 
		time_out--超时时间，范围1-1000ms 
输出参数：	无 
返回值：	0--命令执行成功 
		-100--超时，RSU设备无响应 
		-1000--输入参数错误 
		-1001--设备未打开 
		-2000--其他错误 
*/ 
int EVENT_REPORT_rq(uint8 event_type, uint8 ant_id) 
{
	int ret = SUCCESS;    
	uint8 chk = 0;
	uint8 sbuf[BUF_LEN] = {0}; 
	uint16 slen = 0, len_pos = 0, i = 0;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl; //帧序号  
	sbuf[slen++] = 0xf0; 	//命令号,bst以及action原语 
	sbuf[slen++] = 0x04;	//event-report
	len_pos = slen++; 		//留出总长度位置
	//组包transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;	//mac控制域
	sbuf[slen++] = 0x03;	//llc控制域
	sbuf[slen++] = 0x91;	//段字头 
	sbuf[slen++] = 0x60;	//eventreport标识0x60
	sbuf[slen++] = 0x00;	//DID
	sbuf[slen++] = event_type;//eventType
	if(g_bst_type == BST_TRADE_ZYL)
	{
		sbuf[slen++] = ant_id;
	}
  	sbuf[len_pos] = slen - len_pos - 1;
	chk = sbuf[2];
	for(i = 3; i < slen; i++)
	{
		chk = chk ^ sbuf[i];
	}
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;
	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);
	ret = serial_send();	//调用底层发送函数
	memset(g_com_tx_buf, 0x00, sizeof(g_com_tx_buf));
	g_com_tx_len = 0;
	ret = serial_send();	//调用底层发送函数
	return ret; 
} 

int  PrWRQ_rs(uint8 *obu_mac, uint8 *ant_id, int time_out)
{
	int i;
	int ret = 0; 
	int len;
	int pos;

    for (i=0; i<time_out; i++)
	{
		ret = sem_trywait(&g_sem_prwrq);
		if (0 == ret)
			break;
		else
		{
			if(i == time_out - 1)
			{
				is_prwrq_recved = 1; //收到或者超时退出都不再收数
				sem_destroy(&g_sem_prwrq);
				return -1; //超时，没有收到prwrq
			}
			usleep(20000);
		}
	}	
	is_prwrq_recved = 1; //收到或者超时退出都不再收数
	sem_destroy(&g_sem_prwrq);

	pos = 3; //去掉帧序号、命令号0xe0、原语类型0x00

	len = g_com_rx_buf[pos++];
	
	for(i = 0;i < 4;i++)
		obu_mac[i] = g_com_rx_buf[pos++];
	pos++;  //MAC控制域
	*ant_id = g_com_rx_buf[pos++];
	
	return 0; 
}

int  PrWA_rq(uint8 *obu_mac, uint8 ant_id)
{
	int slen,len_pos,i; 
	int ret;   
	uint8 sbuf[512];  
	uint8 chk = 0;
 
	memset(sbuf,0,512); 
 
	slen = 0; 
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl+1)%16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl;
	sbuf[slen++] = 0xf0; // 命令号,bst以及action原语 
	sbuf[slen++] = 0x06;//PrWA
	len_pos = slen++; //留出总长度位置 

	sbuf[slen++] = obu_mac[0];
	sbuf[slen++] = obu_mac[1];
	sbuf[slen++] = obu_mac[2];
	sbuf[slen++] = obu_mac[3];
	sbuf[slen++] = 0x08;
	sbuf[slen++] = ant_id;

	sbuf[len_pos] = slen - len_pos - 1;
	chk = sbuf[2];
	for(i = 3;i < slen;i++)
		chk = chk  ^ sbuf[i];
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;

	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);
 
	// 调用底层发送函数 
	ret = serial_send(); 

	sem_init(&g_sem_vst,0,0);
	is_vst_recved = 0; //初始没有收到
 
	return ret; 
}

int SetSecure_rq(uint8 *accessCredentials, FILE_PIB *file, uint8 *randRsuForAuthen, uint8 randRsuForAuthenIndex, uint8 AntID, uint8 OBUActions)
{	
	int slen,len_pos,i; 
	int ret;    
	int apdulen;
	uint8 chk = 0;
	uint8 sbuf[512]; 
	
	memset(sbuf,0,512); 
 
	slen = 0; 
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl+1)%16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl;
	sbuf[slen++] = 0xf0; // 命令号,bst以及action原语 
	sbuf[slen++] = 0x02;
	len_pos = slen++; //留出总长度位置 

	//组包SetSecure_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;//mac控制域
	sbuf[slen++] = 0x77;//llc控制域
	sbuf[slen++] = 0x91; //段字头 
	sbuf[slen++] = 0x0d; //action标识0x05 
	sbuf[slen++] = 0x03; //DID
	sbuf[slen++] = 0x01; //actiontype

	for (i=0; i<8; i++) 
		sbuf[slen++] = accessCredentials[i]; //访问证书
	
	sbuf[slen++] = 0x16; //SetSecureRq
	sbuf[slen++] = 0x00; //KeyIDforEncryptOp不存在
	sbuf[slen++] = 0x02; //FID

	for (i=0; i<2; i++) 
		sbuf[slen++] = 0x00; //offset

	sbuf[slen++] = 0x14; //file length

	for (i=0; i<4; i++) 
		sbuf[slen++] = file->exchangeTime[i]; 

	for (i=0; i<6; i++) 
		sbuf[slen++] = file->PSAMID[i]; 

	for (i=0; i<4; i++) 
		sbuf[slen++] = file->Tr_SN[i]; 

	for (i=0; i<2; i++) 
		sbuf[slen++] = file->exchangeStation[i]; 

	sbuf[slen++] = file->exchangeRoadway; 

	for (i=0; i<2; i++) 
		sbuf[slen++] = file->balance[i]; 

	sbuf[slen++] = file->vehicleStatus; 

	for (i=0; i<8; i++) 
		sbuf[slen++] = randRsuForAuthen[i]; 
	sbuf[slen++] = randRsuForAuthenIndex;
	sbuf[slen++] = AntID;
	sbuf[slen++] = OBUActions; 

	sbuf[len_pos] = slen - len_pos - 1;
	chk = sbuf[2];
	for(i = 3;i < slen;i++)
		chk = chk  ^ sbuf[i];
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;

	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);
 
	// 调用底层发送函数 
	ret = serial_send(); 

	sem_init(&g_sem_setsecure,0,0);
	is_setsecure_recved = 0; //收到或者超时退出都不再收数

	return ret; 
}

int  QuickAck_rs(int time_out)
{
	int i;
	int ret = 0; 
	int len;
	int pos;

    for (i=0; i<time_out; i++)
	{
		ret = sem_trywait(&g_sem_quickack);
		if (0 == ret)
			break;
		else
		{
			if(i == time_out - 1)
			{
				is_quickack_recved = 1; //收到或者超时退出都不再收数
				sem_destroy(&g_sem_quickack);
				return -1; //超时，没有收到quickack
			}
			usleep(20000);
		}
	}	
	is_quickack_recved = 1; //收到或者超时退出都不再收数
	sem_destroy(&g_sem_quickack);

	pos = 3; //去掉帧序号、命令号0xe0、原语类型0x00

	len = g_com_rx_buf[pos++];
	
	//for(i = 0;i < 4;i++)
		//macid[i] = g_com_rx_buf[pos++];
	//pos++;  //MAC控制域
	
	return 0; 
}

int  GetAns_rq(uint8 *obu_mac, uint8 ant_id)
{
	uint8 sbuf[512]; 
	int slen,len_pos,i; 
	int ret;    
	uint8 chk = 0;
 
	memset(sbuf,0,512); 
 
	slen = 0; 
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl+1)%16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl;
	sbuf[slen++] = 0xf0; // 命令号,bst以及action原语 
	sbuf[slen++] = 0x02;
	len_pos = slen++; //留出总长度位置 

	sbuf[slen++] = obu_mac[0];
	sbuf[slen++] = obu_mac[1];
	sbuf[slen++] = obu_mac[2];
	sbuf[slen++] = obu_mac[3];
	sbuf[slen++] = 0x40;
	sbuf[slen++] = 0xf7;
	sbuf[slen++] = 0x91;
	sbuf[slen++] = 0x01;
	sbuf[slen++] = 0x03;
	sbuf[slen++] = 0x01;
	sbuf[slen++] = ant_id;

	sbuf[len_pos] = slen - len_pos - 1;
	chk = sbuf[2];
	for(i = 3;i < slen;i++)
		chk = chk  ^ sbuf[i];
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;

	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);
 
	// 调用底层发送函数 
	ret = serial_send(); 

	sem_init(&g_sem_setsecure,0,0);
	is_setsecure_recved = 0; //初始没有收到
 
	return ret; 
}

int SetSecure_rs(int time_out)
{
	int i;
	int ret = 0; 
	int len;
	int pos;

_WAIT:
    for (i=0; i<time_out; i++)
	{
		ret = sem_trywait(&g_sem_setsecure);
		if (0 == ret)
			break;
		else
		{
			if(i == time_out - 1)
			{
				is_setsecure_recved = 1; //收到或者超时退出都不再收数
				sem_destroy(&g_sem_setsecure);
				return -1; //超时，没有收到setsecure
			}
			usleep(20000);
		}
	}	
	is_setsecure_recved = 1; //收到或者超时退出都不再收数
	sem_destroy(&g_sem_setsecure);

	pos = 3; //去掉帧序号、命令号0xe0、原语类型0x00

	len = g_com_rx_buf[pos++];

	if(len == 0x0d)	//收到的是快速应答帧
	{
		ret = GetAns_rq(prwrq_pib.OBUMAC, prwrq_pib.AntID); //发送取结果帧
		if(ret != 0)
		{
			return -2 + ret*10;
		}
		goto _WAIT;
	}else if(len == 0x0a)//收到的是交易超时帧
	{
		return -3 ;
	}	
	return 0;
}

/*
写车辆信息文件，金益老版（00,10版）OBU写车辆信息自动激活：
*/
int WriteVehFile_rq_Jinyi(int psam_slot,uint8 offset,uint8 length,uint8 * file)
{
	uint8 sbuf[512]; 
	int slen,len_pos,i; 
	int ret;    
	uint8 chk = 0;
 
	memset(sbuf,0,512); 

	slen = 0; 
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0x13; // 帧序号 
	sbuf[slen++] = 0xf0; // 命令号,bst以及action原语 
	sbuf[slen++] = 0x02;
	len_pos = slen++; //留出总长度位置 

	//组包transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;//mac控制域
	sbuf[slen++] = 0xf7;//llc控制域
	sbuf[slen++] = 0x91; //段字头 
	sbuf[slen++] = 0x81;
	//4个字节，文件目录信息
	sbuf[slen++] = 0x01; //表示目录号：00根目录，01应用目录
	sbuf[slen++] = 0x01; //表示写文件
	sbuf[slen++] = 0x00;
	sbuf[slen++] = offset;

	sbuf[slen++] = length; //系统信息长度32个字节
	for(i = 0;i < length;i++)
		sbuf[slen++]= file[i];

	// 计算ESAM卡系统信息文件MAC 
//    ret = psamCalcEsamMac(psam_slot, offset,length,file,1);//系统信息标志是0,因为所用分散因子不同hmh
	if (ret != 0)
		return -1+ret*10;
	for (i=0; i<4; i++)
		sbuf[slen++] = esam_mac[i];       //MAC

	sbuf[len_pos] = slen - len_pos - 1;

	chk = sbuf[2];
	for(i = 3;i < slen;i++)
		chk = chk  ^ sbuf[i];
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;

	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);


	//测试代码
//	ret = psamGetKey(g_psam_slot,0x08);
//	if (ret != 0)
//	{
//		return -998;
//	}
//	print_info_Info(cur_key,16,UDPNET);
//	if(1 == 1)
//	{
//		print_info_Info(g_com_tx_buf,g_com_tx_len,UDPNET);
//		return -999;
//	}


	// 调用底层发送函数
	ret = serial_send(); 
	sem_init(&g_sem_transfer,0,0);
	is_transfer_recved = 0;	//收到或者超时退出都不再收数
	return ret; 
}

/*
写车辆信息文件，金益老版（00,10版）OBU写车辆信息自动激活：
*/
int WriteVehFile_rq_Jinyi_online(int psam_slot,uint8 offset,uint8 length,uint8 * file)
{
	uint8 sbuf[512];
	int slen,len_pos,i;
	int ret;
	uint8 chk = 0;

	memset(sbuf,0,512);

	slen = 0;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0x13; // 帧序号
	sbuf[slen++] = 0xf0; // 命令号,bst以及action原语
	sbuf[slen++] = 0x02;
	len_pos = slen++; //留出总长度位置

	//组包transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;//mac控制域
	sbuf[slen++] = 0xf7;//llc控制域
	sbuf[slen++] = 0x91; //段字头
	sbuf[slen++] = 0x81;
	//4个字节，文件目录信息
	sbuf[slen++] = 0x01; //表示目录号：00根目录，01应用目录
	sbuf[slen++] = 0x01; //表示写文件
	sbuf[slen++] = 0x00;

//	sbuf[slen++] = 0x04; //表示目录号：00根目录，01应用目录
//	sbuf[slen++] = 0xd6; //表示写文件
//	sbuf[slen++] = 0x81;
	sbuf[slen++] = offset;

	sbuf[slen++] = length;
	for(i = 0;i < length;i++)
		sbuf[slen++]= file[i];

	for(i = 0;i < 4;i++)
			sbuf[slen++]= file[i+59];//mac是最后4个字节，不论写信息的长度是多少

	sbuf[len_pos] = slen - len_pos - 1;

	chk = sbuf[2];
	for(i = 3;i < slen;i++)
		chk = chk  ^ sbuf[i];
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;

	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);


	//测试代码
//	ret = psamGetKey(g_psam_slot,0x08);
//	if (ret != 0)
//	{
//		return -998;
//	}
//	print_info_Info(cur_key,16,UDPNET);
//	if(1 == 1)
//	{
//		print_info_Info(g_com_tx_buf,g_com_tx_len,UDPNET);
//		return -999;
//	}


	// 调用底层发送函数
	ret = serial_send();
	sem_init(&g_sem_transfer,0,0);
	is_transfer_recved = 0;	//收到或者超时退出都不再收数
	return ret;
}
/*
写车辆信息指令返回：
4字节OBU MacID + e0 f7(77) 00 91 + 90 00 01 00
*/
int WriteVehFile_rs_JinYi(int time_out)
{
	uint8 WriteSysFile_rs_len = 0;
	unsigned char rbuf[512];
	int rlen;

	int ret = SUCCESS;
	uint16 pos = 0, i = 0, data_len = 0; 
	uint8 transfer_rs_len = 0, action_para;
    for(i = 0; i < time_out; i++)
	{
		ret = sem_trywait(&g_sem_transfer);
		if(ret == SUCCESS)
		{
			break;
		}
		else
		{
			if(i == time_out - 1)
			{
				is_transfer_recved = 1;		//收到或者超时退出都不再收数
				sem_destroy(&g_sem_transfer);
				return -1;		//超时，没有收到vst
			}
			usleep(20000);
		}
	}
	is_transfer_recved = 1;	//收到或者超时退出都不再收数
	sem_destroy(&g_sem_transfer);
	ret = IsTransfer_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//判断是否时TransferChannel_rs
	{
		return -2 + ret*10;
	}
		
	pos = 3; //去掉帧序号、命令号0xe0、原语类型0x02

	WriteSysFile_rs_len = g_com_rx_buf[pos++];
	//解析transfer_rs
	pos += 4; //跳过macid
	pos++; //mac控制域0xe0
	pos++;//llc控制域
	pos++; //状态子域
	pos++; //段子头
	if(g_com_rx_buf[pos++] != 0x90)
		return -3; //0x90
	pos++; //表示目录号：00根目录，01应用目录
	pos++; //0x01
	if(g_com_rx_buf[pos++] != 0x00)//pos++; //0x00
	{
		return -4; //非0x00
	}
//	if(WriteSysFile_rs_len != pos - 4)
//		return -1;

	return 0;
}

/*
写系统信息文件，金益老版（00,10版：
*/
int WriteSysFile_rq_Jinyi(int psam_slot,uint8 offset,uint8 length,uint8 * file)
{
	uint8 sbuf[512];
	int slen,len_pos,i;
	int ret;
	uint8 chk = 0;

	memset(sbuf,0,512);

	slen = 0;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0x13; // 帧序号
	sbuf[slen++] = 0xf0; // 命令号,bst以及action原语
	sbuf[slen++] = 0x02;
	len_pos = slen++; //留出总长度位置

	//组包transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;//mac控制域
	sbuf[slen++] = 0xf7;//llc控制域
	sbuf[slen++] = 0x91; //段字头
	sbuf[slen++] = 0x81;
	//4个字节，文件目录信息
	sbuf[slen++] = 0x00; //表示目录号：00根目录，01应用目录
	sbuf[slen++] = 0x01; //表示写文件
	sbuf[slen++] = 0x00;
	sbuf[slen++] = offset;

	sbuf[slen++] = length; //系统信息长度32个字节
	for(i = 0;i < length;i++)
		sbuf[slen++]= file[i];

//	金溢老版obu 把从esam中取到的随机数esam_random赋给上传信息帧(写系统信息随机数)  add by wll 2013-7-5 17:07:36
	memcpy(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom,esam_random,sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom));

	// 计算ESAM卡系统信息文件MAC
    ret = psamCalcEsamMac(psam_slot, offset,length,file,0);//系统信息标志是0,因为所用分散因子不同
	if (ret != 0)
		return -1 + ret*10;
	for (i = 0; i < 4; i ++)
		sbuf[slen++] = esam_mac[i];       //MAC
//	金溢老版obu  把写车辆信息指令复制到上传交易信息帧中(写车辆信息指令)  add by wll 2013-7-5 15:16:09
	memcpy(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand,&sbuf[slen - 4 - length - 9],9 + length + 4);
//	补0
	memset(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[9 + length + 4], 0x00, sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand) - (9 + length + 4));

	sbuf[len_pos] = slen - len_pos - 1;

	chk = sbuf[2];
	for(i = 3;i < slen;i++)
		chk = chk ^ sbuf[i];
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;

	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);


	//测试代码
//	ret = psamGetKey(g_psam_slot,0x08);
//	if (ret != 0)
//	{
//		return -998;
//	}
//	print_info_Info(cur_key,16,UDPNET);
//	if(1 == 1)
//	{
//		print_info_Info(g_com_tx_buf,g_com_tx_len,UDPNET);
//		return -999;
//	}


	// 调用底层发送函数
	ret = serial_send();
	sem_init(&g_sem_transfer,0,0);
	is_transfer_recved = 0;	//收到或者超时退出都不再收数
	if(ret != 0)
		return -3 + ret *10;
	return ret;
}


int WriteSysFile_rq_Jinyi_online(int psam_slot,uint8 offset,uint8 length,uint8 * file)
{
	uint8 sbuf[512];
	int slen,len_pos,i;
	int ret;
	uint8 chk = 0;

	memset(sbuf,0,512);

	slen = 0;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0x13; // 帧序号
	sbuf[slen++] = 0xf0; // 命令号,bst以及action原语
	sbuf[slen++] = 0x02;
	len_pos = slen++; //留出总长度位置

	//组包transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;//mac控制域
	sbuf[slen++] = 0xf7;//llc控制域
	sbuf[slen++] = 0x91; //段字头
	sbuf[slen++] = 0x81;
	//4个字节，文件目录信息
	sbuf[slen++] = 0x00; //表示目录号：00根目录，01应用目录
	sbuf[slen++] = 0x01; //表示写文件
	sbuf[slen++] = 0x00;
	sbuf[slen++] = offset;

	sbuf[slen++] = length; //系统信息长度32个字节
	for(i = 0;i < length;i++)
		sbuf[slen++]= file[i];

	for(i = 0;i < 4;i++)
		sbuf[slen++]= file[i+9];//网络协议中mac存放在最后的4个字节
//	金溢老版obu 把从esam中取到的随机数esam_random赋给上传信息帧(写系统信息随机数)  add by wll 2013-7-5 17:07:36
	memcpy(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom,esam_random,sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom));

//	金溢老版obu  把写车辆信息指令复制到上传交易信息帧中(写车辆信息指令)  add by wll 2013-7-5 15:16:09
	memcpy(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand,&sbuf[slen - 4 - length - 9],9 + length + 4);
//	补0
	memset(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[9 + length + 4], 0x00, sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand) - (9 + length + 4));

	sbuf[len_pos] = slen - len_pos - 1;

	chk = sbuf[2];
	for(i = 3;i < slen;i++)
		chk = chk ^ sbuf[i];
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;

	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);


	//测试代码
//	ret = psamGetKey(g_psam_slot,0x08);
//	if (ret != 0)
//	{
//		return -998;
//	}
//	print_info_Info(cur_key,16,UDPNET);
//	if(1 == 1)
//	{
//		print_info_Info(g_com_tx_buf,g_com_tx_len,UDPNET);
//		return -999;
//	}


	// 调用底层发送函数
	ret = serial_send();
	sem_init(&g_sem_transfer,0,0);
	is_transfer_recved = 0;	//收到或者超时退出都不再收数
	if(ret != 0)
		return -3 + ret *10;
	return ret;
}

/*
写系统信息指令返回
*/
int WriteSysFile_rs_JinYi(int time_out)
{
	uint8 WriteSysFile_rs_len = 0;
	unsigned char rbuf[512];
	int rlen;

	int ret = SUCCESS;
	uint16 pos = 0, i = 0, data_len = 0;
	uint8 transfer_rs_len = 0, action_para;
    for(i = 0; i < time_out; i++)
	{
		ret = sem_trywait(&g_sem_transfer);
		if(ret == SUCCESS)
		{
			break;
		}
		else
		{
			if(i == time_out - 1)
			{
				is_transfer_recved = 1;		//收到或者超时退出都不再收数
				sem_destroy(&g_sem_transfer);
				return -1;
			}
			usleep(20000);
		}
	}
	is_transfer_recved = 1;	//收到或者超时退出都不再收数
	sem_destroy(&g_sem_transfer);
	ret = IsTransfer_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//判断是否时TransferChannel_rs
	{
		return -2 + ret*10;
	}

	pos = 3; //去掉帧序号、命令号0xe0、原语类型0x02

	WriteSysFile_rs_len = g_com_rx_buf[pos++];
	//解析transfer_rs
	pos += 4; //跳过macid
	pos++; //mac控制域0xe0
	pos++;//llc控制域
	pos++; //状态子域
	pos++; //段子头
	if(g_com_rx_buf[pos++] != 0x90)
		return -3; //0x90
	pos++; //表示目录号：00根目录，01应用目录
	pos++; //0x01
	if(g_com_rx_buf[pos++] != 0x00)//pos++; //0x00
	{
		return -4; //非0x00
	}
//	if(WriteSysFile_rs_len != pos - 4)
//		return -1;

	return 0;
}
int IsVst(uint8 *pkg)
{
	
	{
		if(Getbit(pkg[11],7) && Getbit(pkg[11],6) && !Getbit(pkg[11],5) && Getbit(pkg[11],4))
		{
			return 0;
		}
		else if(Getbit(pkg[12],7) && Getbit(pkg[12],6) && !Getbit(pkg[12],5) && Getbit(pkg[12],4))
			return 0;
		else
		{
			return -1;
		}
	}
}
