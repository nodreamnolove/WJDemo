package wj.ble.obu.comjar;

import wj.ble.obu.comjar.*;
//import wj.ble.obu.comjar.PROG_ICC0002_INFO;
//import wj.ble.obu.comjar.PROG_ICC0012_INFO;
//import wj.ble.obu.comjar.PROG_ICC0015_INFO;
//import wj.ble.obu.comjar.vehicleDimensionsType;



public class WJStructs {
	public static class vehicleDimensionsType {
		public byte[] vehicleLength = new byte[2]; // 车辆长度
		public byte vehicleWidth; // 车辆宽度
		public byte vehicleHeight; // 车辆高度
	}

	public static class CardConsumeRecord {
		public String applicationId;//1  复合应用类型标识符
		public String recordLength;//1  记录长度
		public String applicationLockFlag;//1  应用锁定标志
		public  String tollRoadNetworkId;//2  入/出口收费路网号
		public String tollStationId;//2  入/出口收费站号
		public String tollLaneId;//1  入/出口收费车道号
		public  String timeUnix;//4  入/出口时间 UNIX时间
		public String vehicleModel;//1  车型
		public  String passStatus;//1  入出口状态
		public String reserve1; //9自定义应用
		public String staffNo;//3  收费员工号二进制方式存放入口员工号后六位
		public String mtcSequenceNo;//1  入/出口班次 MTC车道收费班次
		public String vehicleNumber;//12 车牌号码
		public String reserve2; //4 收费公路ETC预留

	}
	public static class CardTransactionRecord{
		
		public String onlineSn;// 2 用户卡内产生的交易流水号
		public String overdrawLimit;// 3 透支限额（单位分，10进制）
		public String transAmount;// 4 交易金额（单位分，10进制）
		public String transType;// 1 交易类型标识（02=圈存，06=消费）
		public String terminalNo;// 6 终端机编号
		public String transDate; // 4 交易日期CCYYMMDD
		public String transTime; // 3 交易时间HHMMSS

	    
	}
	public static class CardOwner {
		public String ownerId;// 1持卡人身份标识
		public String staffId;// 1本系统职工标识
		public String ownerName;// 20持卡人姓名
		public String ownerLicenseNumber;// 32持卡人证件号码
		public String ownerLicenseType;// 1持卡人证件类型
	}


	public static class PKG_DATA {
		public byte[] contractProvider = new byte[8]; // 服务提供商编码
		public byte contractType; // 协约类型
		public byte contractVersion; // 合同版本
		public byte[] contractSerialNumber = new byte[8]; // 合同序列号
		public byte[] contractSignedDate = new byte[4]; // 合同签署日期
		public byte[] contractExpiredDate = new byte[4]; // 合同过期日期
		public byte tamperFlag; // 拆卸状态

		public byte[] vehicleLicencePlateNumber = new byte[12]; // 车牌号
		public byte[] vehicleLicencePlateColor = new byte[2]; // 车牌颜色
		public byte vehicleClass; // 车型
		public byte vehicleUserType; // 车辆用户类型
		public vehicleDimensionsType vehicleDimensions = new vehicleDimensionsType(); //车辆尺寸 (长 [2 byte] * 宽[1 byte] * 高[1 byte])
		public byte vehicleWheels; // 车轮数
		public byte vehicleAxles; // 车轴数
		public byte[] vehicleWheelBases = new byte[2]; // 轴距
		public byte[] vehicleWeightLimits = new byte[3]; // 车辆载重/座位数
		public byte[] vehicleSpecificInformation = new byte[16]; // 车辆特征描述
		public byte[] vehicleEngineNumber = new byte[16]; // 车辆发动机号
		public byte[] OBUID = new byte[4];
		public byte[] C3Command = new byte[255];
		public long C3Len;
	}

	public static class PROG_ICC0002_INFO {
		public byte[] IccBanlance = new byte[4];// IC卡余额
	}

	public static class PROG_ICC0012_INFO {
		public byte[] InRoadNetID = new byte[2]; // 入口收费路网号
		public byte[] InRoadID = new byte[2]; // 入口收费站号
		public byte InRoadLandNO; // 入口收费车道号
		public byte[] InRoadTime = new byte[4]; // 入口时间
		public byte VehType; // 车型
		public byte InOutStatus; // 入出口状态
		public byte[] Identificationstation = new byte[9]; // 标识站
		public byte[] StaffID = new byte[3]; // 收费员工号
		public byte InRoadShift; // 入口班次
		public byte[] bindedPlate = new byte[12]; // 车牌号码
		public byte[] OtherInfo = new byte[4]; // 其他信息
	}

	public static class PROG_ICC0015_INFO {
		public byte[] cardIssuerID = new byte[8]; // 发卡方标识
		public byte cardType; // 卡片类型
		public byte cardVersion; // 卡片版本号
		public byte[] roadswebID = new byte[2]; // 路网编号
		public byte[] cardNo = new byte[8]; // CPU卡编号
		public byte[] SignedDate = new byte[4]; // 启用时间
		public byte[] ExpiredDate = new byte[4]; // 到期时间
		public byte[] bindedPlate = new byte[12]; // 车牌号码
		public byte userType; // 用户类型
		public byte[] OtherInfo = new byte[2]; // 其他FCI信息
	}

	public static class PKG_ICCINFO_DATA {
		public PROG_ICC0002_INFO ICC0002_INFO = new PROG_ICC0002_INFO();
		public PROG_ICC0012_INFO ICC0012_INFO = new PROG_ICC0012_INFO();
		public PROG_ICC0015_INFO ICC0015_INFO = new PROG_ICC0015_INFO();
	}
	
	public static class FLASH_COUNT {
		public PROG_ICC0002_INFO ICC0002_INFO = new PROG_ICC0002_INFO();
		public PROG_ICC0012_INFO ICC0012_INFO = new PROG_ICC0012_INFO();
		public PROG_ICC0015_INFO ICC0015_INFO = new PROG_ICC0015_INFO();
	}
	
}
