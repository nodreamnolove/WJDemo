//
//  ObuSDK.m
//  WJDemo
//
//  Created by 段瑞权 on 16/2/1.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "ObuSDK.h"
#import <CoreBluetooth/CoreBluetooth.h>

@interface ObuSDK()<CBCentralManagerDelegate>

//@property (nonatomic,strong) CBCentralManager * wjCentralManger;

@end

@implementation ObuSDK

static ObuSDK * _instance;

static CBCentralManager * wjCentralManger;

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
        wjCentralManger = [[CBCentralManager alloc]initWithDelegate:_instance queue:nil];
    });
    return _instance;
}



-(BOOL)isEnabledBluetooth
{
    if (wjCentralManger) {
        
        NSLog(@"%zi",wjCentralManger.state);
    }
    return YES;
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    NSLog(@"%@",central);
}

@end
