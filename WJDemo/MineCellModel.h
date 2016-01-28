//
//  MineCellModel.h
//  WJDemo
//
//  Created by 段瑞权 on 16/1/28.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
typedef NS_ENUM(NSInteger , MineCellType)
{
    MineCellTypeHeader = 0,
    mineCellTypeBody = 1
};

@interface MineCellModel : NSObject

@property (nonatomic,assign) MineCellType   cellType;

@property (nonatomic,assign)    UITableViewCellAccessoryType accessoryType;

@property (nonatomic,strong)  UIImage * leftImage;

@property (nonatomic,strong) UIImage * rightImage;

@property (nonatomic,copy) NSString * title;

@property (nonatomic,copy) NSString * subtitle;

@end



@interface MineCellFrame : NSObject

@property (nonatomic,strong) MineCellModel * cellModel;

@property (nonatomic,assign) CGRect   leftImageViewFrame;

@property (nonatomic,assign) CGRect   titleLableFrame;

@property (nonatomic,assign) CGRect   subTitleLableFrame;

@property (nonatomic,assign) CGRect   rightImageViewFrame;

@property (nonatomic,assign) CGRect   cellFrame;

@end