//
//  ObuSDK.h
//  WJDemo
//
//  Created by 段瑞权 on 16/2/1.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ObuSDK : NSObject
//创建ObuSDK对象的实例（单例）
+(instancetype)sharedObuSDK;

//查询蓝牙打开状态
-(BOOL)isEnabledBluetooth;

//连接OBU
-(void)connectDevice;

//断开OBU连接
-(void)disconnectDevice;

//读取OBU的卡片信息
-(void)getCardInformation;

//读取OBU的设备信息
-(void)getObuInformation;

//充值写卡：获取Mac1等数据
-(void)loadCreditGetMac1;

//充值写卡：执行写卡操作
-(void)loadCreditWriteCard;

//读终端交易记录文件
-(void)loadCardTransactionRecord;

//读联网收费复合消费过程文件
-(void)readCardConsumeRecord;

//读持卡人基本数据文件
-(void)readCardOwnerRecord;

//数据透传
-(void)transCommand;
@end
