/*
 * i2c.h
 *
 *  Created on: 2018��9��29��
 *      Author: jason.qin
 */

#ifndef MODULES_I2C_H_
#define MODULES_I2C_H_

/* To determine what functionality is present */
#define I2C_FUNC_I2C			0x00000001
#define I2C_FUNC_10BIT_ADDR		0x00000002
#define I2C_FUNC_PROTOCOL_MANGLING	0x00000004 /* I2C_M_IGNORE_NAK etc. */
#define I2C_FUNC_SMBUS_PEC		0x00000008
#define I2C_FUNC_NOSTART		0x00000010 /* I2C_M_NOSTART */
#define I2C_FUNC_SLAVE			0x00000020
#define I2C_FUNC_SMBUS_BLOCK_PROC_CALL	0x00008000 /* SMBus 2.0 */
#define I2C_FUNC_SMBUS_QUICK		0x00010000
#define I2C_FUNC_SMBUS_READ_BYTE	0x00020000
#define I2C_FUNC_SMBUS_WRITE_BYTE	0x00040000
#define I2C_FUNC_SMBUS_READ_BYTE_DATA	0x00080000
#define I2C_FUNC_SMBUS_WRITE_BYTE_DATA	0x00100000
#define I2C_FUNC_SMBUS_READ_WORD_DATA	0x00200000
#define I2C_FUNC_SMBUS_WRITE_WORD_DATA	0x00400000
#define I2C_FUNC_SMBUS_PROC_CALL	0x00800000
#define I2C_FUNC_SMBUS_READ_BLOCK_DATA	0x01000000
#define I2C_FUNC_SMBUS_WRITE_BLOCK_DATA 0x02000000
#define I2C_FUNC_SMBUS_READ_I2C_BLOCK	0x04000000 /* I2C-like block xfer  */
#define I2C_FUNC_SMBUS_WRITE_I2C_BLOCK	0x08000000 /* w/ 1-byte reg. addr. */
#define I2C_FUNC_SMBUS_HOST_NOTIFY	0x10000000

#define I2C_FUNC_SMBUS_BYTE		(I2C_FUNC_SMBUS_READ_BYTE | \
					 I2C_FUNC_SMBUS_WRITE_BYTE)
#define I2C_FUNC_SMBUS_BYTE_DATA	(I2C_FUNC_SMBUS_READ_BYTE_DATA | \
					 I2C_FUNC_SMBUS_WRITE_BYTE_DATA)
#define I2C_FUNC_SMBUS_WORD_DATA	(I2C_FUNC_SMBUS_READ_WORD_DATA | \
					 I2C_FUNC_SMBUS_WRITE_WORD_DATA)
#define I2C_FUNC_SMBUS_BLOCK_DATA	(I2C_FUNC_SMBUS_READ_BLOCK_DATA | \
					 I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)
#define I2C_FUNC_SMBUS_I2C_BLOCK	(I2C_FUNC_SMBUS_READ_I2C_BLOCK | \
					 I2C_FUNC_SMBUS_WRITE_I2C_BLOCK)

#define I2C_FUNC_SMBUS_EMUL		(I2C_FUNC_SMBUS_QUICK | \
					 I2C_FUNC_SMBUS_BYTE | \
					 I2C_FUNC_SMBUS_BYTE_DATA | \
					 I2C_FUNC_SMBUS_WORD_DATA | \
					 I2C_FUNC_SMBUS_PROC_CALL | \
					 I2C_FUNC_SMBUS_WRITE_BLOCK_DATA | \
					 I2C_FUNC_SMBUS_I2C_BLOCK | \
					 I2C_FUNC_SMBUS_PEC)

/*flags for the client struct: */
#define I2C_CLIENT_PEC		0x04	/* Use Packet Error Checking */
#define I2C_CLIENT_TEN		0x10	/* we have a ten bit chip address */
					/* Must equal I2C_M_TEN below */
#define I2C_CLIENT_SLAVE	0x20	/* we are the slave */
#define I2C_CLIENT_HOST_NOTIFY	0x40	/* We want to use I2C host notify */
#define I2C_CLIENT_WAKE		0x80	/* for board_info; true iff can wake */
#define I2C_CLIENT_SCCB		0x9000	/* Use Omnivision SCCB protocol */
					/* Must match I2C_M_STOP|IGNORE_NAK */

struct i2c_msg {
	u16 addr;	/* slave address			*/
	u16 flags;
#define I2C_M_RD		0x0001	/* read data, from slave to master */
					/* I2C_M_RD is guaranteed to be 0x0001! */
#define I2C_M_TEN		0x0010	/* this is a ten bit chip address */
#define I2C_M_DMA_SAFE		0x0200	/* the buffer of this message is DMA safe */
					/* makes only sense in kernelspace */
					/* userspace buffers are copied anyway */
#define I2C_M_RECV_LEN		0x0400	/* length will be first received byte */
#define I2C_M_NO_RD_ACK		0x0800	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_IGNORE_NAK	0x1000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_REV_DIR_ADDR	0x2000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NOSTART		0x4000	/* if I2C_FUNC_NOSTART */
#define I2C_M_STOP		0x8000	/* if I2C_FUNC_PROTOCOL_MANGLING */
	u16 len;		/* msg length				*/
	u8 *buf;		/* pointer to msg data			*/
	u16 *dma_tx_buf;		/* pointer to dma tx msg data			*/
	u16 *link_addr;      /*pointer to link msg addr */
	u16 *link_len;     /*pointer to link msg data len*/
	u8 *Rx_link_addr;   /*point to rx addr*/
	void *priv;
	u8 *i2c_mode;      /*transmit mode*/
};

struct dw_i2c_dev {
	u16			*tx_dma_buf; 
	void	*base;
	u32 source_clk;
	u16 dev_address;
	struct dw_i2c_controller *controller;
	struct i2c_msg		*msgs;
	struct i2c_dev device;
	u32			xfer_complete;
	u32			xfer_doing;
	bool		addr_10;
	int			mode;
	int			cmd_err;
	int			msgs_num;
	u32			tx_buf_len;
	u8			*tx_buf;		
	u32			rx_buf_len;
	u8			*rx_buf;
	int			msg_err;
	u32		status;
	u32			abort_source;
	int			irq;
	u32			flags;
	u32			functionality;
	u32			master_cfg;
	u32		tx_fifo_depth;
	u32		rx_fifo_depth;
	u32			clk_freq;
	u32			sda_hold_time;
	u32			scl_rise_time;
	u32			scl_fall_time;
	u32		tLow;
	u32		tHigh;
	u32		su_dat;
	u32		tSp;
	u16			ss_hcnt;
	u16			ss_lcnt;
	u16			fs_hcnt;
	u16			fs_lcnt;
	u16			fp_hcnt;
	u16			fp_lcnt;
	u16			hs_hcnt;
	u16			hs_lcnt;
	int			rx_outstanding;
	int			msg_read_idx;
	int			msg_write_idx;
	bool		initial;
	int         TX_time;
};

int i2c_dw_init_master(struct dw_i2c_dev *dev);

int i2c_dw_xfer(struct dw_i2c_dev *dev, struct i2c_msg msgs[], int num);

void i2c_complete(struct dw_i2c_dev *dev);

void i2c_dw_init(enum i2c_index id);

//default fifo mode
//#define I2C_DMA_MODE
//#define I2C_DMA_LINKLIST

#endif /* MODULES_I2C_DW_I2C_H_ */
