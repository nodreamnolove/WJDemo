package wj.ble.obu.comjar;

import wj.ble.obu.comjar.WJGlobalVar;


public class WJIssue_Ble {
//	
//	/**
//	 * init_C0() ��ʼ��C0֡
//	 * @param prog_comm_c0
//	 * @return
//	 */
//	static public native int init_C0_Ble(WJGlobalVar.PROG_COMM_C0 prog_comm_c0);
//
//	/** init_C1() ��ʼ��C1֡
//	 * @param prog_comm_c1
//	 * @return
//	 */
//	public static native int init_C1_Ble(WJGlobalVar.PROG_COMM_C1 prog_comm_c1);
//
//	/**init_C2() ��ʼ��C2֡
//	 * @param prog_comm_c2
//	 * @return
//	 */
//	public static native int init_C2_Ble(WJGlobalVar.PROG_COMM_C2 prog_comm_c2);
//
//	/**init_C3() ��ʼ��C3֡
//	 * @param prog_comm_c3
//	 * @return
//	 */
//	public static native int init_C3_Ble(WJGlobalVar.PROG_COMM_C3 prog_comm_c3);
//
//	/** ��ʼ��C4֡ 
//	 * @param prog_comm_c4
//	 * @param nType ��ʼ��C4֡������ 
//	 *   0x01:��ϵͳ��Ϣ
//	 *   0x02:��������Ϣ 
//	 *   0x03:��ϵͳ��Ϣ+��������Ϣ 
//	 *   0x10:��ȡIC����� 
//	 *   0x12:��ȡ0012�ļ� 
//	 *   0x15:��ȡ0015�ļ� 
//	 *   0x19:��ȡ0019�ļ�
//	 *   ����:��ϵͳ��Ϣ
//	 * @return
//	 */
//	public static native int init_C4_Ble(WJGlobalVar.PROG_COMM_C4 prog_comm_c4, int nType);
	
	// *****************************����OBU���в����ӿ�********************/
	/**
	 * ��������OBU��ʹ����OBU�����������״̬
	 * 
	 * @param prog_c0ԭ��ʼ��715�ṹ��
	 *            ���ù�������ʵ�����壬
	 * @return
	 */
	static public native int send_c0_Ble(WJGlobalVar.PROG_COMM_C0 prog_c0);

	static public native int recv_b0_Ble(WJGlobalVar.PROG_COMM_B0 prog_b0,
			int time_out);
	static public native int SetTimeOutMax(int TimeOutMax);
	static public native int SetTimeOutCnt(int TimeOutCnt);
	/**
	 * ����BST������OBU
	 * 
	 * @param prog_c1
	 * @return
	 */
	static public native int send_c1_Ble(WJGlobalVar.PROG_COMM_C1 prog_c1);

	/**
	 * ����VST
	 * 
	 * @param prog_b1
	 *            B1 -- VST��Ӧ��Ϣ֡
	 * @return
	 */
	static public native int recv_b1_Ble(WJGlobalVar.PROG_COMM_B1 prog_b1,
			int time_out);

	/**
	 * ��Կ�滻��һ�η������õ�
	 * 
	 * @param prog_c2
	 *            C2 -- ��Կ�滻ָ��֡
	 * @return
	 */
	static public native int send_c2_Ble(WJGlobalVar.PROG_COMM_C2 prog_c2,
			int time_out);

	/**
	 * �ļ����£����η��С����߶�ESAM��IC����Ϣʱ���õ�
	 * 
	 * @param prog_c3
	 * @return
	 */
	static public native int send_c3_Ble(WJGlobalVar.PROG_COMM_C3 prog_c3,
			int time_out);

	/**
	 * �ļ����µĻظ����
	 * 
	 * @param prog_b2
	 *            B2 -- ��������ִ����Ӧ֡
	 * @return
	 */
	static public native int recv_b2_Ble(WJGlobalVar.PROG_COMM_B2 prog_b2);

	/**
	 * ��������ָ��,����SETMMI_RQָ�������OBU����ʾ��ʾ��Ϣ
	 * 
	 * @param prog_c5
	 *            C5 -- ��������ָ��
	 * @return
	 */
	static public native int send_c5_Ble(WJGlobalVar.PROG_COMM_C5 prog_c5);

	/**
	 * ��������ָ��Ļظ�ָ��,����SETMMI_RSָ��(����OBU�ظ��Ľ�������ָ����Ӧָ��) ������EVENTREPORT ������·����
	 * 
	 * @param prog_b4
	 * @param time_out
	 * @return
	 */
	static public native int recv_b4_Ble(WJGlobalVar.PROG_COMM_B4 prog_b4,
			int time_out);

	/**
	 * ��ʾ��Ϣָ��,����SETMMI_RQָ�������OBU����ʾ��ʾ��Ϣ��
	 * �ú�����SETMMI֡�ṹ�е�Paramter�ֶ�����Ϊ0x10�����ڲ����������20���ֽڵ���֤֡��Ϣ�����Ȳ�����油0x00��
	 * 
	 * @param prog_c5
	 *            C5 -- ��������ָ��
	 * @param IdentInfo
	 *            ��֤��Ϣ��OBU��ʾ����Ϣ
	 * @return
	 */
	static public native int send_c5_Ble_new(WJGlobalVar.PROG_COMM_C5 prog_c5,
			byte[] IdentInfo);

	/**
	 * ����SETMMI_RSָ��(����OBU�ظ��Ľ�������ָ����Ӧָ��)���ú����в���������EVENTREPORT
	 * 
	 * @param prog_b4
	 * @param time_out
	 * @return
	 */
	static public native int recv_b4_Ble_new(WJGlobalVar.PROG_COMM_B4 prog_b4,
			int time_out);

	// // ��ȡOBU SN��
	// static public native int send_c6_Ble(int TimeOut);
	//
	// static public native int recv_b6_Ble(byte[] EsamSerialNum);

	/**
	 * д������Ϣ��������Ӧ���ն���obu��
	 * 
	 * @param prog_c4
	 *            C4 -- �ļ���ȡָ��
	 * @param time_out
	 *            �ȴ���ʱʱ��
	 * @return
	 */
	static public native int send_c7_Ble(WJGlobalVar.PROG_COMM_C4 prog_c4,
			int time_out);

	/**
	 * @param prog_b3
	 * @return
	 */
	static public native int recv_b7_Ble(WJGlobalVar.PROG_COMM_B3 prog_b3);

	/**
	 * ��������Ϣ��������Ӧ���ն���obu��
	 * 
	 * @param prog_c4
	 *            C4 -- �ļ���ȡָ��
	 * @param time_out
	 *            �ȴ���ʱʱ��
	 * @return
	 */
	static public native int send_c8_Ble(WJGlobalVar.PROG_COMM_C4 prog_c4,
			int time_out);

	/**
	 * @param prog_b3 -- OBU�ļ���Ϣ֡
	 * @return
	 */
	static public native int recv_b8_Ble(WJGlobalVar.PROG_COMM_B3 prog_b3);

	/**
	 * ��ϵͳ��Ϣ��������Ӧ���ն���obu��
	 * 
	 * @param prog_c4
	 *            C4 -- �ļ���ȡָ��
	 * @param time_out
	 *            �ȴ���ʱʱ��
	 * @return
	 */
	static public native int send_c9_Ble(WJGlobalVar.PROG_COMM_C4 prog_c4,
			int time_out);
	
	static public native int send_c90018file_Ble(WJGlobalVar.PROG_COMM_C4 prog_c4,byte[] pinCode,
			int time_out);
	
	static public native int send_c90016file_Ble(WJGlobalVar.PROG_COMM_C4 prog_c4,byte[] pinCode,
			int time_out);
	
	/**
	 * @param prog_b3 -- OBU�ļ���Ϣ֡
	 * @return
	 */
	static public native int recv_b9_Blefile(WJGlobalVar.PROG_COMM_B3 prog_b3,byte DidFid);
	static public native int recv_b9_Ble(WJGlobalVar.PROG_COMM_B3 prog_b3);
	static public native int recv_b9list_Ble(WJGlobalVar.PROG_COMM_B3LIST prog_b3list);

	/**
	 * ��ESAMĿ¼��������
	 * 
	 * @param dir
	 *            Ŀ¼
	 * @param time_out
	 *            �ȴ���ʱʱ��
	 * @return
	 */
	static public native int Enter_EsamDir(int dir, int time_out);

	/**
	 * ��ȡ���׼�¼����
	 * 
	 * @param TradeInfo
	 *            ������Ϣ
	 * @param record_num
	 *            ���׼�¼���
	 * @param TimeOut
	 *            �ȴ���ʱʱ��
	 * @return
	 */
	static public native int send_c10_Ble(byte[] TradeInfo, int record_num,
			int TimeOut);

	/**
	 * ʵ��Ȧ�湦��
	 * 
	 * @param UserName
	 *            �û���
	 * @param money
	 *            Ȧ����~
	 * @param miyaotype
	 *            ��Կ����
	 * @param miyaoFlag
	 *            ��Կ��־
	 * @param TimeOut
	 *            �ȴ���ʱʱ��
	 * @return
	 */
	static public native int send_c11_Ble(byte[] UserName, byte[] money, int miyaotype,
			int miyaoFlag, int TimeOut);

	/**
	 * ��ֵָ��
	 * 
	 * @param BankICcardNum
	 *            ���п���
	 * @param money
	 *            ��ֵ���
	 * @param UserName
	 *            �û���
	 * @return
	 */
	static public native int send_c12_Ble(byte[] BankICcardNum, byte[] money,
			byte[] UserName);

	/**
	 * @param pcurBalance
	 *            ��ǰ�˻�ʣ����
	 * @param TimeOut
	 *            �ȴ���ʱʱ��
	 * @return
	 */
	static public native int recv_b12_Ble(byte[] pcurBalance, int TimeOut);

	/**
	 * ��̨�û���½
	 * 
	 * @param UserName
	 *            ��̨�˻���
	 * @param UserNameLen
	 *            ��̨�˻�������
	 * @param UserPassword
	 *            ��̨�˻�����
	 * @param UserPasswordLen
	 *            ��̨�˻����볤��
	 * @param TimeOut
	 *            �ȴ���ʱʱ��
	 * @return
	 */
	static public native int send_c13_Ble(byte[] UserName, int UserNameLen,
			byte[] UserPassword, int UserPasswordLen, int TimeOut);

	/**
	 * �춨���кϷ���֡������ͼƬ�����֣��ϴ�����ͼƬ��
	 * 
	 * @param ChkPhotoName
	 *            ��Ҫ�ϴ���ͼƬ������
	 * @param ChePhotoNameLen
	 *            ��Ҫ�ϴ���ͼƬ�����ֵĳ���
	 * @param ChkInfo
	 *            У����Ϣ
	 * @param ChkInfoLen
	 *            У����Ϣ�ĳ���
	 * @param TimeOut
	 *            �ȴ���ʱʱ��
	 * @return
	 */
	static public native int send_c14_Ble(byte[] ChkPhotoName,
			int ChePhotoNameLen, byte[] ChkInfo, int ChkInfoLen, int TimeOut);

	/**
	 * �춨���кϷ���֡���ϴ�ͼ���֡����֡��ÿ֡��1005�ֽڣ���5�ֽ�֡ͷ��1000�ֽ�ͼƬ���ݣ���
	 * 
	 * @param PhotoDataBuf
	 *            ͼƬ���ݽضϵĴ���������
	 * @param PhotoDataBufLen
	 *            ���������鳤��
	 * @param FrameIndex
	 *            ���ضϵ�֡���
	 * @param TimeOut
	 *            �ȴ���ʱʱ��
	 * @return
	 */
	static public native int send_c15_Ble(byte[] PhotoDataBuf,
			int PhotoDataBufLen, int FrameIndex);	
	static public native int recv_b15_Ble(int TimeOut);

	/**
	 * ����Ϊ�ͷ���·ָ��
	 * 
	 * @param event_type
	 *            �ͼ�ָ���и�ֵ�涨Ϊ0
	 * @param ant_id
	 *            �ڱ��������������и�ֵ�������� ������ģʽ��ֵ����Ϊ0����
	 * @return
	 */
	static public native int send_cc_EVENT_REPORT(byte event_type, byte ant_id);
	
	
	/**
	 * ����Ϊ ���ƽӿ�
	 * 
	 * @param channelid
	 *            5�� ǰ�ƣ�6�����
	 * @param flashCount
	 *            ���ƴ���
	 * @param time_out
	 *            ��ʱʱ�䣬����100ms
	 * @return
	 */
	static public native int send_c16_Ble(int channelid,int flashCount, int time_out);
	 
	/**
	 * ����Ϊ ���Ʒ��ؽӿ�
	 * 
	 * @param time_out
	 *            ��ʱʱ�䣬����10s����
	 * @return 0���ɹ��� <0:ʧ��
	 */	
	static public native int recv_b16_Ble( int time_out);
	
	/**
	 * ����Ϊ �����·��ؽӿ�
	 * 
	 * @param time_out
	 *            ��ʱʱ�䣬����10s����
	 * @return 0:����λ���� 1������λ����  <0������
	 */	
	static public native int getWriteSysInfo_Rq_Ble(WJGlobalVar.FrameWriteSysInfoRq data_rq);
	static public native int setWriteSysInfo_Rs_Ble(WJGlobalVar.FrameWriteSysInfoRs data_rs);
	static public native int recv_b17_Ble( int time_out);
	static public native int getQuanCun_Rq_Ble(WJGlobalVar.FrameQuanCunRq data_rq);
	static public native int setQuanCun_Rs_Ble(WJGlobalVar.FrameQuanCunRs data_rs, byte[] UserName, byte[] money, int miyaotype,
			int miyaoFlag, int TimeOut);
	static public native int getQuanCunInit_Rq_Ble(WJGlobalVar.FrameQuanCunInitRq data_rq,byte[] UserName, byte[] money, int miyaotype,
			int miyaoFlag, int TimeOut);
	static public native int setQuanCunInit_Rs_Ble(WJGlobalVar.FrameQuanCunInitRs data_rs,byte[] UserName, byte[] money, int miyaotype,
			int miyaoFlag, int TimeOut);
	static public native int getFrameReadVehInfo_Rq_Ble(WJGlobalVar.FrameReadVehInfoRq data_rq,WJGlobalVar.PROG_COMM_C4 prog_c4_data);
	static public native int setFrameReadVehInfo_Rs_Ble(WJGlobalVar.FrameReadVehInfoRs data_rs,WJGlobalVar.PROG_COMM_C4 prog_c4_data);
	static public native int sendC2Java(bleService bleservice);
	
	static public native int jni2loadCreditGetMac1(byte[] cardId, int credit,byte[] terminalNo, byte[] pinCode, int procType, int keyIndex,WJGlobalVar.loadCreditGetMac1Ret data_ret);
	static public native int jniloadCreditWriteCard(byte[] mac2);
	static public native int readCardConsumeRecord(int maxNumber,WJGlobalVar.CardConsumeRecordListRet listret);
	static native int cardCommand_Recv(WJGlobalVar.PROG_COMM_DATA recv_data,int TimeOut);
}
