//
//  MBProgressHUD+HMH.m
//  WJDemo
//
//  Created by mac on 16/3/20.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "MBProgressHUD+HMH.h"

@implementation MBProgressHUD (HMH)
+(void)showInfo:(id)info toView:(UIView *)view
{
    if (view == nil) view = [[UIApplication sharedApplication].windows lastObject];
     MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:view animated:YES];
    if ([info isKindOfClass:[NSString class]]) {
        hud.labelText = info;
    }
    else if([info isKindOfClass:[NSArray class]]){
        hud.labelText = @"";
        hud.detailsLabelText = @"";
    }
    else if ([info isKindOfClass:[NSDictionary class]]){
        NSArray* keyArr = [info allKeys];
        NSArray* valueArr = [info allValues];
        CGFloat lableY = 0;
        for (int i=0; i<keyArr.count; i++) {
            lableY = i*20;
            UILabel *titleLabel = [[UILabel alloc]initWithFrame:CGRectMake(0, lableY, 100, 20)];
            UILabel *contentLabel = [[UILabel alloc]initWithFrame:CGRectMake(100, lableY, ScreenWidth-100, 20)];
            titleLabel.text = [NSString stringWithFormat:@"%@",keyArr[i]];
            contentLabel.text = [NSString stringWithFormat:@"%@",valueArr[i]];
            [hud addSubview:titleLabel];
            [hud addSubview:contentLabel];
        }
    }
    else
    {
        NSString *totalInfo = [NSString stringWithFormat:@"%@",info];
        hud.labelText = totalInfo;
    }
    
}
@end
