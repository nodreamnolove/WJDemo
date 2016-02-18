#import "CToOcType.h"
#include "lib2rsu.h"
#include "common.h"
#include "lib2hd.h"
#include "psam.h"


int len_vehicleinfo;

int IsVst(uint8 *pkg);
/* 
BST 
���������� RSU�㲥BST��OBU 
��������� bst_type--BST����
		 beacon_id--��1�ֽ�manufacturerID��3�ֽ�individualID��� 
		 unix_time--UNIXʱ�䣬����Ϊ4�ֽڣ���2000��1��1��00:00�����������ۼ� 
		 profile--����ѡ�����Ϊ1�ֽڣ�����ֵ���£� 00H--����0(A��)���ŵ�1	01H--����0(A��)���ŵ�2 
											    10H--����1(B��)���ŵ�1	11H--����1(B��)���ŵ�2 
		 obu_init_mode--OBU���в�����ģʽ
��������� �� 
����ֵ�� 0--�ɹ�, ��0--ʧ��  
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
	sbuf[slen++] = g_rsctl; // ֡���  
	sbuf[slen++] = 0xf0; // �����,bst�Լ�actionԭ�� 
	sbuf[slen++] = 0x00; //bst
	len_pos = slen++;    //�����ܳ���λ�� 

	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0x50;
	sbuf[slen++] = 0x03;
	sbuf[slen++] = 0x91; //����ͷ 
	sbuf[slen++] = 0xc0; //bst��ʶ 
	sbuf[slen++] = beacon_id[0]; 
	sbuf[slen++] = beacon_id[1]; 
	sbuf[slen++] = beacon_id[2]; 
	sbuf[slen++] = beacon_id[3]; 
	sbuf[slen++] = unix_time[0]; 
	sbuf[slen++] = unix_time[1]; 
	sbuf[slen++] = unix_time[2]; 
	sbuf[slen++] = unix_time[3]; 
	sbuf[slen++] = profile; 
	sbuf[slen++] = 0x01;	//MandApplications ����չ��1��Ӧ�ã�ȡֵ1
//	sbuf[slen++] = 0x02;	//�������߷���Э��Ҫ��
#if 1//����Э��
	if(bst_type == BST_ISSUE_BJ)
	{
		if(obu_init_mode == 0x2f)
			sbuf[slen++] = 0x41; //paramter����,AID = 0������ʾ����
		else
			sbuf[slen++] = 0x40; //paramter����,AID = 0������ʾ����
		sbuf[slen++] = obu_init_mode; 
		sbuf[slen++] = 0x00; //profilelist,����չ
	}

#endif
#if 1
	if(bst_type == BST_TRADE_GB)
	{
		sbuf[slen++] = 0x41; //paramter����
		/* ��������˵��
		 icctransmode��0x87--��ֵ���ͼ��˿�֧�ִ�ͳ���ѡ��������ѣ����Ȳ��ø������ѽ���ģʽ��֧��OBUԤ������ٽ���ģʽ
		               0xaf--��ֵ���ͼ��˿�����֧�ָ������ѽ���ģʽ�����Ȳ��ø������ѽ���ģʽ��֧��OBUԤ������ٽ���ģʽ
		               0xd1--��ֵ���ͼ��˿�����֧�ִ�ͳ���ѽ���ģʽ�����Ȳ��ô�ͳ���ѽ���ģʽ��֧��OBUԤ������ٽ���ģʽ
		               0xcb--��ֵ��֧�ִ�ͳ���ѽ���ģʽ�����˿�֧�ָ������ѽ���ģʽ�����Ȳ��ø������ѽ���ģʽ��֧��OBUԤ������ٽ���ģʽ
		               0xb5--��ֵ��֧�ָ������ѽ���ģʽ�����˿�֧�ִ�ͳ���ѽ���ģʽ�����Ȳ��ø������ѽ���ģʽ��֧��OBUԤ������ٽ���ģʽ*/
		sbuf[slen++] = 0xaf; 
		sbuf[slen++] = 0x29;
		/*�Ƿ��ȡ0002�ļ�:1
		  �Ƿ��ȡ0012�ļ�:0
		  �Ƿ��ȡ0015�ļ�:1
		  �Ƿ��ȡ0019�ļ�:0
		  fill:0000*/
		sbuf[slen++] = 0xa0;
		sbuf[slen++] = 0x1a;
		sbuf[slen++] = 0x00; //0002�ļ�ƫ����
		sbuf[slen++] = ICC_0002_LEN; //0002�ļ�����
		sbuf[slen++] = 0x00; //0015�ļ�ƫ����
		sbuf[slen++] = ICC_0015_LEN; //0015�ļ�����
	}
	else if(bst_type == BST_ISSUE_NOPARAM)
	{
		sbuf[slen++] = 0x01;//paramter������
	}
	else if(bst_type == BST_ISSUE_NOREAD)
	{
		sbuf[slen++] = 0x41;//paramter����
		sbuf[slen++] = 0x87; 
		sbuf[slen++] = 0x29;
		sbuf[slen++] = 0x00;
		sbuf[slen++] = 0x1a;
	}
	else if(bst_type == BST_ISSUE_PREREAD)
	{
		sbuf[slen++] = 0x41;//paramter����
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
		sbuf[slen++] = 0x51;	//Dsrc-did����ʾ��Parameter���ڣ�AID=17���人·��ETCӦ�ã�
		sbuf[slen++] = 0x01;	//A�ཻ��ģʽ��A�͵�Ƭ��Դ���ӱ�ǩ����ESAM��
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
		is_prwrq_recved = 0;	//��ʼû���յ�
	} else {
		sem_init(&g_sem_vst, 0, 0);
		is_vst_recved = 0;	//��ʼû���յ�
	}
	ret = serial_send();	//���õײ㷢�ͺ���





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
���������� ����OBU���ص�VST 
��������� time_out--��ʱʱ���������20msΪ��λ
��������� obu_mac--OBU��MACID
	     sys_info--OBU��ϵͳ��Ϣ��26�ֽ�
   		 icc_info--Ԥ����IC����Ϣ
		 rnd_info--�����
		 reset_info--��λ��Ϣ
		 obu_status--OBU״̬
����ֵ�� 0--�ɹ�, ��0--ʧ��  
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
//				is_vst_recved = 1;	//�յ����߳�ʱ�˳�����������
//				sem_destroy(&g_sem_vst);
//				return -1;	//��ʱ��û���յ�vst
//			}
//			usleep(20000);
//		}
//	}
//	is_vst_recved = 1;	//�յ����߳�ʱ�˳�����������
//	sem_destroy(&g_sem_vst);
	if(IsVst(g_com_rx_buf) != 0)	//�ж��Ƿ���VST
	{
		return -2;
	}
	pos = 3;	//ȥ��֡��š������0xe0��ԭ������0x00
	vst_len = g_com_rx_buf[pos++];
	//����vst
	if(g_bst_type == BST_TRADE_ZYL)
	{
		for(i = 0;i < 4;i++)
		{
			obu_mac[i] = g_com_rx_buf[pos++];
		}
		mac_control = g_com_rx_buf[pos++];	//mac������0xc0
		pos++;	//llc������0x03
		pos++;	//�ֶ���ͷ0x91
		pos++;	//vst��ʶ0xd0
		pos++;	//�ŵ���
		pos++;	//0xd1,aid������ֽ�
		pos++;	//did,0x03
		for(i = 0;i < 8;i++)
		{
			vst.RandOBE[i] = g_com_rx_buf[pos++];	//8�ֽڵ������
		}
		for(i = 0;i < 12;i++)
		{
			vst.vehicleLicensePlateNumber[i] = g_com_rx_buf[pos++];	//12�ֽڵĳ��ƺ�
		}
		vst.vehicleType = g_com_rx_buf[pos++];	//����
		vst.vehicleUserType = g_com_rx_buf[pos++];	//�����û�����
		for(i = 0; i < 8; i++)
		{
			vst.ContractSerialNumber[i] = g_com_rx_buf[pos++];	//��ͬ���к�
		}
		for(i = 0; i < 4; i++)
		{
			vst.ObuConfigurationMacID[i] = g_com_rx_buf[pos++];	//OBU MACID
		}
		for(i = 0; i < 3; i++)
		{
			vst.obustatus[i] = g_com_rx_buf[pos++];	//OBU״̬��Ϣ
		}
	
	}/* end if(g_bst_type == BST_TRADE_ZYL) */
	else if(g_bst_type == BST_ISSUE_ZYL)
	{
		for(i = 0;i < 4;i++)
		{
			obu_mac[i] = g_com_rx_buf[pos++];
		}
		mac_control = g_com_rx_buf[pos++];	//mac������0xc0
		pos++;	//llc������0x03
		pos++;	//�ֶ���ͷ0x91
		pos++;	//vst��ʶ0xd0
		pos++;	//�ŵ���
		pos++;	//0xd1,aid������ֽ�
		pos++;	//did,0x03
		//8�ֽڵĺ�ͬ���к�
		for(i = 0; i < 8; i++)
		{
			g_contract_sn[i] = g_com_rx_buf[pos++];
		}
		pos += 4;	//4�ֽڵ�obu macid,3�ֽڵ�obustatus
		for(i = 0; i < 3; i++)
		{
			obu_status[i] = g_com_rx_buf[pos++];	//obu״̬��Ϣ
		}
	}/* end else if(g_bst_type == BST_ISSUE_ZYL) */
	else
	{
		for(i = 0; i < 4; i++)
		{
			obu_mac[i] = g_com_rx_buf[pos++];
		}
		mac_control = g_com_rx_buf[pos++]; //mac������0xc0
		pos++;	//llc������0x03
		if(mac_control == 0xe0)
		{
			pos++;	//״̬����
		}
		pos++;	//�ֶ���ͷ0x91
		pos++;	//vst��ʶ0xd0
		pos++;	//�ŵ���
		pos++;	//Ӧ���б���0x01
		dsrc_op = g_com_rx_buf[pos++];	//�Ӹߵ���,�ֱ��ʶdid��ʾ��parameter���ڡ�aid
		if(Getbit(dsrc_op,7))
		{
			pos++;	//did
		}
		if(Getbit(dsrc_op,6))
		{
			container_ops = g_com_rx_buf[pos++];	//�Ӹߵ��֣�rndOBE_op��privateinfo_op��gbiccinfo_op��reset_info_op
			pos++;	//��������0x27����0x20
			//ϵͳ��Ϣ����
			for(i = 0; i < 26; i++)
			{
				sys_info[i] = g_com_rx_buf[pos++]; 
			}
			//���������
			if(Getbit(container_ops,7))
			{
				pos++;	//��������0x1d
				for(i = 0; i < 8; i++)
				{
					rnd_info[i] = g_com_rx_buf[pos++];
				}
			}
			//privateinfo,Ŀǰ������
			if(Getbit(container_ops,6))
			{
				/* do nothing */
			}
			//gbiccinfo_op
			if(Getbit(container_ops,5))
			{
				if(g_bst_type == BST_TRADE_GB)	//���״�Ԥ��
				{
					len_icc_info = ICC_0002_LEN + ICC_0015_LEN;
					pos++;	//��������0x28
					icc_info[0] = len_icc_info;
					for(i = 1; i <= len_icc_info; i ++)
					{
						icc_info[i] = g_com_rx_buf[pos++];
					}
					print_info_Info(icc_info,len_icc_info + 1,COM);
				}
				else if(g_bst_type == BST_ISSUE_PREREAD)	//���д�Ԥ��
				{	
					if (g_com_rx_buf[pos] == 0x28)
					{
						pos++;
					}
				}
				else if (g_bst_type == BST_ISSUE_BJ)	//��������
				{
					icc_info[0] = 62;
					for(i = 0;i < 62;i++)
					{
						icc_info[i+1] = g_com_rx_buf[pos++];
					}
				}
			}
			//��λ��Ϣ����,��������ר��
			//sprintf(InfoTmp,"fu wei xin xi, container_ops = %X\n", container_ops);
			//print_info_Info(InfoTmp,strlen(InfoTmp),COM);
			//��λ��Ϣ����,��������ר��
			if(Getbit(container_ops,4))
			{
				pos++;	//��������0x50
				for(i = 0; i < 15; i++)
				{
					reset_info[i] = g_com_rx_buf[pos++];
				}
				sprintf(InfoTmp,"fu wei xin xi, reset_info = %X %X ..... %X %X\n", reset_info[0],reset_info[1], reset_info[13],reset_info[14]);
				print_info_Info(InfoTmp,strlen(InfoTmp),COM);

			}

		}/* end if(Getbit(dsrc_op,6)) */
		pos += 4;//obu_mac
		pos++; //Ӳ���汾��
		*obu_status = g_com_rx_buf[pos++];//obu״̬��Ϣ
		pos++; //obu_status�еı����ֽ�
	}	
 
	if(vst_len != pos - 4)
	{
		 
		return -3;
	}
	return 0;
}

/*
* �������ƣ�	GetSecure_rq()
* ����������	GetSecure_Request
* �����б�	��
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
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
	sbuf[slen++] = g_rsctl;	//֡���  
	sbuf[slen++] = 0xf0;	//�����,bst�Լ�actionԭ�� 
	sbuf[slen++] = 0x01;	//getsecure_rq
	len_pos = slen++;		//�����ܳ���λ��
	//���getsecure_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;	//mac������
	sbuf[slen++] = 0x77 | g_u8LLCFlag ;	//llc������,0xf7
	if(g_u8LLCFlag == 0x80)
		g_u8LLCFlag = 0x00;
	else
		g_u8LLCFlag = 0x80;

 

	sbuf[slen++] = 0x91;	//����ͷ
//	if(vst.sysinfo[9] == 0x16||vst.sysinfo[9] == 0x12)
	{
		sbuf[slen++] = 0x05;
	}
//	else
//	{
//		sbuf[slen++] = 0x0d;
//	}
//	sbuf[slen++] = 0x05;	//action��ʶ0x05,�����Ϸ���accessCredentialsOp����,��Ϊ0x0d
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
	sbuf[slen++] = 0x14;	//container��ʶ
	sbuf[slen++] = KeyIDforEncryptOp;	//0x80-������Կ��ʶ���ڣ�0x00-������
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
		g_veh_file_sign = 0;//���ķ�ʽ,
	}else
	{
		sbuf[slen++] = 0x00;	//keyIDforAuthen
		sbuf[slen++] = KeyIDforEncrypt;
		g_veh_file_sign = 1;	//���ķ�ʽ,����Э���ǰ���Э�����ͷֵ�
	}
//	sbuf[slen++] = 0x00;	//keyIDforAuthen
//	if(KeyIDforEncryptOp == 0x80)
//	{
//		sbuf[slen++] = KeyIDforEncrypt;
//		g_veh_file_sign = 1;	//���ķ�ʽ,����Э���ǰ���Э�����ͷֵ�
//	}
	sbuf[len_pos] = slen - len_pos - 1;
	chk = sbuf[2];
	for(i = 3;i < slen;i++)
	{
		chk = chk ^ sbuf[i];
	}
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;
	len_vehicleinfo = length;	//���ڽ���getsecure_rs
	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);
	ret = serial_send();	//���õײ㷢�ͺ���
	sem_init(&g_sem_getsecure, 0, 0);
	is_getsecure_recved = 0;	//��ʼû���յ�
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
* �������ƣ�	GetSecure_rs()
* ����������	GetSecure_Response
* �����б�	��
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
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
				is_getsecure_recved = 1;	//�յ����߳�ʱ�˳�����������
				sem_destroy(&g_sem_getsecure);
				return -1;	//��ʱ��û���յ�vst
			}
			usleep(20000);
		}
	}
	is_getsecure_recved = 1;	//�յ����߳�ʱ�˳�����������
	sem_destroy(&g_sem_getsecure);
	if(IsGetseq_rs(g_com_rx_buf) != SUCCESS)	//�ж��Ƿ���GetSecure_rs
	{
		return -2;
	}
	pos = 3;	//ȥ��֡��š������0xe0��ԭ������0x01
	getsecure_rs_len = g_com_rx_buf[pos++];
	//����getsecure_rs
	pos += 4;	//����macid
	pos++;		//mac������0xe0
	pos++;		//llc������
	pos++;		//״̬����
	pos++;		//����ͷ
	action_para = g_com_rx_buf[pos++];	//action��ʶ0x18
	pos++;		//DID 0x01
	if(Getbit(action_para,3))
	{
		pos++;	//��������0x15
		pos++;	// FID 0x01
		//��������
//		if(g_veh_file_sign)
//		{
			len_vehicleinfo = len_vehicleinfo + 8 + 1;	//���ĳ��ȼ�8���ֽڼ����룬1���ֽڳ�����Ϣ
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
	//������������г���

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
����������RSUͨ��͸��ͨ����OBU���в��� 
���������DID--Ҫ��ȡ��OBUӦ�úţ�ETCӦ��Ϊ1����ʶվӦ��Ϊ2 
		ChannelID--ͨ��ID�� 
		APDULIST--��ͨ��������APDU������ 
		APDU--��ͨ��������APDU�����˳��Ϊָ��1����(1�ֽ�)��ָ��1��ָ��2����(1�ֽ�)��ָ��2���� 
���������	�� 
����ֵ��	0--����ִ�гɹ� 
		-100--��ʱ��RSU�豸����Ӧ 
		-1000--����������� 
		-1001--�豸δ�� 
		-2000--�������� 
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
	sbuf[slen++] = g_rsctl; //֡���  
	sbuf[slen++] = 0xf0; 	//�����,bst�Լ�actionԭ�� 
	sbuf[slen++] = 0x02;	//transferchannel_rq
	len_pos = slen++;		//�����ܳ���λ��
	//���transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;	//mac������
//	sbuf[slen++] = 0x77;	//llc������  0xf7
	sbuf[slen++] = 0x77 | g_u8LLCFlag ;	//llc������,0xf7
	if(g_u8LLCFlag == 0x80)
		g_u8LLCFlag = 0x00;
	else
		g_u8LLCFlag = 0x80;
	sbuf[slen++] = 0x91;	//����ͷ 
	sbuf[slen++] = 0x05;	//action��ʶ0x05 
	sbuf[slen++] = DID;		//DID,��Ŀ¼0x00,Ӧ����Ŀ¼0x01
	sbuf[slen++] = 0x03;	//actiontype
	sbuf[slen++] = 0x18;	//��������
	sbuf[slen++] = ChannelID;	//icc-0x01,esam-0x02
	sbuf[slen++] = APDULIST; 
	//����APDU�ܳ���
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
		if(g_IscanSend != 0)//�Ƿ���Ҫ�ȴ����ʹ������ݣ�0���ȴ���1��ֱ�ӷ���; 2:�����ʼֵ
			break;
		usleep(1000*100);
	}

	memset(&g_com_rx_buf,0,sizeof(g_com_rx_buf));



	is_transfer_recved = 0;	//�յ����߳�ʱ�˳�����������

	int re_serial_send_count=g_timeout_max;
	while(re_serial_send_count>0)
	{

			sem_init(&g_sem_transfer,0,0);
			ret = serial_send();	//���õײ㷢�ͺ���
		 
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
���������������00�漰10��OBUȡ���������
�����������
�����������
����ֵ��	 0--����ִ�гɹ�
		��0--���ݷ���ʧ��
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
	sbuf[slen++] = g_rsctl; //֡���
	sbuf[slen++] = 0xf0; 	//�����,bst�Լ�actionԭ��
	sbuf[slen++] = 0x02;	//transferchannel_rq
	len_pos = slen++;		//�����ܳ���λ��
	//���transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	//50 F7 91 04 01 02 04
	sbuf[slen++] = 0x50;
	sbuf[slen++] = 0xF7;
	sbuf[slen++] = 0x91;
	sbuf[slen++] = 0x04;
	sbuf[slen++] = 0x01;//ԭΪ0x01
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
		if(g_IscanSend != 0)//�Ƿ���Ҫ�ȴ����ʹ������ݣ�0���ȴ���1��ֱ�ӷ���; 2:�����ʼֵ
			break;
		usleep(1000*100);
	}
	ret = serial_send();	//���õײ㷢�ͺ���
	sem_init(&g_sem_transfer,0,0);
	is_transfer_recved = 0;	//�յ����߳�ʱ�˳�����������
	return ret;
}
/*
* �������ƣ�	IsTransfer_rs()
* ����������	����ĿǰЭ�飬û����GetSeq_rs��ƴ��
* �����б�	��
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
static uint8 IsTransfer_rs(uint8 *pkg)
{
	if(((vst.macid[0] == 0x02)&&((vst.sysinfo[9] == 0x00)||(vst.sysinfo[9] == 0x10))))
	//����1��OBU(00��10��)
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
����������͸��ͨ����������Ӧ 
���������	fd--�豸����� 
		time_out--��ʱʱ�䣬��Χ1-1000ms 
���������	DID--OBUӦ�úţ�ETCӦ��Ϊ1����ʶվӦ��Ϊ2 
		ChannelID--ͨ��ID�� 
		APDULIST--��ͨ��������APDU������ 
		Data--��ͨ��������APDU��������ݣ���˳��Ϊ����1����(1�ֽ�)������1������2����(1�ֽ�)������2���� 
		ReturnStatus--OBU����״̬ 
����ֵ��	0--����ִ�гɹ� 
		-1--��ʱ��RSU�豸����Ӧ
		����--�ظ�֡����transfer֡
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
//				is_transfer_recved = 1;		//�յ����߳�ʱ�˳�����������
//				sem_destroy(&g_sem_transfer);
//				return -1;		//��ʱ��û���յ�vst
//			}
//			usleep(20000);
//		}
//	}
//	is_transfer_recved = 1;	//�յ����߳�ʱ�˳�����������
//	sem_destroy(&g_sem_transfer);
	ret = IsTransfer_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//�ж��Ƿ�ʱTransferChannel_rs
	{
		return -2 + ret*10;
	}
	pos = 3;	//ȥ��֡��š������0xe0��ԭ������0x02
	transfer_rs_len = g_com_rx_buf[pos++];
	//����transfer_rs
	pos += 4;	//����macid
	pos++;		//mac������0xe0
	pos++;		//llc������
	pos++;		//״̬����
	pos++;		//����ͷ
	action_para = g_com_rx_buf[pos++];	//action��ʶ0x18
	pos++;		//DID 0x01
	//�ж������Ƿ����
	if(Getbit(action_para,3))
	{
		pos++;	//��������0x19
		pos++;	//ͨ����
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
����������͸��ͨ����������Ӧ
���������	fd--�豸�����
		time_out--��ʱʱ�䣬��Χ1-1000ms
���������	DID--OBUӦ�úţ�ETCӦ��Ϊ1����ʶվӦ��Ϊ2
		ChannelID--ͨ��ID��
		APDULIST--��ͨ��������APDU������
		Data--��ͨ��������APDU��������ݣ���˳��Ϊ����1����(1�ֽ�)������1������2����(1�ֽ�)������2����
		ReturnStatus--OBU����״̬
����ֵ��	0--����ִ�гɹ�
		-1--��ʱ��RSU�豸����Ӧ
		����--�ظ�֡����transfer֡
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
				is_transfer_recved = 1;		//�յ����߳�ʱ�˳�����������
				//sem_destroy(&g_sem_transfer);
				return -1;		//��ʱ��û���յ�vst
			}
			//usleep(20000);
		}
	}
	is_transfer_recved = 1;	//�յ����߳�ʱ�˳�����������
	//sem_destroy(&g_sem_transfer);
	ret = IsTransfer_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//�ж��Ƿ�ʱTransferChannel_rs
	{
		return -2 + ret*10;
	}
	pos = 3;	//ȥ��֡��š������0xe0��ԭ������0x02
	transfer_rs_len = g_com_rx_buf[pos++];
	//����transfer_rs
	pos += 4;	//����macid
	pos++;		//mac������0xe0
	pos++;		//llc������
	pos++;		//״̬����
	pos++;		//����ͷ
	action_para = g_com_rx_buf[pos++];	//action��ʶ0x18
	pos++;		//DID 0x01
	//�ж������Ƿ����
	if(Getbit(action_para,3))
	{
		pos++;	//��������0x19
		//pos++;	//ͨ����
		*channelID = g_com_rx_buf[pos++];	//ͨ����
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
����������͸��ͨ����������Ӧ
���������	fd--�豸�����
		time_out--��ʱʱ�䣬��Χ1-1000ms
���������	DID--OBUӦ�úţ�ETCӦ��Ϊ1����ʶվӦ��Ϊ2
		ChannelID--ͨ��ID��
		APDULIST--��ͨ��������APDU������
		Data--��ͨ��������APDU��������ݣ���˳��Ϊ����1����(1�ֽ�)������1������2����(1�ֽ�)������2����
		ReturnStatus--OBU����״̬
����ֵ��	0--����ִ�гɹ�
		-100--��ʱ��RSU�豸����Ӧ
		-101--��ʱ��OBU����Ӧ
		-103--��ʱ��ͨ���豸(IC����ESAM��)����Ӧ
		-1000--�����������
		-1001--�豸δ��
		-2000--��������
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
				is_transfer_recved = 1;		//�յ����߳�ʱ�˳�����������
				sem_destroy(&g_sem_transfer);
				return -1;		//��ʱ
			}
			usleep(20000);
		}
	}
	is_transfer_recved = 1;	//�յ����߳�ʱ�˳�����������
	sem_destroy(&g_sem_transfer);
	ret = IsTransfer_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//�ж��Ƿ�ʱTransferChannel_rs
	{
		return -2 + ret*10;
	}
	pos = 3;	//ȥ��֡��š������0xe0��ԭ������0x02
	transfer_rs_len = g_com_rx_buf[pos++];
	//����transfer_rs
	pos += 4;	//����macid
	pos++;		//mac������0xe0
	pos++;		//llc������
	pos++;		//״̬����
	pos++;		//����ͷ
	action_para = g_com_rx_buf[pos++];	//action��ʶ0x18
	pos++;		//DID 0x01
	//�ж������Ƿ����
	if(Getbit(action_para,3))
	{
		pos++;	//��������0x19
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
������������OBU���н�����ʾ 
���������fd--�豸����� 
		mode--ȷ��ģʽ��1����Ӧ��0������Ӧ�� 
		DID--OBUӦ�úţ�ETCӦ��Ϊ1����ʶվӦ��Ϊ2 
		SetMMIPara--�˻�������� 
		time_out--��ʱʱ�䣬��Χ1-1000ms 
���������	�� 
����ֵ��	0-����ִ�гɹ� 
		-100--��ʱ��RSU�豸����Ӧ 
		-1000--����������� 
		-1001--�豸δ�� 
		-2000--�������� 
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
	sbuf[slen++] = g_rsctl; //֡���  
	sbuf[slen++] = 0xf0; 	//�����,bst�Լ�actionԭ�� 
	sbuf[slen++] = 0x03;	//Setmmi_rq
	len_pos = slen++; 		//�����ܳ���λ��
	//���transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;	//mac������
//	sbuf[slen++] = 0xf7;	//llc������
	sbuf[slen++] = 0x77 | g_u8LLCFlag ;	//llc������,0xf7
	if(g_u8LLCFlag == 0x80)
		g_u8LLCFlag = 0x00;
	else
		g_u8LLCFlag = 0x80;
	sbuf[slen++] = 0x91;	//����ͷ 
	sbuf[slen++] = 0x05;	//action��ʶ0x05 
	sbuf[slen++] = 0x01;	//DID
	sbuf[slen++] = 0x04;	//actiontype
	sbuf[slen++] = 0x1A;	//��������
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
	ret = serial_send();	//���õײ㷢�ͺ���
	sem_init(&g_sem_setmmi,0,0);
	is_setmmi_recved = 0;	//�յ����߳�ʱ�˳�����������
	return ret; 
}

/*
SetMMI_Request
������������OBU���н�����ʾ
���������fd--�豸�����
		mode--ȷ��ģʽ��1����Ӧ��0������Ӧ��
		DID--OBUӦ�úţ�ETCӦ��Ϊ1����ʶվӦ��Ϊ2
		SetMMIPara--�˻��������
		time_out--��ʱʱ�䣬��Χ1-1000ms
���������	��
����ֵ��	0-����ִ�гɹ�
		-100--��ʱ��RSU�豸����Ӧ
		-1000--�����������
		-1001--�豸δ��
		-2000--��������
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
	sbuf[slen++] = g_rsctl; //֡���
	sbuf[slen++] = 0xf0; 	//�����,bst�Լ�actionԭ��
	sbuf[slen++] = 0x03;	//Setmmi_rq
	len_pos = slen++; 		//�����ܳ���λ��
	//���transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;	//mac������
//	sbuf[slen++] = 0xf7;	//llc������
	sbuf[slen++] = 0x77 | g_u8LLCFlag ;	//llc������,0xf7
	if(g_u8LLCFlag == 0x80)
		g_u8LLCFlag = 0x00;
	else
		g_u8LLCFlag = 0x80;
	sbuf[slen++] = 0x91;	//����ͷ
	sbuf[slen++] = 0x05;	//action��ʶ0x05
	sbuf[slen++] = 0x01;	//DID
	sbuf[slen++] = 0x04;	//actiontype
	sbuf[slen++] = 0x1A;	//��������
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
	ret = serial_send();	//���õײ㷢�ͺ���
	sem_init(&g_sem_setmmi,0,0);
	is_setmmi_recved = 0;	//�յ����߳�ʱ�˳�����������
	return ret;
}

/*
* �������ƣ�	IsSetMMI_rs()
* ����������	ĿǰЭ�鲻֧��ƴ��
* ���������	��
* ���������  prog_trade_b5--�ɹ����׽���֡
* ����ֵ��	0--�ɹ�����0--ʧ��
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
������������OBU���н�����ʾ����Ӧ 
���������	fd--�豸����� 
		time_out--��ʱʱ�䣬��Χ1-1000ms 
���������	DID--OBUӦ�úţ�ETCӦ��Ϊ1����ʶվӦ��Ϊ2 
		ReturnStatus--OBU����״̬ 
����ֵ��	0--����ִ�гɹ� 
		-100--��ʱ��RSU�豸����Ӧ 
		-101--��ʱ��OBU����Ӧ 
		-1000--����������� 
		-1001--�豸δ�� 
		-2000--�������� 
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
				is_setmmi_recved = 1;	//�յ����߳�ʱ�˳�����������
				//sem_destroy(&g_sem_setmmi);
				return -1;	//��ʱ��û���յ�vst
			}
			//usleep(20000);
		}
	}
	is_setmmi_recved = 1;	//�յ����߳�ʱ�˳�����������
	//sem_destroy(&g_sem_setmmi);
	ret = IsSetMMI_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//�ж��Ƿ���SetMMI_rs
	{
		return -2 + ret*10;
	}
	pos = 3;	//ȥ��֡��š������0xe0��ԭ������0x03
	setmmi_rs_len = g_com_rx_buf[pos++];
	//����setmmi_rs
	pos += 4;//����macid
	pos++;//mac������0xe0
	pos++;//llc������
	pos++;//״̬����
	pos++;//����ͷ
	action_para = g_com_rx_buf[pos++];//action��ʶ0x18
	pos++;//DID 0x01
	//�����Ƿ����
	if(Getbit(action_para,3))
	{
		pos++; //��������0x1b
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
�����������ͷ�OBU 
���������	fd--�豸����� 
		mode--ȷ��ģʽ��1����Ӧ��0������Ӧ�� 
		DID--DSRC-DID��Ŀ¼�� 
		EventType--Release=0 
		time_out--��ʱʱ�䣬��Χ1-1000ms 
���������	�� 
����ֵ��	0--����ִ�гɹ� 
		-100--��ʱ��RSU�豸����Ӧ 
		-1000--����������� 
		-1001--�豸δ�� 
		-2000--�������� 
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
	sbuf[slen++] = g_rsctl; //֡���  
	sbuf[slen++] = 0xf0; 	//�����,bst�Լ�actionԭ�� 
	sbuf[slen++] = 0x04;	//event-report
	len_pos = slen++; 		//�����ܳ���λ��
	//���transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;	//mac������
	sbuf[slen++] = 0x03;	//llc������
	sbuf[slen++] = 0x91;	//����ͷ 
	sbuf[slen++] = 0x60;	//eventreport��ʶ0x60
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
	ret = serial_send();	//���õײ㷢�ͺ���
	memset(g_com_tx_buf, 0x00, sizeof(g_com_tx_buf));
	g_com_tx_len = 0;
	ret = serial_send();	//���õײ㷢�ͺ���
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
				is_prwrq_recved = 1; //�յ����߳�ʱ�˳�����������
				sem_destroy(&g_sem_prwrq);
				return -1; //��ʱ��û���յ�prwrq
			}
			usleep(20000);
		}
	}	
	is_prwrq_recved = 1; //�յ����߳�ʱ�˳�����������
	sem_destroy(&g_sem_prwrq);

	pos = 3; //ȥ��֡��š������0xe0��ԭ������0x00

	len = g_com_rx_buf[pos++];
	
	for(i = 0;i < 4;i++)
		obu_mac[i] = g_com_rx_buf[pos++];
	pos++;  //MAC������
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
	sbuf[slen++] = 0xf0; // �����,bst�Լ�actionԭ�� 
	sbuf[slen++] = 0x06;//PrWA
	len_pos = slen++; //�����ܳ���λ�� 

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
 
	// ���õײ㷢�ͺ��� 
	ret = serial_send(); 

	sem_init(&g_sem_vst,0,0);
	is_vst_recved = 0; //��ʼû���յ�
 
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
	sbuf[slen++] = 0xf0; // �����,bst�Լ�actionԭ�� 
	sbuf[slen++] = 0x02;
	len_pos = slen++; //�����ܳ���λ�� 

	//���SetSecure_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;//mac������
	sbuf[slen++] = 0x77;//llc������
	sbuf[slen++] = 0x91; //����ͷ 
	sbuf[slen++] = 0x0d; //action��ʶ0x05 
	sbuf[slen++] = 0x03; //DID
	sbuf[slen++] = 0x01; //actiontype

	for (i=0; i<8; i++) 
		sbuf[slen++] = accessCredentials[i]; //����֤��
	
	sbuf[slen++] = 0x16; //SetSecureRq
	sbuf[slen++] = 0x00; //KeyIDforEncryptOp������
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
 
	// ���õײ㷢�ͺ��� 
	ret = serial_send(); 

	sem_init(&g_sem_setsecure,0,0);
	is_setsecure_recved = 0; //�յ����߳�ʱ�˳�����������

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
				is_quickack_recved = 1; //�յ����߳�ʱ�˳�����������
				sem_destroy(&g_sem_quickack);
				return -1; //��ʱ��û���յ�quickack
			}
			usleep(20000);
		}
	}	
	is_quickack_recved = 1; //�յ����߳�ʱ�˳�����������
	sem_destroy(&g_sem_quickack);

	pos = 3; //ȥ��֡��š������0xe0��ԭ������0x00

	len = g_com_rx_buf[pos++];
	
	//for(i = 0;i < 4;i++)
		//macid[i] = g_com_rx_buf[pos++];
	//pos++;  //MAC������
	
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
	sbuf[slen++] = 0xf0; // �����,bst�Լ�actionԭ�� 
	sbuf[slen++] = 0x02;
	len_pos = slen++; //�����ܳ���λ�� 

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
 
	// ���õײ㷢�ͺ��� 
	ret = serial_send(); 

	sem_init(&g_sem_setsecure,0,0);
	is_setsecure_recved = 0; //��ʼû���յ�
 
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
				is_setsecure_recved = 1; //�յ����߳�ʱ�˳�����������
				sem_destroy(&g_sem_setsecure);
				return -1; //��ʱ��û���յ�setsecure
			}
			usleep(20000);
		}
	}	
	is_setsecure_recved = 1; //�յ����߳�ʱ�˳�����������
	sem_destroy(&g_sem_setsecure);

	pos = 3; //ȥ��֡��š������0xe0��ԭ������0x00

	len = g_com_rx_buf[pos++];

	if(len == 0x0d)	//�յ����ǿ���Ӧ��֡
	{
		ret = GetAns_rq(prwrq_pib.OBUMAC, prwrq_pib.AntID); //����ȡ���֡
		if(ret != 0)
		{
			return -2 + ret*10;
		}
		goto _WAIT;
	}else if(len == 0x0a)//�յ����ǽ��׳�ʱ֡
	{
		return -3 ;
	}	
	return 0;
}

/*
д������Ϣ�ļ��������ϰ棨00,10�棩OBUд������Ϣ�Զ����
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
	sbuf[slen++] = 0x13; // ֡��� 
	sbuf[slen++] = 0xf0; // �����,bst�Լ�actionԭ�� 
	sbuf[slen++] = 0x02;
	len_pos = slen++; //�����ܳ���λ�� 

	//���transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;//mac������
	sbuf[slen++] = 0xf7;//llc������
	sbuf[slen++] = 0x91; //����ͷ 
	sbuf[slen++] = 0x81;
	//4���ֽڣ��ļ�Ŀ¼��Ϣ
	sbuf[slen++] = 0x01; //��ʾĿ¼�ţ�00��Ŀ¼��01Ӧ��Ŀ¼
	sbuf[slen++] = 0x01; //��ʾд�ļ�
	sbuf[slen++] = 0x00;
	sbuf[slen++] = offset;

	sbuf[slen++] = length; //ϵͳ��Ϣ����32���ֽ�
	for(i = 0;i < length;i++)
		sbuf[slen++]= file[i];

	// ����ESAM��ϵͳ��Ϣ�ļ�MAC 
//    ret = psamCalcEsamMac(psam_slot, offset,length,file,1);//ϵͳ��Ϣ��־��0,��Ϊ���÷�ɢ���Ӳ�ͬhmh
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


	//���Դ���
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


	// ���õײ㷢�ͺ���
	ret = serial_send(); 
	sem_init(&g_sem_transfer,0,0);
	is_transfer_recved = 0;	//�յ����߳�ʱ�˳�����������
	return ret; 
}

/*
д������Ϣ�ļ��������ϰ棨00,10�棩OBUд������Ϣ�Զ����
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
	sbuf[slen++] = 0x13; // ֡���
	sbuf[slen++] = 0xf0; // �����,bst�Լ�actionԭ��
	sbuf[slen++] = 0x02;
	len_pos = slen++; //�����ܳ���λ��

	//���transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;//mac������
	sbuf[slen++] = 0xf7;//llc������
	sbuf[slen++] = 0x91; //����ͷ
	sbuf[slen++] = 0x81;
	//4���ֽڣ��ļ�Ŀ¼��Ϣ
	sbuf[slen++] = 0x01; //��ʾĿ¼�ţ�00��Ŀ¼��01Ӧ��Ŀ¼
	sbuf[slen++] = 0x01; //��ʾд�ļ�
	sbuf[slen++] = 0x00;

//	sbuf[slen++] = 0x04; //��ʾĿ¼�ţ�00��Ŀ¼��01Ӧ��Ŀ¼
//	sbuf[slen++] = 0xd6; //��ʾд�ļ�
//	sbuf[slen++] = 0x81;
	sbuf[slen++] = offset;

	sbuf[slen++] = length;
	for(i = 0;i < length;i++)
		sbuf[slen++]= file[i];

	for(i = 0;i < 4;i++)
			sbuf[slen++]= file[i+59];//mac�����4���ֽڣ�����д��Ϣ�ĳ����Ƕ���

	sbuf[len_pos] = slen - len_pos - 1;

	chk = sbuf[2];
	for(i = 3;i < slen;i++)
		chk = chk  ^ sbuf[i];
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;

	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);


	//���Դ���
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


	// ���õײ㷢�ͺ���
	ret = serial_send();
	sem_init(&g_sem_transfer,0,0);
	is_transfer_recved = 0;	//�յ����߳�ʱ�˳�����������
	return ret;
}
/*
д������Ϣָ��أ�
4�ֽ�OBU MacID + e0 f7(77) 00 91 + 90 00 01 00
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
				is_transfer_recved = 1;		//�յ����߳�ʱ�˳�����������
				sem_destroy(&g_sem_transfer);
				return -1;		//��ʱ��û���յ�vst
			}
			usleep(20000);
		}
	}
	is_transfer_recved = 1;	//�յ����߳�ʱ�˳�����������
	sem_destroy(&g_sem_transfer);
	ret = IsTransfer_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//�ж��Ƿ�ʱTransferChannel_rs
	{
		return -2 + ret*10;
	}
		
	pos = 3; //ȥ��֡��š������0xe0��ԭ������0x02

	WriteSysFile_rs_len = g_com_rx_buf[pos++];
	//����transfer_rs
	pos += 4; //����macid
	pos++; //mac������0xe0
	pos++;//llc������
	pos++; //״̬����
	pos++; //����ͷ
	if(g_com_rx_buf[pos++] != 0x90)
		return -3; //0x90
	pos++; //��ʾĿ¼�ţ�00��Ŀ¼��01Ӧ��Ŀ¼
	pos++; //0x01
	if(g_com_rx_buf[pos++] != 0x00)//pos++; //0x00
	{
		return -4; //��0x00
	}
//	if(WriteSysFile_rs_len != pos - 4)
//		return -1;

	return 0;
}

/*
дϵͳ��Ϣ�ļ��������ϰ棨00,10�棺
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
	sbuf[slen++] = 0x13; // ֡���
	sbuf[slen++] = 0xf0; // �����,bst�Լ�actionԭ��
	sbuf[slen++] = 0x02;
	len_pos = slen++; //�����ܳ���λ��

	//���transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;//mac������
	sbuf[slen++] = 0xf7;//llc������
	sbuf[slen++] = 0x91; //����ͷ
	sbuf[slen++] = 0x81;
	//4���ֽڣ��ļ�Ŀ¼��Ϣ
	sbuf[slen++] = 0x00; //��ʾĿ¼�ţ�00��Ŀ¼��01Ӧ��Ŀ¼
	sbuf[slen++] = 0x01; //��ʾд�ļ�
	sbuf[slen++] = 0x00;
	sbuf[slen++] = offset;

	sbuf[slen++] = length; //ϵͳ��Ϣ����32���ֽ�
	for(i = 0;i < length;i++)
		sbuf[slen++]= file[i];

//	�����ϰ�obu �Ѵ�esam��ȡ���������esam_random�����ϴ���Ϣ֡(дϵͳ��Ϣ�����)  add by wll 2013-7-5 17:07:36
	memcpy(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom,esam_random,sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom));

	// ����ESAM��ϵͳ��Ϣ�ļ�MAC
    ret = psamCalcEsamMac(psam_slot, offset,length,file,0);//ϵͳ��Ϣ��־��0,��Ϊ���÷�ɢ���Ӳ�ͬ
	if (ret != 0)
		return -1 + ret*10;
	for (i = 0; i < 4; i ++)
		sbuf[slen++] = esam_mac[i];       //MAC
//	�����ϰ�obu  ��д������Ϣָ��Ƶ��ϴ�������Ϣ֡��(д������Ϣָ��)  add by wll 2013-7-5 15:16:09
	memcpy(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand,&sbuf[slen - 4 - length - 9],9 + length + 4);
//	��0
	memset(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[9 + length + 4], 0x00, sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand) - (9 + length + 4));

	sbuf[len_pos] = slen - len_pos - 1;

	chk = sbuf[2];
	for(i = 3;i < slen;i++)
		chk = chk ^ sbuf[i];
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;

	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);


	//���Դ���
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


	// ���õײ㷢�ͺ���
	ret = serial_send();
	sem_init(&g_sem_transfer,0,0);
	is_transfer_recved = 0;	//�յ����߳�ʱ�˳�����������
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
	sbuf[slen++] = 0x13; // ֡���
	sbuf[slen++] = 0xf0; // �����,bst�Լ�actionԭ��
	sbuf[slen++] = 0x02;
	len_pos = slen++; //�����ܳ���λ��

	//���transferchannel_rq
	sbuf[slen++] = vst.macid[0];
	sbuf[slen++] = vst.macid[1];
	sbuf[slen++] = vst.macid[2];
	sbuf[slen++] = vst.macid[3];
	sbuf[slen++] = 0x40;//mac������
	sbuf[slen++] = 0xf7;//llc������
	sbuf[slen++] = 0x91; //����ͷ
	sbuf[slen++] = 0x81;
	//4���ֽڣ��ļ�Ŀ¼��Ϣ
	sbuf[slen++] = 0x00; //��ʾĿ¼�ţ�00��Ŀ¼��01Ӧ��Ŀ¼
	sbuf[slen++] = 0x01; //��ʾд�ļ�
	sbuf[slen++] = 0x00;
	sbuf[slen++] = offset;

	sbuf[slen++] = length; //ϵͳ��Ϣ����32���ֽ�
	for(i = 0;i < length;i++)
		sbuf[slen++]= file[i];

	for(i = 0;i < 4;i++)
		sbuf[slen++]= file[i+9];//����Э����mac���������4���ֽ�
//	�����ϰ�obu �Ѵ�esam��ȡ���������esam_random�����ϴ���Ϣ֡(дϵͳ��Ϣ�����)  add by wll 2013-7-5 17:07:36
	memcpy(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom,esam_random,sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoRandom));

//	�����ϰ�obu  ��д������Ϣָ��Ƶ��ϴ�������Ϣ֡��(д������Ϣָ��)  add by wll 2013-7-5 15:16:09
	memcpy(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand,&sbuf[slen - 4 - length - 9],9 + length + 4);
//	��0
	memset(&g_frame_uploadtradeinfo_rq.WriteSysInfoCommand[9 + length + 4], 0x00, sizeof(g_frame_uploadtradeinfo_rq.WriteSysInfoCommand) - (9 + length + 4));

	sbuf[len_pos] = slen - len_pos - 1;

	chk = sbuf[2];
	for(i = 3;i < slen;i++)
		chk = chk ^ sbuf[i];
	sbuf[slen++] = chk & 0xff;
	sbuf[slen++] = 0xff;

	g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);


	//���Դ���
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


	// ���õײ㷢�ͺ���
	ret = serial_send();
	sem_init(&g_sem_transfer,0,0);
	is_transfer_recved = 0;	//�յ����߳�ʱ�˳�����������
	if(ret != 0)
		return -3 + ret *10;
	return ret;
}

/*
дϵͳ��Ϣָ���
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
				is_transfer_recved = 1;		//�յ����߳�ʱ�˳�����������
				sem_destroy(&g_sem_transfer);
				return -1;
			}
			usleep(20000);
		}
	}
	is_transfer_recved = 1;	//�յ����߳�ʱ�˳�����������
	sem_destroy(&g_sem_transfer);
	ret = IsTransfer_rs(g_com_rx_buf);
	if(ret != SUCCESS)	//�ж��Ƿ�ʱTransferChannel_rs
	{
		return -2 + ret*10;
	}

	pos = 3; //ȥ��֡��š������0xe0��ԭ������0x02

	WriteSysFile_rs_len = g_com_rx_buf[pos++];
	//����transfer_rs
	pos += 4; //����macid
	pos++; //mac������0xe0
	pos++;//llc������
	pos++; //״̬����
	pos++; //����ͷ
	if(g_com_rx_buf[pos++] != 0x90)
		return -3; //0x90
	pos++; //��ʾĿ¼�ţ�00��Ŀ¼��01Ӧ��Ŀ¼
	pos++; //0x01
	if(g_com_rx_buf[pos++] != 0x00)//pos++; //0x00
	{
		return -4; //��0x00
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
