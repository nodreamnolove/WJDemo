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
#include "lib2hd.h"

@interface ObuSDK()<CBCentralManagerDelegate,CBPeripheralManagerDelegate,CBPeripheralDelegate>

@property (nonatomic,copy) NSString * characteristic_uuid; //特征id

@property (nonatomic,copy) NSString * deveice_uuid;//设备id

@property (nonatomic,strong) CBCentralManager * wjCentralManger;

@property (nonatomic,strong) CBPeripheral * connectedPeripheral;

@property (nonatomic,strong) dispatch_queue_t  bluetoothQueue;

@property (nonatomic,strong) CBCharacteristic * readwriteCharacter;

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
            
        }
        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:self.characteristic_uuid]]) {
            [peripheral setNotifyValue:YES forCharacteristic:characteristic];
            //找到需要的特征 预定成功
        }
    }
}
//发送成功调用
- (void)peripheral:(CBPeripheral *)peripheral didWriteValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    NSLog(@"发现成功调用%s",__func__);
}

//处理蓝牙发过来的数据
-(void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    if(error){
        NSLog(@"发现特征错误：%@",[error localizedDescription]);
        return;
    }
    
    NSString *getStringData = [[NSString alloc]initWithData:characteristic.value encoding:NSUTF8StringEncoding];
    NSLog(@"收到的字符：%@",getStringData);
    
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
    if ( dispatch_semaphore_wait(self.obuSemaphore, DISPATCH_TIME_NOW+20) == 0) {
        send_c1_Ble_OC(progc1);
        
        
    }else{
        //超时失败
        
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
-(void)loadCreditGetMac1:(NSString *)credit \
cardId:(NSString*)cardId  \
terminalNo:(NSString *)terminalNo \
picCode:(NSString *)pinCode \
procType:(NSString*)procType \
keyIndex:(NSString *)keyIndex \
callBack:(obuCallBack)callBack
{
    
}

//9.充值写卡：执行写卡操作
-(void)loadCreditWriteCard:(NSString *)dateMAC2 callBack:(obuCallBack)callBack
{
    
}

//10.读终端交易记录文件
-(void)readCardTransactionRecord:(obuCallBack)callBack;
{
    
}

//11.读联网收费复合消费过程文件
-(void)readCardConsumeRecord:(obuCallBack)callBack;
{
    
}

//12.读持卡人基本数据文件
-(void)readCardOwnerRecord:(obuCallBack)callBack;
{
    
}

//13.数据透传
-(void)transCommand:(NSData*)reqData callBack:(obuCallBack)callBack
{
    
}
@end
