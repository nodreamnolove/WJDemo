#ifndef _ICC_H
#define	_ICC_H


#define MAX_ICC_APDU_LEN						128


extern void iccInitFrame(ST_TRANSFER_CHANNEL * transfer_rq);
extern void iccReadFileFrame(ST_TRANSFER_CHANNEL *transfer_rq,uint8 fid,uint8 offset,uint8 length);
extern int iccPayInitFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 trans_mode, uint8 *pay_val);
extern int iccPayFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 *time);
extern int iccUpdateBufFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 *station, uint8 *record0019);
extern int iccReadMoneyFrame(ST_TRANSFER_CHANNEL *transfer_rq);
extern int iccCheck(uint8 *data,uint8 curlist);
extern int iccinit_for_loadFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 Addmoney[], uint8 zhongduanhao[], uint8 miyaotype, uint8 miyaoFlag);
extern int icc_enter_dir(ST_TRANSFER_CHANNEL *transfer_rq, uint16 did);
extern int icc_select_file(ST_TRANSFER_CHANNEL *transfer_rq, uint8 data[], uint16 datalen);
extern int icc_check_Pin(ST_TRANSFER_CHANNEL *transfer_rq, uint16 datalen, uint8 data[]);
extern int icc_loadFrame(ST_TRANSFER_CHANNEL *transfer_rq, uint8 dealtime[], uint8 mac2[]);
#endif

