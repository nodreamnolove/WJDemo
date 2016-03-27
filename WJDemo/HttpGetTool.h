 
#import <Foundation/Foundation.h>
typedef void(^getNetDataSuccess)(NSDictionary *dict);
typedef void (^getNetDataFailure)();

@interface HttpGetTool : NSObject
//1、取随机数
+(void)getRandomNum:(NSString*)obuID    andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;
//2、上传视频信息
+(void)uploadVedioInfo:(NSDictionary *)vedioInfo andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;
//3、上传OBUID
+(void)uploadOBUID:(NSString *)obuID andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;
//4、创建订单
+(void)createOrder:(NSString* )userName andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;
//5、支付信息
+(void)payInfo:(NSDictionary *)payInfo andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;
//6、客户登录
+(void)login:(NSString *)userName andPassword:(NSString *)password andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;
//7、余额查询
+(void)getUserMoney:(NSString *)userName andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;
//8、用户信息
+(void)getUserInfo:(NSString *)userName andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;
//9、充值卡充值
+(void)deposit:(NSString *)userName andCardNo:(NSString *)cardNo andCardPassword:(NSString *)cardPassword andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;
//10、命令帧
+(void)OrderFrame:(NSDictionary *)frameDict  andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;
//11、修改密码
+(void)modifyPassword:(NSString *)userName andOldPassword:(NSString *)oldpassword andNewPassword:(NSString *)newpassword andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;;

@end
