 

#import <Foundation/Foundation.h>

@interface NSString (NSStringHexToBytes)
-(NSData*) hexToBytes;
+(NSString *)byteToNSString:(Byte *)byteArr andLength:(int )length;
+(NSString *)stringByByte:(Byte)oneByte;
@end
