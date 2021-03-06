//
//  HexByteConvert.m
//  WJDemo
//
//  Created by mac on 16/3/22.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "HexByteConvert.h"

@implementation HexByteConvert
+(NSData*) hexToBytes:(NSString *)string
{
    NSMutableData* data = [NSMutableData data];
    int idx;
    for (idx = 0; idx+2 <= string.length; idx+=2) {
        NSRange range = NSMakeRange(idx, 2);
        NSString* hexStr = [string substringWithRange:range];
        NSScanner* scanner = [NSScanner scannerWithString:hexStr];
        unsigned int intValue;
        [scanner scanHexInt:&intValue];
        [data appendBytes:&intValue length:1];
    }
    return data;
}

+(NSString *)byteToNSString:(Byte *)byteArr andLength:(int )length
{
    NSString *hexStr=@"";
    for (int i=0; i<length; i++) {
        NSString *newHexStr = [NSString stringWithFormat:@"%x",byteArr[i]&0xff];
        if (newHexStr.length==1) {
            hexStr = [NSString stringWithFormat:@"%@0%@",hexStr,newHexStr];
        }
        else
            hexStr = [NSString stringWithFormat:@"%@%@",hexStr,newHexStr];
    }
    return  hexStr;
}
+(NSString *)byteToNSString:(Byte *)byteArr fromIndex:(int)startPos andLength:(int )length
{
    NSString *hexStr=@"";
    //&&i<sizeof(byteArr)/sizeof(byteArr[0])
    for (int i=startPos; i<startPos+length; i++) {
        NSString *newHexStr = [NSString stringWithFormat:@"%x",byteArr[i]&0xff];
        if (newHexStr.length==1) {
            hexStr = [NSString stringWithFormat:@"%@0%@",hexStr,newHexStr];
        }
        else
            hexStr = [NSString stringWithFormat:@"%@%@",hexStr,newHexStr];
    }
    return  hexStr;
}
+(NSString *)stringByByte:(Byte)oneByte
{
    NSString * hexStr = @"";
    hexStr = [NSString stringWithFormat:@"%02x",oneByte];
    return hexStr;
}
@end
