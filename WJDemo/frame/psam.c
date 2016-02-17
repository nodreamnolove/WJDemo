
#include "lib2hd.h"
#include "common.h"
#include "psam.h"
#include "lib2rsu.h"
#include "crc.h"


static const uint8 psam_password[8] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38};	//pin��У��



/*
* �������ƣ�	psamEnterDir()
* ����������	 
* �����б�	��
* ���ؽ����	
*/
int8 psamEnterDir(int psam_slot, uint16 dir)
{
	uint8 apdulist = 1;	//��������
	uint8 apdu[128];
	int datalist;	//��������
	uint8 data[128];
	int ret, len, i;
	apdu[0] = 0x07;				//7	 ����ETCӦ��Ŀ¼
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
		//ע��psam�����ĳ��Ȳ�����9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			return SUCCESS;
		}
	}
	return -2 + ret*10;
}

/*
* �������ƣ�	psamEnterETCDir()
* ����������	PSAMĸ��������֤������ETCӦ��Ŀ¼ 
* �����б�	��
* ���ؽ����	
*/
int8 psamEnterETCDir(int psam_slot)
{
	uint8 apdulist = 2;	//��������
	uint8 apdu[128];
	int datalist;	//��������
	uint8 data[128];
	int ret, len, i;
	apdu[0] = 0x0d;	//13,������֤
	apdu[1] = 0x00;
	apdu[2] = 0x20;
	apdu[3] = 0x00;
	apdu[4] = 0x01;
	apdu[5] = 0x08;	//8
	for(i = 0;i < 8;i++)
	{
	    apdu[i + 6] = psam_password[i];
	}
	apdu[14] = 0x07;	//7,����ETCӦ��Ŀ¼
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
		len = data[3];	//�ڶ�������ķ������ݳ���
		//ע��psam�����ĳ��Ȳ�����9000
		if((data[len + 4] == 0x90) && (data[len + 5] == 0x00))
		{
			return SUCCESS;
		}
	}
	return -2 + ret*10;
}

/*
* �������ƣ�	psamGetMK_MF()
* ����������	ȡ��ϵͳ������Կ
* �����б�	��
* ���ؽ����	
*/
int psamGetMK_MF(int psam_slot,uint8 P1,uint8 P2)
{
	return 1; 
}

/*
* �������ƣ�	psamGetDAMK_MF()
* ����������	ȡ��MFϵͳά����Կ
* �����б�	��
* ���ؽ����	
*/
int8 psamGetDAMK_MF(int psam_slot)
{
	uint8 apdulist = 2;	//��������
	uint8 apdu[128];
	int datalist;//��������
	uint8 data[128];
	int ret, len, i;
	//ָ��һ�����µķ�ɢ���ӽ�����Կ��ɢ
	apdu[0] = 0x13;	//19,������Կ��ɢ
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
		apdu[i + 12] = g_contract_sn[i]; //��ͬ���к�
	}
	//ָ������ھɵ���Կ�����µ����µ�������Կ
	apdu[20] = 0x19;	//25,������Կ
	apdu[21] = 0x80;
	apdu[22] = 0x42;
	apdu[23] = 0x01;
	apdu[24] = 0x00;
	apdu[25] = 0x14;	//20
	apdu[26] = 0x84;			  
	apdu[27] = 0xd4;	
	apdu[28] = 0x01;			  
	apdu[29] = 0xff;
	//оƬ���к���Ϊ������Կ��ɢ����,��Ϊ������Կ����оƬ���кŷ�ɢ��
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
		len = data[3]; //�ڶ�������ķ������ݳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamGetMK_DF01()
* ����������	ȡ��DF01������Կ
* �����б�	��
* ���ؽ����	
*/
uint8 psamGetMK_DF01(int psam_slot,uint8 P1,uint8 P2)
{
	return 1;
}

/*
* �������ƣ�	psamGetDAMK_DF01()
* ����������	ȡ��DF01Ӧ��ά����Կ 
* �����б�	��
* ���ؽ����	
*/
int8 psamGetDAMK_DF01(int psam_slot)
{
	uint8 apdulist = 2;	//��������
	uint8 apdu[128];
	int datalist;	//��������
	uint8 data[128];
	int ret, len, i;
	//��һ��ָ���Կ��ɢ
	apdu[0] = 0x13;	//19,һ����Կ��ɢ
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
	//ָ�������Կ����
	apdu[20] = 0x1a;	//25,������Կ��ɢ
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
		len = data[3]; //�ڶ�������ķ������ݳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamGetKey()
* ����������	 
* �����б�	��
* ���ؽ����	
*/
// ȡ��DF01Ӧ����֤��Կ 
int8 psamGetRK1_DF01(int psam_slot)
{
	uint8 apdulist;//��������
	uint8 apdu[128];
	int datalist;//��������
	uint8 data[128];
	int ret;
	int len;
	int i;

	apdulist = 2;

	//��һ��ָ���Կ��ɢ
	apdu[0] = 0x13;			    //19  һ����Կ��ɢ
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

	//�ڶ���ָ���Կ����
	apdu[20] = 0x1a;				//25   ������Կ��ɢ
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
		len = data[3]; //�ڶ�������ķ������ݳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamGetKey()
* ����������	 
* �����б�	��
* ���ؽ����	
*/
// ȡ��DF01Ӧ�ü�����Կ���汾1�� 
int8 psamGetRK2_DF01_V1(int psam_slot)
{
	uint8 apdulist;//��������
	uint8 apdu[128];
	int datalist;//��������
	uint8 data[128];
	int ret, len, i;

	apdulist = 2;

	//��һ��ָ���Կ��ɢ
	apdu[0] = 0x13;			    //19  һ����Կ��ɢ
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

	//�ڶ���ָ���Կ����
	apdu[20] = 0x1a;				//25   ������Կ��ɢ
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
		len = data[3]; //�ڶ�������ķ������ݳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamGetKey()
* ����������	 
* �����б�	��
* ���ؽ����	
*/
// ȡ��DF01Ӧ�ü�����Կ���汾2�� 
int8 psamGetRK2_DF01_V2(int psam_slot)
{
	uint8 apdulist;//��������
	uint8 apdu[128];
	int datalist;//��������
	uint8 data[128];
	int ret;
	int len;
	int i;

	apdulist = 2;

	//��һ��ָ���Կ��ɢ
	apdu[0] = 0x13;			    //19  һ����Կ��ɢ
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

	//�ڶ���ָ���Կ����
	apdu[20] = 0x1a;				//25   ������Կ��ɢ
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
		len = data[3]; //�ڶ�������ķ������ݳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamGetKey()
* ����������	 
* �����б�	��
* ���ؽ����	
*/
// ȡ��DF01Ӧ�ü�����Կ���汾3�� 
int8 psamGetRK2_DF01_V3(int psam_slot)
{
	uint8 apdulist;//��������
	uint8 apdu[128];
	int datalist;//��������
	uint8 data[128];
	int ret;
	int len;
	int i;

	apdulist = 2;

	//��һ��ָ���Կ��ɢ
	apdu[0] = 0x13;			    //19  һ����Կ��ɢ
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

	//�ڶ���ָ���Կ����
	apdu[20] = 0x1a;				//25   ������Կ��ɢ
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
		len = data[3]; //�ڶ�������ķ������ݳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamCalcEsamMac()
* ����������	����ESAM���ļ�MAC,����PSAM��
* �����б�	
* ���ؽ����	
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
	uint8 apdulist;	//��������
	uint8 apdu[128] = {0};
	int datalist;	//��������
	uint8 data[128] = {0};
	int ret, len, temp, i;
	apdulist = 2;
	if(g_psam_disperse == 1)
	{
		//ֻ����һ�η�ɢ
		//��һ��ָ���Կ��ɢ  ��������Կ��ɢ��
		apdu[0] = 0x0d;	//13
		apdu[1] = 0x80;
		apdu[2] = 0x1a;
		//apdu[3] = 0x26;//����
		apdu[3] = 0x28;//����Э�� change by wll	2013-5-6 12:40:48

		if (g_OBU_ContractVersion == 0x16)//����OBU��ͬ�汾
		{
			if(type == 0)
			{
				apdu[4] = 0x01;	//����ϵͳ��Ϣ�ļ�
			}
			else
			{
				apdu[4] = 0x02;	//����Ӧ�ó�����Ϣ�ļ�
			}
			for(i = 0; i < 8; i++)
			{
				apdu[i + 6] = g_contract_sn[i];   //��ͬ���к�
			}
		}
		else//1��OBU��ͬ�汾
		{
			if(type == 0)//ϵͳ��Ϣ
			{
				apdu[4] = 0x08;	//����ϵͳ��Ϣ�ļ�
				if((vst.macid[0] == 0x02)&&((vst.sysinfo[9] == 0x00)||(vst.sysinfo[9] == 0x10)))
				{
					for(i = 0; i < 8; i++)
					{
						apdu[i + 6] = g_contract_sn[i];   //��ͬ���к�
	//							apdu[i + 6] = EsamDv[i];   //��λ��Ϣ
					}
				}
				else
				{
					for(i = 0; i < 8; i++)
					{
						apdu[i + 6] = EsamDv[i];   //��λ��Ϣ
					}
				}

			}
			else//������Ϣ
			{
				apdu[4] = 0x0a;	//����Ӧ�ó�����Ϣ�ļ�
				for(i = 0; i < 8; i++)
				{
					apdu[i + 6] = g_contract_sn[i];   //��ͬ���к�
				}
			}

		}

		apdu[5] = 0x08;		//16
		//�ڶ���ָ�����mac
		apdu[15] = 0x80;
		apdu[16] = 0xfa;
		apdu[17] = 0x05;
		apdu[18] = 0x00;
		//����mac���õ��������ݣ����������+esam��������
		for(i = 0; i < 4; i++)
		{
			apdu[i + 20] = esam_random[i];
			apdu[i + 24] = 0x00;
		}
		//psam�������õ�����Ҫ��esam��������һ��
		apdu[28] = 0x04;
		apdu[29] = 0xd6;
		apdu[30] = 0x81;
		apdu[31] = offset;
		apdu[32] = length + 4;	//length+4,��4��ʾ4���ֽ�mac
		for(i = 0;i < length;i++)
		{
			apdu[i + 33] = file[i];
		}
		temp = (length + 13) % 8;	//����mac�������ݳ���ģ8
		//�¹��ܷ�8��������Ӧ�ò���һ��0x80+0x00,����8�ı���
		apdu[length + 33] = 0x80;
		for(i = 0;i < (7 - temp);i++)
		{
			apdu[i +length + 34] = 0x00;
		}
		apdu[14] = length + 26 - temp;	//length+5+5+8+8-temp
		apdu[19] = length + 21 - temp;	//8+5+length+8-temp������mac�������ݵĳ���
	}/* end if(g_psam_disperse == 1) */
	else if(g_psam_disperse == 2)
	{
		//�������η�ɢ
		//��һ��ָ���Կ��ɢ
		apdu[0] = 0x15;			    
		apdu[1] = 0x80;
		apdu[2] = 0x1a;
		apdu[3] = 0x46;
		if(type == 0)
		{
			apdu[4] = 0x01;	//����ϵͳ��Ϣ�ļ�
			if (g_OBU_ContractVersion == 0x16)
			{
				for(i = 0; i < 8; i++)
				{
					apdu[i + 6] = g_contract_sn[i];
				}
			}
			else	//һ��OBUдϵͳ��Ϣ��ɢ����ʱ����OBU��λ��Ϣ
			{
				for(i = 0; i < 8; i++)
				{
					apdu[i + 6] = EsamDv[i];
				}
			}
		}
		else
		{
			apdu[4] = 0x02;	//����Ӧ�ó�����Ϣ�ļ�
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
		//�ڶ���ָ�����mac
		apdu[23] = 0x80;
		apdu[24] = 0xfa;
		apdu[25] = 0x05;
		apdu[26] = 0x00;
		//����mac���õ��������ݣ����������+esam��������
		for(i = 0; i < 4; i++)
		{
			apdu[i + 28] = esam_random[i];
			apdu[i + 32] = 0x00;
		}
		//psam�������õ�����Ҫ��esam��������һ��
		apdu[36] = 0x04;
		apdu[37] = 0xd6;
		apdu[38] = 0x81;
		apdu[39] = offset;
		apdu[40] = length + 4;	//length+4,��4��ʾ4���ֽ�mac
		for(i = 0;i < length;i++)
		{
			apdu[i + 41] = file[i];
		}
		temp = (length + 13) % 8;	//����mac�������ݳ���ģ8
		//�¹��ܷ�8��������Ӧ�ò���һ��0x80+0x00,����8�ı���
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
		len = data[3]; //�ڶ�������ķ������ݳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamCalcEsamMac_JuLi()
* ����������	����ESAM���ļ�MAC,����PSAM��
* �����б�	
* ���ؽ����	
*/
int8 psamCalcEsamMac_JuLi(int psam_slot,uint8 offset,uint8 length,uint8 *file,uint8 type)
{
	uint8 apdulist;	//��������
	uint8 apdu[128] = {0};
	int datalist;	//��������
	uint8 data[128] = {0};
	int ret, len, temp, i;
	int pos = 0;
	int syslen = 0;   //����8000֮��ĳ���
	apdulist = 2;
	if(g_psam_disperse == 1)
	{

//		apdu[pos++]= 0x07;//��һ��ָ���
//		apdu[pos++]= 0x00;//��һ��ָ��
//		apdu[pos++]= 0xA4;
//		apdu[pos++]= 0x00;
//		apdu[pos++]= 0x00;
//		apdu[pos++]= 0x02;
//		apdu[pos++]= 0x10;
//		apdu[pos++]= 0x01;

		apdu[pos++]= 0x0d;//�ڶ���ָ���
		apdu[pos++]= 0x80;//�ڶ���ָ��	
		apdu[pos++]= 0x1A;	
		apdu[pos++]= 0x28;	
		apdu[pos++]= 0x0a;//��λ  ϵͳ��Ϣ��08
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
		apdu[pos++]= syslen+13;//������ָ���
		apdu[pos++]= 0x80;//������ָ��
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
		//�������η�ɢ
		//��һ��ָ���Կ��ɢ
		apdu[0] = 0x15;			    
		apdu[1] = 0x80;
		apdu[2] = 0x1a;
		apdu[3] = 0x46;
		if(type == 0)
		{
			apdu[4] = 0x01;	//����ϵͳ��Ϣ�ļ�
			if (g_OBU_ContractVersion == 0x16)
			{
				for(i = 0; i < 8; i++)
				{
					apdu[i + 6] = g_contract_sn[i];
				}
			}
			else	//һ��OBUдϵͳ��Ϣ��ɢ����ʱ����OBU��λ��Ϣ
			{
				for(i = 0; i < 8; i++)
				{
					apdu[i + 6] = EsamDv[i];
				}
			}
		}
		else
		{
			apdu[4] = 0x02;	//����Ӧ�ó�����Ϣ�ļ�
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
		//�ڶ���ָ�����mac
		apdu[23] = 0x80;
		apdu[24] = 0xfa;
		apdu[25] = 0x05;
		apdu[26] = 0x00;
		//����mac���õ��������ݣ����������+esam��������
		for(i = 0; i < 4; i++)
		{
			apdu[i + 28] = esam_random[i];
			apdu[i + 32] = 0x00;
		}
		//psam�������õ�����Ҫ��esam��������һ��
		apdu[36] = 0x04;
		apdu[37] = 0xd6;
		apdu[38] = 0x81;
		apdu[39] = offset;
		apdu[40] = length + 4;	//length+4,��4��ʾ4���ֽ�mac
		for(i = 0;i < length;i++)
		{
			apdu[i + 41] = file[i];
		}
		temp = (length + 13) % 8;	//����mac�������ݳ���ģ8
		//�¹��ܷ�8��������Ӧ�ò���һ��0x80+0x00,����8�ı���
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
		len = data[3]; //�ڶ�������ķ������ݳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamGetKey()
* ����������	 
* �����б�	��
* ���ؽ����	
*/
int psamGetKey(int psam_slot,uint8 P2)
{
	uint8 apdulist;	//��������
	uint8 apdu[128];
	int datalist;	//��������
	uint8 data[128];
	int ret, len, i;
	apdulist = 1;
	apdu[0] = 0x0d;			  //����DF01Ӧ�ü�����Կ���汾1��
	apdu[1] = 0x80;
	apdu[2] = 0xf6;
	apdu[3] = 0x80;
	apdu[4] = P2;
	apdu[5] = 0x08;
	for(i=0;i<8;i++)
	{
//		if((vst.macid[0] == 0x02)&&((vst.sysinfo[9] == 0x00)||(vst.sysinfo[9] == 0x10)))
//		{
//			apdu[i + 6] = EsamDv[i];   //��λ��Ϣ
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
		len = data[0]; //�������ݵĳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamGetRandom()
* ����������	ȡ8�ֽ������ 
* �����б�	��
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int psamGetRandom(int psam_slot)
{
	uint8 apdulist;	//��������
	uint8 apdu[128];
	int datalist;	//��������
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
		len = data[0];	//�������ݵĳ���
		//ע��psam�����ĳ��Ȳ�����9000
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

// У���ESAM����ȡ��Ӧ�ó�����Ϣ 	   ����Э��	 ��ȡ����ĸ����Կ���ģ��������MAC
int8 psamVerifyEsamVehInfo(int psam_slot,uint8 *src,uint8 src_len,uint8 *dst_mac)
{
	uint8 i,len;
	uint16 crc_value;
	uint8 cur_crc[8];

//	src_len = src[1] - 8;  //ȥ��8�ֽ�MAC
	
	crc_value = do_crc(0xffff, src, src_len);
	cur_crc[0] = crc_value&0x00ff;
	cur_crc[1] = crc_value>>8;
	//�Գ�����Ϣ�ļ�����crc��Ȼ���������crc�滻getsecure_rq�з����������ǰ�����ֽ�
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
//			psam_mac[i] = psam_random[i];     //����10�汾OBUֱ�ӷ���PSAM�漴��
//		}
//	}

	//00��10��OBU��У�鳵����Ϣmac
    if(((vst.macid[0] == 0x02)&&(g_OBU_ContractVersion == 0x00||g_OBU_ContractVersion == 0x10))
    		||((vst.macid[0] == 0x03)&&(g_OBU_ContractVersion == 0x00||g_OBU_ContractVersion == 0x10)))
    {
    	for(i=0;i<g_read_file.len[1];i++)
    		esam_read_vehicle[i] = src[i];

    	return SUCCESS;
    }
	//У��
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
// ���ܴ�ESAM����ȡ��Ӧ�ó�����Ϣ 
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

/* ***********************���º���Ӧ����Ӳsam������******************************** */

/*
* �������ƣ�	psamDESInit()
* ����������	���ܳ�ʼ��
* �����б�	��
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int8 psamDESInit(int psam_slot)
{
	int ret;
	uint8 apdulist;	//��������
	uint8 apdu[128];
	int datalist;	//��������
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
	//��VST�еõ�ϵͳ��Ϣ�ļ�	
	for(i = 0;i < 8;i++)
	{
		apdu[len++] = g_contract_sn[i];		 // 8�ֽں�ͬ���к�
	}
	//�����ɢ����
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
		len = data[0];	//�������ݵĳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamDES()
* ����������	����
* �����б�	��
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int8 psamDES(int psam_slot, uint8 *src_file, int src_len, uint8 *dst_file, uint8 *dst_mac)
{
	int ret = SUCCESS;
	uint8 apdulist;	//��������
	uint8 apdu[128];
	int datalist;	//��������
	uint8 data[128];
	uint16 len = 0, i = 0;
	apdulist = 1;
	apdu[len++] = 6 + src_len;
	apdu[len++] = 0x80;
	apdu[len++] = 0xfa;
	apdu[len++] = 0x80;
	apdu[len++] = 0x00;
	apdu[len++] = src_len;	//����Ϊ0x48,���꽻��Ϊ0x20
	//�����ļ�����
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
		len = data[0];	//PSAM�������ݵĳ���
		//ע��psam�����ĳ��Ȳ�����9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			len = data[1];	//����ʵ�����ĳ���,MAC+������Ϣ
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
* �������ƣ�	psamMAC()
* ����������	���ܺ����MAC
* �����б�	��
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int8 psamMAC(int psam_slot,uint8 *rand,uint8 *file,int file_len)
{
	int ret;
	uint8 apdulist = 1;	//��������
	uint8 apdu[128];
	int datalist;	//��������
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
		apdu[len++] = rand[i];	//�����
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
		len = data[0]; //�������ݵĳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamCheckMAC1()
* ����������	У��MAC1C
* �����б�	��
* ���ؽ����	
*/
int psamCheckMAC1(uint8 psam_slot, uint8* payserial, uint8 *rnd, uint8 trans_mode, uint8 *pay_val, uint8 *time)
{
	uint8 apdulist;//��������
	uint8 apdu[128];
	int datalist;//��������
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
	apdu[len++] = rnd[0]; // ���ֽ������
	apdu[len++] = rnd[1];
	apdu[len++] = rnd[2];
	apdu[len++] = rnd[3];
	apdu[len++] = payserial[0]; // 2�ֽ��û����������
	apdu[len++] = payserial[1];
	if(pay_val!=NULL)
	{
		apdu[len++] = pay_val[0]; // ���ѽ��
		apdu[len++] = pay_val[1];
		apdu[len++] = pay_val[2];
		apdu[len++] = pay_val[3];
	} 
	else
	{
		apdu[len++] = 0; // ���ѽ��
		apdu[len++] = 0;
		apdu[len++] = 0;
		apdu[len++] = 0;
	}
	if(trans_mode==0)
	{
		apdu[len++] = 0x06;	//��ʶ�������ͣ���ͳ
	}
	else
	{
		apdu[len++] = 0x09; // ��ʶ�������ͣ�����
	}
	//�������ں�ʱ��
	apdu[len++] = time[0];
	apdu[len++] = time[1];
	apdu[len++] = time[2];
	apdu[len++] = time[3];
	apdu[len++] = time[4];
	apdu[len++] = time[5];
	apdu[len++] = time[6];
	//��Ƭ�ڲ���� ��ɢ����
	apdu[len++] = 0x01;
	apdu[len++] = 0x00;
	apdu[len++] = icc_pib.icc0015[12]; // 8�ֽ��û���Ӧ�����к�
	apdu[len++] = icc_pib.icc0015[13];
	apdu[len++] = icc_pib.icc0015[14];
	apdu[len++] = icc_pib.icc0015[15];
	apdu[len++] = icc_pib.icc0015[16];
	apdu[len++] = icc_pib.icc0015[17];
	apdu[len++] = icc_pib.icc0015[18];
	apdu[len++] = icc_pib.icc0015[19];
	apdu[len++] = icc_pib.icc0015[0];  // 8�ֽ������ɢ����
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
		len = data[0];	//�������ݵĳ���
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamCheckMAC2()
* ����������	У��MAC2
* ���������	
* ���������  ��
* ����ֵ��	0--�ɹ�����0--ʧ��
*/
int psamCheckMAC2(uint8 psam_slot, uint8* mac)
{
	int ret = SUCCESS;
	uint8 apdulist = 1;	//��������
	uint8 apdu[128];
	int datalist;	//��������
	uint8 data[128];
	uint16 len = 0, i = 0;
	apdu[len++] = 9;
	apdu[len++] = 0x80;
	apdu[len++] = 0x72;
	apdu[len++] = 0x00;
	apdu[len++] = 0x00;
	apdu[len++] = 0x04;
	apdu[len++] = mac[0];	//���ֽ������
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
		len = data[0];	//�������ݵĳ���
		//ע��psam�����ĳ��Ȳ�����9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			return SUCCESS;
		} 
		return -2;
	}
	return -3 + ret*10;
}

/*
* �������ƣ�	psamSetKeyindex()
* ����������	������Կ�汾��
* ���������	
* ���������  ��
* ����ֵ��	
*/
int psamSetKeyindex(uint8 psam_slot, uint8 keyindex)
{
	uint8 apdulist;	//��������
	uint8 apdu[128];
	int datalist;	//��������
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
		//ע��psam�����ĳ��Ȳ�����9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			return SUCCESS;
		}	
		return -2;
	}
	return -3 + ret*10;
}

/*
* �������ƣ�	psamAccesscredentials()
* ����������	
* ���������	
* ���������  ��
* ����ֵ��	
*/
int psamAccesscredentials(uint8 psam_slot, const uint8 *contractserialnum, uint8 contractserialnumlen, uint8 keyversionid, const uint8 *Rand, uint8 Randlen)
{
	int i;
	uint8 apdulist;	//��������
	uint8 apdu[128];
	int datalist;	//��������
	uint8 data[128];
	int ret, len, pos, accessCredentialsLen = 17;
	apdulist = 1;
	//����ָ��
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
	for(i = 0; i < contractserialnumlen; i++)	//8�ֽں�ͬ���к�
	{
		apdu[pos++] = contractserialnum[i];
	}
	apdu[pos++] = keyversionid;	//��Կ�汾��
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
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	psamGetChallenge()
* ����������	
* ���������	
* ���������  ��
* ����ֵ��	
*/
int psamGetChallenge(uint8 psam_slot)
{
	int i, ret, len;
	int datalist;	//��������
	uint8 apdulist;	//��������
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
		//ע��psam�����ĳ��Ȳ�����9000
		if((data[len + 1] == 0x90) && (data[len + 2] == 0x00))
		{
			for(i = 0; i < 8; i++)
			{
				psam_pib.randRsuForAuthen[i] = data[i+1];	//������Ϣ����������
			}
			psam_pib.randRsuForAuthenIndex = data[9];	//��������к�
			return SUCCESS;
		} 
		return -2;

	}
	return -3 + ret*10;
}

/*
* �������ƣ�	psamReadBinarayFile()
* ����������	
* �����б�	��
* ���ؽ����	
*/
int psamReadBinarayFile(uint8 psam_slot, uint8 fid,uint8 offset,uint8 length)
{
	uint8 apdulist;	//��������
	uint8 apdu[128];
	int datalist;	//��������
	uint8 data[128];
	int ret, len, i,pos;
	apdulist = 1;
	pos = 0;
	//���ļ�����
	apdu[pos++] = 0x05;	//7,����ETCӦ��Ŀ¼
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
		//ע��psam�����ĳ��Ȳ�����9000
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
* �������ƣ�	PSAM_RESET_rq()
* ����������	psam��λ
* �����б�	��
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int PSAM_RESET_rq(int PSAMSlot,int Baud)
{
	int ret = 0;    
	uint8 chk = 0;
	int cmdtype = 0xf0; //����psam�ۺŵĲ�ͬ����ͬ
	uint32 slen = 0, i = 0;
	uint8 sbuf[BUF_LEN] = {0};
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl; //֡��� 
	cmdtype |= PSAMSlot;	//PSAMSlot = 0x01--�ۺ�1,PSAMSlot=0x02--�ۺ�2��PSAMSlot = 0x03--�ۺ�3
	sbuf[slen++] = cmdtype; //0xf1��0xf2��0xf3
	sbuf[slen++] = 0x00;	//psam��λ0x00
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
	ret = serial_send();	//���õײ㷢�ͺ���
	sem_init(&g_sem_psamreset, 0, 0);
	is_psamreset_recved = 0;	//�յ����߳�ʱ�˳�����������
	return ret;
}

/*
* �������ƣ�	PSAM_RESET_rs()
* ����������	
* �����б�	��
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
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
				is_psamreset_recved = 1;	//�յ����߳�ʱ�˳�����������
				sem_destroy(&g_sem_psamreset);
				return -1;	//��ʱ��û���յ�PSAM�ظ�֡
			}
			usleep(20000);
		}
	}
	is_psamreset_recved = 1;	//�յ����߳�ʱ�˳�����������
	sem_destroy(&g_sem_psamreset);
	pos = 3;	//ȥ��֡��š������0xe3��ԭ������0x01
	//����psamchannel_rs
	if(g_com_rx_buf[pos++] == 1) //�������ݵ���Ч�ԣ�0x00--��Ч��0x01--��Ч,��ֹ��psam�������
	{
		return -2;
	}
	for(i = 0; i < 6; i++)
	{
		PSAMNO[i] = g_com_rx_buf[pos++];	//����PSAM�ն˱��,6���ֽ�
	}
	return SUCCESS; 
}

/*
* �������ƣ�	PSAM_CHANNEL_rq()
* ����������	PSAMͨ��ָ�� 
* �����б�	��
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
*/
int PSAM_CHANNEL_rq(int PSAMSlot, int APDUList, char *APDU) 
{
	int ret = 0;
	uint8 chk = 0;
	uint8 sbuf[BUF_LEN] = {0}; 
	uint16 slen = 0, len_pos = 0, i = 0;
	int cmdtype = 0xf0; //����psam�ۺŵĲ�ͬ����ͬ
	int apdulen = 0;
	sbuf[slen++] = 0xff;
	sbuf[slen++] = 0xff;
	g_rsctl = (g_rsctl + 1) % 16;
	g_rsctl = g_rsctl | 0x00;
	sbuf[slen++] = g_rsctl;	//֡���  
	cmdtype |= PSAMSlot;	//PSAMSlot = 0x01--�ۺ�1,PSAMSlot=0x02--�ۺ�2��PSAMSlot = 0x03--�ۺ�3
	sbuf[slen++] = cmdtype; //0xf1��0xf2��0xf3
	sbuf[slen++] = 0x01;	//psam�Ǹ�λ����0x01
	len_pos = slen++; 		//�����ܳ���λ�� 
	sbuf[slen++] = APDUList;
	//����APDU�ܳ��� 
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
	ret = serial_send();	//���õײ㷢�ͺ���
	sem_init(&g_sem_psamchannel, 0, 0);
	is_psamchannel_recved = 0;	//�յ����߳�ʱ�˳�����������
	return ret;
} 

/*
* �������ƣ�	PSAM_CHANNEL_rs()
* ����������	PSAMͨ��ָ��� 
* �����б�	��
* ���ؽ����	0	--	�ɹ�
*			��0	--	ʧ��
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
				is_psamchannel_recved = 1;	//�յ����߳�ʱ�˳�����������
				sem_destroy(&g_sem_psamchannel);
				//DEBUGP("PSAM_CHANNEL_rs TIME OUT ERR, ret = %d\n", ret);
				return -1;	//��ʱ��û���յ�vst
			}
			usleep(20000);
		}
	}
	is_psamchannel_recved = 1;	//�յ����߳�ʱ�˳�����������
	sem_destroy(&g_sem_psamchannel);
	pos = 3;	//ȥ��֡��š������0xe3��ԭ������0x01
	//����psamchannel_rs
	if(g_com_rx_buf[pos++] == 1)	//�������ݵ���Ч�ԣ�0x00--��Ч��0x01--��Ч,��ֹ��psam�������
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


