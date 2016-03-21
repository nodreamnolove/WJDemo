

#import "CardTransactionRecord.h"

@implementation CardTransactionRecord

-(NSString *)description
{
    NSString *descString = [NSString stringWithFormat:@"onlineSn:用户卡内产生的交易流水号,overdrawLimit:透支限额:单位分，10进制,transAmount:交易金额:单位分，10进制,transType:交易类型标识:02=圈存,06=消费,terminalNo:终端机编号,transDate:交易日期CCYYMMDD,transTime:交易时间HHMMSS"];
    return  descString;
}

@end
