/*
 * (C) Copyright 2019
 * Molchip <www.molchip.com>
 * Written-by: Jason Qin <Jason.qin@molchip.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <config.h>
#include <common.h>
#include <linux/bitops.h>
#include <phy.h>
#include <miiphy.h>
#include <asm/mach-types.h>
#include <asm/io.h>
#include <command.h>
#include <net.h>
#include "../molchip_dwc_eth_qos.h"

__attribute__((aligned(32))) static unsigned char send_packet[1024] = {0};

extern struct record_eth record_eths[4];

static struct udevice *current_eth;
extern int qos_send(struct udevice *dev, void *packet, int length);
extern int qos_recv(struct udevice *dev, int flags, uchar **packetp);
extern int qos_free_pkt(struct udevice *dev, uchar *packet, int length);
unsigned long int next = 1;
unsigned int  test_rand(void){
	next = next * 1103515245 + 12345;
	return (unsigned int)(next/65535)%32768;
}

void test_srand(unsigned long int seed){
	next = seed;
}

void memset_int(u32 * buf, int data, size_t size)
{
	int i = 0;
	for(i=0; i < (size/4); i ++) {
		*(buf + i) = data;
	}
	
	return;
}
void construct_tx_frame(unsigned char *buf, int size, int data,int random_seed)
{
	int i = 0;
	unsigned char *buf_addr;
	buf_addr = buf;
	if(data == 0xffff){
		//printf("set random data, buf add is %x\n",(unsigned int)buf);
		printf("set random data\n");
		while(i < size){
			test_srand(random_seed + i + data);
			//printf("--buf%x\n",test_rand());
			memset(buf++, (unsigned char)test_rand() , 1);
			i++;
		}
	}else if ((u32)data <= 0xff) {
			printf("one byte set all to 0x%x\n",data);
			memset(buf, data , size);
	} else {
			printf("one int set all to 0x%x\n",data);
			memset_int((u32 *)buf, data , size);
	}
	printf("current packet 1st data:0x%x\n", (*(unsigned int *)buf_addr));
}

extern int mochip_ephy_test(struct phy_device *phydev);
int do_net_send(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int speed;
	int duplex;
	int loop_size;
	int data;
	char *endp;
    uchar *buf_send = send_packet;
	struct qos_priv *gmac;
	int ret;
	int cnt = 1;

	if (argc != 6) {
		cmd_usage(cmdtp);
		return -1;
	}

	if (!strncmp(argv[1],"eth0",4)) {

		printf("miku record_eths:%s\n",(record_eths[0].dev)->name);
		env_set("ethact", (record_eths[0].dev)->name);

		miiphy_set_current_dev((record_eths[0].dev)->name);


    } else if (!strncmp(argv[1],"eth1",4)) {
		env_set("ethact", (record_eths[1].dev)->name);
		miiphy_set_current_dev((record_eths[1].dev)->name);
	} else {
		return CMD_RET_USAGE;
	}



	eth_halt();

	eth_set_current();

#if 0	
	ret = eth_init();
	if (ret < 0) {
		printf("miku %s %d\n",__func__, __LINE__);
		return -2;
	}
#endif
	current_eth = eth_get_dev();

	if (!current_eth) {
		printf("miku %s %d\n",__func__, __LINE__);
		return -2;
	} else {

		printf("miku current eth: %s \n",current_eth->name);

	}
#if 0	
	if (!eth_is_active(current_eth)) {
			printf("miku %s %d\n",__func__, __LINE__);
			return -2;
	}
#endif	
	gmac = dev_get_priv(current_eth);

	gmac->ephy_test_pattern = 1;

	speed = simple_strtoul(argv[2], &endp, 0);
	duplex = simple_strtoul(argv[3], &endp, 0);
	loop_size = simple_strtoul(argv[4], &endp, 0);
	data = simple_strtoul(argv[5], &endp, 16);

	if (speed == 3) {
		printf("miku-- %s%s : loopsize:%d: data: %x\n", "auto",  "auto", loop_size,data);		
	}else if (speed == 2) {
		printf("miku-- %s%s : loopsize:%d: data: %x\n", "1000M", duplex == 1 ? "_duplex_full" : "_duplex_half", loop_size,data);
	} else 
		printf("miku-- %s%s : loopsize:%d: data: %x\n",speed == 1 ? "100M" : "10M", duplex == 1 ? "_duplex_full" : "_duplex_half", loop_size,data);
	printf("sendpkg addr:%p\n",send_packet);
	construct_tx_frame(send_packet,sizeof(send_packet),data,loop_size);
	if (gmac->phy == NULL) {
		printf("miku need create new phydev\n");
	}

	if (gmac->phy_type == INTERNAL_PHY) {
		gmac->autoneg = AUTONEG_DISABLE;
		gmac->duplex = (duplex == 1 ? DUPLEX_FULL : DUPLEX_HALF);
		gmac->speed = (speed == 1 ? SPEED_100 : SPEED_10);
		debug("mac Qos speed:%d\n",gmac->speed);
	}

	if (gmac->phy_type == EXTERNAL_PHY) {
		if (speed == 3) {
			gmac->autoneg = AUTONEG_ENABLE;
			gmac->duplex = DUPLEX_FULL;
			gmac->speed = SPEED_1000;
		} else if (speed == 2) {
			gmac->autoneg = AUTONEG_DISABLE;
			gmac->duplex =  (duplex == 1 ? DUPLEX_FULL : DUPLEX_HALF);
			gmac->speed = SPEED_1000;
		} else {
			gmac->autoneg = AUTONEG_DISABLE;
			gmac->duplex = (duplex == 1 ? DUPLEX_FULL : DUPLEX_HALF);
			gmac->speed = (speed == 1 ? SPEED_100 : SPEED_10);
		}
	}

#if 1
	ret = eth_init();
	if (ret < 0) {
			printf("miku %s %d\n",__func__, __LINE__);
			return -2;
	}
#endif
	while(loop_size--){
		if(data == 0xff) {
			buf_send[0] = (char)loop_size;
		}

		if (data == 0xffff)
		{
			construct_tx_frame(send_packet,sizeof(send_packet),data,cnt);
			eth_send(send_packet, sizeof(send_packet));
			cnt++;
		}else {
			eth_send(send_packet, sizeof(send_packet));
		}
	}
	printf("send done...\n");
	return 0;
}


U_BOOT_CMD(
	net_send,	6,	1,	do_net_send,
	"usage:qos send  frame\n",
	" netport speed duplex loop_times send_data (abort: ctl + c)\n"
	"netport:eth0/eth1\n"
	"speed: 3--auto 2--1000M 1--100M 0--10M\n"
	"duplex: 1--full 0--half\n"
	"loop_times: loop count number,e.g 8888\n"
	"send_data: the data to be send, special data,\n"
	"0xff--send all 1\n"
	"0xffff--send random data\n"
	"0x84218421--send 0x84218421\n"
	"e.g: net_send eth0 3 1 8888 0x0f0f0f0f\n"
	"net_send eth0 3 1 8888 0x84218421\n"
	"net_send eth0 0 1 8888 0xff\n"
	"net_send eth0 0 1 8888 0xffff\n"
);

