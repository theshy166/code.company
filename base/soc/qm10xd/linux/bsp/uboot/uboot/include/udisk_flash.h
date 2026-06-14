/*
 * (C) Copyright 2024 Quaming
 *
 * David G, Quaming Software Engineering, guzhihuan@quaming.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
 
 
#ifndef __UDISK_FLASH_H__
#define __UDISK_FLASH_H__

#include "compiler.h"
#include <stdbool.h>
#include <asm/byteorder.h>
#include <command.h>
#include <spl.h>

/*SCRIPT format header*/
#define SCRIPT_CMD_MAGIC "SCRIPT!"
#define SCRIPT_CMD_MAGIC_SIZE 7

#define SCRIPT_KEY_REBOOT "REBOOT"
#define SCRIPT_KEY_REBOOT_SIZE 6
#define SCRIPT_KEY_DONE "DONE"
#define SCRIPT_KEY_DONE_SIZE 4

#define SCRIPT_KEY_ETHADDR_START "ETHADDR_START"
#define SCRIPT_KEY_ETHADDR_START_SIZE 13
#define SCRIPT_KEY_ETHADDR_END "ETHADDR_END"
#define SCRIPT_KEY_ETHADDR_END_SIZE 11

#define SCRIPT_KEY_ETH0 "ethaddr"
#define SCRIPT_KEY_ETH1 "eth1addr"
#define SCRIPT_KEY_ETH2 "eth2addr"

#define SCRIPT_KEY_WRITE_SN "WRITE_SN"
#define SCRIPT_KEY_WRITE_SN_SIZE 8

#define SCRIPT_CMD_START "CMD:\n"
#define SCRIPT_CMD_START_SIZE 5

#define MAX_LINE_LENGTH 256
#define MAX_KEY_LENGTH 64
#define MAX_VALUE_LENGTH 64

/* read out the script file from U-disk, then run them automatically */
#define script_file "script.ini"
#define isrun_script_file "script-DONE.txt"

#define macid_ini "macid.ini"
#define MACID_INI_MAX_SIZE	512
#define macid_dat "macid.dat"
#define MACID_FILE_SIZE	18 

#define sn_ini "sn.ini"
#define SN_INI_MAX_SIZE	512 
#define sn_dat "sn.dat"
#define SN_FILE_SIZE 11
#define SN_PREFIX "2528W"
#define SN_PREFIX_LEN 5
#define SN_NUM_LEN 6


typedef struct script_img_hdr script_img_hdr;
struct script_img_hdr {
    /* Must be SCRIPT_CMD_MAGIC. */
    char magic[SCRIPT_CMD_MAGIC_SIZE];
    uint8_t LF; // '/n'
    char config[MAX_LINE_LENGTH];
} __attribute__((packed));

extern script_img_hdr global_script;
int script_check_header(const struct script_img_hdr *hdr);
int script_check_config(const char *config,const char *key, const char *data, unsigned int length);
int script_check_ethaddr(const char *config, char *ethaddr, unsigned int *lowint, unsigned int *highint);
void int2ethaddr(uint32_t lowint, uint32_t highint, char *mac_string) ;
void ethaddr2int( char *mac_string,unsigned int *lowint ,unsigned int *highint);
int flash_commands(void);
int macid_commands(void);
uint32_t script_get_header_size(const script_img_hdr *hdr);
ulong script_get_header(const script_img_hdr *hdr);
ulong script_get_data(const script_img_hdr *hdr);
int bootup_commands(void);
int ota_commands(void);
int isFlashMode(void);
#if defined(CONFIG_WRITE_SN)
int sn_commands(void);
#endif
#endif	/* __UDISK_FLASH_H__ */

 
