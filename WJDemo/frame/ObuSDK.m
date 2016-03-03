

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
-(void)setConnectBlockWithSuccess:(void (^)(BOOL status,NSDictionary *data, NSString *errorMsg))success failure:(void (^)(BOOL status,NSDictionary *data, NSString *errorMsg))failure;

@end

@implementation ObuSDK

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
    if (RSSI.integerValue > -15) {
        return;
    }
    if (RSSI.integerValue < -85) {
        return;
    }
 
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
  
    self.connectedPeripheral.delegate = self;
    if (!self.deveice_uuid) {
        self.deveice_uuid = [peripheral.identifier UUIDString];
    }
    [self.connectedPeripheral discoverServices:nil];//@[[CBUUID UUIDWithString:self.deveice_uuid]]];
//    NSLog(@"%@",peripheral);
    [self stopScan]; //停止扫描
    
    
    if (self.connectBlock) {
        self.connectBlock(YES,nil,nil);
    }
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


//处理蓝牙发过来的数据
-(void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    if(error){
        NSLog(@"发现特征错误：%@",[error localizedDescription]);
        return;
    }
    [characteristic.value getBytes:g_com_rx_buf+g_com_rx_len length:characteristic.value.length];
    g_com_rx_len += characteristic.value.length;
    
    if (g_com_rx_len > g_com_needrx_len) {
        NSData *printData = [NSData dataWithBytes:g_com_rx_buf length:g_com_rx_len];
        NSLog(@"接收到的数据：%@",printData);
        dispatch_semaphore_signal(self.obuSemaphore);
    }
    NSLog(@"测试：%@",characteristic);
}
-(void)peripheral:(CBPeripheral *)peripheral didUpdateNotificationStateForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    if (error) {
        NSLog(@"通知更新成功%@",[error localizedDescription]);
        return;
    }
    NSLog(@"通知更新成功%@---%@",characteristic,peripheral);
}


-(void)sendData:(int )length andRepeat:(int )repeatNum
{
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
    if (![self sendC1AndWaitB1:callBack]) {
        return;
    }
    
    NSString *obuMAC = [NSString byteToNSString:vst.macid andLength:4];
    NSDictionary *dict = @{@"mac":obuMAC,@"name":@"WanJi_303",@"indentify":self.deveice_uuid,@"sn":@""};
    callBack(YES,dict,nil);
    
}


#pragma mark  8.充值写卡：获取Mac1等数据
-(void)loadCreditGetMac1:(NSString *)credit    cardId:(NSString*)cardId     terminalNo:(NSString *)terminalNo  picCode:(NSString *)pinCode    procType:(NSString*)procType     keyIndex:(NSString *)keyIndex callBack:(obuCallBack)callBack
{
    //1.发送c1≥
    PROG_COMM_C1 progc1;
    PROG_COMM_B1 progb1;
    if ( dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*20) == 0)
    {
        int length = send_c1_Ble_OC(progc1);
        NSLog(@"length=%d",length);
        g_com_rx_len = 0;
        g_com_needrx_len = 50;
        [self sendData:length andRepeat:1];
        if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3) == 0) {
            //1.解析bst
            if(recv_b1_Ble_OC(&progb1, 20)==SUCCESS)
            {
                //2.发送TransferChannel_rq≥
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
                
                uint8 dealtime[9] = { 0x20, 0x15, 0x01, 0x01, 0x10, 0x10, 0x10 };//DateTime.Now.ToString("yyyyMMddHHmmss");
                
                
                GetTimebufFunction(dealtime);
                dealtime[4] = dealtime[5];
                dealtime[5] = dealtime[6];
                dealtime[6] = dealtime[7];
                
                uint8 mac1[10] = { 0 };
                uint8 mac2[10] = { 0 };
                uint8 RecvLen = 0;
                uint8 RecvDate[128];
                iccInitFrame(&transfer_rq);
                icc_getOneDispersed(&transfer_rq);	//ªÒ»°1º∂∑÷…¢“Ú◊”
                ret = TransferChannel_rq_OC(did, transfer_rq.channelid, transfer_rq.apdulist, transfer_rq.apdu);
                
                //等待TransferChannel_rs≤
                //3.发送TransferChannel_rq≥
                //等待TransferChannel_rs≤
                //4.发送TransferChannel_rq≥
                //等待TransferChannel_rs≤
            }
            else{
                NSLog(@"解析出错");
            }
            
        }
        else{
            NSLog(@"超时无响应");
            return ;
        }
        
        
    }
    
    
    
}

#pragma mark 9.充值写卡：执行写卡操作
-(void)loadCreditWriteCard:(NSString *)dateMAC2 callBack:(obuCallBack)callBack
{
    
}

#pragma mark 10.读终端交易记录文件
-(void)readCardTransactionRecord:(NSString *)pinCode maxNumber:(NSInteger)maxNumber callBack:(obuCallBack)callBack
{
    
    
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
    if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3) != 0)
    {
        callBack(NO,nil,@"超时没有收到BST");
        return NO;//  NSLog(@"超时无响应");
    }
    //1.解析B1
    if((ret=recv_b1_Ble_OC(&progb1, 20))!=SUCCESS)
    {
        NSString *errMsg = [NSString stringWithFormat:@"BST解析出错:%d",ret];
        callBack(NO,nil,errMsg);
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


int jni2loadCreditGetMac1(uint8* cardId, int credit, uint8* terminalNo,
                          uint8* pinCode, int procType, int keyIndex,
                          loadCreditGetMac1Ret* data_ret, int TimeOut) {
    
    ST_TRANSFER_CHANNEL transfer_rq;
    
    int datalist;
    uint8 data[128];
    int did, i;
    int ret;
    int j = 0;
    did = 0x01;
    TimeOut = 1000;
    uint8 addmoney[4] = { 0x00, 0x00, 0x00, 0x00 };
    uint8 banlance[4] = { 0x00, 0x00, 0x00, 0x00 };		//(0, 4)”‡∂Ó
    uint8 paySerial[2] = { 0x00, 0x00 };		//(4, 2)¡™ª˙Ωª“◊–Ú∫≈
    uint8 passtyperand[4] = { 0x00, 0x00, 0x00, 0x00 };		//(8, 4)Œ±ÀÊª˙ ˝
    
    uint8 passkey[8] = { 0x00, 0x00, 0x00, 0x00 };	//π˝≥Ã√ÿ‘ø
    
    uint8 dealtime[9] = { 0x20, 0x15, 0x01, 0x01, 0x10, 0x10, 0x10 };//DateTime.Now.ToString("yyyyMMddHHmmss");
    
  
    GetTimebufFunction(dealtime);
    dealtime[4] = dealtime[5];
    dealtime[5] = dealtime[6];
    dealtime[6] = dealtime[7];
    
    uint8 mac1[10] = { 0 };	//(12,4)MAC1¬Î
    uint8 mac2[10] = { 0 };	//MAC2¬Î
    
    uint8 RecvLen = 0;
    uint8 RecvDate[128];
    
   
    iccInitFrame(&transfer_rq);
    
 
    icc_getOneDispersed(&transfer_rq);	//ªÒ»°1º∂∑÷…¢“Ú◊”
  
   
    ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist, transfer_rq.apdu);
    if (ret != 0)	//¥Æø⁄∑¢ÀÕ ß∞‹
        return -3 + ret * 100;
    
  
    ret = TransferChannel_rs(&datalist, data, TimeOut);
    if (ret != 0)	//Ω‚Œˆ ß∞‹ªÚ’ﬂ≥¨ ±
        return -4 + ret * 100;
    

    ret = iccCheckandGetData(data, 0, g_frame_quancun_rq.OneDispersed);
    
    if (ret != 0)
        return -5 + ret * 100;	//»°“ªº∂∑÷…¢“Ú◊” ß∞‹÷±Ω”ÕÀ≥ˆ
 
 
    iccInitFrame(&transfer_rq);
   
    icc_getTwoDispersed(&transfer_rq);	//ªÒ»°2º∂∑÷…¢“Ú◊”

    ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
                             transfer_rq.apdu);
    if (ret != 0)	//¥Æø⁄∑¢ÀÕ ß∞‹
        return -6 + ret * 100;
    
    ret = TransferChannel_rs(&datalist, data, TimeOut);
    if (ret != 0)	//Ω‚Œˆ ß∞‹ªÚ’ﬂ≥¨ ±
        return -7 + ret * 100;
    

    ret = iccCheckandGetData(data, 0, g_frame_quancun_rq.TwoDispersed);
    
    if (ret != 0)
        return -8 + ret * 100;	//»°∂˛º∂∑÷…¢“Ú◊” ß∞‹÷±Ω”ÕÀ≥ˆ

    
   
    iccInitFrame(&transfer_rq);
    icc_enter_dir(&transfer_rq, 0x1001);	//—°‘Ò1001
    icc_check_Pin(&transfer_rq, 3, pinCode);	//—È÷§PIN
 
    
    ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
                             transfer_rq.apdu);
    if (ret != 0)	//¥Æø⁄∑¢ÀÕ ß∞‹
        return -9 + ret * 100;
    
    ret = TransferChannel_rs(&datalist, data, TimeOut);
    if (ret != 0)	//Ω‚Œˆ ß∞‹ªÚ’ﬂ≥¨ ±
        return -10 + ret * 100;
    
  
    ret = iccCheck(data, 0);
    if (ret != 0)	//Ω” ’÷°–£—È ß∞‹
        return -11 + ret * 100;
    
    ret = iccCheck(data, 1);

    if (ret != 0)	//Ω” ’÷°–£—È ß∞‹
        return -12 + ret * 100;
    
   
    for (i = 0; i < 6; i++) {
        
        g_frame_quancun_init_rs.TerminalNO[i] = terminalNo[i];
    }

    iccInitFrame(&transfer_rq);
  
    
    intToBytes2(credit, addmoney);
    
    iccinit_for_loadFrame(&transfer_rq, addmoney, terminalNo, procType,
                          keyIndex);
   
    ret = TransferChannel_rq(did, transfer_rq.channelid, transfer_rq.apdulist,
                             transfer_rq.apdu);
  
    if (ret != 0)	//¥Æø⁄∑¢ÀÕ ß∞‹
        return -13 + ret * 100;
    ret = TransferChannel_rs(&datalist, data, TimeOut);
    for (i = 0; i < (data[0] + 1); i++) {
      
    }
    if (ret != 0)	//Ω‚Œˆ ß∞‹ªÚ’ﬂ≥¨ ±
        return -14 + ret * 100;
    
    ret = iccCheck(data, 0);
    if (ret != 0)
        return -15 + ret * 100;	//Ω” ’ ˝æ›–£—È ß∞‹÷±Ω”ÕÀ≥ˆ
    
    memcpy(RecvDate, &data[1], data[0]);
    
   
    for (i = 0; i < 4; i++) {	//(0, 8)”‡∂Ó
        banlance[i] = RecvDate[i];
        data_ret->a_cbb[i] = RecvDate[i];
    }
    for (i = 0; i < 2; i++) {	//(8, 4)¡™ª˙Ωª“◊–Ú∫≈
        paySerial[i] = RecvDate[4 + i];
        data_ret->a_on[i] = RecvDate[4 + i];
        g_frame_quancun_rq.QuancunTradeNo[i] = RecvDate[4 + i];
      
    }
    for (i = 0; i < 4; i++) {	//16, 8)Œ±ÀÊª˙ ˝
        passtyperand[i] = RecvDate[8 + i];
        data_ret->a_rnd[i] = RecvDate[8 + i];
        g_frame_quancun_rq.BogusRandNumber[i] = RecvDate[8 + i];
        
    }
    for (i = 0; i < 4; i++) {	//(24, 8)MAC1¬Î
        data_ret->a_m1[i] = RecvDate[12 + i];
        mac1[i] = RecvDate[12 + i];
       
    }
    
    for (i = 0; i < 4; i++) {	//16, 8)Œ±ÀÊª˙ ˝
        passtyperand[i] = RecvDate[8 + i];
        data_ret->a_rnd[i] = RecvDate[8 + i];
        g_frame_quancun_rq.BogusRandNumber[i] = RecvDate[8 + i];
      
    }
    

    return 0;
}
@end
