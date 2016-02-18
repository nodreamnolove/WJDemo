package wj.ble.obu.comjar;

import wj.ble.obu.comjar.*;
//import wj.ble.obu.comjar.PROG_ICC0002_INFO;
//import wj.ble.obu.comjar.PROG_ICC0012_INFO;
//import wj.ble.obu.comjar.PROG_ICC0015_INFO;
//import wj.ble.obu.comjar.vehicleDimensionsType;



public class WJStructs {
	public static class vehicleDimensionsType {
		public byte[] vehicleLength = new byte[2]; // ��������
		public byte vehicleWidth; // �������
		public byte vehicleHeight; // �����߶�
	}

	public static class CardConsumeRecord {
		public String applicationId;//1  ����Ӧ�����ͱ�ʶ��
		public String recordLength;//1  ��¼����
		public String applicationLockFlag;//1  Ӧ��������־
		public  String tollRoadNetworkId;//2  ��/�����շ�·����
		public String tollStationId;//2  ��/�����շ�վ��
		public String tollLaneId;//1  ��/�����շѳ�����
		public  String timeUnix;//4  ��/����ʱ�� UNIXʱ��
		public String vehicleModel;//1  ����
		public  String passStatus;//1  �����״̬
		public String reserve1; //9�Զ���Ӧ��
		public String staffNo;//3  �շ�Ա���Ŷ����Ʒ�ʽ������Ա���ź���λ
		public String mtcSequenceNo;//1  ��/���ڰ�� MTC�����շѰ��
		public String vehicleNumber;//12 ���ƺ���
		public String reserve2; //4 �շѹ�·ETCԤ��

	}
	public static class CardTransactionRecord{
		
		public String onlineSn;// 2 �û����ڲ����Ľ�����ˮ��
		public String overdrawLimit;// 3 ͸֧�޶��λ�֣�10���ƣ�
		public String transAmount;// 4 ���׽���λ�֣�10���ƣ�
		public String transType;// 1 �������ͱ�ʶ��02=Ȧ�棬06=���ѣ�
		public String terminalNo;// 6 �ն˻����
		public String transDate; // 4 ��������CCYYMMDD
		public String transTime; // 3 ����ʱ��HHMMSS

	    
	}
	public static class CardOwner {
		public String ownerId;// 1�ֿ�����ݱ�ʶ
		public String staffId;// 1��ϵͳְ����ʶ
		public String ownerName;// 20�ֿ�������
		public String ownerLicenseNumber;// 32�ֿ���֤������
		public String ownerLicenseType;// 1�ֿ���֤������
	}


	public static class PKG_DATA {
		public byte[] contractProvider = new byte[8]; // �����ṩ�̱���
		public byte contractType; // ЭԼ����
		public byte contractVersion; // ��ͬ�汾
		public byte[] contractSerialNumber = new byte[8]; // ��ͬ���к�
		public byte[] contractSignedDate = new byte[4]; // ��ͬǩ������
		public byte[] contractExpiredDate = new byte[4]; // ��ͬ��������
		public byte tamperFlag; // ��ж״̬

		public byte[] vehicleLicencePlateNumber = new byte[12]; // ���ƺ�
		public byte[] vehicleLicencePlateColor = new byte[2]; // ������ɫ
		public byte vehicleClass; // ����
		public byte vehicleUserType; // �����û�����
		public vehicleDimensionsType vehicleDimensions = new vehicleDimensionsType(); //�����ߴ� (�� [2 byte] * ��[1 byte] * ��[1 byte])
		public byte vehicleWheels; // ������
		public byte vehicleAxles; // ������
		public byte[] vehicleWheelBases = new byte[2]; // ���
		public byte[] vehicleWeightLimits = new byte[3]; // ��������/��λ��
		public byte[] vehicleSpecificInformation = new byte[16]; // ������������
		public byte[] vehicleEngineNumber = new byte[16]; // ������������
		public byte[] OBUID = new byte[4];
		public byte[] C3Command = new byte[255];
		public long C3Len;
	}

	public static class PROG_ICC0002_INFO {
		public byte[] IccBanlance = new byte[4];// IC�����
	}

	public static class PROG_ICC0012_INFO {
		public byte[] InRoadNetID = new byte[2]; // ����շ�·����
		public byte[] InRoadID = new byte[2]; // ����շ�վ��
		public byte InRoadLandNO; // ����շѳ�����
		public byte[] InRoadTime = new byte[4]; // ���ʱ��
		public byte VehType; // ����
		public byte InOutStatus; // �����״̬
		public byte[] Identificationstation = new byte[9]; // ��ʶվ
		public byte[] StaffID = new byte[3]; // �շ�Ա����
		public byte InRoadShift; // ��ڰ��
		public byte[] bindedPlate = new byte[12]; // ���ƺ���
		public byte[] OtherInfo = new byte[4]; // ������Ϣ
	}

	public static class PROG_ICC0015_INFO {
		public byte[] cardIssuerID = new byte[8]; // ��������ʶ
		public byte cardType; // ��Ƭ����
		public byte cardVersion; // ��Ƭ�汾��
		public byte[] roadswebID = new byte[2]; // ·�����
		public byte[] cardNo = new byte[8]; // CPU�����
		public byte[] SignedDate = new byte[4]; // ����ʱ��
		public byte[] ExpiredDate = new byte[4]; // ����ʱ��
		public byte[] bindedPlate = new byte[12]; // ���ƺ���
		public byte userType; // �û�����
		public byte[] OtherInfo = new byte[2]; // ����FCI��Ϣ
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
