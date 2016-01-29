//
//  MineCellView.h
//  WJDemo
//
//  Created by hmh on 16/1/28.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MineCellFrame;

@interface MineCellView : UITableViewCell

@property (nonatomic,strong) MineCellFrame * mineCellFrame;

@property (nonatomic,strong) UIImageView * leftImageView;

@property (nonatomic,strong) UILabel * titleLabel;

@property (nonatomic,strong) UILabel * subTitleLabel;

@property (nonatomic,strong) UIImageView * rightImageView;


@end
