package wj.ble.obu.comjar;

import wj.ble.obu.comjar.WJGlobalVar;


public class WJIssue_Ble {
//	
//	/**
//	 * init_C0() 初始化C0帧
//	 * @param prog_comm_c0
//	 * @return
//	 */
//	static public native int init_C0_Ble(WJGlobalVar.PROG_COMM_C0 prog_comm_c0);
//
//	/** init_C1() 初始化C1帧
//	 * @param prog_comm_c1
//	 * @return
//	 */
//	public static native int init_C1_Ble(WJGlobalVar.PROG_COMM_C1 prog_comm_c1);
//
//	/**init_C2() 初始化C2帧
//	 * @param prog_comm_c2
//	 * @return
//	 */
//	public static native int init_C2_Ble(WJGlobalVar.PROG_COMM_C2 prog_comm_c2);
//
//	/**init_C3() 初始化C3帧
//	 * @param prog_comm_c3
//	 * @return
//	 */
//	public static native int init_C3_Ble(WJGlobalVar.PROG_COMM_C3 prog_comm_c3);
//
//	/** 初始化C4帧 
//	 * @param prog_comm_c4
//	 * @param nType 初始化C4帧的类型 
//	 *   0x01:读系统信息
//	 *   0x02:读车辆信息 
//	 *   0x03:读系统信息+读车辆信息 
//	 *   0x10:读取IC卡余额 
//	 *   0x12:读取0012文件 
//	 *   0x15:读取0015文件 
//	 *   0x19:读取0019文件
//	 *   其他:读系统信息
//	 * @return
//	 */
//	public static native int init_C4_Ble(WJGlobalVar.PROG_COMM_C4 prog_comm_c4, int nType);
	
	// *****************************蓝牙OBU发行操作接口********************/
	/**
	 * 唤醒蓝牙OBU，使蓝牙OBU进入接收数据状态
	 * 
	 * @param prog_c0原初始化715结构体
	 *            ，该功能中无实际意义，
	 * @return
	 */
	static public native int send_c0_Ble(WJGlobalVar.PROG_COMM_C0 prog_c0);

	static public native int recv_b0_Ble(WJGlobalVar.PROG_COMM_B0 prog_b0,
			int time_out);
	static public native int SetTimeOutMax(int TimeOutMax);
	static public native int SetTimeOutCnt(int TimeOutCnt);
	/**
	 * 发送BST，查找OBU
	 * 
	 * @param prog_c1
	 * @return
	 */
	static public native int send_c1_Ble(WJGlobalVar.PROG_COMM_C1 prog_c1);

	/**
	 * 接收VST
	 * 
	 * @param prog_b1
	 *            B1 -- VST响应信息帧
	 * @return
	 */
	static public native int recv_b1_Ble(WJGlobalVar.PROG_COMM_B1 prog_b1,
			int time_out);

	/**
	 * 密钥替换，一次发行中用到
	 * 
	 * @param prog_c2
	 *            C2 -- 密钥替换指令帧
	 * @return
	 */
	static public native int send_c2_Ble(WJGlobalVar.PROG_COMM_C2 prog_c2,
			int time_out);

	/**
	 * 文件更新，二次发行、或者读ESAM、IC卡信息时中用到
	 * 
	 * @param prog_c3
	 * @return
	 */
	static public native int send_c3_Ble(WJGlobalVar.PROG_COMM_C3 prog_c3,
			int time_out);

	/**
	 * 文件更新的回复结果
	 * 
	 * @param prog_b2
	 *            B2 -- 更新命令执行响应帧
	 * @return
	 */
	static public native int recv_b2_Ble(WJGlobalVar.PROG_COMM_B2 prog_b2);

	/**
	 * 结束交易指令,发送SETMMI_RQ指令，用于在OBU上显示提示信息
	 * 
	 * @param prog_c5
	 *            C5 -- 结束交易指令
	 * @return
	 */
	static public native int send_c5_Ble(WJGlobalVar.PROG_COMM_C5 prog_c5);

	/**
	 * 结束交易指令的回复指令,接收SETMMI_RS指令(接收OBU回复的结束交易指令响应指令) 并发送EVENTREPORT 结束链路连接
	 * 
	 * @param prog_b4
	 * @param time_out
	 * @return
	 */
	static public native int recv_b4_Ble(WJGlobalVar.PROG_COMM_B4 prog_b4,
			int time_out);

	/**
	 * 提示信息指令,发送SETMMI_RQ指令，用于在OBU上显示提示信息，
	 * 该函数在SETMMI帧结构中的Paramter字段设置为0x10，并在参数后添加了20个字节的验证帧信息（长度不足后面补0x00）
	 * 
	 * @param prog_c5
	 *            C5 -- 结束交易指令
	 * @param IdentInfo
	 *            认证信息，OBU显示该信息
	 * @return
	 */
	static public native int send_c5_Ble_new(WJGlobalVar.PROG_COMM_C5 prog_c5,
			byte[] IdentInfo);

	/**
	 * 接收SETMMI_RS指令(接收OBU回复的结束交易指令响应指令)，该函数中不包含发送EVENTREPORT
	 * 
	 * @param prog_b4
	 * @param time_out
	 * @return
	 */
	static public native int recv_b4_Ble_new(WJGlobalVar.PROG_COMM_B4 prog_b4,
			int time_out);

	// // 读取OBU SN号
	// static public native int send_c6_Ble(int TimeOut);
	//
	// static public native int recv_b6_Ble(byte[] EsamSerialNum);

	/**
	 * 写车辆信息的请求与应答（终端与obu）
	 * 
	 * @param prog_c4
	 *            C4 -- 文件读取指令
	 * @param time_out
	 *            等待超时时间
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
	 * 读车辆信息的请求与应答（终端与obu）
	 * 
	 * @param prog_c4
	 *            C4 -- 文件读取指令
	 * @param time_out
	 *            等待超时时间
	 * @return
	 */
	static public native int send_c8_Ble(WJGlobalVar.PROG_COMM_C4 prog_c4,
			int time_out);

	/**
	 * @param prog_b3 -- OBU文件信息帧
	 * @return
	 */
	static public native int recv_b8_Ble(WJGlobalVar.PROG_COMM_B3 prog_b3);

	/**
	 * 读系统信息的请求与应答（终端与obu）
	 * 
	 * @param prog_c4
	 *            C4 -- 文件读取指令
	 * @param time_out
	 *            等待超时时间
	 * @return
	 */
	static public native int send_c9_Ble(WJGlobalVar.PROG_COMM_C4 prog_c4,
			int time_out);
	
	static public native int send_c90018file_Ble(WJGlobalVar.PROG_COMM_C4 prog_c4,byte[] pinCode,
			int time_out);
	
	static public native int send_c90016file_Ble(WJGlobalVar.PROG_COMM_C4 prog_c4,byte[] pinCode,
			int time_out);
	
	/**
	 * @param prog_b3 -- OBU文件信息帧
	 * @return
	 */
	static public native int recv_b9_Blefile(WJGlobalVar.PROG_COMM_B3 prog_b3,byte DidFid);
	static public native int recv_b9_Ble(WJGlobalVar.PROG_COMM_B3 prog_b3);
	static public native int recv_b9list_Ble(WJGlobalVar.PROG_COMM_B3LIST prog_b3list);

	/**
	 * 进ESAM目录单独操作
	 * 
	 * @param dir
	 *            目录
	 * @param time_out
	 *            等待超时时间
	 * @return
	 */
	static public native int Enter_EsamDir(int dir, int time_out);

	/**
	 * 读取交易记录操作
	 * 
	 * @param TradeInfo
	 *            交易信息
	 * @param record_num
	 *            交易记录序号
	 * @param TimeOut
	 *            等待超时时间
	 * @return
	 */
	static public native int send_c10_Ble(byte[] TradeInfo, int record_num,
			int TimeOut);

	/**
	 * 实现圈存功能
	 * 
	 * @param UserName
	 *            用户名
	 * @param money
	 *            圈存金額
	 * @param miyaotype
	 *            秘钥类型
	 * @param miyaoFlag
	 *            秘钥标志
	 * @param TimeOut
	 *            等待超时时间
	 * @return
	 */
	static public native int send_c11_Ble(byte[] UserName, byte[] money, int miyaotype,
			int miyaoFlag, int TimeOut);

	/**
	 * 充值指令
	 * 
	 * @param BankICcardNum
	 *            银行卡号
	 * @param money
	 *            充值金额
	 * @param UserName
	 *            用户名
	 * @return
	 */
	static public native int send_c12_Ble(byte[] BankICcardNum, byte[] money,
			byte[] UserName);

	/**
	 * @param pcurBalance
	 *            当前账户剩余金额
	 * @param TimeOut
	 *            等待超时时间
	 * @return
	 */
	static public native int recv_b12_Ble(byte[] pcurBalance, int TimeOut);

	/**
	 * 后台用户登陆
	 * 
	 * @param UserName
	 *            后台账户名
	 * @param UserNameLen
	 *            后台账户名长度
	 * @param UserPassword
	 *            后台账户密码
	 * @param UserPasswordLen
	 *            后台账户密码长度
	 * @param TimeOut
	 *            等待超时时间
	 * @return
	 */
	static public native int send_c13_Ble(byte[] UserName, int UserNameLen,
			byte[] UserPassword, int UserPasswordLen, int TimeOut);

	/**
	 * 检定发行合法性帧（根据图片的名字，上传整个图片）
	 * 
	 * @param ChkPhotoName
	 *            需要上传的图片的名字
	 * @param ChePhotoNameLen
	 *            需要上传的图片的名字的长度
	 * @param ChkInfo
	 *            校验信息
	 * @param ChkInfoLen
	 *            校验信息的长度
	 * @param TimeOut
	 *            等待超时时间
	 * @return
	 */
	static public native int send_c14_Ble(byte[] ChkPhotoName,
			int ChePhotoNameLen, byte[] ChkInfo, int ChkInfoLen, int TimeOut);

	/**
	 * 检定发行合法性帧（上传图像的帧，单帧，每帧长1005字节（含5字节帧头和1000字节图片内容））
	 * 
	 * @param PhotoDataBuf
	 *            图片数据截断的待传输数组
	 * @param PhotoDataBufLen
	 *            待传输数组长度
	 * @param FrameIndex
	 *            被截断的帧序号
	 * @param TimeOut
	 *            等待超时时间
	 * @return
	 */
	static public native int send_c15_Ble(byte[] PhotoDataBuf,
			int PhotoDataBufLen, int FrameIndex);	
	static public native int recv_b15_Ble(int TimeOut);

	/**
	 * 以下为释放链路指令
	 * 
	 * @param event_type
	 *            送检指南中该值规定为0
	 * @param ant_id
	 *            在北京自由流交易中该值才有意义 ，其他模式该值设置为0即可
	 * @return
	 */
	static public native int send_cc_EVENT_REPORT(byte event_type, byte ant_id);
	
	
	/**
	 * 以下为 闪灯接口
	 * 
	 * @param channelid
	 *            5： 前灯，6：后灯
	 * @param flashCount
	 *            闪灯次数
	 * @param time_out
	 *            超时时间，初定100ms
	 * @return
	 */
	static public native int send_c16_Ble(int channelid,int flashCount, int time_out);
	 
	/**
	 * 以下为 闪灯返回接口
	 * 
	 * @param time_out
	 *            超时时间，初定10s以内
	 * @return 0：成功， <0:失败
	 */	
	static public native int recv_b16_Ble( int time_out);
	
	/**
	 * 以下为 防拆按下返回接口
	 * 
	 * @param time_out
	 *            超时时间，初定10s以内
	 * @return 0:防拆位按下 1：防拆位弹起  <0：错误
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
