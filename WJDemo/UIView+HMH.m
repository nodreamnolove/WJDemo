//
//  UIView+HMH.m
//  STeBook
//
//  Created by hmh on 15/10/25.
//  Copyright © 2015年 rain. All rights reserved.
//

#import "UIView+HMH.h"

@implementation UIView (HMH)
-(void)setViewHeight:(CGFloat)height
{
    self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, self.frame.size.width, height);
    return ;
}
-(void)setViewY:(CGFloat)y
{
    self.frame = CGRectMake(self.frame.origin.x, y, self.frame.size.width, self.frame.size.height);
    return;
}
-(void)setviewWidth:(CGFloat)width
{
    self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, width, self.frame.size.height);
    return;
}
-(void)setViewX:(CGFloat)x
{
    self.frame = CGRectMake(x, self.frame.origin.y, self.frame.size.width, self.frame.size.height);
    return;
}
@end
