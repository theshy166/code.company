#ifndef _MOL_HW_RSA_H_
#define _MOL_HW_RSA_H_

#define RSA_CFG				0x0
#define DMA_REQ_CFG1			0x4
#define RSA_DEBUG			0x8
#define RSA_INT				0xc
#define RSA_D				0x210
#define RSA_D_X(x)			(RSA_D + ((x) << 2))
#define RSA_N				0x10
#define RSA_N_X(x)			(RSA_N + ((x) << 2))
#define RSA_M				0x410
#define RSA_M_X(x)			(RSA_M + ((x) << 2))
#define RSA_RESULT0			(0x610)
#define RSA_RESULT(x)			(RSA_RESULT0 + ((x) << 2))
#define RSA_BIGDIV_CFG			0x810
#define RSA_ESCAN_CFG0			0x814
#define RSA_ESCAN_CFG1			0x818

#define RSA_MODE			BIT(0)
#define RSA_DMA_EN			BIT(1)
#define RSA_TYPE			BIT(2)
#define RSA_START			BIT(3)
#define RSA_BUS_SEL_OFS			4
#define RSA_M_SIGN			BIT(7)
#define RSA_4096			(1 << 0)
#define RSA_2048			(0 << 0)
#define RSA_ENCRY			(1 << 2)
#define RSA_DECRY			(0 << 2)
#define RSA_M_NEGTIVE			(1 << 7)
#define RSA_M_ACTIVE			(0 << 7)

#define RSA_2048_LEN			2048
#define RSA_4096_LEN			4096
#define RSA_CFG0_2048CFG		0x11301F
#define RSA_CFG0_4096CFG		0x11303F

#define INT_CLR_SW			BIT(2)
#define RSA_INT_STATUS			BIT(4)

#define DIVISOR_SHIFT_CNT_CFG_OFS	0
#define DIVISOR_SHIFT_CNT_CFG_MASK	0x0000003f

#define EBIT_LEN_CNT_CFG_OFS		0
#define EBIT_LEN_CNT_CFG_MASK		0x0000003f
#define BIT_LEN_OFS			6
#define BIT_LEN_MASK			0x00001fc0
#define EBIT_LEN_OFS			16
#define EBIT_LEN_MASK			0x7fff0000

#define WBITS_OFFSET_OFS		0
#define WBIT_OFFSET_MASK		0x0000007f
#define E_SCAN_BIT_CNT_CFG_OFS		16
#define E_SCAN_BIT_CNT_CFG_MASK		0x003f0000

#define RSA_WAIT_COMPLETE_TIME_OUT	15000

#define GET_REG(x)			(*((volatile unsigned int *)(x)))

#define rsa_writel(x, v)		\
	__raw_writel(v, (REG_RSA_BASE + x))
#define rsa_readl(x)			\
	__raw_readl((REG_RSA_BASE + x))

int hw_rsa_modexp(void *E, unsigned int Elen, void *N, unsigned int Nlen,
	void *input, void *output, u32 clk_timer);
//void rsa_cfg_n(unsigned long long *n, unsigned int n_len);
void rsa_enable(unsigned int clk_sel);

#endif
