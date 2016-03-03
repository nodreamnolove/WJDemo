//
//  CardOwnerRecord.h
//  WJDemo
//
//  Created by mac on 16/3/4.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface CardOwnerRecord : NSObject



@property(nonatomic, strong)NSString* ownerId; //持卡人身份标识

@property(nonatomic, strong)NSString* staffId; //本系统职工标识

@property(nonatomic, strong)NSString* ownerName; //持卡人姓名

@property(nonatomic, strong)NSString* ownerLicenseNumber; //持卡人证件号码

@property(nonatomic, strong)NSString* ownerLicenseType; //持卡人证件类型



-(NSString*) description;



@end

@end
