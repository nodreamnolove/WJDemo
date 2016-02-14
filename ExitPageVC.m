//
//  ExitPageVC.m
//  WJDemo
//
//  Created by hmh on 16/1/27.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "ExitPageVC.h"
#import "AppDelegate.h"

@interface ExitPageVC ()

- (IBAction)closeConnect:(UIButton *)sender;

@end

@implementation ExitPageVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.title = @"退出";
    self.view.backgroundColor = [UIColor whiteColor];
    
}



- (IBAction)closeConnect:(UIButton *)sender {
    AppDelegate *appdelegate = [[UIApplication sharedApplication] delegate];
    [appdelegate disconnection];
    UIWindow *window = appdelegate.window;
    [UIView animateWithDuration:1.0 animations:^{
        window.alpha = 0;
        window.frame = CGRectMake(0, 0, 0, 0);
    } completion:^(BOOL finished) {
        exit(0);
    }];
    
}
@end
