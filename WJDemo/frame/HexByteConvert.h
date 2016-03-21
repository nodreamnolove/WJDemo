//
//  HexByteConvert.h
//  WJDemo
//
//  Created by mac on 16/3/22.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HexByteConvert : NSObject
+(NSData*) hexToBytes:(NSString*)string ;
+(NSString *)byteToNSString:(Byte *)byteArr andLength:(int )length;
+(NSString *)byteToNSString:(Byte *)byteArr fromIndex:(int)startPos andLength:(int )length;
+(NSString *)stringByByte:(Byte)oneByte;
@end
