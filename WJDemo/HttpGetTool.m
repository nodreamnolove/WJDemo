 

#import "HttpGetTool.h"
#import "AFNetworking.h"
#import "MBProgressHUD+MJ.h"
#import "AppDelegate.h"

@implementation HttpGetTool
#pragma mark 1、取随机数
+(void)getRandomNum:(NSString *) obuID  andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock
{
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
   
    [params setObject:@"RequestRandom" forKey:@"CmdType"];
    [params setObject:obuID forKey:@"OBUID"];
//    __weak typeof(self) weakself = self;
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                failureBlock();
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        successBlock(rspDict);
                    }
                    else
                        failureBlock();
                }
                else
                    failureBlock();
            }
            else
                failureBlock();
        }
        
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        failureBlock();
    }];
    
}



#pragma mark  2、上传视频信息
+(void)uploadVedioInfo:(NSDictionary *)vedioInfo andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock{
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithDictionary:vedioInfo];
    [params setObject:@"UpLoadVideoInfo" forKey:@"CmdType"];
    
    //    __weak typeof(self) weakself = self;
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                failureBlock();
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        successBlock(rspDict);
                    }
                    else
                        failureBlock();
                }
                else
                    failureBlock();
            }
            else
                failureBlock();
        }
        
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        failureBlock();
    }];
}

#pragma mark 3、上传OBUID
+(void)uploadOBUID:(NSString *)obuID andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock{
    
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
    
    [params setObject:@"SendObuid" forKey:@"CmdType"];
    [params setObject:obuID forKey:@"OBUID"];
    //    __weak typeof(self) weakself = self;
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                failureBlock();
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        successBlock(rspDict);
                    }
                    else
                        failureBlock();
                }
                else
                    failureBlock();
            }
            else
                failureBlock();
        }
        
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        failureBlock();
    }];
}
#pragma mark 4、创建订单
+(void)createOrder:(NSString* )userName andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock{

    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
    
    [params setObject:@"CreateTradeRq" forKey:@"CmdType"];
    [params setObject:userName forKey:@"CustomerAccount"];
    //    __weak typeof(self) weakself = self;
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                failureBlock();
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        successBlock(rspDict);
                    }
                    else
                        failureBlock();
                }
                else
                    failureBlock();
            }
            else
                failureBlock();
        }
        
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        failureBlock();
    }];
}
#pragma mark 5、支付信息
+(void)payInfo:(NSDictionary *)payInfo andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock{
    
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithDictionary:payInfo];
    
    [params setObject:@"IsPaying" forKey:@"CmdType"];
    
    //    __weak typeof(self) weakself = self;
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                failureBlock();
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        successBlock(rspDict);
                    }
                    else
                        failureBlock();
                }
                else
                    failureBlock();
            }
            else
                failureBlock();
        }
        
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        failureBlock();
    }];
}
#pragma mark 6、客户登录
+(void)login:(NSString *)userName andPassword:(NSString *)password andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock{
    
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
    
    [params setObject:@"CustomerLogin" forKey:@"CmdType"];
    [params setObject:userName forKey:@"CustomerAccount"];
    [params setObject:password forKey:@"Password"];
    [params setObject:@"ios" forKey:@"LoginType"];
    
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                failureBlock();
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        successBlock(rspDict);
                    }
                    else
                        failureBlock();
                }
                else
                    failureBlock();
            }
            else
                failureBlock();
        }
        
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        failureBlock();
    }];
}
#pragma mark 7、余额查询
+(void)getUserMoney:(NSString *)userName andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock{
    
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
    
    [params setObject:@"ResearchCustomerMoney" forKey:@"CmdType"];
    [params setObject:userName forKey:@"CustomerAccount"];
    //    __weak typeof(self) weakself = self;
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                failureBlock();
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        successBlock(rspDict);
                    }
                    else
                        failureBlock();
                }
                else
                    failureBlock();
            }
            else
                failureBlock();
        }
        
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        failureBlock();
    }];
}
#pragma mark 8、用户信息
+(void)getUserInfo:(NSString *)userName andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock{
    
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
    
    [params setObject:@"ResearchCustomerInfo" forKey:@"CmdType"];
    [params setObject:userName forKey:@"CustomerAccount"];
    //    __weak typeof(self) weakself = self;
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                failureBlock();
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        successBlock(rspDict);
                    }
                    else
                        failureBlock();
                }
                else
                    failureBlock();
            }
            else
                failureBlock();
        }
        
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        failureBlock();
    }];
}
#pragma mark 9、充值卡充值
+(void)deposit:(NSString *)userName andCardNo:(NSString *)cardNo andCardPassword:(NSString *)cardPassword andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock{
   
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
    
    [params setObject:@"RechargeCard" forKey:@"CmdType"];
    [params setObject:userName forKey:@"CustomerAccount"];
    [params setObject:cardNo forKey:@"CardAccount"];
    [params setObject:cardPassword forKey:@"CardPassword"];
    //    __weak typeof(self) weakself = self;
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                failureBlock();
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        successBlock(rspDict);
                    }
                    else
                        failureBlock();
                }
                else
                    failureBlock();
            }
            else
                failureBlock();
        }
        
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        failureBlock();
    }];
}
#pragma mark 10、命令帧
+(void)OrderFrame:(NSDictionary *)frameDict  andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock{
    
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithDictionary:frameDict];
    
    [params setObject:@"SendRqFra" forKey:@"CmdType"];
    
    //    __weak typeof(self) weakself = self;
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                failureBlock();
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        successBlock(rspDict);
                    }
                    else
                        failureBlock();
                }
                else
                    failureBlock();
            }
            else
                failureBlock();
        }
        
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        failureBlock();
    }];
    
}
#pragma mark 11、修改密码
+(void)modifyPassword:(NSString *)userName andOldPassword:(NSString *)oldpassword andNewPassword:(NSString *)newpassword andSuccess:(getNetDataSuccess)successBlock andFailure:(getNetDataFailure)failureBlock;{
    
    AFHTTPSessionManager *mgr = [AFHTTPSessionManager manager];
    mgr.requestSerializer = [AFJSONRequestSerializer serializer];
    mgr.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json",@"text/json", @"text/plain", @"text/html", @"text/javascript", nil];
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
    
    [params setObject:@"ChangePwd" forKey:@"CmdType"];
    [params setObject:userName forKey:@"CustomerAccount"];
    [params setObject:oldpassword forKey:@"CustomerOldPassword"];
    [params setObject:newpassword forKey:@"CustomerNewPassword"];
    //    __weak typeof(self) weakself = self;
    [mgr POST:ServerUrl parameters:params success:^(NSURLSessionDataTask *task, NSDictionary* responseObject) {
        if (responseObject) {
            if (![[responseObject allKeys]containsObject:@"Response"]) {
                failureBlock();
                return ;
            }
            NSDictionary *resDict = [responseObject objectForKey:@"Response"];
            if ([[resDict allKeys]containsObject:@"RspInfo"]) {
                NSDictionary *rspDict = [resDict objectForKey:@"RspInfo"];
                if ([[rspDict allKeys]containsObject:@"code"]) {
                    NSString *reValue = [rspDict objectForKey:@"code"];
                    if ([reValue isEqualToString:@"0000"]) {
                        //登录成功
                        successBlock(rspDict);
                    }
                    else
                        failureBlock();
                }
                else
                    failureBlock();
            }
            else
                failureBlock();
        }
        
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        failureBlock();
    }];
}
@end
