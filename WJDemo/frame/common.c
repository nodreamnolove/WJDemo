
#include "lib2rsu.h"
#include "common.h"
#include <stdio.h>
//*****************************RSU ÷≥÷…Ë±∏»´æ÷±‰¡ø∂®“Â*****************************//

#define MAXFILESIZE (50*1024)


//OBU»´æ÷±‰¡ø
char gtf_veh_file[128];//≥µ¡æ–≈œ¢buf£¨Ω“Ê¿œ∞ÊOBU–Ë“™Õ®π˝–¥≥µ¡æ–≈œ¢ÕÍ≥…º§ªÓ£¨œ»Ω´±Í«©÷–≥µ¡æ–≈œ¢∂¡≥ˆ£¨‘Ÿ–¥»Î°£2013-8-20 by gaotf
uint8 g_OBU_ContractVersion;	//	OBU∫œÕ¨∞Ê±æ
uint8 g_psam_slot;				//	psamø®≤€
uint8 g_bst_type;
int g_rsctl = 0;
uint8 g_master_card = 3;
uint8 g_issue_card = 1;
uint8 g_trade_card = 2;
uint8 run_mode;	//‘À––ƒ£ Ω£¨”…c0¥´Ω¯¿¥
uint8 g_psam_disperse = 1;	//”…C3¥´Ω¯¿¥£¨PSAM∑÷…¢≤Œ ˝ 1£∫2601  2£∫4601
uint8 g_ObuInitMode = 0;

char g_obu_mac[4];
//¥Æø⁄
int g_serial_fd = -5;		//¥Æø⁄Œƒº˛√Ë ˆ∑˚
uint32 g_com_rx_len = 0;	//¥Æø⁄Ω” ’ ˝æ›≥§∂»
uint32 g_com_needrx_len = 0;
uint32 g_com_tx_len = 0;	//¥Æø⁄∑¢ÀÕ ˝æ›≥§∂»
uint8 g_com_rx_buf[MAXCOMDATALENGHT];	//¥Æø⁄Ω” ’ ˝æ›
uint8 g_com_tx_buf[MAXCOMDATALENGHT];	//¥Æø⁄∑¢ÀÕ ˝æ›
uint32 g_com_len = 0;
uint32 g_com_Framelen = 0;
uint8 g_com_buf[MAXCOMDATALENGHT];
uint16 g_udpnet_start_flag;
uint8 g_udpnet_frame_flag = 0;//Õ¯ø⁄◊È∞¸±Í÷æ
uint8 g_frame_flag = 0;		//¥Æø⁄◊È∞¸±Í÷æ
uint16 g_start_flag;		//≈–∂œ∞¸Õ∑À˘”√

uint32 g_com_tx_len_bst = 0;	//¥Æø⁄∑¢ÀÕ ˝æ›≥§∂»(‘⁄œﬂƒ£ Ωº«¬ºBST)
uint8 g_com_tx_buf_bst[MAXCOMDATALENGHT];	//¥Æø⁄Ω” ’ ˝æ›(‘⁄œﬂƒ£ Ωº«¬ºBST)
//UdpÕ¯ø⁄ add by wll
int g_udpnet_fd = -5;		//UdpÕ¯ø⁄Œƒº˛√Ë ˆ∑˚
uint32 g_udpnet_rx_len = 0;	//UdpÕ¯ø⁄Ω” ’ ˝æ›≥§∂»
uint32 g_udpnet_tx_len = 0;	//UdpÕ¯ø⁄∑¢ÀÕ ˝æ›≥§∂»
uint8 g_udpnet_rx_buf[MAXUDPDATALENGHT];	//UdpÕ¯ø⁄Ω” ’ ˝æ›
uint8 g_udpnet_tx_buf[MAXUDPDATALENGHT];	//UdpÕ¯ø⁄∑¢ÀÕ ˝æ›
uint8 g_udpnet_rx_buf_tmp[MAXUDPDATALENGHT];	//UdpÕ¯ø⁄Ω” ’ ˝æ›
uint8 g_udpnet_tx_buf_tmp[MAXUDPDATALENGHT];	//UdpÕ¯ø⁄∑¢ÀÕ ˝æ›
uint32 g_udpnet_len = 0;
uint8 g_udpnet_buf[MAXUDPDATALENGHT];
uint8 g_IsRecvUdpNetData = -1;//ºÏ≤ÈUDP «∑Ò ’µΩ ˝æ› add by wll 2013-4-20 10:17:59
//0£∫Œ¥ ’µΩ
//1£∫ ’µΩ ˝æ›£¨≤ª±£÷§ ˝æ› «∑Ò’˝»∑
uint8 g_IsRecvUdpNetData_tmp = -1;

FRAME_SYSINFO_CHECK_RQ g_frame_syscheck_rq;
FRAME_UPLOAD_TRADEINFO_RQ g_frame_uploadtradeinfo_rq;
FRAME_READ_VEHINFO_RQ g_frame_readveh_rq;
FRAME_QUERY_BACKSTAGEINFO_RQ g_frame_querybackInfo_rq;
FRAME_WRITE_VEHINFO_RQ g_frame_writeveh_rq;
FRAME_WRITE_SYSINFO_RQ g_frame_writesys_rq;


sem_t g_sem_udpnet_init_00;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-ÃÙ’Ω÷°
sem_t g_sem_udpnet_init_02;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-»œ÷§”¶¥÷°
sem_t g_sem_udpnet_init_14;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-–¥≥µ¡æ–≈œ¢”¶¥÷°
sem_t g_sem_udpnet_init_16;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-∂¡≥µ¡æ–≈œ¢”¶¥÷°
sem_t g_sem_udpnet_init_18;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-–¥œµÕ≥–≈œ¢”¶¥÷°
sem_t g_sem_udpnet_init_1A;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-œµÕ≥–≈œ¢∂‘±»”¶¥÷°
sem_t g_sem_udpnet_init_1C;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-∫ÛÃ®–≈œ¢≤È—Ø
sem_t g_sem_udpnet_init_1E;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-…œ¥´Ωª“◊–≈œ¢”¶¥÷°
sem_t g_sem_udpnet_init_20;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-◊¢œ˙»œ÷§”¶¥÷°
sem_t g_sem_udpnet_init_22;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-»¶¥Ê≥ı ºªØ”¶¥÷°
sem_t g_sem_udpnet_init_24;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-»¶¥Ê”¶¥÷°
sem_t g_sem_udpnet_init_26;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-≥‰÷µ”¶¥÷°
sem_t g_sem_udpnet_init_28;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-∫ÛÃ®’À∫≈µ«¬Ω”¶¥÷°
sem_t g_sem_udpnet_init_2A;    //udpΩ” ’ ˝æ›–≈∫≈¡ø-ºÏ∂®∑¢––∫œ∑®–‘”¶¥÷°

uint8 g_IscanSend = 2;// «∑Ò–Ë“™µ»¥˝∑¢ÀÕ¥Æø⁄ ˝æ›£¨0£∫µ»¥˝£ª1£∫÷±Ω”∑¢ÀÕ; 2:∂®“Â≥ı º÷µ


sem_t g_sem_rsuinit;
sem_t g_sem_vst;			//vst–≈∫≈¡ø
sem_t g_sem_getsecure;		//getsecure–≈∫≈¡ø
sem_t g_sem_transfer;		//transfer–≈∫≈¡ø
sem_t g_sem_setmmi;			//setmmi÷°–≈∫≈¡ø
sem_t g_sem_psamchannel;
sem_t g_sem_psamreset;
sem_t g_sem_iccreset;
sem_t g_sem_enterdir;
sem_t g_sem_readfile;
sem_t g_sem_prwrq;
sem_t g_sem_quickack;
sem_t g_sem_setsecure;
sem_t g_sem_str715ver;
//œ¬¡–±£÷§÷ª ’µΩ“ª¥Œ
int	is_udpnetinit_recved = 1;
int	is_rsuinit_recved = 1;
int is_vst_recved = 1;
int	is_getsecure_recved = 1;
int is_transfer_recved = 1;
int	is_setmmi_recved = 1;
int	is_psamchannel_recved = 1;
int is_psamreset_recved = 1;
int	is_iccreset_recved = 1;
int	is_enterdir_recved = 1;
int is_readfile_recved = 1;
int	is_prwrq_recved = 1;
int is_quickack_recved = 1;
int is_setsecure_recved = 1;
int is_str715ver_recved = 1;
/********************** ¡™ª˙ƒ£ Ωœ¬œ‡πÿΩ”ø⁄ ********************/
int	online_sign;	// «∑Ò¡™ª˙≤Ÿ◊˜
int is_net_recved;
int is_udpnet_recved;//add by wll 2013-3-22 13:54:48
int is_serial_recved;
int is_usbcom_recved;
sem_t g_sem_serialrecv;	//¥Æø⁄ ’µΩ ˝
sem_t g_sem_udpnetrecv;	//Õ¯ø⁄ ’µΩ ˝ add by wll 2013-3-22 13:53:53
//µ±«∞obu ˝æ›–≈œ¢
ST_VST vst;
ST_BST bst;
ICC_PIB icc_pib;
PSAM_PIB psam_pib;
PrWRq_PIB prwrq_pib;
FILE_PIB file_pib;
uint8 esam_random[8];			//esam÷–µƒÀÊª˙ ˝
uint8 EsamDv[8];				//÷˜øÿ√‹‘øÀ˘”√µƒ∑÷…¢“Ú◊”
uint8 EsamSn[8];				//–æ∆¨–Ú¡–∫≈,œµÕ≥÷˜øÿ√‹‘ø∫Õ”¶”√÷˜øÿ√‹‘øÀ˘”√µƒ∑÷…¢“Ú◊”
uint8 g_contract_sn[8];			//∫œÕ¨–Ú¡–∫≈,”¶”√À˘”–√‹‘øÀ˘”√µƒ∑÷…¢“Ú◊”
uint8 psam_encrypted_key[28];	//¥˝ÃÊªªµƒ√‹‘ø
uint8 esam_mac[4];				//–¥Œƒº˛ ±–Ë”√psamÀ„µƒ∞≤»´±®Œƒmac
uint8 cur_key[16];				//Õ®π˝psamµº≥ˆµƒ∏˜¿‡√‹‘ø
uint8 g_veh_file_sign;			//0-√˜Œƒ,1-√‹Œƒ
//esam–≈œ¢∂¡»°À˘”√µΩµƒ±‰¡ø
ST_READ_FILE g_read_file;
uint8 esam_read_sysinfo[128];	//œµÕ≥–≈œ¢
uint8 esam_read_vehicle[128];	//≥µ¡æ–≈œ¢
uint8 esam_read_info[128];
uint8 psam_read_info[128];
uint8 psam_mac[8];				//psamÀ„µƒº¯±¬Î£¨getsecure_rq÷–À˘”√
uint8 psam_random[8];			//psam÷–ªÒµ√µƒÀÊª˙ ˝
uint8 contract_version;			//±±æ©∏˘æ›≤ªÕ¨µƒ∞Ê±æ∫≈£¨«¯∑÷”≤samªπ «»Ìsam
uint8 g_esam_type = 1;  		//ƒ¨»œŒ™”≤ESAM
uint8 once_sign = 0;

char InfoTmp[100];				//¥Ú”°–≈œ¢ ˝◊È




FILE *udpnetfp = NULL;
FILE *comfp = NULL;
FILE *netfp = NULL;
FILE *usbcomfp = NULL;

char filename_udpnet[100] = {0};
char filename_com[100] = {0};
char filename_net[100] = {0};
char filename_usbcom[100] = {0};


PKG_DATA  g_pkg_data;
PKG_ICCINFO_DATA g_pkg_iccinfo_data;
//PROG_COMM_C4   g_c4_data;
//PROG_COMM_B3   g_b3_data;
//add by wll 2014-12-11
//JNIEnv *g_pobjenv;
/*
 * ∫Ø ˝√˚≥∆£∫	CheckSafe()
 * π¶ƒ‹√Ë ˆ£∫	ºÏ≤È ÷≥÷…Ë±∏…œ∆Ù∂Øµƒ”ÎETCœ‡πÿµƒΩ¯≥Ã ˝
 * ≤Œ ˝¡–±Ì£∫
 *			num		--	 ‰≥ˆ≤Œ ˝£¨ ÷≥÷…Ë±∏…œ∆Ù∂Øµƒ”ÎETCœ‡πÿµƒΩ¯≥Ã ˝
 * ∑µªÿΩ·π˚£∫
 *			ERR_NO	--	÷¥––≥…π¶
 *			ERR_OPEN_FILE	--	¥Úø™Œƒº˛ ß∞‹
 */

uint8 CheckSafe(uint8 *num)
{
    FILE *fp;
    uint8 i = 0, count = 0;
    char buf[512] = {0};
    char name[5][20] = {"obu_issue", "read_iccard", "issue_test", "trade_test", "phy_test"};
    
    system("ps > /opt/Qtopia/bin/process_list");    //÷¥––shell√¸¡Ó£¨µ√µΩΩ¯≥Ã¡–±Ì
    if((fp = fopen("/opt/Qtopia/bin/process_list","rb")) == NULL)
    {
        //DEBUGP("cannot open file \n");
        return ERR_OPEN_FILE;
    }
    while(!feof(fp))
    {
        fgets(buf, 256, fp);	//∂¡»°“ª––ƒ⁄»›
        for(i = 0; i < 5; i++)
        {
            if(strstr(buf, name[i]) != NULL)	//ºÏ≤È◊÷∑˚¥Æ
            {
                count++;
            }
        }
    }
    *num = count;
    fclose(fp);
    return SUCCESS;
}

/*
 * ∫Ø ˝√˚≥∆£∫	pkt_code()
 * π¶ƒ‹√Ë ˆ£∫	 ˝æ›◊™“Â
 * ≤Œ ˝¡–±Ì£∫
 *			src_buf			--	◊™“Â«∞ ˝æ›
 *			target_buf		--	◊™“Â∫Û ˝æ›
 *			len				--	◊™“Â«∞ ˝æ›≥§∂»
 * ∑µªÿΩ·π˚£∫
 *			◊™“Â∫Û ˝æ›≥§∂»
 */
uint32 pkt_code(uint8 *src_buf, uint8 *target_buf, uint32 len)
{
    uint8 i = 0, j = 0;
    //÷°Õ∑
    target_buf[i++]=src_buf[0];
    target_buf[i++]=src_buf[1];
    // ˝æ›
    for (j=2; j<len-1; j++)
    {
        if (src_buf[j]==0xFF)
        {
            target_buf[i++]=0xFE;
            target_buf[i++]=0x01;
        }
        else if (src_buf[j]==0xFE)
        {
            target_buf[i++]=0xFE;
            target_buf[i++]=0x00;
        }
        else
        {
            target_buf[i++]=src_buf[j];
        }
    }
    
    target_buf[i++]=0xFF;
    return i;
}
/*
 * ∫Ø ˝√˚≥∆£∫	pkt_code_udpnet()
 * π¶ƒ‹√Ë ˆ£∫	 ˝æ›◊™“Â
 * ≤Œ ˝¡–±Ì£∫
 *			src_buf			--	◊™“Â«∞ ˝æ›
 *			target_buf		--	◊™“Â∫Û ˝æ›
 *			len				--	◊™“Â«∞ ˝æ›≥§∂»
 * ∑µªÿΩ·π˚£∫
 *			◊™“Â∫Û ˝æ›≥§∂»
 */
uint32 pkt_code_udpnet(uint8 *src_buf, uint8 *target_buf,uint32 len)
{
    uint8 i = 0, j = 0;
    //÷°Õ∑
    // 	target_buf[i++]=src_buf[0];
    // 	target_buf[i++]=src_buf[1];
    // ˝æ›
    for (j=0; j<len; j++)
    {
        if (src_buf[j]==0xFF&&0)
        {
            target_buf[i++]=0xFE;
            target_buf[i++]=0x01;
        }
        else if (src_buf[j]==0xFE&&0)
        {
            target_buf[i++]=0xFE;
            target_buf[i++]=0x00;
        }
        else
        {
            target_buf[i++]=src_buf[j];
        }
    }
    //÷°Œ≤
    /*	target_buf[i++]=0xFF;*/
    return i;
}

/*
 * ∫Ø ˝√˚≥∆£∫	pkt_decode()
 * π¶ƒ‹√Ë ˆ£∫	Ω¯–– ˝æ›∞¸µƒΩ‚¬Îπ§◊˜
 * ≤Œ ˝¡–±Ì£∫
 *			buf			--	Ω” ’µΩµƒ ˝æ›
 *			len			--	 ˝æ›≥§∂»
 *			port		--  ∂Àø⁄¿‡–Õ£¨¥Æø⁄ªÚÕ¯ø⁄
 * ∑µªÿΩ·π˚£∫
 *			ERR_NO		--	 ˝æ›–£—È≥…π¶
 *			ERR_CHECK_WRONG	--	 ˝æ›–£—È ß∞‹
 */
uint8 pkt_decode(uint8 *buf, uint32 len, uint8 port)
{
    uint32 i = 0, j=0;
    uint8 chk = 0;
    //»•µÙÕ∑
    if(buf[0] == 0xff)
    {
        i++;
    }
    if(buf[1] == 0xff)
    {
        i++;
    }
    do
    {
        if(buf[i] == 0xff)
        {
            
            break;	// «∑ÒµΩŒ≤
        }
        else
        {
            buf[j] = buf[i];
            if(buf[i] == 0xfe)
            {
                buf[j] |= buf[i + 1];
                i++;
            }
            chk ^= buf[j];
            if(i == 2){
                
            }
            
            i++;
            j++;
        }
    }while (1);
    // –£—È’˝»∑
    if((j > 1) && (chk == 0))
    {
        len = j - 1;	//»•µÙ–£—ÈŒª
        if(port == COM)
        {
            for(i = 0; i < len; i++)
            {
                g_com_rx_buf[i] = buf[i];
            }
            g_com_rx_len = len;
        }
        else if( port == UDPNET )
        {
            for( i=0; i<len; i++)
                g_udpnet_rx_buf[i] = buf[i];
            g_udpnet_rx_len = len;
        }
#if 0
        else if( port == NET )
        {
            for( i=0; i<len; i++)
                g_net_rx_buf[i] = buf[i];
            g_net_rx_len = len;
        }else if( port == USBCOM )
        {
            for( i=0; i<len; i++)
                g_usbcom_rx_buf[i] = buf[i];
            g_usbcom_rx_len = len;
        }
#endif
        
        return ERR_NO;
    }
    else
    {
        return ERR_CHECK_WRONG;	//≥ˆ¥Ì
    }
    
}
/*
 * ∫Ø ˝√˚≥∆£∫	pkt_decode()
 * π¶ƒ‹√Ë ˆ£∫	Ω¯–– ˝æ›∞¸µƒΩ‚¬Îπ§◊˜
 * ≤Œ ˝¡–±Ì£∫
 *			buf			--	Ω” ’µΩµƒ ˝æ›
 *			len			--	 ˝æ›≥§∂»
 *			port		--  ∂Àø⁄¿‡–Õ£¨¥Æø⁄ªÚÕ¯ø⁄
 * ∑µªÿΩ·π˚£∫
 *			ERR_NO		--	 ˝æ›–£—È≥…π¶
 *			ERR_CHECK_WRONG	--	 ˝æ›–£—È ß∞‹
 */
uint8 pkt_decode_udp(uint8 *buf, uint32 len, uint8 port)
{
    uint32 i = 0, j=0;
    
    // 	if(port == COM)
    // 	{
    // 		for(i = 0; i < len; i++)
    // 		{
    // 			g_com_rx_buf[i] = buf[i];
    // 		}
    // 		g_com_rx_len = len;
    // 	}
    // 	else
    if( port == UDPNET )
    {
        for( i=0; i<len; i++)
            g_udpnet_rx_buf[i] = buf[i];
        g_udpnet_rx_len = len;
    }
    return ERR_NO;
}

int aaaaaaaaaaaa = 1;
void print_info(int dev_type,int transmit_type)
{
    int i;
    int filesize = 0;
    uint8 timebuf[10];
    char filename[100];
    char timetmp[100];
    // 	if(access("/sdcard/WJ_LOG_UDPNET",0)==-1)//access∫Ø ˝ «≤Èø¥Œƒº˛º– «≤ª «¥Ê‘⁄
    // 	{
    // 		system("mkdir /sdcard/WJ_LOG_UDPNET");//Œƒº˛º–≤ª¥Ê‘⁄‘Ú¥¥Ω®
    // 		if(access("/sdcard/WJ_LOG_UDPNET",0)==-1)////ºÏ≤ÈŒƒº˛º– «∑Ò¥¥Ω®≥…π¶£¨»Áπ˚ªπ≤ª¥Ê‘⁄‘ÚÕÀ≥ˆ
    // 		{
    // 			return ;
    // 		}
    // 	}
    //
    // 	if(access("/sdcard/WJ_LOG_COM",0)==-1)//access∫Ø ˝ «≤Èø¥Œƒº˛º– «≤ª «¥Ê‘⁄
    // 	{
    // 		system("mkdir /sdcard/WJ_LOG_COM");//Œƒº˛º–≤ª¥Ê‘⁄‘Ú¥¥Ω®
    // 		if(access("/sdcard/WJ_LOG_COM",0)==-1)//ºÏ≤ÈŒƒº˛º– «∑Ò¥¥Ω®≥…π¶£¨»Áπ˚ªπ≤ª¥Ê‘⁄‘ÚÕÀ≥ˆ
    // 		{
    // 			return ;
    // 		}
    // 	}
    if(run_mode == 0)
    {
        switch(dev_type)
        {
            case COM:
            {
                //≈–∂œsdø®…œµƒWJ_LOG_COMŒƒº˛º– «∑Ò¥Ê‘⁄£¨≤ª¥Ê‘⁄‘Ú¥¥Ω®
                if(access("/storage/sdcard0/WJ_LOG_COM",0)==-1)//access∫Ø ˝ «≤Èø¥Œƒº˛º– «≤ª «¥Ê‘⁄
                {
                    system("mkdir /storage/sdcard0/WJ_LOG_COM");//Œƒº˛º–≤ª¥Ê‘⁄‘Ú¥¥Ω®
                    if(access("/storage/sdcard0/WJ_LOG_COM",0)==-1)//ºÏ≤ÈŒƒº˛º– «∑Ò¥¥Ω®≥…π¶£¨»Áπ˚ªπ≤ª¥Ê‘⁄‘ÚÕÀ≥ˆ
                    {
                        return ;
                    }
                }
                //≈–∂œŒƒº˛ «∑Ò¥Ê‘⁄
                if(comfp == NULL && filename_com[0]==0)
                {
                    GetTimebufFunction(timebuf);
                    sprintf(timetmp,"/storage/sdcard0/WJ_LOG_COM/%04d_%02d_%02d_%02d_%02d_%02d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                    //fprintf(udpnetfp,"%s",timetmp);
                    strcpy(filename_com,timetmp);
                }
                else
                {
                    filesize = getFileSize(filename_com);
                    if(filesize > MAXFILESIZE)
                    {
                        GetTimebufFunction(timebuf);
                        sprintf(timetmp,"/storage/sdcard0/WJ_LOG_COM/%04d_%02d_%02d_%02d_%02d_%02d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                        //fprintf(udpnetfp,"%s",timetmp);
                        strcpy(filename_com,timetmp);
                    }
                }
                comfp=fopen(filename_com , "a+");
                if ( comfp == NULL )
                {
                    printf("file not exist!");
                    return ;
                }
                GetTimebufFunction(timebuf);
                sprintf(timetmp,"%04d-%02d-%02d %02d:%02d:%02d",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                fprintf(comfp,"%s",timetmp);
                if(transmit_type == RECV)
                {
                    fprintf(comfp,"\nserial: receive(len=%d): [ ",g_com_rx_len);
                    for (i=0; i<g_com_rx_len; i++)
                    {
                        fprintf(comfp,"%2.2X ", g_com_rx_buf[i]);
                    }
                    fprintf(comfp,"]\n");
                }
                if(transmit_type == SEND)
                {
                    fprintf(comfp,"\nserial: send(len=%d): [ ",g_com_tx_len);
                    for (i=0; i<g_com_tx_len; i++)
                    {
                        fprintf(comfp,"%2.2X ", g_com_tx_buf[i]);
                    }
                    fprintf(comfp,"]\n");
                    
                }
                fclose(comfp);
            }/* end case COM: */
                break;
            case UDPNET:
            {
                //≈–∂œsdø®…œµƒWJ_LOG_UDPNETŒƒº˛º– «∑Ò¥Ê‘⁄£¨≤ª¥Ê‘⁄‘Ú¥¥Ω®
                if(access("/storage/sdcard0/WJ_LOG_UDPNET",0)==-1)//access∫Ø ˝ «≤Èø¥Œƒº˛º– «≤ª «¥Ê‘⁄
                {
                    system("mkdir /storage/sdcard0/WJ_LOG_UDPNET");//Œƒº˛º–≤ª¥Ê‘⁄‘Ú¥¥Ω®
                    if(access("/storage/sdcard0/WJ_LOG_UDPNET",0)==-1)////ºÏ≤ÈŒƒº˛º– «∑Ò¥¥Ω®≥…π¶£¨»Áπ˚ªπ≤ª¥Ê‘⁄‘ÚÕÀ≥ˆ
                    {
                        return ;
                    }
                }
                if(udpnetfp == NULL && filename_udpnet[0]==0)
                {
                    GetTimebufFunction(timebuf);
                    sprintf(timetmp,"/storage/sdcard0/WJ_LOG_UDPNET/%04d_%02d_%02d_%02d_%02d_%02d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                    //fprintf(udpnetfp,"%s",timetmp);
                    strcpy(filename_udpnet,timetmp);
                }
                else
                {
                    filesize = getFileSize(filename_udpnet);
                    //					filesize = 100;
                    if(filesize > MAXFILESIZE)
                    {
                        GetTimebufFunction(timebuf);
                        sprintf(timetmp,"/storage/sdcard0/WJ_LOG_UDPNET/%04d_%02d_%02d_%02d_%02d_%02d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                        //fprintf(udpnetfp,"%s",timetmp);
                        strcpy(filename_udpnet,timetmp);
                    }
                }
                udpnetfp=fopen(filename_udpnet, "a+");
                if ( udpnetfp == NULL )
                {
                    printf("file not exist!");
                    return ;
                }
                GetTimebufFunction(timebuf);
                sprintf(timetmp,"%04d-%02d-%02d %02d:%02d:%02d",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                fprintf(udpnetfp,"%s",timetmp);
                if(transmit_type == RECV)
                {
                    fprintf(udpnetfp,"\nudpnet: receive(len=%d): [ ",g_udpnet_rx_len);
                    for (i=0; i<g_udpnet_rx_len; i++)
                    {
                        fprintf(udpnetfp,"%2.2X ", g_udpnet_rx_buf[i]);
                    }
                    fprintf(udpnetfp,"]\n\n");
                }
                if(transmit_type == SEND)
                {
                    fprintf(udpnetfp,"\nudpnet: send(len=%d): [ ",g_udpnet_tx_len);
                    for (i=0; i<g_udpnet_tx_len; i++)
                    {
                        fprintf(udpnetfp,"%2.2X ", g_udpnet_tx_buf[i]);
                    }
                    fprintf(udpnetfp,"]\n");
                    
                }
                
                fclose(udpnetfp);
            }/* end case COM: */
            default:
            {
                /* do nothing */
            }
                break;
        }/* end switch(dev_type) */
    }/* end if(run_mode == 0) */
#if 0 //∆¡±Œ by wll 2015-4-7 11:08:39
    else if(run_mode == 4)
    {
        switch(dev_type)
        {
            case UDPNET:
            {
                //≈–∂œsdø®…œµƒWJ_LOG_UDPNETŒƒº˛º– «∑Ò¥Ê‘⁄£¨≤ª¥Ê‘⁄‘Ú¥¥Ω®
                if(access("/storage/sdcard0/WJ_LOG_UDPNET",0)==-1)//access∫Ø ˝ «≤Èø¥Œƒº˛º– «≤ª «¥Ê‘⁄
                {
                    system("mkdir /storage/sdcard0/WJ_LOG_UDPNET");//Œƒº˛º–≤ª¥Ê‘⁄‘Ú¥¥Ω®
                    if(access("/storage/sdcard0/WJ_LOG_UDPNET",0)==-1)////ºÏ≤ÈŒƒº˛º– «∑Ò¥¥Ω®≥…π¶£¨»Áπ˚ªπ≤ª¥Ê‘⁄‘ÚÕÀ≥ˆ
                    {
                        return ;
                    }
                }
                if(udpnetfp == NULL && filename_udpnet[0]==0)
                {
                    GetTimebufFunction(timebuf);
                    sprintf(timetmp,"/storage/sdcard0/WJ_LOG_UDPNET/%04d_%02d_%02d_%02d_%02d_%02d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                    strcpy(filename_udpnet,timetmp);
                }
                else
                {
                    filesize = getFileSize(filename_udpnet);
                    if(filesize > MAXFILESIZE)
                    {
                        GetTimebufFunction(timebuf);
                        sprintf(timetmp,"/storage/sdcard0/WJ_LOG_UDPNET/%04d_%02d_%02d_%02d_%02d_%02d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                        strcpy(filename_udpnet,timetmp);
                    }
                }
                udpnetfp=fopen(filename_udpnet, "a+");
                if ( udpnetfp == NULL )
                {
                    printf("file not exist!");
                    return ;
                }
                GetTimebufFunction(timebuf);
                sprintf(timetmp,"%04d-%02d-%02d %02d:%02d:%02d",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                fprintf(udpnetfp,"%s",timetmp);
                if(transmit_type == RECV)
                {
                    if((g_udpnet_rx_buf[0] == 0x19)&& (g_udpnet_rx_buf[1] == 0x50))
                    {
                        fprintf(udpnetfp,"udpnet: receive(len=%d): [ ",g_udpnet_rx_len);
                        for (i=0; i<g_udpnet_rx_len; i++)
                        {
                            fprintf(udpnetfp,"%2.2X ", g_udpnet_rx_buf[i]);
                        }
                        fprintf(udpnetfp,"]\n\n");
                    }
                }
                if(transmit_type == SEND)
                {
                    if((g_udpnet_tx_buf[0] == 0x1A)&& (g_udpnet_tx_buf[1] == 0x30))
                    {
                        fprintf(udpnetfp,"\nudpnet: send(len=%d): [ ",g_udpnet_tx_len);
                        for (i=0; i<g_udpnet_tx_len; i++)
                        {
                            fprintf(udpnetfp,"%2.2X ", g_udpnet_tx_buf[i]);
                        }
                        fprintf(udpnetfp,"]\n");
                    }
                }
                
                fclose(udpnetfp);
            }/* end case COM: */
            default:
            {
                /* do nothing */
            }
                break;
        }/* end switch(dev_type) */
    }/* end if(run_mode == 10) */
    else if(run_mode == 5)
    {
        //≈–∂œsdø®…œµƒWJ_LOG_UDPNETŒƒº˛º– «∑Ò¥Ê‘⁄£¨≤ª¥Ê‘⁄‘Ú¥¥Ω®
        if(access("/storage/sdcard0/WJ_LOG_UDPNET",0)==-1)//access∫Ø ˝ «≤Èø¥Œƒº˛º– «≤ª «¥Ê‘⁄
        {
            system("mkdir /storage/sdcard0/WJ_LOG_UDPNET");//Œƒº˛º–≤ª¥Ê‘⁄‘Ú¥¥Ω®
            if(access("/storage/sdcard0/WJ_LOG_UDPNET",0)==-1)////ºÏ≤ÈŒƒº˛º– «∑Ò¥¥Ω®≥…π¶£¨»Áπ˚ªπ≤ª¥Ê‘⁄‘ÚÕÀ≥ˆ
            {
                return ;
            }
        }
    }
    else if(run_mode == 6)
    {
        //≈–∂œsdø®…œµƒWJ_LOG_UDPNETŒƒº˛º– «∑Ò¥Ê‘⁄£¨≤ª¥Ê‘⁄‘Ú¥¥Ω®
        if(access("/storage/sdcard0/WJ_LOG_UDPNET",0)==-1)//access∫Ø ˝ «≤Èø¥Œƒº˛º– «≤ª «¥Ê‘⁄
        {
            system("mkdir /storage/sdcard0/WJ_LOG_UDPNET");//Œƒº˛º–≤ª¥Ê‘⁄‘Ú¥¥Ω®
            if(access("/storage/sdcard0/WJ_LOG_UDPNET",0)==-1)////ºÏ≤ÈŒƒº˛º– «∑Ò¥¥Ω®≥…π¶£¨»Áπ˚ªπ≤ª¥Ê‘⁄‘ÚÕÀ≥ˆ
            {
                return ;
            }
        }
        if(udpnetfp == NULL && filename_udpnet[0]==0)
        {
            GetTimebufFunction(timebuf);
            sprintf(timetmp,"/storage/sdcard0/WJ_LOG_UDPNET/%04d_%02d_%02d_%02d_%02d_%02d_%d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7],aaaaaaaaaaaa);
            //fprintf(udpnetfp,"%s",timetmp);
            strcpy(filename_udpnet,timetmp);
            aaaaaaaaaaaa ++;
        }
        else
        {
            //			filesize = getFileSize(filename_udpnet);
            filesize = 100;
            if(filesize > 99)
            {
                GetTimebufFunction(timebuf);
                sprintf(timetmp,"/storage/sdcard0/WJ_LOG_UDPNET/%04d_%02d_%02d_%02d_%02d_%02d_%d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7],aaaaaaaaaaaa);
                //fprintf(udpnetfp,"%s",timetmp);
                strcpy(filename_udpnet,timetmp);
                aaaaaaaaaaaa ++;
            }
        }
    }
    else if(run_mode == 7)
    {
        //≈–∂œsdø®…œµƒWJ_LOG_COMŒƒº˛º– «∑Ò¥Ê‘⁄£¨≤ª¥Ê‘⁄‘Ú¥¥Ω®
        if(access("/storage/sdcard0/WJ_LOG_COM",0)==-1)//access∫Ø ˝ «≤Èø¥Œƒº˛º– «≤ª «¥Ê‘⁄
        {
            system("mkdir /storage/sdcard0/WJ_LOG_COM");//Œƒº˛º–≤ª¥Ê‘⁄‘Ú¥¥Ω®
            if(access("/storage/sdcard0/WJ_LOG_COM",0)==-1)////ºÏ≤ÈŒƒº˛º– «∑Ò¥¥Ω®≥…π¶£¨»Áπ˚ªπ≤ª¥Ê‘⁄‘ÚÕÀ≥ˆ
            {
                return ;
            }
        }
        if(comfp == NULL && filename_com[0]==0)
        {
            GetTimebufFunction(timebuf);
            sprintf(timetmp,"/storage/sdcard0/WJ_LOG_COM/%04d_%02d_%02d_%02d_%02d_%02d_%d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7],aaaaaaaaaaaa);
            //fprintf(udpnetfp,"%s",timetmp);
            strcpy(filename_com ,timetmp);
        }
        else
        {
            filesize = getFileSize(filename_com );
            if(filesize == -1)
                return;
            //			filesize = 100;
            if(filesize > 99000)
            {
                GetTimebufFunction(timebuf);
                sprintf(timetmp,"/storage/sdcard0/WJ_LOG_COM/%04d_%02d_%02d_%02d_%02d_%02d_%d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7],aaaaaaaaaaaa);
                //				fprintf(udpnetfp,"%s",timetmp);
                strcpy(filename_com ,timetmp);
                aaaaaaaaaaaa ++;
            }
        }
        comfp=fopen(filename_com , "a+");
        if ( comfp == NULL )
        {
            printf("file not exist!");
            return ;
        }
        GetTimebufFunction(timebuf);
        sprintf(timetmp,"%04d-%02d-%02d %02d:%02d:%02d",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
        fprintf(comfp,"%s",timetmp);
        if(transmit_type == RECV)
        {
            fprintf(comfp,"serial: receive(len=%d): [ ",g_com_rx_len);
            for (i=0; i<g_com_rx_len; i++)
            {
                fprintf(comfp,"%2.2X ", g_com_rx_buf[i]);
            }
            fprintf(comfp,"]\n");
        }
        if(transmit_type == SEND)
        {
            fprintf(comfp,"\nserial: send(len=%d): [ ",g_com_tx_len);
            for (i=0; i<g_com_tx_len; i++)
            {
                fprintf(comfp,"%2.2X ", g_com_tx_buf[i]);
            }
            fprintf(comfp,"]\n");
            
        }
        fclose(comfp);
    }
#endif
    else if(run_mode > 10)
    {
        usleep(100*run_mode);
    }
    
}
void print_info_Info(char Info[],int Len,int dev_type)
{
    int i;
    int filesize = 0;
    uint8 timebuf[10];
    char filename[100];
    char timetmp[100];
    if(run_mode == 0)
    {
        switch(dev_type)
        {
            case COM:
                //≈–∂œsdø®…œµƒWJ_LOG_COMŒƒº˛º– «∑Ò¥Ê‘⁄£¨≤ª¥Ê‘⁄‘Ú¥¥Ω®
                if(access("/sdcard/WJ_LOG_COM",0)==-1)//access∫Ø ˝ «≤Èø¥Œƒº˛º– «≤ª «¥Ê‘⁄
                {
                    system("mkdir /sdcard/WJ_LOG_COM");//Œƒº˛º–≤ª¥Ê‘⁄‘Ú¥¥Ω®
                    if(access("/sdcard/WJ_LOG_COM",0)==-1)//ºÏ≤ÈŒƒº˛º– «∑Ò¥¥Ω®≥…π¶£¨»Áπ˚ªπ≤ª¥Ê‘⁄‘ÚÕÀ≥ˆ
                    {
                        return ;
                    }
                }
                //≈–∂œŒƒº˛ «∑Ò¥Ê‘⁄
                if(comfp == NULL && filename_com[0]==0)
                {
                    GetTimebufFunction(timebuf);
                    sprintf(timetmp,"/sdcard/WJ_LOG_COM/%04d_%02d_%02d_%02d_%02d_%02d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                    //fprintf(udpnetfp,"%s",timetmp);
                    strcpy(filename_com,timetmp);
                }
                else
                {
                    filesize = getFileSize(filename_com);
                    if(filesize > MAXFILESIZE)
                    {
                        GetTimebufFunction(timebuf);
                        sprintf(timetmp,"/sdcard/WJ_LOG_COM/%04d_%02d_%02d_%02d_%02d_%02d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                        //fprintf(udpnetfp,"%s",timetmp);
                        strcpy(filename_com,timetmp);
                    }
                }
                comfp=fopen(filename_com , "a+");
                if ( comfp == NULL )
                {
                    printf("file not exist!");
                    return ;
                }
                GetTimebufFunction(timebuf);
                sprintf(timetmp,"\n%04d-%02d-%02d %02d:%02d:%02d",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                fprintf(comfp,"%s",timetmp);
                
                fprintf(comfp,"PRINT_INFO: (len=%d): [ ",Len);
                for (i=0; i<Len; i++)
                {
                    fprintf(comfp,"%c",Info[i]);
                }
                fprintf(comfp,"]\n");
                fclose(comfp);
                break;
            case UDPNET:
                //≈–∂œsdø®…œµƒWJ_LOG_UDPNETŒƒº˛º– «∑Ò¥Ê‘⁄£¨≤ª¥Ê‘⁄‘Ú¥¥Ω®
                if(access("/sdcard/WJ_LOG_UDPNET",0)==-1)//access∫Ø ˝ «≤Èø¥Œƒº˛º– «≤ª «¥Ê‘⁄
                {
                    system("mkdir /sdcard/WJ_LOG_UDPNET");//Œƒº˛º–≤ª¥Ê‘⁄‘Ú¥¥Ω®
                    if(access("/sdcard/WJ_LOG_UDPNET",0)==-1)//ºÏ≤ÈŒƒº˛º– «∑Ò¥¥Ω®≥…π¶£¨»Áπ˚ªπ≤ª¥Ê‘⁄‘ÚÕÀ≥ˆ
                    {
                        return ;
                    }
                }
                //≈–∂œŒƒº˛ «∑Ò¥Ê‘⁄
                if(udpnetfp == NULL && filename_udpnet[0]==0)
                {
                    GetTimebufFunction(timebuf);
                    sprintf(timetmp,"/sdcard/WJ_LOG_UDPNET/%04d_%02d_%02d_%02d_%02d_%02d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                    //fprintf(udpnetfp,"%s",timetmp);
                    strcpy(filename_udpnet,timetmp);
                }
                else
                {
                    filesize = getFileSize(filename_udpnet);
                    if(filesize > MAXFILESIZE)
                    {
                        GetTimebufFunction(timebuf);
                        sprintf(timetmp,"/sdcard/WJ_LOG_UDPNET/%04d_%02d_%02d_%02d_%02d_%02d.txt",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                        //fprintf(udpnetfp,"%s",timetmp);
                        strcpy(filename_udpnet,timetmp);
                    }
                }
                udpnetfp=fopen(filename_udpnet , "a+");
                if ( udpnetfp == NULL )
                {
                    printf("file not exist!");
                    return ;
                }
                GetTimebufFunction(timebuf);
                sprintf(timetmp,"\n%04d-%02d-%02d %02d:%02d:%02d",timebuf[0]+timebuf[1]*256,timebuf[2],timebuf[3],timebuf[5],timebuf[6],timebuf[7]);
                fprintf(udpnetfp,"%s",timetmp);
                
                fprintf(udpnetfp,"PRINT_INFO: (len=%d): [ ",Len);
                for (i=0; i<Len; i++)
                {
                    fprintf(udpnetfp,"%2.2X ",Info[i]);
                }
                fprintf(udpnetfp,"]\n");
                fclose(udpnetfp);
                break;
            default:
                break;
        }
        
    }/* end if(run_mode == 0) */
}

int DtoHex(int data)
{
    char temp[10];
    int result;
    memset(temp, 0, 10);
    sprintf(temp, "%d", data);
    sscanf(temp, "%x", &result);
    return result;
}

//int IsVst(uint8 *pkg)
//{
//	if(pkg[8] == 0xc0)
//	{
//		if(Getbit(pkg[11],7) && Getbit(pkg[11],6) && !Getbit(pkg[11],5) && Getbit(pkg[11],4))
//		{
//			return 0;
//		}
//		else
//		{
//			return 2;
//		}
//	}
//	else
//	{
//		return 1;
//	}
//}

int CreateFilePath1(char* filename)
{
    udpnetfp=fopen(filename , "a+");
    if ( udpnetfp == NULL )
    {
        printf("file not exist!");
        return -1;
    }
    else
    {
        fprintf(udpnetfp,"123456789");
        //fclose(udpnetfp);
        return 0;
    }
    
    return 0;
    //mkdir("//mnt//sdcard//weilinlin"); /*¥¥Ω®Œƒº˛º–*/
    //system("ys.exe"); /*‘À––ƒ≥∏ˆ≥Ã–Ú*/
    //getch();
}


/*
 * ∫Ø ˝√˚≥∆£∫	GetTimeStampFunction()
 * π¶ƒ‹√Ë ˆ£∫	º∆À„µ±«∞ ±º‰¥¡£¨≤¢∑µªÿ
 *  ‰»Î≤Œ ˝£∫	Œﬁ
 *  ‰≥ˆ≤Œ ˝£∫	TimeInfo--µ√µΩµƒ ±º‰¥¡ ˝◊È
 * ∑µªÿΩ·π˚£∫	0--÷¥––≥…π¶
 *				∑«0--÷¥–– ß∞‹
 */
int GetTimebufFunction(uint8 *TimeInfo)
{
    int tmp, pos = 0;
    time_t ltime;
    struct tm *newtime;
    
    time(&ltime);
    newtime = localtime(&ltime);	//◊™ªªŒ™struct tmΩ·ππµƒµ±µÿ ±º‰--CST ±º‰(∂´∞À«¯)
    
    tmp = newtime->tm_year + 1900;
    TimeInfo[pos++] = tmp&0xff;			//ƒÍ£¨µÕ8Œª
    TimeInfo[pos++] = (tmp>>8)&0xff;	//ƒÍ£¨∏ﬂ8Œª
    TimeInfo[pos++] = newtime->tm_mon + 1;//‘¬
    TimeInfo[pos++] = newtime->tm_mday;	//»’
    TimeInfo[pos++] = newtime->tm_wday; //÷‹
    TimeInfo[pos++] = newtime->tm_hour;	// ±
    TimeInfo[pos++] = newtime->tm_min;	//∑÷
    TimeInfo[pos++] = newtime->tm_sec;	//√Î
    return 0;
}

/*
 ∫Ø ˝√˚£∫getFileSize(char * strFileName)
 π¶ƒ‹£∫ªÒ»°÷∏∂®Œƒº˛µƒ¥Û–°
 ≤Œ ˝£∫
 strFileName (char *)£∫Œƒº˛√˚
 ∑µªÿ÷µ£∫
 size (int)£∫Œƒº˛¥Û–°
 */
int getFileSize(char strFileName[])
{
    int size = 0;
    FILE *fp = fopen(strFileName, "r");
    if(fp!= NULL)
        //    fseek(fp, 0L, SEEK_END);
        //    size = ftell(fp);
    {
        fclose(fp);
        return -1;
    }
    
    return size;
}

