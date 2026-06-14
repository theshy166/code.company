/*
 * Copyright (c) 2022  quaming
 *
 */
#ifndef _WFIF_CFG_H_
#define _WFIF_CFG_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>

#include <stdlib.h>


#define QM_MAX_SSID_NUM  50
#define QM_MAX_SSID_STR_LEN 100
#define QM_MAX_CFG_SSID_STR_LEN 300

#define BT_TYPE_RECALL      0x00
#define WIFI_TYPE_RECALL    0x01
#define MAX_TYPE_RECALL     0x02


#define MAILBOX_CAPACITY 30


typedef struct WlanStatus{
    char connect;
	int signal;
}WlanStatus;

#if 0
typedef struct {
    void *messages[MAILBOX_CAPACITY];
    int front, rear, count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} Mailbox;
#endif
typedef void (*callFunction)(void*);

typedef struct wifi_ssid_list{
	char ssid_num;
	char wifi_SSID[QM_MAX_SSID_NUM][QM_MAX_SSID_STR_LEN];
	double signel[QM_MAX_SSID_NUM];
}wifi_ssid_list;


#define MAX_AP 100
#define MAX_SSID_LEN 32

typedef struct {
    char ssid[MAX_SSID_LEN];
    int signal;
} WifiAP;




typedef struct MailNode {
    void* msg;
    struct MailNode* next;
} MailNode;

typedef struct {
    MailNode* head;
    MailNode* tail;
    int size;
    pthread_mutex_t lock;
} Mailbox;

Mailbox* create_mailbox();
bool send_message(Mailbox* mb, void* msg);
void* receive_message(Mailbox* mb);
bool is_mailbox_empty(Mailbox* mb);
void destroy_mailbox(Mailbox* mb);

int get_interface_flags(const char *ifname);
int get_default_gateway(char *gateway, size_t len);
int get_interface_ip(const char *ifname, char *ip_buf, size_t len);

int qm_wifi_wlan0_down(void);

int qm_connection_wifi_stop(void);

int qm_connection_wifi_restart(void);

int qm_wifi_cfg(const char *ssid,const char *passwd,const char * key);

int qm_wifi_connecttion_result(void);

int qm_wifi_status_query(const char *witTims);

int qm_wifi_connect_status(void);

int qm_ping_network(void );

int qm_wifi_start_wlan0_scan(void);

int qm_wifi_scan_ssid_list_start(void);

int qm_wifi_scan_ssid_list_stop(void);

int qm_wifi_get_ssid_list(wifi_ssid_list * ssidList);
/*********************************************/
//next two function path /var/wpa_supplicant
int qm_wpa_cli_scan_start(void);
//input wifilist struct prt retun value>= 0 wifi nume -1 wpa_supplicant not start
int qm_wpa_cli_ap_list(WifiAP *ap_list);
/*********************************************/
//next two fucntion /tmp/wifi/wpa_supplicant path  nxf7 usefucntion
int qm_tmp_path_wpa_cli_ap_list(WifiAP *ap_list);
//input wifilist struct prt retun value>= 0 wifi nume -1 wpa_supplicant not start
int qm_tamp_path_wpa_cli_scan_start(void);

int qm_wifi_get_rssi_test_new(char freqIndex);

int qm_get_ssid_psw(char *Ssid, char *passwd);

int qm_bt_servicer_start(void);

int qm_bt_servicer_stop(void);

int qm_bt_client_start(void);

int qm_bt_clinet_stop(void);

void qm_stop_host_ap(void);

int qm_wpa_stop(void);

//void qm_host_start5g(char *ssid,char *passwd);
void qm_host_start5g(char *ssid,char *passwd,unsigned char channel,char blueStart);

int qm_get_ip_address(const char *iface,char *ip);

WlanStatus  qm_wifi_get_ssid_signal(void);

void * qm_drv_hal_msg_snd_proc(void* txmb);

void * qm_bt_msg_fsc_proc(void *arg);


//extern callFunction  g_sndMsg_to_ap[];

extern void * sndMsgtoDrv(void *msg);

extern int qm_drv_hal_Init(char type,callFunction  rcvMsgFromDrv);


#endif
