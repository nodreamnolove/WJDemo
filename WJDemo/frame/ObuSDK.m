//
//  ObuSDK.m
//  WJDemo
//
//  Created by 段瑞权 on 16/2/1.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "ObuSDK.h"
#import <UIKit/UIKit.h>
#import <CoreBluetooth/CoreBluetooth.h>
#include "issue_Ble_send.h"
#include "issue_Ble_recv.h"
#include "lib2hd.h"
#include "icc.h"
#include "NSString+NSStringHexToBytes.h"

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
        _obuSemaphore = dispatch_semaphore_create(1);
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



//4.连接OBU
-(void)connectDevice:(obuCallBack)callBack
{
     self.connectBlock = callBack;
    [self startScan];       //开始扫描
}
//5.断开连接
-(void)disconnectDevice:(obuCallBack)callBack
{
    self.disconnectBlock = callBack;
    if (self.isBlueConnected) {
        [self.wjCentralManger cancelPeripheralConnection:self.connectedPeripheral];
    }
    
}
//6.读取OBU的卡片信息
-(void)getCardInformation:(obuCallBack)callBack
{
    self.getCardInfoBlock = callBack;
    //1.发c1
    //成功
    PROG_COMM_C1 progc1;
    PROG_COMM_B1 progb1;
    if ( dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*20) == 0)
    {
        int length = send_c1_Ble_OC(progc1);
        NSLog(@"length=%d",length);
        g_com_rx_len = 0;
        g_com_needrx_len = 50;
        [self sendData:length andRepeat:1];
//        NSData *sendData2 = [NSData dataWithBytes:g_com_tx_buf+20 length:20];
//         [self.connectedPeripheral writeValue:sendData2 forCharacteristic:self.readwriteCharacter type:CBCharacteristicWriteWithResponse];
        
        if (dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+NSEC_PER_SEC*3) == 0) {
            //1.解析bst
            if(recv_b1_Ble_OC(&progb1, 20)==SUCCESS)
            {
                //2.发送c9
                PROG_COMM_C4 progc4;
                c4_init(progc4, (byte)0x01);
                send_c9_Ble_OC(progc4, 1000);
                //3.解析b9
                //4.发送c5
                //5.解析b5
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
//    else
    {
        //超时失败
         NSLog(@"超时无响应");
        return;
    }
   
    //2.等b1
    
    //3.发c9
    
    //4.等b9
    
    //5.发c5
    
    //6.等b4
    
}
//7.读取OBU的设备信息
-(void)getObuInformation:(obuCallBack)callBack
{
    self.getOBUInfoBlock = callBack;
    //发送数据
//    NSData *sendData;
//    [peripheral writeValue:datastr forCharacteristic:writeCharacteristic type:CBCharacteristicWriteWithResponse];
//    [self.connectedPeripheral writeValue:sendData forCharacteristic:nil type:CBCharacteristicWriteWithoutResponse];
}


//8.充值写卡：获取Mac1等数据
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

//9.充值写卡：执行写卡操作
-(void)loadCreditWriteCard:(NSString *)dateMAC2 callBack:(obuCallBack)callBack
{
    
}

//10.读终端交易记录文件
-(void)readCardTransactionRecord:(NSString *)pinCode maxNumber:(NSInteger)maxNumber callBack:(obuCallBack)callBack
{
    
}

//11.读联网收费复合消费过程文件
-(void)readCardConsumeRecord:(NSInteger)maxNumber callBack:(obuCallBack)callBack
{
    
}

//12.读持卡人基本数据文件
-(void)readCardOwnerRecord:(obuCallBack)callBack
{
    
}

//13.数据透传
-(void)transCommand:(NSData*)reqData callBack:(obuCallBack)callBack
{
    
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
