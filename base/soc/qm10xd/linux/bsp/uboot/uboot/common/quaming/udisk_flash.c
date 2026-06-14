/*
 * (C) Copyright 2024 Quaming
 *
 * David G, Software Engineering, guzhihuan@quaming.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <watchdog.h>

#ifdef CONFIG_LOGBUFFER
#include <logbuff.h>
#endif

#include <rtc.h>

#include <environment.h>
#include <image.h>
#include <mapmem.h>

#include <u-boot/md5.h>
#include <u-boot/sha1.h>
#include <linux/errno.h>
#include <asm/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>

DECLARE_GLOBAL_DATA_PTR;

#include <u-boot/md5.h>
#include <linux/time.h>
#include <image.h>
#include <autoboot.h>
#include <cli.h>
#include <console.h>
#include <serial.h>
#include <post.h>
#include <common.h>
#include <command.h>

#include <u-boot/crc.h>
#include "udisk_flash.h"
#include "util_flash.h"
#include <android_bootloader_message.h>
#if defined(CONFIG_UBOOT_XKPHOTO)
#define UBOOT_DOWN_ADDR		0x41000000
#define KERNEL_LOAD_ADDR        0x40008000
#define LOGO_LOAD_ADDR		0x43A00000
#define LOGO_LOAD_SIZE 		0xAFCC
#define MISC_OFFSET			0x000F8000
#elif defined(CONFIG_UBOOT_ESL)
#define UBOOT_DOWN_ADDR     0x41000000
#define MISC_OFFSET			0x00300000
#else
#define UBOOT_DOWN_ADDR     0x41000000
#define MISC_OFFSET			0x00300000
#endif

#define MAX_BOOT_COUNT (3-1)//max boot count 3
#define BOOT_SUCCESSFUL 1
#if defined(CONFIG_UBOOT_XKPHOTO)
#define BOOTARGS_RECOVERY "loglevel=4 mem=48M earlycon console=ttyS0,115200 user_debug=31"
#define READ_CMD "sf read"
#define WRITE_CMD "sf write"
#define ERASE_CMD "sf erase"
#elif defined(CONFIG_UBOOT_ESL)
#define BOOTARGS_RECOVERY "loglevel=4 ubi.mtd=7 ubi.mtd=11 root=ubi0:rootfs init=/init rootfstype=ubifs mem=32M earlycon console=ttyS0,115200 user_debug=31"
#define READ_CMD "nand read"
#define WRITE_CMD "nand write"
#define ERASE_CMD "nand erase"
#else
#define BOOTARGS_RECOVERY "loglevel=4 ubi.mtd=7 ubi.mtd=11 root=ubi0:rootfs init=/init rootfstype=ubifs mem=32M earlycon console=ttyS0,115200 user_debug=31"
#define READ_CMD "nand read"
#define WRITE_CMD "nand write"
#define ERASE_CMD "nand erase"
#endif

script_img_hdr global_script;

uint32_t script_get_header_size(const script_img_hdr *hdr)
{
	char *cmd_pos = strstr((char *)hdr, SCRIPT_CMD_START);
	if (cmd_pos != NULL) {
		return (uint32_t)cmd_pos;
	}
	return 0;
}
ulong script_get_header(const script_img_hdr *hdr)
{
      script_img_hdr *des_hdr = &global_script;
      char *cmd_pos = strstr((char *)hdr, SCRIPT_CMD_START);
      if (cmd_pos != NULL) {
              memcpy(des_hdr, hdr, ((ulong)cmd_pos > MAX_LINE_LENGTH) ? MAX_LINE_LENGTH:(ulong)cmd_pos);
		return 1;
      }
      return 0;
}

ulong script_get_data(const script_img_hdr *hdr)
{
       char *cmd_pos = strstr((char *)hdr, SCRIPT_CMD_START);

       if (cmd_pos != NULL) {
		   return ((ulong)cmd_pos + SCRIPT_CMD_START_SIZE);
       }
       return 0;
}
int script_check_header(const struct script_img_hdr *hdr)
{
	return memcmp(SCRIPT_CMD_MAGIC, hdr->magic, SCRIPT_CMD_MAGIC_SIZE);
}

char *get_key_value(const char *data, const char *key) {
    const char *newline = "\n";
  //  const char *delimiter = "=";
    char *line;
    char *key_value;
    char tmp[MAX_LINE_LENGTH];

    if(!data)
		return NULL;

    strcpy(tmp,data);

    // Tokenize data by newline
    line = strtok((char *)tmp, newline);

    while (line != NULL) {
        // Tokenize each line by "="
        key_value = strchr(line, '=');
        if (key_value != NULL) {
            // Split key and value
            *key_value = '\0'; // Terminate key
            key_value++; // Move past '='

            // Trim leading spaces from key
            while (*line == ' ') line++;

            // Trim leading spaces from value
            while (*key_value == ' ') key_value++;

            if (strcmp(key, line) == 0) {
                // Found key, return value
                return strdup(key_value);
            }
        }

        // Move to the next line
        line = strtok(NULL, newline);
    }

    // Key not found
    return NULL;
}

int script_check_config(const char *config,const char *key, const char *data, unsigned int length)
{
	char *value = get_key_value((char *)config, key);
	if (value != NULL) {
		if(!memcmp(value,data,length)){
			free(value); // Free the dynamically allocated memory
			return 0;
		}
		free(value); // Free the dynamically allocated memory
	}
	return -1;
}

int script_get_num(const char *config,const char *key, int *num)
{
	char *value = get_key_value((char *)config, key);
	if (value != NULL) {
		*num = (int)simple_strtoul(value, NULL, 10);
		free(value); // Free the dynamically allocated memory
		return 0;
	}
	return -1;
}
void parse_ethaddr(const char *addr, uchar *enetaddr) {
    char *end;
    int i;

    for (i = 0; i < 6; ++i) {
        enetaddr[i] = addr ? simple_strtoul(addr, &end, 16) : 0;
        if (addr) {
            // 如果当前字符是冒号，则跳过
            if (*end == ':')
                end++;
            addr = (*end) ? end : end;
        }
    }
}

void ethaddr2int( char *mac_string,unsigned int *lowint ,unsigned int *highint)
{
	unsigned char mac[6];
	parse_ethaddr(mac_string, mac);
	*highint = (mac[0] << 8) | mac[1];
	*lowint = (mac[2] << 24) | (mac[3] << 16) | (mac[4] << 8) | mac[5];
}

void int2ethaddr(uint32_t lowint, uint32_t highint, char *mac_string)
{
    unsigned char mac[6];

    mac[0] = (highint >> 8) & 0xFF;
    mac[1] = highint & 0xFF;

    mac[2] = (lowint >> 24) & 0xFF;
    mac[3] = (lowint >> 16) & 0xFF;
    mac[4] = (lowint >> 8) & 0xFF;
    mac[5] = lowint & 0xFF;

    sprintf(mac_string, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

int script_check_ethaddr(const char *config, char *ethaddr, unsigned int *lowint, unsigned int *highint)
{
	unsigned int lowint_start, highint_start;
	unsigned int lowint_end , highint_end;
	unsigned int lowint_current ,highint_current;

	/*get ethaddr start*/
	char *value = get_key_value((char *)config, SCRIPT_KEY_ETHADDR_START);
	if (!value)
		goto fail;

	ethaddr2int(value,&lowint_start,&highint_start);
	free(value); // Free the dynamically allocated memory
	/*get ethaddr poul end*/
	value = get_key_value((char *)config, SCRIPT_KEY_ETHADDR_END);
	if (!value)
		goto fail;

	ethaddr2int(value,&lowint_end,&highint_end);
	free(value); // Free the dynamically allocated memory

	/*get ethaddr from doc in udisk*/
	if (!ethaddr)
		goto fail;

	ethaddr2int(ethaddr,&lowint_current,&highint_current);
	printf("highint_current:0x%x,highint_start:0x%x,highint_end:0x%x\n",\
		highint_current,highint_start,highint_end);
	printf("lowint_current:0x%x,lowint_start:0x%x,lowint_end:0x%x\n",\
		lowint_current,lowint_start,lowint_end);

	if((highint_current==highint_start) && (highint_current==highint_end))
	{
		if((lowint_current>=lowint_start) && (lowint_current<=lowint_end))
		{
			*lowint 	= lowint_current;
			*highint 	= highint_current;
			int2ethaddr(lowint_current,highint_current,ethaddr);
			printf("script_check_ethaddr: ethaddr:%s\n",ethaddr);
			return 0;
		}
	}
fail:
	return 1;
}

#if defined(CONFIG_LED_INDICATOR_FOR_FLASH)
#define LEDON           0
#define LEDOFF          1
#define LEDBLINK        2

int led_control(int status)
{
	int ret = 0;
	char cmd[50];

	if (status == 0) {
		snprintf(cmd, sizeof(cmd), "blinkled %d o", CONFIG_LED_INDICATOR_GPIO);
	} else if (status == 1) {
		snprintf(cmd, sizeof(cmd), "blinkled %d f", CONFIG_LED_INDICATOR_GPIO);
	} else if (status == 2) {
		snprintf(cmd, sizeof(cmd), "blinkled %d b 600 11", CONFIG_LED_INDICATOR_GPIO);
	} else {
		printf("Error: Invalid status value\n");
		return -1;
	}

	ret = run_command(cmd, 0);
	if (ret != 0) {
		printf("Error: Failed to execute command '%s'\n", cmd);
	}

	return ret;
}
#endif

int flash_commands(void)
{
	int ret=0;
	char script_cmd[100]={0};
	memset((char *)UBOOT_DOWN_ADDR,0,0x2000);
	//detect the flag file as the condition to run script cmd or not
#if defined(CONFIG_SD_FLASH)
	sprintf(script_cmd,"fatload mmc 0:1 0x%lx %s",(long)UBOOT_DOWN_ADDR,script_file);
#elif defined(CONFIG_USB_FLASH)
	sprintf(script_cmd,"fatload usb 0:1 0x%lx %s",(long)UBOOT_DOWN_ADDR,script_file);
#endif
	ret = run_command(script_cmd, 0);
	printf("ret=%d,run command:%s\n",ret,script_cmd);
	if (ret){
		ret = -1;
		goto fail;
	}
	//start read script.ini file from udisk or sd card
#if !defined(CONFIG_FLASH_FW_ON_KEY_PRESS)
	memset(script_cmd,0,sizeof(script_cmd));
#if defined(CONFIG_SD_FLASH)
	sprintf(script_cmd,"fatload mmc 0:1 0x%lx %s",(long)UBOOT_DOWN_ADDR,isrun_script_file);
#elif defined(CONFIG_USB_FLASH)
	sprintf(script_cmd,"fatload usb 0:1 0x%lx %s",(long)UBOOT_DOWN_ADDR,isrun_script_file);
#endif
	ret = run_command(script_cmd, 0);
	printf("ret=%d,run command:%s\n",ret,script_cmd);
	if (!ret){
#if defined(CONFIG_SD_FLASH)
		printf("%s file already existed in sd card, so ignor script cmds\n",isrun_script_file);
#elif defined(CONFIG_USB_FLASH)
		printf("%s file already existed in U disk, so ignor script cmds\n",isrun_script_file);
#endif
		ret = -2;
		goto fail;
	}
#endif // !defined(CONFIG_FLASHING_ON_KEY_PRESS)
	//start run source cmd to run all commands in script.ini file
#if defined(CONFIG_LED_INDICATOR_FOR_FLASH)
	led_control(LEDON);
#endif
	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"source 0x%lx",(long)UBOOT_DOWN_ADDR);
	ret = run_command(script_cmd, 0);
	printf("ret=%d,run command:%s\n",ret,script_cmd);
	if (ret){
		goto fail;
	}
	//
	if(!script_check_config(global_script.config,SCRIPT_KEY_DONE, "yes", 3)) {
		// create a new file as the flag to script cmd ok
		memset(script_cmd,0,sizeof(script_cmd));
#if defined(CONFIG_SD_FLASH)
		sprintf(script_cmd,"fatwrite mmc 0:1 0x%lx %s 7",(long)UBOOT_DOWN_ADDR,isrun_script_file);
#elif defined(CONFIG_USB_FLASH)
 		sprintf(script_cmd,"fatwrite usb 0:1 0x%lx %s 7",(long)UBOOT_DOWN_ADDR,isrun_script_file);
#endif
		ret = run_command(script_cmd, 0);
		printf("ret=%d,run command:%s\n",ret,script_cmd);
		if (ret){
			goto fail;
		}
	}

fail:
#if defined(CONFIG_LED_INDICATOR_FOR_FLASH)
	if (ret > 0) {
		led_control(LEDOFF);
	}
#endif
	return ret;
}

int macid_commands(void)
{
	int ret=0;
	char script_cmd[100]={0};
	unsigned int lowint=0;
	unsigned int highint=0;
	char macid[MACID_FILE_SIZE+1]={0};
	char config[MACID_INI_MAX_SIZE+1]={0};
	int ethnum;
	char ethxaddr[10];
	int i;

	//read ethaddr from macid.dat file in U-disk
	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"mw 0x%lx 0 0x100",(long)UBOOT_DOWN_ADDR);
	ret = run_command(script_cmd, 0);
	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"fatload usb 0:1 0x%lx %s",(long)UBOOT_DOWN_ADDR,macid_dat);
	ret = run_command(script_cmd, 0);
	printf("ret=%d,run command:%s\n",ret,script_cmd);
	if (ret){
		printf("no %s file in U disk\n",macid_dat);
		ret = -1;
		goto fail;
	}
	memcpy(macid,(char *)UBOOT_DOWN_ADDR,MACID_FILE_SIZE);
	printf("macid:%s\n",macid);

	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"mw 0x%lx 0 0x100",(long)UBOOT_DOWN_ADDR);
	ret = run_command(script_cmd, 0);
	//start read macid_ini file from U-disk
	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"fatload usb 0:1 0x%lx %s",(long)UBOOT_DOWN_ADDR,macid_ini);
	ret = run_command(script_cmd, 0);
	printf("ret=%d,run command:%s\n",ret,script_cmd);
	if (ret){
		ret = -2;
		goto fail;
	}
	memcpy((char *)config,(char *)UBOOT_DOWN_ADDR,MACID_INI_MAX_SIZE);

	//check ethaddr and write it into env and files
	ret = script_check_ethaddr((char *)config,(char *)macid,&lowint,&highint);
	if (ret){
		ret = -3;
		goto fail;
	}
	script_get_num((char *)config,"ethnum",&ethnum);
	#if defined(CONFIG_LED_INDICATOR_FOR_FLASH)
        //start macid writing, led on
        if(ethnum > 0)
		led_control(LEDON);
	#endif
	printf ("ethnum=%d\n",ethnum);
	for (i=0;i<ethnum;i++)
	{
		if(i==0) {
			strcpy(ethxaddr,"ethaddr");//eth0 mac id
		}
		else
			sprintf(ethxaddr,"eth%daddr", i);
		if(!script_check_config((char *)config,ethxaddr, "yes", 3))
		{
			// read ethxaddr from env
			memset(script_cmd,0,sizeof(script_cmd));
			sprintf(script_cmd,"printenv %s",ethxaddr);
			ret = run_command(script_cmd, 0);
			printf("ret=%d,run command:%s\n",ret,script_cmd);
			if (ret){
				// no ethxaddr yet, write the latest macid into macid_file
				lowint++;
				memset(script_cmd,0,sizeof(script_cmd));
				memset((char *)UBOOT_DOWN_ADDR,0,MACID_FILE_SIZE);
				int2ethaddr(lowint, highint, (char *)UBOOT_DOWN_ADDR);
				sprintf(script_cmd,"fatwrite usb 0:1 0x%lx %s %d",(long)UBOOT_DOWN_ADDR,macid_dat,MACID_FILE_SIZE);
				ret = run_command(script_cmd, 0);
				printf("ret=%d,run command:%s\n",ret,script_cmd);
				if (ret){
					goto fail;
				}
				// write ethxaddr into env
				memset(script_cmd,0,sizeof(script_cmd));
				sprintf(script_cmd,"setenv %s %s",ethxaddr,(char *)UBOOT_DOWN_ADDR);
				ret = run_command(script_cmd, 0);
				printf("ret=%d,run command:%s\n",ret,script_cmd);
				if (ret){
					goto fail;
				}
				// save env
				memset(script_cmd,0,sizeof(script_cmd));
				strcpy(script_cmd,"saveenv");
				ret = run_command(script_cmd, 0);
				printf("ret=%d,run command:%s\n",ret,script_cmd);
				if (ret){
					goto fail;
				}

			}
		}
	}

fail:
#if defined(CONFIG_LED_INDICATOR_FOR_FLASH)
        if (ret > 0) {
                //macid writing fail, led off
                led_control(LEDOFF);
        }
#endif
	return ret;
}

#if defined(CONFIG_MOLCHIP_WATCHDOG)
extern void hw_watchdog_reset(void);
#endif
int bootup_commands(void)
{
	int ret=0;
	int flash_result=0;

#if defined(CONFIG_OTA_SCRIPT_CMD) && !defined(CONFIG_FLASH_FW_ON_KEY_PRESS)
	printf("judeg misc command to flashing");
	return 0;
#endif

#if defined(CONFIG_FLASH_FW_ON_KEY_PRESS)
	if (!flashing_fw_requested()) {
		printf("skip flashing fw...\n");
		return 0;
	}
#endif
#if defined(CONFIG_MOLCHIP_WATCHDOG)
        molchip_wdt_disable();
#endif
#if defined(CONFIG_USB_FLASH)
	ret = run_command("usb start", 0);
	printf("ret=%d run command:usb start\n",ret);
	if (ret){
		ret = -1;
		goto fail;
	}
	//udisk flashing
	printf("udisk flashing....\n");
#elif defined(CONFIG_SD_FLASH)
	printf("sd card flashing....\n");
#endif
#if defined(CONFIG_WRITE_SN)
	//check CLEAN_SN flag
	// 读取sn.ini
	char config[SN_INI_MAX_SIZE+1]={0};
	char script_cmd[100]={0};
	sprintf(script_cmd, "fatload usb 0:1 0x%lx %s", (long)UBOOT_DOWN_ADDR, sn_ini);
	ret = run_command(script_cmd, 0);
	if (ret) {
		ret = -2;//no sn.ini file in U disk
		goto flash;
	}
	memcpy(config, (char *)UBOOT_DOWN_ADDR, SN_INI_MAX_SIZE);
	char *clean_sn = NULL;
    // 从sn.ini读取CLEAN_SN配置
    clean_sn = get_key_value(config, "CLEAN_SN");
    if (!clean_sn) {
        printf("no 'CLEAN_SN' value in sn.ini\n");
        goto flash;
    }
	printf("clean_sn: %s\n", clean_sn);
	if(memcmp(clean_sn,"yes",3) == 0) {
		// 如果CLEAN_SN为yes，则清除sn
		printf("it goona clean sn, then write new one\n");
		run_command("setenv sn", 0);//clean sn
		mdelay(100);
	}
	free(clean_sn); // 释放内存
	// 检查是否已设置SN
	ret = run_command("printenv sn", 0);
	if (!ret) {
		// SN已设置，跳过写入,直接返回开机
		printf("SN already set, skipping write, then bootup normally\n");
		run_command("usb stop", 0);
		#if defined(CONFIG_MOLCHIP_WATCHDOG)
        hw_watchdog_reset();
		#endif
		return ret;
	}
flash:
#endif
	//udisk flash
	flash_result = flash_commands();
	if (flash_result < 0){
		//note: some cmd in source will return 1, but the cmd does run ok
		//like use "setenv ethaddr" cmd to clean macid, but it will case source cmd return 1
		//to save normal bootup time, if don't have udisk flash ini file, don't write macid ether.
		goto fail;
	}
#if defined(CONFIG_WRITE_MACID)
	//macid writing
	printf("macid writing....\n");
	ret = macid_commands();
	if (ret){
		goto fail;
	}
#endif
#if defined(CONFIG_WRITE_SN)
	//sn writing
	printf("sn writing....\n");
	ret = sn_commands();
	if (ret){
		goto fail;
	}
#endif

fail:
#if defined(CONFIG_USB_FLASH)
	printf("run command:usb stop,ret=%d\n",ret);
	run_command("usb stop", 0);
#endif

	if(!script_check_config((char *)global_script.config,SCRIPT_KEY_REBOOT, "yes", 3)) {
		printf ("system will reboot\n");
		mdelay(100);
		run_command("reboot", 0);
	}
#if defined(CONFIG_LED_INDICATOR_FOR_FLASH)
	if (!ret && !flash_result) {
		//both flash and macid writing pass, led blinding and block system
		//note: this LEDBLIND cmd will block the uboot system forever, till power off
		led_control(LEDBLINK);
	}
	if((ret > 0) || flash_result > 0){
		//flash or macid writing fail, led closed and block system
		led_control(LEDOFF);
		printf("usb image flash failed or macid write failed!!!\n");
		while(1);
	}
#endif
#if defined(CONFIG_MOLCHIP_WATCHDOG)
        hw_watchdog_reset();
#endif
	return ret;
}

/* return value
 * 0 : is flash mode
 * 1: otherwise  */
int isFlashMode(void)
{
	int ret=0;
	char script_cmd[100]={0};
	struct android_bootloader_message bcb={0};
	static bool sf_probe = false;

	if (!sf_probe) {
		sf_probe = true;
		memset(script_cmd,0,sizeof(script_cmd));
		sprintf(script_cmd,"sf probe");
		ret = run_command(script_cmd, 0);
	}

	memset((char *)UBOOT_DOWN_ADDR,0,0x2000);
	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"sf read 0x%lx 0x%lx 0x%x",(long)UBOOT_DOWN_ADDR,(long)MISC_OFFSET,sizeof(bcb));
	ret = run_command(script_cmd, 0);
	printf("ret=%d run command:%s\n",ret,script_cmd);
	if (ret){
		return -1;
	}

	memcpy(&bcb,(char *)(UBOOT_DOWN_ADDR),\
		sizeof(struct android_bootloader_message));

	if(strlen(bcb.command) >0 || (bcb.successful_boot[0] == 0 && bcb.tries_remaining[0] >= 254)) {
		return 0;
	}
	else {
		return 1;
	}
}

int write_misc(char boot_success,char tries_remaining,char *command)
{
	int ret=0;
	char script_cmd[100]={0};
	struct android_bootloader_message bcb={0};
	memset((char *)UBOOT_DOWN_ADDR,0,0x2000);
	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"sf read 0x%lx 0x%lx 0x%x",(long)UBOOT_DOWN_ADDR,(long)MISC_OFFSET,sizeof(struct android_bootloader_message));
	ret = run_command(script_cmd, 0);
	printf("ret=%d run command:%s\n",ret,script_cmd);
	if (ret){
		return -1;
	}
	memcpy(&bcb,(char *)(UBOOT_DOWN_ADDR),\
		sizeof(struct android_bootloader_message));

	bcb.successful_boot[0] = boot_success;
	bcb.tries_remaining[0] = tries_remaining;
	memcpy(bcb.command,command,sizeof(bcb.command));

	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"sf erase 0x%lx 0x%x",(long)MISC_OFFSET,sizeof(bcb));
	ret = run_command(script_cmd, 0);
	printf("ret=%d run command:%s\n",ret,script_cmd);
	if (ret){
		return -2;
	}
	memset(script_cmd,0,sizeof(script_cmd));
	memcpy((char *)UBOOT_DOWN_ADDR,&bcb,sizeof(bcb));
	sprintf(script_cmd,"sf write 0x%lx 0x%lx 0x%x",(long)UBOOT_DOWN_ADDR,(long)MISC_OFFSET,sizeof(bcb));
	ret = run_command(script_cmd, 0);
	printf("ret=%d run command:%s\n",ret,script_cmd);
	if (ret){
		return -3;
	}
	return 0;
}

int ota_commands(void)
{
	int ret=0;
	char script_cmd[100]={0};
	char misc_data[100]={0xFF};
	memset(misc_data,0xFF,100);

#if defined(CONFIG_MOLCHIP_WATCHDOG)
        molchip_wdt_disable();
#endif
	//read misc flag
	//misc (1M)
	struct android_bootloader_message bcb;

	memset((char *)UBOOT_DOWN_ADDR,0,0x2000);
	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"sf read 0x%lx 0x%lx 0x%x",(long)UBOOT_DOWN_ADDR,(long)MISC_OFFSET,sizeof(bcb));
	ret = run_command(script_cmd, 0);
	printf("ret=%d run command:%s\n",ret,script_cmd);
	if (ret){
		ret = -1;
		memset(script_cmd,0,sizeof(script_cmd));
		sprintf(script_cmd,"sf erase 0x%lx 0x%x",\
			(long)MISC_OFFSET,sizeof(bcb));
		ret = run_command(script_cmd, 0);
		printf("ret=%d run command:%s\n",ret,script_cmd);

		//goto flash;//nand read misc fail, try to flash again
		printf("%s:boot-recovery enter recovery\n",__func__);
                char *s;
                s = env_get("recoveryboot");
                if(s) {
			    env_set("bootargs",BOOTARGS_RECOVERY);
                        run_command_list(s, -1, 0);
                        //run into recovery os
                }
                printf("No recoveryboot:%s, available\n",s);
	}
	memcpy(&bcb,(char *)(UBOOT_DOWN_ADDR),\
		sizeof(struct android_bootloader_message));
	printf("bcb.command:%s\n",bcb.command);
	printf("bcb.tries_remaining:%d\n",bcb.tries_remaining[0]);
	printf("uboot bcb.successful_boot:%d\n",bcb.successful_boot[0]);
	if (!strncmp("usb-update", bcb.command,10)) {
		printf("%s: update from usb\n",__func__);
		memset(script_cmd,0,sizeof(script_cmd));
		sprintf(script_cmd,"usb start;fatload usb 0:1 41000000 script.ini; source 41000000;");
		ret = run_command(script_cmd, 0);
		printf("ret=%d run command:%s\n",ret,script_cmd);
		//---- to clear misc----------------------
		memset(script_cmd,0,sizeof(script_cmd));
		sprintf(script_cmd,"sf erase 0x%lx 0x%x",\
			(long)MISC_OFFSET,sizeof(bcb));
		ret = run_command(script_cmd, 0);
		printf("ret=%d run command:%s\n",ret,script_cmd);
		goto bootup;
	}

	if (!strncmp("tcard-update", bcb.command,10)) {
		printf("%s: update from TCard\n",__func__);
		memset(script_cmd,0,sizeof(script_cmd));
		sprintf(script_cmd,"fatload mmc 0:1 41000000 script.ini; source 41000000;");
		ret = run_command(script_cmd, 0);
		printf("ret=%d run command:%s\n",ret,script_cmd);
		//---- to clear misc----------------------
		memset(script_cmd,0,sizeof(script_cmd));
		sprintf(script_cmd,"sf erase 0x%lx 0x%x",\
			(long)MISC_OFFSET,sizeof(bcb));
		ret = run_command(script_cmd, 0);
		printf("ret=%d run command:%s\n",ret,script_cmd);
		goto bootup;
	}

	if (!strncmp("bootonce-bootloader", bcb.command,19)) {
		/* Erase the message in the BCB since this value should be used
		 * only once.
		 */
		//set tries_remaining to 3 and clean command
		char command[32]={0};
		write_misc(0,MAX_BOOT_COUNT,command);
		goto flash;
	}

	//if (!strncmp("boot-recovery", bcb.command,13) || !memcmp(misc_data, bcb.command,32) ) {
	if (!strncmp("boot-recovery", bcb.command,13) || (bcb.successful_boot[0] == 0 && bcb.tries_remaining[0] >= 254)) {
		printf("%s:boot-recovery enter recovery\n",__func__);
		#if defined(CONFIG_UBOOT_XKPHOTO)
		char *bootargs_str;
		bootargs_str = env_get("bootargs");
		if(bootargs_str == NULL)
			goto bootup;
		printf("bootargs_str:%s\n",bootargs_str);
		memset(script_cmd,0,sizeof(script_cmd));
		sprintf(script_cmd,"fatload mmc 0:1 0x%lx recovery.img.md5",(long)KERNEL_LOAD_ADDR);
		ret = run_command(script_cmd, 0);
		printf("ret=%d,run command:%s\n",ret,script_cmd);
		if (ret)
		{
			//load recovery.img from udisk
			memset(script_cmd,0,sizeof(script_cmd));
			sprintf(script_cmd,"setenv bootargs '%s'",BOOTARGS_RECOVERY);
			ret = run_command(script_cmd, 0);
			printf("ret=%d,run command:%s\n",ret,script_cmd);
			ret = run_command("usb stop;usb start;", 0);
			printf("ret=%d run command:usb stop;usb start\n",ret);
			if (!ret) {
				//load logo_update.jpg
				memset(script_cmd,0,sizeof(script_cmd));
				sprintf(script_cmd,"fatload usb 0:1 0x%lx .logo_update.jpp",(long)LOGO_LOAD_ADDR);
				ret = run_command(script_cmd, 0);
				printf("ret=%d,run command:%s\n",ret,script_cmd);
				if(!ret) {
					memset(script_cmd,0,sizeof(script_cmd));
					sprintf(script_cmd,"qua_logo 0x%lx 0x%lx",(long)LOGO_LOAD_ADDR, (long)LOGO_LOAD_SIZE);
					ret = run_command(script_cmd, 0);
				}
				//load recovery.img
				sprintf(script_cmd,"fatload usb 0:1 0x%lx recovery.img",(long)KERNEL_LOAD_ADDR);
				ret = run_command(script_cmd, 0);
				printf("ret=%d,run command:%s\n",ret,script_cmd);
				if(!ret) {
                                  memset(script_cmd,0,sizeof(script_cmd));
                                  sprintf(script_cmd,"bootz 0x%lx - -",(long)KERNEL_LOAD_ADDR);
                                  ret = run_command(script_cmd, 0);
                                  //run into recovery os
                                  printf("ret=%d,run command:%s\n",ret,script_cmd);
				}
			}
			printf("bootargs:%s\n",bootargs_str);
			memset(script_cmd,0,sizeof(script_cmd));
                        sprintf(script_cmd,"setenv bootargs '%s'",bootargs_str);
                        ret = run_command(script_cmd, 0);
                        printf("ret=%d,run command:%s\n",ret,script_cmd);
		}else
		{
			//load logo_update.jpg
                        memset(script_cmd,0,sizeof(script_cmd));
                        sprintf(script_cmd,"fatload mmc 0:1 0x%lx logo_update.jpg",(long)LOGO_LOAD_ADDR);
                        ret = run_command(script_cmd, 0);
                        printf("ret=%d,run command:%s\n",ret,script_cmd);
                        if(!ret) {
                                memset(script_cmd,0,sizeof(script_cmd));
                                sprintf(script_cmd,"qua_logo 0x%lx 0x%lx",(long)LOGO_LOAD_ADDR, (long)LOGO_LOAD_SIZE);
                                ret = run_command(script_cmd, 0);
                        }
			//load recovery.img from sdcard
			memset(script_cmd,0,sizeof(script_cmd));
                        sprintf(script_cmd,"setenv bootargs '%s'",BOOTARGS_RECOVERY);
                        ret = run_command(script_cmd, 0);
                        printf("ret=%d,run command:%s\n",ret,script_cmd);
			ret = run_command("usb start", 0);
			printf("ret=%d run command:usb start\n",ret);
			//
			memset(script_cmd,0,sizeof(script_cmd));
			sprintf(script_cmd,"fatload mmc 0:1 0x%lx recovery.img",(long)KERNEL_LOAD_ADDR);
			ret = run_command(script_cmd, 0);
			printf("ret=%d,run command:%s\n",ret,script_cmd);
			if(!ret) {
				memset(script_cmd,0,sizeof(script_cmd));
				sprintf(script_cmd,"bootz 0x%lx - -",(long)KERNEL_LOAD_ADDR);
				ret = run_command(script_cmd, 0);
				//run into recovery os
				printf("ret=%d,run command:%s\n",ret,script_cmd);
			}
			printf("bootargs:%s\n",bootargs_str);
			memset(script_cmd,0,sizeof(script_cmd));
                        sprintf(script_cmd,"setenv bootargs '%s'",bootargs_str);
                        ret = run_command(script_cmd, 0);
                        printf("ret=%d,run command:%s\n",ret,script_cmd);
                }
		#else
		char *s;
		s = env_get("recoveryboot");
		if(s) {
			env_set("bootargs",BOOTARGS_RECOVERY);
			run_command_list(s, -1, 0);
			//run into recovery os
		}
		printf("No recoveryboot:%s, available\n",s);
		#endif
	}

	goto bootup;

flash:
#if defined(CONFIG_UBOOT_XKPHOTO)
	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"fatload mmc 0:1 0x%lx script-ota-mmc.ini",(long)UBOOT_DOWN_ADDR);
	memset((char *)UBOOT_DOWN_ADDR,0,0x2000);
        ret = run_command(script_cmd, 0);
        printf("ret=%d,run command:%s\n",ret,script_cmd);
	if (ret)
	{
                ret = run_command("usb stop;usb start;", 0);
		printf("ret=%d run command:usb stop;usb start\n",ret);
		if (!ret) {
			sprintf(script_cmd,"fatload usb 0:1 0x%lx script-ota.ini",(long)UBOOT_DOWN_ADDR);
		}
        }

	mdelay(100);
	memset((char *)UBOOT_DOWN_ADDR,0,0x2000);
	ret = run_command(script_cmd, 0);
	printf("ret=%d,run command:%s\n",ret,script_cmd);
	if (ret){
		ret = -1;
		goto bootup;
	}
#else
	ret = run_command("ubi part data;", 0);
	printf("ret=%d run command:ubi part data;\n",ret);
	if (ret){
		ret = -1;
		goto fail;
	}
	mdelay(100);
	ret = run_command("ubifsmount ubi0:data;", 0);
	printf("ret=%d run command:ubifsmount ubi0:data\n",ret);
	if (ret){
		ret = -1;
		goto fail;
	}
	mdelay(100);
	#if defined(CONFIG_UBOOT_ESL)
	//read board id
	const char *board_id = get_board_id();
	printf("board_id:%s\n",board_id);
	//read version.txt
	memset((char *)UBOOT_DOWN_ADDR,0,0x2000);
	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"ubifsload 0x%lx /version.txt;",(long)UBOOT_DOWN_ADDR);
	ret = run_command(script_cmd, 0);
	if (ret){
		ret = -1;
		printf("no version.txt file in ota package, so goto bootup\n");
		goto bootup;
	}
	//compare version.txt and board id
	char *version = (char *)UBOOT_DOWN_ADDR;
	printf("version.txt:%s\n",version);
	if (strncmp(version, board_id, strlen(board_id)) != 0) {
		printf("version.txt and board id not match\n");
		printf("version.txt:%s, board_id:%s\n", version, board_id);
		ret = -1;
		goto bootup;
	}
	#endif // CONFIG_UBOOT_ESL
	memset((char *)UBOOT_DOWN_ADDR,0,0x2000);
	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"ubifsload 0x%lx /script-ota.ini;",(long)UBOOT_DOWN_ADDR);
	ret = run_command(script_cmd, 0);
	printf("ret=%d run command:%s\n",ret,script_cmd);
	if (ret){
		ret = -1;
		goto bootup;
	}
#endif
	memset(script_cmd,0,sizeof(script_cmd));
	sprintf(script_cmd,"source 0x%lx",(long)UBOOT_DOWN_ADDR);
	ret = run_command(script_cmd, 0);
	printf("ret=%d run command:%s\n",ret,script_cmd);
	if (ret){
		ret = -1;
		goto fail;
	}
	//ota finished
	printf("ota flash finished\n");

fail:
	printf("run command ret=%d\n",ret);
	printf ("system will reboot\n");
	mdelay(100);
	run_command("reboot", 0);
	return ret;
bootup:
	if(bcb.tries_remaining[0] >= 0 && bcb.successful_boot[0] == 0) {
		write_misc(0,bcb.tries_remaining[0]-1,bcb.command);
	}
#if defined(CONFIG_MOLCHIP_WATCHDOG)
        hw_watchdog_reset();
#endif
	return ret;
}

#if defined(CONFIG_WRITE_SN)
int script_check_sn(const char *config, char *sn, unsigned int *current_num)
{
    unsigned int sn_start, sn_end;
	char *prefix = NULL;
    // 从ini读取SN前缀
    prefix = get_key_value(config, "SN_PREFIX");
    if (!prefix) {
        printf("Error: SN_PREFIX not found in config\n");
        return 1;
    }
    // 从ini读取SN范围
    char *value = get_key_value((char *)config, "SN_START");
    if (!value) goto fail;
    sn_start = simple_strtoul(value, NULL, 10);
    free(value);
	printf("sn_start:%d\n",sn_start);
    value = get_key_value((char *)config, "SN_END");
    if (!value) goto fail;
    sn_end = simple_strtoul(value, NULL, 10);
    free(value);
	printf("sn_end:%d\n",sn_end);
    // 解析当前SN数值
    if(strlen(sn) >= SN_PREFIX_LEN) {
        *current_num = simple_strtoul(sn + SN_PREFIX_LEN, NULL, 10);
    } else {
        *current_num = sn_start; // 默认从起始值开始
    }
	printf("*current_num:%d\n",*current_num);
    // 检查范围有效性
    if(*current_num >= sn_start && *current_num <= sn_end) {
        return 0;
    }

	// 生成SN时使用配置的前缀
    snprintf(sn, SN_FILE_SIZE, "%s%06d", prefix, *current_num);
	printf("Generated SN: %s\n", sn);
    free(prefix); // 释放内存
fail:
    return 1;
}

int sn_commands(void)
{
    int ret = 0;
    char script_cmd[100] = {0};
    unsigned int current_num = 0;
    char sn[SN_FILE_SIZE + 1] = {0};
    char config[SN_INI_MAX_SIZE + 1] = {0};

    // 读取sn.dat
    sprintf(script_cmd, "mw 0x%lx 0 0x100", (long)UBOOT_DOWN_ADDR);
    run_command(script_cmd, 0);
	memset(script_cmd, 0, sizeof(script_cmd));
    sprintf(script_cmd, "fatload usb 0:1 0x%lx %s", (long)UBOOT_DOWN_ADDR, sn_dat);
    ret = run_command(script_cmd, 0);
    if (ret) {
        printf("No %s file, init default\n", sn_dat);
        snprintf(sn, sizeof(sn), "%s0000001", SN_PREFIX); // 初始化默认SN
    } else {
        memcpy(sn, (char *)UBOOT_DOWN_ADDR, SN_FILE_SIZE);
        sn[SN_FILE_SIZE] = '\0';
    }

    // 读取sn.ini
	memset(script_cmd, 0, sizeof(script_cmd));
    sprintf(script_cmd, "fatload usb 0:1 0x%lx %s", (long)UBOOT_DOWN_ADDR, sn_ini);
    ret = run_command(script_cmd, 0);
    if (ret) {
        ret = 0;//no sn.ini file in U disk,as normal udisk flash steps
        goto fail;
    }
    memcpy(config, (char *)UBOOT_DOWN_ADDR, SN_INI_MAX_SIZE);

    // 校验SN范围
    ret = script_check_sn(config, sn, &current_num);
    if (ret) {
        ret = -3;
        goto fail;
    }

    // 检查是否已设置SN
	memset(script_cmd, 0, sizeof(script_cmd));
    sprintf(script_cmd, "printenv sn");
    ret = run_command(script_cmd, 0);
    if (ret) {
        // 生成新SN并写入
        current_num++;
		char *prefix = NULL;
		// 从ini读取SN前缀
		prefix = get_key_value(config, "SN_PREFIX");
		if (!prefix) {
			printf("Error: SN_PREFIX not found in config\n");
			return -4;
		}
		snprintf(sn, sizeof(sn), "%s%06d", prefix, current_num);
		printf("Generated SN: %s\n", sn);
		free(prefix); // 释放内存
        // 写回sn.dat
        memset(script_cmd, 0, sizeof(script_cmd));
        memcpy((char *)UBOOT_DOWN_ADDR, sn, SN_FILE_SIZE);
        sprintf(script_cmd, "fatwrite usb 0:1 0x%lx %s %d",
               (long)UBOOT_DOWN_ADDR, sn_dat, SN_FILE_SIZE);
        ret = run_command(script_cmd, 0);
		printf("run command:%s,ret=%d\n",script_cmd,ret);
        // 设置环境变量
		memset(script_cmd, 0, sizeof(script_cmd));
        sprintf(script_cmd, "setenv sn %s", sn);
        ret = run_command(script_cmd, 0);
		printf("run command:%s,ret=%d\n",script_cmd,ret);
        run_command("saveenv", 0);

    }else{
		printf("sn already existed,no need to write again\n");
	}

fail:
#if defined(CONFIG_LED_INDICATOR_FOR_FLASH)
    if (ret > 0) {
        led_control(LEDOFF);
    }
#endif
    return ret;
}
#endif // CONFIG_WRITE_SN
