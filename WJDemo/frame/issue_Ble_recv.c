#include "issue_Ble_recv.h"
#include "common.h"
#include "lib2rsu.h"

int recv_b1_Ble_OC(PROG_COMM_B1 *prog_b1, int time_out)
{
    uint8 i = 0;
//    ST_TRANSFER_CHANNEL transfer_rq;
    int  ret = SUCCESS;
//    uint8 data[128] = { 0 };
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
    return ret;
}

int recv_b9_Ble_OC(PROG_COMM_B3 *prog_b3 ,int timeout) {
    int i, k;
    prog_b3->RSCTL = 0x13;
    prog_b3->FrameType = 0xb3;
    prog_b3->ErrorCode = 0x00;
    prog_b3->NumOfFiles = g_read_file.NumOfFiles;
    for (i = 0; i < prog_b3->NumOfFiles; i++) {
        if (g_read_file.DIDnFID[i] == 0x01)
        {
            prog_b3->Length[i] = g_read_file.len[i];
            for (k = 0; k < prog_b3->Length[i]; k++) {
                prog_b3->FileContent[i][k] = esam_read_sysinfo[k + g_read_file.offset[i]];
            }
        }
    }
    if ((vst.obustatus[0] & 0x80) == 0x00)
    {
        i = 2;
        prog_b3->Length[i] = 4;
        for (k = 0; k < prog_b3->Length[i]; k++) {
            prog_b3->FileContent[i][k] = icc_pib.Balance[k];
        }
        prog_b3->Length[i + 1] = 43;
        for (k = 0; k < prog_b3->Length[i + 1]; k++) {
            prog_b3->FileContent[i + 1][k] = icc_pib.icc0012[k];
        }
        prog_b3->Length[i + 2] = 43;
        for (k = 0; k < prog_b3->Length[i + 2]; k++) {
            prog_b3->FileContent[i + 2][k] = icc_pib.icc0015[k];
        }
        prog_b3->Length[i + 3] = 43;
        for (k = 0; k < prog_b3->Length[i + 3]; k++) {
            prog_b3->FileContent[i + 3][k] = icc_pib.icc0019[k];
        }
    }
    return SUCCESS;
}
//接收完B4 发送event_report_rq
int recv_b4_Ble_OC(PROG_COMM_B4 *prog_b4, int time_out)
{
    int ret = SUCCESS;
    ret = SetMMI_rs_OC();
    prog_b4->RSCTL = 0x13;
    prog_b4->FrameType = 0xb4;
    prog_b4->ErrorCode = 0;
    return ret;
}



int SetMMI_rs_OC()
{
    int ret = SUCCESS;
    uint16 pos = 0 ;
    uint8 setmmi_rs_len = 0, action_para;
    is_setmmi_recved = 1;
    ret = IsSetMMI_rs(g_com_rx_buf);
    if(ret != SUCCESS)
    {
        return -2 + ret*10;
    }
    pos = 3;	//»•µÙ÷°–Ú∫≈°¢√¸¡Ó∫≈0xe0°¢‘≠”Ô¿‡–Õ0x03
    setmmi_rs_len = g_com_rx_buf[pos++];
    pos += 4;
    pos++;
    pos++;
    pos++;
    pos++;
    action_para = g_com_rx_buf[pos++];
    pos++;
    if(Getbit(action_para,3))
    {
        pos++;
    }
    pos++;
    if(setmmi_rs_len != pos - 4)
    {
        return -3;
    }
    return SUCCESS;
}

int TransferChannel_rs_OC(int * DATALIST, char *Data, int time_out)
{
    int ret = SUCCESS;
    uint16 pos = 0,  data_len = 0;
    uint8 transfer_rs_len = 0, action_para;
    ret = IsTransfer_rs(g_com_rx_buf);
    if(ret != SUCCESS)
    {
        return -2 + ret*10;
    }
    pos = 3;
    transfer_rs_len = g_com_rx_buf[pos++];
    pos += 4;
    pos++;
    pos++;
    pos++;
    pos++;
    action_para = g_com_rx_buf[pos++];
    pos++;
    if(Getbit(action_para,3))
    {
        pos++;
        pos++;
        *DATALIST = g_com_rx_buf[pos++];
        data_len = transfer_rs_len + 4 - pos - 1;
        if(data_len < 128)
        {
            memcpy(Data,&g_com_rx_buf[pos],data_len);
        }
        pos += data_len;
    }
    pos++;
    return SUCCESS;
    return 1;
}

int INITIALISATION_rs_OC(char *obu_mac, char *sys_info, char *icc_info, char *rnd_info, char *reset_info, char *obu_status, int time_out)
{
    int len_icc_info = 0;
    int pos,i;
    uint8 vst_len = 0;
    uint8 mac_control = 0;
    uint8 dsrc_op = 0;
    uint8 container_ops = 0;
//    char InfoTmp[100];
//    memset(icc_info,0x00,sizeof(icc_info));
    memset(icc_info,0x00,strlen(icc_info));//hmh
    if(IsVst(g_com_rx_buf) != 0)
    {
        return -2;
    }
    pos = 3;
    vst_len = g_com_rx_buf[pos++];
   
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
        mac_control = g_com_rx_buf[pos++];
        pos++;
        pos++;	//
        pos++;	//
        pos++;	//
        pos++;	//
        pos++;	//
        for(i = 0; i < 8; i++)
        {
            g_contract_sn[i] = g_com_rx_buf[pos++];
        }
        pos += 4;
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
            
            if(Getbit(container_ops,4))
            {
                pos++;	//»›∆˜¿‡–Õ0x50
                for(i = 0; i < 15; i++)
                {
                    reset_info[i] = g_com_rx_buf[pos++];
                }
                
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
