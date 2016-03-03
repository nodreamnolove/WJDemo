 
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
    for (int i=startPos; i<length; i++) {
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
    hexStr = [NSString stringWithFormat:@"%x",oneByte];
    return hexStr;
}
@end
