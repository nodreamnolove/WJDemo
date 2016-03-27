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
#import "HttpGetTool.h"

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

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
    
    [self.view endEditing:YES];
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
 
    __weak typeof(self) weakSelf = self;
  [HttpGetTool login:self.userNameF.text andPassword:self.passwordF.text andSuccess:^(NSDictionary *dict) {
      if ([[dict allKeys]containsObject:@"code"]) {
          NSString *reValue = [dict objectForKey:@"code"];
          if ([reValue isEqualToString:@"0000"]) {
               [AppDelegate instance].userName = weakSelf.userNameF.text;
              [HttpGetTool getUserMoney:weakSelf.userNameF.text andSuccess:^(NSDictionary *dict) {
                   NSString *usermoney = [dict objectForKey:@"totalMoney"];
                  [AppDelegate instance].userMoney = usermoney;
                  //登录成功
                  if (weakSelf.loginSuccessBlock) {
                      weakSelf.loginSuccessBlock(weakSelf.userNameF.text,usermoney);
                  }
                  [weakSelf.navigationController popViewControllerAnimated:YES];
              } andFailure:^{
                  weakSelf.loginSuccessBlock(weakSelf.userNameF.text,nil);
                  [weakSelf.navigationController popViewControllerAnimated:YES];
              }];
            }
      }
  } andFailure:^{
      
  }];
}
@end
