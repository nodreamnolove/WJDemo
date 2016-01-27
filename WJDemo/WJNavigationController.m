//
//  WJNavigationController.m
//  WJDemo
//
//  Created by 段瑞权 on 16/1/27.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "WJNavigationController.h"

@interface WJNavigationController ()

@end

@implementation WJNavigationController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    NSDictionary *textDict = @{NSForegroundColorAttributeName:[UIColor whiteColor],NSFontAttributeName:[UIFont systemFontOfSize:18]};
    self.navigationBar.titleTextAttributes = textDict;
    self.navigationBar.barTintColor = [UIColor colorWithRed:56.0f/255.0f green:55.0f/255.0f blue:61.0f/255.0f alpha:1];
    
}



@end
