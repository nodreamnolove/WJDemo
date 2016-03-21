#import <Foundation/Foundation.h>

//1.回调函数(status成功、失败，data数据，errorMsg报错信息)
typedef void(^obuCallBack)(BOOL status,id data, NSString *errorMsg);

@interface ObuSDK : NSObject
//2.创建ObuSDK对象的实例（单例）
+(instancetype)sharedObuSDK;

//3.查询蓝牙打开状态 打开返回YES 关闭返回NO
@property (nonatomic,assign,getter=isEnableBluetooth) BOOL   bluetoothState;

//4.连接OBU
-(void)connectDevice:(int)timeout    callBack:(obuCallBack)callBack;

//5.断开OBU连接
-(void)disconnectDevice:(obuCallBack)callBack;

//6.读取OBU的卡片信息
-(void)getCardInformation:(obuCallBack)callBack;

//7.读取OBU的设备信息
-(void)getObuInformation:(obuCallBack)callBack;

//8.充值写卡：获取Mac1等数据
-(void)loadCreditGetMac1:(NSString *)credit \
cardId:(NSString*)cardId  \
terminalNo:(NSString *)terminalNo \
picCode:(NSString *)pinCode \
procType:(NSString*)procType \
keyIndex:(NSString *)keyIndex \
callBack:(obuCallBack)callBack;

//9.充值写卡：执行写卡操作
-(void)loadCreditWriteCard:(NSString *)dateMAC2 callBack:(obuCallBack)callBack;

//10.读终端交易记录文件
-(void)readCardTransactionRecord:(NSString *)pinCode maxNumber:(NSInteger)maxNumber callBack:(obuCallBack)callBack;

//11.读联网收费复合消费过程文件
-(void)readCardConsumeRecord:(NSInteger)maxNumber callBack:(obuCallBack)callBack;;

//12.读持卡人基本数据文件
-(void)readCardOwnerRecord:(obuCallBack)callBack;;

//13.数据透传
-(void)transCommand:(NSData*)reqData callBack:(obuCallBack)callBack;


@end
