//
//  AppDelegate.h
//  WJDemo
//
//  Created by hmh on 16/1/27.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import <UIKit/UIKit.h>
@class AsyncSocket;


@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (strong ,nonatomic) AsyncSocket *asocket;

@property (nonatomic,copy)  NSString * ip;

@property (nonatomic,assign) UInt16 port;

@property (nonatomic,copy) NSString * userName;

-(BOOL)connection;
-(BOOL)disconnection;
+ (instancetype)instance;
@end

