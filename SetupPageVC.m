//
//  SetupPageVC.m
//  WJDemo
//
//  Created by hmh on 16/1/27.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "SetupPageVC.h"
#import "MBProgressHUD+MJ.h"
#import "AppDelegate.h"

@interface SetupPageVC ()

@property (weak, nonatomic) IBOutlet UITextField *yuming;

@property (weak, nonatomic) IBOutlet UITextField *portstr;

- (IBAction)connectSer:(UIButton *)sender;

@end

@implementation SetupPageVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.title = @"设置";
    self.view.backgroundColor = [UIColor whiteColor];
    
}



- (IBAction)connectSer:(UIButton *)sender {
    if (!self.yuming.text) {
        [MBProgressHUD showError:@"请输入用户名"];
        return ;
    }
    if (!self.portstr.text) {
        [MBProgressHUD showError:@"请输入密码"];
        return;
    }
    
    
    
}
@end
