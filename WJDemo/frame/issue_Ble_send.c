
#include "issue_Ble_send.h"
#include "lib2hd.h"
#include "common.h"
#include "lib2rsu.h"
#include "esam.h"
#include "psam.h"
#include "icc.h"


int tsend_c1_Ble_OC(PROG_COMM_C0 prog_c0) {
    int ret = SUCCESS;
    uint32 len = 0;
    uint8 i = 0, chk = 0;
    uint8 buf[512] = { 0 };
    int TerminalTradeNO = 0;
    //∏¯»´æ÷±‰¡ø÷°÷–µƒ÷’∂ÀΩª“◊–Ú∫≈∏≥÷µchange by wll 2013-5-27 14:00:16
    //	g_frame_uploadtradeinfo_rq.TerminalTradeNO = TerminalTradeNO ;
    for (i = 3; (i >= 0) && (i <= 3); i--) {
        g_frame_writeveh_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
        g_frame_writesys_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
        g_frame_readveh_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
        g_frame_syscheck_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
        g_frame_querybackInfo_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
        g_frame_uploadtradeinfo_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
        g_frame_logoutvalidate_rq.TerminalTradeNO[i] = TerminalTradeNO & 0xFF;
        TerminalTradeNO = TerminalTradeNO >> 8;
    }
    
    buf[len++] = 0xFF;
    buf[len++] = 0xFF;
    g_rsctl = 0;
    g_rsctl = (g_rsctl + 1) % 16;
    g_rsctl = g_rsctl | 0x00;
    buf[len++] = g_rsctl; //÷°–Ú∫≈
    buf[len++] = 0xFF;
    buf[len++] = 0x00;
    buf[len++] = 0x00;
    buf[len++] = 0x04;
    g_rsctl = 0;
    g_rsctl = (g_rsctl + 1) % 16;
    g_rsctl = g_rsctl | 0x00;
    buf[len++] = g_rsctl;	//prog_c0.RSCTL
    buf[len++] = prog_c0.CMDType;
    for (i = 0; i < 4; i++) {
        buf[len++] = prog_c0.UnixTime[i];
    }
    buf[len++] = prog_c0.BSTInterval;
    buf[len++] = prog_c0.TxPower;
    buf[len++] = prog_c0.PHYChannelID;
    buf[len++] = prog_c0.CRCMode;
    buf[len++] = prog_c0.RFPower;
    buf[len++] = prog_c0.PSAMPower;
    run_mode = prog_c0.run_mode;
    for (i = 0; i < 2; i++) {
        buf[len++] = prog_c0.Reserved[i];
    }
    buf[5] = len - 6;
    chk = buf[2];	//µ⁄“ª∏ˆÀ„CRCµƒ ˝æ›
    for (i = 3; i < len; i++) {
        chk = chk ^ buf[i];
    }
    buf[len++] = chk & 0xFF;
    buf[len++] = 0xFF;
    g_com_tx_len = pkt_code(&buf[0], &g_com_tx_buf[0], len); //g_com_tx_buf为发送数据
   
    memset(&g_com_rx_buf, 0, sizeof(g_com_rx_buf));//接收清空
    
    return 1;
}

int send_c5_Ble_OC(PROG_COMM_C5 prog_c5) {
    g_frame_uploadtradeinfo_rq.OnLineDenoteByte = 0xA5;
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
    len_pos = slen++; 		//¡Ù≥ˆ◊‹≥§∂»Œª÷√
    //◊È∞¸transferchannel_rq
    sbuf[slen++] = vst.macid[0];
    sbuf[slen++] = vst.macid[1];
    sbuf[slen++] = vst.macid[2];
    sbuf[slen++] = vst.macid[3];
    sbuf[slen++] = 0x40;	//macøÿ÷∆”Ú
    //	sbuf[slen++] = 0xf7;	//llcøÿ÷∆”Ú
    sbuf[slen++] = 0x77 | g_u8LLCFlag ;	//llcøÿ÷∆”Ú,0xf7
    if(g_u8LLCFlag == 0x80)
        g_u8LLCFlag = 0x00;
    else
        g_u8LLCFlag = 0x80;
    sbuf[slen++] = 0x91;	//∂Œ◊÷Õ∑
    sbuf[slen++] = 0x05;	//action±Í ∂0x05
    sbuf[slen++] = 0x01;	//DID
    sbuf[slen++] = 0x04;	//actiontype
    sbuf[slen++] = 0x1A;	//»›∆˜¿‡–Õ
    sbuf[slen++] = prog_c5.SetMMIMode;
    sbuf[len_pos] = slen - len_pos - 1;
    chk = sbuf[2];
    for(i = 3;i < slen;i++)
    {
        chk = chk  ^ sbuf[i];
    }
    sbuf[slen++] = chk & 0xff;
    sbuf[slen++] = 0xff;
    g_com_tx_len = pkt_code(&sbuf[0],&g_com_tx_buf[0],slen);
    return 1;
}

int send_c9_Ble_OC(PROG_COMM_C4 prog_c4, int time_out) {
    int i, ret, did, datalist, sys_flag = 0;
    int icc_flag = 0, icc_offset = 0, icc_Length = 0;
    int keyid_for_encrypt_op, keyid_for_encrypt, psam_slot = g_psam_slot;
    int gs_veh_len;
    char gs_veh_file[128];
    char gs_authenticator[8];
    char after_decrypt_vehinfo[128];
    char after_decrypt_mac[8];
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
        ret = TransferChannel_rq(did, transfer_rq.channelid,
                                 transfer_rq.apdulist, transfer_rq.apdu);
        if (ret != SUCCESS) {
            return -1 + ret * 100;	//¥Æø⁄∑¢ÀÕ ß∞‹
        }
        ret = TransferChannel_rs(&datalist, data, time_out);
        if (ret != SUCCESS) {
          
            return -2 + ret * 100;	//Ω‚Œˆ ß∞‹ªÚ’ﬂ≥¨ ±
        }
        
        int i = 0;
        for (i = 0; i < 30; i++)
          
            ret = esamCheck(data, 0);
        if (ret != SUCCESS) {
            return -3 + ret * 100;
        }
        ret = esamCheckReadSysInfo(data, 1);
        //print_info_Info(data,50,UDPNET);
        
        if (ret != SUCCESS) {
            return -4 + ret * 100;
        }
        if ((vst.obustatus[0] & 0x80) == 0x00) //»Áπ˚OBU≤Âø®,‘Ú∂¡»°0002∫Õ0015Œƒº˛
        {           
            did = 0x01;	//”¶”√÷˜ƒø¬º
            iccInitFrame(&transfer_rq);
            iccReadMoneyFrame(&transfer_rq);
            iccReadFileFrame(&transfer_rq, 0x0015, 0x00, 0x2b);
            ret = TransferChannel_rq(did, transfer_rq.channelid,
                                     transfer_rq.apdulist, transfer_rq.apdu);
            if (ret != SUCCESS) {
                return -5 + ret * 100;	//¥Æø⁄∑¢ÀÕ ß∞‹
            }
            ret = TransferChannel_rs(&datalist, data, time_out);
            if (ret != SUCCESS) {
                return -6 + ret * 100;	//Ω‚Œˆ ß∞‹ªÚ’ﬂ≥¨ ±
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
        //Ã·»°œµÕ≥–≈œ¢÷–µƒ≤Œ ˝
        for (k = 0; k < 4; k++) {
            g_frame_syscheck_rq.OBUContractSignedDate[k] = esam_read_sysinfo[k
                                                                             + 18 + g_read_file.offset[0]];
        }
        for (k = 0; k < 4; k++) {
            g_frame_syscheck_rq.OBUContractExpiredDate[k] = esam_read_sysinfo[k
                                                                              + 22 + g_read_file.offset[0]];
        }
        g_frame_syscheck_rq.DisassembleState = esam_read_sysinfo[k + 22
                                                                 + g_read_file.offset[0]];
        
        return SUCCESS;
    }/* end if(sys_flag == 1) */
    
    
    if (icc_flag != 0) {
        
        if ((vst.obustatus[0] & 0x80) == 0x00) {
            did = 0x01;	//”¶”√÷˜ƒø¬º
            iccInitFrame(&transfer_rq);
            iccReadFileFrame(&transfer_rq, icc_flag, icc_offset, icc_Length);
          
            ret = TransferChannel_rq(did, transfer_rq.channelid,
                                     transfer_rq.apdulist, transfer_rq.apdu);
            if (ret != SUCCESS) {
                
                return -5 + ret * 100;	//¥Æø⁄∑¢ÀÕ ß∞‹
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

