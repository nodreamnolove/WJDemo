//
//  MineCellView.m
//  WJDemo
//
//  Created by 段瑞权 on 16/1/28.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "MineCellView.h"
#import "MineCellModel.h"

@implementation MineCellView

-(instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.leftImageView = [UIImageView new];
        self.rightImageView = [UIImageView new];
        self.titleLabel = [UILabel new];
        self.subTitleLabel = [UILabel new];
        
        [self.contentView addSubview:self.leftImageView];
        [self.contentView addSubview:self.titleLabel];
        [self.contentView addSubview:self.subTitleLabel];
        [self.contentView addSubview:self.rightImageView];
    }
    return self;
}

-(void)setMineCellFrame:(MineCellFrame *)mineCellFrame
{
    self.leftImageView.frame = mineCellFrame.leftImageViewFrame;
    self.titleLabel.frame = mineCellFrame.titleLableFrame;
    self.subTitleLabel.frame = mineCellFrame.subTitleLableFrame;
    self.rightImageView.frame = mineCellFrame.rightImageViewFrame;
    
    self.leftImageView.image = mineCellFrame.cellModel.leftImage;
    self.titleLabel.text = mineCellFrame.cellModel.title;
    self.subTitleLabel.text = mineCellFrame.cellModel.subtitle;
    self.rightImageView.image = mineCellFrame.cellModel.rightImage;
    if (mineCellFrame.cellModel.cellType == MineCellTypeHeader) {
        self.leftImageView.layer.cornerRadius = 5.0;
        self.leftImageView.layer.masksToBounds = YES;
        self.titleLabel.font = [UIFont systemFontOfSize:18];
        self.subTitleLabel.font = [UIFont systemFontOfSize:16];
    }
    else{
        self.titleLabel.font = [UIFont systemFontOfSize:18];
    }


}

@end
