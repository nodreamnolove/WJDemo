

#import "ObuSDK.h"
#import <UIKit/UIKit.h>
#import <CoreBluetooth/CoreBluetooth.h>
#include "issue_Ble_send.h"
#include "issue_Ble_recv.h"
#include "lib2hd.h"
#include "icc.h"
#include "esam.h"
#include "NSString+NSStringHexToBytes.h"
#import "CardOwnerRecord.h"
#import "CardConsumeRecord.h"
#import "CardTransactionRecord.h"

@interface ObuSDK()<CBCentralManagerDelegate,CBPeripheralManagerDelegate,CBPeripheralDelegate>

@property (nonatomic,copy) NSString * characteristic_uuid; //特征id

@property (nonatomic,copy) NSString * deveice_uuid;//设备id

@property (nonatomic,strong) CBCentralManager * wjCentralManger;

@property (nonatomic,strong) CBPeripheral * connectedPeripheral;

@property (nonatomic,strong) dispatch_queue_t  bluetoothQueue;

@property (nonatomic,strong) CBCharacteristic * readwriteCharacter;

@property (nonatomic,strong) CBCharacteristic * notifyCharacter;

@property (nonatomic, copy) obuCallBack connectBlock;//连接block
@property (nonatomic, copy) obuCallBack disconnectBlock;//断开
@property (nonatomic, copy) obuCallBack getCardInfoBlock;//卡片信息
@property (nonatomic, copy) obuCallBack getOBUInfoBlock;//obu信息

@property (nonatomic,strong) dispatch_semaphore_t  obuSemaphore;

@property (nonatomic,assign,getter=isBlueConnected) BOOL blueConnected;

//连接成功失败
//-(void)setConnectBlockWithSuccess:(void (^)(BOOL status,NSDictionary *data, NSString *errorMsg))success failure:(void (^)(BOOL status,NSDictionary *data, NSString *errorMsg))failure;

@end

@implementation ObuSDK

static bool isStart;
static ObuSDK * _instance;

+(instancetype)allocWithZone:(struct _NSZone *)zone
{
  
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [super allocWithZone:zone];
    });
    return _instance;
}

+(instancetype)sharedObuSDK
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc]init];
    });
    return _instance;
}
-(dispatch_semaphore_t)obuSemaphore
{
    if (_obuSemaphore==nil) {
        _obuSemaphore = dispatch_semaphore_create(0);
    }
    return _obuSemaphore;
}
-(instancetype)init
{
    self = [super init];
    if (self) {
        NSLog(@"init%@",[NSThread currentThread]);
        _bluetoothQueue = dispatch_get_main_queue();//dispatch_queue_create("BlueQueue", NULL);
        _wjCentralManger = [[CBCentralManager alloc]initWithDelegate:self queue:_bluetoothQueue options:@{CBCentralManagerOptionShowPowerAlertKey:@YES}];
        _bluetoothState = NO;
        
    }
    return self;
}

-(void)startScan
{
    [_wjCentralManger scanForPeripheralsWithServices:nil options:@{CBCentralManagerScanOptionAllowDuplicatesKey:@YES}];
    [[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:YES];
}

-(void)stopScan
{
    [_wjCentralManger stopScan];
    [[UIApplication sharedApplication]setNetworkActivityIndicatorVisible:NO];
}
-(void)connectDeviceold:(CBPeripheral *)connectPeripheral
{
    //停止扫描
    [self stopScan];
    [[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:YES];
    self.connectedPeripheral = connectPeripheral;
}

-(void)disconnectDevice
{
    if (self.connectedPeripheral) {
        [_wjCentralManger cancelPeripheralConnection:self.connectedPeripheral];
        self.connectedPeripheral = nil;
    }
}

//本地蓝牙状态
- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    NSLog(@"centralManagerDidUpdateState%@",[NSThread currentThread]);
    if (central.state == CBCentralManagerStatePoweredOn) {
        _bluetoothState = YES;
        [self startScan];
    }
    else if (central.state == CBCentralManagerStatePoweredOff){
        _bluetoothState = NO;
        if (self.connectBlock) {
            self.connectBlock(NO,@(1),@"蓝牙未开启");
        }
    }
}

//存储蓝牙状态
- (void)centralManager:(CBCentralManager *)central willRestoreState:(NSDictionary<NSString *, id> *)dict
{
    NSLog(@"centralManager %@",central);
}

//发现设备
- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI
{
//    NSLog(@"%@++%@----%@",[NSThread currentThread],central,peripheral.name);
//    NSLog(@"&&&&%@",advertisementData);
    // 过滤信号强度范围
    NSLog(@"1、%s",__FUNCTION__);
    if (RSSI.integerValue > -15) {
        return;
    }
    if (RSSI.integerValue < -85) {
        return;
    }
    NSLog(@"2、%s",__FUNCTION__);
    if ([[advertisementData objectForKey:@"kCBAdvDataLocalName"] isEqualToString:@"WanJi_303"]) {
        if(self.isBlueConnected)
            return;
        else
            ;
//        {
//            //取 UUID
//            NSArray *keys = [advertisementData allKeys];
//            for (int i=0; i< [keys count]; ++i) {
//                id  key = [keys objectAtIndex:i];
//                NSString *keyName = (NSString *)key;
//                NSObject *value = [advertisementData objectForKey:key];
//                if ([value isKindOfClass:[NSArray class]]) {
//                    if ([keyName isEqualToString:@"kCBAdvDataServiceUUIDs"]) {
//                        NSArray *values = (NSArray *)value;
//                        for (int j=0; j<[values count]; j++) {
//                            if ([[values objectAtIndex:j] isKindOfClass:[CBUUID class]]) {
//                                CBUUID *obuuuid = [values objectAtIndex:j];
//                                self.deveice_uuid = [obuuuid UUIDString];
//                                break ;
//                            }
//                        }
//                    }
//                }
//            }
//            
            self.connectedPeripheral = peripheral;
        [self.wjCentralManger connectPeripheral:self.connectedPeripheral options:[NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES] forKey:CBConnectPeripheralOptionNotifyOnDisconnectionKey]];
//            [self.wjCentralManger connectPeripheral:self.connectedPeripheral options:nil];//@{CBConnectPeripheralOptionNotifyOnConnectionKey:@(YES)}];
//        }
    }
    else
        return ;
 
}

//连接上设备
- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
   NSLog(@"1、%s",__FUNCTION__);
    self.connectedPeripheral.delegate = self;
    if (!self.deveice_uuid) {
        self.deveice_uuid = [peripheral.identifier UUIDString];
    }
    [self.connectedPeripheral discoverServices:nil];//@[[CBUUID UUIDWithString:self.deveice_uuid]]];
//    NSLog(@"%@",peripheral);
    [self stopScan]; //停止扫描
    
    NSLog(@"%s",__FUNCTION__);
   
}

//连接失败
- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error
{
    self.connectedPeripheral = nil;
    self.deveice_uuid = nil;
    if (self.connectBlock) {
        self.connectBlock(NO,error,nil);
    }
    [self startScan];
}

//断开连接
- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error
{
    self.connectedPeripheral = nil;
    self.blueConnected = NO;
    if (self.disconnectBlock) {
        self.disconnectBlock(YES,nil,nil);
    }
}
//发现服务
-(void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error
{
    if (error) {
        NSLog(@"发现特征错误：%@",error);
        
        return ;
    }
//    if (self.deveice_uuid) {
//        [peripheral discoverCharacteristics:@[[CBUUID UUIDWithString:self.deveice_uuid]] forService:];
//    }
    
//    NSLog(@"发现服务%@",peripheral.services);
//    if (peripheral.services.count>0) {
//        CBService *service  = peripheral.services[0];
//        NSLog(@"%@",service);
//    }
//    
    for (CBService *service  in peripheral.services) {
        if (service.UUID) {
            NSData *uuData = service.UUID.data;
            NSUUID *createuuid = [[NSUUID alloc]initWithUUIDBytes:[uuData bytes]];
            NSLog(@"service.UUID=%@",service.UUID);
            self.characteristic_uuid = [createuuid UUIDString];
            [peripheral discoverCharacteristics:nil forService:service];
//            [peripheral discoverCharacteristics:@[[CBUUID UUIDWithString:self.characteristic_uuid],[CBUUID UUIDWithString:self.deveice_uuid]] forService:service];
        }
    }


}
//发现特征
-(void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error
{
    if (error) {
        
        return;
    }
    for (CBCharacteristic *characteristic in service.characteristics) {
        if(characteristic.properties == (CBCharacteristicPropertyWriteWithoutResponse|CBCharacteristicPropertyWrite))
        {
            self.connectedPeripheral = peripheral;
            self.readwriteCharacter = characteristic;
            
        }
        else if (characteristic.properties == CBCharacteristicPropertyNotify) {
            self.notifyCharacter = characteristic;
            [peripheral setNotifyValue:YES forCharacteristic:characteristic];
            //找到需要的特征 预定成功
        }
    }
}
//发送成功调用
- (void)peripheral:(CBPeripheral *)peripheral didWriteValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    NSLog(@"发送成功调用%s，characteristic=%@",__func__,characteristic);
}

uint8 recvBuffer[40];
int recvLen;
//处理蓝牙发过来的数据
-(void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    if(error){
        NSLog(@"发现特征错误：%@",[error localizedDescription]);
        return;
    }
    NSLog(@"特征值：%@",characteristic.value);
    recvLen = (int)characteristic.value.length;
    [characteristic.value getBytes:recvBuffer length:recvLen];
    if (recvBuffer[0]==0xff&&recvBuffer[1]==0xff)
    {
        isStart = YES;
        g_com_rx_len += characteristic.value.length;
        memcpy(&g_com_rx_buf[g_com_rx_len], recvBuffer, recvLen);
        for (int i=2; i<recvLen; i++) {
            if (recvBuffer[i] == 0xff) {
                dispatch_semaphore_signal(self.obuSemaphore);
                g_com_rx_len += recvLen;
                isStart = NO;
                break;
            }
        }
    }
    else if (isStart == YES)
    {
        for (int i=0; i<recvLen;i++)
        {
            if (recvBuffer[i] == 0xff) {
                if (i==recvLen-1) {
                    memcpy(&g_com_rx_buf[g_com_rx_len], recvBuffer, recvLen);
                    g_com_rx_len += recvLen;
                    dispatch_semaphore_signal(self.obuSemaphore);
                    isStart = NO;
                    return;
                }
                else
                {
                    if (recvBuffer[i+1] != 0xff ) {
                         memcpy(&g_com_rx_buf[g_com_rx_len], recvBuffer, recvLen);
                         g_com_rx_len += recvLen;
                         dispatch_semaphore_signal(self.obuSemaphore);
                        isStart = NO;
                        return;
                    }
                    else{
                        memcpy(&g_com_rx_buf[0], &recvBuffer[i], recvLen-i);
                        g_com_rx_len = recvLen - i;
                        isStart = YES;
                        return;
                    }
                }
            }
        }
        if (g_com_rx_len + recvLen > 1024) {
            g_com_rx_len = 0;
            recvLen = 0;
        }
        memcpy(&g_com_rx_buf[g_com_rx_len], recvBuffer, recvLen);
        g_com_rx_len += recvLen;
    }
    
}
-(void)peripheral:(CBPeripheral *)peripheral didUpdateNotificationStateForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    if (error) {
        NSLog(@"通知更新成功%@",[error localizedDescription]);
        return;
    }
    NSLog(@"通知更新成功%@---%@",characteristic,peripheral);
    if (self.connectBlock) {
        self.connectBlock(YES,@{@"character":characteristic,@"deceive":peripheral},nil);
    }
}


-(void)sendData:(int )length andRepeat:(int )repeatNum
{
//    dispatch_async(dispatch_get_global_queue(0, 0), ^{
    for (int i=0; i<repeatNum; i++) {
            BOOL didsend = YES;
            NSInteger needToSend = 0 ;
            NSInteger sendedIndex = 0;
            NSData *sendData = [NSData dataWithBytes:g_com_tx_buf length:length];
            while (didsend) {
                needToSend = sendData.length-sendedIndex;
                if (needToSend > NOTIFY_MTU) {
                    needToSend = NOTIFY_MTU;
                }
                NSData *dataChunk = [NSData dataWithBytes:g_com_tx_buf+sendedIndex length:needToSend];
                [self.connectedPeripheral writeValue:dataChunk forCharacteristic:self.readwriteCharacter type:CBCharacteristicWriteWithResponse];
                
                sendedIndex += NOTIFY_MTU;
                if (sendedIndex >= sendData.length) {
                    didsend = NO;
                    g_com_rx_len = 0;
                }
                [NSThread sleepForTimeInterval:0.08];
            }
        NSLog(@"%@",[NSThread currentThread]);
           [NSThread sleepForTimeInterval:0.3];
         }
//    });
}



#pragma mark 4.连接OBU
-(void)connectDevice:(obuCallBack)callBack
{
     self.connectBlock = callBack;
    [self startScan];       //开始扫描
}
#pragma mark 5.断开连接
-(void)disconnectDevice:(obuCallBack)callBack
{
    self.disconnectBlock = callBack;
    if (self.isBlueConnected) {
        [self.wjCentralManger cancelPeripheralConnection:self.connectedPeripheral];
    }
    
}
#pragma mark 6.读取OBU的卡片信息 ok
-(void)getCardInformation:(obuCallBack)callBack
{
    int ret;
    self.getCardInfoBlock = callBack;
    //1.发c1
    if([self sendC1AndWaitB1:callBack]!= YES)
        return ; //结束
        //2.发送c9
   PROG_COMM_C4 progc4;
   c4_init(progc4, (byte)0x01);
   int needble2;
    int icc_flag;
    int icc_Length;
   int length = send_c9_Ble1_OC(progc4, &needble2,&icc_flag,&icc_Length);
    //等B9 1
   [self sendData:length andRepeat:1];
    if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3)!=0)
    {
        callBack(NO,nil,@"超时未接收到C9帧");
        return;
    }
    uint8 data[128];
    int datalist;
    if(TransferChannel_rs_OC(&datalist, data, 100)!=SUCCESS)
    {
        callBack(NO,nil,@"B9帧1解析出错");
        return ;//解析出错
    }
    if (needble2==0) {
        if(iccCheck(data, 0)!=SUCCESS)
        {
            callBack(NO,nil,@"B9帧1icc校验失败");
            return ;//icc校验失败
        }
        if (icc_flag == 0x0002) {
            memcpy(icc_pib.Balance, &data[1], icc_Length);	//0002Œƒº˛
        } else if (icc_flag == 0x0012) {
            memcpy(icc_pib.icc0012, &data[1], icc_Length);	//0012Œƒº˛
        } else if (icc_flag == 0x0015) {
            memcpy(icc_pib.icc0015, &data[1], icc_Length);	//0015Œƒº˛
        } else if (icc_flag == 0x0019) {
            memcpy(icc_pib.icc0019, &data[1], icc_Length);	//0019Œƒº˛
        }
    }
    else
    {
         if(esamCheck(data, 0)!=SUCCESS)
         {
             callBack(NO,nil,@"B9帧esam校验失败");
             return ;//esam校验失败
         }
         if(esamCheckReadSysInfo(data, 1)!=SUCCESS)
         {
             callBack(NO,nil,@"B9帧esam信息校验失败");
             return; //esam信息校验失败
         }
        if ((vst.obustatus[0] & 0x80) == 0x00)
        {
            int did = 0x01;
            ST_TRANSFER_CHANNEL transfer_rq;
            iccInitFrame(&transfer_rq);
            iccReadMoneyFrame(&transfer_rq);
            iccReadFileFrame(&transfer_rq, 0x0015, 0x00, 0x2b);
            
            length = TransferChannel_rq_OC(did, transfer_rq.channelid,transfer_rq.apdulist, transfer_rq.apdu);
            [self sendData:length andRepeat:1];
            if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3)!=0)
            {
                callBack(NO,nil,@"超时未接收到B9帧2");
                return;
            }
            
            if(TransferChannel_rs_OC(&datalist, data, 100)!=SUCCESS)
            {
                callBack(NO,nil,@"B9帧2解析出错");
                return ;// -6 + ret * 100; 解析出错
            }
            
            if(iccCheck(data, 0)!=SUCCESS){
                callBack(NO,nil,@"超时未接收到B9帧2icc检验失败");
                return ;//-7 + ret * 100;校验失败

            }
            if(iccCheck(data, 1)!=SUCCESS){
               callBack(NO,nil,@"超时未接收到B9帧2icc检验1失败");
               return ;// -8 + ret * 100;检验失败
            }
            memcpy(icc_pib.Balance, &data[1], 4);
            memcpy(icc_pib.icc0015, &data[8], 43);
        }
    }
    PROG_COMM_B3 prog_b3;
    
    recv_b9_Ble_OC(&prog_b3 ,100);
    
    save_Info_OC(progc4,prog_b3);
    
    if([self sendC5AndWaitB4:callBack]){
        
        NSMutableDictionary *reDict = [NSMutableDictionary dictionaryWithCapacity:11];
        [reDict setObject:[NSString byteToNSString:g_pkg_iccinfo_data.ICC0015_INFO.cardIssuerID andLength:8] forKey:@"cardId"];
        [reDict setObject:[NSString stringByByte:g_pkg_iccinfo_data.ICC0015_INFO.cardType] forKey:@"cardType"];
        [reDict setObject:[NSString stringByByte:g_pkg_iccinfo_data.ICC0015_INFO.cardVersion] forKey:@"cardVersion"];
        [reDict setObject:[NSString byteToNSString:g_pkg_data.contractProvider andLength:8] forKey:@"provider"];
        [reDict setObject:[NSString byteToNSString:g_pkg_iccinfo_data.ICC0015_INFO.SignedDate andLength:4] forKey:@"signedDate"];
        [reDict setObject:[NSString byteToNSString:g_pkg_iccinfo_data.ICC0015_INFO.ExpiredDate andLength:4] forKey:@"expiredDate"];
        [reDict setObject:[NSString byteToNSString:g_pkg_data.vehicleLicencePlateNumber andLength:16] forKey:@"vehicleNumber"];
        [reDict setObject:[NSString stringByByte:g_pkg_data.vehicleUserType] forKey:@"userType"];
        [reDict setObject:[NSString byteToNSString:g_pkg_data.vehicleLicencePlateColor andLength:2] forKey:@"plateColor"];
        [reDict setObject:[NSString stringByByte:g_pkg_data.vehicleClass] forKey:@"vehicleModel"];
        [reDict setObject:[NSString byteToNSString:g_pkg_iccinfo_data.ICC0002_INFO.IccBanlance andLength:4] forKey:@"balance"];
        callBack(YES,reDict,@"成功");
    }
}
#pragma mark  7.读取OBU的设备信息 ok
-(void)getObuInformation:(obuCallBack)callBack
{
    self.getOBUInfoBlock = callBack;
    //发送数据
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        if (![self sendC1AndWaitB1:callBack]) {
            NSLog(@"失败1");
            return;
        }
          NSLog(@"失败2");
        NSString *obuMAC = [NSString byteToNSString:vst.macid andLength:4];
        NSDictionary *dict = @{@"mac":obuMAC,@"name":@"WanJi_303",@"indentify":self.deveice_uuid,@"sn":@""};
        callBack(YES,dict,nil);
    });
}


#pragma mark  8.充值写卡：获取Mac1等数据
-(void)loadCreditGetMac1:(NSString *)credit    cardId:(NSString*)cardId     terminalNo:(NSString *)terminalNo  picCode:(NSString *)pinCode    procType:(NSString*)procType     keyIndex:(NSString *)keyIndex callBack:(obuCallBack)callBack
{
    
    if([self sendC1AndWaitB1:callBack]!= YES)
        return ; //结束
    
    cardId = @"00000000000000000001";
    
    byte* l_abcardId = [[cardId hexToBytes] bytes];
    
    int l_nprocType = 0x01;
    if ([procType isEqualToString:@"ED"]) {
        
        l_nprocType = 0x01;
    } else if ([procType isEqualToString:@"EP"]) {
        
        l_nprocType = 0x02;
    }
    
    int l_nkeyIndex =  [keyIndex integerValue];
    byte* l_abterminalNo = [[terminalNo hexToBytes] bytes];
    byte* l_abpinCode = [[pinCode hexToBytes]bytes];
    
    
    ST_TRANSFER_CHANNEL transfer_rq;
    
    int datalist;
    uint8 data[128];
    int did, i;
    int ret;
    int j = 0;
    did = 0x01;
    int TimeOut = 1000;
    uint8 addmoney[4] = { 0x00, 0x00, 0x00, 0x00 };
    uint8 banlance[4] = { 0x00, 0x00, 0x00, 0x00 };
    uint8 paySerial[2] = { 0x00, 0x00 };
    uint8 passtyperand[4] = { 0x00, 0x00, 0x00, 0x00 };
    uint8 passkey[8] = { 0x00, 0x00, 0x00, 0x00 };
    uint8 dealtime[9] = { 0x20, 0x15, 0x01, 0x01, 0x10, 0x10, 0x10 };
    GetTimebufFunction(dealtime);
    dealtime[4] = dealtime[5];
    dealtime[5] = dealtime[6];
    dealtime[6] = dealtime[7];
    uint8 mac1[10] = { 0 };
    uint8 mac2[10] = { 0 };
    uint8 RecvLen = 0;
    uint8 RecvDate[128];
    iccInitFrame(&transfer_rq);
    icc_getOneDispersed(&transfer_rq);
    int length = TransferChannel_rq_OC(did, transfer_rq.channelid, transfer_rq.apdulist, transfer_rq.apdu);
    [self sendData:length andRepeat:1];
    if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3) != 0)
    {
        callBack(NO,nil,@"超时没有收到TransferChannel_rs");
        return ;//  NSLog(@"超时无响应");
    }
 
    ret = TransferChannel_rs_OC(&datalist, data, TimeOut);
    if (ret != SUCCESS)
    {
        callBack(NO,nil,@" TransferChannel_rs解析出错");
        return ;
    }
    
    ret = iccCheckandGetData(data, 0, g_frame_quancun_rq.OneDispersed);
    
    if (ret != 0)
    {
          callBack(NO,nil,@" iccCheckandGetData解析出错");
        return  ;
    }
    
    iccInitFrame(&transfer_rq);
    
    icc_getTwoDispersed(&transfer_rq);	//ªÒ»°2º∂∑÷…¢“Ú◊”
    
    length = TransferChannel_rq_OC(did, transfer_rq.channelid, transfer_rq.apdulist,transfer_rq.apdu);
    [self sendData:length andRepeat:1];
    if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3) != 0)
    {
        callBack(NO,nil,@"超时没有收到TransferChannel_rs 2");
        return ;//  NSLog(@"超时无响应");
    }

    
    
    ret = TransferChannel_rs_OC(&datalist, data, TimeOut);
    if (ret != SUCCESS)
    {
        callBack(NO,nil,@"TransferChannel_rs解析出错2");
        return ;
    }
    
    ret = iccCheckandGetData(data, 0, g_frame_quancun_rq.TwoDispersed);
    
    if (ret != SUCCESS)
    {
        callBack(NO,nil,@"iccCheckandGetData解析出错2");
        return ;
    }
    
    iccInitFrame(&transfer_rq);
    icc_enter_dir(&transfer_rq, 0x1001);
    icc_check_Pin(&transfer_rq, 3, l_abpinCode);
    
    
    length = TransferChannel_rq_OC(did, transfer_rq.channelid, transfer_rq.apdulist,
                             transfer_rq.apdu);
 
    [self sendData:length andRepeat:1];
    if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3) != 0)
    {
        callBack(NO,nil,@"超时没有收到TransferChannel_rs 3");
        return ;//  NSLog(@"超时无响应");
    }
    
    ret = TransferChannel_rs_OC(&datalist, data, TimeOut);
    if (ret != SUCCESS)
    {
        callBack(NO,nil,@" TransferChannel_rs解析出错3");
        return ;
    }
    
    
    ret = iccCheck(data, 0);
    if (ret != SUCCESS)
    {
        callBack(NO,nil,@" iccCheck校验出错0");
        return ;
        
    }
    
    ret = iccCheck(data, 1);
    if (ret != SUCCESS)
    {
        callBack(NO,nil,@" iccCheck校验出错1");
        return ;
        
    }
    
    for (i = 0; i < 6; i++) {
        g_frame_quancun_init_rs.TerminalNO[i] = l_abterminalNo[i];
    }
    
    iccInitFrame(&transfer_rq);
    
    
//    intToBytes2(l_abcardId, addmoney);
    memcpy(addmoney, l_abcardId, 4);
    
    iccinit_for_loadFrame(&transfer_rq, addmoney, l_abterminalNo, l_nprocType,
                          l_nkeyIndex);
    
    length = TransferChannel_rq_OC(did, transfer_rq.channelid, transfer_rq.apdulist,
                             transfer_rq.apdu);
    [self sendData:length andRepeat:1];
    if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3) != 0)
    {
        callBack(NO,nil,@"超时没有收到TransferChannel_rs 4");
        return ;//  NSLog(@"超时无响应");
    }
    ret = TransferChannel_rs_OC(&datalist, data, TimeOut);
    if (ret != SUCCESS)
    {
        callBack(NO,nil,@" TransferChannel_rs解析出错4");
        return ;
    }
   
    
    ret = iccCheck(data, 0);
 
    if (ret != SUCCESS)
    {
        callBack(NO,nil,@"iccCheck校验出错4");
        return ;
    }
    
    memcpy(RecvDate, &data[1], data[0]);
    
    
    for (i = 0; i < 4; i++) {
        banlance[i] = RecvDate[i];
        g_loadCredit_GetMac1.a_cbb[i] = RecvDate[i];
    }
    for (i = 0; i < 2; i++) {
        paySerial[i] = RecvDate[4 + i];
        g_loadCredit_GetMac1.a_on[i] = RecvDate[4 + i];
        g_frame_quancun_rq.QuancunTradeNo[i] = RecvDate[4 + i];
        
    }
    for (i = 0; i < 4; i++) {
        passtyperand[i] = RecvDate[8 + i];
        g_loadCredit_GetMac1.a_rnd[i] = RecvDate[8 + i];
        g_frame_quancun_rq.BogusRandNumber[i] = RecvDate[8 + i];
        
    }
    for (i = 0; i < 4; i++) {
        g_loadCredit_GetMac1.a_m1[i] = RecvDate[12 + i];
        mac1[i] = RecvDate[12 + i];
        
    }
    
    for (i = 0; i < 4; i++) {
        passtyperand[i] = RecvDate[8 + i];
        g_loadCredit_GetMac1.a_rnd[i] = RecvDate[8 + i];
        g_frame_quancun_rq.BogusRandNumber[i] = RecvDate[8 + i];
    }
    NSMutableDictionary *getMac1Dict = [NSMutableDictionary dictionary];
    NSString *acbb = [NSString byteToNSString:g_loadCredit_GetMac1.a_cbb andLength:4];
    [getMac1Dict setObject:[NSString byteToNSString:g_loadCredit_GetMac1.a_cbb andLength:4] forKey:@"a_cbb"];
    [getMac1Dict setObject:[NSString byteToNSString:g_loadCredit_GetMac1.a_on andLength:2] forKey:@"a_on"];
    [getMac1Dict setObject:[NSString byteToNSString:g_loadCredit_GetMac1.a_rnd andLength:8] forKey:@"a_rnd"];
    [getMac1Dict setObject:[NSString byteToNSString:g_loadCredit_GetMac1.a_m1 andLength:10] forKey:@"a_m1"];
    [getMac1Dict setObject:[NSString byteToNSString:g_loadCredit_GetMac1.a_cid andLength:10] forKey:@"a_cid"];
    [getMac1Dict setObject:[NSString byteToNSString:g_loadCredit_GetMac1.a_on andLength:2] forKey:@"a_pt"];
    callBack(YES,getMac1Dict,nil);
    
    
}

#pragma mark 9.充值写卡：执行写卡操作 FD_SingleMAC tac
-(void)loadCreditWriteCard:(NSString *)dateMAC2 callBack:(obuCallBack)callBack
{
    
    NSData *macData = [dateMAC2 hexToBytes];
    uint8 *mac2 = [macData bytes];
        
    ST_TRANSFER_CHANNEL transfer_rq;
    int datalist;
    uint8 data[128];
    int did, i;
    int ret;
    did = 0x01;
    int TimeOut = 1000;
  
    uint8 l_dealtime[7];
    uint8 l_mac2[11];
    memcpy(l_dealtime, mac2, 7);
    memcpy(l_mac2, &mac2[7], 4);
        
#ifdef	WJOBUTEST
      
    uint8 passkey[8] = { 0x00, 0x00, 0x00, 0x00 };
    uint8 dealtime[9] = { 0x20, 0x15, 0x01, 0x01, 0x10, 0x10, 0x10 };//
    GetTimebufFunction(dealtime);
    dealtime[4] = dealtime[5];
    dealtime[5] = dealtime[6];
    dealtime[6] = dealtime[7];
      
    memcpy(l_dealtime, dealtime, 7);
    
    memcpy(passkey, g_frame_quancun_rs.PassKey, 8);
        
        
    uint8 MacData[30] = { 0 };
    memcpy(&MacData[0], &g_sret.a_pt[0], 4);
    
    MacData[4] = 0x02;
    memcpy(&MacData[5], &g_frame_quancun_init_rs.TerminalNO[0], 6);
   
    memcpy(&MacData[11], &l_dealtime[0], 7);
    
    uint8 Macinit[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
   
//    FD_SingleMAC(passkey, Macinit, 18, MacData, l_mac2);?????
    
#endif
        
    uint8 RecvLen = 0;
    uint8 RecvDate[128];
    iccInitFrame(&transfer_rq);
    icc_loadFrame(&transfer_rq, l_dealtime, l_mac2);	 
    int length = TransferChannel_rq_OC(did, transfer_rq.channelid, transfer_rq.apdulist,transfer_rq.apdu);
    //等B9 1
    [self sendData:length andRepeat:1];
    if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3)!=0)
    {
        callBack(NO,nil,@"超时未接收到TransferChannel帧");
        return;
    }
    
    
    ret = TransferChannel_rs_OC(&datalist, data, TimeOut);
    
    if (ret != SUCCESS)
    {
        callBack(NO,nil,@"解析帧TransferChannel出错");
        return;
    }
    
    
    ret = iccCheck(data, 0);
    if (ret != SUCCESS)
    {
        
        callBack(NO,nil,@"iccCheck检验出错");
        return;
        
    }
    callBack(YES,@{@"tac":@""},nil);
    [self sendC5AndWaitB4:callBack];
    
   
}

#pragma mark 10.读终端交易记录文件 OK
-(void)readCardTransactionRecord:(NSString *)pinCode maxNumber:(NSInteger)maxNumber callBack:(obuCallBack)callBack
{
    if([self sendC1AndWaitB1:callBack]!= YES)
        return ; //结束
    NSData *pinData = [pinCode hexToBytes];
    PROG_COMM_C4 prog_c4;
    NSMutableArray *transactionRecordsArr = [NSMutableArray array];
    for (int i = 0; i < maxNumber; i++) {
        PROG_COMM_C4 progc4;
        init_C4_ReadIccInfo_OC(0x0018,0x01,progc4);
        
        int ret = 0;
    
        int datalist;
        ST_TRANSFER_CHANNEL transfer_rq;
        uint8 data[128];
        int did, i;
        int icc_flag = 0, icc_offset = 0, icc_Length = 0;
        int j = 0;
        did = 0x01;
     
        iccInitFrame(&transfer_rq);
        icc_enter_dir(&transfer_rq, 0x1001);
        icc_check_Pin(&transfer_rq, 3, [pinData bytes]);
     
        
       int length = TransferChannel_rq_OC(did, transfer_rq.channelid, transfer_rq.apdulist,
                                 transfer_rq.apdu);
      
        //等B9 1
        [self sendData:length andRepeat:1];
        if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3)!=0)
        {
            callBack(NO,nil,@"超时未接收到C9帧");
            return;
        }
        ret = TransferChannel_rs_OC(&datalist, data, 100);
        if (ret != 0)
        {
             callBack(NO,nil,@"B9帧1解析出错");
            return ;
        }
        
        ret = iccCheck(data, 0);
        if (ret != 0)
        {
            callBack(NO,nil,@"ICC检验1失败");
            return  ;
        }
        
        ret = iccCheck(data, 1);
        
        if (ret != 0) {
            callBack(NO,nil,@"ICC检验2失败");
            return ;
        }
        
        for (i = 0; i < prog_c4.NumOfFiles; i++) {
            g_read_file.DIDnFID[i] = prog_c4.DIDnFID[i];
            g_read_file.offset[i] = prog_c4.Offset[i];
            g_read_file.len[i] = prog_c4.Length[i];
            
            
            if ((prog_c4.DIDnFID[i] == 0x02) || (prog_c4.DIDnFID[i] == 0x12)
                || (prog_c4.DIDnFID[i] == 0x15) || (prog_c4.DIDnFID[i] == 0x18)
                || (prog_c4.DIDnFID[i] == 0x19))	//∂¡ICø®–≈œ¢
            {
                icc_flag = prog_c4.DIDnFID[i];
               
                icc_offset = prog_c4.Offset[i];
               
                icc_Length = prog_c4.Length[i];
               
            }
            else
            {
                callBack(false,nil,@"DIDnFID错误");
                return ;
            }
            
        }
        
        if ((vst.obustatus[0] & 0x80) == 0x00) {
            did = 0x01;
            iccInitFrame(&transfer_rq);
            iccReadFileFrame(&transfer_rq, icc_flag, icc_offset, icc_Length);
            length  = TransferChannel_rq_OC(did, transfer_rq.channelid,
                                     transfer_rq.apdulist, transfer_rq.apdu);
            [self sendData:length andRepeat:1];
            if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3)!=0)
            {
                callBack(NO,nil,@"超时未接收到C9帧2");
                return;
            }
            
            ret = TransferChannel_rs_OC(&datalist, data, 100);
            
            if (ret != SUCCESS) {
                callBack(NO,nil,@"B9帧2解析出错");
                return  ;	//Ω‚Œˆ ß∞‹ªÚ’ﬂ≥¨ ±
            }
            ret = iccCheck(data, 0);
            if (ret != SUCCESS) {
                callBack(NO,nil,@"ICC2检验出错");
                return  ;
            }
            if (icc_flag == 0x0002) {
                memcpy(icc_pib.Balance, &data[1], icc_Length);	//0002Œƒº˛
            } else if (icc_flag == 0x0012) {
                memcpy(icc_pib.icc0012, &data[1], icc_Length);	//0012Œƒº˛
            } else if (icc_flag == 0x0015) {
                memcpy(icc_pib.icc0015, &data[1], icc_Length);	//0015Œƒº˛
            } else if (icc_flag == 0x0018) {
                memcpy(icc_pib.icc0018, &data[1], icc_Length);	//0019Œƒº˛
            } else if (icc_flag == 0x0019) {
                memcpy(icc_pib.icc0019, &data[1], icc_Length);	//0019Œƒº˛
            }
        } else {
            callBack(NO,nil,@"icc_pib文件异常");
            return ;
        }
        PROG_COMM_B3 prog_b3;
        recv_b9_Blefile_OC(&prog_b3,0x0018);//READ_CPUCARD_FILE_0018
        CardTransactionRecord *transactionRecord = [CardTransactionRecord new];
        transactionRecord.onlineSn = [[NSString stringByByte:prog_b3.FileContent[0][0]]stringByAppendingString:[NSString stringByByte:prog_b3.FileContent[0][1]]];
        byte bmoney[4];
        bmoney[0] = prog_b3.FileContent[0][2];
        bmoney[1] = prog_b3.FileContent[0][3];
        bmoney[2] = prog_b3.FileContent[0][4];
        bmoney[3] = 0;
        float money = byteToFloat(bmoney);
        transactionRecord.overdrawLimit = [NSString stringWithFormat:@"%f",money];
        transactionRecord.terminalNo = [NSString byteToNSString:prog_b3.FileContent[0] fromIndex:10 andLength:6];
        bmoney[0] = prog_b3.FileContent[0][5];
        bmoney[1] = prog_b3.FileContent[0][6];
        bmoney[2] = prog_b3.FileContent[0][7];
        bmoney[3] = prog_b3.FileContent[0][8];
        money = byteToFloat(bmoney);
        transactionRecord.transAmount = [NSString stringWithFormat:@"%f",money];
        transactionRecord.transDate = [NSString stringWithFormat:@"%@%@%@",[NSString stringByByte:prog_b3.FileContent[0][17]],[NSString stringByByte:prog_b3.FileContent[0][18]],[NSString stringByByte:prog_b3.FileContent[0][19]]];
        transactionRecord.transTime = [NSString stringWithFormat:@"%@%@%@",[NSString stringByByte:prog_b3.FileContent[0][20]],[NSString stringByByte:prog_b3.FileContent[0][21]],[NSString stringByByte:prog_b3.FileContent[0][22]]];;
        transactionRecord.transType = [NSString stringByByte:prog_b3.FileContent[0][9]];
        [transactionRecordsArr addObject:transactionRecord];
        save_CpuCardinfo_OC(prog_b3);
    }
    callBack(YES,transactionRecordsArr,nil);
    
    [self sendC5AndWaitB4:callBack];
}

#pragma mark 11.读联网收费复合消费过程文件 ok
-(void)readCardConsumeRecord:(NSInteger)maxNumber callBack:(obuCallBack)callBack
{
    if([self sendC1AndWaitB1:callBack]!= YES)
        return ; //结束
    NSMutableArray *consumeRecordsArr = [NSMutableArray array];
    for (int i=0; i<maxNumber; i++) {
        PROG_COMM_C4 progc4;
        init_C4_ReadIccInfo_OC(0x0019, 0x01, progc4);
        
        int needble2;
        int icc_flag;
        int icc_Length;
        int length = send_c9_Ble1_OC(progc4, &needble2,&icc_flag,&icc_Length);
        //等B9 1
        [self sendData:length andRepeat:1];
        if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3)!=0)
        {
            callBack(NO,nil,@"超时未接收到C9帧");
            return;
        }
        uint8 data[128];
        int datalist;
        if(TransferChannel_rs_OC(&datalist, data, 100)!=SUCCESS)
        {
            callBack(NO,nil,@"B9帧1解析出错");
            return ;//解析出错
        }
        if (needble2==0) {
            if(iccCheck(data, 0)!=SUCCESS)
            {
                callBack(NO,nil,@"B9帧1icc校验失败");
                return ;//icc校验失败
            }
            if (icc_flag == 0x0002) {
                memcpy(icc_pib.Balance, &data[1], icc_Length);	//0002Œƒº˛
            } else if (icc_flag == 0x0012) {
                memcpy(icc_pib.icc0012, &data[1], icc_Length);	//0012Œƒº˛
            } else if (icc_flag == 0x0015) {
                memcpy(icc_pib.icc0015, &data[1], icc_Length);	//0015Œƒº˛
            } else if (icc_flag == 0x0019) {
                memcpy(icc_pib.icc0019, &data[1], icc_Length);	//0019Œƒº˛
            }
        }
        else
        {
            if(esamCheck(data, 0)!=SUCCESS)
            {
                callBack(NO,nil,@"B9帧esam校验失败");
                return ;//esam校验失败
            }
            if(esamCheckReadSysInfo(data, 1)!=SUCCESS)
            {
                callBack(NO,nil,@"B9帧esam信息校验失败");
                return; //esam信息校验失败
            }
            if ((vst.obustatus[0] & 0x80) == 0x00)
            {
                int did = 0x01;
                ST_TRANSFER_CHANNEL transfer_rq;
                iccInitFrame(&transfer_rq);
                iccReadMoneyFrame(&transfer_rq);
                iccReadFileFrame(&transfer_rq, 0x0015, 0x00, 0x2b);
                
                length = TransferChannel_rq_OC(did, transfer_rq.channelid,transfer_rq.apdulist, transfer_rq.apdu);
                [self sendData:length andRepeat:1];
                if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3)!=0)
                {
                    callBack(NO,nil,@"超时未接收到B9帧2");
                    return;
                }
                
                if(TransferChannel_rs_OC(&datalist, data, 100)!=SUCCESS)
                {
                    callBack(NO,nil,@"B9帧2解析出错");
                    return ;// -6 + ret * 100; 解析出错
                }
                
                if(iccCheck(data, 0)!=SUCCESS){
                    callBack(NO,nil,@"超时未接收到B9帧2icc检验失败");
                    return ;//-7 + ret * 100;校验失败
                    
                }
                if(iccCheck(data, 1)!=SUCCESS){
                    callBack(NO,nil,@"超时未接收到B9帧2icc检验1失败");
                    return ;// -8 + ret * 100;检验失败
                }
                memcpy(icc_pib.Balance, &data[1], 4);
                memcpy(icc_pib.icc0015, &data[8], 43);
            }
        }
        PROG_COMM_B3 prog_b3;
        recv_b9_Blefile_OC(&prog_b3,0x0019);//READ_CPUCARD_FILE_0019
        CardConsumeRecord *consumeRecord = [CardConsumeRecord new];
        consumeRecord.applicationId = [NSString stringByByte:prog_b3.FileContent[0][0]];
        consumeRecord.recordLength = [NSString stringByByte:prog_b3.FileContent[0][1]];
        consumeRecord.applicationLockFlag = [NSString stringByByte:prog_b3.FileContent[0][2]];
        consumeRecord.tollRoadNetworkId = [[NSString stringByByte:prog_b3.FileContent[0][3]]stringByAppendingString:[NSString stringByByte:prog_b3.FileContent[0][4]]];
        
        consumeRecord.tollStationId = [[NSString stringByByte:prog_b3.FileContent[0][5]]stringByAppendingString:[NSString stringByByte:prog_b3.FileContent[0][6]]];
        
        consumeRecord.tollLaneId = [NSString stringByByte:prog_b3.FileContent[0][7]];
        consumeRecord.timeUnix = [NSString byteToNSString:prog_b3.FileContent[0] fromIndex:8 andLength:4];
        consumeRecord.vehicleModel = [NSString stringByByte:prog_b3.FileContent[0][12]];
        consumeRecord.passStatus = [NSString stringByByte:prog_b3.FileContent[0][13]];
        consumeRecord.reserve1 = [NSString byteToNSString:prog_b3.FileContent[0] fromIndex:14 andLength:9];
        consumeRecord.staffNo = [NSString byteToNSString:prog_b3.FileContent[0] fromIndex:23 andLength:3];;
        consumeRecord.mtcSequenceNo = [NSString stringByByte:prog_b3.FileContent[0][26]];
        consumeRecord.vehicleNumber = [NSString byteToNSString:prog_b3.FileContent[0] fromIndex:27 andLength:12];

        consumeRecord.reserve2 = [NSString byteToNSString:prog_b3.FileContent[0] fromIndex:39 andLength:4];

        [consumeRecordsArr addObject:consumeRecord];
        save_CpuCardinfo_OC(prog_b3);
    }
    
    callBack(YES,consumeRecordsArr,nil);
    
     [self sendC5AndWaitB4:callBack];
}

#pragma mark  12.读持卡人基本数据文件 ok
-(void)readCardOwnerRecord:(obuCallBack)callBack
{
    self.getCardInfoBlock = callBack;
    //1.发c1
    if([self sendC1AndWaitB1:callBack]!= YES)
        return ; //结束
    PROG_COMM_C4 prog_c4;
    init_C4_ReadIccInfo_OC(0x0016,(byte)1,prog_c4);
    int ret = 0;
    int datalist;
    ST_TRANSFER_CHANNEL transfer_rq;
    uint8 data[128];
    uint8 pinCode[4]={0};
    int did, i;
    int icc_flag = 0, icc_offset = 0, icc_Length = 0;
    int j = 0;
    did = 0x01;
    iccInitFrame(&transfer_rq);
    icc_enter_dir(&transfer_rq, 0x1001);
    icc_check_Pin(&transfer_rq, 3, pinCode);
    int length;
    length = TransferChannel_rq_OC(did, transfer_rq.channelid, transfer_rq.apdulist,
                             transfer_rq.apdu);
    
    [self sendData:length andRepeat:1];
    if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3)!=0)
    {
        callBack(NO,nil,@"超时未接收到TransferChannel");
        return;
    }
    
    if(TransferChannel_rs_OC(&datalist, data, 100)!= SUCCESS)
    {
        callBack(NO,nil,@"TransferChannel解析错误1");
        return;
    }
   
    if(iccCheck(data, 0)!=SUCCESS)
    {
        callBack(NO,nil,@"iccCheck检验错误");
        return;
//        return -3 + ret * 100;
    }
    
    if(iccCheck(data, 1)!=SUCCESS)
    {
        callBack(NO,nil,@"iccCheck检验错误2");
        return ;
//        return -4 + ret * 100;
    }
    
    for (int i = 0; i < prog_c4.NumOfFiles; i++) {
        g_read_file.DIDnFID[i] = prog_c4.DIDnFID[i];
        g_read_file.offset[i] = prog_c4.Offset[i];
        g_read_file.len[i] = prog_c4.Length[i];
        if ((prog_c4.DIDnFID[i] == 0x02) || (prog_c4.DIDnFID[i] == 0x12)
            || (prog_c4.DIDnFID[i] == 0x15) || (prog_c4.DIDnFID[i] == 0x16)
            || (prog_c4.DIDnFID[i] == 0x18) || (prog_c4.DIDnFID[i] == 0x19))//∂¡ICø®–≈œ¢
        {
            icc_flag = prog_c4.DIDnFID[i];
            icc_offset = prog_c4.Offset[i];
            icc_Length = prog_c4.Length[i];
            
        } else
        {
            callBack(NO,nil,@"NumOfFiles 不存在02 12 15 16 18 19 文件");
            return ;
        }
    }
    
    if ((vst.obustatus[0] & 0x80) == 0x00) {
        iccInitFrame(&transfer_rq);
        icc_enter_dir(&transfer_rq, 0x3F00);
        length =TransferChannel_rq_OC(did, transfer_rq.channelid,transfer_rq.apdulist, transfer_rq.apdu);
        [self sendData:length andRepeat:1];
        if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3)!=0)
        {
            callBack(NO,nil,@"超时未接收到TransferChannel2");
            return;
        }
       
        if(TransferChannel_rs(&datalist, data, 100)!=SUCCESS)
        {
            callBack(NO,nil,@" TransferChannel2解析错误");
            return;
        }
        did = 0x00;
        iccInitFrame(&transfer_rq);
        iccReadFileFrame(&transfer_rq, icc_flag, icc_offset, icc_Length);
        length = TransferChannel_rq(did, transfer_rq.channelid,transfer_rq.apdulist, transfer_rq.apdu);
        [self sendData:length andRepeat:1];
        if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3)!=0)
        {
            callBack(NO,nil,@"超时未接收到TransferChannel3");
            return;
        }
        if(TransferChannel_rs(&datalist, data, 100)!=SUCCESS)
        {
            callBack(NO,nil,@"TransferChannel3解析错误");
            return;
        }
        
        if(iccCheck(data, 0) != SUCCESS)
        {
            callBack(NO,nil,@"iccCheck 3解析错误");
            return ;
        }
        if (icc_flag == 0x0002) {
            memcpy(icc_pib.Balance, &data[1], icc_Length);
        } else if (icc_flag == 0x0012) {
            memcpy(icc_pib.icc0012, &data[1], icc_Length);
        } else if (icc_flag == 0x0015) {
            memcpy(icc_pib.icc0015, &data[1], icc_Length);
        } else if (icc_flag == 0x0016) {
            memcpy(icc_pib.icc0016, &data[1], icc_Length);
        } else if (icc_flag == 0x0018) {
            memcpy(icc_pib.icc0018, &data[1], icc_Length);
        } else if (icc_flag == 0x0019) {
            memcpy(icc_pib.icc0019, &data[1], icc_Length);
        }
    } else {
        callBack(NO,nil,@"VST状态位错误");
        return ;
    }
    PROG_COMM_B3 prog_b3;
    recv_b9_Blefile_OC(&prog_b3,0x0016);//READ_CPUCARD_FILE_0016
    save_CpuCardinfo_OC(prog_b3);
    CardOwnerRecord *ower = [CardOwnerRecord new];
    ower.ownerId = [NSString stringByByte:prog_b3.FileContent[0][0]];
    ower.staffId = [NSString stringByByte:prog_b3.FileContent[0][1]];;
    ower.ownerName = [NSString byteToNSString:prog_b3.FileContent[0] andLength:20];;
    ower.ownerLicenseNumber = [NSString byteToNSString:prog_b3.FileContent[0] fromIndex:22 andLength:32];
    ower.ownerLicenseType = [NSString stringByByte:prog_b3.FileContent[0][54]];
    callBack(YES,ower,nil);
    [self sendC5AndWaitB4:callBack];
}

#pragma mark 13.数据透传
-(void)transCommand:(NSData*)reqData callBack:(obuCallBack)callBack
{
    
}
-(BOOL)sendC1AndWaitB1:(obuCallBack)callBack
{
    int length = 0;
    int ret ;
    PROG_COMM_C1 progc1;
    PROG_COMM_B1 progb1;
    //发送C1
    length = send_c1_Ble_OC(progc1);
    g_com_rx_len = 0;
    g_com_needrx_len = 50;
    [self sendData:length andRepeat:1];
    if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3) != 0)//
    {
        NSLog(@"超时没有收到BST");
        callBack(NO,nil,@"超时没有收到BST");
        return NO;//  NSLog(@"超时无响应");
    }
      NSLog(@"第3步");
    //1.解析B1
    if((ret=recv_b1_Ble_OC(&progb1, 20))!=SUCCESS)
    {
        NSString *errMsg = [NSString stringWithFormat:@"BST解析出错:%d",ret];
        callBack(NO,nil,errMsg);
        NSLog(@"b1解析出错");
        return NO;//b1解析出错
    }
    return YES;
}
-(BOOL)sendC5AndWaitB4:(obuCallBack)callBack
{
    int length=0;
    //4.发送c5
    PROG_COMM_C5 progc5;
    length = send_c5_Ble_OC(progc5);
    if (length<1) {
        return NO;
    }
    //5.解析b4
    if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3)!=0){
        callBack(NO,nil,@"超时没有收到B4");
        return NO;//超时未收到数据
    }
    PROG_COMM_B4 progb4;
    if(recv_b4_Ble_OC(&progb4,1)!=SUCCESS)
    {
        callBack(NO,nil,@"B4解析出错");
        return NO;//解析出错
    }
    length = EVENT_REPORT_rq_OC(0, 0);
    if (length<1) {
        return NO;
    }
    [self sendData:length andRepeat:1];
    return YES;
}


@end
