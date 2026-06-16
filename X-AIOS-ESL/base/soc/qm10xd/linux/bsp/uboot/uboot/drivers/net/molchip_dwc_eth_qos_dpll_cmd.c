#include <common.h>
#include <command.h>
#include <net.h>
#include <wait_bit.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>
#include <malloc.h>
#include <dm.h>
#include <phy.h>
#include <linux/bitops.h>
#include <exports.h>

#include "molchip_dwc_eth_qos.h"


//#define CONFIG_ARP_TIMEOUT  500UL
//#define CONFIG_NET_RETRY_COUNT	2


#define TX_DELAY_NUM 256
#define RX_DELAY_NUM 256

enum gmac_scan_cmd {
	SCAN_TX_DELAY,
	SCAN_RX_DELAY,
	SCAN_ALL_DELAY
};

static int log_level = 0;

static int scan_result[TX_DELAY_NUM][RX_DELAY_NUM];
static int scan_step = 1;
static int scantx_base_val = 0;
static int scantx_end_val = TX_DELAY_NUM;
static int scanrx_base_val = 0;
static int scanrx_end_val = RX_DELAY_NUM;

/*****auto loopback scan****/
static int loopback_auto_scan = 0;
static int used_txdelay = 0;
static int used_rxdelay = 0;
/*****auto loopback scan***end***/

static int tx_replace2rxneg = 0;

static int one_row2column(int scan_result[][256], int m, int n)
{
        int j = 0;
	printf("transform one row to clolumn\n");
	printf("RXDLY\n");
        for(j = 0; j < scanrx_end_val; j += scan_step) {
              printf("0x%02x---0x%02x\n",j, scan_result[0][j]);
        }

	return 0;
}
static int calculation_delay_value(int scan_result[][256], int m, int n)
{
	int ret = 0;
	int i = 0;
        int j = 0;
	int head_i = 0;
	int used_i = -1;
	int start_i_count = 0;
	int tail_i = 0;

	int head_j = 0;
	int used_j = -1;
	int start_j_count = 0;
	int tail_j = 0;

        for (i = 0; i < scantx_end_val; i += scan_step) {

                for(j = 0; j < scanrx_end_val; j += scan_step) {
			if (scan_result[i][j] == 0x11) {

				if (!start_j_count && (scan_result[i][j + scan_step] == 0x11)  && (scan_result[i][j + 2 * scan_step] == 0x11) )  {
					start_j_count = 1;
					head_j = j;
					debug("head_j:%x\n",head_j);
				}

			} else if (start_j_count && scan_result[i][j] == 0x00) {
				start_j_count = 0;
				//tail_j = (j - scan_step);
				tail_j = j;
				debug("tail_j:%x\n",tail_j);
				used_j = ((tail_j + head_j) / 2);
				debug("used_j%x\n",used_j);
				goto j_done;
			}
                }

        }
j_done:
	j = 0;
	//used_j = ALIGN(used_j, scan_step);
	if ( (m > 0) && (n == 0)) {
		used_j = 0;

	} else {
		used_j = (used_j/scan_step) * scan_step;
	}

        for (i = 0; i < scantx_end_val; i += scan_step) {
		if (scan_result[i][used_j] == 0x11) {

			if (!start_i_count && (scan_result[i+scan_step][used_j] == 0x11)  && (scan_result[i + 2 * scan_step][used_j] == 0x11) )  {
				start_i_count = 1;
				head_i = i;
				debug("head_i:%x\n",head_i);
			}

		} else if (start_i_count && scan_result[i][used_j] == 0x00) {
			start_i_count = 0;
			//tail_i = (i - scan_step);
			tail_i = i;
			debug("tail_i:%x\n",tail_i);
			used_i = ((tail_i + head_i) / 2);
			debug("used_i%x\n",used_i);
			goto i_done;

		}

	}
i_done:
	//used_i = ALIGN(used_i, scan_step);
	if ( (m == 0) && (n > 0)) {
		used_i = 0;
	} else {
		used_i = (used_i/scan_step) * scan_step;
	}
	if ( (m == 0) && (n > 0)) {
		printf("recommand rxdelay:0x%x\n", used_j);

	} else if ((m > 0) && (n == 0)) {
		printf("recommand txdelay:0x%x\n", used_i);
	} else {
		printf("recommand txdelay:0x%x  rxdelay:0x%x\n",used_i, used_j);
		used_txdelay = used_i;
		used_rxdelay = used_j;
	}

	if (loopback_auto_scan == 1) {
		char str[17];
		tail_i += scan_step;
		tail_j += scan_step;
		debug("tx_base_val:0x%x\n",head_i);
		debug("tx_end_val:0x%x\n",tail_i);
		debug("rx_base_val:0x%x\n",head_j);
		debug("rx_end_val:0x%x\n",tail_j);
		sprintf(str, "0x%x", head_i);
		env_set("scantx_base_val", str);
		sprintf(str, "0x%x", tail_i);
		env_set("scantx_end_val", str);
		sprintf(str, "0x%x", head_j);
		env_set("scanrx_base_val", str);
		sprintf(str, "0x%x", tail_j);
		env_set("scanrx_end_val", str);
		//scantx_base_val = head_i;
		//scantx_end_val = tail_i;
		//scanrx_base_val = head_j;
		//scanrx_end_val = tail_j;
	}

	return ret;
}
static int print_scan_result(int scan_result[][256], int m, int n)
{
	int i = 0;
        int j = 0;

	//printf("      ");
	printf("RXDLY ");
        for(j = 0; j < scanrx_end_val; j += scan_step) {
              printf(" 0x%02x",j);
        }

        printf("\n");
        printf("      ");

        for(j = 0; j < scanrx_end_val; j += scan_step) {
              printf(" ----");
        }
        printf("\n");
		if (tx_replace2rxneg == 0x0)
        	printf("TXDLY \n");
		else
			printf("NEGrxDLY \n");
        for (i = 0; i < scantx_end_val; i += scan_step) {
                printf(" 0x%02x|",i);
                for(j = 0; j < scanrx_end_val; j += scan_step) {
                        printf(" 0x%02x",scan_result[i][j]);
                }

                printf("\n");

        }
	printf("m:%d--n:%d\n",m,n);
	if (((m == 0 )|| (m == 1)) && (n != 0)) {
		one_row2column(scan_result, m, n);	
	}

	return 0;
}
/******scan function defined******/
static char ipaddr[16];
extern struct in_addr net_ping_ip;
static int ping_test(void)
{
	if (net_ping_ip.s_addr == 0)
		return CMD_RET_USAGE;

	if (net_loop(PING) < 0) {
		printf("ping failed; host %s is not alive\n", ipaddr);
		return CMD_RET_FAILURE;
	}
	printf("host %s is alive\n", ipaddr);
	return CMD_RET_SUCCESS;
}


static int loop_scan = 0;
static int init_eth = 0;
static void *scan_tx_buf = NULL;
static uchar *scan_rx_buf = NULL;
//static uchar tx_buf[1500];
//static uchar rx_buf[1500];
//static void *scan_tx_buf = tx_buf;
//static uchar *scan_rx_buf = rx_buf;
static uchar prbs7[20] = {0xfb,0x38,0x6a,0x45,0xfc,0x08,0x30,0xa3,0xc8,0xb3,0xa9,0xf4,0x38,0x93,0x6b,0x7b,0x1a,0x5d,0xcc};
static int scan_buf_length = 1500;
static struct udevice *current_eth;
extern int qos_send(struct udevice *dev, void *packet, int length);
extern int qos_recv(struct udevice *dev, int flags, uchar **packetp);
extern int qos_free_pkt(struct udevice *dev, uchar *packet, int length);
static int check_recv(uchar *tx_buf, uchar *rx_buf, int delay_val)
{
	int i;
	int ret;
	for (i = 0; i < scan_buf_length; i ++) {
		if (tx_buf[i] != rx_buf[i]) {
			ret = CMD_RET_FAILURE;
			break;
		}
	}

	ret = CMD_RET_SUCCESS;

	return ret;
}

extern int change_loopback(struct udevice *dev, int switchon);
extern int miiphy_set_current_dev(const char *devname);
static int init_gmac(void)
{
	int ret;
	int i = 0;
	//if (!init_eth) {
		init_eth = 1;
		eth_halt();
		eth_set_current();

		ret = eth_init();
		if (ret < 0) {
			printf("miku %s %d\n",__func__, __LINE__);
			return -2;
		}
		current_eth = eth_get_dev();

		if (!current_eth) {
			printf("miku %s %d\n",__func__, __LINE__);
			return -2;
		}

		if (!eth_is_active(current_eth)) {
			printf("miku %s %d\n",__func__, __LINE__);
			return -2;
		}
		if (loop_scan) {
			if (scan_tx_buf == NULL) {
				scan_tx_buf = (void *)malloc_simple(scan_buf_length);
				for (i = 0; i < scan_buf_length/20; i ++)
					memcpy((scan_tx_buf + i * 20), prbs7, sizeof(prbs7));
				memset(scan_tx_buf, 0xff, 14);
			}
			if (!scan_tx_buf) {
				printf("miku %s %d\n",__func__, __LINE__);
				return -2;
			}
			debug("scan_tx_buf:%p\n",scan_tx_buf);
			if (scan_rx_buf == NULL)
				scan_rx_buf = (uchar *)malloc_simple(scan_buf_length);
			if (!scan_rx_buf) {
				printf("miku %s %d\n",__func__, __LINE__);
				return -2;
			}
			debug("scan_rx_buf:%p\n",scan_rx_buf);
		}
		//udelay(10000);
	//}


	return 0;
}

static int loop_send_test(int delay_val)
{
	int flags;
	int i = 0;
	int ret = CMD_RET_SUCCESS;

	//memset(scan_tx_buf, delay_val, scan_buf_length);
	//memset(scan_tx_buf, 0xff, 14);
	*((unsigned char *)scan_tx_buf  + 15)= (unsigned char)delay_val;
	ret = qos_send(current_eth, scan_tx_buf, scan_buf_length);
	if (ret < 0) {
		debug(" %s %d\n",__func__, __LINE__);
		ret = CMD_RET_FAILURE;
		return ret;
	}
	flags = ETH_RECV_CHECK_DEVICE;

	//for (i = 0; i < 1000000; i++) {
	for (i = 0; i < 20; i++) {
		ret = qos_recv(current_eth, flags, &scan_rx_buf);
		if (ret > 0)
			break;
		udelay(1);
	}

	if (i >= 20) {
		ret = CMD_RET_FAILURE;
		debug("recv timeout\n");
	}

	/*
	for (i = 0; i < 1000000; i++) {
		udelay(1);
	}
	ret = qos_recv(current_eth, flags, &scan_rx_buf);
	*/

	debug("recv %d\n",ret);
	if (ret < scan_buf_length) {
		debug("%s %d\n",__func__, __LINE__);
		ret = CMD_RET_FAILURE;
		return ret;
	}
	if (ret > 0)
		ret = check_recv(scan_tx_buf,scan_rx_buf,delay_val);
	if (ret == CMD_RET_SUCCESS) {
		debug("check ok\n");
	} else {
		debug("check nok\n");
	}

	qos_free_pkt(current_eth, scan_rx_buf, scan_buf_length);

	return ret;
}


static int scan_end(void)
{
#if 0
	if (scan_tx_buf)
		free(scan_tx_buf);
	if (scan_rx_buf)
		free(scan_rx_buf);

	eth_halt();
#endif
#if 0
	if (loop_scan == 1) {
		eth_halt();
		loop_scan = 0;
		env_set("gmac_loopback", "");
		loopback_auto_scan = 0;

		puts("end loop mode scan\n");
	}
#endif
	return CMD_RET_SUCCESS;
}

/******scan function defined end******/

static int config_eth_txdelay(int value)
{
	struct qos_priv *eqos = dev_get_priv(current_eth);
	if (log_level > 2)
		printf("%s--%d\n",__func__,value);
	
	if ((eqos->dpll_addr_map.txdpll_base != FDT_ADDR_T_NONE) && (value >= 0) 
		&& (value <= 0xff)) {
		
		writel(0x19,REG_GMAC_TXDLL_CTRL0);
		writel(value,REG_GMAC_TXDLL_CTRL2);
		writel(0x6a04,REG_GMAC_TXDLL_CTRL1);
		writel(0x4a04,REG_GMAC_TXDLL_CTRL1);
	} else {
		printf("%s skip gmac txdelay config--val:%x\n",current_eth->name, value);
		debug("\n%s: config GMAC dpll txdpll_base:0x%lx,rxdpll_base:0x%lx,eco0_base:0x%lx,eco1_base:0x%lx\n",
			__func__, eqos->dpll_addr_map.txdpll_base, eqos->dpll_addr_map.rxdpll_base,
			eqos->dpll_addr_map.eco0_base, eqos->dpll_addr_map.eco1_base);
	}

	return 0;
}

/*************bit ops*************/
/*****bit_check_and_modify <set_reg> <bit_idx(0-31)> <check_reg> <bit_check_idx(0-31)****/

static int bit_check_and_modify(unsigned int* modify_val, int bit_idx,
	unsigned int * check_reg, int bit_check_idx)
{

	if (*check_reg & (0x1 << bit_check_idx)) {
			//set bit
			*modify_val |= (0x1 << bit_idx);
	} else {
			//clr bit
			*modify_val &= ~(0x1 << bit_idx);
	}

	return 0;
}


static int config_eth_rxnegdelay(int value)
{
	struct qos_priv *eqos = dev_get_priv(current_eth);
	unsigned int rdata0 = 0x0;
	unsigned int rdata1 = 0x0;
	unsigned int checkval = 0x0;
	
	value &= GENMASK(7, 0);
	rdata0 = readl(REG_GMAC_RXDLL_ECO1);
	rdata1 = readl(REG_GMAC_RXDLL_ECO0);
	
	if (eqos->ctrl_id == 0) {
		checkval = readl(REG_GMAC_RXDLL_CTRL0);
		rdata0 &= ~(0xff << 4);
		rdata0 |= (value << 4);
		bit_check_and_modify(&rdata1,0,&checkval,4);
		bit_check_and_modify(&rdata0,0,&checkval,3);
		bit_check_and_modify(&rdata1,1,&checkval,2);
		bit_check_and_modify(&rdata0,1,&checkval,1);
		bit_check_and_modify(&rdata1,2,&checkval,0);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);

		//0x6a04
		checkval=0x6a04;
		bit_check_and_modify(&rdata0,2,&checkval,11);
		bit_check_and_modify(&rdata1,3,&checkval,12);
		bit_check_and_modify(&rdata0,3,&checkval,13);
		bit_check_and_modify(&rdata1,4,&checkval,14);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);
		//0x4a04
		checkval=0x4a04;
		bit_check_and_modify(&rdata0,2,&checkval,11);
		bit_check_and_modify(&rdata1,3,&checkval,12);
		bit_check_and_modify(&rdata0,3,&checkval,13);
		bit_check_and_modify(&rdata1,4,&checkval,14);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);
	} else if (eqos->ctrl_id == 1) {
		checkval = readl(REG_GMAC_RXDLL_CTRL0);
		rdata0 &= ~(0xff << 16);
		rdata0 |= (value << 16);
		bit_check_and_modify(&rdata1,5,&checkval,4);
		bit_check_and_modify(&rdata0,12,&checkval,3);
		bit_check_and_modify(&rdata1,6,&checkval,2);
		bit_check_and_modify(&rdata0,13,&checkval,1);
		bit_check_and_modify(&rdata1,7,&checkval,0);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);
	
		//0x6a04
		checkval=0x6a04;
		bit_check_and_modify(&rdata0,14,&checkval,11);
		bit_check_and_modify(&rdata1,8,&checkval,12);
		bit_check_and_modify(&rdata0,15,&checkval,13);
		bit_check_and_modify(&rdata1,9,&checkval,14);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);
		//0x4a04
		checkval=0x4a04;
		bit_check_and_modify(&rdata0,14,&checkval,11);
		bit_check_and_modify(&rdata1,8,&checkval,12);
		bit_check_and_modify(&rdata0,15,&checkval,13);
		bit_check_and_modify(&rdata1,9,&checkval,14);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);

	} else {
		printf("error gmac ctrl_id!!!\n");
	}

	return 0;
}

static int config_eth_rxdelay(int value)
{
	struct qos_priv *eqos = dev_get_priv(current_eth);
	
	if (log_level > 2)
		printf("%s--%d\n",__func__,value);
	
	
	if ((eqos->dpll_addr_map.rxdpll_base != FDT_ADDR_T_NONE) && (value >= 0) 
		&& (value <= 0xff)) {
		
		writel(0x19,REG_GMAC_RXDLL_CTRL0);
		if (eqos->dpll_addr_map.dll_addr_is_fixup == true)
			writel(value, 0x34400400);
		else
			writel(value,REG_GMAC_RXDLL_CTRL2);
		writel(0x6a04,REG_GMAC_RXDLL_CTRL1);
		writel(0x4a04,REG_GMAC_RXDLL_CTRL1);

	} else {
		printf("%s skip gmac rxdelay config--val:%x\n",current_eth->name,value);
		printf("\n%s: config GMAC dpll txdpll_base:0x%lx,rxdpll_base:0x%lx,eco0_base:0x%lx,eco1_base:0x%lx\n",
			__func__, eqos->dpll_addr_map.txdpll_base, eqos->dpll_addr_map.rxdpll_base,
			eqos->dpll_addr_map.eco0_base, eqos->dpll_addr_map.eco1_base);
	}


	return 0;
}


static int scan_function(int ethx, int cmd)
{
	int i = 0;
	int j = 0;
	int ret = -1;
	int m = 0;
	int n = 0;
	char *s;

	switch (cmd) {
		case SCAN_TX_DELAY:
		      scanrx_base_val = 0;
		      scanrx_end_val = 1;
		      scantx_base_val = 0;
		      scantx_end_val = TX_DELAY_NUM;
		      break;	     
		case SCAN_RX_DELAY:
		      scantx_base_val = 0;
		      scantx_end_val = 1;
		      scanrx_base_val = 0;
		      scanrx_end_val = RX_DELAY_NUM;
		      break;
		case SCAN_ALL_DELAY:
		default:
		      scanrx_base_val = 0;
		      scanrx_end_val = RX_DELAY_NUM;
		      scantx_base_val = 0;
		      scantx_end_val = TX_DELAY_NUM;
		      break;
	
	}

	s = env_get("scantx_base_val");
	if (s != NULL) {
		scantx_base_val = simple_strtoul(s, NULL, 16);	
		if (scantx_base_val > 255 || scantx_base_val < 0)
			scantx_base_val = 0;
	}

	s = env_get("scantx_end_val");
	if (s != NULL) {
		scantx_end_val = simple_strtoul(s, NULL, 16);	
		if (scantx_end_val > 255 || scantx_end_val < 0)
			scantx_end_val = 255;
	}

	s = env_get("scanrx_base_val");
	if (s != NULL) {
		scanrx_base_val = simple_strtoul(s, NULL, 16);	
		if (scanrx_base_val > 255 || scanrx_base_val < 0)
			scanrx_base_val = 0;
	}

	s = env_get("scanrx_end_val");
	if (s != NULL) {
		scanrx_end_val = simple_strtoul(s, NULL, 16);	
		if (scanrx_end_val > 255 || scanrx_end_val < 0)
			scanrx_end_val = 255;
	}
		debug("wwtx_base_val:0x%x\n",scantx_base_val);
		debug("wwtx_end_val:0x%x\n",scantx_end_val);
		debug("wwrx_base_val:0x%x\n",scanrx_base_val);
		debug("wwrx_end_val:0x%x\n",scanrx_end_val);

	for (i = scantx_base_val; i < scantx_end_val; i += scan_step) {

		if ( (cmd == SCAN_ALL_DELAY) || (cmd == SCAN_TX_DELAY)) {

			if ((ethx == 0) || (ethx == 1) ) {
				if (tx_replace2rxneg == 0x0)
					config_eth_txdelay(i);
				else
					config_eth_rxnegdelay(i);
			} else {
				printf("err ethx:%d\n",ethx);	
			}

		}


		for(j = scanrx_base_val; j < scanrx_end_val; j += scan_step) {

			if ( (cmd == SCAN_ALL_DELAY) || (cmd == SCAN_RX_DELAY)) {
				if ((ethx == 0) || (ethx == 1) ) {
					config_eth_rxdelay(j);		
				} else {
					printf("err ethx:%d\n",ethx);	
				}
			}
			//mdelay(500);
			if (loop_scan) {
				ret = loop_send_test(j);

			} else {
				ret = ping_test();
			}
			if (ret == CMD_RET_SUCCESS) {

				scan_result[i][j] = 0x11;	

			} else if (ret == CMD_RET_FAILURE) {

				scan_result[i][j] = 0x00;	
			
			} else {
			
				scan_result[i][j] = 0xff;	
			}

			
			/*
			 *	Abort if ctrl-c was pressed.
		 	*/
			if (ctrlc()) {
				puts("\nAbort\n");
				goto done;
			}

		
		}

	
	}
done:
	m = (scantx_end_val - scantx_base_val)/scan_step;
	n = (scanrx_end_val - scanrx_base_val)/scan_step;
	//printf("scantx_end_val:%d--scantx_base_val:%d--scan_step:%d\n",scantx_end_val,scantx_base_val,scan_step);
	//printf("--m:%d--n:%d\n",m,n);
	if (log_level > 0)
		print_scan_result(scan_result, m, n);

	calculation_delay_value(scan_result, m, n);

	scan_end();

	return 0;
}
extern struct record_eth record_eths[4];
int gmac_loopback_autoscan(struct udevice *dev, int *tx_delay, int *rx_delay)
{
	int ethx = 0;
	int cmd = 2;
	int ret = -1;
	struct qos_priv *eqos = dev_get_priv(dev);
	if (eqos == NULL){
		puts("failed get netdevice\n");
		goto err_auto_scan;
	}
	loop_scan = 1;
	loopback_auto_scan = 1;
	scan_step = 5;
	env_set("gmac_loopback", "1");
	env_set("ethrotate", "no");
	puts("loop mode scan\n");

	ethx = eqos->ctrl_id;
	env_set("ethact", dev->name);
	miiphy_set_current_dev(dev->name);

	init_eth = 0;
	ret = init_gmac();
	if (ret < 0) {
		puts("failed init_gmac\n");
		goto err_auto_scan;
	}

	ret = change_loopback(current_eth, 1);
	if (ret < 0) {
		puts("failed change_loopback\n");
		goto err_auto_scan;
	}
	
	scantx_base_val = 0;
	scantx_end_val = 128;
	scanrx_base_val = 0;
	scanrx_end_val = 128;

	scan_function(ethx,cmd);
	scan_step = 1;
	scan_function(ethx,cmd);

	/*restore*/
	change_loopback(current_eth, 0);
	scantx_base_val = 0;
	scantx_end_val = TX_DELAY_NUM;
	scanrx_base_val = 0;
	scanrx_end_val = RX_DELAY_NUM;
	env_set("scantx_base_val", "");
	env_set("scantx_end_val", "");
	env_set("scanrx_base_val", "");
	env_set("scanrx_end_val", "");
	eth_halt();
	loop_scan = 0;
	loopback_auto_scan = 0;
	env_set("gmac_loopback", "");
	env_set("ethrotate", "");
	env_set("ethact", (record_eths[0].dev)->name);
	miiphy_set_current_dev((record_eths[0].dev)->name);
	
	
	puts("end loop mode scan\n");


	/*get used delay*/
	*tx_delay = used_txdelay;
	*rx_delay = used_rxdelay;

	return 0;

err_auto_scan:
	return -1;
}
// gmac_scan ipaddr eth0|eth1 tx|rx|all step
static int do_dpll_scan(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ethx = 0;
	int cmd = 0;
	char *s;
	log_level = 1;
	int ret = 0;
	//gmac_loopback_autoscan();
	//return 0;
	if (argc < 5)
		return CMD_RET_USAGE;
	if (!strncmp("loop", argv[1], 4)) {
		loop_scan = 1;
		env_set("gmac_loopback", "1");

		puts("loop mode scan\n");

	}else {
		copy_filename(ipaddr, argv[1], sizeof(ipaddr));
		net_ping_ip = string_to_ip(ipaddr);
		if (net_ping_ip.s_addr == 0)
			return CMD_RET_USAGE;
	}

	if (!strncmp(argv[2],"eth0",4)) {
		ethx = 0;	
		env_set("ethact", (record_eths[0].dev)->name);
		miiphy_set_current_dev((record_eths[0].dev)->name);

	} else if (!strncmp(argv[2],"eth1",4)) {
		ethx = 1;	
		env_set("ethact", (record_eths[1].dev)->name);
		miiphy_set_current_dev((record_eths[1].dev)->name);

	} else {
		return CMD_RET_USAGE;
	}

	if (!strncmp(argv[3],"tx",2)) {
		cmd = SCAN_TX_DELAY;	
	} else if (!strncmp(argv[3], "rx", 2)) {
		cmd = SCAN_RX_DELAY;
	} else {
		cmd = SCAN_ALL_DELAY;
	}

	scan_step = simple_strtoul(argv[4], NULL, 10);
	if (scan_step > 255 || scan_step <= 0)
		scan_step = 1;

	//if (loop_scan) {
		init_gmac();
	//}
	
	s = env_get("tx_replace2rxneg");
	if (s != NULL) {
		tx_replace2rxneg = simple_strtoul(s, NULL, 16);	
	}
	
	ret = change_loopback(current_eth, 1);
	if (ret < 0) {
		puts("failed change_loopback\n");
	}

	scan_function(ethx,cmd);
	
	ret = change_loopback(current_eth, 0);
	if (ret < 0) {
		puts("failed change_loopback\n");
	}
	
	log_level = 0;
	
	return 0;
}

U_BOOT_CMD(
	gmac_dpll_scan,	5,	0,	do_dpll_scan,
	"net GMAC scan tx/rx delay ",
	"serverip|loop eth0|eth1 tx|rx|all step"
);
