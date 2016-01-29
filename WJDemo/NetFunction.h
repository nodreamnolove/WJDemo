//
//  NetFunction.h
//  WJDemo
//
//  Created by hmh on 16/1/28.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "singleton.h"
@class AsyncSocket;

typedef NS_ENUM(NSInteger,CutType)
{
       CutTypeByServer,
       CutTypeByUser
};





@interface NetFunction : NSObject
singleton_interface(NetFunction)

@property (nonatomic,strong)  AsyncSocket * socket;

@property (nonatomic,copy) NSString * socketHost;//主机IP

@property (nonatomic,assign) UInt16   port; //端口


//域名解析 返回IP
+(NSString *)getIPByHostName:(NSString *)hostName;
//建立连接
-(void)startConnectHost;
//关闭连接
-(void)closeConnect;

@end
