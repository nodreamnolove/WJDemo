//
//  CardOwnerRecord.m
//  WJDemo
//
//  Created by mac on 16/3/4.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "CardOwnerRecord.h"

@implementation CardOwnerRecord
-(NSString *)description
{
    NSString *descString = [NSString stringWithFormat:@"ownerId:持卡人身份标识,staffId:本系统职工标识,ownerName:持卡人姓名,ownerLicenseNumber:持卡人证件号码,ownerLicenseType:持卡人证件类型"];
    return descString;
}
@end
