//
//  NetFunction.m
//  WJDemo
//
//  Created by hmh on 16/1/28.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "NetFunction.h"
#import <netdb.h>
#import <sys/socket.h>
#import <arpa/inet.h>
#import "AsyncSocket.h"

@interface NetFunction()

@end

@implementation NetFunction
singleton_implementation(NetFunction)


+(NSString *)getIPByHostName:(NSString *)hostName
{
    const char * hostString = [hostName UTF8String];
    struct hostent * phot;
    @try {
        phot = gethostbyname(hostString);
    }
    @catch (NSException *exception) {
        return @"";
    }
    struct in_addr ipAddr;
    memcpy(&ipAddr, phot->h_addr_list[0], 4);
    char ip[20] = {0};
    
    inet_ntop(AF_INET, &ipAddr, ip, sizeof(ip));
    
    NSString* strIPAddress = [NSString stringWithUTF8String:ip];
    return strIPAddress;     
}
@end



//NSString *hostname = @"apple.com";
//CFHostRef hostRef = CFHostCreateWithName(kCFAllocatorDefault, (__bridge CFStringRef)hostname);
//if (hostRef)
//{
//    Boolean result = CFHostStartInfoResolution(hostRef, kCFHostAddresses, NULL);
//    if (result == TRUE)
//    {
//        NSArray *addresses = (__bridge NSArray*)CFHostGetAddressing(hostRef, &result);
//        
//        NSMutableArray *tempDNS = [[NSMutableArray alloc] init];
//        for(int i = 0; i < addresses.count; i++)
//        {
//            struct sockaddr_in* remoteAddr;
//            CFDataRef saData = (CFDataRef)CFArrayGetValueAtIndex((__bridge CFArrayRef)addresses, i);
//            remoteAddr = (struct sockaddr_in*)CFDataGetBytePtr(saData);
//            
//            if(remoteAddr != NULL)
//            {
//                const char *strIP41 = inet_ntoa(remoteAddr->sin_addr);
//                
//                NSString *strDNS =[NSString stringWithCString:strIP41 encoding:NSASCIIStringEncoding];
//                NSLog(@"RESOLVED %d:<%@>", i, strDNS);
//                [tempDNS addObject:strDNS];
//            }
//        }
//    }
//}









