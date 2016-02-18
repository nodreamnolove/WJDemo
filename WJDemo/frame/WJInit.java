package wj.ble.obu.comjar;

import wj.ble.obu.comjar.WJVariables;

public class WJInit {

	/**
	 * 初始化交易信息数组
	 */
//	static public void InitTradeInfoArray() {
//		for (int i = 0; i < 50; i++) {
//			WJVariables.m_TradeInfo[i] = new TradeInfo();
//		}
//	}

	/**
	 * 函数名称： init_C0() 初始化C0帧
	 * 
	 * @param rf_power
	 *            0--关闭射频，1--打开射频
	 */
	static public void init_C0(int rf_power) {
		int power_value = 0;
		long ltime = System.currentTimeMillis() / 1000; // 当前时间从1970.1.1开始的秒数

		WJVariables.g_c0_data.RSCTL = (byte) 0x80;
		WJVariables.g_c0_data.CMDType = (byte) 0xC0;
		WJVariables.g_c0_data.UnixTime[0] = (byte) ((ltime >> 24) & 0xff);
		WJVariables.g_c0_data.UnixTime[1] = (byte) ((ltime >> 16) & 0xff);
		WJVariables.g_c0_data.UnixTime[2] = (byte) ((ltime >> 8) & 0xff);
		WJVariables.g_c0_data.UnixTime[3] = (byte) (ltime & 0xff);
		WJVariables.g_c0_data.BSTInterval = 0x08;
		WJVariables.g_c0_data.TxPower = (byte) power_value;
		WJVariables.g_c0_data.PHYChannelID = 0x00;
		WJVariables.g_c0_data.CRCMode = 0x00;
		WJVariables.g_c0_data.RFPower = (byte) rf_power;
		WJVariables.g_c0_data.PSAMPower = 0x01;
		WJVariables.g_c0_data.run_mode = WJVariables.g_run_mode;
		for (int i = 0; i < 2; i++) {
			WJVariables.g_c0_data.Reserved[i] = 0x00;
		}
	}

	/**
	 * 函数名称： init_C1() 初始化C1帧
	 * 
	 */
	public static void init_C1() {
		short i = 0;
		WJVariables.g_c1_data.RSCTL = (byte) 0x80;
		WJVariables.g_c1_data.CMDType = (byte) 0xC1;
		WJVariables.g_c1_data.ObuInitMode = 0x00;
		for (i = 0; i < 5; i++) {
			WJVariables.g_c1_data.Reserved[i] = 0x00;
		}
	}

	/**
	 * init_C2() 初始化C2帧
	 */
	public static void init_C2() {
		short i = 0;
		WJVariables.g_c2_data.RSCTL = (byte) 0x81;
		WJVariables.g_c2_data.CMDType = (byte) 0xC2;
		WJVariables.g_c2_data.KCType = 0x00;
		WJVariables.g_c2_data.TCType = 0x00;
		for (i = 0; i < 5; i++) {
			WJVariables.g_c2_data.Reserved[i] = 0x00;
		}
	}

	/**
	 * init_C3() 初始化C3帧
	 * 
	 * @param mode
	 *            0为防拆位设置,1为脱机二次发行 在线发行请使用 mode = 0
	 */
	public static void init_C3(int mode) {
		short i = 0, j = 0, pos = 0;
		WJVariables.g_c3_data.RSCTL = (byte) 0x81;
		WJVariables.g_c3_data.CMDType = (byte) 0xC3;
		if (mode == 0) {
			WJVariables.g_c3_data.NumOfFiles = 0x01;
			WJVariables.g_c3_data.DIDnFID[0] = 0x01;
			WJVariables.g_c3_data.Offset[0] = 0x1A;
			WJVariables.g_c3_data.Length[0] = 0x01;
			WJVariables.g_c3_data.FileContent[0][0] = 0x01;
		} else if (mode == 1) {
			// MainActivity.ReadIssueInfo();//将界面中的信息读取到g_c3_data中
			WJVariables.g_c3_data.NumOfFiles = 0x02;
			for (i = 0; i < WJVariables.g_c3_data.NumOfFiles; i++) {
				pos = 0;
				if (i == 0) {
					WJVariables.g_c3_data.DIDnFID[i] = 0x01;
					WJVariables.g_c3_data.Offset[i] = 0x00;
					WJVariables.g_c3_data.Length[i] = 0x1B;
					for (j = 0; j < 8; j++) {
						WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.contractProvider[j];
					}
					WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.contractType;
					WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.contractVersion;
					for (j = 0; j < 4; j++) {
						WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.contractSerialNumber[j];
					}
					for (j = 0; j < 4; j++) {
						WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.contractSignedDate[j];
					}
					for (j = 0; j < 4; j++) {
						WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.contractExpiredDate[j];
					}
					WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.tamperFlag;
				} else if (i == 1) {
					WJVariables.g_c3_data.DIDnFID[i] = 0x11;
					WJVariables.g_c3_data.Offset[i] = 0x00;
					WJVariables.g_c3_data.Length[i] = 0x3B;
					for (j = 0; j < 12; j++) {
						WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleLicencePlateNumber[j];
					}
					for (j = 0; j < 2; j++) {
						WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleLicencePlateColor[j];
					}
					WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleClass;
					WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleUserType;
					WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleDimensions.vehicleLength[0];
					WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleDimensions.vehicleLength[1];
					WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleDimensions.vehicleWidth;
					WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleDimensions.vehicleHeight;
					WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleWheels;
					WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleAxles;
					for (j = 0; j < 2; j++) {
						WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleWheelBases[j];
					}
					for (j = 0; j < 3; j++) {
						WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleWeightLimits[j];
					}
					for (j = 0; j < 16; j++) {
						WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleSpecificInformation[j];
					}
					for (j = 0; j < 16; j++) {
						WJVariables.g_c3_data.FileContent[i][pos++] = WJVariables.g_pkg_data.vehicleEngineNumber[j];
					}
				}
			}
		}/* end else if(mode == 1) */
	}

	/**
	 * 初始化C4帧 参数列表：
	 * 
	 * @param nType
	 *        初始化C4帧的类型 
	 *    01 ：读系统信息
	 *    02 ：读车辆信息 
	 *    03 ：读系统信息+读车辆信息 
	 *    其他：读系统信息
	 */
	public static void init_C4(byte nType) {
		WJVariables.g_c4_data.RSCTL = (byte) 0x81;
		WJVariables.g_c4_data.CMDType = (byte) 0xC4;
		WJVariables.g_c4_data.C4Flag = 0;
		if (nType == 0x01) {
			WJVariables.g_c4_data.NumOfFiles = 0x01;
			// 读系统信息文件
			WJVariables.g_c4_data.DIDnFID[0] = 0x01;
			WJVariables.g_c4_data.Offset[0] = 0x00;
			WJVariables.g_c4_data.Length[0] = 0x1B;
		} else if (nType == 0x02) {
			WJVariables.g_c4_data.NumOfFiles = 0x01;
			// 读车辆信息文件
			WJVariables.g_c4_data.DIDnFID[0] = 0x11;
			WJVariables.g_c4_data.Offset[0] = 0x00;
			WJVariables.g_c4_data.Length[0] = 0x3B;
		} else if (nType == 0x03) {
			WJVariables.g_c4_data.NumOfFiles = 0x02;
			// 读系统信息文件
			WJVariables.g_c4_data.DIDnFID[0] = 0x01;
			WJVariables.g_c4_data.Offset[0] = 0x00;
			WJVariables.g_c4_data.Length[0] = 0x1B;
			// 读车辆信息文件
			WJVariables.g_c4_data.DIDnFID[1] = 0x11;
			WJVariables.g_c4_data.Offset[1] = 0x00;
			WJVariables.g_c4_data.Length[1] = 0x3B;
		} else {
			WJVariables.g_c4_data.NumOfFiles = 0x01;
			// 读系统信息文件
			WJVariables.g_c4_data.DIDnFID[0] = 0x01;
			WJVariables.g_c4_data.Offset[0] = 0x00;
			WJVariables.g_c4_data.Length[0] = 0x1B;
		}
	}

	/**
	 * 初始化读取IC卡时使用的C4帧
	 * 
	 * @param nType
	 * 0x02:读取IC卡余额
	 * 0x12:读取0012文件
	 * 0x15:读取0015文件
	 * 0x19:读取0019文件
	 */
	public static void init_C4_ReadIccInfo(byte nType) {
		WJVariables.g_c4_data.RSCTL = (byte) 0x81;
		WJVariables.g_c4_data.CMDType = (byte) 0xC4;
		WJVariables.g_c4_data.C4Flag = 0;
		WJVariables.g_c4_data.NumOfFiles = 0x02;

		// 读IC卡信息
		WJVariables.g_c4_data.DIDnFID[0] = nType;
		switch (nType) {
		case 0x02://
			WJVariables.g_c4_data.Offset[0] = 0x02;
			WJVariables.g_c4_data.Length[0] = 4;
			break;
		case 0x12:
			WJVariables.g_c4_data.Offset[0] = 0x00;
			WJVariables.g_c4_data.Length[0] = 40;
			break;
		case 0x15:
			WJVariables.g_c4_data.Offset[0] = 0x00;
			WJVariables.g_c4_data.Length[0] = 43;
			break;
		case 0x19:
			WJVariables.g_c4_data.Offset[0] = (byte) 0xCC;
			WJVariables.g_c4_data.Length[0] = 43;
			break;
		default:
			break;
		}
	}
	public static void init_C4_ReadIccInfo(byte nType,byte index) {
		WJVariables.g_c4_data.RSCTL = (byte) 0x81;
		WJVariables.g_c4_data.CMDType = (byte) 0xC4;
		WJVariables.g_c4_data.C4Flag = 0;
		WJVariables.g_c4_data.NumOfFiles = index;

		// 读IC卡信息00B201C417
		WJVariables.g_c4_data.DIDnFID[0] = nType;
		switch (nType) {
		case 0x02://
			WJVariables.g_c4_data.Offset[0] = 0x02;
			WJVariables.g_c4_data.Length[0] = 4;
			break;
		case 0x12:
			WJVariables.g_c4_data.Offset[0] = 0x00;
			WJVariables.g_c4_data.Length[0] = 40;
			break;
		case 0x15:
			WJVariables.g_c4_data.Offset[0] = 0x00;
			WJVariables.g_c4_data.Length[0] = 43;
			break;
		case 0x16:
			WJVariables.g_c4_data.Offset[0] = 0x00;
			WJVariables.g_c4_data.Length[0] = 55;
			break;
		case 0x18:
			WJVariables.g_c4_data.Offset[0] = (byte)0xC4;
			WJVariables.g_c4_data.Length[0] = 0x17;
			break;
		case 0x19:
			WJVariables.g_c4_data.Offset[0] = (byte) 0xCC;
			WJVariables.g_c4_data.Length[0] = 43;
			break;
		default:
			break;
		}
	}

	/**
	 * 函数名称： init_C5() 功能描述： 初始化C5帧 参数列表： 无 返回结果： void
	 */
	public static void init_C5() {
		short i = 0;
		WJVariables.g_c5_data.RSCTL = (byte) 0x82;
		WJVariables.g_c5_data.CMDType = (byte) 0xC5;
		WJVariables.g_c5_data.SetMMIMode = 0x00;
		for (i = 0; i < 5; i++) {
			WJVariables.g_c5_data.Reserved[i] = 0x00;
		}
	}

	/**
	 * 函数名称： save_info() 功能描述： 保存系统及车辆信息 参数列表： 无 返回结果： void
	 */
	public static void save_info() {
		int i = 0;
		int oppt = 0;
		for (i = 0; i < WJVariables.g_b3_data.NumOfFiles; i++) {
			if (WJVariables.g_c4_data.DIDnFID[i] == 0x01) {
				for (oppt = 0; oppt < 8;) {
					WJVariables.g_pkg_data.contractProvider[oppt] = WJVariables.g_b3_data.FileContent[i][oppt++];
				}
				WJVariables.g_pkg_data.contractType = WJVariables.g_b3_data.FileContent[i][oppt++];
				WJVariables.g_pkg_data.contractVersion = WJVariables.g_b3_data.FileContent[i][oppt++];
				for (int k = 0; k < 8;) {
					WJVariables.g_pkg_data.contractSerialNumber[k++] = WJVariables.g_b3_data.FileContent[i][oppt++];
				}
				for (int k = 0; k < 4;) {
					WJVariables.g_pkg_data.contractSignedDate[k++] = WJVariables.g_b3_data.FileContent[i][oppt++];
				}
				for (int k = 0; k < 4;) {
					WJVariables.g_pkg_data.contractExpiredDate[k++] = WJVariables.g_b3_data.FileContent[i][oppt++];
				}
				WJVariables.g_pkg_data.tamperFlag = WJVariables.g_b3_data.FileContent[i][oppt++];
				// 以上实现Cfuncs.memcpy((char *)&WJVariables.g_pkg_data +
				// WJVariables.g_c4_data.Offset[i], (char
				// *)&WJVariables.g_b3_data.FileContent[i],
				// WJVariables.g_b3_data.Length[i]);
			} else if (WJVariables.g_c4_data.DIDnFID[i] == 0x11) {
				for (oppt = 0; oppt < 12;) {
					WJVariables.g_pkg_data.vehicleLicencePlateNumber[oppt] = WJVariables.g_b3_data.FileContent[i][oppt++];
				}
				for (int k = 0; k < 2;) {
					WJVariables.g_pkg_data.vehicleLicencePlateColor[k++] = WJVariables.g_b3_data.FileContent[i][oppt++];
				}
				WJVariables.g_pkg_data.vehicleClass = WJVariables.g_b3_data.FileContent[i][oppt++];
				WJVariables.g_pkg_data.vehicleUserType = WJVariables.g_b3_data.FileContent[i][oppt++];
				for (int k = 0; k < 2;) {
					WJVariables.g_pkg_data.vehicleDimensions.vehicleLength[k++] = WJVariables.g_b3_data.FileContent[i][oppt++];
				}
				WJVariables.g_pkg_data.vehicleDimensions.vehicleWidth = WJVariables.g_b3_data.FileContent[i][oppt++];
				WJVariables.g_pkg_data.vehicleDimensions.vehicleHeight = WJVariables.g_b3_data.FileContent[i][oppt++];
				WJVariables.g_pkg_data.vehicleWheels = WJVariables.g_b3_data.FileContent[i][oppt++];
				WJVariables.g_pkg_data.vehicleAxles = WJVariables.g_b3_data.FileContent[i][oppt++];
				for (int k = 0; k < 2;) {
					WJVariables.g_pkg_data.vehicleWheelBases[k++] = WJVariables.g_b3_data.FileContent[i][oppt++];
				}
				for (int k = 0; k < 3;) {
					WJVariables.g_pkg_data.vehicleWeightLimits[k++] = WJVariables.g_b3_data.FileContent[i][oppt++];
				}
				for (int k = 0; k < 16;) {
					WJVariables.g_pkg_data.vehicleSpecificInformation[k++] = WJVariables.g_b3_data.FileContent[i][oppt++];
				}
				for (int k = 0; k < 16;) {
					WJVariables.g_pkg_data.vehicleEngineNumber[k++] = WJVariables.g_b3_data.FileContent[i][oppt++];
				}
			}
		}

		// 0002
		if (WJVariables.g_b3_data.Length[2] != 0) {
			oppt = 0;
			i = 2;
			for (int k = 0; k < 4; k++) {
				byte a = 0;
				a = WJVariables.g_b3_data.FileContent[i][oppt++];
				WJVariables.g_pkg_iccinfo_data.ICC0002_INFO.IccBanlance[k] = a;
			}
		}

		// 0012
		if (WJVariables.g_b3_data.Length[3] != 0) {
			oppt = 0;
			i = 3;
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InRoadNetID[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InRoadID[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InRoadLandNO = WJVariables.g_b3_data.FileContent[i][oppt++];
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InRoadTime[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.VehType = WJVariables.g_b3_data.FileContent[i][oppt++];
			WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InOutStatus = WJVariables.g_b3_data.FileContent[i][oppt++];
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.Identificationstation[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.StaffID[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InRoadShift = WJVariables.g_b3_data.FileContent[i][oppt++];
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.bindedPlate[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.OtherInfo[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
		}
		// 0015
		if (WJVariables.g_b3_data.Length[4] != 0) {
			oppt = 0;
			i = 4;
			for (int k = 0; k < 8; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.cardIssuerID[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.cardType = WJVariables.g_b3_data.FileContent[i][oppt++];
			WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.cardVersion = WJVariables.g_b3_data.FileContent[i][oppt++];
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.roadswebID[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 8; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.cardNo[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 4; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.SignedDate[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 4; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.ExpiredDate[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 12; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.bindedPlate[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.userType = WJVariables.g_b3_data.FileContent[i][oppt++];
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.OtherInfo[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
		}
		// 0019
		if (WJVariables.g_b3_data.Length[5] != 0) {
			oppt = 0;
			i = 5;
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InRoadNetID[k]=
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InRoadID[k] =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InRoadLandNO =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InRoadTime[k] =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.VehType =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InOutStatus =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.Identificationstation[k]
			// = WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.StaffID[k] =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InRoadShift =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.bindedPlate[k]=
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.OtherInfo[k] =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
		}
	}

	/**
	 * 函数名称： save_info() 功能描述： 保存系统及车辆信息 参数列表： 无 返回结果： void
	 */
	public static void save_CpuCardinfo() {
		int i = 0;
		int oppt = 0;
		// 0002
		if (WJVariables.g_b3_data.Length[2] != 0) {
			oppt = 0;
			i = 2;
			for (int k = 0; k < 4; k++) {
				byte a = 0;
				a = WJVariables.g_b3_data.FileContent[i][oppt++];
				WJVariables.g_pkg_iccinfo_data.ICC0002_INFO.IccBanlance[k] = a;
			}
		}

		// 0012
		if (WJVariables.g_b3_data.Length[3] != 0) {
			oppt = 0;
			i = 3;
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InRoadNetID[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InRoadID[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InRoadLandNO = WJVariables.g_b3_data.FileContent[i][oppt++];
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InRoadTime[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.VehType = WJVariables.g_b3_data.FileContent[i][oppt++];
			WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InOutStatus = WJVariables.g_b3_data.FileContent[i][oppt++];
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.Identificationstation[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.StaffID[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.InRoadShift = WJVariables.g_b3_data.FileContent[i][oppt++];
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.bindedPlate[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0012_INFO.OtherInfo[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
		}
		// 0015
		if (WJVariables.g_b3_data.Length[4] != 0) {
			oppt = 0;
			i = 4;
			for (int k = 0; k < 8; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.cardIssuerID[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.cardType = WJVariables.g_b3_data.FileContent[i][oppt++];
			WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.cardVersion = WJVariables.g_b3_data.FileContent[i][oppt++];
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.roadswebID[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 8; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.cardNo[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 4; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.SignedDate[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 4; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.ExpiredDate[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			for (int k = 0; k < 12; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.bindedPlate[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
			WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.userType = WJVariables.g_b3_data.FileContent[i][oppt++];
			for (int k = 0; k < 2; k++) {
				WJVariables.g_pkg_iccinfo_data.ICC0015_INFO.OtherInfo[k] = WJVariables.g_b3_data.FileContent[i][oppt++];
			}
		}
		// 0019
		if (WJVariables.g_b3_data.Length[5] != 0) {
			oppt = 0;
			i = 5;
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InRoadNetID[k]=
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InRoadID[k] =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InRoadLandNO =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InRoadTime[k] =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.VehType =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InOutStatus =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.Identificationstation[k]
			// = WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.StaffID[k] =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.InRoadShift =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.bindedPlate[k]=
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
			// for(int k = 0;k < 2; k++) {
			// WJVariables.g_pkg_iccinfo_data.ICC0019_INFO.OtherInfo[k] =
			// WJVariables.g_b3_data.FileContent[i][oppt++];
			// }
		}
	}

//	/**
//	 * 函数名称： construct_D0() 功能描述： 构造D0帧 参数列表： port_type -- 端口类型,
//	 * WJVariables.NET:网口, WJVariables.USBCOM:USB转串口 返回结果： void
//	 */
//	public static void construct_D0(int port_type) {
//		int len = 0;
//		byte i = 0, chk = 0;
//		byte[] buf = new byte[512];
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x80;
//		buf[len++] = (byte) 0xd0;
//		buf[len++] = 0x00;
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//		}
//	}
//
//	/**
//	 * @exception 构造D1帧
//	 * @param port_type
//	 *            -- 端口类型, WJVariables.NET:网口, WJVariables.USBCOM:USB转串口
//	 */
//	public static void construct_D1(int port_type) {
//		int len = 0;
//		byte i = 0, chk = 0;
//		byte[] buf = new byte[512];
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x81;
//		buf[len++] = (byte) 0xd1;
//		buf[len++] = 0x00;
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//		}
//	}
//
//	/**
//	 * 函数名称： construct_D2() 功能描述： 构造D2帧 参数列表： port_type -- 端口类型,
//	 * WJVariables.NET:网口, WJVariables.USBCOM:USB转串口 返回结果： void
//	 */
//	public static void construct_D2(int port_type) {
//		int len = 0;
//		byte i = 0, chk = 0;
//		byte[] buf = new byte[512];
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x82;
//		buf[len++] = (byte) 0xd2;
//		buf[len++] = 0x00;
//		buf[len++] = WJVariables.g_d2_data.ListNum[0];
//		buf[len++] = WJVariables.g_d2_data.ListNum[1];
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//		}
//	}
//
//	/**
//	 * 函数名称： construct_D3() 功能描述： 构造D3帧 参数列表： port_type -- 端口类型,
//	 * WJVariables.NET:网口, WJVariables.USBCOM:USB转串口 返回结果： void
//	 */
//	public static void construct_D3(int port_type) {
//		int len = 0;
//		byte i = 0, chk = 0;
//		byte[] buf = new byte[512];
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x83;
//		buf[len++] = (byte) 0xd3;
//		buf[len++] = 0x00;
//		for (i = 0; i < 8; i++) {
//			buf[len++] = WJVariables.g_d3_data.ContractSerialNumber[i];
//		}
//		for (i = 0; i < 12; i++) {
//			buf[len++] = WJVariables.g_d3_data.VehicleLicencePlateNumber[i];
//		}
//		buf[len++] = WJVariables.g_d3_data.IssueResult;
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//		}
//	}
//
//	/**
//	 * 函数名称： construct_D4() 功能描述： 构造D4帧 参数列表： port_type -- 端口类型,
//	 * WJVariables.NET:网口, WJVariables.USBCOM:USB转串口 返回结果： void
//	 */
//	public static void construct_D4(int port_type) {
//		int len = 0;
//		byte i = 0, chk = 0;
//		byte[] buf = new byte[512];
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x84;
//		buf[len++] = (byte) 0xd4;
//		buf[len++] = 0x00;
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//		}
//	}
//
//	/**
//	 * 函数名称： construct_D5() 功能描述： 构造D5帧 参数列表： port_type -- 端口类型,
//	 * WJVariables.NET:网口, WJVariables.USBCOM:USB转串口 返回结果： void
//	 */
//	public static void construct_D5(int port_type) {
//		int len = 0;
//		byte i = 0, chk = 0;
//		byte[] buf = new byte[512];
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x85;
//		buf[len++] = (byte) 0xd5;
//		buf[len++] = (byte) 0x00;
//		buf[len++] = WJVariables.g_d5_data.LogNum[0];
//		buf[len++] = WJVariables.g_d5_data.LogNum[1];
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//		}
//	}
//
//	/**
//	 * 函数名称： construct_D6() 功能描述： 构造D6帧 参数列表： port_type -- 端口类型,
//	 * WJVariables.NET:网口, WJVariables.USBCOM:USB转串口 返回结果： void
//	 */
//	public static void construct_D6(int port_type) {
//		int len = 0;
//		byte i = 0, chk = 0;
//		byte[] buf = new byte[512];
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x86;
//		buf[len++] = (byte) 0xd6;
//		buf[len++] = 0x00;
//		for (i = 0; i < 4; i++) {
//			buf[len++] = WJVariables.g_d6_data.IssueDate[i];
//		}
//		for (i = 0; i < 8; i++) {
//			buf[len++] = WJVariables.g_d6_data.ContractSerialNumber[i];
//		}
//		buf[len++] = WJVariables.g_d6_data.IssueResult;
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//		}
//	}
//
//	/**
//	 * 函数名称： construct_D7() 功能描述： 构造D7帧 参数列表： port_type -- 端口类型,
//	 * WJVariables.NET:网口, WJVariables.USBCOM:USB转串口 返回结果： void
//	 */
//	public static void construct_D7(int port_type) {
//		int len = 0;
//		byte i = 0, chk = 0;
//		byte[] buf = new byte[512];
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x87;
//		buf[len++] = (byte) 0xd7;
//		buf[len++] = 0x00;
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//		}
//	}
//
//	/**
//	 * 函数名称： construct_D8() 功能描述： 构造D8帧 参数列表： port_type -- 端口类型,
//	 * WJVariables.NET:网口, WJVariables.USBCOM:USB转串口 返回结果： void
//	 */
//	public static void construct_D8(int port_type) {
//		int len = 0;
//		byte i = 0, chk = 0;
//		byte[] buf = new byte[512];
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x87;
//		buf[len++] = (byte) 0xd8;
//		buf[len++] = 0x00;
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//		}
//	}
//
//	/**
//	 * 函数名称： save_E1() 功能描述： 保存E1帧 参数列表： port_type -- 端口类型, WJVariables.NET:网口,
//	 * WJVariables.USBCOM:USB转串口 index -- 序号 返回结果： void
//	 */
//	public static void save_E1(int port_type, int index) {
//		byte[] rx_buf = new byte[512];
//		int pos = 0, i = 0, num = 0;
//		num = (index <= 1000) ? (index - 1) : (index % 1000 - 1);
//		if (port_type == WJVariables.NET) {
//			Cfuncs.memcpy(rx_buf, WJVariables.g_net_rx_buf, 512);
//		} else if (port_type == WJVariables.USBCOM) {
//			Cfuncs.memcpy(rx_buf, WJVariables.g_usbcom_rx_buf, 512);
//		}
//		WJVariables.g_e1_data.RSCTL[num] = rx_buf[pos++];
//		WJVariables.g_e1_data.CMDType[num] = rx_buf[pos++];
//		for (i = 0; i < 8; i++) {
//			WJVariables.g_e1_data.ContractSerialNumber[num][i] = rx_buf[pos++];
//		}
//		for (i = 0; i < 12; i++) {
//			WJVariables.g_e1_data.VehicleLicencePlateNumber[num][i] = rx_buf[pos++];
//		}
//		WJVariables.g_e1_data.IssueResult[num] = rx_buf[pos++];
//	}
//
//	/*
//	 * 函数名称： save_E8() 功能描述： 保存E8帧 参数列表： port_type -- 端口类型, WJVariables.NET:网口,
//	 * WJVariables.USBCOM:USB转串口 返回结果： void
//	 */
//	public static void save_E8(int port_type) {
//		byte[] rx_buf = new byte[512];
//		int i = 0, pos = 0;
//		if (port_type == WJVariables.NET) {
//			Cfuncs.memcpy(rx_buf, WJVariables.g_net_rx_buf, 512);
//		} else if (port_type == WJVariables.USBCOM) {
//			Cfuncs.memcpy(rx_buf, WJVariables.g_usbcom_rx_buf, 512);
//		}
//		WJVariables.g_e8_data.RSCTL = rx_buf[pos++];
//		WJVariables.g_e8_data.CMDType = rx_buf[pos++];
//		for (i = 0; i < 7; i++) {
//			WJVariables.g_e8_data.SystemTime[i] = rx_buf[pos++];
//		}
//		for (i = 0; i < 5; i++) {
//			WJVariables.g_e8_data.Reserved[i] = rx_buf[pos++];
//		}
//	}
//
//	// /*
//	// * 函数名称： envInit()
//	// * 功能描述： 读取配置文件
//	// * 输入参数： 无
//	// * 输出参数： 无
//	// * 返回值： 0--成功,非0--失败
//	// */
//	// public static int envInit()
//	// {
//	// int ret = WJVariables.SUCCESS;
//	// byte[] buf = new byte[128];
//	// Structs.NET_PARAM netParam = new Structs.NET_PARAM();
//	// //Cfuncs.memset(netParam, 0, 574); //byte数组默认初始化为全0
//	// //获取运行模式，即是否打印串口或者网口信息
//	// ret = lib2hd.get_config_key(WJVariables.hdrsu_init_conf, "RUNMODE",
//	// "RunMode", buf);
//	// if(ret == WJVariables.SUCCESS)
//	// {
//	// WJVariables.g_run_mode = (byte) Cfuncs.atoi(buf);
//	// }
//	// else
//	// {
//	// //write_log
//	// lib2hd.recover_hdrsu_conf(WJVariables.hdrsu_init_conf);
//	// }
//	// //获取区域代码代号
//	// Cfuncs.memset(buf, 0, 128);
//	// ret = lib2hd.get_config_key(WJVariables.hdrsu_init_conf, "RUNMODE",
//	// "RegionApply", buf);
//	// if(ret == WJVariables.SUCCESS)
//	// {
//	// WJVariables.g_region_apply = (byte) Cfuncs.atoi(buf);
//	// }
//	// else
//	// {
//	// //write_log
//	// lib2hd.recover_hdrsu_conf(WJVariables.hdrsu_init_conf);
//	// }
//	//
//	// //读配置文件
//	// ret = readNetConf(WJVariables.hdrsu_port_conf, netParam);
//	//
//	// if(ret == WJVariables.SUCCESS)
//	// {
//	// WJVariables.g_server_port = Cfuncs.atoi(netParam.port);
//	// }
//	// else
//	// {
//	// //write_log
//	// lib2hd.recover_hdrsu_conf(WJVariables.hdrsu_init_conf);
//	// }
//	//
//	// Cfuncs.memset(buf, 0, 128);
//	// ret = lib2hd.get_config_key(WJVariables.hdrsu_init_conf, "SERVERINFO",
//	// "ServerPort", buf);
//	// if(ret == WJVariables.SUCCESS)
//	// {
//	// WJVariables.g_server_port = Cfuncs.atoi(buf);
//	// }
//	// else
//	// {
//	// //write_log
//	// lib2hd.recover_hdrsu_conf(WJVariables.hdrsu_init_conf);
//	// }
//	//
//	// return ret;
//	// }
//
//	/*
//	 * 函数名称： recover_config_file() 功能描述： 恢复配置文件 参数列表： 无 返回结果： void
//	 */
//	public static void recover_config_file() {
//		// 复制配置文件到/etc目录下
//		// system("cp /etc/backup/etc_hd_init.conf /etc/ -a");
//	}
//
//	/*
//	 * 函数名称： readNetConf() 功能描述： 输入参数： fileName --文件名，netParam--网络参数 输出参数： 无
//	 * 返回值： 0--成功，非0--失败
//	 */
//	public static int readNetConf(String fileName, Structs.NET_PARAM netParam) {
//		try {
//			File fp = new File(fileName);
//			if (!fp.exists() || !fp.isFile()) {
//				return -1;
//			}
//			FileReader fr = new FileReader(fp);
//
//			int value_index = 0;
//			char[] line = new char[256];
//			String sline, left_value, right_value;
//
//			while (fr.read(line) != 0) {
//				sline = String.valueOf(line);
//				value_index = sline.indexOf("=");
//				if (value_index > 0) {
//					left_value = sline.substring(0, value_index); // 不包括value_index位置的字符
//					right_value = sline.substring(value_index + 1);
//					if (left_value.equals("IP")) {
//						netParam.ip = right_value.getBytes();
//					} else if (left_value.equals("Mask")) {
//						netParam.netmask = right_value.getBytes();
//					} else if (left_value.equals("Gateway")) {
//						netParam.gateway = right_value.getBytes();
//					} else if (left_value.equals("DNS")) {
//						netParam.dns = right_value.getBytes();
//					} else if (left_value.equals("ServerPort")) {
//						netParam.port = right_value.getBytes();
//					} else if (left_value.equals("MAC")) {
//						netParam.mac = right_value.getBytes();
//					}
//				}
//			} // end of while(fr.read(line) != 0)
//
//			fr.close();
//
//		} catch (Exception e) {
//			System.out.println(e.toString());
//			return -1;
//		}
//		return 0;
//	}
//
//	/*
//	 * 函数名称： save_CA(int Port——type) 功能描述： 保存CA帧 参数列表： 无 返回结果： void 张俊丰修改
//	 * 2010.8.16
//	 */
//	public static void save_CA(int port_type) {
//		byte i = 0;
//		byte pos = 0;
//
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_ca_data.RSCTL = WJVariables.g_net_rx_buf[pos++];
//			WJVariables.g_ca_data.CMDType = WJVariables.g_net_rx_buf[pos++];
//			for (i = 0; i < 32; i++)
//				WJVariables.g_ca_data.HardwareName[i] = WJVariables.g_net_rx_buf[pos++];
//			for (i = 0; i < 2; i++)
//				WJVariables.g_ca_data.NumOfPackages[i] = WJVariables.g_net_rx_buf[pos++];
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_ca_data.RSCTL = WJVariables.g_usbcom_rx_buf[pos++];
//			WJVariables.g_ca_data.CMDType = WJVariables.g_usbcom_rx_buf[pos++];
//			for (i = 0; i < 32; i++)
//				WJVariables.g_ca_data.HardwareName[i] = WJVariables.g_usbcom_rx_buf[pos++];
//			for (i = 0; i < 2; i++)
//				WJVariables.g_ca_data.NumOfPackages[i] = WJVariables.g_usbcom_rx_buf[pos++];
//			;
//
//		}
//	}
//
//	/*
//	 * 函数名称： save_CB(int port_type) 功能描述： 保存CB帧 参数列表： 无 返回结果： void 张俊丰修改
//	 * 2010.8.16
//	 */
//	public static void save_CB(int port_type) {
//		int i = 0;
//		int pos = 0;
//		if (port_type == WJVariables.NET) {
//
//			WJVariables.g_cb_data.RSCTL = WJVariables.g_net_rx_buf[pos++];
//			WJVariables.g_cb_data.CMDType = WJVariables.g_net_rx_buf[pos++];
//
//			for (i = 0; i < 4; i++)
//				WJVariables.g_cb_data.PackageSize[i] = WJVariables.g_net_rx_buf[pos++];
//			WJVariables.g_cb_data.FileNameLen = WJVariables.g_net_rx_buf[pos++];
//			for (i = 0; i < WJVariables.g_cb_data.FileNameLen; i++)
//				WJVariables.g_cb_data.FileName[i] = WJVariables.g_net_rx_buf[pos++];
//			for (i = 0; i < 4; i++)
//				WJVariables.g_cb_data.FileSize[i] = WJVariables.g_net_rx_buf[pos++];
//			for (i = 0; i < 2; i++)
//				WJVariables.g_cb_data.BlockSize[i] = WJVariables.g_net_rx_buf[pos++];
//			for (i = 0; i < 2; i++)
//				WJVariables.g_cb_data.NumOfBlocks[i] = WJVariables.g_net_rx_buf[pos++];
//
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_cb_data.RSCTL = WJVariables.g_usbcom_rx_buf[pos++];
//			WJVariables.g_cb_data.CMDType = WJVariables.g_usbcom_rx_buf[pos++];
//
//			for (i = 0; i < 4; i++)
//				WJVariables.g_cb_data.PackageSize[i] = WJVariables.g_usbcom_rx_buf[pos++];
//			WJVariables.g_cb_data.FileNameLen = WJVariables.g_usbcom_rx_buf[pos++];
//			for (i = 0; i < WJVariables.g_cb_data.FileNameLen; i++)
//				WJVariables.g_cb_data.FileName[i] = WJVariables.g_usbcom_rx_buf[pos++];
//			for (i = 0; i < 4; i++)
//				WJVariables.g_cb_data.FileSize[i] = WJVariables.g_usbcom_rx_buf[pos++];
//			for (i = 0; i < 2; i++)
//				WJVariables.g_cb_data.BlockSize[i] = WJVariables.g_usbcom_rx_buf[pos++];
//			for (i = 0; i < 2; i++)
//				WJVariables.g_cb_data.NumOfBlocks[i] = WJVariables.g_usbcom_rx_buf[pos++];
//		}
//	}
//
//	/*
//	 * 函数名称： save_CC(int port_type) 功能描述： 保存CC帧 参数列表： 无 返回结果： void 张俊丰修改
//	 * 2010.8.16
//	 */
//	public static void save_CC(int port_type) {
//		int i = 0;
//		int pos = 0;
//
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_cc_data.RSCTL = WJVariables.g_net_rx_buf[pos++];
//			WJVariables.g_cc_data.CMDType = WJVariables.g_net_rx_buf[pos++];
//
//			for (i = 0; i < WJVariables.g_net_rx_len - 2; i++)
//				WJVariables.g_cc_data.file[i] = WJVariables.g_net_rx_buf[pos++];
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_cc_data.RSCTL = WJVariables.g_usbcom_rx_buf[pos++];
//			WJVariables.g_cc_data.CMDType = WJVariables.g_usbcom_rx_buf[pos++];
//
//			for (i = 0; i < WJVariables.g_usbcom_rx_len - 2; i++)
//				WJVariables.g_cc_data.file[i] = WJVariables.g_usbcom_rx_buf[pos++];
//
//		}
//	}
//
//	/*
//	 * 函数名称： save_CD(int port_type) 功能描述： 保存CC帧 参数列表： 无 返回结果： void 张俊丰修改
//	 * 2010.8.16
//	 */
//	public static void save_CD(int port_type) {
//		byte pos = 0;
//
//		if (port_type == WJVariables.NET) {
//			WJVariables.g_cd_data.RSCTL = WJVariables.g_net_rx_buf[pos++];
//			WJVariables.g_cd_data.CMDType = WJVariables.g_net_rx_buf[pos++];
//			WJVariables.g_cd_data.state = WJVariables.g_net_rx_buf[pos++];
//
//		} else if (port_type == WJVariables.USBCOM) {
//			WJVariables.g_cd_data.RSCTL = WJVariables.g_usbcom_rx_buf[pos++];
//			WJVariables.g_cd_data.CMDType = WJVariables.g_usbcom_rx_buf[pos++];
//			WJVariables.g_cd_data.state = WJVariables.g_usbcom_rx_buf[pos++];
//
//		}
//
//	}
//
//	/*
//	 * 函数名称： save_E9(int port_type) 功能描述： 保存E9帧 参数列表： 返回结果： void
//	 */
//	public static void save_E9(int port_type) {
//		int i = 0;
//		int pos = 0;
//		byte[] rx_buf = new byte[512];
//
//		if (port_type == WJVariables.NET) {
//			Cfuncs.memcpy(rx_buf, WJVariables.g_net_rx_buf, 512);
//		} else if (port_type == WJVariables.USBCOM) {
//			Cfuncs.memcpy(rx_buf, WJVariables.g_usbcom_rx_buf, 512);
//		}
//
//		WJVariables.g_e9_data.RSCTL = rx_buf[pos++];
//		WJVariables.g_e9_data.CMDType = rx_buf[pos++];
//
//		for (i = 0; i < 4; i++) {
//			WJVariables.g_e9_data.MACID[i] = rx_buf[pos++];
//		}
//
//	}
//
//	/*
//	 * 函数名称： construct_D9(int port_type) 功能描述： 构造D9帧 参数列表： 无 返回结果： void 张俊丰修改
//	 * 2010.8.16
//	 */
//	public static void construct_D9(int port_type) {
//		byte i = 0;
//		int len = 0;
//		byte chk = 0;
//		byte[] buf = new byte[512];
//
//		len = 0;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x87;
//		buf[len++] = (byte) 0xd9;
//		buf[len++] = 0x00;
//
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET)
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		else if (port_type == WJVariables.USBCOM)
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//	}
//
//	/*
//	 * 函数名称： construct_DA() 功能描述： 构造DA帧 参数列表： 无 返回结果： void
//	 */
//	// public static void construct_DA(int port_type)
//	// {
//	// byte i = 0;
//	// int len = 0;
//	// byte chk = 0;
//	// byte[] buf = new byte[512];
//	// byte[] tmp = new byte[10];
//	// byte[] rsu_id = new byte[10];
//	//
//	// Cfuncs.memset(tmp, 0, 10);
//	// lib2hd.get_config_key(WJVariables.obuissue_conf, "RSUINFO", "RSUID", tmp);
//	// Cfuncs.str_to_hex(tmp, rsu_id, 4);
//	//
//	// len = 0;
//	// buf[len++]=(byte) 0xff;
//	// buf[len++]=(byte) 0xff;
//	// buf[len++]=(byte) 0x87;
//	// buf[len++]=(byte) 0xda;
//	// buf[len++]=0x00;
//	// for (i=0; i<4; i++)
//	// {
//	// buf[len++]=rsu_id[i]; //RSUID
//	// }
//	//
//	// for (i=0; i<5; i++)
//	// {
//	// buf[len++]=0x00;
//	// }
//	// chk=buf[2];
//	// for (i=3;i<len;i++)
//	// {
//	// chk=(byte) (chk^buf[i]);
//	// }
//	// buf[len++]=(byte) (chk&0xFF);
//	// buf[len++]=(byte) 0xFF;
//	// if(port_type == WJVariables.NET)
//	// WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//	// WJVariables.g_net_tx_buf, len);
//	// else if(port_type == WJVariables.USBCOM)
//	// WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//	// WJVariables.g_usbcom_tx_buf, len);
//	//
//	// }
//
//	/*
//	 * 函数名称： construct_DB(int port_type) 功能描述： 构造DB帧 参数列表： 无 返回结果： void 张俊丰修改
//	 * 2010.8.16
//	 */
//	public static void construct_DB(int port_type) {
//		byte i = 0;
//		int len = 0;
//		byte chk = 0;
//		byte[] buf = new byte[512];
//
//		len = 0;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x87;
//		buf[len++] = (byte) 0xdb;
//		buf[len++] = 0x00;
//
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//
//		if (port_type == WJVariables.NET)
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		else if (port_type == WJVariables.COM)
//			WJVariables.g_com_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_com_tx_buf, len);
//		else if (port_type == WJVariables.USBCOM)
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//	}
//
//	/*
//	 * 函数名称： construct_DC(int port_type) 功能描述： 构造DC帧 参数列表： 无 返回结果： void 张俊丰修改
//	 * 2010.8.16
//	 */
//	public static void construct_DC(int port_type) {
//		byte i = 0;
//		int len = 0;
//		byte chk = 0;
//		byte[] buf = new byte[512];
//
//		len = 0;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x87;
//		buf[len++] = (byte) 0xdc;
//		buf[len++] = 0x00;
//
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET)
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		else if (port_type == WJVariables.COM)
//			WJVariables.g_com_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_com_tx_buf, len);
//		else if (port_type == WJVariables.USBCOM)
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//	}
//
//	/*
//	 * 函数名称： construct_DD(int port_type) 功能描述： 构造DD帧 参数列表： 无 返回结果： void 张俊丰修改
//	 * 2010.8.16
//	 */
//	public static void construct_DD(int port_type) {
//		byte i = 0;
//		int len = 0;
//		byte chk = 0;
//		byte[] buf = new byte[512];
//
//		len = 0;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x87;
//		buf[len++] = (byte) 0xdd;
//		buf[len++] = 0x00;
//
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//
//		if (port_type == WJVariables.NET)
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		else if (port_type == WJVariables.COM)
//			WJVariables.g_com_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_com_tx_buf, len);
//		else if (port_type == WJVariables.USBCOM)
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//	}
//
//	/*
//	 * 函数名称： construct_DE(int port_type) 功能描述： 构造DE帧 参数列表： 无 返回结果： void 张俊丰修改
//	 * 2010.8.16
//	 */
//	public static void construct_DE(int port_type) {
//		byte i = 0;
//		int len = 0;
//		byte chk = 0;
//		byte[] buf = new byte[512];
//
//		len = 0;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0x87;
//		buf[len++] = (byte) 0xde;
//		buf[len++] = 0x00;
//
//		for (i = 0; i < 5; i++) {
//			buf[len++] = 0x00;
//		}
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//
//		if (port_type == WJVariables.NET)
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		else if (port_type == WJVariables.USBCOM)
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//
//	}
//
//	/*
//	 * 函数名称： construct_BA(int port_type) 功能描述： 构造BA帧,上位机网口\串口 参数列表： 无 返回结果： void
//	 * 张俊丰修改 2010.8.16
//	 */
//	public static void construct_BA(int port_type) {
//		byte i = 0;
//		int len = 0;
//		byte chk = 0;
//		byte[] buf = new byte[512];
//
//		len = 0;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = WJVariables.g_ba_data.RSCTL;
//		buf[len++] = WJVariables.g_ba_data.FrameType;
//		buf[len++] = WJVariables.g_ba_data.ErrorCode;
//
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//
//		if (port_type == WJVariables.NET)
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		else if (port_type == WJVariables.USBCOM)
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//	}
//
//	/*
//	 * 函数名称： construct_BB(int port_type) 功能描述： 构造BB帧,上位机网口\串口 参数列表： 无 返回结果： void
//	 * 张俊丰修改 2010.8.16
//	 */
//	public static void construct_BB(int port_type) {
//		byte i = 0;
//		int len = 0;
//		byte chk = 0;
//		byte[] buf = new byte[512];
//
//		len = 0;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = WJVariables.g_bb_data.RSCTL;
//		buf[len++] = WJVariables.g_bb_data.FrameType;
//		buf[len++] = WJVariables.g_bb_data.ErrorCode;
//
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET)
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		else if (port_type == WJVariables.USBCOM)
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//
//	}
//
//	/*
//	 * 函数名称： construct_BC(int port_type) 功能描述： 构造BC帧,上位机网口 参数列表： 无 返回结果： void
//	 * 张俊丰修改 2010.8.16
//	 */
//	public static void construct_BC(int port_type) {
//		byte i = 0;
//		int len = 0;
//		byte chk = 0;
//		byte[] buf = new byte[512];
//
//		len = 0;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = WJVariables.g_bc_data.RSCTL;
//		buf[len++] = WJVariables.g_bc_data.FrameType;
//		buf[len++] = WJVariables.g_bc_data.ErrorCode;
//
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET)
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		else if (port_type == WJVariables.USBCOM)
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//	}
//
//	/*
//	 * 函数名称： construct_BD(int port_type) 功能描述： 构造BC帧,上位机网口 参数列表： 无 返回结果： void
//	 * 张俊丰修改 2010.8.16
//	 */
//	public static void construct_BD(int port_type) {
//		byte i = 0;
//		int len = 0;
//		byte chk = 0;
//		byte[] buf = new byte[512];
//
//		len = 0;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = WJVariables.g_bd_data.RSCTL;
//		buf[len++] = WJVariables.g_bd_data.FrameType;
//		buf[len++] = WJVariables.g_bd_data.ErrorCode;
//
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET)
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		else if (port_type == WJVariables.USBCOM)
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//	}
//
//	/*
//	 * 函数名称： construct_BF(int port_type) 功能描述： 构造BF帧,上位机网口 参数列表： 无 返回结果： void
//	 * 张俊丰修改 2010.8.16
//	 */
//	public static void construct_BF(int port_type) {
//		byte i = 0;
//		int len = 0;
//		byte chk = 0;
//		byte[] buf = new byte[512];
//
//		len = 0;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = (byte) 0xff;
//		buf[len++] = 0x13;
//		buf[len++] = (byte) 0xbf;
//		buf[len++] = 0;
//
//		chk = buf[2];
//		for (i = 3; i < len; i++) {
//			chk = (byte) (chk ^ buf[i]);
//		}
//		buf[len++] = (byte) (chk & 0xFF);
//		buf[len++] = (byte) 0xFF;
//		if (port_type == WJVariables.NET)
//			WJVariables.g_net_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_net_tx_buf, len);
//		else if (port_type == WJVariables.USBCOM)
//			WJVariables.g_usbcom_tx_len = ThreadEx.hd_pkt_code(buf,
//					WJVariables.g_usbcom_tx_buf, len);
//	}

}
