package wj.ble.obu.comjar;

import java.text.DecimalFormat;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.TimeUnit;

import wj.ble.common.CommonFunct;
import wj.ble.issuelib.Variables;
import wj.ble.obu.comjar.WJStructs.CardConsumeRecord;
import wj.ble.obu.comjar.WJStructs.CardOwner;
import wj.ble.obu.comjar.WJStructs.CardTransactionRecord;
import wj.ble.obuissue.R;
import Parse.MyParse;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

public class wjOBU {
	// 蓝牙配置程序相关
	public static BluetoothDevice mDevice = null;
	public static BluetoothAdapter mBluetoothAdapter = null;
	private static final long SCAN_PERIOD = 10000;
	public static final int REQUEST_SELECT_DEVICE = 1;// 蓝牙设备列表的事件回复
	public static final int REQUEST_ENABLE_BT = 2;// 提示打开蓝牙设备的事件回复
	public static int m_timeOutSecond = 60;// 超时时间20s
	// Context context;
	// public wjOBU(Context context) {
	// this.context = context;
	// }
	private boolean mScanning;

	// private ServiceConnection mServiceConnection = new ServiceConnection() {
	// public void onServiceConnected(ComponentName className,IBinder rawBinder)
	// {
	//
	// Variables.mService = ((bleService.LocalBinder) rawBinder).getService();
	//
	// Log.i("123", "连接service");
	// if (Variables.mService.initialize() != 0) {
	// Log.e(Variables.TAG, "Unable to initialize Bluetooth");
	// return;
	// }
	//
	// }
	//
	// public void onServiceDisconnected(ComponentName classname) {
	// Variables.mService = null;
	// }
	// };
	public ServiceStatus connectDevice() {

		bleService l_t = new bleService();
		ServiceStatus l_ret = new ServiceStatus();
//		Threadscan thread = new Threadscan();
//		thread.start();
//		try{
//			thread.join();
//		}catch(InterruptedException ex){
//			;
//		}
		
		//Log.v("connectDevice", "step 1");
		Log.v("wjjar.log","connectDevice begin");
		if (WJVariables.mService != null) {
//			Log.v("connectDevice", "step 1.1");
//			l_ret.ServiceInfo = "WJVariables.mService != null error";
//			l_ret.ServiceCode = -1;
//			l_ret.ObuCode = l_ret.ServiceCode;
//			l_ret.ObuInfo = l_ret.ServiceInfo;
//			Log.v("wjjar.log","connectDevice begin:WJVariables.mService != null");
//			return l_ret;

		    // msgHandler_dtcpuinf = new MsgHandler_Detection_CPUCard(this);
			if (bleCommonVariable.mDevice != null) {
				  disconnectDevice();
			   }
		}
		Log.v("connectDevice", "step 1.2");
		if (bleCommonVariable.mDevice != null) {
			l_ret.ServiceInfo = "bleCommonVariable.mDevice != null error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			Log.v("wjjar.log","connectDevice stop:bleCommonVariable.mDevice != null error");
			return l_ret;
		}
//		Log.v("connectDevice", "step 2");
//		bleCommonVariable.mDevice = null;
//		WJVariables.mService = null;

		// if (!context.getPackageManager().hasSystemFeature(
		// PackageManager.FEATURE_BLUETOOTH_LE)) {
		// return l_ret;
		// //Toast.makeText(this, R.string.ble_not_supported,
		// Toast.LENGTH_SHORT).show();
		// //context.finish();
		// }
		//

		// Intent bindIntent = new Intent(context, bleService.class);
		// if(!context.bindService(bindIntent, mServiceConnection,
		// Context.BIND_AUTO_CREATE))
		// {
		// return l_ret;
		// }

		// 初始化蓝牙设备
		// final BluetoothManager bluetoothManager = (BluetoothManager)
		// context.getSystemService(Context.BLUETOOTH_SERVICE);
		// mBluetoothAdapter = bluetoothManager.getAdapter();
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		if (!mBluetoothAdapter.isEnabled()) {
			//Log.v("connectDevice", "step 3");
			Log.v("wjjar.log","connectDevice Enable mBluetoothAdapter");
			mBluetoothAdapter.enable();
		}
		bleCommonVariable.mBluetoothAdapter = mBluetoothAdapter;
		//Log.v("connectDevice", "step 4");
		WJVariables.mService = l_t;
		// WJVariables.mService = new bleService();
		// l_ret.ServiceCode = WJVariables.mService.initialize();
//		Log.v("connectDevice", "step 5");
//		if (l_ret.ServiceCode != 0) {
//			l_ret.ServiceInfo = "蓝牙无法打开";
//			Log.v("connectDevice", "failed: can not open ble");
//			Log.v("wjjar.log","connectDevice stop mBluetoothAdapter");
//		}
		// 扫描蓝牙设备
		if (mScanning == false) {

			new Handler().postDelayed(new Runnable() {
				@Override
				public void run() {
					if (mScanning == true) {
						mScanning = false;
						if (WJVariables.mService != null)
						{
							//Log.v("connectDevice", "stopscan");
							Log.v("wjjar.log","connectDevice stop scanDevice at postDelayed");
							WJVariables.mService.scanLeDevice(false);						
						}
					}
				}
			}, m_timeOutSecond*1000);
//			Threadscan thread = new Threadscan();
//			thread.start();
//			new Handler().postDelayed(new Runnable() {
//				@Override
//				public void run() {
//					mScanning = true;
//					WJVariables.mService.scanLeDevice(true);
//				}
//			}, 0);
			mScanning = true;
			//Log.v("connectDevice", "step 6");
			Log.v("wjjar.log","connectDevice begin scanDevice");
			WJVariables.mService.scanLeDevice(true);
		}

		try {
			boolean bret = WJVariables.semWait.tryAcquire(m_timeOutSecond*1000, TimeUnit.MILLISECONDS);
			if(bret == false)
			{
				if (mScanning == true) {
					mScanning = false;
					//Log.v("connectDevice", "stopscan");
					Log.v("wjjar.log","connectDevice stop scanDevice at tryAcquire timeout");
					WJVariables.mService.scanLeDevice(false);
					WJVariables.mService = null;
					bleCommonVariable.mDevice = null;
					l_ret.ServiceCode = -1;
					l_ret.ServiceInfo = "蓝牙未连接";
					l_ret.ObuCode = l_ret.ServiceCode;
					l_ret.ObuInfo = l_ret.ServiceInfo;
					return l_ret;
				}
			}

		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		int cnt = 0;
//		while(WJVariables.scanstate == 0){
//			
//			try {
//				//bleCommonVariable.mBluetoothAdapter.
//				Thread.sleep(1000);
//				cnt++;
//			} catch (InterruptedException e) {
//				e.printStackTrace();
//				break;
//			}
//			if(cnt >= m_timeOutSecond){
//				break;
//			}
//		}
		WJVariables.scanstate = 0;
		// 延迟闪灯
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		if (WJVariables.mService != null && bleCommonVariable.mDevice != null) {
			if (0 != WJVariables.mService.connect()) {
				// if(mScanning == true)
				// {
				// mScanning = false;
				// WJVariables.mService.scanLeDevice(false);
				// }
				Log.v("wjjar.log","connectDevice stop scanDevice at can not connect");
				WJVariables.mService = null;
				mBluetoothAdapter = null;
			} else {
				Log.v("wjjar.log","connectDevice success");
				if (mScanning == true) {
					mScanning = false;
					//Log.v("connectDevice", "stopscan");
					Log.v("wjjar.log","connectDevice stop scanDevice after connect");
					WJVariables.mService.scanLeDevice(false);
					l_ret.ServiceCode = 0;
					l_ret.ServiceInfo = "蓝牙连接成功";
					l_ret.ObuCode = l_ret.ServiceCode;
					l_ret.ObuInfo = l_ret.ServiceInfo;
				}
			}
		}
		else{
				WJVariables.mService = null;
				bleCommonVariable.mDevice = null;
		}

		//Log.v("connectDevice", "success");
		Log.v("wjjar.log","connectDevice end");
		return l_ret;
	}

	public ServiceStatus disconnectDevice() {
		Log.v("wjjar.log","disconnectDevice begin");
		if (mScanning == true) {
			mScanning = false;
			Log.v("wjjar.log","disconnectDevice stop scanDevice at disconnectDevice");
			WJVariables.mService.scanLeDevice(false);
		}
		ServiceStatus l_ret = new ServiceStatus();
		WJVariables.mService.disconnect();
		
		//bleCommonVariable.mDevice = null;
		WJVariables.scanstate = 0;
		//WJVariables.mService = null;
		l_ret.ServiceCode = 0;
		l_ret.ServiceInfo = "蓝牙断开";
		l_ret.ObuCode = l_ret.ServiceCode;
		l_ret.ObuInfo = l_ret.ServiceInfo;
		Log.v("wjjar.log","disconnectDevice success");
		return l_ret;
	}

	public int sendC2Java(byte[] dataBuf, int nDataBufLen) {

		//Log.v("nDataBufLen", "" + nDataBufLen);
		byte[] aa = new byte[nDataBufLen];
		System.arraycopy(dataBuf, 0, aa, 0, nDataBufLen);
		Log.v("sendC2Java data", MyParse.Frame2String(dataBuf));
		//bleService l_u = new bleService();
		int ret = 0;
		int sendtime = 3;
		while (sendtime >= 0) {
			if (0 == (ret = WJVariables.mService.writeRXCharacteristic(aa))) {
				//Log.v("sendC2Java", "1");
				break;
			}
			//Log.v("sendC2Java", "2");
			sendtime--;
		}

		if (sendtime >= 0) {
			Log.v("wjjar.log","sendC2Java success");
			Log.v("sendC2Java", "success");
			return 0;
		} else {
			Log.v("wjjar.log","sendC2Java failed");
			Log.v("sendC2Java", "failed");
			return -1;
		}
	}

	public void setTimeOutSecond(int timeOutSecond) {
		m_timeOutSecond = timeOutSecond;
		int ret = 0;
		setTimeOutMax(m_timeOutSecond);
		
		
	}

	private int setTimeOutMax(int time){
		
		int ret = WJIssue_Ble.SetTimeOutMax(time);
		return ret;
	}
	
	public int setTimeOutCnt(int time){
		
		int ret = WJIssue_Ble.SetTimeOutCnt(time);
		return ret;
	}
	public ServiceStatus getCardInformation(CardInformation cardinfo) {

		ServiceStatus l_ret = new ServiceStatus();

		WJIssue_Ble.SetTimeOutCnt(0);

		int ret = WJVariables.SUCCESS;
		l_ret.ServiceCode = WJVariables.SUCCESS;
//		WJInit.init_C0(WJVariables.RF_POWERON);
//		Log.v("process", "init_C0");
//
//		ret = WJIssue_Ble.send_c0_Ble(WJVariables.g_c0_data);
//		if (ret != WJVariables.SUCCESS) {
//			Log.v("error", "init_C0");
//			return l_ret; // 本次流程终止
//		}
//
//		// 等待b0
//		ret = WJIssue_Ble.recv_b0_Ble(WJVariables.g_b0_data,
//				WJVariables.TIME_OUT);
//		if (ret != WJVariables.SUCCESS) {
//			// sendMsg(ret);
//			return l_ret;
//		}
		// sendMsg(WJVariables.PROCESS_10); // 更新进度

		// 发送C1(BST),等待B1
		WJInit.init_C1();
		l_ret.ServiceCode = WJIssue_Ble.send_c1_Ble(WJVariables.g_c1_data);
		if (l_ret.ServiceCode  != WJVariables.SUCCESS) {
			// sendMsg(ret);
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "send_c1_Ble error";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			
			return l_ret;
		}

		l_ret.ServiceCode  = WJIssue_Ble.recv_b1_Ble(WJVariables.g_b1_data,
				WJVariables.TIME_OUT);
		Log.i("123", "ssss");
		if (l_ret.ServiceCode  != WJVariables.SUCCESS) {
			l_ret.ServiceInfo = "recv_b1_Ble error";
			// sendMsg(ret);
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		if (WJVariables.g_b1_data.ErrorCode != 0) {
			l_ret.ServiceCode = WJVariables.g_b1_data.ErrorCode;
			l_ret.ServiceInfo = "WJVariables.g_b1_data.ErrorCode";
			// sendMsg(ret);
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		// sendMsg(WJVariables.PROCESS_30);

		
		WJInit.init_C4((byte) 0x01);
		l_ret.ServiceCode  = WJIssue_Ble.send_c9_Ble(WJVariables.g_c4_data,
				WJVariables.TIME_OUT);
		if (l_ret.ServiceCode  != WJVariables.SUCCESS) {
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "send_c9_Ble error";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			// sendMsg(ret);
			return l_ret;
		}

		l_ret.ServiceCode  = WJIssue_Ble.recv_b9_Ble(WJVariables.g_b3_data);
		if (l_ret.ServiceCode  != WJVariables.SUCCESS) {
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "recv_b9_Ble error";
			// sendMsg(ret);
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		if (WJVariables.g_b3_data.ErrorCode != 0) {
			// sendMsg(ret);
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "g_b3_data.ErrorCode";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		WJInit.save_info();
		// sendMsg(WJVariables.PROCESS_50);
		cardinfo.setCardId(CommonFunct.byte_to_hexstring(
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.cardNo, 8, 0, 8,
				" "));
		cardinfo.setCardType(CommonFunct
				.GetObuSysInfo_CardType(WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.cardType));
		cardinfo.setCardVersion(""
				+ WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.cardVersion);
		cardinfo.setProvider(MyParse
				.Frame2String(WJVariables.g_pkg_data.contractProvider));// 发卡方标识？
		cardinfo.setSignedDate(MyParse
				.Frame2String(WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.SignedDate));
		cardinfo.setExpiredDate(MyParse
				.Frame2String(WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.ExpiredDate));
		cardinfo.setVehicleNumber(CommonFunct.GetObuSysInfo_PlateNum(
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.bindedPlate, 12, 0,
				12));
		cardinfo.setUserType(CommonFunct
				.GetObuSysInfo_ICCUserType(WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.userType));
		cardinfo.setPlateColor(CommonFunct.GetObuVehInfo_VehPlateColor(WJVariables.g_pkg_data.vehicleLicencePlateColor,2));
		cardinfo.setVehicleMode(CommonFunct.Get0012FileInfo_VehType(WJVariables.g_pkg_data.vehicleClass));

		float money = CommonFunct.GetObuSysInfo_Money(
				WJVariables.g_pkg_iccinfo_data.ICC0002_INFO.IccBanlance, 4);
		//DecimalFormat df = new DecimalFormat("0.00");
		cardinfo.setBalance((int)(money/100));
		// if (mode == 1)
		// {
		// // 发送C4,等待B3(读车辆信息)
		// WJInit.init_C4((byte) 0x02);
		// ret = WJIssue_Ble.send_c8_Ble(WJVariables.g_c4_data,
		// WJVariables.TIME_OUT);
		// if (ret != WJVariables.SUCCESS) {
		// // sendMsg(ret);
		// return l_ret;
		// }
		// // sendMsg(WJVariables.PROCESS_60);
		// ret = WJIssue_Ble.recv_b8_Ble(WJVariables.g_b3_data);
		// if (ret != WJVariables.SUCCESS) {
		// // sendMsg(ret);
		// return l_ret;
		// }
		// if (WJVariables.g_b3_data.ErrorCode != 0) {
		// // sendMsg(ret);
		// return l_ret;
		// }
		// WJInit.save_info();
		// // sendMsg(WJVariables.PROCESS_80);
		// }
		// // 发送C5,等待B4
		 WJInit.init_C5();
		 l_ret.ServiceCode  = WJIssue_Ble.send_c5_Ble(WJVariables.g_c5_data);
		 if (l_ret.ServiceCode  != WJVariables.SUCCESS) {
			 l_ret.ServiceInfo = "send_c5_Ble error";
			 l_ret.ObuCode = l_ret.ServiceCode;
				l_ret.ObuInfo = l_ret.ServiceInfo;
				l_ret.ServiceCode = -1;
		 // sendMsg(ret);
		 return l_ret;
		 }
		
		 // 通知OBU释放链路
		 l_ret.ServiceCode  = WJIssue_Ble.recv_b4_Ble(WJVariables.g_b4_data,
		 WJVariables.TIME_OUT);
		 if ( l_ret.ServiceCode != WJVariables.SUCCESS) {
			 l_ret.ServiceInfo = "recv_b4_Ble error";
			 l_ret.ObuCode = l_ret.ServiceCode;
				l_ret.ObuInfo = l_ret.ServiceInfo;
				l_ret.ServiceCode = -1;
		 // sendMsg(ret);
		 return l_ret;
		 }
		 if (WJVariables.g_b4_data.ErrorCode != 0) {
		 // sendMsg(ret);
			 l_ret.ServiceInfo = "WJVariables.g_b4_data.ErrorCode";
			 l_ret.ObuCode = l_ret.ServiceCode;
				l_ret.ObuInfo = l_ret.ServiceInfo;
				l_ret.ServiceCode = -1;
		 return l_ret;
		 }
		 l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
		return l_ret;
	}

	public ServiceStatus loadCreditGetMac1(String cardId, int credit,
			String terminalNo, String pinCode, String procType, String keyIndex) {

		ServiceStatus l_ret = new ServiceStatus();
		WJIssue_Ble.SetTimeOutCnt(0);
		l_ret.ServiceCode = 0;
//		WJInit.init_C0(WJVariables.RF_POWERON);
//		// 发送c0
//		l_ret.ObuCode = WJIssue_Ble.send_c0_Ble(WJVariables.g_c0_data);
//		if (l_ret.ObuCode != WJVariables.SUCCESS) {
//			// pHandler.sendEmptyMessage(1);
//			return l_ret; // 本次流程终止
//		}
//		// 等待b0
//		l_ret.ObuCode = WJIssue_Ble.recv_b0_Ble(WJVariables.g_b0_data,
//				WJVariables.TIME_OUT);
//		if (l_ret.ObuCode != WJVariables.SUCCESS) {
//			// pHandler.sendEmptyMessage(1);
//			return l_ret; // 本次流程终止
//		}

		// 发送C1(BST),等待B1
		WJInit.init_C1();
		l_ret.ObuCode = WJIssue_Ble.send_c1_Ble(WJVariables.g_c1_data);
		if (l_ret.ObuCode != WJVariables.SUCCESS) {
			// pHandler.sendEmptyMessage(1);
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret; // 本次流程终止
		}

		l_ret.ObuCode = WJIssue_Ble.recv_b1_Ble(WJVariables.g_b1_data,
				WJVariables.TIME_OUT);
		if (l_ret.ObuCode != WJVariables.SUCCESS) {
			// pHandler.sendEmptyMessage(1);
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret; // 本次流程终止
		}
		if (WJVariables.g_b1_data.ErrorCode != 0) {
			// pHandler.sendEmptyMessage(1);
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret; // 本次流程终止
		}

		cardId = "00000000000000000001";

		byte[] l_abcardId = MyParse.Parse(cardId);

		int l_nprocType = 0x01;
		if (procType.equals("ED")) {

			l_nprocType = 0x01;
		} else if (procType.equals("EP")) {

			l_nprocType = 0x02;
		}

		int l_nkeyIndex = Integer.parseInt(keyIndex);
		byte[] l_abterminalNo = MyParse.Parse(terminalNo);
		byte[] l_abpinCode = MyParse.Parse(pinCode);

		l_ret.ServiceCode = WJIssue_Ble.jni2loadCreditGetMac1(l_abcardId,
				credit, l_abterminalNo, l_abpinCode, l_nprocType, l_nkeyIndex,
				WJVariables.g_loadCreditGetMac1Ret);

		l_ret.ServiceInfo = "a_cid="
				+ MyParse
						.Frame2String(WJVariables.g_loadCreditGetMac1Ret.a_cid)
						.substring(4)// 后16位
				+ "&a_pt="
				+ MyParse.Frame2String(WJVariables.g_loadCreditGetMac1Ret.a_pt)
				+ "&a_rnd="
				+ MyParse
						.Frame2String(WJVariables.g_loadCreditGetMac1Ret.a_rnd)
						.substring(0, 8)
				+ "&a_cbb="
				+ MyParse
						.Frame2String(WJVariables.g_loadCreditGetMac1Ret.a_cbb)
				+ "&a_m1="
				+ MyParse.Frame2String(WJVariables.g_loadCreditGetMac1Ret.a_m1)
						.subSequence(0, 8) + "&a_on="
				+ MyParse.Frame2String(WJVariables.g_loadCreditGetMac1Ret.a_on);

		Log.v("l_ret.ServiceInfo", l_ret.ServiceInfo);
		l_ret.ObuCode = l_ret.ServiceCode;
		l_ret.ObuInfo = l_ret.ServiceInfo;
		return l_ret;

	}

	public ServiceStatus loadCreditWriteCard(String mac2) {
		ServiceStatus l_ret = new ServiceStatus();
		WJIssue_Ble.SetTimeOutCnt(0);
		byte[] l_mac2 = MyParse.Parse(mac2);
		l_ret.ServiceCode = 0;
		l_ret.ServiceCode = WJIssue_Ble.jniloadCreditWriteCard(l_mac2);

		// 信息提示
		WJInit.init_C5();
		int ret = WJIssue_Ble.send_c5_Ble(WJVariables.g_c5_data);
		if (ret != WJVariables.SUCCESS) {
			// sendMsg(ret);
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "send_c5_Ble err";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}

		// 通知OBU释放链路
		l_ret.ServiceCode = WJIssue_Ble.recv_b4_Ble(WJVariables.g_b4_data,
				WJVariables.TIME_OUT);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "recv_b4_Ble err";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			// sendMsg(ret);
			return l_ret;
		}
		if (WJVariables.g_b4_data.ErrorCode != 0) {
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "WJVariables.g_b4_data.ErrorCode err";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			// sendMsg(ret);
			return l_ret;
		}
		l_ret.ObuCode = l_ret.ServiceCode;
		l_ret.ObuInfo = l_ret.ServiceInfo;
		return l_ret;
	}

	// private b3data2CardConsumeRecord()
	public ServiceStatus readCardConsumeRecord(int maxNumber,
			List<CardConsumeRecord> cardRecordList) {

		ServiceStatus l_ret = new ServiceStatus();
		WJIssue_Ble.SetTimeOutCnt(0);
		l_ret.ServiceCode = 0;
		// l_ret.ServiceCode =
		// WJIssue_Ble.readCardConsumeRecord(maxNumber,WJVariables.g_RecordListRet);
//		WJInit.init_C0(WJVariables.RF_POWERON);
//		// 发送c0
//		l_ret.ServiceCode = WJIssue_Ble.send_c0_Ble(WJVariables.g_c0_data);
//		if (l_ret.ServiceCode != Variables.SUCCESS) {
//
//			return l_ret; // 本次流程终止
//		}
//		// 等待b0
//		l_ret.ServiceCode = WJIssue_Ble.recv_b0_Ble(WJVariables.g_b0_data,
//				WJVariables.TIME_OUT);
//		if (l_ret.ServiceCode != Variables.SUCCESS) {
////
////			return l_ret;
//		}

		// 发送C1(BST),等待B1
		WJInit.init_C1();
		l_ret.ServiceCode = WJIssue_Ble.send_c1_Ble(WJVariables.g_c1_data);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			l_ret.ServiceInfo = "send_c1_Ble err";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}

		l_ret.ServiceCode = WJIssue_Ble.recv_b1_Ble(WJVariables.g_b1_data,
				WJVariables.TIME_OUT);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			l_ret.ServiceInfo = "recv_b1_Ble err";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		if (WJVariables.g_b1_data.ErrorCode != 0) {
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "WJVariables.g_b1_data.ErrorCode err";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		Log.v("c9:", "begin");
		// for(int m = 0; m < 10; m++)
		// {
		// Log.v("m:",m+"");
		// Arrays.fill(WJVariables.g_b3_data.FileContent, 0);
		//
		// }
		for (int j = 0; j < maxNumber; j++) {
			WJInit.init_C4_ReadIccInfo(
					(byte) WJVariables.READ_CPUCARD_FILE_0019, (byte) 1);
			l_ret.ServiceCode = WJIssue_Ble.send_c9_Ble(WJVariables.g_c4_data,
					WJVariables.TIME_OUT);
			Log.v("c9:", "mid:" + j);
			if (l_ret.ServiceCode != WJVariables.SUCCESS) {
				return l_ret;
			}
			l_ret.ServiceCode = WJIssue_Ble
					.recv_b9_Blefile(WJVariables.g_b3_data,(byte) WJVariables.READ_CPUCARD_FILE_0019);
			if (l_ret.ServiceCode != WJVariables.SUCCESS) {
				l_ret.ServiceCode = -1;
				l_ret.ServiceInfo = "recv_b9_Blefile err";
				l_ret.ObuCode = l_ret.ServiceCode;
				l_ret.ObuInfo = l_ret.ServiceInfo;
				return l_ret;
			}
			// 解析0019文件

			CardConsumeRecord l_record = new CardConsumeRecord();

			l_record.applicationId = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][0]);
			System.out.println("l_record.applicationId:"
					+ l_record.applicationId);

			l_record.recordLength = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][1]);
			System.out
					.println("l_record.recordLength:" + l_record.recordLength);
			l_record.applicationLockFlag = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][2]);
			System.out.println("l_record.applicationLockFlag:"
					+ l_record.applicationLockFlag);

			l_record.tollRoadNetworkId = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][3])
					+ CommonFunct
							.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][4]);
			System.out.println("l_record.tollRoadNetworkId:"
					+ l_record.tollRoadNetworkId);

			l_record.tollRoadNetworkId = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][5])
					+ CommonFunct
							.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][6]);
			System.out.println("l_record.tollRoadNetworkId:"
					+ l_record.tollRoadNetworkId);

			l_record.tollLaneId = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][7]);
			System.out.println("l_record.tollLaneId:" + l_record.tollLaneId);

			byte[] temp = new byte[4];
			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 8, temp, 0,
					4);
			l_record.timeUnix = CommonFunct.byte_To_strTime(temp, 4, 0, 4);
			System.out.println("l_record.timeUnix:" + l_record.timeUnix);

			l_record.vehicleModel = CommonFunct
					.Get0012FileInfo_VehType(WJVariables.g_b3_data.FileContent[0][12]);
			System.out
					.println("l_record.vehicleModel:" + l_record.vehicleModel);

			// 入出口状态
			l_record.passStatus = CommonFunct
					.Get0012FileInfo_InOutStatus(WJVariables.g_b3_data.FileContent[0][13]);
			System.out.println("l_record.passStatus:" + l_record.passStatus);

			// 标识站 浙江协议为自定义应用
			temp = new byte[9];
			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 14, temp, 0,
					9);
			l_record.reserve1 = CommonFunct.byte_to_hexstring(temp,
					temp.length, 0, temp.length, "");
			System.out.println("l_record.reserve1:" + l_record.reserve1);

			temp = new byte[3];
			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 23, temp, 0,
					3);
			l_record.staffNo = CommonFunct.byte_to_hexstring(temp, temp.length,
					0, temp.length, " ");
			System.out.println("l_record.staffNo:" + l_record.staffNo);

			l_record.mtcSequenceNo = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][26]);
			System.out.println("l_record.mtcSequenceNo:"
					+ l_record.mtcSequenceNo);

			// 车牌号码
			temp = new byte[12];
			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 27, temp, 0,
					12);
			l_record.vehicleNumber = CommonFunct.GetObuSysInfo_PlateNum(temp,
					temp.length, 0, temp.length);
			System.out.println("l_record.vehicleNumber:"
					+ l_record.vehicleNumber);

			// 收费公路ETC预留
			temp = new byte[12];
			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 39, temp, 0,
					4);
			l_record.reserve2 = CommonFunct.byte_to_hexstring(temp,
					temp.length, 0, temp.length, " ");
			System.out.println("l_record.reserve2:" + l_record.reserve2);

			cardRecordList.add(l_record);
			// l_record.applicationId = WJVariables.g_b3_data.
			Log.v("RSCTL:", WJVariables.g_b3_data.RSCTL + "");
			Log.v("FrameType:", WJVariables.g_b3_data.FrameType + "");
			Log.v("ErrorCode:", WJVariables.g_b3_data.ErrorCode + "");
			Log.v("NumOfFiles:", WJVariables.g_b3_data.NumOfFiles + "");
			Log.v("Length[0]:", WJVariables.g_b3_data.Length[0] + "");
			for (int k = 0; k < WJVariables.g_b3_data.Length[0]; k++) {
				Log.v("FileContent[0]", WJVariables.g_b3_data.FileContent[0][k]
						+ "");
			}
		}
		Log.v("c9:", "end");

		if (WJVariables.g_b3_data.ErrorCode != 0) {
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "WJVariables.g_b3_data.ErrorCode err";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		WJInit.save_CpuCardinfo();
		// 发送C5,等待B4
		WJInit.init_C5();
		l_ret.ServiceCode = WJIssue_Ble.send_c5_Ble(WJVariables.g_c5_data);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "send_c5_Ble err";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}

		// 通知OBU释放链路
		l_ret.ServiceCode = WJIssue_Ble.recv_b4_Ble(WJVariables.g_b4_data,
				WJVariables.TIME_OUT);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "recv_b4_Ble err";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		if (WJVariables.g_b4_data.ErrorCode != 0) {
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "recv_b4_Ble err";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		return l_ret;
	}

	public ServiceStatus readCardTransactionRecord(String pinCode, int maxNumber,
			List<CardTransactionRecord> cardRecordList) {
		ServiceStatus l_ret = new ServiceStatus();
		WJIssue_Ble.SetTimeOutCnt(0);
		byte[] l_abpinCode = MyParse.Parse(pinCode);
		// l_ret.ServiceCode =
		// WJIssue_Ble.readCardConsumeRecord(maxNumber,WJVariables.g_RecordListRet);
//		WJInit.init_C0(WJVariables.RF_POWERON);
//		// 发送c0
//		l_ret.ServiceCode = WJIssue_Ble.send_c0_Ble(WJVariables.g_c0_data);
//		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
//
//			return l_ret; // 本次流程终止
//		}
//		// 等待b0
//		l_ret.ServiceCode = WJIssue_Ble.recv_b0_Ble(WJVariables.g_b0_data,
//				WJVariables.TIME_OUT);
//		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
//
//			return l_ret;
//		}

		// 发送C1(BST),等待B1
		WJInit.init_C1();
		l_ret.ServiceCode = WJIssue_Ble.send_c1_Ble(WJVariables.g_c1_data);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			l_ret.ServiceInfo = "send_c1_Ble error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}

		l_ret.ServiceCode = WJIssue_Ble.recv_b1_Ble(WJVariables.g_b1_data,
				WJVariables.TIME_OUT);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			l_ret.ServiceInfo = "recv_b1_Ble error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		if (WJVariables.g_b1_data.ErrorCode != 0) {
			l_ret.ServiceInfo = "g_b1_data.ErrorCode";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		Log.v("c9:", "begin");
		// for(int m = 0; m < 10; m++)
		// {
		// Log.v("m:",m+"");
		// Arrays.fill(WJVariables.g_b3_data.FileContent, 0);
		//
		// }
		for (int i = 0; i < maxNumber; i++) {
			WJInit.init_C4_ReadIccInfo(
					(byte) WJVariables.READ_CPUCARD_FILE_0018, (byte)1);
			l_ret.ServiceCode = WJIssue_Ble.send_c90018file_Ble(WJVariables.g_c4_data,l_abpinCode,
					WJVariables.TIME_OUT);
			Log.v("c9:", "mid:" + i);
			if (l_ret.ServiceCode != WJVariables.SUCCESS) {
				l_ret.ServiceInfo = "send_c90018file_Ble error";
				l_ret.ServiceCode = -1;
				l_ret.ObuCode = l_ret.ServiceCode;
				l_ret.ObuInfo = l_ret.ServiceInfo;
				return l_ret;
			}
			l_ret.ServiceCode = WJIssue_Ble
					.recv_b9_Blefile(WJVariables.g_b3_data,(byte)WJVariables.READ_CPUCARD_FILE_0018);
			if (l_ret.ServiceCode != WJVariables.SUCCESS) {

				l_ret.ServiceInfo = "send_c5_Ble error";
				l_ret.ServiceCode = -1;
				l_ret.ObuCode = l_ret.ServiceCode;
				l_ret.ObuInfo = l_ret.ServiceInfo;
				return l_ret;
			}
			// 解析0018文件
			CardTransactionRecord l_record = new CardTransactionRecord();
			l_record.onlineSn = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][0])
					+ CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][1]);			
			System.out.println("l_record.onlineSn:"
			+ l_record.onlineSn);
			
			byte[] bmoney = new byte[3];
			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 2, bmoney, 0, 3);
			float money = CommonFunct.GetObuSysInfo_Money(bmoney,
					3);
			l_record.overdrawLimit = money+"";
			bmoney = null;
			bmoney = new byte[4];
			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 5, bmoney, 0, 4);
			money = CommonFunct.GetObuSysInfo_Money(bmoney,
					4);
			l_record.transAmount = money+"";
			
			l_record.transType = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][9]);
			
			byte[] temp = new byte[6];
			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 10, temp, 0,
					6);
			l_record.terminalNo = CommonFunct.byte_to_hexstring(temp,
					temp.length, 0, temp.length, "");
			
			l_record.transDate =  CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][16])
					+CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][17])
					+CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][18])
					+CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][19]);
			
			l_record.transTime =  CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][20])
					+CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][21])
					+CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][22]);
			cardRecordList.add(l_record);
			Log.v("RSCTL:", WJVariables.g_b3_data.RSCTL + "");
			Log.v("FrameType:", WJVariables.g_b3_data.FrameType + "");
			Log.v("ErrorCode:", WJVariables.g_b3_data.ErrorCode + "");
			Log.v("NumOfFiles:", WJVariables.g_b3_data.NumOfFiles + "");
			Log.v("Length[0]:", WJVariables.g_b3_data.Length[0] + "");
			for (int k = 0; k < WJVariables.g_b3_data.Length[0]; k++) {
				Log.v("FileContent[0]", WJVariables.g_b3_data.FileContent[0][k]
						+ "");
			}
		}
		Log.v("c9:", "end");

		if (WJVariables.g_b3_data.ErrorCode != 0) {
			l_ret.ServiceCode = -1;
			l_ret.ServiceInfo = "WJVariables.g_b3_data.ErrorCode error";
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		WJInit.save_CpuCardinfo();
		
		WJInit.init_C5();
		l_ret.ServiceCode = WJIssue_Ble.send_c5_Ble(WJVariables.g_c5_data);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			
			l_ret.ServiceInfo = "send_c5_Ble error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}

		
		l_ret.ServiceCode = WJIssue_Ble.recv_b4_Ble(WJVariables.g_b4_data,
				WJVariables.TIME_OUT);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {

			l_ret.ServiceInfo = "recv_b4_Ble error";
			l_ret.ServiceInfo = "send_c5_Ble error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		if (WJVariables.g_b4_data.ErrorCode != 0) {

			l_ret.ServiceInfo = "WJVariables.g_b4_data.ErrorCode";
			l_ret.ServiceInfo = "send_c5_Ble error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			return l_ret;
		}
		return l_ret;
	}
	
	public ServiceStatus readCardOwnerRecord(String pinCode, CardOwner owner){
		
		ServiceStatus l_ret = new ServiceStatus();
		WJIssue_Ble.SetTimeOutCnt(0);
		byte[] l_abpinCode = MyParse.Parse(pinCode);
		// l_ret.ServiceCode =
		// WJIssue_Ble.readCardConsumeRecord(maxNumber,WJVariables.g_RecordListRet);
//		WJInit.init_C0(WJVariables.RF_POWERON);
//		// 发送c0
//		l_ret.ServiceCode = WJIssue_Ble.send_c0_Ble(WJVariables.g_c0_data);
//		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
//
//			return l_ret; // 本次流程终止
//		}
//		// 等待b0
//		l_ret.ServiceCode = WJIssue_Ble.recv_b0_Ble(WJVariables.g_b0_data,
//				WJVariables.TIME_OUT);
//		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
//
//			return l_ret;
//		}

		// 发送C1(BST),等待B1
		WJInit.init_C1();
		l_ret.ServiceCode = WJIssue_Ble.send_c1_Ble(WJVariables.g_c1_data);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			l_ret.ServiceInfo = "send_c1_Ble error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			Log.v("wjjar.log","send_c1_Ble failed");
			return l_ret;
		}

		l_ret.ServiceCode = WJIssue_Ble.recv_b1_Ble(WJVariables.g_b1_data,
				WJVariables.TIME_OUT);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			l_ret.ServiceInfo = "recv_b1_Ble error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			Log.v("wjjar.log","recv_b1_Ble failed");
			return l_ret;
		}
		if (WJVariables.g_b1_data.ErrorCode != 0) {
			l_ret.ServiceInfo = "WJVariables.g_b1_data.ErrorCode error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			Log.v("wjjar.log","WJVariables.g_b1_data.ErrorCode failed");
			return l_ret;
		}
		Log.v("c9:", "begin");
		// for(int m = 0; m < 10; m++)
		// {
		// Log.v("m:",m+"");
		// Arrays.fill(WJVariables.g_b3_data.FileContent, 0);
		//
		// }
		
			WJInit.init_C4_ReadIccInfo(
					(byte) WJVariables.READ_CPUCARD_FILE_0016, (byte)1);
			l_ret.ServiceCode = WJIssue_Ble.send_c90016file_Ble(WJVariables.g_c4_data,l_abpinCode,
					WJVariables.TIME_OUT);
			if (l_ret.ServiceCode != WJVariables.SUCCESS) {
				l_ret.ServiceInfo = "send_c90016file_Ble error";
				l_ret.ServiceCode = -1;
				l_ret.ObuCode = l_ret.ServiceCode;
				l_ret.ObuInfo = l_ret.ServiceInfo;
				Log.v("wjjar.log","send_c90016file_Ble failed");
				return l_ret;
			}
			l_ret.ServiceCode = WJIssue_Ble
					.recv_b9_Blefile(WJVariables.g_b3_data,(byte)WJVariables.READ_CPUCARD_FILE_0016);
			if (l_ret.ServiceCode != WJVariables.SUCCESS) {
				l_ret.ServiceInfo = "recv_b9_Blefile error";
				l_ret.ServiceCode = -1;
				l_ret.ObuCode = l_ret.ServiceCode;
				l_ret.ObuInfo = l_ret.ServiceInfo;
				Log.v("wjjar.log","recv_b9_Blefile failed");
				return l_ret;
			}
			// 解析0016文件
			owner.ownerId = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][0]);
			owner.staffId = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][1]);
			owner.ownerName = CommonFunct.GetObuSysInfo_PlateNum(
					WJVariables.g_b3_data.FileContent[0], 20, 2,
					22);
			byte[] temp = new byte[32];
			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 22, temp, 0,32);
			owner.ownerLicenseNumber = CommonFunct.byte_to_hexstring(temp,
					temp.length, 0, temp.length, "");
			owner.ownerLicenseType = CommonFunct
					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][54]);
					
//					byte[] temp = new byte[6];
//					System.arraycopy(WJVariables.g_b3_data.FileContent[0], 10, temp, 0,
//							6);
//					l_record.terminalNo = CommonFunct.byte_to_hexstring(temp,
//							temp.length, 0, temp.length, "");
//			CardTransactionRecord l_record = new CardTransactionRecord();
//			l_record.onlineSn = CommonFunct
//					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][0])
//					+ CommonFunct
//					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][1]);			
//			System.out.println("l_record.onlineSn:"
//			+ l_record.onlineSn);
//			
//			byte[] bmoney = new byte[3];
//			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 2, bmoney, 0, 3);
//			float money = CommonFunct.GetObuSysInfo_Money(bmoney,
//					3);
//			l_record.overdrawLimit = money+"";
//			bmoney = null;
//			bmoney = new byte[4];
//			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 5, bmoney, 0, 4);
//			money = CommonFunct.GetObuSysInfo_Money(bmoney,
//					4);
//			l_record.transAmount = money+"";
//			
//			l_record.transType = CommonFunct
//					.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][9]);
//			
//			byte[] temp = new byte[6];
//			System.arraycopy(WJVariables.g_b3_data.FileContent[0], 10, temp, 0,
//					6);
//			l_record.terminalNo = CommonFunct.byte_to_hexstring(temp,
//					temp.length, 0, temp.length, "");
//			
//			l_record.transDate =  CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][16])
//					+CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][17])
//					+CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][18])
//					+CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][19]);
//			
//			l_record.transTime =  CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][20])
//					+CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][21])
//					+CommonFunct.byte_to_hexstring(WJVariables.g_b3_data.FileContent[0][22]);
//			cardRecordList.add(l_record);
//			Log.v("RSCTL:", WJVariables.g_b3_data.RSCTL + "");
//			Log.v("FrameType:", WJVariables.g_b3_data.FrameType + "");
//			Log.v("ErrorCode:", WJVariables.g_b3_data.ErrorCode + "");
//			Log.v("NumOfFiles:", WJVariables.g_b3_data.NumOfFiles + "");
//			Log.v("Length[0]:", WJVariables.g_b3_data.Length[0] + "");
//			for (int k = 0; k < WJVariables.g_b3_data.Length[0]; k++) {
//				Log.v("FileContent[0]", WJVariables.g_b3_data.FileContent[0][k]
//						+ "");
//			}
		Log.v("c9:", "end");

		if (WJVariables.g_b3_data.ErrorCode != 0) {
			l_ret.ServiceInfo = "WJVariables.g_b3_data.ErrorCode error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			Log.v("wjjar.log","WJVariables.g_b3_data.ErrorCode failed");
			return l_ret;
		}
		WJInit.save_CpuCardinfo();
		// 发送C5,等待B4
		WJInit.init_C5();
		l_ret.ServiceCode = WJIssue_Ble.send_c5_Ble(WJVariables.g_c5_data);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			l_ret.ServiceInfo = "send_c5_Ble error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			Log.v("wjjar.log","send_c5_Ble failed");
			return l_ret;
		}

		// 通知OBU释放链路
		l_ret.ServiceCode = WJIssue_Ble.recv_b4_Ble(WJVariables.g_b4_data,
				WJVariables.TIME_OUT);
		if (l_ret.ServiceCode != WJVariables.SUCCESS) {
			l_ret.ServiceInfo = "recv_b4_Ble error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			Log.v("wjjar.log","recv_b4_Ble failed");
			return l_ret;
		}
		if (WJVariables.g_b4_data.ErrorCode != 0) {
			l_ret.ServiceInfo = "WJVariables.g_b4_data.ErrorCode error";
			l_ret.ServiceCode = -1;
			l_ret.ObuCode = l_ret.ServiceCode;
			l_ret.ObuInfo = l_ret.ServiceInfo;
			Log.v("wjjar.log","WJVariables.g_b4_data.ErrorCode failed");
			return l_ret;
		}
		return l_ret;
	}
	
	public String cardCommand(String command){
		
		int ret;
		WJIssue_Ble.SetTimeOutCnt(0);
		String str = "";
		WJVariables.g_rec_data.sendbuf = MyParse.Parse(command);
		WJVariables.g_rec_data.len0 = (byte)WJVariables.g_rec_data.sendbuf.length;
		//sendC2Java(buf,buf.length);
//		WJInit.init_C0(WJVariables.RF_POWERON);
//		Log.v("process", "init_C0");
//
//		ret = WJIssue_Ble.send_c0_Ble(WJVariables.g_c0_data);
//		if (ret != WJVariables.SUCCESS) {
//			Log.v("error", "init_C0");
//			
//			return "send_c0_Ble error"; // 本次流程终止
//		}
//
//		// 等待b0
//		ret = WJIssue_Ble.recv_b0_Ble(WJVariables.g_b0_data,
//				WJVariables.TIME_OUT);
//		if (ret != WJVariables.SUCCESS) {
//			// sendMsg(ret);
//			return "recv_b0_Ble error";
//		}
		// sendMsg(WJVariables.PROCESS_10); // 更新进度

		// 发送C1(BST),等待B1
		WJInit.init_C1();
		ret = WJIssue_Ble.send_c1_Ble(WJVariables.g_c1_data);
		if (ret != WJVariables.SUCCESS) {
			// sendMsg(ret);
			return "send_c1_Ble error";
		}

		ret = WJIssue_Ble.recv_b1_Ble(WJVariables.g_b1_data,
				WJVariables.TIME_OUT);
		Log.i("123", "ssss");
		if (ret != WJVariables.SUCCESS) {
			// sendMsg(ret);
			return "recv_b1_Ble error";
		}
		if (WJVariables.g_b1_data.ErrorCode != 0) {
			// sendMsg(ret);
			return "g_b1_data.ErrorCode error";
		}
		WJIssue_Ble.cardCommand_Recv(WJVariables.g_rec_data,1000);	
		byte[] temp = new byte[WJVariables.g_rec_data.len];
		System.arraycopy(WJVariables.g_rec_data.FileContent, 0, temp, 0, temp.length);
		str = MyParse.Frame2String(temp);
		
		WJInit.init_C5();
		ret = WJIssue_Ble.send_c5_Ble(WJVariables.g_c5_data);
		if (ret != WJVariables.SUCCESS) {

			return "send_c5_Ble error";
		}

		// 通知OBU释放链路
		ret = WJIssue_Ble.recv_b4_Ble(WJVariables.g_b4_data,
				WJVariables.TIME_OUT);
		if (ret != WJVariables.SUCCESS) {

			return "recv_b4_Ble error";
		}
		if (WJVariables.g_b4_data.ErrorCode != 0) {

			return "g_b4_data.ErrorCode error";
		}
		return str;
		
	}

}
