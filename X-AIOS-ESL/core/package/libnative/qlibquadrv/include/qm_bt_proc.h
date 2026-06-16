/*
 * Copyright (c) 2022  quaming
 *
 */
#ifndef _BT_PROC_H_
#define _BT_PROC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

#include "qm_wifi_cfg.h"

#define BT_TYPE_FSC    0x00

#define QM_MAX_BT_NUM  1
#define QM_MAX_BTNAME_STR_LEN 100

#define AT_BUFFER_SIZE 1024

#define QM_BT_TYPE_INIT_MSG_ID         0x00

#define QM_BT_SCAN_REQ_MSG_ID          0x01
#define QM_BT_SCAN_IND_MSG_ID          0x02
#define QM_BT_SCAN_END_MSG_ID          0x10
#define QM_BT_PLIST_END_MSG_ID          0x11

#define QM_BT_PLSIT_REQ_MSG_ID         0x03
#define QM_BT_PLIST_IND_MSG_ID         0x04
#define QM_BT_PLSIT_CLEAR_REQ_MSG_ID   0x05
//qm_Fsc_BT_AT_PIN_Read
#define QM_BT_PIN_READ_REQ_MSG_ID      0x06
#define QM_BT_PIN_READ_IND_MSG_ID      0x07
#define QM_BT_PIN_SETTING_REQ_MSG_ID   0x08

#define QM_BT_PAIRREQ_REQ_MSG_ID       0x09
#define QM_BT_PAIRED_REQ_MSG_ID        0x0a
#define QM_BT_CFN_REQ_MSG_ID           0x0b
//AT+HFPCONN
#define QM_BT_HFPCONN_REQ_MSG_ID       0x0c
#define QM_BT_DISCANNECT_REQ_MSG_ID    0x0d
#define QM_BT_ONOFF_REQ_MSG_ID         0x0e
#define QM_BT_HFPCONN_SUCCESS_MSG_ID   0x0f

#if 0

#define MAILBOX_CAPACITY 20

typedef struct {
    void *messages[MAILBOX_CAPACITY];
    int front, rear, count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} Mailbox;

#endif

typedef struct Msg_Head{
	unsigned int msg_id;
	unsigned msg_len;
}Msg_Head;



/*
<< AT+SCAN=1
>> OK
+SCAN=1<FF>-32<FF>3<FF>B019C66209FA<FF>wt-iphone<FF>7A020C
+SCAN=2<FF>-74<FF>0<FF>DC0D30000053<FF>BW226<FF>040680
+SCAN=3<FF>-43<FF>0<FF>00158354F994<FF>LAPTOP-3L<FF>120104
+SCAN=E
+PAIR
RSSI (-127 ~ -1)
*/
typedef struct bt_list_result{
	char index;
    signed int  RSSI;
    signed int  devAddressType;//(0) BR/EDR address (1) LE public address (2) LE random address (3) IOS device with Carplay support
	char macAddress[13];//MAC地址
	char btName[QM_MAX_BTNAME_STR_LEN];
	char Devtype[7];
}bt_list_result;


typedef struct bt_list_msg{
	Msg_Head head;
	char number;
	bt_list_result list[QM_MAX_BT_NUM];
}bt_list_msg;

typedef struct bt_scan_Req_msg{
	Msg_Head head;
}bt_scan_Req_msg;

typedef struct bt_scan_end_msg{
	Msg_Head head;
}bt_scan_end_msg;

typedef struct bt_Plist{
	char index;
    unsigned int  profile;
	char macAddress[13];//MAC地址
	char btName[QM_MAX_BTNAME_STR_LEN];//bt name string

}bt_Plist;

typedef struct bt_Plist_msg{
	Msg_Head head;
	char number;
	bt_Plist list[QM_MAX_BT_NUM];
}bt_Plist_msg;

typedef struct bt_Plist_Req_msg{
	Msg_Head head;
}bt_Plist_Req_msg;

typedef struct bt_Plist_clear_Req_msg{
	Msg_Head head;
	char index;//0xff  mac address or index
	char mac[13];
}bt_Plist_clear_Req_msg;


typedef struct bt_PAIRREQ_msg{
	Msg_Head head;
	char Passkey[7];
	char mac[13];
	char name[QM_MAX_BTNAME_STR_LEN];
}bt_PAIRREQ_msg;

typedef struct bt_PAIRE_result_msg{
	Msg_Head head;
	char result;
	char mac[13];
}bt_PAIRE_result_msg;

typedef struct bt_PAIRE_confirm_msg{
	Msg_Head head;
	char mac[13];
	char accept;//(0)拒绝远端的配对请求(1)接受远端的配对请求
}bt_PAIRE_confirm_msg;


typedef struct bt_type_init_msg{
	Msg_Head head;
	char type;//0x00 fsc   default 0xff
}bt_type_init_msg;

typedef struct bt_on_off_msg{
	Msg_Head head;
	char onoff;//on 1,off 0
}bt_on_off_msg;

typedef struct bt_PIN_Read_req_msg{
	Msg_Head head;
}bt_PIN_Read_req_msg; //读配对密码

typedef struct bt_PIN_Read_msg{
	Msg_Head head;
	char pin[QM_MAX_BTNAME_STR_LEN];
}bt_PIN_Read_msg; //读配对密码

typedef struct bt_PIN_Setting_msg{
	Msg_Head head;
	char pin[QM_MAX_BTNAME_STR_LEN];
}bt_PIN_Setting_msg;//写配对密码

typedef struct bt_connect_msg{
	Msg_Head head;
	char mac[13];
}bt_connect_msg;

typedef struct bt_connect_success_msg{
	Msg_Head head;
}bt_connect_success_msg;

typedef struct bt_disconnect_msg{
	Msg_Head head;
	char mac[13];
	char name[QM_MAX_BTNAME_STR_LEN];
}bt_disconnect_msg;

typedef void (*callFunction)(void*);

//callFunction  g_sndMsg_to_ap = NULL;  
extern int qm_drv_hal_Init(char type,callFunction  rcvMsgFromDrv);
//extern int qm_drv_hal_Init(callFunction  rcvMsgFromDrv);
extern void * sndMsgtoDrv(void *msg);

//extern callFunction  g_sndMsg_to_ap;

extern void mailbox_put(Mailbox *mb, void *msg);
//extern void mailbox_put(Mailbox *mb, void *msg);
//extern void* mailbox_get(Mailbox *mb);

//extern char QmBtRcvBuffer[AT_BUFFER_SIZE];

extern int qm_drv_bt_Init(callFunction  rcvMsgFromDrv);

extern int qm_Fsc_BT_AT_SCAN_CMD(void);

extern int qm_Fsc_BT_AT_DSCA_CMD(void);

extern int qm_Fsc_BT_AT_BTEN_CMD(char  *param);

extern int qm_Fsc_BT_AT_PLIST_CMD(const char *prarm);

extern int qm_Fsc_BT_AT_PLIST_Result(void);

extern int qm_Fsc_BT_AT_CFM_CMD(const char *mac,const char *flag);

extern int qm_Fsc_BT_AT_PIN_Setting(char  *param);

extern int qm_Fsc_BT_AT_PIN_Read(void);

extern int qm_Fsc_BT_AT_CONECT_REQ(char  *mac);


#endif
