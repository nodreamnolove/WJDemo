#include "issue_Ble_recv.h"
#include "common.h"
#include "lib2rsu.h"

int recv_b1_Ble_OC(PROG_COMM_B1 *prog_b1, int time_out)
{
    uint8 i = 0;
    ST_TRANSFER_CHANNEL transfer_rq;
    int did, ret = SUCCESS, datalist;
    uint8 data[128] = { 0 };
    memset(prog_b1, 0x00, sizeof(PROG_COMM_B1));
    prog_b1->ErrorCode = 0x00;
    memset(&vst, 0xFF, sizeof(ST_VST));
    ret = INITIALISATION_rs_OC(vst.macid, vst.sysinfo, vst.iccinfo, vst.rndinfo,vst.resetinfo, &vst.obustatus[0], time_out);
    if (ret != SUCCESS) {
        prog_b1->ErrorCode = 0x01;
    }
    prog_b1->RSCTL = 0x13;
    prog_b1->FrameType = 0xb1;
    memcpy(prog_b1->ContractProvider, vst.sysinfo, 26);
    g_OBU_ContractVersion = prog_b1->ContractVersion;
    g_frame_readveh_rq.OBUContractVersion = vst.sysinfo[9];
    g_frame_writeveh_rq.OBUContractVersion = vst.sysinfo[9];
    g_frame_querybackInfo_rq.OBUContractVersion = vst.sysinfo[9];
    g_frame_uploadtradeinfo_rq.OBUContractVersion = vst.sysinfo[9];    
    for (i = 0; i < 8; i++) {
        g_frame_readveh_rq.OBUContractSerialNO[i] = vst.sysinfo[10 + i];
        g_frame_writeveh_rq.OBUContractSerialNO[i] = vst.sysinfo[10 + i];
        g_frame_writesys_rq.CopherDecentGene[i] = vst.sysinfo[10 + i];
        g_frame_syscheck_rq.OBUContractSerialNO[i] = vst.sysinfo[10 + i];
        g_frame_querybackInfo_rq.OBUContractSerialNO[i] = vst.sysinfo[10 + i];
        g_frame_uploadtradeinfo_rq.OBUContractSerialNO[i] = vst.sysinfo[10 + i];
    }
    if (g_ObuInitMode == 0x2f) {
        for (i = 0; i < 8; i++) {
            g_frame_writeveh_rq.GBCPUCardID[i] = vst.iccinfo[6 + i];
            g_frame_querybackInfo_rq.GBCPUCardID[i] = vst.iccinfo[6 + i];
            g_frame_uploadtradeinfo_rq.GBCPUCardID[i] = vst.iccinfo[6 + i];
        }
    }
    for (i = 0; i < 4; i++) {
       
        EsamDv[i] = vst.resetinfo[i + 1];
        EsamDv[i + 4] = vst.resetinfo[i + 11];
    }
    if ((g_frame_uploadtradeinfo_rq.OBUContractVersion == 0x11)
        || (g_frame_uploadtradeinfo_rq.OBUContractVersion == 0x12)) {
        for (i = 0; i < 8; i++)
        {
            g_frame_uploadtradeinfo_rq.CopherDecentGene[i] = EsamDv[i];
            g_frame_writesys_rq.CopherDecentGene[i] = EsamDv[i];
        }
    } else if ((g_frame_uploadtradeinfo_rq.OBUContractVersion == 0x00)
               || (g_frame_uploadtradeinfo_rq.OBUContractVersion == 0x10)) {
        for (i = 0; i < 4; i++)
        {
            g_frame_uploadtradeinfo_rq.CopherDecentGene[i] = vst.macid[i];
            g_frame_uploadtradeinfo_rq.CopherDecentGene[i + 4] =  ~(vst.macid[i]);
           
            g_frame_writesys_rq.CopherDecentGene[i] = vst.macid[i];
            g_frame_writesys_rq.CopherDecentGene[i + 4] = ~(vst.macid[i]);
        }
    } else {
        for (i = 0; i < 8; i++)
        {
            g_frame_uploadtradeinfo_rq.CopherDecentGene[i] =
            vst.sysinfo[10 + i];
        }
    }
    if (ret != SUCCESS) {
        return -1 + ret * 100;
    }
    for (i = 0; i < 4; i++) {
        prog_b1->Reserved[i] = vst.macid[i];
    }
    prog_b1->Reserved[4] = vst.obustatus[0];
    prog_b1->Reserved[5] = vst.obustatus[2];
    if (vst.iccinfo[0] == 0x00)
    {
        for (i = 0; i < 8; i++) {
            g_frame_writeveh_rq.GBCPUCardID[i] = 0xFF;
            g_frame_querybackInfo_rq.GBCPUCardID[i] = 0xFF;
        }
        for (i = 0; i < 100; i++) {
            prog_b1->FileContent_ICCInfo[i] = 0;
        }
    } else {
        for (i = 0; i < vst.iccinfo[0] + 1; i++) {
            prog_b1->FileContent_ICCInfo[i] = vst.iccinfo[i];
        }
    }
    return 1;
}

int recv_b4_Ble_OC(PROG_COMM_C5 prog_c5)
{
    
    return 1;
}

int recv_b9_Ble_OC(PROG_COMM_C4 prog_c4, int time_out)
{
    return 1;
}


int TransferChannel_rs_OC(int DID,int ChannelID,int APDULIST,uint8 * APDU)
{
    return 1;
}

int INITIALISATION_rs_OC(char *obu_mac, char *sys_info, char *icc_info, char *rnd_info, char *reset_info, char *obu_status, int time_out)
{
    int len_icc_info = 0;
    int pos,i,ret;
    uint8 vst_len = 0;
    uint8 mac_control = 0;
    uint8 dsrc_op = 0;
    uint8 container_ops = 0;
    char InfoTmp[100];
    memset(icc_info,0x00,sizeof(icc_info));
    if(IsVst(g_com_rx_buf) != 0)
    {
        return -2;
    }
    pos = 3;
    vst_len = g_com_rx_buf[pos++];
    //Ω‚Œˆvst
    if(g_bst_type == BST_TRADE_ZYL)
    {
        for(i = 0;i < 4;i++)
        {
            obu_mac[i] = g_com_rx_buf[pos++];
        }
        mac_control = g_com_rx_buf[pos++];	//macøÿ÷∆”Ú0xc0
        pos++;	//llcøÿ÷∆”Ú0x03
        pos++;	//◊÷∂Œ◊÷Õ∑0x91
        pos++;	//vst±Í ∂0xd0
        pos++;	//–≈µ¿∫≈
        pos++;	//0xd1,aidµƒÃÓ≥‰◊÷Ω⁄
        pos++;	//did,0x03
        for(i = 0;i < 8;i++)
        {
            vst.RandOBE[i] = g_com_rx_buf[pos++];	//8◊÷Ω⁄µƒÀÊª˙ ˝
        }
        for(i = 0;i < 12;i++)
        {
            vst.vehicleLicensePlateNumber[i] = g_com_rx_buf[pos++];
        }
        vst.vehicleType = g_com_rx_buf[pos++];
        vst.vehicleUserType = g_com_rx_buf[pos++];
        for(i = 0; i < 8; i++)
        {
            vst.ContractSerialNumber[i] = g_com_rx_buf[pos++];
        }
        for(i = 0; i < 4; i++)
        {
            vst.ObuConfigurationMacID[i] = g_com_rx_buf[pos++];
        }
        for(i = 0; i < 3; i++)
        {
            vst.obustatus[i] = g_com_rx_buf[pos++];
        }
        
    }
    else if(g_bst_type == BST_ISSUE_ZYL)
    {
        for(i = 0;i < 4;i++)
        {
            obu_mac[i] = g_com_rx_buf[pos++];
        }
        mac_control = g_com_rx_buf[pos++];	//macøÿ÷∆”Ú0xc0
        pos++;	//llcøÿ÷∆”Ú0x03
        pos++;	//◊÷∂Œ◊÷Õ∑0x91
        pos++;	//vst±Í ∂0xd0
        pos++;	//–≈µ¿∫≈
        pos++;	//0xd1,aidµƒÃÓ≥‰◊÷Ω⁄
        pos++;	//did,0x03
        //8◊÷Ω⁄µƒ∫œÕ¨–Ú¡–∫≈
        for(i = 0; i < 8; i++)
        {
            g_contract_sn[i] = g_com_rx_buf[pos++];
        }
        pos += 4;	//4◊÷Ω⁄µƒobu macid,3◊÷Ω⁄µƒobustatus
        for(i = 0; i < 3; i++)
        {
            obu_status[i] = g_com_rx_buf[pos++];	//obu◊¥Ã¨–≈œ¢
        }
    }/* end else if(g_bst_type == BST_ISSUE_ZYL) */
    else
    {
        for(i = 0; i < 4; i++)
        {
            obu_mac[i] = g_com_rx_buf[pos++];
        }
        mac_control = g_com_rx_buf[pos++]; //macøÿ÷∆”Ú0xc0
        pos++;	//llcøÿ÷∆”Ú0x03
        if(mac_control == 0xe0)
        {
            pos++;	//◊¥Ã¨◊””Ú
        }
        pos++;	//◊÷∂Œ◊÷Õ∑0x91
        pos++;	//vst±Í ∂0xd0
        pos++;	//–≈µ¿∫≈
        pos++;	//”¶”√¡–±Ì ˝0x01
        dsrc_op = g_com_rx_buf[pos++];	//¥”∏ﬂµΩµÕ,∑÷±±Í ∂didœ‘ æ°¢parameter¥Ê‘⁄°¢aid
        if(Getbit(dsrc_op,7))
        {
            pos++;	//did
        }
        if(Getbit(dsrc_op,6))
        {
            container_ops = g_com_rx_buf[pos++];	//¥”∏ﬂµΩµ÷£¨rndOBE_op°¢privateinfo_op°¢gbiccinfo_op°¢reset_info_op
            pos++;	//»›∆˜¿‡–Õ0x27ªÚ’ﬂ0x20
            //œµÕ≥–≈œ¢»›∆˜
            for(i = 0; i < 26; i++)
            {
                sys_info[i] = g_com_rx_buf[pos++];
            }
            //ÀÊª˙ ˝»›∆˜
            if(Getbit(container_ops,7))
            {
                pos++;	//»›∆˜¿‡–Õ0x1d
                for(i = 0; i < 8; i++)
                {
                    rnd_info[i] = g_com_rx_buf[pos++];
                }
            }
            //privateinfo,ƒø«∞≤ª¥Ê‘⁄
            if(Getbit(container_ops,6))
            {
                /* do nothing */
            }
            //gbiccinfo_op
            if(Getbit(container_ops,5))
            {
                if(g_bst_type == BST_TRADE_GB)	//Ωª“◊¥¯‘§∂¡
                {
                    len_icc_info = ICC_0002_LEN + ICC_0015_LEN;
                    pos++;	//»›∆˜¿‡–Õ0x28
                    icc_info[0] = len_icc_info;
                    for(i = 1; i <= len_icc_info; i ++)
                    {
                        icc_info[i] = g_com_rx_buf[pos++];
                    }
                    print_info_Info(icc_info,len_icc_info + 1,COM);
                }
                else if(g_bst_type == BST_ISSUE_PREREAD)	//∑¢––¥¯‘§∂¡
                {	
                    if (g_com_rx_buf[pos] == 0x28)
                    {
                        pos++;
                    }
                }
                else if (g_bst_type == BST_ISSUE_BJ)	//±±æ©∑¢––
                {
                    icc_info[0] = 62;
                    for(i = 0;i < 62;i++)
                    {
                        icc_info[i+1] = g_com_rx_buf[pos++];
                    }
                }
            }
            //∏¥Œª–≈œ¢»›∆˜,±±æ©∑¢––◊®”√
            //sprintf(InfoTmp,"fu wei xin xi, container_ops = %X\n", container_ops);
            //print_info_Info(InfoTmp,strlen(InfoTmp),COM);
            //∏¥Œª–≈œ¢»›∆˜,±±æ©∑¢––◊®”√
            if(Getbit(container_ops,4))
            {
                pos++;	//»›∆˜¿‡–Õ0x50
                for(i = 0; i < 15; i++)
                {
                    reset_info[i] = g_com_rx_buf[pos++];
                }
                sprintf(InfoTmp,"fu wei xin xi, reset_info = %X %X ..... %X %X\n", reset_info[0],reset_info[1], reset_info[13],reset_info[14]);
                print_info_Info(InfoTmp,strlen(InfoTmp),COM);
                
            }
            
        }/* end if(Getbit(dsrc_op,6)) */
        pos += 4;//obu_mac
        pos++; //”≤º˛∞Ê±æ∫≈
        *obu_status = g_com_rx_buf[pos++];//obu◊¥Ã¨–≈œ¢
        pos++; //obu_status÷–µƒ±£¡Ù◊÷Ω⁄
    }	
    
    if(vst_len != pos - 4)
    {
        
        return -3;
    }
    return 0;
}