//
//  AppDelegate.m
//  WJDemo
//
//  Created by hmh on 16/1/27.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "AppDelegate.h"
#import "WJTabBarController.h"
#import "NetFunction.h"
#import "AsyncSocket.h"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    self.window = [[UIWindow alloc]initWithFrame:[UIScreen mainScreen].bounds];
    
    UIStoryboard *storyBoard = [UIStoryboard storyboardWithName:@"Main" bundle:[NSBundle mainBundle]];
    self.window.rootViewController = [storyBoard instantiateInitialViewController];
    [self.window makeKeyAndVisible];
   
    return  YES;
   
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}



#pragma mark
//接受消息成功之后
- (void)onSocket:(AsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag
{
    
}
//
-(void)onSocket:(AsyncSocket *)sock willDisconnectWithError:(NSError *)err
{
    
}

- (void)onSocket:(AsyncSocket *)sock didConnectToHost:(NSString *)host port:(UInt16)port
{
    
}
-(void)onSocketDidDisconnect:(AsyncSocket *)sock
{
    self.asocket = nil;
}

-(AsyncSocket *)asocket
{
    if (_asocket==nil) {
        _asocket = [[AsyncSocket alloc]initWithDelegate:self];
    }
    return _asocket;
}
-(NSString *)ip
{
    if (_ip==nil) {
        _ip =  [NetFunction getIPByHostName:@"wanjiwuhanyuming.oicp.net"];
    }
    return _ip;
}

//连接
-(BOOL)connection
{
    NSError *error;
    if (!self.asocket) {
        return NO;
    }
    if (self.asocket.isConnected) {
        return YES;
    }
    
//    self.port = 8088;
    if(![self.asocket connectToHost:self.ip onPort:self.port  error:&error])
    {
        //连接失败
        return NO;
    }
    [self.asocket readDataWithTimeout:-1 tag:0];
    return YES;
}

-(BOOL)disconnection
{
    if ([self.asocket isConnected]) {
        [self.asocket disconnect];
    }
    return YES;
}
@end
