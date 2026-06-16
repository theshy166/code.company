/*
 * i2c.h
 *
 *  Created on: 2018��9��29��
 *      Author: jason.qin
 */

#ifndef MODULES_I2C_API_H_
#define MODULES_I2C_API_H_

typedef volatile unsigned long long u64;
typedef volatile unsigned int u32;
typedef volatile int s32;
typedef volatile unsigned short u16;
typedef volatile short s16;
typedef volatile unsigned char u8;
typedef volatile char s8;

typedef volatile unsigned int bool;

#define FALSE	(1 == 0)
#define TRUE	(1 == 1)
#define NULL	((void *)0)

#define false	(1 == 0)
#define true	(1 == 1)

#define ENABLE	(1 == 1)
#define DISABLE	(1 == 0)

typedef void(*i2c_func_t)(void *data);

enum i2c_transmit_mode {
	I2C_FIFO_MODE = 0,
	I2C_DMA_MODE,	
	I2C_DMA_LINKLIST,
};

enum i2c_index {
	I2C_INDEX_0 = 0,/* REG_I2C0_BASE */
	I2C_INDEX_1,	/* REG_I2C1_BASE */
	I2C_INDEX_VAL,
};

enum i2c_op {
	I2C_OP_WRITE = 0,	/* write data, to slave to master */
	I2C_OP_READ,		/* read data, from slave to master */
	I2C_OP_VAL,
};

enum i2c_speed {
	I2C_SPEED_STD = 0,	/*stand mode: SCL 100K */
	I2C_SPEED_FS,		/*fast mode: SCL 400K */
	I2C_SPEED_FP,		/*fast mode plus: SCL 1000K */
	I2C_SPEED_VAL,
};

struct i2c_req {
	enum i2c_op	op; /* opertion: read/write	*/
	u8  read_addr;	/* destination address for read operation, 	*/	
	void *buf;		/* point to data buf, Note: when write operation, 
					   place the write address firstly, and then write data ...*/
	u16 len;		/* buf length, Note:including write address when write operation */				   
	void(*func)(void *data); /* call back */
	void *data;	/* call back data*/
	u8 status;	/* give back xfer status */
	void *link_addr;  /*point to data addr*/
	void *link_len;  /*point to data len*/
	void *Rx_link_addr;  /*point to RX data addr*/
};

struct i2c_cfg {
	enum i2c_index id;		/* i2c controller id	*/
	u32 clk;				/* source clk	*/
	enum i2c_speed speed; 	/* scl clk		*/
	u16 dev_addr;			/* device address	*/
	bool addr_10;			/* Is 10-bit addressing	*/
	enum i2c_transmit_mode mode;
};

struct i2c_dev {
	void *controller_base;
	u32 source_clk;
	u32 scl_clk;
	u8 dev_addr;
};

extern int i2c_config(struct i2c_cfg *cfg, struct i2c_dev ** device);
extern int i2c_xfer_request(struct i2c_dev *dev, struct i2c_req *req);
extern void i2c_update_address(struct i2c_dev *dev, u16 slave_address);


#endif /* MODULES_I2C_DW_I2C_H_ */
