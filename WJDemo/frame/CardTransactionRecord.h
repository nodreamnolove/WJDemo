

#import <Foundation/Foundation.h>

@interface CardTransactionRecord : NSObject

@property(nonatomic, copy)NSString* onlineSn; //用户卡内产生的交易流水号

@property(nonatomic, copy)NSString* overdrawLimit; //透支限额:单位分，10进制

@property(nonatomic, copy)NSString* transAmount; //交易金额:单位分，10进制

@property(nonatomic, copy)NSString* transType; //交易类型标识:02=圈存,06=消费

@property(nonatomic, copy)NSString* terminalNo; //终端机编号

@property(nonatomic, copy)NSString* transDate; //交易日期CCYYMMDD

@property(nonatomic, copy)NSString* transTime; //交易时间HHMMSS

-(NSString *)description;

@end
