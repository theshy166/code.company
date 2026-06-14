#include "mci.h"
#include "type_use.h"
#include "sstring.h"
#include "glb_reg.h"
#include "pkcs5.h"
#include "rsa.h"
#include "bignum.h"
#include "mc_hw_rsa.h"
#include "uart_drv.h"


void SET_REG_M(unsigned long addr, unsigned int value, unsigned int mask)
{
	unsigned int reg_value = 0;

	reg_value = *(volatile unsigned int *)addr;
	reg_value &= ~mask;
	reg_value |= value;
	*(volatile unsigned int *)addr = reg_value;
}


static  void write_rsa_param(unsigned int  addr,
	unsigned int  val)
{
	rsa_writel(addr, ((val)));

}

void rsa_enable(unsigned int clk_sel)
{
	unsigned int val;

	val = __raw_readl(CEN_PERI_CLK_CTRL);
	val &= ~CKG_RSA_EN;
	__raw_writel(val, CEN_PERI_CLK_CTRL);

	val = __raw_readl(CEN_PERI_APB_CLK_CTRL);
	val &= ~CKG_APB_RSA_EN;
	__raw_writel(val, CEN_PERI_APB_CLK_CTRL);

	mc_udelay(1);
	//for(i=0;i<0x1000;i++);

	val = __raw_readl(AP_PERI_SOFT_RST0);
	val |= RSA_SOFT_RST;
	__raw_writel(val, AP_PERI_SOFT_RST0);

	val = __raw_readl(CLK_AP_APB_RSA);
	val &= (~0x7);
	__raw_writel(val, CLK_AP_APB_RSA);
	
	val |= clk_sel;
	__raw_writel(val, CLK_AP_APB_RSA);
	
	mc_udelay(1);
	//for(i=0;i<0x1000;i++);

	val = __raw_readl(CEN_PERI_CLK_CTRL);
	val |= CKG_RSA_EN;
	__raw_writel(val, CEN_PERI_CLK_CTRL);

	val = __raw_readl(CEN_PERI_APB_CLK_CTRL);
	val |= CKG_APB_RSA_EN;
	__raw_writel(val, CEN_PERI_APB_CLK_CTRL);

	val = __raw_readl(AP_PERI_SOFT_RST0);
	val &= ~RSA_SOFT_RST;
	__raw_writel(val, AP_PERI_SOFT_RST0);
}

static void rsa_cfg_d(unsigned int *d, unsigned int n_len)
{
	unsigned int i = 0;
	unsigned int len = 0;

	if (n_len == RSA_2048_LEN)
		len = 64;
	else
		len = 128;

	for (i = 0; i < len; i++) {
		write_rsa_param(RSA_D_X(i), *d);
		d++;
	}
}

static void rsa_cfg_m(unsigned int *m, unsigned int n_len)
{
	unsigned int i = 0;
	unsigned int len = 0;

	if (n_len == RSA_2048_LEN)
		len = 64;
	else
		len = 128;

	for (i = 0; i < len; i++) {
		write_rsa_param(RSA_M_X(i), *m);
		m++;
	}
}

void rsa_cfg_n(unsigned   int  *n, unsigned int n_len)
{
	unsigned int i = 0;
	unsigned int len = 0;

	if (n_len == RSA_2048_LEN)
		len = 64;
	else
		len = 128;

	for (i = 0; i < len; i++) {
		write_rsa_param(RSA_N_X(i), *n);
		n++;
	}
}

static void parsersa_m_data(unsigned int *out, void *in,
	unsigned int insize)
{
	int i = 0;
	char *s = NULL;
	char *d = NULL;

	mmemset(out, 0, 512);
	if (insize < 64) {
		/* E */
		out[0] = (unsigned long long)(*(unsigned long *) in);
	} else {
		if ((insize & 0x3f) != 0)
			insize = (insize / 64 + 1)*64;

		i = 0;
		s = (char *)in + insize/8 - 1;
		d = (char *)out;
		for (i = 0; i < insize / 8; i++) {
			*d = *s;
			s--;
			d++;
		}
	}
}

static void parsersa_n_data(unsigned int *out, void *in,
	unsigned int insize)
{
	int i = 0;
	char *s = NULL;
	char *d = NULL;

	mmemset(out, 0, 512);
	if (insize < 64) {
		/* E */
		out[0] = (unsigned long long)(*(unsigned long *) in);
	} else {
		if ((insize & 0x3f) != 0)
			insize = (insize / 64 + 1) * 64;

		i = 0;
		s = in;
		d = (char *)out;
		for (i = 0; i < insize / 8; i++) {
			*d = *s;
			s++;
			d++;
		}
	}
}

static void write_rsa_result_data(unsigned long long *out, void *in,
	unsigned long insize)
{
	int i = 0;
	char *s = in;
	char *d = (char *)out;

	mmemset(out, 0, insize);

	for (i = 0; i < insize / 8; i++) {
		*d = *s;
		s++;
		d++;
	}
}

int rsa_start_workaround(u32 count)
{
	u32 start_second = 0;

	while (rsa_readl(RSA_DEBUG) == 0) {
		SET_REG_M(REG_RSA_BASE + RSA_CFG, 0, RSA_START);
		SET_REG_M(REG_RSA_BASE + RSA_CFG, RSA_START, RSA_START);
		start_second++;
		if (start_second > count)
			return false;
	}

	return true;
}

int hw_rsa_modexp(void *E, unsigned int Elen, void *N, unsigned int Nlen,
	void *input, void *output, u32 clk_timer)
{
	unsigned int val;
	u32 v;
	u32 t1, t2;
	unsigned int  *writebuf = NULL;
	u32 status = RSA_INT_STATUS;
	u32 rsa_timeout = 0;
	
	rsa_timeout = ((clk_timer >> 4) & 0x1FFF);
	rsa_timeout=500000;

	writebuf = (unsigned int *)mbedtls_calloc(
		4, 64);

	if ((Nlen != RSA_4096_LEN) && (Nlen != RSA_2048_LEN)) {
		uart_puts("NCF\r\n");
		return false;
	}

	rsa_writel(RSA_INT, 0x5);
	rsa_writel(RSA_INT, 0x1);
	rsa_writel(RSA_CFG, 0x0);

	if (Nlen == RSA_4096_LEN) {
		SET_REG_M(REG_RSA_BASE + RSA_CFG, 0x1, 0x1);
		rsa_writel(RSA_ESCAN_CFG0, RSA_CFG0_4096CFG);
	} else {
		SET_REG_M(REG_RSA_BASE + RSA_CFG, 0x0, 0x1);
		rsa_writel(RSA_ESCAN_CFG0, RSA_CFG0_2048CFG);
	}

	SET_REG_M(REG_RSA_BASE + RSA_ESCAN_CFG0, Elen<<16, 0xffff0000);

	val = rsa_readl(RSA_ESCAN_CFG1);
	val &= ~(E_SCAN_BIT_CNT_CFG_MASK | WBIT_OFFSET_MASK);
	val |= ((63 << E_SCAN_BIT_CNT_CFG_OFS) & E_SCAN_BIT_CNT_CFG_MASK);
	val |= ((1 << (6 - 1)) & WBIT_OFFSET_MASK);
	rsa_writel(RSA_ESCAN_CFG1, val);

	parsersa_n_data(writebuf, N, Nlen);
	rsa_cfg_n(writebuf, Nlen);

	parsersa_n_data(writebuf, E, Elen);
	rsa_cfg_d(writebuf, Nlen);

	parsersa_m_data(writebuf, input, Nlen);
	rsa_cfg_m(writebuf, Nlen);

	mbedtls_free(writebuf);

	SET_REG_M(REG_RSA_BASE + RSA_CFG, RSA_START, RSA_START);

	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		v = rsa_readl(RSA_INT);
		if ((v & status) == status) {
			val = rsa_readl(RSA_INT);
			val |= (INT_CLR_SW);
			rsa_writel(RSA_INT, val);

			val = rsa_readl(RSA_INT);
			val &= (~INT_CLR_SW);
			rsa_writel(RSA_INT, val);
			goto rsa_complete;
		}
	} while ((t2 - t1) < rsa_timeout);

	return false;

rsa_complete:
	write_rsa_result_data(output, (void *)(REG_RSA_BASE + RSA_RESULT0),
		Nlen);

	return true;
}
