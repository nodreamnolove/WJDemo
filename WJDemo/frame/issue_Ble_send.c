
#include "issue_Ble_send.h"
#include "issue_Ble_recv.h"
#include "lib2hd.h"
#include "common.h"
#include "lib2rsu.h"
#include "esam.h"
#include "psam.h"
#include "icc.h"
//c1初始化
int c1_init(PROG_COMM_C1 prog_c1)
{
    short i = 0;
    prog_c1.RSCTL = (byte) 0x80;
    prog_c1.CMDType = (byte) 0xC1;
    prog_c1.ObuInitMode = 0x00;
    for (i = 0; i < 5; i++) {
        prog_c1.Reserved[i] = 0x00;
    }
    return 1;
}
//c9初始化nType=0x01
int c4_init(PROG_COMM_C4 prog_c4,byte nType)
{
    prog_c4.RSCTL = (byte) 0x81;
    prog_c4.CMDType = (byte) 0xC4;
    prog_c4.C4Flag = 0;
    if (nType == 0x01) {
        prog_c4.NumOfFiles = 0x01;
        prog_c4.DIDnFID[0] = 0x01;
        prog_c4.Offset[0] = 0x00;
        prog_c4.Length[0] = 0x1B;
    } else if (nType == 0x02) {
        prog_c4.NumOfFiles = 0x01;
        prog_c4.DIDnFID[0] = 0x11;
        prog_c4.Offset[0] = 0x00;
        prog_c4.Length[0] = 0x3B;
    } else if (nType == 0x03) {
        prog_c4.NumOfFiles = 0x02;
        prog_c4.DIDnFID[0] = 0x01;
        prog_c4.Offset[0] = 0x00;
        prog_c4.Length[0] = 0x1B;
        prog_c4.DIDnFID[1] = 0x11;
        prog_c4.Offset[1] = 0x00;
        prog_c4.Length[1] = 0x3B;
    } else {
        prog_c4.NumOfFiles = 0x01;
        prog_c4.DIDnFID[0] = 0x01;
        prog_c4.Offset[0] = 0x00;
        prog_c4.Length[0] = 0x1B;
    }
    return 1;
}
int  c5_init(PROG_COMM_C5 prog_c5)
{
    short i = 0;
    prog_c5.RSCTL = (byte) 0x82;
    prog_c5.CMDType = (byte) 0xC5;
    prog_c5.SetMMIMode = 0x00;
    for (i = 0; i < 5; i++) {
        prog_c5.Reserved[i] = 0x00;
    }
    return 1;
}
#pragma mark 发送C1帧 
//返回编码长度
int send_c1_Ble_OC(PROG_COMM_C1 prog_c1) {
    int ret = SUCCESS, profile;
    c1_init(prog_c1);//C1初始化
    char beacon_id[4] = { 0 };
    char unix_time[4] = { 0 };
    static uint32 count = 1;
    time_t ltime;
    g_bst_type = BST_TRADE_GB;
    beacon_id[0] = 0x08;
    beacon_id[1] = (count >> 16) & 0xff;
    beacon_id[2] = (count >> 8) & 0xff;
    beacon_id[3] = count & 0xff;
    count++;
    time(&ltime);
    unix_time[0] = (ltime >> 24) & 0xff;
    unix_time[1] = (ltime >> 16) & 0xff;
    unix_time[2] = (ltime >> 8) & 0xff;
    unix_time[3] = ltime & 0xff;
    profile = prog_c1.Reserved[0];
    g_ObuInitMode = prog_c1.ObuInitMode;
    g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;
    ret = INITIALISATION_rq_OC(g_bst_type, beacon_id, unix_time, profile,prog_c1.ObuInitMode);
    return ret;
}
#pragma mark 发送C5帧 返回帧长度
int send_c5_Ble_OC(PROG_COMM_C5 prog_c5) {
    g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;
    c5_init(prog_c5);
    int ret;
    g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;
    ret = SetMMI_rq_OC(prog_c5.SetMMIMode);
    return ret;
}
#pragma mark 发送C9帧
int send_c9_Ble_OC(PROG_COMM_C4 prog_c4, int time_out) {
    int i, ret, did, datalist, sys_flag = 0;
    int icc_flag = 0, icc_offset = 0, icc_Length = 0;
    uint8 data[128];
    ST_TRANSFER_CHANNEL transfer_rq;
    g_read_file.NumOfFiles = prog_c4.NumOfFiles;
    g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;
    for (i = 0; i < prog_c4.NumOfFiles; i++) {
        g_read_file.DIDnFID[i] = prog_c4.DIDnFID[i];
        g_read_file.offset[i] = prog_c4.Offset[i];
        g_read_file.len[i] = prog_c4.Length[i];
        if (prog_c4.DIDnFID[i] == 0x01) {
            sys_flag = 1;
        }
        if ((prog_c4.DIDnFID[i] == 0x02) || (prog_c4.DIDnFID[i] == 0x12)
            || (prog_c4.DIDnFID[i] == 0x15) || (prog_c4.DIDnFID[i] == 0x19))//∂¡ICø®–≈œ¢
        {
            icc_flag = prog_c4.DIDnFID[i];
            icc_offset = prog_c4.Offset[i];
            icc_Length = prog_c4.Length[i];
        }
       
        
    }
    
    if (sys_flag == 1) {
        if (prog_c4.C4Flag == 0) {
            did = 0x01;
        } else {
            did = 0x03;
            vst.obustatus[0] = 0x80;
        }
        esamInitFrame(&transfer_rq);
        esamEnterDirFrame(&transfer_rq, 0x3f00);
        esamReadSysInfoFrame(&transfer_rq, prog_c4.Offset[0],
                             prog_c4.Length[0]);
        ret = TransferChannel_rq_OC(did, transfer_rq.channelid,transfer_rq.apdulist, transfer_rq.apdu);
        if (ret != SUCCESS) {
            return -1 + ret * 100;
        }
        ret = TransferChannel_rs(&datalist, data, time_out);
        if (ret != SUCCESS) {
          
            return -2 + ret * 100;
        }
        
        int i = 0;
        for (i = 0; i < 30; i++)
          
            ret = esamCheck(data, 0);
        if (ret != SUCCESS) {
            return -3 + ret * 100;
        }
        ret = esamCheckReadSysInfo(data, 1);
       
        
        if (ret != SUCCESS) {
            return -4 + ret * 100;
        }
        if ((vst.obustatus[0] & 0x80) == 0x00)
        {           
            did = 0x01;
            iccInitFrame(&transfer_rq);
            iccReadMoneyFrame(&transfer_rq);
            iccReadFileFrame(&transfer_rq, 0x0015, 0x00, 0x2b);
            ret = TransferChannel_rq_OC(did, transfer_rq.channelid, transfer_rq.apdulist, transfer_rq.apdu);
            if (ret != SUCCESS) {
                return -5 + ret * 100;
            }
            ret = TransferChannel_rs(&datalist, data, time_out);
            if (ret != SUCCESS) {
                return -6 + ret * 100;
            }
            ret = iccCheck(data, 0);
            if (ret != SUCCESS) {
                return -7 + ret * 100;
            }
            ret = iccCheck(data, 1);
            if (ret != SUCCESS) {
                return -8 + ret * 100;
            }
            memcpy(icc_pib.Balance, &data[1], 4);	//0002Œƒº˛
            memcpy(icc_pib.icc0015, &data[8], 43);	//0015Œƒº˛
        }
        int k;
        
        for (k = 0; k < 4; k++) {
            g_frame_syscheck_rq.OBUContractSignedDate[k] = esam_read_sysinfo[k + 18 + g_read_file.offset[0]];
        }
        for (k = 0; k < 4; k++) {
            g_frame_syscheck_rq.OBUContractExpiredDate[k] = esam_read_sysinfo[k + 22 + g_read_file.offset[0]];
        }
        g_frame_syscheck_rq.DisassembleState = esam_read_sysinfo[k + 22 + g_read_file.offset[0]];
        
        return SUCCESS;
    }/* end if(sys_flag == 1) */
    
    
    if (icc_flag != 0) {
        
        if ((vst.obustatus[0] & 0x80) == 0x00) {
            did = 0x01;	//”¶”√÷˜ƒø¬º
            iccInitFrame(&transfer_rq);
            iccReadFileFrame(&transfer_rq, icc_flag, icc_offset, icc_Length);
          
            ret = TransferChannel_rq_OC(did, transfer_rq.channelid, transfer_rq.apdulist, transfer_rq.apdu);
            if (ret != SUCCESS) {
                
                return -5 + ret * 100;
            }
         
            ret = TransferChannel_rs(&datalist, data, time_out);
          
            if (ret != SUCCESS) {
               
                return -6 + ret * 100;	//Ω‚Œˆ ß∞‹ªÚ’ﬂ≥¨ ±
            }
            
            ret = iccCheck(data, 0);
            if (ret != SUCCESS) {
                
                return -7 + ret * 100;
            }
          
            if (icc_flag == 0x0002) {
                memcpy(icc_pib.Balance, &data[1], icc_Length);	//0002Œƒº˛
            } else if (icc_flag == 0x0012) {
                memcpy(icc_pib.icc0012, &data[1], icc_Length);	//0012Œƒº˛
            } else if (icc_flag == 0x0015) {
                memcpy(icc_pib.icc0015, &data[1], icc_Length);	//0015Œƒº˛
            } else if (icc_flag == 0x0019) {
                memcpy(icc_pib.icc0019, &data[1], icc_Length);	//0019Œƒº˛
            }
        } else {
            return -8;
        }
    }
    
    return SUCCESS;
}


int TransferChannel_rq_OC(int DID,int ChannelID,int APDULIST,uint8 * APDU)
{
    int ret = 0;
    uint8 chk = 0;
    uint8 sbuf[BUF_LEN] = {0};
    uint16 slen = 0, len_pos = 0, i = 0, apdulen = 0;
    sbuf[slen++] = 0xff;
    sbuf[slen++] = 0xff;
    g_rsctl = (g_rsctl + 1) % 16;
    g_rsctl = g_rsctl | 0x00;
    sbuf[slen++] = g_rsctl; //÷°–Ú∫≈
    sbuf[slen++] = 0xf0; 	//√¸¡Ó∫≈,bst“‘º∞action‘≠”Ô
    sbuf[slen++] = 0x02;	//transferchannel_rq
    len_pos = slen++;
    //◊È∞¸transferchannel_rq
    sbuf[slen++] = vst.macid[0];
    sbuf[slen++] = vst.macid[1];
    sbuf[slen++] = vst.macid[2];
    sbuf[slen++] = vst.macid[3];
    sbuf[slen++] = 0x40;
    sbuf[slen++] = 0x77 | g_u8LLCFlag ;
    if(g_u8LLCFlag == 0x80)
        g_u8LLCFlag = 0x00;
    else
        g_u8LLCFlag = 0x80;
    sbuf[slen++] = 0x91;
    sbuf[slen++] = 0x05;
    sbuf[slen++] = DID;
    sbuf[slen++] = 0x03;
    sbuf[slen++] = 0x18;
    sbuf[slen++] = ChannelID;
    sbuf[slen++] = APDULIST;
    apdulen = 0;
    for(i = 0; i < APDULIST; i++)
    {
        apdulen += APDU[apdulen] + 1;
    }
    for(i = 0; i < apdulen; i++)
    {
        sbuf[slen++] = APDU[i];
        
    }
    sbuf[len_pos] = slen - len_pos - 1;
    chk = sbuf[2];
    for(i = 3;i < slen;i++)
    {
        chk = chk  ^ sbuf[i];
    }
    sbuf[slen++] = chk & 0xff;
    sbuf[slen++] = 0xff;
    g_com_tx_len = pkt_code(&sbuf[0], &g_com_tx_buf[0], slen);
    memset(&g_com_rx_buf,0,sizeof(g_com_rx_buf));
    return 1;
}

int INITIALISATION_rq_OC(int bst_type, char *beacon_id, char *unix_time, int profile, int obu_init_mode)
{
    int i;
    int slen;
    int len_pos;
    int ret;
    uint8 chk = 0;
    uint8 sbuf[512];
    memset(sbuf,0,512);
    slen = 0;
    sbuf[slen++] = 0xff;
    sbuf[slen++] = 0xff;
    g_rsctl = (g_rsctl+1)%16;
    g_rsctl = g_rsctl | 0x00;
    sbuf[slen++] = g_rsctl; // ÷°–Ú∫≈
    sbuf[slen++] = 0xf0; // √¸¡Ó∫≈,bst“‘º∞action‘≠”Ô
    sbuf[slen++] = 0x00; //bst
    len_pos = slen++;    //¡Ù≥ˆ◊‹≥§∂»Œª÷√
    
    sbuf[slen++] = 0xff;
    sbuf[slen++] = 0xff;
    sbuf[slen++] = 0xff;
    sbuf[slen++] = 0xff;
    sbuf[slen++] = 0x50;
    sbuf[slen++] = 0x03;
    sbuf[slen++] = 0x91; //∂Œ◊÷Õ∑
    sbuf[slen++] = 0xc0; //bst±Í ∂
    sbuf[slen++] = beacon_id[0];
    sbuf[slen++] = beacon_id[1];
    sbuf[slen++] = beacon_id[2];
    sbuf[slen++] = beacon_id[3];
    sbuf[slen++] = unix_time[0];
    sbuf[slen++] = unix_time[1];
    sbuf[slen++] = unix_time[2];
    sbuf[slen++] = unix_time[3];
    sbuf[slen++] = profile;
    sbuf[slen++] = 0x01;
#if 1
    if(bst_type == BST_ISSUE_BJ)
    {
        if(obu_init_mode == 0x2f)
            sbuf[slen++] = 0x41; //paramter¥Ê‘⁄,AID = 0±±æ©±Í æ∑¢––
        else
            sbuf[slen++] = 0x40; //paramter¥Ê‘⁄,AID = 0±±æ©±Í æ∑¢––
        sbuf[slen++] = obu_init_mode;
        sbuf[slen++] = 0x00; //profilelist,Œﬁ¿©’π
    }
    
#endif
#if 1
    if(bst_type == BST_TRADE_GB)
    {
        sbuf[slen++] = 0x41;
        sbuf[slen++] = 0xaf;
        sbuf[slen++] = 0x29;
        sbuf[slen++] = 0xa0;
        sbuf[slen++] = 0x1a;
        sbuf[slen++] = 0x00; //0002Œƒº˛∆´“∆¡ø
        sbuf[slen++] = ICC_0002_LEN; //0002Œƒº˛≥§∂»
        sbuf[slen++] = 0x00; //0015Œƒº˛∆´“∆¡ø
        sbuf[slen++] = ICC_0015_LEN; //0015Œƒº˛≥§∂»
    }
    else if(bst_type == BST_ISSUE_NOPARAM)
    {
        sbuf[slen++] = 0x01;//paramter≤ª¥Ê‘⁄
    }
    else if(bst_type == BST_ISSUE_NOREAD)
    {
        sbuf[slen++] = 0x41;//paramter¥Ê‘⁄
        sbuf[slen++] = 0x87;
        sbuf[slen++] = 0x29;
        sbuf[slen++] = 0x00;
        sbuf[slen++] = 0x1a;
    }
    else if(bst_type == BST_ISSUE_PREREAD)
    {
        sbuf[slen++] = 0x41;//paramter¥Ê‘⁄
        sbuf[slen++] = 0x87;
        sbuf[slen++] = 0x29;
        sbuf[slen++] = 0xb0;
        sbuf[slen++] = 0x1a;
        sbuf[slen++] = 0x00;
        sbuf[slen++] = 0x00;
        sbuf[slen++] = 0x00;
        sbuf[slen++] = 0x00;
        sbuf[slen++] = 0x00;
        sbuf[slen++] = 0x00;
    }
    else if(bst_type == BST_TRADE_ZYL)
    {
        sbuf[slen++] = 0x51;
        sbuf[slen++] = 0x01;
        sbuf[slen++] = 0x00;
        sbuf[slen++] = 0x06;
    }
    else if(bst_type == BST_ISSUE_ZYL)
    {
        sbuf[slen++] = 0x11;
    }
    sbuf[slen++] = 0x00;
#endif
    sbuf[len_pos] = slen - len_pos - 1;
    chk = sbuf[2];
    for (i = 3; i < slen; i++) {
        chk = chk ^ sbuf[i];
    }
    sbuf[slen++] = chk & 0xff;
    sbuf[slen++] = 0xff;
    memset(&g_com_rx_buf,0,sizeof(g_com_rx_buf));
    g_com_tx_len = pkt_code(&sbuf[0], &g_com_tx_buf[0], slen);
    return g_com_tx_len;
}

int SetMMI_rq_OC(int SetMMIPara)
{
    int ret = SUCCESS;
    uint8 chk = 0;
    uint8 sbuf[BUF_LEN] = {0};
    uint16 slen = 0, len_pos = 0, i = 0;
    sbuf[slen++] = 0xff;
    sbuf[slen++] = 0xff;
    g_rsctl = (g_rsctl + 1) % 16;
    g_rsctl = g_rsctl | 0x00;
    sbuf[slen++] = g_rsctl; //÷°–Ú∫≈
    sbuf[slen++] = 0xf0; 	//√¸¡Ó∫≈,bst“‘º∞action‘≠”Ô
    sbuf[slen++] = 0x03;	//Setmmi_rq
    len_pos = slen++;
    sbuf[slen++] = vst.macid[0];
    sbuf[slen++] = vst.macid[1];
    sbuf[slen++] = vst.macid[2];
    sbuf[slen++] = vst.macid[3];
    sbuf[slen++] = 0x40;
    sbuf[slen++] = 0x77 | g_u8LLCFlag ;
    if(g_u8LLCFlag == 0x80)
        g_u8LLCFlag = 0x00;
    else
        g_u8LLCFlag = 0x80;
    sbuf[slen++] = 0x91;	//∂Œ◊÷Õ∑
    sbuf[slen++] = 0x05;	//action±Í ∂0x05
    sbuf[slen++] = 0x01;	//DID
    sbuf[slen++] = 0x04;	//actiontype
    sbuf[slen++] = 0x1A;	//»›∆˜¿‡–Õ
    sbuf[slen++] = SetMMIPara;
    sbuf[len_pos] = slen - len_pos - 1;
    chk = sbuf[2];
    for(i = 3;i < slen;i++)
    {
        chk = chk  ^ sbuf[i];
    }
    sbuf[slen++] = chk & 0xff;
    sbuf[slen++] = 0xff;
    g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);
    return g_com_tx_len;
}

