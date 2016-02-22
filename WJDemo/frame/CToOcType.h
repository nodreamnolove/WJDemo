

#ifndef CToOcType_h
#define CToOcType_h

#define uint8 uint8_t
#define  int8 int8_t
#define uint16 uint16_t
#define int16 int16_t

#define uint32 uint32_t
#define int32 int32_t

#define byte  unsigned char
//每包大小
#define NOTIFY_MTU      20


int g_timeout_cnt = 0;
int g_timeout_max = 20;

inline int checktimeout();
int checktimeout(){
    if(g_timeout_cnt < g_timeout_max){
        
        g_timeout_cnt++;
        return 0;
    }else{
        g_timeout_cnt = g_timeout_max;
        
        return -1;
    }
}



#endif /* CToOcType_h */


/*


1、getCardInformation：
 
 发c1(send_c1_Ble_OC)    b1≤
 c9≥    b9≤
 c5≥    b4≤
 
2、getObuInformation:(与getCardInformation一样)
 c1≥    b1≤
 c9≥    b9≤
 c5≥    b4≤
 
3、loadCreditGetMac1:
 c1≥    b1≤
 TransferChannel_rq≥    TransferChannel_rs≤
 TransferChannel_rq≥    TransferChannel_rs≤
 TransferChannel_rq≥    TransferChannel_rs≤
 
4、loadCreditWriteCard:
 TransferChannel_rq≥    TransferChannel_rs≤
 c5≥    b4≤
 
5、readCardTransactionRecord:
 c1≥    b1≤
 c9≥    b9≤
 c5≥    b4≤
 
6、readCardConsumeRecord:
 c1≥    b1≤
 c9≥    b9≤
 c5≥    b4≤

7、readCardOwnerRecord：
 c1≥    b1≤
 c9≥    b9≤
 c5≥    b4≤
 
8、transCommand:
 c1≥    b1≤
 c5≥    b4≤
























*/