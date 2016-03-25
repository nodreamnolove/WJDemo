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
    NSString *urlstring = @"http://wanjiwuhanyuming.oicp.net:8082/OBU/";
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
    [params setObject:self.userNameF.text forKey:@"CustomerAccount"];
    [params setObject:self.passwordF.text forKey:@"Password"];
    [params setObject:@"CustomerLogin" forKey:@"CmdType"];
    [params setObject:@"android" forKey:@"LoginType"];
    
    [mgr POST:urlstring parameters:params success:^(NSURLSessionDataTask *task, id responseObject) {
        NSLog(@"responseObject = %@",responseObject);
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        NSLog(@"%@",error);
    }];
    
    
}
@end
