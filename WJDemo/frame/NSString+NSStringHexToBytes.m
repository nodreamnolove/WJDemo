//
//  NSString+NSStringHexToBytes.m
//  WJDemo
//
//  Created by 段瑞权 on 16/2/25.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "NSString+NSStringHexToBytes.h"

@implementation NSString (NSStringHexToBytes)
-(NSData*) hexToBytes
{
    NSMutableData* data = [NSMutableData data];
    int idx;
    for (idx = 0; idx+2 <= self.length; idx+=2) {
        NSRange range = NSMakeRange(idx, 2);
        NSString* hexStr = [self substringWithRange:range];
        NSScanner* scanner = [NSScanner scannerWithString:hexStr];
        unsigned int intValue;
        [scanner scanHexInt:&intValue];
        [data appendBytes:&intValue length:1];
    }
    return data;
}
@end
