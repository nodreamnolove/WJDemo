//
//  LoginVC.m
//  WJDemo
//
//  Created by mac on 16/3/26.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "LoginVC.h"
#import "AFNetworking.h"
#import "MBProgressHUD+MJ.h"
#import "AppDelegate.h"

@interface LoginVC ()

@property (weak, nonatomic) IBOutlet UITextField *userNameF;
@property (weak, nonatomic) IBOutlet UITextField *passwordF;


@property (weak, nonatomic) IBOutlet UIButton *loginBtn;

- (IBAction)loginAction:(id)sender;

@end

@implementation LoginVC

- (void)viewDidLoad {
    [super viewDidLoad];
    
}



- (IBAction)loginAction:(id)sender {
    
    if (!self.userNameF.text||self.userNameF.text.length<1) {
        [MBProgressHUD showError:@"请输入用户名"];
        return ;
    }
    if (!self.passwordF.text||self.passwordF.text.length<1) {
        [MBProgressHUD showError:@"请输入密码"];
        return;
    }
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
//    mgr.responseSerializer = [AFJSONResponseSerializer serializer];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
//    NSString *urlstring = @"http://wanjiwuhanyuming.oicp.net:8082/OBU/WJOBU";
    NSMutableDictionary *params = [NSMutableDictionary dictionary];//@{@"CmdType":@"CustomerLogin",@"CustomerAccount":self.userNameF.text,@"Password":self.passwordF.text,@"LoginType":@"android"};
    [params setObject:self.userNameF.text forKey:@"CustomerAccount"];
    [params setObject:self.passwordF.text forKey:@"Password"];
    [params setObject:@"CustomerLogin" forKey:@"CmdType"];
    [params setObject:@"android" forKey:@"LoginType"];
    __weak typeof(self) weakself = self;
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                [MBProgressHUD showError:@"登录失败"];
                [AppDelegate instance].userName = @"";
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        
                        if (self.loginSuccessBlock) {
                            self.loginSuccessBlock(weakself.userNameF.text);
                        }
                        [AppDelegate instance].userName = weakself.userNameF.text;
                        [self.navigationController popViewControllerAnimated:YES];
                    }
                }
            }
            
        }
        NSLog(@"responseObject = %@",responseObject);
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        NSLog(@"%@",error);
        [MBProgressHUD showError:@"登录失败"];
        [AppDelegate instance].userName = @"";
    }];
    
    
    
//    NSString *usrstring = @"http://wanjiwuhanyuming.oicp.net:8082/OBU/WJOBU";
//    
//    AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
//    manager.requestSerializer = [AFJSONRequestSerializer serializer];
//    manager.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
//    
//    NSDictionary *param = @{@"CmdType":@"CustomerLogin",@"CustomerAccount":@"shaochong",@"Password":@"123456",@"LoginType":@"android"};
//    
//    
//    [manager POST:usrstring parameters:param success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
//        NSLog(@"%@",responseObject);
//        
//    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
//        NSLog(@"%@",error);
//    }];
    
}
@end
