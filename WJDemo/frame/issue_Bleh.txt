#ifndef _ISSUE_BLE_H
#define _ISSUE_BLE_H
#include "lib2hd.h"



//*****************************蓝牙OBU程序发行操作接口********************//add by wll 2013-3-25 9:22:54
//RSU初始化
extern int send_c0_Ble(PROG_COMM_C0 prog_c0);
extern int recv_b0_Ble(PROG_COMM_B0 *prog_b0, int time_out);
//BST VST
extern int send_c1_Ble(PROG_COMM_C1 prog_c1);
extern int recv_b1_Ble(PROG_COMM_B1 *prog_b1, int time_out);
//密钥替换
extern int send_c2_Ble(PROG_COMM_C2 prog_c2, int time_out);
extern int recv_b2_Ble(PROG_COMM_B2 *prog_b2);
//文件更新
extern int send_c3_Ble(PROG_COMM_C3 prog_c3, int time_out);
extern int send_c5_Ble(PROG_COMM_C5 prog_c5);
extern int recv_b4_Ble(PROG_COMM_B4 *prog_b4, int time_out);

extern int send_c5_Ble_new(PROG_COMM_C5 prog_c5, uint8 *IdentInfo);
extern int recv_b4_Ble_new(PROG_COMM_B4 *prog_b4, int time_out);

extern int send_c4_Ble(PROG_COMM_C4 prog_c4, int time_out);	//读取文件
extern int recv_b3_Ble(PROG_COMM_B3 *prog_b3);				//读取文件的返回
//读取OBU SN号
extern int send_c6_Ble(int TimeOut);
extern int recv_b6_Ble(uint8 *EsamSerialNum);

//写系统信息的请求与应答（终端与obu）
extern int send_c7_Ble(PROG_COMM_C4 prog_c4, int time_out);
extern int recv_b7_Ble(PROG_COMM_B3 *prog_b3);

//读车辆信息的请求与应答（终端与obu）
extern int send_c8_Ble(PROG_COMM_C4 prog_c4, int time_out);
extern int recv_b8_Ble(PROG_COMM_B3 *prog_b3);

//读系统信息的请求与应答（终端与obu）
extern int send_c9_Ble(PROG_COMM_C4 prog_c4, int time_out);
extern int recv_b9_Ble(PROG_COMM_B3 *prog_b3);

extern int send_c10_Ble(uint8 *pTradeInfo, int record_num, int TimeOut);

extern int send_c11_Ble(uint8 UserName[], uint8 Addmoney[], int miyaotype, int miyaoFlag, int TimeOut);
extern int getQuanCunInit_Rq_Ble(FrameQuanCunInitRq *data_rq,uint8 *UserName,uint8 Addmoney[], int miyaotype,
			int miyaoFlag, int TimeOut);
//充值指令
extern int send_c12_Ble(uint8 BankICcardNum[], uint8 money[], uint8 UserName[]);
extern int recv_b12_Ble(uint8 *pcurBalance, int TimeOut);
extern int g_ndid;
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
extern int send_c13_Ble(uint8 UserName[], int UserNameLen, uint8 UserPassword[], int UserPasswordLen, int TimeOut);

//验证图片信息按照图片文件名称传输给后台服务器
extern int send_c14_Ble(uint8 ChkPhotoName[], int ChkPhotoNameLen, uint8 ChkInfo[], int ChkInfoLen, int TimeOut);

//验证信息按照帧序号传输给后台服务器
extern int send_c15_Ble(uint8 PhotoDataBuf[], int PhotoDataBufLen, int nIndex);
extern int recv_b15_Ble(int TimeOut);

extern int Enter_EsamDir(int dir, int time_out);


extern int jniloadCreditWriteCard(uint8* mac2,int TimeOut);

#endif

