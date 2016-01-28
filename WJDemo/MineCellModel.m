//
//  MineCellModel.m
//  WJDemo
//
//  Created by 段瑞权 on 16/1/28.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "MineCellModel.h"

@implementation MineCellModel

@end



@implementation MineCellFrame
 
-(void)setCellModel:(MineCellModel *)cellModel
{
    _cellModel = cellModel;
    if (cellModel.cellType == MineCellTypeHeader) {
        self.leftImageViewFrame = CGRectMake(14, 11, 65, 65);
        self.titleLableFrame = CGRectMake(89, 22, ScreenWidth-50-70, 17);
        self.subTitleLableFrame = CGRectMake(89, 48, ScreenWidth-50-70, 17);
        self.rightImageViewFrame = CGRectZero;
        self.cellFrame = CGRectMake(0, 0, ScreenWidth, 87);
    }
    else
    {
        self.leftImageViewFrame = CGRectMake(15, 11, 22, 22);
        self.titleLableFrame = CGRectMake(45, 11, ScreenWidth-90-60, 22);
        self.subTitleLableFrame = CGRectZero;
        self.rightImageViewFrame = CGRectZero;
        self.cellFrame = CGRectMake(0, 0, ScreenWidth, 44);
    }
}

@end