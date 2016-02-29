 

#import <Foundation/Foundation.h>

@interface CardConsumeRecord : NSObject

@property(nonatomic, copy)NSString* applicationId; //复合应用类型标识符

@property(nonatomic, copy)NSString* recordLength; //记录长度

@property(nonatomic, copy)NSString* applicationLockFlag; //应用锁定标志

@property(nonatomic, copy)NSString* tollRoadNetworkId; // 入/出口收费路网号

@property(nonatomic, copy)NSString* tollStationId; // 入/出口收费站号

@property(nonatomic, copy)NSString* tollLaneId; // 入/出口收费车道号

@property(nonatomic, copy)NSString* timeUnix; // 入/出口时间 UNIX时间

@property(nonatomic, copy)NSString* vehicleModel; //车型

@property(nonatomic, copy)NSString* passStatus; // 入出口状态

@property(nonatomic, copy)NSString* reserve1; //保留字节1

@property(nonatomic, copy)NSString* staffNo; //收费员工号二进制方式存放入口员工号后六位

@property(nonatomic, copy)NSString* mtcSequenceNo; //收费员工号二进制方式存放入口员工号后六位

@property(nonatomic, copy)NSString* vehicleNumber; //车牌号码

@property(nonatomic, copy)NSString* reserve2; //保留字节2


-(NSString*) description;



@end
