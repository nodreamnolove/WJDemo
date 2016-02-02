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

@interface ObuSDK()<CBCentralManagerDelegate>

@property (nonatomic,strong) CBCentralManager * wjCentralManger;

@property (nonatomic,strong) CBPeripheral * connectedPeripheral;

@property (nonatomic,strong) dispatch_queue_t  bluetoothQueue;

@end

@implementation ObuSDK

static ObuSDK * _instance;

//static CBCentralManager * _wjCentralManger;

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

-(instancetype)init
{
    self = [super init];
    if (self) {
        NSLog(@"init%@",[NSThread currentThread]);
        _bluetoothQueue = dispatch_queue_create("BlueQueue", NULL);
        _wjCentralManger = [[CBCentralManager alloc]initWithDelegate:_instance queue:_bluetoothQueue options:@{CBCentralManagerOptionShowPowerAlertKey:@YES}];
        
        _bluetoothState = NO;
    }
    return self;
}

-(void)startScan
{
    NSLog(@"startScan%@",[NSThread currentThread]);
//    if(![_wjCentralManger isScanning])
    {
        [_wjCentralManger scanForPeripheralsWithServices:nil options:@{CBCentralManagerScanOptionAllowDuplicatesKey:@YES}];
        [[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:YES];
    }
}

-(void)stopScan
{
//    if ([_wjCentralManger isScanning]) {
        [_wjCentralManger stopScan];
        [[UIApplication sharedApplication]setNetworkActivityIndicatorVisible:NO];
//    }
}
-(void)connectDevice:(CBPeripheral *)connectPeripheral
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
   
}

//存储蓝牙状态
- (void)centralManager:(CBCentralManager *)central willRestoreState:(NSDictionary<NSString *, id> *)dict
{
    NSLog(@"centralManager %@",central);
}

//发现设备
- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI
{
    NSLog(@"%@++%@",[NSThread currentThread],central);
}

//连接上设备
- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
    self.connectedPeripheral = peripheral;
}

//连接失败
- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error
{
    self.connectedPeripheral = nil;
}

//断开连接
- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error
{
    self.connectedPeripheral = nil;
}

@end
