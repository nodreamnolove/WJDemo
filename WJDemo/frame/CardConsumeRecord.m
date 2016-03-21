

#import "CardConsumeRecord.h"

@implementation CardConsumeRecord

-(NSString *)description
{
    NSString *descString = [NSString stringWithFormat:@"applicationId:复合应用类型标识符,recordLength:记录长度,applicationLockFlag:应用锁定标志,tollRoadNetworkId:入/出口收费路网号,tollLaneId:入/出口收费车道号,timeUnix:入/出口时间 UNIX时间,vehicleModel:车型,passStatus:入出口状态,reserve1:保留字节1,staffNo:收费员工号二进制方式存放入口员工号后六位,mtcSequenceNo:收费员工号二进制方式存放入口员工号后六位,vehicleNumber:车牌号码,reserve2:保留字节2"];
    return  descString;
}


@end
