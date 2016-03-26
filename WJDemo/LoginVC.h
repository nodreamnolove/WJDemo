//
//  LoginVC.h
//  WJDemo
//
//  Created by mac on 16/3/26.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef void(^loginBlock)(NSString *);


@interface LoginVC : UIViewController

@property (nonatomic,copy) loginBlock loginSuccessBlock;
@end
