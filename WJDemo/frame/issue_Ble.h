#ifndef _ISSUE_BLE_H
#define _ISSUE_BLE_H
#include "lib2hd.h"



//*****************************����OBU�����в����ӿ�********************//add by wll 2013-3-25 9:22:54
//RSU��ʼ��
extern int send_c0_Ble(PROG_COMM_C0 prog_c0);
extern int recv_b0_Ble(PROG_COMM_B0 *prog_b0, int time_out);
//BST VST
extern int send_c1_Ble(PROG_COMM_C1 prog_c1);
extern int recv_b1_Ble(PROG_COMM_B1 *prog_b1, int time_out);
//��Կ�滻
extern int send_c2_Ble(PROG_COMM_C2 prog_c2, int time_out);
extern int recv_b2_Ble(PROG_COMM_B2 *prog_b2);
//�ļ�����
extern int send_c3_Ble(PROG_COMM_C3 prog_c3, int time_out);
extern int send_c5_Ble(PROG_COMM_C5 prog_c5);
extern int recv_b4_Ble(PROG_COMM_B4 *prog_b4, int time_out);

extern int send_c5_Ble_new(PROG_COMM_C5 prog_c5, uint8 *IdentInfo);
extern int recv_b4_Ble_new(PROG_COMM_B4 *prog_b4, int time_out);

extern int send_c4_Ble(PROG_COMM_C4 prog_c4, int time_out);	//��ȡ�ļ�
extern int recv_b3_Ble(PROG_COMM_B3 *prog_b3);				//��ȡ�ļ��ķ���
//��ȡOBU SN��
extern int send_c6_Ble(int TimeOut);
extern int recv_b6_Ble(uint8 *EsamSerialNum);

//дϵͳ��Ϣ��������Ӧ���ն���obu��
extern int send_c7_Ble(PROG_COMM_C4 prog_c4, int time_out);
extern int recv_b7_Ble(PROG_COMM_B3 *prog_b3);

//��������Ϣ��������Ӧ���ն���obu��
extern int send_c8_Ble(PROG_COMM_C4 prog_c4, int time_out);
extern int recv_b8_Ble(PROG_COMM_B3 *prog_b3);

//��ϵͳ��Ϣ��������Ӧ���ն���obu��
extern int send_c9_Ble(PROG_COMM_C4 prog_c4, int time_out);
extern int recv_b9_Ble(PROG_COMM_B3 *prog_b3);

extern int send_c10_Ble(uint8 *pTradeInfo, int record_num, int TimeOut);

extern int send_c11_Ble(uint8 UserName[], uint8 Addmoney[], int miyaotype, int miyaoFlag, int TimeOut);
extern int getQuanCunInit_Rq_Ble(FrameQuanCunInitRq *data_rq,uint8 *UserName,uint8 Addmoney[], int miyaotype,
			int miyaoFlag, int TimeOut);
//��ֵָ��
extern int send_c12_Ble(uint8 BankICcardNum[], uint8 money[], uint8 UserName[]);
extern int recv_b12_Ble(uint8 *pcurBalance, int TimeOut);
extern int g_ndid;
/*
* �������ƣ�	send_c13_Ble()
* ����������	��̨�û���½
* ���������	uint8 UserName[],�û���
* 			int UserNameLen,�û�������
* 			uint8 UserPassword[],�û�����
* 			int UserPassword,�û����볤��
* 			int TimeOut ��ʱʱ��
* ���������	��
* ����ֵ��	0--�ɹ�����0--ʧ��
*/
extern int send_c13_Ble(uint8 UserName[], int UserNameLen, uint8 UserPassword[], int UserPasswordLen, int TimeOut);

//��֤ͼƬ��Ϣ����ͼƬ�ļ����ƴ������̨������
extern int send_c14_Ble(uint8 ChkPhotoName[], int ChkPhotoNameLen, uint8 ChkInfo[], int ChkInfoLen, int TimeOut);

//��֤��Ϣ����֡��Ŵ������̨������
extern int send_c15_Ble(uint8 PhotoDataBuf[], int PhotoDataBufLen, int nIndex);
extern int recv_b15_Ble(int TimeOut);

extern int Enter_EsamDir(int dir, int time_out);


extern int jniloadCreditWriteCard(uint8* mac2,int TimeOut);

#endif

