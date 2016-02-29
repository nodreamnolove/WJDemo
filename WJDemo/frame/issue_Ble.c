#include "issue_Ble.h"
#include "lib2hd.h"
#include "common.h"
#include "lib2rsu.h"
#include "esam.h"
#include "psam.h"
#include "icc.h"
#define __android_log_print(...)
//#include "sha1.h"
//#include "hmac_sha1.h"
int g_ndid;
loadCreditGetMac1Ret g_sret;
int g_timeout_cnt = 0;
int g_timeout_max = 20;

inline int checktimeout();
int checktimeout(){
	if(g_timeout_cnt < g_timeout_max){

		g_timeout_cnt++;
		return 0;
	}else{
		g_timeout_cnt = g_timeout_max;

		return -1;
	}
}

int send_c0_Ble(PROG_COMM_C0 prog_c0) {
	int ret = SUCCESS;
	uint32 len = 0;
	uint8 i = 0, chk = 0;
	uint8 buf[512] = { 0 };
	int TerminalTradeNO = 0;
	//给全局变量帧中的终端交易序号赋值change by wll 2013-5-27 14:00:16
//	g_frame_uploadtradeinfo_rq.TerminalTradeNO = TerminalTradeNO ;
	for (i = 3; (i >= 0) && (i <= 3); i--) {
		g_frame_writeveh_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
		g_frame_writesys_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
		g_frame_readveh_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
		g_frame_syscheck_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
		g_frame_querybackInfo_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
		g_frame_uploadtradeinfo_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
		g_frame_logoutvalidate_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
		TerminalTradeNO = TerminalTradeNO >> 8;
	}

	buf[len++] = 0xFF;
	buf[len++] = 0xFF;
	g_rsctl = 0;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	buf[len++] = g_rsctl; //帧序号
	buf[len++] = 0xFF;
	buf[len++] = 0x00;
	buf[len++] = 0x00;
	buf[len++] = 0x04;
	g_rsctl = 0;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	buf[len++] = g_rsctl;	//prog_c0.RSCTL
	buf[len++] = prog_c0.CMDType;
	for (i = 0; i < 4; i++) {
		buf[len++] = prog_c0.UnixTime[i];
	}
	buf[len++] = prog_c0.BSTInterval;
	buf[len++] = prog_c0.TxPower;
	buf[len++] = prog_c0.PHYChannelID;
	buf[len++] = prog_c0.CRCMode;
	buf[len++] = prog_c0.RFPower;
	buf[len++] = prog_c0.PSAMPower;
	run_mode = prog_c0.run_mode;
	for (i = 0; i < 2; i++) {
		buf[len++] = prog_c0.Reserved[i];
	}
	buf[5] = len - 6;
	chk = buf[2];	//第一个算CRC的数据
	for (i = 3; i < len; i++) {
		chk = chk ^ buf[i];
	}

	buf[len++] = chk & 0xFF;
	buf[len++] = 0xFF;
	g_com_tx_len = pkt_code(&buf[0], &g_com_tx_buf[0], len);
	//调用底层发送函数
	memset(&g_com_rx_buf, 0, sizeof(g_com_rx_buf));
	//sem_init(&g_sem_rsuinit, 0, 0);

	for (i = 0; i < len; i++) {
//		__android_log_print(ANDROID_LOG_INFO, "g_sem_rsuinit buf", "%x",
//				buf[i]);
	}
	is_rsuinit_recved = 0;	//收到或者超时退出都不再收数

	int re_sendB0_count = 2;
	while (re_sendB0_count > 0) {
//		__android_log_print(ANDROID_LOG_INFO, "g_sem_rsuinit", "%x",
//				&g_sem_rsuinit);
		sem_init(&g_sem_rsuinit, 0, 0);
		ret = serial_send();
//		ret = serial_recv_sem(&g_sem_rsuinit, g_timeout_max);
		if (ret != SUCCESS) {
//			__android_log_print(ANDROID_LOG_INFO, "send", "failed");
			memset(&g_com_rx_buf, 0, sizeof(g_com_rx_buf));
			//sem_init(&g_sem_rsuinit, 0, 0);
			//ret = serial_send();
			re_sendB0_count--;
		} else {
			break;
		}
	}

	if (ret != SUCCESS) {
		//DEBUGP("send_c0 ERR, ret = %d\n", ret);
		return -1 + ret * 100;
	} else {
		return SUCCESS;
	}
}

/*
 * 函数名称：	recv_b0_OnLine()
 * 功能描述：	返回射频初始化应答，PSAM卡复位及进目录
 * 输入参数：	psam_slot--PSAM卡使用的卡槽号（二次发行PSAM卡默认使用卡槽1，交易PSAM卡和解密PSAM卡使用卡槽2，发行母卡使用卡槽3）
 *				dir--PSAM卡进入的目录
 *				time_out--超时时间计数，以20ms为单位
 * 输出参数：  prog_b0--设备状态信息帧
 * 返回值：	0--成功，非0--失败
 */
int recv_b0_Ble(PROG_COMM_B0 *prog_b0, int time_out) {
//	__android_log_print(ANDROID_LOG_INFO, "JNI_OBUISSUE", "333");

	uint8 i = 0, pos = 0;
	int ret = SUCCESS;
	char psam_num[10] = { 0 };	//PSAM卡终端机编号
//	__android_log_print(ANDROID_LOG_INFO, "JNI_OBUISSUE", "444");

//	for(i = 0; i < time_out; i++)
//	{
//		ret = sem_trywait(&g_sem_rsuinit);
//		if(ret == SUCCESS)
//		{
//			break;
//		}
//		else
//		{
//			if(i == time_out - 1)
//			{
//				is_rsuinit_recved = 1;	//收到或者超时退出都不再收数
//				sem_destroy(&g_sem_rsuinit);
//				//DEBUGP("recv_b0 ERR, TIME_OUT\n");
//				return -1;	//超时，rsu无反应
//			}
//			usleep(20000);
//		}
//	}/* end for(i = 0; i < time_out; i++) */
//	is_rsuinit_recved = 1;	//收到或者超时退出都不再收数
//	sem_destroy(&g_sem_rsuinit);

//	__android_log_print(ANDROID_LOG_INFO, "JNI_OBUISSUE", "666");

	prog_b0->RSCTL = g_com_rx_buf[pos++];
//	for (i = 0; i < g_com_rx_len; i++) {
//		__android_log_print(ANDROID_LOG_INFO, "c0 g_com_rx_buf", "%x",
//				g_com_rx_buf[i]);
//	}
//	prog_b0->FrameType = g_com_rx_buf[pos++];
//	prog_b0->RSUStatus = g_com_rx_buf[pos++];
// 	for(i = 0; i < 2; i++)
//	{
//		prog_b0->RSUManuID[i] = g_com_rx_buf[pos++];
//	}
//	for(i = 0; i < 2; i++)
//	{
//		prog_b0->RSUID[i] = g_com_rx_buf[pos++];
//	}
//	for(i = 0; i < 2; i++)
//	{
//		prog_b0->RSUVersion[i] = g_com_rx_buf[pos++];
//	}
//	for(i = 0; i < 5; i++)
//	{
//		prog_b0->Reserved[i] = g_com_rx_buf[pos++];
//	}
	return SUCCESS;
}

/*
 * 函数名称：	send_c1_OnLine()
 * 功能描述：	发送BST
 * 输入参数：	prog_c1--OBU发行操作开始指令
 * 输出参数：  无
 * 返回值：	0--成功，非0--失败
 */
int send_c1_Ble(PROG_COMM_C1 prog_c1) {
	int ret = SUCCESS, profile;
	int i;
	char beacon_id[4] = { 0 };
	char unix_time[4] = { 0 };
	static uint32 count = 1;
	time_t ltime;
//	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c1_Ble");
	//BST参数生成
	g_bst_type = BST_TRADE_GB;	//BST类型为带预读
//	g_bst_type = BST_ISSUE_BJ;	//BST类型为北京发行
			//memcpy(beacon_id, "\x08\xff\xff\x00", 4);
	beacon_id[0] = 0x08;	//厂商ID，0x08表示万集科技
	beacon_id[1] = (count >> 16) & 0xff;
	beacon_id[2] = (count >> 8) & 0xff;
	beacon_id[3] = count & 0xff;
	count++;
	time(&ltime);
	unix_time[0] = (ltime >> 24) & 0xff;
	unix_time[1] = (ltime >> 16) & 0xff;
	unix_time[2] = (ltime >> 8) & 0xff;
	unix_time[3] = ltime & 0xff;
	profile = prog_c1.Reserved[0];	//信道号
	g_ObuInitMode = prog_c1.ObuInitMode;

	g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;
//	g_frame_writesys_rq.SecretSelctFlag = 0xA5;

	ret = INITIALISATION_rq(g_bst_type, beacon_id, unix_time, profile,
			prog_c1.ObuInitMode);

	//记录BST
	g_com_tx_len_bst = g_com_tx_len;
	for (i = 0; i < g_com_tx_len_bst; i++) {
		g_com_tx_buf_bst[i] = g_com_tx_buf[i];
		//__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c1_Ble g_com_tx_buf_bst = ",g_com_tx_buf_bst[i]);
	}
	if (ret != SUCCESS) {
		//DEBUGP("INITIALISATION_rq ERR, ret = %d\n", ret);
//		__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c1_Ble failed");
		return -1 + ret * 100;
	} else {
//		__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c1_Ble success");
		return SUCCESS;
	}
}

/*
 * 函数名称：	recv_b1_OnLine()
 * 功能描述：	接收VST
 * 输入参数：	time_out--超时时间计数，以20ms为单位
 * 输出参数：  prog_b1--VST响应信息帧
 * 返回值：	0--成功，非0--失败
 */
int recv_b1_Ble(PROG_COMM_B1 *prog_b1, int time_out) {
	uint8 i = 0;
//	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "recv_b1_Ble");
	ST_TRANSFER_CHANNEL transfer_rq;
	int did, ret = SUCCESS, datalist;
	uint8 data[128] = { 0 };
//	for (i = 0; i < g_com_rx_len; i++) {
//		__android_log_print(ANDROID_LOG_INFO, "c1 g_com_rx_buf", "%x",
//				g_com_rx_buf[i]);
//	}
	memset(prog_b1, 0x00, sizeof(PROG_COMM_B1));	//将结构体清空
	prog_b1->ErrorCode = 0x00;
	memset(&vst, 0xFF, sizeof(ST_VST));	//清空VST add by wll 2013-4-11 17:37:40

	ret = INITIALISATION_rs(vst.macid, vst.sysinfo, vst.iccinfo, vst.rndinfo,vst.resetinfo, &vst.obustatus[0], time_out);
	if (ret != SUCCESS) {
		prog_b1->ErrorCode = 0x01;
	}

	prog_b1->RSCTL = 0x13;
	prog_b1->FrameType = 0xb1;
	memcpy(prog_b1->ContractProvider, vst.sysinfo, 26);
	g_OBU_ContractVersion = prog_b1->ContractVersion;	//记录OBU合同版本，区分1、2版秘钥
	//提取OBU合同版本  add by wll 2013-4-11 14:44:14
	g_frame_readveh_rq.OBUContractVersion = vst.sysinfo[9];
	g_frame_writeveh_rq.OBUContractVersion = vst.sysinfo[9];
	g_frame_querybackInfo_rq.OBUContractVersion = vst.sysinfo[9];
	g_frame_uploadtradeinfo_rq.OBUContractVersion = vst.sysinfo[9];
	//提取OBU合同序列号
	for (i = 0; i < 8; i++) {
		g_frame_readveh_rq.OBUContractSerialNO[i] = vst.sysinfo[10 + i];
		g_frame_writeveh_rq.OBUContractSerialNO[i] = vst.sysinfo[10 + i];
		g_frame_writesys_rq.CopherDecentGene[i] = vst.sysinfo[10 + i];
		g_frame_syscheck_rq.OBUContractSerialNO[i] = vst.sysinfo[10 + i];
		g_frame_querybackInfo_rq.OBUContractSerialNO[i] = vst.sysinfo[10 + i];
		g_frame_uploadtradeinfo_rq.OBUContractSerialNO[i] = vst.sysinfo[10 + i];
	}
	//提取国标cpu卡信息
	if (g_ObuInitMode == 0x2f) {
		for (i = 0; i < 8; i++) {
			g_frame_writeveh_rq.GBCPUCardID[i] = vst.iccinfo[6 + i];
			g_frame_querybackInfo_rq.GBCPUCardID[i] = vst.iccinfo[6 + i];
			g_frame_uploadtradeinfo_rq.GBCPUCardID[i] = vst.iccinfo[6 + i];
		}
	}
	//更新系统信息所用的分散因子
	for (i = 0; i < 4; i++) {
		//主控密钥的分散因子
		EsamDv[i] = vst.resetinfo[i + 1];
		EsamDv[i + 4] = vst.resetinfo[i + 11];
	}
	if ((g_frame_uploadtradeinfo_rq.OBUContractVersion == 0x11)
			|| (g_frame_uploadtradeinfo_rq.OBUContractVersion == 0x12)) {
		for (i = 0; i < 8; i++)	//秘钥分散因子		-"OBU合同版本"为"0x11"和"0x12"时，该字段填写OBU复位信息，即2字节芯片厂商标识 + 2字节ITSC分配的ID号 + 4字节ESAM芯片序列号。
				{
			g_frame_uploadtradeinfo_rq.CopherDecentGene[i] = EsamDv[i];
			g_frame_writesys_rq.CopherDecentGene[i] = EsamDv[i];
		}
	} else if ((g_frame_uploadtradeinfo_rq.OBUContractVersion == 0x00)
			|| (g_frame_uploadtradeinfo_rq.OBUContractVersion == 0x10)) {
		for (i = 0; i < 4; i++)	//秘钥分散因子		-"OBU合同版本"为"0x00"和"0x10"时，该字段填写OBU的MAC地址及地址取反后的8字节信息。
				{
			g_frame_uploadtradeinfo_rq.CopherDecentGene[i] = vst.macid[i];
			g_frame_uploadtradeinfo_rq.CopherDecentGene[i + 4] =
					~(vst.macid[i]);
			//赋值条件还差一个秘钥选择标识///////////////////////////////////////////////-------------------------------------------waiting by wll 2013-7-9 10:30:26
			g_frame_writesys_rq.CopherDecentGene[i] = vst.macid[i];
			g_frame_writesys_rq.CopherDecentGene[i + 4] = ~(vst.macid[i]);
		}
	} else {
		for (i = 0; i < 8; i++)	//秘钥分散因子		-其他版本为合同序列号。
				{
			g_frame_uploadtradeinfo_rq.CopherDecentGene[i] =
					vst.sysinfo[10 + i];
		}
	}
	if (ret != SUCCESS) {
		return -1 + ret * 100;
	}
	for (i = 0; i < 4; i++) {
		prog_b1->Reserved[i] = vst.macid[i];
	}
	prog_b1->Reserved[4] = vst.obustatus[0];
	prog_b1->Reserved[5] = vst.obustatus[2];

	if (vst.iccinfo[0] == 0x00)	//初始化但是没赋值，所以是没有预读ic卡信息的情况
			{
		//提取国标cpu卡信息
		for (i = 0; i < 8; i++) {
			g_frame_writeveh_rq.GBCPUCardID[i] = 0xFF;
			g_frame_querybackInfo_rq.GBCPUCardID[i] = 0xFF;
		}
		for (i = 0; i < 100; i++) {
			prog_b1->FileContent_ICCInfo[i] = 0;
		}
	} else {
		for (i = 0; i < vst.iccinfo[0] + 1; i++) {
			prog_b1->FileContent_ICCInfo[i] = vst.iccinfo[i];
		}
	}

//	if(g_ObuInitMode != 0x2F)
//	{
//		//添加读OBU中IC卡信息的操作  根据  速通联机协议要求  2013-5-30 16:29:28
//		if((vst.obustatus[0] & 0x80) == 0x00) //如果OBU插卡,则读取0002和0015文件
//		{
//			did = 0x01;	//应用主目录
//			iccInitFrame(&transfer_rq);
//			iccReadMoneyFrame(&transfer_rq);
//			iccReadFileFrame(&transfer_rq, 0x0015, 0x00, 0x2b);
//			ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist, transfer_rq.apdu);
//			if(ret != SUCCESS)
//			{
//				DEBUGP("recv_b1_OnLine TransferChannel_rq ERR, ret = %d\n", ret);
//				return -2 + ret*100;	//串口发送失败
//			}
//			ret = TransferChannel_rs(&datalist, data, time_out);
//			if(ret != SUCCESS)
//			{
//				DEBUGP("recv_b1_OnLine TransferChannel_rs ERR, ret = %d\n", ret);
//				return -3 + ret*100;	//解析失败或者超时
//			}
//
//			ret = iccCheck(data, 0);
//			if(ret != SUCCESS)
//			{
//				DEBUGP("recv_b1_OnLine iccCheck ERR, ret = %d\n", ret);
//				return -4 + ret*100;
//			}
//			ret = iccCheck(data, 1);
//			if(ret != SUCCESS)
//			{
//				DEBUGP("recv_b1_OnLine iccCheck ERR, ret = %d\n", ret);
//				return -5 + ret*100;
//			}
//			memcpy(icc_pib.Balance, &data[1], 4);	//0002文件
//			memcpy(icc_pib.icc0015, &data[8], 43);	//0015文件
//
//			//提取国标cpu卡信息
//			for (i = 0; i < 8; i ++)
//			{
//				g_frame_writeveh_rq.GBCPUCardID[i] = icc_pib.icc0015[12+i];
//				g_frame_querybackInfo_rq.GBCPUCardID[i] = icc_pib.icc0015[12+i];
//			}
//			for (i = 0; i < 43 ;i ++)
//			{
//				prog_b1->FileContent_ICCInfo[i] = icc_pib.icc0015[i];
//			}
//			for(;i < 43+4;i++)
//			{
//				prog_b1->FileContent_ICCInfo[i] = icc_pib.Balance[i-43];
//			}
//		}
//		else
//		{
//			for (i = 0; i < 100 ;i ++)
//			{
//				prog_b1->FileContent_ICCInfo[i] = 0;
//			}
//		}
//	}
	return SUCCESS;
}

/*
 * 函数名称：	send_c2_OnLine()
 * 功能描述：	密钥替换
 * 输入参数：	prog_c2--密钥替换指令帧
 *			time_out--超时时间计数，以20ms为单位
 * 输出参数：  无
 * 返回值：	0--成功，非0--失败
 */
int send_c2_Ble(PROG_COMM_C2 prog_c2, int time_out) {
	uint8 i = 0;
	int did, ret = SUCCESS, datalist;
	int replacekey_flag; //主控密钥是1,其他密钥均为0
	int psam_slot = g_master_card;
	uint8 data[128] = { 0 };
	ST_TRANSFER_CHANNEL transfer_rq;
	g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;

	//替换之前需要获得芯片序列号,并且准备替换主维护密钥所用的随机数
	did = 0x00;	//根目录
	esamInitFrame(&transfer_rq);
	esamEnterDirFrame(&transfer_rq, 0x3f00);
	esamGetSn(&transfer_rq);
	esamGetRandFrame(&transfer_rq);
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	//串口发送失败
	if (ret != SUCCESS) {
		//DEBUGP("send_c2 TransferChannel_rq ERR, ret = %d\n", ret);
		return -1 + ret * 100;
	}
	ret = TransferChannel_rs(&datalist, data, time_out);
	//解析失败或者超时
	if (ret != SUCCESS) {
		//DEBUGP("send_c2 TransferChannel_rs ERR, ret = %d\n", ret);
		return -2 + ret * 100;
	}
	ret = esamCheckGetSn(data, 0);
	if (ret != SUCCESS) {
		//DEBUGP("send_c2 esamCheckGetSn ERR, ret = %d\n", ret);
		return -3 + ret * 100;
	}
	ret = esamCheckGetRand(data, 1);
	if (ret != SUCCESS) {
		//DEBUGP("send_c2 esamCheckGetRand ERR, ret = %d\n", ret);
		return -4 + ret * 100;	//取随机数失败直接退出
	}
	/* ********************替换主维护密钥*********************** */
	ret = psamGetDAMK_MF(psam_slot);
	if (ret != SUCCESS) {
		//DEBUGP("send_c2 psamGetDAMK_MF ERR, ret = %d\n", ret);
		return -5 + ret * 100;
	}
	did = 0x00;	//根目录
	esamInitFrame(&transfer_rq);
	esamReplaceKey(&transfer_rq, 0);
	esamGetRandFrame(&transfer_rq);	//准备接下来写系统信息所需的随机数
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != SUCCESS) {
		return -6 + ret * 100;	//串口发送失败
	}
	ret = TransferChannel_rs(&datalist, data, time_out);
	//解析失败或者超时
	if (ret != SUCCESS) {
		return -7 + ret * 100;
	}
	ret = esamCheck(data, 0);
	if (ret != SUCCESS) {
		return -8 + ret * 100;
	}
	ret = esamCheckGetRand(data, 1);
	if (ret != SUCCESS) {
		return -9 + ret * 100;	//取随机数失败直接退出
	}
	/* ****************注意：替换完主维护密钥之后立即更新系统信息中的合同序列号***************** */
	esamInitFrame(&transfer_rq);
	//ret = esamWriteSysInfo(&transfer_rq,prog_c2.sys_offset,prog_c2.sys_length,prog_c2.sys_file);
	ret = esamWriteSysInfo(&transfer_rq, 10, 8, prog_c2.Contractsn);
	if (ret != SUCCESS) {
		return -10 + ret * 100;	//psam计算mac错误
	}
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != SUCCESS) {
		return -11 + ret * 100;	//串口发送失败
	}
	ret = TransferChannel_rs(&datalist, data, time_out);
	if (ret != SUCCESS) {
		return -12 + ret * 100;	//解析失败或者超时
	}
	ret = esamCheck(data, 0);
	if (ret != SUCCESS) {
		return -13 + ret * 100;
	}
	//进入df01目录,并重新获取随机数
	did = 0x01;	//应用主目录
	esamInitFrame(&transfer_rq);
	esamEnterDirFrame(&transfer_rq, 0xdf01);
	esamGetRandFrame(&transfer_rq);
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != SUCCESS) {
		return -14 + ret * 100;	//串口发送失败
	}
	ret = TransferChannel_rs(&datalist, data, time_out);
	if (ret != SUCCESS) {
		return -15 + ret * 100;	//解析失败或者超时
	}
	ret = esamCheck(data, 0);
	if (ret != SUCCESS) {
		return -16 + ret * 100;
	}
	ret = esamCheckGetRand(data, 1);
	if (ret != SUCCESS) {
		return -17 + ret * 100;	//取随机数失败直接退出
	}
	/*************替换应用维护、应用认证、应用加密1、应用加密2、应用加密3共5个密钥************** */
	did = 0x01;	//应用主目录
	for (i = 0; i < 5; i++) {
		switch (i) {
		case 0:	//应用维护密钥
		{
			ret = psamGetDAMK_DF01(psam_slot);
		}
			break;
		case 1:	//应用认证密钥
		{
			ret = psamGetRK1_DF01(psam_slot);
		}
			break;
		case 2:	//应用加密密钥1
		{
			ret = psamGetRK2_DF01_V1(psam_slot);
		}
			break;
		case 3:	//应用加密密钥2
		{
			ret = psamGetRK2_DF01_V2(psam_slot);
		}
			break;
		case 4:	//应用加密密钥3
		{
			ret = psamGetRK2_DF01_V3(psam_slot);
		}
			break;
		default: {
			ret = -1;
		}
			break;
		}/* end switch(i) */
		if (ret != SUCCESS) {
			return -18 - i * 10 + ret * 100;
			return ret;
		}
		esamInitFrame(&transfer_rq);
		esamReplaceKey(&transfer_rq, 0);
		if (i < 4) {
			esamGetRandFrame(&transfer_rq);
		}
		ret = TransferChannel_rq(did, transfer_rq.channelid,
				transfer_rq.apdulist, transfer_rq.apdu);
		if (ret != SUCCESS) {
			return -19 - i * 10 + ret * 100;
			return ret;	//串口发送失败
		}
		ret = TransferChannel_rs(&datalist, data, time_out);
		if (ret != SUCCESS) {
			return -20 - i * 10 + ret * 100;
			return ret;	//解析失败或者超时
		}
		ret = esamCheck(data, 0);
		if (ret != SUCCESS) {
			return -21 - i * 10 + ret * 100;
			return ret;
		}
		if (i < 4) {
			ret = esamCheckGetRand(data, 1);
			if (ret != SUCCESS) {
				return -22 - i * 10 + ret * 100;
				return ret;	//取随机数失败直接退出
			}
		}
	}/* end for(i = 0; i < 5; i++) */
	return SUCCESS;
}

/*
 * 函数名称：	send_c3_OnLine()
 * 功能描述：	OBU文件更新(写系统信息，包括起始截止日期及防拆位)
 *				流程: TRANSFER_CHANNEL初始化，指令数加1，把从前置机获得的写系统信息指令赋给帧中的apdu元素，向OBU发送TRANSFER_CH ANNEL ，如果密钥匹配，可通过OBU认证，OBU更新文件内容。
 * 输入参数：	prog_c3--文件更新指令帧
 *				注：该帧中只NumOfFiles和DIDnFID[i]有效
 *				time_out--超时时间计数，以20ms为单位
 * 输出参数：	无
 * 返回值：	0--成功，非0--失败
 */
int send_c3_Ble(PROG_COMM_C3 prog_c3, int time_out) {
//	int i = 0, j = 0, ret = 0, did = 0, datalist = 0;
//	uint8 data[128] = {0};
//	uint8 FunctionDoFlag = 0;//判断for循环中有没有实际执行过
//	ST_TRANSFER_CHANNEL transfer_rq;
//	g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;
//
//	//开始更新文件
//	for(i = 0; i < prog_c3.NumOfFiles; i++)
//	{
//		if(prog_c3.DIDnFID[i] == 0x01)  //更新系统信息文件
//		{
//			FunctionDoFlag = 1;
//			if ((vst.macid[0] == 0x03)&&((vst.sysinfo[9] == 0x00)||(vst.sysinfo[9] == 0x10)))//聚利0x00,0x10版本OBU
//			{
//////				did = 0x01;		//根目录
//////				esamInitFrame(&transfer_rq);
//////				transfer_rq.apdulist++;
//////				transfer_rq.frame_len = 1;
//////				transfer_rq.apdu[0] = 1;
//////				transfer_rq.apdu[1] = 0x83;
//////				transfer_rq.channelid = 0x00;
//////				ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist, transfer_rq.apdu);
//////				if(ret != SUCCESS)
//////				{
//////					//DEBUGP("send_c3 TransferChannel_rq ERR, ret = %d\n", ret);
//////					sprintf(InfoTmp,"send_c3 TransferChannel_rq ERR, ret = %d\n", ret);
//////					print_info_Info(InfoTmp,strlen(InfoTmp),COM);
//////					return -1 + ret*100;	//串口发送失败
//////				}
//////				ret = TransferChannel_rs(&datalist, data, time_out);
//////				if(ret != SUCCESS)
//////				{
//////					//DEBUGP("send_c3 TransferChannel_rs ERR, ret = %d\n", ret);
//////					sprintf(InfoTmp,"send_c3 TransferChannel_rs ERR, ret = %d\n", ret);
//////					print_info_Info(InfoTmp,strlen(InfoTmp),COM);
//////					return -2 + ret*100;	//解析失败或者超时
//////				}
//////				ret = esamCheck_JuLi_Acv(data, 0);
//////				if(ret != SUCCESS)
//////				{
//////					//DEBUGP("send_c3 esamCheck ERR, ret = %d\n", ret);
//////					sprintf(InfoTmp,"send_c3 esamCheck_JuLi_Acv ERR, ret = %d\n", ret);
//////					print_info_Info(InfoTmp,strlen(InfoTmp),COM);
//////					return -3 + ret*100;
//////				}
//			}
//			else
//			{
//				did = 0x01;		//根目录
//				esamInitFrame(&transfer_rq);
//				//命令数量+1
//				transfer_rq.apdulist++;
//				//设置内容
//				transfer_rq.apdu[0]  = 18;
////				if ((vst.macid[0] == 0x03)&&((vst.sysinfo[9] == 0x00)||(vst.sysinfo[9] == 0x10)))
////				{
////					g_frame_writesys_rs.WriteSysInfoCommand[4] = 0x08;
////					for (j = 0; j < 17; j ++)//只写8个字节，mac存放在最后的4个字节中，需跳过中间的一个0
////					{
////						if(j < 13)
////						{
////							transfer_rq.apdu[j+1] = g_frame_writesys_rs.WriteSysInfoCommand[j];
////						}
////						else
////						{
////							transfer_rq.apdu[j+1] = g_frame_writesys_rs.WriteSysInfoCommand[j+1];
////						}
////					}
////				}
////				else
//				{
//					for (j = 0; j < 18; j ++)
//					{
//						transfer_rq.apdu[j+1] = g_frame_writesys_rs.WriteSysInfoCommand[j];
//					}
//				}
//
//				if ((vst.macid[0] == 0x02)&&((vst.sysinfo[9] == 0x00)||(vst.sysinfo[9] == 0x10)))//金益0x00,0x10版本OBU
//				{
//					//添加单独接口写车辆信息，与脱机保持一致。
//					uint8 file[13] = {0};
//					int k = 0;
//					for(k = 0;k < 13;k++)
//					{
//						file[k] = g_frame_writesys_rs.WriteSysInfoCommand[5+k];
//					}
////					ret = WriteSysFile_rq_Jinyi_online(g_psam_slot,g_frame_writesys_rs.WriteSysInfoCommand[3],g_frame_writesys_rs.WriteSysInfoCommand[4],file);
//					ret = WriteSysFile_rq_Jinyi_online(g_psam_slot,0x12,0x08,file);
//					if(ret != SUCCESS)
//					{
//						return -11 + ret*100;	//串口发送失败
//					}
//					ret = WriteSysFile_rs_JinYi(time_out);
//					if(ret != SUCCESS)
//					{
//						return -12 + ret*100;	//解析失败或者超时
//					}
//				}
////				else if ((vst.macid[0] == 0x03)&&((vst.sysinfo[9] == 0x00)||(vst.sysinfo[9] == 0x10)))//juli0x00,0x10版本OBU
////				{
////
////				}
//				else
//				{
//					ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist, transfer_rq.apdu);
//					if(ret != SUCCESS)
//					{
//						//DEBUGP("send_c3 TransferChannel_rq ERR, ret = %d\n", ret);
//						return -1 + ret*100;	//串口发送失败
//					}
//					ret = TransferChannel_rs(&datalist, data, time_out);
//					if(ret != SUCCESS)
//					{
//						//DEBUGP("send_c3 TransferChannel_rs ERR, ret = %d\n", ret);
//						return -2 + ret*100;	//解析失败或者超时
//					}
//					ret = esamCheck(data, 0);
//					if(ret != SUCCESS)
//					{
//						//DEBUGP("send_c3 esamCheck ERR, ret = %d\n", ret);
//						return -3 + ret*100;
//					}
//				}
//			}
//
//		}/* end if(prog_c3.DIDnFID[i] == 0x01) */
//	}/* end for(i = 0; i < prog_c3.NumOfFiles; i++) */
//	if(FunctionDoFlag == 0)
//		return -4;
//	return SUCCESS;
    return 1;
}

/*
 * 函数名称：	recv_b2()
 * 功能描述：	无实质功能
 * 输入参数：	无
 * 输出参数：  prog_b2--更新命令执行响应帧
 * 返回值：	0--成功，非0--失败
 */
int recv_b2_Ble(PROG_COMM_B2 *prog_b2) {
	prog_b2->RSCTL = 0x13;
	prog_b2->FrameType = 0xb2;
	prog_b2->ErrorCode = 0x00;
	return SUCCESS;
}

/*
 * 函数名称：	send_c5_OnLine()
 * 功能描述：	发送SetMMI帧
 * 输入参数：	prog_c5--结束交易指令帧
 * 输出参数：  无
 * 返回值：	0--成功，非0--失败
 */
int send_c5_Ble(PROG_COMM_C5 prog_c5) {
	int ret;
	g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;
	ret = SetMMI_rq(prog_c5.SetMMIMode);
	if (ret != SUCCESS) {
		//DEBUGP("send_c5 SetMMI_rq ERR, ret = %d\n", ret);
		return -1 + ret * 100;
	} else {
		return SUCCESS;
	}
}

/*
 * 函数名称：	recv_b4_OnLine()
 * 功能描述：	结束交易
 * 输入参数：	time_out--超时时间计数，以20ms为单位
 * 输出参数：  prog_b4--交易结束响应帧
 * 返回值：	0--成功，非0--失败
 */
int recv_b4_Ble(PROG_COMM_B4 *prog_b4, int time_out) {
	int ret = SUCCESS;
	//发送SetMMI_rq以后，由于实质性的工作已经完成，所以不再对SetMMI_rs做判断。
	ret = SetMMI_rs(g_timeout_max);
	ret = EVENT_REPORT_rq(0, 0);

	prog_b4->RSCTL = 0x13;
	prog_b4->FrameType = 0xb4;
	prog_b4->ErrorCode = 0;
	return SUCCESS;
}

/*
 * 函数名称：	send_c5_OnLine()
 * 功能描述：	发送SetMMI帧
 * 输入参数：	prog_c5--结束交易指令帧
 * 输出参数：  无
 * 返回值：	0--成功，非0--失败
 */
int send_c5_Ble_new(PROG_COMM_C5 prog_c5, uint8 *IdentInfo) {
	int ret;
	g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;
	ret = SetMMI_rq_new(prog_c5.SetMMIMode, IdentInfo);
	if (ret != SUCCESS) {
		//DEBUGP("send_c5 SetMMI_rq ERR, ret = %d\n", ret);
		return -1 + ret * 100;
	} else {
		return SUCCESS;
	}
}

/*
 * 函数名称：	recv_b4_OnLine()
 * 功能描述：	结束交易
 * 输入参数：	time_out--超时时间计数，以20ms为单位
 * 输出参数：  prog_b4--交易结束响应帧
 * 返回值：	0--成功，非0--失败
 */
int recv_b4_Ble_new(PROG_COMM_B4 *prog_b4, int time_out) {
	int ret = SUCCESS;
	//发送SetMMI_rq以后，由于实质性的工作已经完成，所以不再对SetMMI_rs做判断。
	ret = SetMMI_rs(g_timeout_max);
//	ret = EVENT_REPORT_rq(0, 0);

	prog_b4->RSCTL = 0x13;
	prog_b4->FrameType = 0xb4;
	prog_b4->ErrorCode = 0;
	return SUCCESS;
}

//获得ESAM芯片号
int send_c6_Ble(int TimeOut) {
	ST_TRANSFER_CHANNEL transfer_rq;
	int datalist;
	uint8 data[128];
	int did;
	int ret;
	g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;

	did = 0x01;	 //根目录
	esamInitFrame(&transfer_rq);
	esamGetSn(&transfer_rq);
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	//串口发送失败
	if (ret != 0) {
		return -1 + ret * 100;
	}
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	//解析失败或者超时
	if (ret != 0) {
		return -2 + ret * 100;
	}
	ret = esamCheckGetSn(data, 0);
	if (ret != 0) {
		return -3 + ret * 100;
	}
	return 0;
}

int recv_b6_Ble(uint8 *EsamSerialNum) {
	memcpy(EsamSerialNum, EsamSn, 4);
	return 0;
}

/*
 * 函数名称：	send_c7_OnLine()
 * 功能描述：	OBU文件更新(写系统信息)
 * 操作流程: 	在OBU的DF01目录取随机数->与前置机交互，获取随机数及写车辆信息指令->
 初始化TRANSFER_CHANNEL帧,并给指令号和指令内容赋值，发送TRANSFER_CHANNEL帧->
 如果密钥匹配，可通过OBU认证，OBU更新车信息文件内容。
 * 输入参数：	prog_c3--文件更新指令帧
 *				注：prog_c3帧中只NumOfFiles和DIDnFID[i]有效
 *				time_out--超时时间计数，以20ms为单位
 * 输出参数：	无
 * 返回值：	0--成功，非0--失败
 */

int send_c7_Ble(PROG_COMM_C4 prog_c4, int time_out) {
//	__android_log_print(ANDROID_LOG_INFO, "JNI_OBUISSUE", "send_c7_Ble begin");
	int i, ret, did, datalist, sys_flag = 0, veh_flag = 0;
	int keyid_for_encrypt_op, keyid_for_encrypt, psam_slot = g_psam_slot;
	//如下变量是从getsecure_rs中得到
	int gs_veh_len;
	char gs_veh_file[128];
	char gs_authenticator[8];
	//硬sam解密之后的车辆信息和mac码
	uint8 data[128];
	ST_TRANSFER_CHANNEL transfer_rq;
	g_read_file.NumOfFiles = prog_c4.NumOfFiles;
	for (i = 0; i < prog_c4.NumOfFiles; i++) {
		g_read_file.DIDnFID[i] = prog_c4.DIDnFID[i];
		g_read_file.offset[i] = prog_c4.Offset[i];
		g_read_file.len[i] = prog_c4.Length[i];
		//判断是否需要读取车辆信息，
		if (prog_c4.DIDnFID[i] == 0x11) {
			veh_flag = 1;
		}
	}
	//读取车辆信息
	if (veh_flag == 1) {
		if (prog_c4.C4Flag == 0) {
			ret = send_readveh_OnLine_To_Front_End_Processor(
					g_frame_readveh_rq);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine send_readveh_OnLine_To_Front_End_Processor ERR, ret = %d\n", ret);
				return -1001;
			}
			ret = recv_readveh_OnLine_From_Front_End_Processor(
					&g_frame_readveh_rs, time_out);

			if (ret != SUCCESS) {
#ifdef REPEAT_SEND_UDPNET
				ret = UdpNet_Repeat_Framework(ret,READ_VEHINFO_MODE,time_out);
#endif
				if (ret != 0) {
					//DEBUGP("send_c8_OnLine recv_readveh_OnLine_From_Front_End_Processor ERR, ret = %d\n", ret);
					return -1002;
				}
			}

			for (i = 0; i < 8; i++) {
				psam_random[i] = g_frame_readveh_rs.TradeRandomNumber[i];
				//为发送c9帧做准备  给c9帧中交易随机数赋值  add by wll 2013-3-26 9:40:29
				g_frame_writesys_rq.TradeRandomNumber[i] = psam_random[i];
				g_frame_syscheck_rq.TradeRandomNumber[i] = psam_random[i];
			}
			//************************End of Change ***********************************************/

			//进DF01目录 （为后面取车辆信息做准备）
			did = 0x01;
			g_ndid = did;
			esamInitFrame(&transfer_rq);
			esamEnterDirFrame(&transfer_rq, 0xdf01);
			ret = TransferChannel_rq(did, transfer_rq.channelid,
					transfer_rq.apdulist, transfer_rq.apdu);
			if (ret != SUCCESS) {
				return -1003;	//串口发送失败
			}
			ret = TransferChannel_rs(&datalist, data, time_out);
			if (ret != SUCCESS) {
				return -1004;	//解析失败或者超时
			}
			// 读车辆信息
			keyid_for_encrypt_op = 0x80;
			keyid_for_encrypt = 0x00;	//可以指定obu采用哪个加密密钥
			ret = GetSecure_rq(psam_mac, 0x00, 59, psam_random,
					keyid_for_encrypt_op, keyid_for_encrypt);	//原来是59
			if (ret != SUCCESS) {
				return -1005;
			}
			ret = GetSecure_rs(&gs_veh_len, gs_veh_file, gs_authenticator,
					time_out);
			if (ret != SUCCESS) {
				return -1006;
			}
			//为发送c9帧做准备  给c9帧中车辆信息帧密文赋值 add by wll 2013-3-26 9:40:29
			for (i = 0; i < 72; i++) {
				g_frame_writesys_rq.EncryptedVehInfo[i] = gs_veh_file[i];
			}
		}/* end if(prog_c4.C4Flag ==0) */
		else {
			did = 0x03;
			g_ndid = did;
			esamInitFrame(&transfer_rq);
			esamEnterDirFrame(&transfer_rq, 0xdf03);
			esamReadFileInfoFrame(&transfer_rq, prog_c4.Offset[1],
					prog_c4.Length[1]);
			ret = TransferChannel_rq(did, transfer_rq.channelid,
					transfer_rq.apdulist, transfer_rq.apdu);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine TransferChannel_rq ERR, ret = %d\n", ret);
				return -1007;	//串口发送失败
			}
			ret = TransferChannel_rs(&datalist, data, time_out);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine TransferChannel_rs ERR, ret = %d\n", ret);
				return -1008;	//解析失败或者超时
			}
			ret = esamCheck(data, 0);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine esamCheck ERR, ret = %d\n", ret);
				return -1009;
			}
			ret = esamCheckReadVehInfo(data, 1);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine esamCheckReadVehInfo ERR, ret = %d\n", ret);
				return -1010;	//取随机数失败直接退出
			}
		}
	}/* end if(veh_flag == 1) */

	/**************** 进3F00 目录 ，取随机数 为后面激活做准备 （如果有激活操作的话） *************************/
	did = 0x01;	//根目录
	g_ndid = did;
	esamInitFrame(&transfer_rq);
	esamEnterDirFrame(&transfer_rq, 0x3f00);
	esamGetRandFrame(&transfer_rq);
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != SUCCESS) {
		return -1011;	//串口发送失败
	}
	ret = TransferChannel_rs(&datalist, data, time_out);
	if (ret != SUCCESS) {
		return -1012;	//解析失败或者超时
	}
	ret = esamCheck(data, 0);
	if (ret != SUCCESS) {
		return -1013;
	}
	ret = esamCheckGetRand(data, 1);
	if (ret != SUCCESS) {
		return -1014;	//取随机数失败直接退出
	}

	//为发送向前置机发送写系统信息帧做准备  给帧中  随机数  赋值 add by wll 2013-3-26 9:55:43
	for (i = 0; i < 4; i++) {
		g_frame_writesys_rq.RandomNumber[i] = esam_random[i];
	}
	/********** END 进3F00 目录 ，取随机数 为后面激活做准备 （如果有激活操作的话） *************************/

	if ((vst.obustatus[0] & 0x80) == 0x00) //如果OBU插卡,则读取0002和0015文件
			{
		did = 0x01;	//应用主目录
		g_ndid = did;
		iccInitFrame(&transfer_rq);
		iccReadMoneyFrame(&transfer_rq);
		iccReadFileFrame(&transfer_rq, 0x0015, 0x00, 0x2b);
		ret = TransferChannel_rq(did, transfer_rq.channelid,
				transfer_rq.apdulist, transfer_rq.apdu);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine TransferChannel_rq ERR, ret = %d\n", ret);
			return -1015;	//串口发送失败
		}
		ret = TransferChannel_rs(&datalist, data, time_out);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine TransferChannel_rs ERR, ret = %d\n", ret);
			return -1016;	//解析失败或者超时
		}
		ret = iccCheck(data, 0);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine iccCheck ERR, ret = %d\n", ret);
			return -1017;
		}
		ret = iccCheck(data, 1);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine iccCheck ERR, ret = %d\n", ret);
			return -1018;
		}
		memcpy(icc_pib.Balance, &data[1], 4);	//0002文件
		memcpy(icc_pib.icc0015, &data[8], 43);	//0015文件
	}

	/*********************写系统信息请求帧指令，由原来的从psam计算mac转换为由前置机计算mac*********/
	//系统信息标志是0,因为所用分散因子不同
	//向前置机发送		  OBU写系统信息请求指令
	ret = send_writesys_OnLine_To_Front_End_Processor(g_frame_writesys_rq);
	if (ret != SUCCESS) {
		return -1019;
	}
	//从前置机接收		  OBU写系统信息应答指令   返回mac地址
	ret = recv_writesys_OnLine_From_Front_End_Processor(&g_frame_writesys_rs,
			time_out);
	if (ret != SUCCESS) {
#ifdef REPEAT_SEND_UDPNET
		ret = UdpNet_Repeat_Framework(ret,WRITE_SYSINFO_MODE,time_out);
#endif
		if (ret != 0) {
			//DEBUGP("send_c8_OnLine recv_writesys_OnLine_From_Front_End_Processor ERR, ret = %d\n", ret);
			return -1020;
		}
	}
	//指令的18个字节中后4个字节就是MAC地址   remark by wll 2013-3-27 16:38:15
	for (i = 0; i < 4; i++) {
		//esam_mac 是代表从前置机获取的（前置机通过终端发的obu随机数计算出的）mac
		esam_mac[i] = g_frame_writesys_rs.WriteSysInfoCommand[14 + i];
	}

	//add by wll 2014-12-19 14:36:55 写系统信息
	esamInitFrame(&transfer_rq);
	transfer_rq.apdulist++;
	transfer_rq.apdu[0] = 18;
	memcpy(&transfer_rq.apdu[1], &g_frame_writesys_rs.WriteSysInfoCommand[0],
			18);
	transfer_rq.frame_len = 18;
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != SUCCESS) {
		return -1021;	//串口发送失败
	}
	ret = TransferChannel_rs(&datalist, data, time_out);
	if (ret != SUCCESS) {
		return -1022;	//解析失败或者超时
	}

	int j = 0;
	for (j = 0; j < 30; j++)
//		__android_log_print(ANDROID_LOG_INFO, "send_c7", "%x", data[j]);
	ret = esamCheck(data, 0);
	if (ret != SUCCESS) {
		return -1023;
	}
	return SUCCESS;
}

/*
 * 函数名称：	recv_b7_OnLine()
 * 功能描述：	写车辆信息应答，但该函数只是给参数prog_b2赋值，无实质功能
 * 输入参数：	无
 * 输出参数：	prog_b2--更新命令执行响应帧
 * 返回值：	0--成功，非0--失败
 */
int recv_b7_Ble(PROG_COMM_B3 *prog_b3) {
	int i, k, ret;
	prog_b3->RSCTL = 0x13;
	prog_b3->FrameType = 0xb3;
	prog_b3->ErrorCode = 0x00;
	prog_b3->NumOfFiles = g_read_file.NumOfFiles;
	for (i = 0; i < prog_b3->NumOfFiles; i++) {
		if (g_read_file.DIDnFID[i] == 0x11)	//车辆信息文件
				{
			prog_b3->Length[i] = g_read_file.len[i];
			for (k = 0; k < prog_b3->Length[i]; k++) {
				prog_b3->FileContent[i][k] =
						g_frame_writesys_rs.DecryptionVehInfo[k];//esam_read_vehicle[k + g_read_file.offset[i]];
			}
		}
	}
	if ((vst.obustatus[0] & 0x80) == 0x00)	//如果OBU插卡,则读取0002和0015文件
			{
		prog_b3->Length[i] = 4;	//0002文件长度
		for (k = 0; k < prog_b3->Length[i]; k++) {
			prog_b3->FileContent[i][k] = icc_pib.Balance[k];
		}
		prog_b3->Length[i + 1] = 43;	//0015文件长度
		for (k = 0; k < prog_b3->Length[i + 1]; k++) {
			prog_b3->FileContent[i + 1][k] = icc_pib.icc0015[k];
		}
	}
	return SUCCESS;
}

/*
 * 函数名称：	send_c8_OnLine()
 * 功能描述：	OBU车辆信息读取
 * 操作流程：	提取prog_c4帧中的数据及判断是否为读车辆信息帧
 ->写系统信息模式则与前置机进行交互获取交易随机数，并赋值给全局变量psam_random，个性化定制模式则忽略该步操作
 ->进入obu的DF01目录读取车辆信息并返回车辆信息密文
 ->读取OBU根目录3F00的随机数'如果OBU插卡则读取0002和0015文件
 ->与前置机进行写系统信息交互
 * 输入参数：	prog_c4--读车辆信息文件指令帧
 * 				c8_type--c8函数类型：
 0--写系统信息流程模式，1--个性化定制流程模式
 *				time_out--超时时间计数，以20ms为单位
 * 输出参数：	无
 * 返回值：	0--成功，非0--失败
 */
//读车辆信息的请求与应答
int send_c8_Ble(PROG_COMM_C4 prog_c4, int time_out) {
	int i, ret, did, datalist, sys_flag = 0, veh_flag = 0;
	int keyid_for_encrypt_op, keyid_for_encrypt, psam_slot = g_psam_slot;
	//如下变量是从getsecure_rs中得到
	int gs_veh_len;
	char gs_veh_file[128];
	char gs_authenticator[8];
	//硬sam解密之后的车辆信息和mac码
	uint8 data[128];
	ST_TRANSFER_CHANNEL transfer_rq;
	g_read_file.NumOfFiles = prog_c4.NumOfFiles;
	for (i = 0; i < prog_c4.NumOfFiles; i++) {
		g_read_file.DIDnFID[i] = prog_c4.DIDnFID[i];
		g_read_file.offset[i] = prog_c4.Offset[i];
		g_read_file.len[i] = prog_c4.Length[i];
		//判断是否需要读取车辆信息，
		if (prog_c4.DIDnFID[i] == 0x11) {
			veh_flag = 1;
		}
	}
	//读取车辆信息
	if (veh_flag == 1) {
		if (prog_c4.C4Flag == 0) {
			ret = send_readveh_OnLine_To_Front_End_Processor(
					g_frame_readveh_rq);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine send_readveh_OnLine_To_Front_End_Processor ERR, ret = %d\n", ret);
				return -1001;
			}
			ret = recv_readveh_OnLine_From_Front_End_Processor(
					&g_frame_readveh_rs, time_out);

			if (ret != SUCCESS) {
#ifdef REPEAT_SEND_UDPNET
				ret = UdpNet_Repeat_Framework(ret,READ_VEHINFO_MODE,time_out);
#endif
				if (ret != 0) {
					//DEBUGP("send_c8_OnLine recv_readveh_OnLine_From_Front_End_Processor ERR, ret = %d\n", ret);
					return -1002;
				}
			}

			for (i = 0; i < 8; i++) {
				psam_random[i] = g_frame_readveh_rs.TradeRandomNumber[i];
				//为发送c9帧做准备  给c9帧中交易随机数赋值  add by wll 2013-3-26 9:40:29
				g_frame_writesys_rq.TradeRandomNumber[i] = psam_random[i];
				g_frame_syscheck_rq.TradeRandomNumber[i] = psam_random[i];
			}
			//************************End of Change ***********************************************/

			//进DF01目录 （为后面取车辆信息做准备）
			did = 0x01;
			esamInitFrame(&transfer_rq);
			esamEnterDirFrame(&transfer_rq, 0xdf01);
			ret = TransferChannel_rq(did, transfer_rq.channelid,
					transfer_rq.apdulist, transfer_rq.apdu);
			if (ret != SUCCESS) {
				return -1003;	//串口发送失败
			}
			ret = TransferChannel_rs(&datalist, data, time_out);
			if (ret != SUCCESS) {
				return -1004;	//解析失败或者超时
			}
			// 读车辆信息
			keyid_for_encrypt_op = 0x80;
			keyid_for_encrypt = 0x00;	//可以指定obu采用哪个加密密钥
			ret = GetSecure_rq(psam_mac, 0x00, 59, psam_random,
					keyid_for_encrypt_op, keyid_for_encrypt);	//原来是59
			if (ret != SUCCESS) {
				return -1005;
			}
			ret = GetSecure_rs(&gs_veh_len, gs_veh_file, gs_authenticator,
					time_out);
			if (ret != SUCCESS) {
				return -1006;
			}
			//为发送c9帧做准备  给c9帧中车辆信息帧密文赋值 add by wll 2013-3-26 9:40:29
			for (i = 0; i < 72; i++) {
				g_frame_writesys_rq.EncryptedVehInfo[i] = gs_veh_file[i];
			}
		}/* end if(prog_c4.C4Flag ==0) */
		else {
			did = 0x03;
			esamInitFrame(&transfer_rq);
			esamEnterDirFrame(&transfer_rq, 0xdf03);
			esamReadFileInfoFrame(&transfer_rq, prog_c4.Offset[1],
					prog_c4.Length[1]);
			ret = TransferChannel_rq(did, transfer_rq.channelid,
					transfer_rq.apdulist, transfer_rq.apdu);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine TransferChannel_rq ERR, ret = %d\n", ret);
				return -1007;	//串口发送失败
			}
			ret = TransferChannel_rs(&datalist, data, time_out);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine TransferChannel_rs ERR, ret = %d\n", ret);
				return -1008;	//解析失败或者超时
			}
			ret = esamCheck(data, 0);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine esamCheck ERR, ret = %d\n", ret);
				return -1009;
			}
			ret = esamCheckReadVehInfo(data, 1);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine esamCheckReadVehInfo ERR, ret = %d\n", ret);
				return -1010;	//取随机数失败直接退出
			}
		}
	}/* end if(veh_flag == 1) */

	/**************** 进3F00 目录 ，取随机数 为后面激活做准备 （如果有激活操作的话） *************************/
	did = 0x01;	//根目录
	esamInitFrame(&transfer_rq);
	esamEnterDirFrame(&transfer_rq, 0x3f00);
	esamGetRandFrame(&transfer_rq);
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != SUCCESS) {
		return -1011;	//串口发送失败
	}
	ret = TransferChannel_rs(&datalist, data, time_out);
	if (ret != SUCCESS) {
		return -1012;	//解析失败或者超时
	}
	ret = esamCheck(data, 0);
	if (ret != SUCCESS) {
		return -1013;
	}
	ret = esamCheckGetRand(data, 1);
	if (ret != SUCCESS) {
		return -1014;	//取随机数失败直接退出
	}

	//为发送向前置机发送写系统信息帧做准备  给帧中  随机数  赋值 add by wll 2013-3-26 9:55:43
	for (i = 0; i < 4; i++) {
		g_frame_writesys_rq.RandomNumber[i] = esam_random[i];
	}
	/********** END 进3F00 目录 ，取随机数 为后面激活做准备 （如果有激活操作的话） *************************/

	if ((vst.obustatus[0] & 0x80) == 0x00) //如果OBU插卡,则读取0002和0015文件
			{
		did = 0x01;	//应用主目录
		iccInitFrame(&transfer_rq);
		iccReadMoneyFrame(&transfer_rq);
		iccReadFileFrame(&transfer_rq, 0x0015, 0x00, 0x2b);
		ret = TransferChannel_rq(did, transfer_rq.channelid,
				transfer_rq.apdulist, transfer_rq.apdu);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine TransferChannel_rq ERR, ret = %d\n", ret);
			return -1015;	//串口发送失败
		}
		ret = TransferChannel_rs(&datalist, data, time_out);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine TransferChannel_rs ERR, ret = %d\n", ret);
			return -1016;	//解析失败或者超时
		}
		ret = iccCheck(data, 0);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine iccCheck ERR, ret = %d\n", ret);
			return -1017;
		}
		ret = iccCheck(data, 1);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine iccCheck ERR, ret = %d\n", ret);
			return -1018;
		}
		memcpy(icc_pib.Balance, &data[1], 4);	//0002文件
		memcpy(icc_pib.icc0015, &data[8], 43);	//0015文件
	}

	/*********************写系统信息请求帧指令，由原来的从psam计算mac转换为由前置机计算mac*********/
	//系统信息标志是0,因为所用分散因子不同
	//向前置机发送		  OBU写系统信息请求指令
	ret = send_writesys_OnLine_To_Front_End_Processor(g_frame_writesys_rq);
	if (ret != SUCCESS) {
		return -1019;
	}
	//从前置机接收		  OBU写系统信息应答指令   返回mac地址
	ret = recv_writesys_OnLine_From_Front_End_Processor(&g_frame_writesys_rs,
			time_out);
	if (ret != SUCCESS) {
#ifdef REPEAT_SEND_UDPNET
		ret = UdpNet_Repeat_Framework(ret,WRITE_SYSINFO_MODE,time_out);
#endif
		if (ret != 0) {
			//DEBUGP("send_c8_OnLine recv_writesys_OnLine_From_Front_End_Processor ERR, ret = %d\n", ret);
			return -1020;
		}
	}
	//指令的18个字节中后4个字节就是MAC地址   remark by wll 2013-3-27 16:38:15
	for (i = 0; i < 4; i++) {
		//esam_mac 是代表从前置机获取的（前置机通过终端发给他的obu随机数计算出的）mac
		esam_mac[i] = g_frame_writesys_rs.WriteSysInfoCommand[14 + i];
	}

//	//add by wll 2014-12-19 14:36:55 写系统信息
//	esamInitFrame(&transfer_rq);
//	transfer_rq.apdulist ++ ;
//	transfer_rq.apdu[0] = 18;
//	memcpy(&transfer_rq.apdu[1], &g_frame_writesys_rs.WriteSysInfoCommand[0], 18);
//	transfer_rq.frame_len = 18;
//	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist, transfer_rq.apdu);
//	if(ret != SUCCESS)
//	{
//		//DEBUGP("send_c8_OnLine TransferChannel_rq ERR, ret = %d\n", ret);
//		return -1021;	//串口发送失败
//	}
//	ret = TransferChannel_rs(&datalist, data, time_out);
//	if(ret != SUCCESS)
//	{
//		//DEBUGP("send_c8_OnLine TransferChannel_rs ERR, ret = %d\n", ret);
//		return -1022;	//解析失败或者超时
//	}
//	ret = esamCheck(data, 0);
//	if(ret != SUCCESS)
//	{
//		//DEBUGP("send_c8_OnLine esamCheck ERR, ret = %d\n", ret);
//		return -1023;
//	}
	return SUCCESS;
}

/*
 * 函数名称：	recv_b8_OnLine()
 * 功能描述：	读车辆信息应答，但该函数只是给参数prog_b3赋值，无实质功能
 * 输入参数：	无
 * 输出参数：	prog_b3--读命令执行响应帧
 * 返回值：	0--成功，非0--失败
 */
int recv_b8_Ble(PROG_COMM_B3 *prog_b3) {
	int i, k, ret;
	prog_b3->RSCTL = 0x13;
	prog_b3->FrameType = 0xb3;
	prog_b3->ErrorCode = 0x00;
	prog_b3->NumOfFiles = g_read_file.NumOfFiles;
	for (i = 0; i < prog_b3->NumOfFiles; i++) {
		if (g_read_file.DIDnFID[i] == 0x11)	//车辆信息文件
				{
			prog_b3->Length[i] = g_read_file.len[i];
			for (k = 0; k < prog_b3->Length[i]; k++) {
				prog_b3->FileContent[i][k] =
						g_frame_writesys_rs.DecryptionVehInfo[k];//esam_read_vehicle[k + g_read_file.offset[i]];
			}
		}
	}
	if ((vst.obustatus[0] & 0x80) == 0x00)	//如果OBU插卡,则读取0002和0015文件
			{
		i = 2;
		prog_b3->Length[i] = 4;	//0002文件长度
		for (k = 0; k < prog_b3->Length[i]; k++) {
			prog_b3->FileContent[i][k] = icc_pib.Balance[k];
		}
		prog_b3->Length[i + 1] = 43;	//0015文件长度
		for (k = 0; k < prog_b3->Length[i + 1]; k++) {
			prog_b3->FileContent[i + 1][k] = icc_pib.icc0015[k];
		}
	}
	return SUCCESS;
}

int send_c90016file_Ble(PROG_COMM_C4 prog_c4, uint8 pinCode[], int TimeOut) {

	int ret = 0;
	int datalist;
	ST_TRANSFER_CHANNEL transfer_rq;
//	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble");
	uint8 data[128];
	int did, i;
	int icc_flag = 0, icc_offset = 0, icc_Length = 0;
	int j = 0;
	did = 0x01;
//	__android_log_print(ANDROID_LOG_INFO, "send_c90016file_Ble", "1");
	did = 0x01;
//		__android_log_print(ANDROID_LOG_INFO, "send_c90018file_Ble", "1");
		iccInitFrame(&transfer_rq);
		icc_enter_dir(&transfer_rq, 0x1001);	//选择1001
		icc_check_Pin(&transfer_rq, 3, pinCode);	//验证PIN
		for (i = 0; i < 3; i++) {
//			__android_log_print(ANDROID_LOG_INFO, "pinCode", "%x", pinCode[i]);
		}
//		__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble icc_check_Pin rq");
		ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
				transfer_rq.apdu);
		if (ret != 0)	//串口发送失败
		{
//			__android_log_print(ANDROID_LOG_INFO, "wjjar.log", "error 1");
			return -1 + ret * 100;
		}
//		__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble icc_check_Pin rs");
		ret = TransferChannel_rs(&datalist, data, TimeOut);
		if (ret != 0)	//解析失败或者超时
		{
			__android_log_print(ANDROID_LOG_INFO, "wjjar.log", "send_c90016file_Ble icc_check_Pin rs failed");
			return -2 + ret * 100;
		}

		//__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1", "iccCheck");
//		__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble iccCheck0");
		ret = iccCheck(data, 0);
		if (ret != 0)	//接收帧校验失败
			{
//			__android_log_print(ANDROID_LOG_INFO, "wjjar.log", "iccCheck failed");
			return -3 + ret * 100;
			}

		//__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1", "iccCheck");
//		__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble iccCheck1");
		ret = iccCheck(data, 1);
		//__android_log_print(ANDROID_LOG_INFO, "iccCheck data", "check");
//		__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble finish");
		if (ret != 0) {
//			__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble failed jni");
			return -4 + ret * 100;
		}

	for (i = 0; i < prog_c4.NumOfFiles; i++) {
		g_read_file.DIDnFID[i] = prog_c4.DIDnFID[i];
		g_read_file.offset[i] = prog_c4.Offset[i];
		g_read_file.len[i] = prog_c4.Length[i];
		//判断是否需要读取系统或者车辆，有则全读
//		__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "prog_c4.DIDnFID[i] = %x",prog_c4.DIDnFID[i]);
		if ((prog_c4.DIDnFID[i] == 0x02) || (prog_c4.DIDnFID[i] == 0x12)
				|| (prog_c4.DIDnFID[i] == 0x15) || (prog_c4.DIDnFID[i] == 0x16)
				|| (prog_c4.DIDnFID[i] == 0x18) || (prog_c4.DIDnFID[i] == 0x19))//读IC卡信息
				{
			icc_flag = prog_c4.DIDnFID[i];
//			__android_log_print(ANDROID_LOG_INFO, "send_c90016file_Ble",
//					"icc_flag = %x", icc_flag);
			icc_offset = prog_c4.Offset[i];
//			__android_log_print(ANDROID_LOG_INFO, "send_c90016file_Ble",
//					"icc_offset = %x", icc_offset);
			icc_Length = prog_c4.Length[i];
//			__android_log_print(ANDROID_LOG_INFO, "send_c90018file_Ble",
//					"icc_Length = %x", icc_Length);
		} else
			return -1;
		//		__android_log_print(ANDROID_LOG_INFO, "JNI_OBUISSUE", "icc_flag = " );

	}
	//如果OBU插卡,则读取icc文件信息
	if ((vst.obustatus[0] & 0x80) == 0x00) {
		iccInitFrame(&transfer_rq);
		icc_enter_dir(&transfer_rq, 0x3F00);	//选择1001
		//icc_check_Pin(&transfer_rq, 3, pinCode);	//验证PIN
		for (i = 0; i < 3; i++) {
			//__android_log_print(ANDROID_LOG_INFO, "pinCode", "%x", pinCode[i]);
			//__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble pinCode = %x",pinCode[i]);
		}
		ret = TransferChannel_rq(did, transfer_rq.channelid,
				transfer_rq.apdulist, transfer_rq.apdu);
		if (ret != 0)	//串口发送失败
			{
//			__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble TransferChannel_rq failed");
			return -1 + ret * 100;
			}


		ret = TransferChannel_rs(&datalist, data, TimeOut);
		if (ret != 0)	//解析失败或者超时
		{
//			__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble TransferChannel_rs failed");
			return -2 + ret * 100;
		}
		did = 0x00;	//根目录
		iccInitFrame(&transfer_rq);
		iccReadFileFrame(&transfer_rq, icc_flag, icc_offset, icc_Length);
//		__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble iccReadFileFrame");
		ret = TransferChannel_rq(did, transfer_rq.channelid,
				transfer_rq.apdulist, transfer_rq.apdu);
		if (ret != SUCCESS) {
//			__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble TransferChannel_rq failed");
			return -5 + ret * 100;	//串口发送失败
		}
		ret = TransferChannel_rs(&datalist, data, TimeOut);
		for (i = 0; i < data[0] + 1; i++) {
//			__android_log_print(ANDROID_LOG_INFO, "C9 TransferChannel_rs", "%x",
//					data[i]);
			//__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble C9 TransferChannel_rs = %x",data[i]);
		}
		if (ret != SUCCESS) {
//			__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble TransferChannel_rs failed");
			return -6 + ret * 100;	//解析失败或者超时
		}
		ret = iccCheck(data, 0);
		if (ret != SUCCESS) {
//			__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90016file_Ble iccCheck failed");
			return -7 + ret * 100;
		}
		if (icc_flag == 0x0002) {
			memcpy(icc_pib.Balance, &data[1], icc_Length);	//0002文件
		} else if (icc_flag == 0x0012) {
			memcpy(icc_pib.icc0012, &data[1], icc_Length);	//0012文件
		} else if (icc_flag == 0x0015) {
			memcpy(icc_pib.icc0015, &data[1], icc_Length);	//0015文件
		} else if (icc_flag == 0x0016) {
			memcpy(icc_pib.icc0016, &data[1], icc_Length);	//0015文件
		} else if (icc_flag == 0x0018) {
			memcpy(icc_pib.icc0018, &data[1], icc_Length);	//0019文件
		} else if (icc_flag == 0x0019) {
			memcpy(icc_pib.icc0019, &data[1], icc_Length);	//0019文件
		}
	} else {
		return -8;
	}

	return ret;
}
/*
 * 函数名称：	send_c9_OnLine()
 * 功能描述：	OBU系统信息读取
 * 操作流程：	提取prog_c4帧中的数据及判断是否为读车辆信息帧
 ->进入obu的3F00目录读取系统信息并返回系统信息
 ->如果OBU插卡则读取0002和0015文件
 ->与前置机进行系统信息对比交互
 * 输入参数：	prog_c4--读系统信息文件指令帧
 *				time_out--超时时间计数，以20ms为单位
 * 输出参数：	无
 * 返回值：	0--成功，非0--失败
 */
//读系统信息的请求与应答
int send_c90018file_Ble(PROG_COMM_C4 prog_c4, uint8 pinCode[], int TimeOut) {

	int ret = 0;
//	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90018file_Ble");
	int datalist;
	ST_TRANSFER_CHANNEL transfer_rq;
	uint8 data[128];
	int did, i;
	int icc_flag = 0, icc_offset = 0, icc_Length = 0;
	int j = 0;
	did = 0x01;
//	__android_log_print(ANDROID_LOG_INFO, "send_c90018file_Ble", "1");
	iccInitFrame(&transfer_rq);
	icc_enter_dir(&transfer_rq, 0x1001);	//选择1001
	icc_check_Pin(&transfer_rq, 3, pinCode);	//验证PIN
	for (i = 0; i < 3; i++) {
		//__android_log_print(ANDROID_LOG_INFO, "pinCode", "%x", pinCode[i]);
		//__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90018file_Ble pinCode = ",pinCode[i]);
	}

	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
//	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90018file_Ble icc_check_Pin rq");
	if (ret != 0)	//串口发送失败
		return -1 + ret * 100;

//	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90018file_Ble icc_check_Pin rs");
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	if (ret != 0)	//解析失败或者超时
		return -2 + ret * 100;

//	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90018file_Ble icc_check_Pin rs finish");
//	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1", "iccCheck");
	ret = iccCheck(data, 0);
	if (ret != 0)	//接收帧校验失败
		return -3 + ret * 100;

//	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1", "iccCheck");
	ret = iccCheck(data, 1);
//	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90018file_Ble icc_check_Pin iccCheck");
//	__android_log_print(ANDROID_LOG_INFO, "iccCheck data", "check");
	if (ret != 0) {
		return -4 + ret * 100;
//		__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c90018file_Ble icc_check_Pin iccCheck failed");
	}

	for (i = 0; i < prog_c4.NumOfFiles; i++) {
		g_read_file.DIDnFID[i] = prog_c4.DIDnFID[i];
		g_read_file.offset[i] = prog_c4.Offset[i];
		g_read_file.len[i] = prog_c4.Length[i];
		//判断是否需要读取系统或者车辆，有则全读

		if ((prog_c4.DIDnFID[i] == 0x02) || (prog_c4.DIDnFID[i] == 0x12)
				|| (prog_c4.DIDnFID[i] == 0x15) || (prog_c4.DIDnFID[i] == 0x18)
				|| (prog_c4.DIDnFID[i] == 0x19))	//读IC卡信息
				{
			icc_flag = prog_c4.DIDnFID[i];
//			__android_log_print(ANDROID_LOG_INFO, "send_c90018file_Ble",
//					"icc_flag = %x", icc_flag);
			icc_offset = prog_c4.Offset[i];
//			__android_log_print(ANDROID_LOG_INFO, "send_c90018file_Ble",
//					"icc_offset = %x", icc_offset);
			icc_Length = prog_c4.Length[i];
//			__android_log_print(ANDROID_LOG_INFO, "send_c90018file_Ble",
//					"icc_Length = %x", icc_Length);
		} else
			return -1;
		//		__android_log_print(ANDROID_LOG_INFO, "JNI_OBUISSUE", "icc_flag = " );

	}
	//如果OBU插卡,则读取icc文件信息
	if ((vst.obustatus[0] & 0x80) == 0x00) {
		did = 0x01;	//应用主目录
		iccInitFrame(&transfer_rq);
		iccReadFileFrame(&transfer_rq, icc_flag, icc_offset, icc_Length);
		ret = TransferChannel_rq(did, transfer_rq.channelid,
				transfer_rq.apdulist, transfer_rq.apdu);
		if (ret != SUCCESS) {
//			__android_log_print(ANDROID_LOG_INFO, "send_c90018file_Ble", "TransferChannel_rq failed");
			return -5 + ret * 100;	//串口发送失败
		}
		ret = TransferChannel_rs(&datalist, data, TimeOut);
		for (i = 0; i < data[0] + 1; i++) {
//			__android_log_print(ANDROID_LOG_INFO, "C9 TransferChannel_rs", "%x",
//					data[i]);
//			__android_log_print(ANDROID_LOG_INFO, "send_c90018file_Ble", "C9 TransferChannel_rs = %x",data[i]);
		}
		if (ret != SUCCESS) {
//			__android_log_print(ANDROID_LOG_INFO, "send_c90018file_Ble", "C9 TransferChannel_rs failed");
			return -6 + ret * 100;	//解析失败或者超时
		}
		ret = iccCheck(data, 0);
		if (ret != SUCCESS) {
			return -7 + ret * 100;
		}
		if (icc_flag == 0x0002) {
			memcpy(icc_pib.Balance, &data[1], icc_Length);	//0002文件
		} else if (icc_flag == 0x0012) {
			memcpy(icc_pib.icc0012, &data[1], icc_Length);	//0012文件
		} else if (icc_flag == 0x0015) {
			memcpy(icc_pib.icc0015, &data[1], icc_Length);	//0015文件
		} else if (icc_flag == 0x0018) {
			memcpy(icc_pib.icc0018, &data[1], icc_Length);	//0019文件
		} else if (icc_flag == 0x0019) {
			memcpy(icc_pib.icc0019, &data[1], icc_Length);	//0019文件
		}
	} else {
		return -8;
	}

	return ret;
}
int send_c9_Ble(PROG_COMM_C4 prog_c4, int time_out) {
	int i, ret, did, datalist, sys_flag = 0;
	int icc_flag = 0, icc_offset = 0, icc_Length = 0;
	int keyid_for_encrypt_op, keyid_for_encrypt, psam_slot = g_psam_slot;
//	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c9_Ble");
	//如下变量是从getsecure_rs中得到
	int gs_veh_len;
	char gs_veh_file[128];
	char gs_authenticator[8];
	//硬sam解密之后的车辆信息和mac码
	char after_decrypt_vehinfo[128];
	char after_decrypt_mac[8];
	uint8 data[128];
	ST_TRANSFER_CHANNEL transfer_rq;
	g_read_file.NumOfFiles = prog_c4.NumOfFiles;
	g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;
	for (i = 0; i < prog_c4.NumOfFiles; i++) {
		g_read_file.DIDnFID[i] = prog_c4.DIDnFID[i];
		g_read_file.offset[i] = prog_c4.Offset[i];
		g_read_file.len[i] = prog_c4.Length[i];
		//判断是否需要读取系统或者车辆，有则全读
		if (prog_c4.DIDnFID[i] == 0x01) {
			sys_flag = 1;
		}

		if ((prog_c4.DIDnFID[i] == 0x02) || (prog_c4.DIDnFID[i] == 0x12)
				|| (prog_c4.DIDnFID[i] == 0x15) || (prog_c4.DIDnFID[i] == 0x19))//读IC卡信息
				{
			icc_flag = prog_c4.DIDnFID[i];
			icc_offset = prog_c4.Offset[i];
			icc_Length = prog_c4.Length[i];
		}

	}

	if (sys_flag == 1) {
		if (prog_c4.C4Flag == 0) {
			did = 0x01;
		} else {
			did = 0x03;
			vst.obustatus[0] = 0x80;
		}
		esamInitFrame(&transfer_rq);
		esamEnterDirFrame(&transfer_rq, 0x3f00);
		esamReadSysInfoFrame(&transfer_rq, prog_c4.Offset[0],
				prog_c4.Length[0]);
		ret = TransferChannel_rq(did, transfer_rq.channelid,transfer_rq.apdulist, transfer_rq.apdu);
		if (ret != SUCCESS) {
			return -1 + ret * 100;
		}
		ret = TransferChannel_rs(&datalist, data, time_out);
		if (ret != SUCCESS) {
			return -2 + ret * 100;
		}

		int i = 0;


		ret = esamCheck(data, 0);
		if (ret != SUCCESS) {
//			__android_log_print(ANDROID_LOG_INFO, "send_c9", "-103 ");
			return -3 + ret * 100;
		}
		ret = esamCheckReadSysInfo(data, 1);
		//print_info_Info(data,50,UDPNET);

		if (ret != SUCCESS) {
//			__android_log_print(ANDROID_LOG_INFO, "send_c9 err", "%x",ret);
			return -4 + ret * 100;
		}
//		__android_log_print(ANDROID_LOG_INFO, "esamCheckReadSysInfo", "finish");
		if ((vst.obustatus[0] & 0x80) == 0x00) //如果OBU插卡,则读取0002和0015文件
				{
//			__android_log_print(ANDROID_LOG_INFO, "vst.obustatus[0] & 0x80", "true");
			did = 0x01;	//应用主目录
			iccInitFrame(&transfer_rq);
			iccReadMoneyFrame(&transfer_rq);
			iccReadFileFrame(&transfer_rq, 0x0015, 0x00, 0x2b);
			ret = TransferChannel_rq(did, transfer_rq.channelid,
					transfer_rq.apdulist, transfer_rq.apdu);
			if (ret != SUCCESS) {
				return -5 + ret * 100;	//串口发送失败
			}
			ret = TransferChannel_rs(&datalist, data, time_out);
			if (ret != SUCCESS) {
				return -6 + ret * 100;	//解析失败或者超时
			}
			ret = iccCheck(data, 0);
			if (ret != SUCCESS) {
				return -7 + ret * 100;
			}
			ret = iccCheck(data, 1);
			if (ret != SUCCESS) {
				return -8 + ret * 100;
			}
			memcpy(icc_pib.Balance, &data[1], 4);	//0002文件
			memcpy(icc_pib.icc0015, &data[8], 43);	//0015文件
		}

		int k;
		//提取系统信息中的参数
		for (k = 0; k < 4; k++) {
			g_frame_syscheck_rq.OBUContractSignedDate[k] = esam_read_sysinfo[k
					+ 18 + g_read_file.offset[0]];
		}
		for (k = 0; k < 4; k++) {
			g_frame_syscheck_rq.OBUContractExpiredDate[k] = esam_read_sysinfo[k
					+ 22 + g_read_file.offset[0]];
		}
		g_frame_syscheck_rq.DisassembleState = esam_read_sysinfo[k + 22
				+ g_read_file.offset[0]];

		return SUCCESS;
	}/* end if(sys_flag == 1) */

	//读卡操作
	if (icc_flag != 0) {
		//如果OBU插卡,则读取icc文件信息
		if ((vst.obustatus[0] & 0x80) == 0x00) {
			did = 0x01;	//应用主目录
			iccInitFrame(&transfer_rq);
			iccReadFileFrame(&transfer_rq, icc_flag, icc_offset, icc_Length);
//			__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c9_Ble iccReadFileFrame rq");
			ret = TransferChannel_rq(did, transfer_rq.channelid,transfer_rq.apdulist, transfer_rq.apdu);
			if (ret != SUCCESS) {
//				__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c9_Ble iccReadFileFrame failed");
				return -5 + ret * 100;	//串口发送失败
			}
//			__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c9_Ble iccReadFileFrame rs");
			ret = TransferChannel_rs(&datalist, data, time_out);
			for (i = 0; i < data[0]; i++) {
//				__android_log_print(ANDROID_LOG_INFO, "C9 TransferChannel_rs",
//						"%x", data[i]);
			}
			if (ret != SUCCESS) {
//				__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c9_Ble iccReadFileFrame rs failed");
				return -6 + ret * 100;	//解析失败或者超时
			}
//			__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c9_Ble iccCheck");
			ret = iccCheck(data, 0);
			if (ret != SUCCESS) {
//				__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c9_Ble iccCheck failed");
				return -7 + ret * 100;
			}
//			__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "send_c9_Ble iccCheck success");
			if (icc_flag == 0x0002) {
				memcpy(icc_pib.Balance, &data[1], icc_Length);	//0002文件
			} else if (icc_flag == 0x0012) {
				memcpy(icc_pib.icc0012, &data[1], icc_Length);	//0012文件
			} else if (icc_flag == 0x0015) {
				memcpy(icc_pib.icc0015, &data[1], icc_Length);	//0015文件
			} else if (icc_flag == 0x0019) {
				memcpy(icc_pib.icc0019, &data[1], icc_Length);	//0019文件
			}
		} else {
			return -8;
		}
	}

	return SUCCESS;
}

int recv_b9_Blefile(PROG_COMM_B3 *prog_b3, uint8 DidFid) {
	int i, k, ret;
	prog_b3->RSCTL = 0x13;
	prog_b3->FrameType = 0xb3;
	prog_b3->ErrorCode = 0x00;
	prog_b3->NumOfFiles = g_read_file.NumOfFiles;
//	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "recv_b9_Blefile");
//	for (i = 0; i < prog_b3->NumOfFiles; i++) {
//		if (g_read_file.DIDnFID[i] == 0x01)	//系统信息文件
//				{
//			prog_b3->Length[i] = g_read_file.len[i];
//			for (k = 0; k < prog_b3->Length[i]; k++) {
//				prog_b3->FileContent[i][k] = esam_read_sysinfo[k
//						+ g_read_file.offset[i]];
//			}
//		}
//	}
//	if ((vst.obustatus[0] & 0x80) == 0x00)	//如果OBU插卡,则读取0002和0015文件
//			{
//		i = 2;
//		prog_b3->Length[i] = 4;	//0002文件长度
//		for (k = 0; k < prog_b3->Length[i]; k++) {
//			prog_b3->FileContent[i][k] = icc_pib.Balance[k];
//		}
//
//		prog_b3->Length[i + 1] = 43;	//0012文件长度
//		for (k = 0; k < prog_b3->Length[i + 1]; k++) {
//			prog_b3->FileContent[i + 1][k] = icc_pib.icc0012[k];
//		}
//
//		prog_b3->Length[i + 2] = 43;	//0015文件长度
//		for (k = 0; k < prog_b3->Length[i + 2]; k++) {
//			prog_b3->FileContent[i + 2][k] = icc_pib.icc0015[k];
//		}
//
//		prog_b3->Length[i + 3] = 43;	//0019文件长度
//		for (k = 0; k < prog_b3->Length[i + 3]; k++) {
//			prog_b3->FileContent[i + 3][k] = icc_pib.icc0019[k];
//		}
//	}
	if (DidFid == 0x19) {
		prog_b3->Length[0] = 43;	//0019文件长度
		for (k = 0; k < prog_b3->Length[0]; k++) {
			prog_b3->FileContent[0][k] = icc_pib.icc0019[k];
		}
	}else if(DidFid == 0x16){
		prog_b3->Length[0] = 55;	//0016文件长度
		for (k = 0; k < prog_b3->Length[0]; k++) {
			prog_b3->FileContent[0][k] = icc_pib.icc0016[k];
//			__android_log_print(ANDROID_LOG_INFO, "icc_pib.icc0016", "%x",icc_pib.icc0016[k]);
		}
	}
	else if (DidFid == 0x18) {

		prog_b3->Length[0] = 23;	//0019文件长度
		for (k = 0; k < prog_b3->Length[0]; k++) {
			prog_b3->FileContent[0][k] = icc_pib.icc0018[k];
		}
	}

	return SUCCESS;
}
/*
 * 函数名称：	recv_b9_OnLine()
 * 功能描述：	写车辆信息应答，但该函数只是给参数prog_b3赋值，无实质功能
 * 输入参数：	无
 * 输出参数：	prog_b3--更新命令执行响应帧
 * 返回值：	0--成功，非0--失败
 */
int recv_b9_Ble(PROG_COMM_B3 *prog_b3) {
//	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "recv_b9_Ble");
	int i, k, ret;
	prog_b3->RSCTL = 0x13;
	prog_b3->FrameType = 0xb3;
	prog_b3->ErrorCode = 0x00;
	prog_b3->NumOfFiles = g_read_file.NumOfFiles;
	for (i = 0; i < prog_b3->NumOfFiles; i++) {
		if (g_read_file.DIDnFID[i] == 0x01)	//系统信息文件
				{
			prog_b3->Length[i] = g_read_file.len[i];
			for (k = 0; k < prog_b3->Length[i]; k++) {
				prog_b3->FileContent[i][k] = esam_read_sysinfo[k
						+ g_read_file.offset[i]];
			}
		}
	}
	if ((vst.obustatus[0] & 0x80) == 0x00)	//如果OBU插卡,则读取0002和0015文件
			{
		i = 2;
		prog_b3->Length[i] = 4;	//0002文件长度
		for (k = 0; k < prog_b3->Length[i]; k++) {
			prog_b3->FileContent[i][k] = icc_pib.Balance[k];
		}

		prog_b3->Length[i + 1] = 43;	//0012文件长度
		for (k = 0; k < prog_b3->Length[i + 1]; k++) {
			prog_b3->FileContent[i + 1][k] = icc_pib.icc0012[k];
		}

		prog_b3->Length[i + 2] = 43;	//0015文件长度
		for (k = 0; k < prog_b3->Length[i + 2]; k++) {
			prog_b3->FileContent[i + 2][k] = icc_pib.icc0015[k];
		}

		prog_b3->Length[i + 3] = 43;	//0019文件长度
		for (k = 0; k < prog_b3->Length[i + 3]; k++) {
			prog_b3->FileContent[i + 3][k] = icc_pib.icc0019[k];
		}
	}
	return SUCCESS;
}

//int recv_b9list_Ble(PROG_COMM_B3LIST *prog_b3list) {
//	int i, k, ret;
//	prog_b3list->RSCTL = 0x13;
//	prog_b3list->FrameType = 0xb3;
//	prog_b3list->ErrorCode = 0x00;
//	prog_b3list->NumOfFiles = g_read_file.NumOfFiles;
//	for (i = 0; i < prog_b3list->NumOfFiles; i++) {
//		if (g_read_file.DIDnFID[i] == 0x01)	//系统信息文件
//				{
//			prog_b3list->Length[i] = g_read_file.len[i];
//			for (k = 0; k < prog_b3list->Length[i]; k++) {
//				prog_b3list->FileContent[i][k] = esam_read_sysinfo[k
//						+ g_read_file.offset[i]];
//			}
//		}
//	}
//	if ((vst.obustatus[0] & 0x80) == 0x00)	//如果OBU插卡,则读取0002和0015文件
//			{
////		i = 2;
////		prog_b3->Length[i] = 4;	//0002文件长度
////		for (k = 0; k < prog_b3list->Length[i]; k++) {
////			prog_b3list->FileContent[i][k] = icc_pib.Balance[k];
////		}
////
////		prog_b3->Length[i + 1] = 43;	//0012文件长度
////		for (k = 0; k < prog_b3list->Length[i + 1]; k++) {
////			prog_b3list->FileContent[i + 1][k] = icc_pib.icc0012[k];
////		}
////
////		prog_b3->Length[i + 2] = 43;	//0015文件长度
////		for (k = 0; k < prog_b3list->Length[i + 2]; k++) {
////			prog_b3list->FileContent[i + 2][k] = icc_pib.icc0015[k];
////		}
////
////		prog_b3->Length[i + 3] = 43;	//0019文件长度
////		for (k = 0; k < prog_b3list->Length[i + 3]; k++) {
////			prog_b3->FileContent[i + 3][k] = icc_pib.icc0019[k];
////		}
//	}
//	return SUCCESS;
//}

/*
 * 函数名称：	send_c10_Ble()
 * 功能描述：	读取交易记录文件
 * 输入参数：	int record_num,记录号
 *		   int TimeOut  超时时间
 * 输出参数：	pTradeInfo 指定记录号的交易信息的结构体内容
 * 返回值：	0--成功，非0--失败
 */
//
int send_c10_Ble(uint8 *pTradeInfo, int record_num, int TimeOut) {
	ST_TRANSFER_CHANNEL transfer_rq;
	int datalist;
	uint8 data[128];
	int did, i;
	int ret;
	did = 0x03;		// 根目录
	esamInitFrame(&transfer_rq);
	esamReadRecordInfoFrame(&transfer_rq, record_num, 0x39);
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	//串口发送失败
	if (ret != 0)
		return -1 + ret * 10;
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	//解析失败或者超时
	if (ret != 0)
		return -2 + ret * 10;

	ret = esamCheckReadFileInfo(data, 0);
	if (ret != 0)
		return -3 + ret * 10;		//取随机数失败直接退出

	memcpy((char *) pTradeInfo, &data[1], 0x39);

//模拟写交易记录
//	esamInitFrame(&transfer_rq);
//	esamWriteRecordInfoFrame(&transfer_rq, record_num, 0x39);
//	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist, transfer_rq.apdu);
//	//串口发送失败
//	if(ret != 0)
//		return -1010;
//	is_transfer_recved = 1;	//收到或者超时退出都不再收数
//	sem_destroy(&g_sem_transfer);
//	ret = TransferChannel_rs(&datalist,data,TimeOut);
//	//解析失败或者超时
//	if(ret != 0)
//		return -1011;

	return 0;
}

/*
 * 函数名称：	send_c11_Ble()
 * 功能描述：	圈存实现过程
 * 输入参数：
 * uint8 Addmoney[], 圈存金额
 * int miyaotype,秘钥类型
 * int miyaoFlag,秘钥标志
 * int TimeOut 等待超时时间
 * 输出参数：无
 * 返回值：	0--成功，非0--失败
 */
//
int send_c11_Ble(uint8 UserName[], uint8 Addmoney[], int miyaotype,
		int miyaoFlag, int TimeOut) {
	ST_TRANSFER_CHANNEL transfer_rq;
	int datalist;
	uint8 data[128];
	int did, i;
	int ret;
	did = 0x01;
	miyaotype = 2;
	uint8 banlance[4] = { 0x00, 0x00, 0x00, 0x00 };		//(0, 4)余额
	uint8 paySerial[2] = { 0x00, 0x00 };		//(4, 2)联机交易序号
	uint8 passtyperand[4] = { 0x00, 0x00, 0x00, 0x00 };		//(8, 4)伪随机数

	uint8 passkey[8] = { 0x00, 0x00, 0x00, 0x00 };	//过程秘钥

	uint8 dealtime[9] = { 0x20, 0x15, 0x01, 0x01, 0x10, 0x10, 0x10 };//DateTime.Now.ToString("yyyyMMddHHmmss");
	GetTimebufFunction(dealtime);
	dealtime[4] = dealtime[5];
	dealtime[5] = dealtime[6];
	dealtime[6] = dealtime[7];

	uint8 mac1[10] = { 0 };	//(12,4)MAC1码
	uint8 mac2[10] = { 0 };	//MAC2码

	uint8 RecvLen = 0;
	uint8 RecvDate[128];

	memcpy(&g_frame_quancun_init_rq.UserName, &UserName[0],
			sizeof(g_frame_quancun_init_rq.UserName));
	memcpy(&g_frame_quancun_init_rq.Quancun_Money[0], &Addmoney[0],
			sizeof(g_frame_quancun_init_rq.Quancun_Money));

	//g_frame_quancun_init_rs = g(g_frame_quancun_init_rq);
	ret = send_Quancun_Init_OnLine_To_Front_End_Processor(
			g_frame_quancun_init_rq);
	if (ret != SUCCESS) {
		return -1 + ret * 100;
	}

	ret = recv_Quancun_Init_OnLine_From_Front_End_Processor(
			&g_frame_quancun_init_rs, TimeOut);
	if (ret != SUCCESS) {
#ifdef REPEAT_SEND_UDPNET
		ret = UdpNet_Repeat_Framework(ret,QUANCUN_INIT_MODE,TimeOut);
#endif
		if (ret != 0) {
			return -2 + ret * 100;
		}
	}

	iccInitFrame(&transfer_rq);
	icc_getOneDispersed(&transfer_rq);	//获取1级分散因子
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != 0)	//串口发送失败
		return -3 + ret * 100;
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	if (ret != 0)	//解析失败或者超时
		return -4 + ret * 100;

	ret = iccCheckandGetData(data, 0, g_frame_quancun_rq.OneDispersed);
	if (ret != 0)
		return -5 + ret * 100;	//取一级分散因子失败直接退出

	iccInitFrame(&transfer_rq);
	icc_getTwoDispersed(&transfer_rq);	//获取2级分散因子
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != 0)	//串口发送失败
		return -6 + ret * 100;
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	if (ret != 0)	//解析失败或者超时
		return -7 + ret * 100;

	ret = iccCheckandGetData(data, 0, g_frame_quancun_rq.TwoDispersed);
	if (ret != 0)
		return -8 + ret * 100;	//取二级分散因子失败直接退出

	uint8 datatmp[3] = { 0x12, 0x34, 0x56 };
	iccInitFrame(&transfer_rq);
	icc_enter_dir(&transfer_rq, 0x1001);	//选择1001
	icc_check_Pin(&transfer_rq, 3, datatmp);	//验证PIN

	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != 0)	//串口发送失败
		return -9 + ret * 100;
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	if (ret != 0)	//解析失败或者超时
		return -10 + ret * 100;

	ret = iccCheck(data, 0);
	if (ret != 0)	//接收帧校验失败
		return -11 + ret * 100;

	ret = iccCheck(data, 1);
	if (ret != 0)	//接收帧校验失败
		return -12 + ret * 100;

	//圈存初始化
	iccInitFrame(&transfer_rq);
	iccinit_for_loadFrame(&transfer_rq, Addmoney,
			g_frame_quancun_init_rs.TerminalNO, miyaotype, miyaoFlag);
//	__android_log_print(ANDROID_LOG_INFO, "miyaotype", "%x",miyaotype);
//	__android_log_print(ANDROID_LOG_INFO, "miyaoFlag", "%x",miyaoFlag);
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
//	for(i = 0;i<(transfer_rq.apdu[0]+1);i++){
//				__android_log_print(ANDROID_LOG_INFO, "quancuninit rq transfer_rq.apdu", "%x",transfer_rq.apdu[i]);
//			}
	if (ret != 0)	//串口发送失败
		return -13 + ret * 100;
	ret = TransferChannel_rs(&datalist, data, TimeOut);
//	for(i = 0;i<(data[0]+1);i++){
//					__android_log_print(ANDROID_LOG_INFO, "quancuninit rs data", "%x",data[i]);
//				}
	if (ret != 0)	//解析失败或者超时
		return -14 + ret * 100;

	ret = iccCheck(data, 0);
	if (ret != 0)
		return -15 + ret * 100;	//接收数据校验失败直接退出

	memcpy(RecvDate, &data[1], data[0]);

	for (i = 0; i < 4; i++) {	//(0, 8)余额
		banlance[i] = RecvDate[i];
	}
	for (i = 0; i < 2; i++) {	//(8, 4)联机交易序号
		paySerial[i] = RecvDate[4 + i];
		g_frame_quancun_rq.QuancunTradeNo[i] = RecvDate[4 + i];
	}
	for (i = 0; i < 4; i++) {	//16, 8)伪随机数
		passtyperand[i] = RecvDate[8 + i];
		g_frame_quancun_rq.BogusRandNumber[i] = RecvDate[8 + i];
	}
	for (i = 0; i < 4; i++) {	//(24, 8)MAC1码
		mac1[i] = RecvDate[12 + i];
	}

	//软件验证MAC1模块
	//(暂无)
	//过程密钥
	ret = send_Quancun_OnLine_To_Front_End_Processor(g_frame_quancun_rq);
	if (ret != SUCCESS) {
		return -16 + ret * 100;
	}
	ret = recv_Quancun_OnLine_From_Front_End_Processor(&g_frame_quancun_rs,
			TimeOut);
	if (ret != SUCCESS) {
#ifdef REPEAT_SEND_UDPNET
		ret = UdpNet_Repeat_Framework(ret,QUANCUN_MODE,TimeOut);
#endif
		if (ret != 0) {
			return -17 + ret * 100;
		}
	}
	//my_TripleDES(0x01,MainKey,8,passkey,KEY);
	memcpy(passkey, g_frame_quancun_rs.PassKey, 8);

	//软件计算MAC
	uint8 MacData[30] = { 0 };
	memcpy(&MacData[0], &Addmoney[0], 4);
	MacData[4] = 0x02;
	memcpy(&MacData[5], &g_frame_quancun_init_rs.TerminalNO[0], 6);
	memcpy(&MacData[11], &dealtime[0], 7);

	uint8 Macinit[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	FD_SingleMAC(passkey, Macinit, 18, MacData, mac2);

	//组帧并发送圈存指令
	iccInitFrame(&transfer_rq);
	icc_loadFrame(&transfer_rq, dealtime, mac2);	//圈存
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != 0)	//串口发送失败
		return -18 + ret * 100;;
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	if (ret != 0)	//解析失败或者超时
		return -19 + ret * 100;;

	ret = iccCheck(data, 0);
	if (ret != 0)
		return -20 + ret * 100;;

	return 0;
}

/*
 * 函数名称：	send_c12_Ble()
 * 功能描述：	后台用户登陆
 * 输入参数：	BankICcardNum,用户名
 * 			int UserNameLen,用户名长度
 * 			uint8 UserPassword[],用户密码
 * 			int UserPassword,用户密码长度
 * 			int TimeOut 超时时间
 * 输出参数：	无
 * 返回值：	0--成功，非0--失败
 */
int send_c12_Ble(uint8 BankICcardNum[], uint8 money[], uint8 UserName[]) {
	int ret;

	uint8 RecvLen = 0;
	uint8 RecvDate[128];
	memset(&g_frame_Recharge_rq.Net_UserName[0], 0x00, 22);
	memset(&g_frame_Recharge_rq.BAND_ICCardNum[0], 0x00, 20);
	memset(&g_frame_Recharge_rq.ICCard_RechargeMoney[0], 0x00, 4);

	memcpy(&g_frame_Recharge_rq.Net_UserName[0], &UserName[0], 22);
	memcpy(&g_frame_Recharge_rq.BAND_ICCardNum[0], &BankICcardNum[0], 20);
	memcpy(&g_frame_Recharge_rq.ICCard_RechargeMoney[0], &money[0], 4);

	ret = send_Recharge_OnLine_To_Front_End_Processor(g_frame_Recharge_rq);
	if (ret != SUCCESS) {
		return -1 + ret * 10;
	}

	return 0;
}

/*
 * 函数名称：	recv_b12_Ble()
 * 功能描述：	后台用户登陆
 * 输入参数：	uint8 UserName[],用户名
 * 			int UserNameLen,用户名长度
 * 			uint8 UserPassword[],用户密码
 * 			int UserPassword,用户密码长度
 * 			int TimeOut 超时时间
 * 输出参数：	无
 * 返回值：	0--成功，非0--失败
 */
int recv_b12_Ble(uint8 *pcurBalance, int TimeOut) {
	int ret;

	ret = recv_Recharge_OnLine_From_Front_End_Processor(&g_frame_Recharge_rs,
			TimeOut);
	if (ret != SUCCESS) {
#ifdef REPEAT_SEND_UDPNET
		ret = UdpNet_Repeat_Framework(ret,RECHARGE_MODE,TimeOut);
#endif
		if (ret != 0) {
			return -1 + ret * 10;
		}
	}

	memcpy(pcurBalance, g_frame_Recharge_rs.User_curAccount,
			sizeof(g_frame_Recharge_rs.User_curAccount));
	return 0;
}

/*
 * 函数名称：	send_c13_Ble()
 * 功能描述：	后台用户登陆
 * 输入参数：	uint8 UserName[],用户名
 * 			int UserNameLen,用户名长度
 * 			uint8 UserPassword[],用户密码
 * 			int UserPassword,用户密码长度
 * 			int TimeOut 超时时间
 * 输出参数：	无
 * 返回值：	0--成功，非0--失败
 */
int send_c13_Ble(uint8 UserName[], int UserNameLen, uint8 UserPassword[],
		int UserPasswordLen, int TimeOut) {
	int ret;

	uint8 RecvLen = 0;
	uint8 RecvDate[128];

	memset(&g_frame_NetLogin_rq.Net_UserName[0], 0x00, 22);
	memset(&g_frame_NetLogin_rq.Net_UserPsaaword[0], 0x00, 16);

	memcpy(&g_frame_NetLogin_rq.Net_UserName[0], &UserName[0], UserNameLen);
	memcpy(&g_frame_NetLogin_rq.Net_UserPsaaword[0], &UserPassword[0],
			UserPasswordLen);

	ret = send_NetLogin_OnLine_To_Front_End_Processor(g_frame_NetLogin_rq);
	if (ret != SUCCESS) {
		return -1 + ret * 10;
	}
	ret = recv_NetLogin_OnLine_From_Front_End_Processor(&g_frame_NetLogin_rs,
			TimeOut);
	if (ret != SUCCESS) {
#ifdef REPEAT_SEND_UDPNET
		ret = UdpNet_Repeat_Framework(ret,NETLOGIN_MODE,TimeOut);
#endif
		if (ret != 0) {
			return -2 + ret * 10;
		}
	}
	return 0;
}

/**
 * 函数名称：	send_c14_Ble()
 * 功能描述：	按照传输进来的文件名，发送图片的数据
 * 输入参数：	ChkPhotoName 图片文件名
 * 				ChkPhotoNameLen 照片文件名长度（确保文件名的正确，无后缀乱码的数据出现）
 * 				ChkInfo 验证信息
 *				ChkInfoLen 验证信息的长度
 * 				TimeOut 超时等待时间
 * 输出参数：	无
 * 返回值：	0--成功，非0--失败
 */
int send_c14_Ble(uint8 ChkPhotoName[], int ChkPhotoNameLen, uint8 ChkInfo[],
		int ChkInfoLen, int TimeOut) {
	int ret;

	uint8 RealSendLen = 0;
	uint8 SendLen = 0;
	uint8 SendFileName[256];
	uint8 SendDate[256];
	uint8 RecvLen = 0;
	uint8 RecvDate[128];
	uint16 FrameNo = 0;

	memset(SendFileName, 0x00, 256);
	memcpy(SendFileName, ChkPhotoName, ChkPhotoNameLen);

	FILE *fp;
	if ((fp = fopen(SendFileName, "rb")) == NULL) {
		return -1;
	}
	rewind(fp);

	int pos = 0;
	while (1) {
		if (FrameNo == 0) {
			memcpy(&g_frame_ChkIssVal_rq.FrameFileContent[0], ChkInfo,
					ChkInfoLen);
			pos = 0x10;
			SendLen = 0xC8;
		} else {
			pos = 0x00;
			SendLen = 0xD8;
		}
		if (SendLen
				!= (RealSendLen = fread(SendDate, sizeof(char), SendLen, fp))) {
			FrameNo = 0xFFFF;
		}

		g_frame_ChkIssVal_rq.FrameSerialNum[0] = (FrameNo >> 8) & 0xFF;
		g_frame_ChkIssVal_rq.FrameSerialNum[1] = FrameNo & 0xFF;
		memcpy(&g_frame_ChkIssVal_rq.FrameFileContent[pos], SendDate, SendLen);

		ret = send_ChkIssVal_OnLine_To_Front_End_Processor(
				g_frame_ChkIssVal_rq);
		if (ret != SUCCESS) {
			return -2 + ret * 10;
		}
		ret = recv_ChkIssVal_OnLine_From_Front_End_Processor(
				&g_frame_ChkIssVal_rs, TimeOut);
		if (ret != SUCCESS) {
#ifdef REPEAT_SEND_UDPNET
			ret = UdpNet_Repeat_Framework(ret,CHKISSVAL_MODE,TimeOut);
#endif
			if (ret != 0) {
				return -3 + ret * 10;
			}
		}

		if (FrameNo == 0xFFFF)
			break;
		FrameNo++;
	}
	fclose(fp);

	return 0;
}

/**
 * 函数名称：	send_c15_Ble()
 * 功能描述：	发送指定帧号的校验帧
 * 输入参数：	PhotoDataBuf 照片数据
 * 				PhotoDataBufLen 照片数据长度
 * 				nIndex 该帧数据的帧序号
 * 输出参数：	无
 * 返回值：	0--成功，非0--失败
 */
int send_c15_Ble(uint8 PhotoDataBuf[], int PhotoDataBufLen, int nIndex) {
	uint8 RecvLen = 0;
	uint8 RecvDate[128];
	uint16 FrameNo = 0;

	int ret = SUCCESS;
	uint32 len = 0;
	uint8 i = 0, chk = 0;
	uint8 buf[2000] = { 0 };

	int pos = 0;

	buf[pos++] = 0x29;	//帧类型编号

	buf[pos++] = ((PhotoDataBufLen + 5) >> 8) & 0xFF;	//帧长度
	buf[pos++] = (PhotoDataBufLen + 5) & 0xFF;	//帧长度

	buf[pos++] = ((nIndex) >> 8) & 0xFF;	//帧序号
	buf[pos++] = (nIndex) & 0xFF;	//帧序号

	memcpy(&buf[pos], PhotoDataBuf, PhotoDataBufLen);
	pos += PhotoDataBufLen;
	len = pos;
	//网络帧数据转义
	g_udpnet_tx_len = len;
	memcpy(&g_udpnet_tx_buf[0], &buf[0], len);
//	g_udpnet_tx_len = pkt_code_udpnet(&buf[0], &g_udpnet_tx_buf[0], len);

	//调用底层网口发送函数
	ret = Net_send();
	sem_init(&g_sem_udpnet_init_2A, 0, 0);
	is_udpnetinit_recved = 0;	//收到或者超时退出都不再收数
	if (ret != SUCCESS) {
		return -1 + ret * 10;
	}
	return 0;
}

/**
 * 函数名称：	recv_b15_Ble()
 * 功能描述：	接收校验帧的回复
 * 输入参数：	TimeOut 超时等待时间
 * 输出参数：	无
 * 返回值：	0--成功，非0--失败
 */
int recv_b15_Ble(int TimeOut) {
	int ret;
	ret = recv_ChkIssVal_OnLine_From_Front_End_Processor(&g_frame_ChkIssVal_rs,
			TimeOut);
	if (ret != SUCCESS) {
#ifdef REPEAT_SEND_UDPNET
		ret = UdpNet_Repeat_Framework(ret, CHKISSVAL_MODE, TimeOut);
#endif
		if (ret != 0) {
			return -2 + ret * 10;
		}
	}
	return 0;
}
/**
 * 函数名称：	Enter_EsamDir()
 * 功能描述：	进ESAM目录
 * 输入参数：	dir 目录
 * 输出参数：	无
 * 返回值：	0--成功，非0--失败
 */
int Enter_EsamDir(int dir, int time_out) {
	int ret, did = 0x01, datalist;
	uint8 data[128];
	ST_TRANSFER_CHANNEL transfer_rq;

	esamInitFrame(&transfer_rq);
	esamEnterDirFrame(&transfer_rq, (uint16) dir);
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != SUCCESS) {
		//DEBUGP("send_c4 TransferChannel_rq ERR, ret = %d\n", ret);
		return -1 + ret * 100;	//串口发送失败
	}
	ret = TransferChannel_rs(&datalist, data, time_out);
	if (ret != SUCCESS) {
		//DEBUGP("send_c4 TransferChannel_rs ERR, ret = %d\n", ret);
		return -2 + ret * 100;	//解析失败或者超时
	}
	ret = esamCheck(data, 0);
	if (ret != SUCCESS) {
		//DEBUGP("send_c4 esamCheck ERR, ret = %d\n", ret);
		return -3 + ret * 100;
	}
	return 0;
}

int send_c16_Ble(int channelid, int flashCount, int time_out) {
	uint8 i = 0;
	uint8 macidtmp[4];
	int did, ret = SUCCESS, datalist;
	int replacekey_flag; //主控密钥是1,其他密钥均为0
	int psam_slot = g_master_card;
	uint8 data[128] = { 0 };
	ST_TRANSFER_CHANNEL transfer_rq;
	g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;

	//替换之前需要获得芯片序列号,并且准备替换主维护密钥所用的随机数
	did = 0x00;	//根目录

	ledInitFrame(&transfer_rq, channelid);
	ledflash(&transfer_rq, flashCount);
	memcpy(macidtmp, vst.macid, sizeof(macidtmp));
	vst.macid[0] = 0x08;
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	memcpy(vst.macid, macidtmp, sizeof(macidtmp));
	//串口发送失败
	if (ret != SUCCESS) {
		//DEBUGP("send_c2 TransferChannel_rq ERR, ret = %d\n", ret);
		return -1 + ret * 100;
	}

	return SUCCESS;
}

int recv_b16_Ble(int time_out) {
	int ret = SUCCESS;

	int datalist;
	uint8 data[128] = { 0 };

	ret = TransferChannel_rs(&datalist, data, time_out);
	//解析失败或者超时
	if (ret != SUCCESS) {
		//DEBUGP("send_c2 TransferChannel_rs ERR, ret = %d\n", ret);
		return -2 + ret * 100;
	}
	ret = ledflashCheck(data, 0);
	if (ret != SUCCESS) {
		//DEBUGP("send_c2 esamCheckGetSn ERR, ret = %d\n", ret);
		return -3 + ret * 100;
	}

	return SUCCESS;
}

int recv_b17_Ble(int time_out) {
	int ret = SUCCESS;

	int datalist;
	uint8 data[128] = { 0 };

	uint8 channelID = 0;

	sem_init(&g_sem_transfer, 0, 0);
	is_transfer_recved = 0;	//收到或者超时退出都不再收数
	ret = TransferChannel_rs_channelID(&channelID, &datalist, data, time_out);
	//解析失败或者超时
	if (ret != SUCCESS) {
		//DEBUGP("send_c2 TransferChannel_rs ERR, ret = %d\n", ret);
		return -2 + ret * 100;
	}
	if (channelID != 0x07) {
		//DEBUGP("send_c2 esamCheckGetSn ERR, ret = %d\n", ret);
		return -3 + ret * 100;
	}
	if (data[0] == 0 || data[0] == 1) {
		return data[0];
	} else {
		return -4 + ret * 100;
	}

}
int getQuanCun_Rq_Ble(FrameQuanCunRq *data_rq) {
	ST_TRANSFER_CHANNEL transfer_rq;
	int datalist;
	uint8 data[128];
	int did, i;
	int ret;
	did = 0x01;

//		//软件验证MAC1模块
//		//(暂无)
//		//过程密钥
	ret = send_Quancun_OnLine_To_Front_End_Processor(g_frame_quancun_rq);
//	memset(&g_frame_quancun_rq, 0x0a, sizeof(g_frame_quancun_rq));
	memcpy(data_rq, &g_frame_quancun_rq, sizeof(data_rq));
//	for (i = 0; i < 5; i++) {
//		data_rq->QuancunTradeNo[i] = 0x0a;
//		//__android_log_print(ANDROID_LOG_INFO, "getQuanCun_Rq_Ble ret","%x",data_rq->QuancunTradeNo[i]);
//	}
	__android_log_print(ANDROID_LOG_INFO, "getQuanCun_Rq_Ble ret", "%x", ret);
	if (ret != SUCCESS) {
		__android_log_print(ANDROID_LOG_INFO, "getQuanCun_Rq_Ble ret2", "%x",
				ret);
		return -16 + ret * 100;
	}

//		ret = recv_Quancun_OnLine_From_Front_End_Processor(&g_frame_quancun_rs,
//				TimeOut);
//		if (ret != SUCCESS) {
//#ifdef REPEAT_SEND_UDPNET
//			ret = UdpNet_Repeat_Framework(ret,QUANCUN_MODE,TimeOut);
//#endif
//			if (ret != 0) {
//				return -17 + ret * 100;
//			}
//		}
//		//my_TripleDES(0x01,MainKey,8,passkey,KEY);
//		memcpy(passkey, g_frame_quancun_rs.PassKey, 8);
//
//		//软件计算MAC
//		uint8 MacData[30] = { 0 };
//		memcpy(&MacData[0], &Addmoney[0], 4);
//		MacData[4] = 0x02;
//		memcpy(&MacData[5], &g_frame_quancun_init_rs.TerminalNO[0], 6);
//		memcpy(&MacData[11], &dealtime[0], 7);
//
//		uint8 Macinit[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//
//		FD_SingleMAC(passkey, Macinit, 18, MacData, mac2);
//
//		//组帧并发送圈存指令
//		iccInitFrame(&transfer_rq);
//		icc_loadFrame(&transfer_rq, dealtime, mac2);			//圈存
//		ret = TransferChannel_rq(did, transfer_rq.channelid,
//				transfer_rq.apdulist, transfer_rq.apdu);
//		if (ret != 0)	//串口发送失败
//			return -18 + ret * 100;;
//		ret = TransferChannel_rs(&datalist, data, TimeOut);
//		if (ret != 0)	//解析失败或者超时
//			return -19 + ret * 100;;
//
//		ret = iccCheck(data, 0);
//		if (ret != 0)
//			return -20 + ret * 100;;
	//__android_log_print(ANDROID_LOG_INFO, "getQuanCun_Rq_Ble ret3","%x",ret);
	return 0;
}
int setQuanCun_Rs_Ble(FrameQuanCunRs *data_rs, uint8 *UserName,
		uint8 Addmoney[], int miyaotype, int miyaoFlag, int TimeOut) {
	ST_TRANSFER_CHANNEL transfer_rq;

	__android_log_print(ANDROID_LOG_INFO, "setQuanCun_Rs_Ble", "just return");
//
//	int datalist;
//	uint8 data[128];
//	int did, i;
//	int ret;
//	did = 0x01;
//	uint8 banlance[4] = { 0x00, 0x00, 0x00, 0x00 };	//(0, 4)余额
//	uint8 paySerial[2] = { 0x00, 0x00 };	//(4, 2)联机交易序号
//	uint8 passtyperand[4] = { 0x00, 0x00, 0x00, 0x00 };	//(8, 4)伪随机数
//
//	uint8 passkey[8] = { 0x00, 0x00, 0x00, 0x00 };	//过程秘钥
//
//	uint8 dealtime[9] = { 0x20, 0x15, 0x01, 0x01, 0x10, 0x10, 0x10 };//DateTime.Now.ToString("yyyyMMddHHmmss");
//	GetTimebufFunction(dealtime);
//	dealtime[4] = dealtime[5];
//	dealtime[5] = dealtime[6];
//	dealtime[6] = dealtime[7];
//	for(i = 0;i < 7; i++){
//		   	   	  __android_log_print(ANDROID_LOG_INFO, "calling l_dealtime", "%x",dealtime[i]);
//		   	}
//	uint8 mac1[10] = { 0 };	//(12,4)MAC1码
//	uint8 mac2[10] = { 0 };	//MAC2码
//
//	uint8 RecvLen = 0;
//	uint8 RecvDate[128];
//	memcpy(passkey, g_frame_quancun_rs.PassKey, 8);
//	for(i = 0;i < 8; i++){
//			__android_log_print(ANDROID_LOG_INFO, "g_frame_quancun_rs.PassKey", "%x",g_frame_quancun_rs.PassKey[i]);
//		}
//	//软件计算MAC
//	uint8 MacData[30] = { 0 };
//	memcpy(&MacData[0], &Addmoney[0], 4);
//	for(i = 0;i < 4; i++){
//			   	   	  __android_log_print(ANDROID_LOG_INFO, "Addmoney", "%x",Addmoney[i]);
//			   	}
//	MacData[4] = 0x02;
//	memcpy(&MacData[5], &g_frame_quancun_init_rs.TerminalNO[0], 6);
//	for(i = 0;i < 6; i++){
//		__android_log_print(ANDROID_LOG_INFO, "MacData TerminalNO", "%x",g_frame_quancun_init_rs.TerminalNO[i]);
//	}
//	memcpy(&MacData[11], &dealtime[0], 7);
//
//	uint8 Macinit[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//
//	FD_SingleMAC(passkey, Macinit, 18, MacData, mac2);
//
//	//组帧并发送圈存指令
//	iccInitFrame(&transfer_rq);
//	icc_loadFrame(&transfer_rq, dealtime, mac2);	//圈存
//	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
//			transfer_rq.apdu);
//	__android_log_print(ANDROID_LOG_INFO, "quancun rq TimeOut", "%x",TimeOut);
//	__android_log_print(ANDROID_LOG_INFO, "quancun rq ret", "%x",ret);
//	for(i = 0;i<(transfer_rq.apdu[0]+1);i++){
//			__android_log_print(ANDROID_LOG_INFO, "quancun rq transfer_rq.apdu", "%x",transfer_rq.apdu[i]);
//		}
//	if (ret != 0)	//串口发送失败
//		return -18 + ret * 100;;
//	ret = TransferChannel_rs(&datalist, data, TimeOut);
//	for(i = 0;i<(data[0]+1);i++){
//		__android_log_print(ANDROID_LOG_INFO, "quancun rs data", "%x",data[i]);
//	}
//
//	if (ret != 0)	//解析失败或者超时
//		return -19 + ret * 100;;
//
//
//	ret = iccCheck(data, 0);
//	if (ret != 0)
//		return -20 + ret * 100;;

	return 0;
}
int getQuanCunInit_Rq_Ble(FrameQuanCunInitRq *data_rq, uint8 *UserName,
		uint8 Addmoney[], int miyaotype, int miyaoFlag, int TimeOut) {
	ST_TRANSFER_CHANNEL transfer_rq;
	__android_log_print(ANDROID_LOG_INFO, "getQuanCunInit_Rq_Ble", "1");
	int datalist;
	uint8 data[128];
	int did, i;
	int ret;
	int j = 0;
	did = 0x01;

	uint8 banlance[4] = { 0x00, 0x00, 0x00, 0x00 };		//(0, 4)余额
	uint8 paySerial[2] = { 0x00, 0x00 };		//(4, 2)联机交易序号
	uint8 passtyperand[4] = { 0x00, 0x00, 0x00, 0x00 };		//(8, 4)伪随机数

	uint8 passkey[8] = { 0x00, 0x00, 0x00, 0x00 };	//过程秘钥

	uint8 dealtime[9] = { 0x20, 0x15, 0x01, 0x01, 0x10, 0x10, 0x10 };//DateTime.Now.ToString("yyyyMMddHHmmss");
	GetTimebufFunction(dealtime);
	dealtime[4] = dealtime[5];
	dealtime[5] = dealtime[6];
	dealtime[6] = dealtime[7];

	uint8 mac1[10] = { 0 };	//(12,4)MAC1码
	uint8 mac2[10] = { 0 };	//MAC2码

	uint8 RecvLen = 0;
	uint8 RecvDate[128];

//	__android_log_print(ANDROID_LOG_INFO, "getQuanCunInit_Rq_Ble", "2");

	memcpy(&data_rq->UserName, &UserName[0], sizeof(data_rq->UserName));
	memcpy(&data_rq->Quancun_Money[0], &Addmoney[0],
			sizeof(data_rq->Quancun_Money));
//	__android_log_print(ANDROID_LOG_INFO, "getQuanCunInit_Rq_Ble", "3");
	//g_frame_quancun_init_rs = g(g_frame_quancun_init_rq);
	ret = send_Quancun_Init_OnLine_To_Front_End_Processor(
			g_frame_quancun_init_rq);
//	__android_log_print(ANDROID_LOG_INFO, "getQuanCunInit_Rq_Ble ret", "%x",
			ret);
//	__android_log_print(ANDROID_LOG_INFO, "getQuanCunInit_Rq_Ble", "4");
	if (ret != SUCCESS) {
		return -1 + ret * 100;
	}
//
//		ret = recv_Quancun_Init_OnLine_From_Front_End_Processor(
//				&g_frame_quancun_init_rs, TimeOut);
//		if (ret != SUCCESS) {
//	#ifdef REPEAT_SEND_UDPNET
//			ret = UdpNet_Repeat_Framework(ret,QUANCUN_INIT_MODE,TimeOut);
//	#endif
//			if (ret != 0) {
//				return -2 + ret * 100;
//			}
//		}
	return 0;
}

int setQuanCunInit_Rs_Ble(FrameQuanCunInitRs *data_rs, uint8 UserName[],
		uint8 Addmoney[], int miyaotype, int miyaoFlag, int TimeOut) {
	ST_TRANSFER_CHANNEL transfer_rq;
//	__android_log_print(ANDROID_LOG_INFO, "getQuanCunInit_Rq_Ble", "1");

	int datalist;
	uint8 data[128];
	int did, i;
	int ret;
	int j = 0;
	did = 0x01;
	TimeOut = 1000;
	uint8 banlance[4] = { 0x00, 0x00, 0x00, 0x00 };		//(0, 4)余额
	uint8 paySerial[2] = { 0x00, 0x00 };		//(4, 2)联机交易序号
	uint8 passtyperand[4] = { 0x00, 0x00, 0x00, 0x00 };		//(8, 4)伪随机数

	uint8 passkey[8] = { 0x00, 0x00, 0x00, 0x00 };	//过程秘钥

	uint8 dealtime[9] = { 0x20, 0x15, 0x01, 0x01, 0x10, 0x10, 0x10 };//DateTime.Now.ToString("yyyyMMddHHmmss");
	GetTimebufFunction(dealtime);
	dealtime[4] = dealtime[5];
	dealtime[5] = dealtime[6];
	dealtime[6] = dealtime[7];

	uint8 mac1[10] = { 0 };	//(12,4)MAC1码
	uint8 mac2[10] = { 0 };	//MAC2码

	uint8 RecvLen = 0;
	uint8 RecvDate[128];

//	__android_log_print(ANDROID_LOG_INFO, "dsb", "2");

	iccInitFrame(&transfer_rq);
	icc_getOneDispersed(&transfer_rq);	//获取1级分散因子
	for (i = 0; i < transfer_rq.frame_len + 1; i++) {
//		__android_log_print(ANDROID_LOG_INFO,
				"g_frame_quancun_rq.OneDispersed apud", "%x",
				transfer_rq.apdu[i]);
	}
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != 0)	//串口发送失败
		return -3 + ret * 100;
//	__android_log_print(ANDROID_LOG_INFO, "dsb", "3");
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	if (ret != 0)	//解析失败或者超时
		return -4 + ret * 100;
//	__android_log_print(ANDROID_LOG_INFO, "dsb", "4");
	ret = iccCheckandGetData(data, 0, g_frame_quancun_rq.OneDispersed);

	if (ret != 0)
		return -5 + ret * 100;	//取一级分散因子失败直接退出
	for (i = 0; i < 8; i++) {
//		__android_log_print(ANDROID_LOG_INFO, "g_frame_quancun_rq.OneDispersed",
//				"%x", g_frame_quancun_rq.OneDispersed[i]);
	}
//	__android_log_print(ANDROID_LOG_INFO, "dsb", "5");
	iccInitFrame(&transfer_rq);
	icc_getTwoDispersed(&transfer_rq);	//获取2级分散因子
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != 0)	//串口发送失败
		return -6 + ret * 100;
//	__android_log_print(ANDROID_LOG_INFO, "dsb", "6");
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	if (ret != 0)	//解析失败或者超时
		return -7 + ret * 100;
//	__android_log_print(ANDROID_LOG_INFO, "dsb", "7");
	ret = iccCheckandGetData(data, 0, g_frame_quancun_rq.TwoDispersed);

	if (ret != 0)
		return -8 + ret * 100;	//取二级分散因子失败直接退出
	for (i = 0; i < 8; i++) {
//		__android_log_print(ANDROID_LOG_INFO, "g_frame_quancun_rq.TwoDispersed",
//				"%x", g_frame_quancun_rq.TwoDispersed[i]);
	}
//	__android_log_print(ANDROID_LOG_INFO, "dsb", "8");
	uint8 datatmp[3] = { 0x12, 0x34, 0x56 };
	iccInitFrame(&transfer_rq);
	icc_enter_dir(&transfer_rq, 0x1001);	//选择1001
	icc_check_Pin(&transfer_rq, 3, datatmp);	//验证PIN

	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != 0)	//串口发送失败
		return -9 + ret * 100;
//	__android_log_print(ANDROID_LOG_INFO, "dsb", "9");
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	if (ret != 0)	//解析失败或者超时
		return -10 + ret * 100;
//	__android_log_print(ANDROID_LOG_INFO, "dsb", "10");
	ret = iccCheck(data, 0);
	if (ret != 0)	//接收帧校验失败
		return -11 + ret * 100;
//	__android_log_print(ANDROID_LOG_INFO, "dsb", "11");
	ret = iccCheck(data, 1);
//	__android_log_print(ANDROID_LOG_INFO, "iccCheck data", "check");
//	for(i = 0;i<5;i++){
//		__android_log_print(ANDROID_LOG_INFO, "iccCheck data", "%x",data[i]);
//	}
	if (ret != 0)	//接收帧校验失败
		return -12 + ret * 100;
//	__android_log_print(ANDROID_LOG_INFO, "getQuanCunInit_Rq_Ble", "12");
	//圈存初始化
	iccInitFrame(&transfer_rq);
	memcpy(&g_frame_quancun_init_rs, &data_rs, sizeof(data_rs));
	iccinit_for_loadFrame(&transfer_rq, Addmoney,
			g_frame_quancun_init_rs.TerminalNO, miyaotype, miyaoFlag);
//	__android_log_print(ANDROID_LOG_INFO, "miyaotype", "%x", miyaotype);
//	__android_log_print(ANDROID_LOG_INFO, "miyaoFlag", "%x", miyaoFlag);
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	for (i = 0; i < (transfer_rq.apdu[0] + 1); i++) {
//		__android_log_print(ANDROID_LOG_INFO, "quancuninit rq transfer_rq.apdu",
//				"%x", transfer_rq.apdu[i]);
	}
	if (ret != 0)	//串口发送失败
		return -13 + ret * 100;
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	for (i = 0; i < (data[0] + 1); i++) {
//		__android_log_print(ANDROID_LOG_INFO, "quancuninit rs data", "%x",
//				data[i]);
	}
	if (ret != 0)	//解析失败或者超时
		return -14 + ret * 100;

	ret = iccCheck(data, 0);
	if (ret != 0)
		return -15 + ret * 100;	//接收数据校验失败直接退出

	memcpy(RecvDate, &data[1], data[0]);

	for (i = 0; i < data[0]; i++) {
//		__android_log_print(ANDROID_LOG_INFO, "data", "%x", data[i]);
	}
	for (i = 0; i < 4; i++) {	//(0, 8)余额
		banlance[i] = RecvDate[i];
	}
	for (i = 0; i < 2; i++) {	//(8, 4)联机交易序号
		paySerial[i] = RecvDate[4 + i];
		g_frame_quancun_rq.QuancunTradeNo[i] = RecvDate[4 + i];
//		__android_log_print(ANDROID_LOG_INFO,
//				"g_frame_quancun_rq.QuancunTradeNo", "%x",
//				g_frame_quancun_rq.QuancunTradeNo[i]);
	}
	for (i = 0; i < 4; i++) {	//16, 8)伪随机数
		passtyperand[i] = RecvDate[8 + i];
		g_frame_quancun_rq.BogusRandNumber[i] = RecvDate[8 + i];
//		__android_log_print(ANDROID_LOG_INFO, "BogusRandNumber", "%x",
//				g_frame_quancun_rq.BogusRandNumber[i]);
	}
	for (i = 0; i < 4; i++) {	//(24, 8)MAC1码
		mac1[i] = RecvDate[12 + i];
	}

//			//软件验证MAC1模块
//			//(暂无)
//			//过程密钥
//			ret = send_Quancun_OnLine_To_Front_End_Processor(g_frame_quancun_rq);
//			if (ret != SUCCESS) {
//				return -16 + ret * 100;
//			}
//			ret = recv_Quancun_OnLine_From_Front_End_Processor(&g_frame_quancun_rs,
//					TimeOut);
//			if (ret != SUCCESS) {
//		#ifdef REPEAT_SEND_UDPNET
//				ret = UdpNet_Repeat_Framework(ret,QUANCUN_MODE,TimeOut);
//		#endif
//				if (ret != 0) {
//					return -17 + ret * 100;
//				}
//			}

	return 0;
}

/*
 *发起读车辆信息请求，获取读车辆信息随机数
 */
int getFrameReadVehInfo_Rq_Ble(PROG_COMM_C4 prog_c4, int time_out) {
	int i, ret, did, datalist, sys_flag = 0, veh_flag = 0;
	int keyid_for_encrypt_op, keyid_for_encrypt, psam_slot = g_psam_slot;
	//如下变量是从getsecure_rs中得到
	int gs_veh_len;
	char gs_veh_file[128];
	char gs_authenticator[8];
	//硬sam解密之后的车辆信息和mac码
	uint8 data[128];
	ST_TRANSFER_CHANNEL transfer_rq;
	g_read_file.NumOfFiles = prog_c4.NumOfFiles;
	for (i = 0; i < prog_c4.NumOfFiles; i++) {
		g_read_file.DIDnFID[i] = prog_c4.DIDnFID[i];
		g_read_file.offset[i] = prog_c4.Offset[i];
		g_read_file.len[i] = prog_c4.Length[i];
		//判断是否需要读取车辆信息，
		if (prog_c4.DIDnFID[i] == 0x11) {
			veh_flag = 1;
		}
	}
//	__android_log_print(ANDROID_LOG_INFO, "veh_flag", "%x", prog_c4.DIDnFID[i]);
//	__android_log_print(ANDROID_LOG_INFO, "veh_flag", "%x", veh_flag);
//	__android_log_print(ANDROID_LOG_INFO, "prog_c4.C4Flag", "%x",
//			prog_c4.C4Flag);
	//读取车辆信息
	if (veh_flag == 1) {
		if (prog_c4.C4Flag == 0) {
			ret = send_readveh_OnLine_To_Front_End_Processor(
					g_frame_readveh_rq);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine send_readveh_OnLine_To_Front_End_Processor ERR, ret = %d\n", ret);
				return -1001;
			}
//			ret = recv_readveh_OnLine_From_Front_End_Processor(
//					&g_frame_readveh_rs, time_out);

		}/* end if(prog_c4.C4Flag ==0) */
	}/* end if(veh_flag == 1) */
	return SUCCESS;
}

/*
 *发起读车辆信息请求，获取读车辆信息随机数
 */
int setFrameReadVehInfo_Rs_Ble(PROG_COMM_C4 prog_c4, int time_out) {

	int i, ret, did, datalist, sys_flag = 0, veh_flag = 0;
	int keyid_for_encrypt_op, keyid_for_encrypt, psam_slot = g_psam_slot;
	//如下变量是从getsecure_rs中得到
	int gs_veh_len;
	char gs_veh_file[128];
	char gs_authenticator[8];
	//硬sam解密之后的车辆信息和mac码
	uint8 data[128];
	ST_TRANSFER_CHANNEL transfer_rq;
	g_read_file.NumOfFiles = prog_c4.NumOfFiles;
	for (i = 0; i < prog_c4.NumOfFiles; i++) {
		g_read_file.DIDnFID[i] = prog_c4.DIDnFID[i];
		g_read_file.offset[i] = prog_c4.Offset[i];
		g_read_file.len[i] = prog_c4.Length[i];
		//判断是否需要读取车辆信息，
		if (prog_c4.DIDnFID[i] == 0x11) {
			veh_flag = 1;
		}
	}
//	__android_log_print(ANDROID_LOG_INFO, "veh_flag", "%x", prog_c4.DIDnFID[i]);
//	__android_log_print(ANDROID_LOG_INFO, "veh_flag", "%x", veh_flag);
//	__android_log_print(ANDROID_LOG_INFO, "prog_c4.C4Flag", "%x",
//			prog_c4.C4Flag);
	//读取车辆信息
	if (veh_flag == 1) {
		if (prog_c4.C4Flag == 0) {
			for (i = 0; i < 8; i++) {
				psam_random[i] = g_frame_readveh_rs.TradeRandomNumber[i];
				//为发送c9帧做准备  给c9帧中交易随机数赋值  add by wll 2013-3-26 9:40:29
				g_frame_writesys_rq.TradeRandomNumber[i] = psam_random[i];
				g_frame_syscheck_rq.TradeRandomNumber[i] = psam_random[i];
			}
			//************************End of Change ***********************************************/

			//进DF01目录 （为后面取车辆信息做准备）
			did = 0x01;
			esamInitFrame(&transfer_rq);
			esamEnterDirFrame(&transfer_rq, 0xdf01);
			ret = TransferChannel_rq(did, transfer_rq.channelid,
					transfer_rq.apdulist, transfer_rq.apdu);
			if (ret != SUCCESS) {
				return -1003;	//串口发送失败
			}
			ret = TransferChannel_rs(&datalist, data, time_out);
			if (ret != SUCCESS) {
				return -1004;	//解析失败或者超时
			}
			// 读车辆信息
			keyid_for_encrypt_op = 0x80;
			keyid_for_encrypt = 0x00;	//可以指定obu采用哪个加密密钥
			ret = GetSecure_rq(psam_mac, 0x00, 59, psam_random,
					keyid_for_encrypt_op, keyid_for_encrypt);	//原来是59
			if (ret != SUCCESS) {
				return -1005;
			}
			ret = GetSecure_rs(&gs_veh_len, gs_veh_file, gs_authenticator,
					time_out);
			if (ret != SUCCESS) {
				return -1006;
			}
			//为发送c9帧做准备  给c9帧中车辆信息帧密文赋值 add by wll 2013-3-26 9:40:29
			for (i = 0; i < 72; i++) {
				g_frame_writesys_rq.EncryptedVehInfo[i] = gs_veh_file[i];
			}
		}/* end if(prog_c4.C4Flag ==0) */
		else {
			did = 0x03;
			esamInitFrame(&transfer_rq);
			esamEnterDirFrame(&transfer_rq, 0xdf03);
			esamReadFileInfoFrame(&transfer_rq, prog_c4.Offset[1],
					prog_c4.Length[1]);
			ret = TransferChannel_rq(did, transfer_rq.channelid,
					transfer_rq.apdulist, transfer_rq.apdu);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine TransferChannel_rq ERR, ret = %d\n", ret);
				return -1007;	//串口发送失败
			}
			ret = TransferChannel_rs(&datalist, data, time_out);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine TransferChannel_rs ERR, ret = %d\n", ret);
				return -1008;	//解析失败或者超时
			}
			ret = esamCheck(data, 0);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine esamCheck ERR, ret = %d\n", ret);
				return -1009;
			}
			ret = esamCheckReadVehInfo(data, 1);
			if (ret != SUCCESS) {
				//DEBUGP("send_c8_OnLine esamCheckReadVehInfo ERR, ret = %d\n", ret);
				return -1010;	//取随机数失败直接退出
			}
		}
	}/* end if(veh_flag == 1) */

	did = 0x01;	//根目录
	esamInitFrame(&transfer_rq);
	esamEnterDirFrame(&transfer_rq, 0x3f00);
	esamGetRandFrame(&transfer_rq);
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != SUCCESS) {
		return -1011;	//串口发送失败
	}
	ret = TransferChannel_rs(&datalist, data, time_out);
	if (ret != SUCCESS) {
		return -1012;	//解析失败或者超时
	}
	ret = esamCheck(data, 0);
	if (ret != SUCCESS) {
		return -1013;
	}
	ret = esamCheckGetRand(data, 1);
	if (ret != SUCCESS) {
		return -1014;	//取随机数失败直接退出
	}

	//为发送向前置机发送写系统信息帧做准备  给帧中  随机数  赋值 add by wll 2013-3-26 9:55:43
	for (i = 0; i < 4; i++) {
		g_frame_writesys_rq.RandomNumber[i] = esam_random[i];
	}
	/********** END 进3F00 目录 ，取随机数 为后面激活做准备 （如果有激活操作的话） *************************/

	if ((vst.obustatus[0] & 0x80) == 0x00) //如果OBU插卡,则读取0002和0015文件
			{
		did = 0x01;	//应用主目录
		iccInitFrame(&transfer_rq);
		iccReadMoneyFrame(&transfer_rq);
		iccReadFileFrame(&transfer_rq, 0x0015, 0x00, 0x2b);
		ret = TransferChannel_rq(did, transfer_rq.channelid,
				transfer_rq.apdulist, transfer_rq.apdu);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine TransferChannel_rq ERR, ret = %d\n", ret);
			return -1015;	//串口发送失败
		}
		ret = TransferChannel_rs(&datalist, data, time_out);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine TransferChannel_rs ERR, ret = %d\n", ret);
			return -1016;	//解析失败或者超时
		}
		ret = iccCheck(data, 0);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine iccCheck ERR, ret = %d\n", ret);
			return -1017;
		}
		ret = iccCheck(data, 1);
		if (ret != SUCCESS) {
			//DEBUGP("send_c8_OnLine iccCheck ERR, ret = %d\n", ret);
			return -1018;
		}
		memcpy(icc_pib.Balance, &data[1], 4);	//0002文件
		memcpy(icc_pib.icc0015, &data[8], 43);	//0015文件
	}

	/*********************写系统信息请求帧指令，由原来的从psam计算mac转换为由前置机计算mac*********/
	//系统信息标志是0,因为所用分散因子不同
	//向前置机发送		  OBU写系统信息请求指令
//	ret = send_writesys_OnLine_To_Front_End_Processor(g_frame_writesys_rq);
//	if (ret != SUCCESS) {
//		return -1019;
//	}
	return 1;
}

int getWriteSysInfo_Rq_Ble(FrameWriteSysInfoRq data_rq) {
	return send_writesys_OnLine_To_Front_End_Processor(g_frame_writesys_rq);

}

int setWriteSysInfo_Rs_Ble(FrameWriteSysInfoRq data_rq, int time_out) {
	ST_TRANSFER_CHANNEL transfer_rq;
	int datalist;
	uint8 data[128];
	int did;
	int ret;
	int i;
	//指令的18个字节中后4个字节就是MAC地址   remark by wll 2013-3-27 16:38:15
	for (i = 0; i < 4; i++) {
		//esam_mac 是代表从前置机获取的（前置机通过终端发的obu随机数计算出的）mac
		esam_mac[i] = g_frame_writesys_rs.WriteSysInfoCommand[14 + i];
	}
//	__android_log_print(ANDROID_LOG_INFO, "setWriteSysInfo_Rs_Ble", "1");
	//add by wll 2014-12-19 14:36:55 写系统信息
	esamInitFrame(&transfer_rq);
	transfer_rq.apdulist++;
	transfer_rq.apdu[0] = 18;
	memcpy(&transfer_rq.apdu[1], &g_frame_writesys_rs.WriteSysInfoCommand[0],
			18);
	transfer_rq.frame_len = 18;
	ret = TransferChannel_rq(g_ndid, transfer_rq.channelid,
			transfer_rq.apdulist, transfer_rq.apdu);
	if (ret != SUCCESS) {
		__android_log_print(ANDROID_LOG_INFO, "setWriteSysInfo_Rs_Ble", "2");
		return -1021;	//串口发送失败
	}
	__android_log_print(ANDROID_LOG_INFO, "setWriteSysInfo_Rs_Ble", "3");
	ret = TransferChannel_rs(&datalist, data, time_out);
	if (ret != SUCCESS) {
		return -1022;	//解析失败或者超时
		__android_log_print(ANDROID_LOG_INFO, "setWriteSysInfo_Rs_Ble", "4");
	}
	__android_log_print(ANDROID_LOG_INFO, "setWriteSysInfo_Rs_Ble", "5");
	int j = 0;
	for (j = 0; j < 30; j++)
		__android_log_print(ANDROID_LOG_INFO, "send_c7", "%x", data[j]);
	ret = esamCheck(data, 0);
	if (ret != SUCCESS) {
		__android_log_print(ANDROID_LOG_INFO, "setWriteSysInfo_Rs_Ble", "6");
		return -1023;
	}
	__android_log_print(ANDROID_LOG_INFO, "setWriteSysInfo_Rs_Ble", "7");
	return SUCCESS;
}

int jni2loadCreditGetMac1(uint8* cardId, int credit, uint8* terminalNo,
		uint8* pinCode, int procType, int keyIndex,
		loadCreditGetMac1Ret* data_ret, int TimeOut) {

	ST_TRANSFER_CHANNEL transfer_rq;
	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "jni2loadCreditGetMac1");
	int datalist;
	uint8 data[128];
	int did, i;
	int ret;
	int j = 0;
	did = 0x01;
	TimeOut = 1000;
	uint8 addmoney[4] = { 0x00, 0x00, 0x00, 0x00 };
	uint8 banlance[4] = { 0x00, 0x00, 0x00, 0x00 };		//(0, 4)余额
	uint8 paySerial[2] = { 0x00, 0x00 };		//(4, 2)联机交易序号
	uint8 passtyperand[4] = { 0x00, 0x00, 0x00, 0x00 };		//(8, 4)伪随机数

	uint8 passkey[8] = { 0x00, 0x00, 0x00, 0x00 };	//过程秘钥

	uint8 dealtime[9] = { 0x20, 0x15, 0x01, 0x01, 0x10, 0x10, 0x10 };//DateTime.Now.ToString("yyyyMMddHHmmss");

	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1", "1");

	GetTimebufFunction(dealtime);
	dealtime[4] = dealtime[5];
	dealtime[5] = dealtime[6];
	dealtime[6] = dealtime[7];

	uint8 mac1[10] = { 0 };	//(12,4)MAC1码
	uint8 mac2[10] = { 0 };	//MAC2码

	uint8 RecvLen = 0;
	uint8 RecvDate[128];

	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"iccInitFrame");
	iccInitFrame(&transfer_rq);

	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"icc_getOneDispersed");
	icc_getOneDispersed(&transfer_rq);	//获取1级分散因子
	for (i = 0; i < transfer_rq.frame_len + 1; i++) {
		__android_log_print(ANDROID_LOG_INFO,
				"g_frame_quancun_rq.OneDispersed apud", "%x",
				transfer_rq.apdu[i]);
	}

	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"TransferChannel_rq");
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != 0)	//串口发送失败
		return -3 + ret * 100;

	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"TransferChannel_rs");
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	if (ret != 0)	//解析失败或者超时
		return -4 + ret * 100;

	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"iccCheckandGetData");
	ret = iccCheckandGetData(data, 0, g_frame_quancun_rq.OneDispersed);

	if (ret != 0)
		return -5 + ret * 100;	//取一级分散因子失败直接退出
	for (i = 0; i < 8; i++) {
		__android_log_print(ANDROID_LOG_INFO, "g_frame_quancun_rq.OneDispersed",
				"%x", g_frame_quancun_rq.OneDispersed[i]);
	}

	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"iccInitFrame");
	iccInitFrame(&transfer_rq);
	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"icc_getTwoDispersed");
	icc_getTwoDispersed(&transfer_rq);	//获取2级分散因子
	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"TransferChannel_rq");
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != 0)	//串口发送失败
		return -6 + ret * 100;

	ret = TransferChannel_rs(&datalist, data, TimeOut);
	if (ret != 0)	//解析失败或者超时
		return -7 + ret * 100;

	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"iccCheckandGetData");
	ret = iccCheckandGetData(data, 0, g_frame_quancun_rq.TwoDispersed);

	if (ret != 0)
		return -8 + ret * 100;	//取二级分散因子失败直接退出
	for (i = 0; i < 8; i++) {
		__android_log_print(ANDROID_LOG_INFO, "g_frame_quancun_rq.TwoDispersed",
				"%x", g_frame_quancun_rq.TwoDispersed[i]);
	}

	//uint8 datatmp[3] = { 0x12, 0x34, 0x56 };修改pin yjun0902
	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"icc_check_Pin");
	iccInitFrame(&transfer_rq);
	icc_enter_dir(&transfer_rq, 0x1001);	//选择1001
	icc_check_Pin(&transfer_rq, 3, pinCode);	//验证PIN
	for (i = 0; i < 3; i++) {
		__android_log_print(ANDROID_LOG_INFO, "pinCode", "%x", pinCode[i]);
	}

	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	if (ret != 0)	//串口发送失败
		return -9 + ret * 100;

	ret = TransferChannel_rs(&datalist, data, TimeOut);
	if (ret != 0)	//解析失败或者超时
		return -10 + ret * 100;

	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1", "iccCheck");
	ret = iccCheck(data, 0);
	if (ret != 0)	//接收帧校验失败
		return -11 + ret * 100;

	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1", "iccCheck");
	ret = iccCheck(data, 1);
	__android_log_print(ANDROID_LOG_INFO, "iccCheck data", "check");
	//	for(i = 0;i<5;i++){
	//		__android_log_print(ANDROID_LOG_INFO, "iccCheck data", "%x",data[i]);
	//	}
	if (ret != 0)	//接收帧校验失败
		return -12 + ret * 100;

	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1", "圈存初始化");

	__android_log_print(ANDROID_LOG_INFO, "credit", "%x", credit);
	__android_log_print(ANDROID_LOG_INFO, "procType", "%x", procType);
	__android_log_print(ANDROID_LOG_INFO, "keyIndex", "%x", keyIndex);
	for (i = 0; i < 6; i++) {
		__android_log_print(ANDROID_LOG_INFO, "terminalNo", "%x",
				terminalNo[i]);
		g_frame_quancun_init_rs.TerminalNO[i] = terminalNo[i];
	}
	//圈存初始化
	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"iccInitFrame");
	iccInitFrame(&transfer_rq);
	//memcpy(&g_frame_quancun_init_rs, &data_rs, sizeof(data_rs));
	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"iccinit_for_loadFrame");

	intToBytes2(credit, addmoney);
	for (i = 0; i < 4; i++) {
		__android_log_print(ANDROID_LOG_INFO, "addmoney", "%x", addmoney[i]);
	}
	iccinit_for_loadFrame(&transfer_rq, addmoney, terminalNo, procType,
			keyIndex);
	__android_log_print(ANDROID_LOG_INFO, "jni2loadCreditGetMac1",
			"TransferChannel_rq");
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	for (i = 0; i < (transfer_rq.apdu[0] + 1); i++) {
		__android_log_print(ANDROID_LOG_INFO, "quancuninit rq transfer_rq.apdu",
				"%x", transfer_rq.apdu[i]);
	}
	if (ret != 0)	//串口发送失败
		return -13 + ret * 100;
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	for (i = 0; i < (data[0] + 1); i++) {
		__android_log_print(ANDROID_LOG_INFO, "quancuninit rs data", "%x",
				data[i]);
	}
	if (ret != 0)	//解析失败或者超时
		return -14 + ret * 100;

	ret = iccCheck(data, 0);
	if (ret != 0)
		return -15 + ret * 100;	//接收数据校验失败直接退出

	memcpy(RecvDate, &data[1], data[0]);

	for (i = 0; i < data[0]; i++) {
		__android_log_print(ANDROID_LOG_INFO, "data", "%x", data[i]);
	}
	for (i = 0; i < 4; i++) {	//(0, 8)余额
		banlance[i] = RecvDate[i];
		data_ret->a_cbb[i] = RecvDate[i];
	}
	for (i = 0; i < 2; i++) {	//(8, 4)联机交易序号
		paySerial[i] = RecvDate[4 + i];
		data_ret->a_on[i] = RecvDate[4 + i];
		g_frame_quancun_rq.QuancunTradeNo[i] = RecvDate[4 + i];

		__android_log_print(ANDROID_LOG_INFO,
				"g_frame_quancun_rq.QuancunTradeNo", "%x",
				g_frame_quancun_rq.QuancunTradeNo[i]);
	}
	for (i = 0; i < 4; i++) {	//16, 8)伪随机数
		passtyperand[i] = RecvDate[8 + i];
		data_ret->a_rnd[i] = RecvDate[8 + i];
		g_frame_quancun_rq.BogusRandNumber[i] = RecvDate[8 + i];
		__android_log_print(ANDROID_LOG_INFO, "BogusRandNumber", "%x",
				data_ret->a_rnd[i]);
	}
	for (i = 0; i < 4; i++) {	//(24, 8)MAC1码
		data_ret->a_m1[i] = RecvDate[12 + i];
		mac1[i] = RecvDate[12 + i];
		__android_log_print(ANDROID_LOG_INFO, "data_ret->a_m1", "%x",
				data_ret->a_m1[i]);
	}

	for (i = 0; i < 4; i++) {	//16, 8)伪随机数
		passtyperand[i] = RecvDate[8 + i];
		data_ret->a_rnd[i] = RecvDate[8 + i];
		g_frame_quancun_rq.BogusRandNumber[i] = RecvDate[8 + i];
		__android_log_print(ANDROID_LOG_INFO, "BogusRandNumber1", "%x",
				data_ret->a_rnd[i]);
	}

	//			//软件验证MAC1模块
	//			//(暂无)
	//			//过程密钥
	//			ret = send_Quancun_OnLine_To_Front_End_Processor(g_frame_quancun_rq);
	//			if (ret != SUCCESS) {
	//				return -16 + ret * 100;
	//			}
	//			ret = recv_Quancun_OnLine_From_Front_End_Processor(&g_frame_quancun_rs,
	//					TimeOut);
	//			if (ret != SUCCESS) {
	//		#ifdef REPEAT_SEND_UDPNET
	//				ret = UdpNet_Repeat_Framework(ret,QUANCUN_MODE,TimeOut);
	//		#endif
	//				if (ret != 0) {
	//					return -17 + ret * 100;
	//				}
	//			}

	return 0;
}

int jniloadCreditWriteCard(uint8* mac2, int TimeOut) {

	//int ret = 0;

	ST_TRANSFER_CHANNEL transfer_rq;
	int datalist;
	uint8 data[128];
	int did, i;
	int ret;
	did = 0x01;
	TimeOut = 1000;
	__android_log_print(ANDROID_LOG_INFO,"wjjar.log", "jniloadCreditWriteCard");
//   	uint8 banlance[4] = { 0x00, 0x00, 0x00, 0x00 };	//(0, 4)余额
//   	uint8 paySerial[2] = { 0x00, 0x00 };	//(4, 2)联机交易序号
//   	uint8 passtyperand[4] = { 0x00, 0x00, 0x00, 0x00 };	//(8, 4)伪随机数
//
//   	uint8 passkey[8] = { 0x00, 0x00, 0x00, 0x00 };	//过程秘钥
	uint8 l_dealtime[7];
	uint8 l_mac2[10];
	memcpy(l_dealtime, mac2, 7);
	for (i = 0; i < 7; i++) {
		__android_log_print(ANDROID_LOG_INFO, "l_dealtime", "%x",
				l_dealtime[i]);
	}

	memcpy(l_mac2, &mac2[7], 4);
	for (i = 0; i < 7; i++) {
		__android_log_print(ANDROID_LOG_INFO, "l_mac2", "%x", l_mac2[i]);
	}
#ifdef	WJOBUTEST
	__android_log_print(ANDROID_LOG_INFO, "jniloadCreditWriteCard",
			"WJOBUTEST");
	uint8 passkey[8] = { 0x00, 0x00, 0x00, 0x00 };	//过程秘钥
	uint8 dealtime[9] = { 0x20, 0x15, 0x01, 0x01, 0x10, 0x10, 0x10 };//DateTime.Now.ToString("yyyyMMddHHmmss");
	GetTimebufFunction(dealtime);
	dealtime[4] = dealtime[5];
	dealtime[5] = dealtime[6];
	dealtime[6] = dealtime[7];
	for (i = 0; i < 7; i++) {
		__android_log_print(ANDROID_LOG_INFO, "calling l_dealtime", "%x",
				dealtime[i]);
	}
	memcpy(l_dealtime, dealtime, 7);

	memcpy(passkey, g_frame_quancun_rs.PassKey, 8);
	for (i = 0; i < 8; i++) {
		__android_log_print(ANDROID_LOG_INFO, "g_frame_quancun_rs.PassKey",
				"%x", g_frame_quancun_rs.PassKey[i]);
	}
	//软件计算MAC
	uint8 MacData[30] = { 0 };
	memcpy(&MacData[0], &g_sret.a_pt[0], 4);
	for (i = 0; i < 4; i++) {
		__android_log_print(ANDROID_LOG_INFO, "g_sret.a_pt", "%x",
				g_sret.a_pt[i]);
	}
	MacData[4] = 0x02;
	memcpy(&MacData[5], &g_frame_quancun_init_rs.TerminalNO[0], 6);
	for (i = 0; i < 6; i++) {
		__android_log_print(ANDROID_LOG_INFO, "MacData TerminalNO", "%x",
				g_frame_quancun_init_rs.TerminalNO[i]);
	}
	memcpy(&MacData[11], &l_dealtime[0], 7);

	uint8 Macinit[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	for (i = 0; i < 7; i++) {
		__android_log_print(ANDROID_LOG_INFO, "called l_dealtime", "%x",
				l_dealtime[i]);
	}
	FD_SingleMAC(passkey, Macinit, 18, MacData, l_mac2);
	for (i = 0; i < 7; i++) {
		__android_log_print(ANDROID_LOG_INFO, "called1 l_dealtime", "%x",
				l_dealtime[i]);
	}
#endif

	uint8 RecvLen = 0;
	uint8 RecvDate[128];
	for (i = 0; i < 7; i++) {
		__android_log_print(ANDROID_LOG_INFO, "cal l_dealtime", "%x",
				l_dealtime[i]);
	}
	for (i = 0; i < 4; i++) {
		__android_log_print(ANDROID_LOG_INFO, "cal l_mac2", "%x", l_mac2[i]);
	}

	//组帧并发送圈存指令
	iccInitFrame(&transfer_rq);
	icc_loadFrame(&transfer_rq, l_dealtime, l_mac2);	//圈存
	ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
			transfer_rq.apdu);
	__android_log_print(ANDROID_LOG_INFO, "quancun rq TimeOut", "%x", TimeOut);
	__android_log_print(ANDROID_LOG_INFO, "quancun rq ret", "%x", ret);
	for (i = 0; i < (transfer_rq.apdu[0] + 1); i++) {
		__android_log_print(ANDROID_LOG_INFO, "quancun rq transfer_rq.apdu",
				"%x", transfer_rq.apdu[i]);
	}
	if (ret != 0)	//串口发送失败
		return -18 + ret * 100;;
	ret = TransferChannel_rs(&datalist, data, TimeOut);
	for (i = 0; i < (data[0] + 1); i++) {
		__android_log_print(ANDROID_LOG_INFO, "quancun rs data", "%x", data[i]);
	}

	if (ret != 0)	//解析失败或者超时
		return -19 + ret * 100;;

	ret = iccCheck(data, 0);
	if (ret != 0)
		return -20 + ret * 100;;

	return 0;
	return ret;

}
