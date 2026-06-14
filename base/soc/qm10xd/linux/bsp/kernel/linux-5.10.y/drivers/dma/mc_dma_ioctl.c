#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/dma/mc_dma64.h>
#include <linux/mm.h>
#include <asm/io.h>
#include<linux/slab.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>  //cdev
#include <linux/device.h>  //class & device
#include <linux/of.h>
#include <linux/of_device.h>


extern void dma64_free_chan_resources(struct dma_chan *chan);

#define DRIVER_NAME					"dmamemcpy"
#define DMA_MAGIC_FLAG				0XFF11


#define DMA_CHN_UNUSED 				0
#define DMA_CHN_USED 				1


#define DMA_MEMCPY_IOC_MAGIC 						'M'
#define DMA_MEMCPY_IOC_INIT 						_IO(DMA_MEMCPY_IOC_MAGIC, 0)  //src or dst
#define DMA_MEMCPY_IOC_START_AND_SYNC 				_IO(DMA_MEMCPY_IOC_MAGIC, 1)  //src or dst
#define DMA_MEMCPY_IOC_DEINIT 						_IO(DMA_MEMCPY_IOC_MAGIC, 2)
#define DMA_MEMCPY_IOC_INIT_EX 						_IO(DMA_MEMCPY_IOC_MAGIC, 3)  //src and dst
#define DMA_MEMCPY_IOC_START_AND_SYNC_EX 			_IO(DMA_MEMCPY_IOC_MAGIC, 4)  //src and dst


typedef struct _PHY_MEM_T{
	unsigned long output_param;
	unsigned long input_phy_addr;
	unsigned int input_len;
}PHY_MEM_T;

typedef struct _DMAMEMCPY_SRART_PARAM{
	unsigned long input_src_param;
	unsigned long input_dst_param;
	unsigned int input_len;
}DMAMEMCPY_SRART_PARAM;



typedef struct _DMA_BUF_NODE_T{
	unsigned long output_param;
	unsigned long input_src_phy_addr;
	unsigned long input_dst_phy_addr;
	unsigned int input_src_len;
	unsigned int input_dst_len;
}DMA_BUF_NODE_T;

typedef struct _DMAMEMCPY_SRART_PARAM_EX{
	unsigned long input_param;
	unsigned int input_len;
}DMAMEMCPY_SRART_PARAM_EX;



//handle struct
typedef struct _DMA_MEM_T{
	int magicnum;

	PHY_MEM_T *phy_buf_ptr;
	struct scatterlist *sg_table;  //record src/dst physical address and length

	DMA_BUF_NODE_T *node_ptr;
	struct scatterlist *src_sg_table;  //record src physical address and length
	struct scatterlist *dst_sg_table;  //recode dest physical address and length
}DMA_MEM_T;



struct mc_dma_chn{
	struct dma_chan *dma_chan;
	unsigned char used;
};


struct mc_dma_memcpy_dev{
	dev_t dmamemcpy_dev_number;
	struct cdev dmamemcpy_cdev;
	struct class *dmamemcpy_dev_class;
	struct device *dmamemcpy_dev_device;

	int flag;
	struct semaphore sem;
	int wq_ready;
	wait_queue_head_t wq;

	struct mc_dma_chn channel;
};


struct dma_callback_param{
	DMAMEMCPY_SRART_PARAM dmamemcpy_start_param;
	struct mc_dma_memcpy_dev *dma_memcpy_dev;
};

/*dma_complete_func: dma callback function
 *@dma complete
 * */
static void dma_complete_func(void *Param)
{
	DMA_MEM_T *dma_mem_src_ptr = NULL;
	DMA_MEM_T *dma_mem_dst_ptr = NULL;
	struct dma_callback_param *call_param = (struct dma_callback_param*)Param;

	dma_mem_src_ptr = (DMA_MEM_T *)call_param->dmamemcpy_start_param.input_src_param;
	dma_mem_dst_ptr = (DMA_MEM_T *)call_param->dmamemcpy_start_param.input_dst_param;


	if(dma_mem_src_ptr->phy_buf_ptr->input_len != call_param->dmamemcpy_start_param.input_len)
	{
		sg_dma_len(dma_mem_src_ptr->sg_table) = dma_mem_src_ptr->phy_buf_ptr->input_len;
	}

	if(dma_mem_dst_ptr->phy_buf_ptr->input_len != call_param->dmamemcpy_start_param.input_len)
	{
		sg_dma_len(dma_mem_dst_ptr->sg_table) = dma_mem_dst_ptr->phy_buf_ptr->input_len;
	}

	dma64_free_chan_resources(call_param->dma_memcpy_dev->channel.dma_chan);  //free descripter

	call_param->dma_memcpy_dev->wq_ready = 1;
	wake_up(&call_param->dma_memcpy_dev->wq);
}



struct dma_callback_param_ex{
	DMAMEMCPY_SRART_PARAM_EX dmamemcpy_start_param_ex;
	struct mc_dma_memcpy_dev *dma_memcpy_dev;
};

/*dma_complete_func_ex: dma callback function
 *@dma complete
 * */
static void dma_complete_func_ex(void *Param)
{
	DMA_MEM_T *dma_mem_ptr = NULL;
	struct dma_callback_param_ex *call_param = (struct dma_callback_param_ex*)Param;
	dma_mem_ptr = (DMA_MEM_T *)call_param->dmamemcpy_start_param_ex.input_param;


	if(dma_mem_ptr->node_ptr->input_src_len != call_param->dmamemcpy_start_param_ex.input_len)
	{
		sg_dma_len(dma_mem_ptr->src_sg_table) = dma_mem_ptr->node_ptr->input_src_len;
	}

	if(dma_mem_ptr->node_ptr->input_dst_len != call_param->dmamemcpy_start_param_ex.input_len)
	{
		sg_dma_len(dma_mem_ptr->dst_sg_table) = dma_mem_ptr->node_ptr->input_dst_len;
	}

	dma64_free_chan_resources(call_param->dma_memcpy_dev->channel.dma_chan);  //free descripter

	call_param->dma_memcpy_dev->wq_ready = 1;
	wake_up(&call_param->dma_memcpy_dev->wq);
}


static int check_handle(unsigned long handle)
{
	DMA_MEM_T *dma_mem_ptr = NULL;

	dma_mem_ptr = (DMA_MEM_T*)handle;
	if( dma_mem_ptr->magicnum != DMA_MAGIC_FLAG )
	{
		printk("handle error line=%d\n",__LINE__);
		return -1;
	}
	return 0;
}


static void dma_memcpy_release_chn(struct mc_dma_memcpy_dev *dma_memcpy_dev)
{
	dma_release_channel(dma_memcpy_dev->channel.dma_chan);
	dma_memcpy_dev->channel.used = DMA_CHN_UNUSED;
}

static int dma_memcpy_deinit_node(unsigned long handle)
{
	DMA_MEM_T *dma_mem_ptr = NULL;

	//check and get handle ptr
	if( check_handle(handle) )
	{
		return -1;
	}

	dma_mem_ptr = (DMA_MEM_T *)handle;

	dma_mem_ptr->magicnum = 0;

	if(dma_mem_ptr->phy_buf_ptr != NULL)
	{
		kfree(dma_mem_ptr->phy_buf_ptr);
	}

	if(dma_mem_ptr->node_ptr != NULL)
	{
		kfree(dma_mem_ptr->node_ptr);
	}

	if(dma_mem_ptr->sg_table != NULL)
	{
		kfree(dma_mem_ptr->sg_table);
	}

	if(dma_mem_ptr->src_sg_table != NULL)
	{
		kfree(dma_mem_ptr->src_sg_table);
	}

	if(dma_mem_ptr->dst_sg_table != NULL)
	{
		kfree(dma_mem_ptr->dst_sg_table);
	}

	if(dma_mem_ptr != NULL)
	{
		kfree(dma_mem_ptr);
	}

	return 0;
}

static void dma_memcpy_waite_finish(struct mc_dma_memcpy_dev *dma_memcpy_dev)
{
	wait_event(dma_memcpy_dev->wq,dma_memcpy_dev->wq_ready);
	dma_memcpy_dev->wq_ready = 0;
}


static int dma_memcpy_start(struct mc_dma_memcpy_dev *dma_memcpy_dev,DMA_MEM_T *dma_mem_src_ptr,DMA_MEM_T *dma_mem_dst_ptr,DMAMEMCPY_SRART_PARAM dmamemcpy_start_param)
{
	struct dma_device *dma_dev = NULL;
	struct dma_async_tx_descriptor *tx = NULL;
	dma_cookie_t cookie = 0;
	enum dma_ctrl_flags flags = 0;
	struct dma_callback_param call_param = {{0}};


	if(dma_mem_src_ptr->phy_buf_ptr->input_len > dmamemcpy_start_param.input_len)
	{
		sg_dma_len(dma_mem_src_ptr->sg_table) = dmamemcpy_start_param.input_len;
	}
	else if(dma_mem_src_ptr->phy_buf_ptr->input_len < dmamemcpy_start_param.input_len)
	{
		printk("error:The length you entered exceeds the previous length line=%d\n",__LINE__);
		return -1;
	}

	if(dma_mem_dst_ptr->phy_buf_ptr->input_len > dmamemcpy_start_param.input_len)
	{
		sg_dma_len(dma_mem_dst_ptr->sg_table) = dmamemcpy_start_param.input_len;
	}
	else if(dma_mem_dst_ptr->phy_buf_ptr->input_len < dmamemcpy_start_param.input_len)
	{
		printk("error:The length you entered exceeds the previous length line=%d\n",__LINE__);
		return -1;
	}

	if(down_interruptible(&dma_memcpy_dev->sem))
	{
		return -1;
	}

	dma_dev = dma_memcpy_dev->channel.dma_chan->device;

	/*generate linklist desc*/
	flags = DMA_PREP_INTERRUPT;
	tx = dma_dev->device_prep_dma_sg(dma_memcpy_dev->channel.dma_chan, dma_mem_dst_ptr->sg_table,1,dma_mem_src_ptr->sg_table,1, flags);
	if(!tx)
	{
		up(&dma_memcpy_dev->sem);
		printk("Failed to prepare DMA memcpy line=%d\n",__LINE__);
		return -1;
	}

	call_param.dmamemcpy_start_param = dmamemcpy_start_param;
	call_param.dma_memcpy_dev = dma_memcpy_dev;

	/*register callback fun*/
	tx->callback = dma_complete_func;
	tx->callback_param = &call_param;

	/*add desc to active list*/
	cookie = tx->tx_submit(tx);
	if(dma_submit_error(cookie)){
		up(&dma_memcpy_dev->sem);
		printk("Failed to dma tx_submit\n");
		return -1;
	}

	/*dma transfer start*/
	dma_async_issue_pending(dma_memcpy_dev->channel.dma_chan);

	return 0;
}


static int dma_memcpy_func(DMA_MEM_T *dma_mem_src_ptr,DMA_MEM_T *dma_mem_dst_ptr,DMAMEMCPY_SRART_PARAM dmamemcpy_start_param)
{
	void __iomem *src_ptr_vir = NULL;
	void __iomem *dst_ptr_vir = NULL;
	src_ptr_vir = ioremap(dma_mem_src_ptr->phy_buf_ptr->input_phy_addr, dmamemcpy_start_param.input_len);
	if(src_ptr_vir == NULL)
	{
		return -1;
	}
	dst_ptr_vir = ioremap(dma_mem_dst_ptr->phy_buf_ptr->input_phy_addr, dmamemcpy_start_param.input_len);
	if(dst_ptr_vir == NULL)
	{
		return -1;
	}

	memcpy(dst_ptr_vir,src_ptr_vir,dmamemcpy_start_param.input_len);

	if(src_ptr_vir != NULL)
	{
		iounmap(src_ptr_vir);
	}
	if(dst_ptr_vir != NULL)
	{
		iounmap(dst_ptr_vir);
	}

	return 0;
}

static int dma_memcpy_start_sync(struct mc_dma_memcpy_dev *dma_memcpy_dev,DMAMEMCPY_SRART_PARAM *dmamemcpy_start_param)
{
	DMA_MEM_T *dma_mem_src_ptr = NULL;
	DMA_MEM_T *dma_mem_dst_ptr = NULL;

	//check and get src handle ptr
	if( check_handle(dmamemcpy_start_param->input_src_param) )
	{
		return -1;
	}
	dma_mem_src_ptr = (DMA_MEM_T *)dmamemcpy_start_param->input_src_param;

	//check and get dst handle ptr
	if( check_handle(dmamemcpy_start_param->input_dst_param) )
	{
		return -1;
	}
	dma_mem_dst_ptr = (DMA_MEM_T *)dmamemcpy_start_param->input_dst_param;

	if(dmamemcpy_start_param->input_len >= 0x0)  // 4k dma
	{
		if( dma_memcpy_start(dma_memcpy_dev,dma_mem_src_ptr,dma_mem_dst_ptr,*dmamemcpy_start_param) )
		{
			return -1;
		}

		dma_memcpy_waite_finish(dma_memcpy_dev);

		up(&dma_memcpy_dev->sem);

	}else{
		if( dma_memcpy_func(dma_mem_src_ptr,dma_mem_dst_ptr,*dmamemcpy_start_param) )  //sys memcpy()
		{
			return -1;
		}
	}

	return 0;
}


static int dma_memcpy_start_ex(struct mc_dma_memcpy_dev *dma_memcpy_dev,DMA_MEM_T *dma_mem_ptr,DMAMEMCPY_SRART_PARAM_EX dmamemcpy_start_param_ex)
{
	struct dma_device *dma_dev = NULL;
	struct dma_async_tx_descriptor *tx = NULL;
	dma_cookie_t cookie = 0;
	enum dma_ctrl_flags flags = 0;
	struct dma_callback_param_ex call_param;

	//printk("line=%d sp_len=%d sn_len=%d \n",__LINE__,dma_mem_ptr->node_ptr->input_src_len,dmamemcpy_start_param_ex.input_len);
	//printk("line=%d dp_len=%d dn_len=%d \n",__LINE__,dma_mem_ptr->node_ptr->input_dst_len,dmamemcpy_start_param_ex.input_len);
	if(dma_mem_ptr->node_ptr->input_src_len > dmamemcpy_start_param_ex.input_len)
	{
		sg_dma_len(dma_mem_ptr->src_sg_table) = dmamemcpy_start_param_ex.input_len;
	}
	else if(dma_mem_ptr->node_ptr->input_src_len < dmamemcpy_start_param_ex.input_len)
	{
		printk("error:The length you entered exceeds the previous length line=%d\n",__LINE__);
		return -1;
	}

	if(dma_mem_ptr->node_ptr->input_dst_len > dmamemcpy_start_param_ex.input_len)
	{
		sg_dma_len(dma_mem_ptr->dst_sg_table) = dmamemcpy_start_param_ex.input_len;
	}
	else if(dma_mem_ptr->node_ptr->input_dst_len < dmamemcpy_start_param_ex.input_len)
	{
		printk("error:The length you entered exceeds the previous length line=%d\n",__LINE__);
		return -1;
	}

	if(down_interruptible(&dma_memcpy_dev->sem))
	{
		return -1;
	}

	dma_dev = dma_memcpy_dev->channel.dma_chan->device;

	/*generate linklist desc*/
	flags = DMA_PREP_INTERRUPT;
	tx = dma_dev->device_prep_dma_sg(dma_memcpy_dev->channel.dma_chan, dma_mem_ptr->dst_sg_table,1,dma_mem_ptr->src_sg_table,1, flags);
	if(!tx)
	{
		printk("Failed to prepare DMA memcpy line=%d\n",__LINE__);
		up(&dma_memcpy_dev->sem);
		return -1;
	}

	call_param.dmamemcpy_start_param_ex = dmamemcpy_start_param_ex;
	call_param.dma_memcpy_dev = dma_memcpy_dev;
	/*register callback fun*/
	tx->callback = dma_complete_func_ex;
	tx->callback_param = &call_param;

	/*add desc to active list*/
	cookie = tx->tx_submit(tx);
	if(dma_submit_error(cookie)){
		printk("Failed to dma tx_submit\n");
		up(&dma_memcpy_dev->sem);
		return -1;
	}

	/*dma transfer start*/
	dma_async_issue_pending(dma_memcpy_dev->channel.dma_chan);
	return 0;
}


static int dma_memcpy_func_ex(DMA_MEM_T *dma_mem_ptr,DMAMEMCPY_SRART_PARAM_EX dmamemcpy_start_param_ex)
{
	void __iomem *src_ptr_vir = NULL;
	void __iomem *dst_ptr_vir = NULL;
	src_ptr_vir = ioremap(dma_mem_ptr->node_ptr->input_src_phy_addr, dmamemcpy_start_param_ex.input_len);
	if(src_ptr_vir == NULL)
	{
		return -1;
	}
	dst_ptr_vir = ioremap(dma_mem_ptr->node_ptr->input_dst_phy_addr, dmamemcpy_start_param_ex.input_len);
	if(dst_ptr_vir == NULL)
	{
		return -1;
	}

	memcpy(dst_ptr_vir,src_ptr_vir,dmamemcpy_start_param_ex.input_len);

	if(src_ptr_vir != NULL)
	{
		iounmap(src_ptr_vir);
	}
	if(dst_ptr_vir != NULL)
	{
		iounmap(dst_ptr_vir);
	}

	return 0;
}

static int dma_memcpy_start_sync_ex(struct mc_dma_memcpy_dev *dma_memcpy_dev,DMAMEMCPY_SRART_PARAM_EX *dmamemcpy_start_param_ex)
{
	DMA_MEM_T *dma_mem_ptr = NULL;


	//check and get handle ptr
	if( check_handle(dmamemcpy_start_param_ex->input_param) )
	{
		return -1;
	}

	dma_mem_ptr = (DMA_MEM_T *)dmamemcpy_start_param_ex->input_param;

	if(dmamemcpy_start_param_ex->input_len >= 0x0)  //4k dma
	{
		if( dma_memcpy_start_ex(dma_memcpy_dev,dma_mem_ptr,*dmamemcpy_start_param_ex) )
		{
			return -1;
		}
		
		dma_memcpy_waite_finish(dma_memcpy_dev);

		up(&dma_memcpy_dev->sem);

	}else{
		if( dma_memcpy_func_ex(dma_mem_ptr,*dmamemcpy_start_param_ex) )  //sys memcpy()
		{
			return -1;
		}
	}

	return 0;
}


static void fill_scatterlist(struct scatterlist* sg, PHY_MEM_T phy_mem_t)
{
	/*init sg src/dst according scnt/dcnt value*/
	sg_init_table(sg, 1);

	/*record sg src/dst phy add and len*/
	sg_dma_address(sg) = phy_mem_t.input_phy_addr;
	sg_dma_len(sg) = phy_mem_t.input_len;
}

static int dma_memcpy_init(PHY_MEM_T *phy_mem_t)
{
	DMA_MEM_T *dma_mem_ptr = NULL;

	//malloc handle ptr
	dma_mem_ptr = (DMA_MEM_T*)kzalloc(sizeof(DMA_MEM_T),GFP_KERNEL);
	if(dma_mem_ptr == NULL)
	{
		printk("kzalloc failed line=%d\n",__LINE__);
		return -1;
	}

	//malloc to save user data
	dma_mem_ptr->phy_buf_ptr = (PHY_MEM_T*)kzalloc(sizeof(PHY_MEM_T),GFP_KERNEL);
	if(dma_mem_ptr->phy_buf_ptr == NULL)
	{
		printk("kzalloc failed line=%d\n",__LINE__);
		return -1;
	}

	dma_mem_ptr->magicnum = DMA_MAGIC_FLAG;  //dma flag

	memcpy(dma_mem_ptr->phy_buf_ptr, phy_mem_t, sizeof(PHY_MEM_T));  //save user data
	phy_mem_t->output_param = (unsigned long)dma_mem_ptr;  //return handle to user


	//malloc scatterlist
	dma_mem_ptr->sg_table = (struct scatterlist*)kzalloc(sizeof(struct scatterlist),GFP_KERNEL);
	if(dma_mem_ptr->sg_table == NULL)
	{
		printk("kzalloc failed line=%d\n",__LINE__);
		return -1;
	}

	//fill struct scatterlist
	fill_scatterlist(dma_mem_ptr->sg_table, *phy_mem_t);

	return 0;
}


static void fill_scatterlist_ex(struct scatterlist* src_sg,struct scatterlist* dst_sg, DMA_BUF_NODE_T dma_buf_node_t)
{
	/*init sg src according scnt/dcnt value*/
	sg_init_table(src_sg, 1);

	/*record sg src phy add and len*/
	sg_dma_address(src_sg) = dma_buf_node_t.input_src_phy_addr;
	sg_dma_len(src_sg) = dma_buf_node_t.input_src_len;


	/*init sg dst according scnt/dcnt value*/
	sg_init_table(dst_sg, 1);

	/*record sg dst phy add and len*/
	sg_dma_address(dst_sg) = dma_buf_node_t.input_dst_phy_addr;
	sg_dma_len(dst_sg) = dma_buf_node_t.input_dst_len;
}

static int dma_memcpy_init_ex(DMA_BUF_NODE_T *dma_buf_node_t)
{
	DMA_MEM_T *dma_mem_ptr = NULL;

	//malloc handle ptr
	dma_mem_ptr = (DMA_MEM_T*)kzalloc(sizeof(DMA_MEM_T),GFP_KERNEL);
	if(dma_mem_ptr == NULL)
	{
		printk("kzalloc failed line=%d\n",__LINE__);
		return -1;
	}

	//malloc to save user data
	dma_mem_ptr->node_ptr = (DMA_BUF_NODE_T*)kzalloc(sizeof(DMA_BUF_NODE_T),GFP_KERNEL);
	if(dma_mem_ptr->node_ptr == NULL)
	{
		printk("kzalloc failed line=%d\n",__LINE__);
		return -1;
	}

	dma_mem_ptr->magicnum = DMA_MAGIC_FLAG;

	memcpy(dma_mem_ptr->node_ptr, dma_buf_node_t, sizeof(DMA_BUF_NODE_T));  //save user data
	dma_buf_node_t->output_param = (unsigned long)dma_mem_ptr;  //return handle to user

	//malloc src scatterlist
	dma_mem_ptr->src_sg_table = (struct scatterlist*)kzalloc(sizeof(struct scatterlist),GFP_KERNEL);
	if(dma_mem_ptr->src_sg_table == NULL)
	{
		printk("kzalloc failed line=%d\n",__LINE__);
		return -1;
	}

	//malloc dst scatterlist
	dma_mem_ptr->dst_sg_table = (struct scatterlist*)kzalloc(sizeof(struct scatterlist),GFP_KERNEL);
	if(dma_mem_ptr->dst_sg_table == NULL)
	{
		printk("kzalloc failed line=%d\n",__LINE__);
		return -1;
	}

	//fill struct scatterlist
	fill_scatterlist_ex(dma_mem_ptr->src_sg_table,dma_mem_ptr->dst_sg_table, *dma_buf_node_t);

	return 0;
}

static int dma_memcpy_get_channel(struct mc_dma_memcpy_dev *dma_memcpy_dev)
{
	dma_cap_mask_t mask = {{0}};
	struct dma_chan *chan = NULL;

	dma_cap_zero(mask);
	dma_cap_set(DMA_SG, mask);
	chan = dma_request_channel(mask, NULL,NULL); //get a dma channel on free application
	if(!chan){
		printk("dma request channel failed line=%d\n",__LINE__);
		return -1;
	}

	dma_memcpy_dev->channel.dma_chan = chan;
	dma_memcpy_dev->channel.used = DMA_CHN_USED;

	return 0;
}



static int mc_dma_memcpy_open(struct inode *inode, struct file *file)
{
	struct mc_dma_memcpy_dev *dma_memcpy_dev = NULL;
	dma_memcpy_dev = container_of(inode->i_cdev,struct mc_dma_memcpy_dev,dmamemcpy_cdev);
	file->private_data = dma_memcpy_dev;

	if(down_interruptible(&dma_memcpy_dev->sem))
	{
		return -1;
	}

	if(dma_memcpy_dev->flag == 0)
	{
		if( dma_memcpy_get_channel(dma_memcpy_dev) )
		{
			printk("dma_memcpy_get_channel failed line=%d\n",__LINE__);
			up(&dma_memcpy_dev->sem);
			return -1;
		}
	}

	dma_memcpy_dev->flag++;
	up(&dma_memcpy_dev->sem);

	return 0;
}

static int mc_dma_memcpy_release(struct inode *inode, struct file *file)
{
	struct mc_dma_memcpy_dev *dma_memcpy_dev = NULL;
	dma_memcpy_dev = file->private_data;

	if(down_interruptible(&dma_memcpy_dev->sem))
	{
		return -1;
	}

	dma_memcpy_dev->flag--;

	if(dma_memcpy_dev->flag == 0)
	{
		dma_memcpy_release_chn(dma_memcpy_dev);
	}

	if(dma_memcpy_dev->flag < 0)
	{
		printk("error:you need not to close\n");
		dma_memcpy_dev->flag = 0;
	}

	up(&dma_memcpy_dev->sem);

	return 0;
}

static ssize_t mc_dma_memcpy_write(struct file *file, const char __user *data, size_t len, loff_t *ppos){
	return 0;
}


static long mc_dma_memcpy_ioctl(struct file *file, u32 cmd, unsigned long arg)
{
	switch(cmd){
		case DMA_MEMCPY_IOC_INIT:
		{
			int ret = -1;
			PHY_MEM_T phy_mem_temp = {0};

			ret = copy_from_user(&phy_mem_temp, (void __user *)arg, sizeof(PHY_MEM_T));
			if(ret){
				printk("Copy from user failed line=%d\n",__LINE__);
				goto error;
			}

			ret = dma_memcpy_init(&phy_mem_temp);
			if(ret)
			{
				printk("dma_memcpy_init failed line=%d\n",__LINE__);
				goto error;
			}

			ret = copy_to_user((void __user *)arg, &phy_mem_temp, sizeof(unsigned long));  //user get handle
			if(ret)
			{
				printk("Copy to user failed line=%d\n",__LINE__);
				goto error;
			}
		}
		break;
		case DMA_MEMCPY_IOC_INIT_EX:
		{
			int ret = -1;
			DMA_BUF_NODE_T dma_buf_node_temp = {0};

			ret = copy_from_user(&dma_buf_node_temp, (void __user *)arg, sizeof(DMA_BUF_NODE_T));
			if(ret){
				printk("Copy from user failed line=%d\n",__LINE__);
				goto error;
			}

			ret = dma_memcpy_init_ex(&dma_buf_node_temp);
			if(ret)
			{
				printk("dma_memcpy_init_ex failed line=%d\n",__LINE__);
				goto error;
			}

			ret = copy_to_user((void __user *)arg, &dma_buf_node_temp, sizeof(unsigned long));  //user get handle
			if(ret)
			{
				printk("Copy to user failed line=%d\n",__LINE__);
				goto error;
			}
		}
		break;
		case DMA_MEMCPY_IOC_START_AND_SYNC:
		{
			int ret = -1;
			DMAMEMCPY_SRART_PARAM dmamemcpy_start_param = {0};
			struct mc_dma_memcpy_dev *dma_memcpy_dev = NULL;
			dma_memcpy_dev = file->private_data;

			ret = copy_from_user(&dmamemcpy_start_param, (void __user *)arg, sizeof(DMAMEMCPY_SRART_PARAM));
			if(ret){
				printk("Copy from user failed line=%d\n",__LINE__);
				goto error;
			}

			if( !(dmamemcpy_start_param.input_src_param && dmamemcpy_start_param.input_dst_param) )
			{
				printk("you input handle error line=%d\n",__LINE__);
				goto error;
			}

			ret = dma_memcpy_start_sync(dma_memcpy_dev,&dmamemcpy_start_param);
			if(ret)
			{
				printk("dma_memcpy_start_sync failed line=%d\n",__LINE__);
				goto error;
			}
		}
		break;
		case DMA_MEMCPY_IOC_START_AND_SYNC_EX:
		{
			int ret = -1;
			DMAMEMCPY_SRART_PARAM_EX dmamemcpy_start_param_ex = {0};
			struct mc_dma_memcpy_dev *dma_memcpy_dev = NULL;
			dma_memcpy_dev = file->private_data;

			ret = copy_from_user(&dmamemcpy_start_param_ex, (void __user *)arg, sizeof(DMAMEMCPY_SRART_PARAM_EX));
			if(ret){
				printk("Copy from user failed line=%d\n",__LINE__);
				goto error;
			}


			if( !dmamemcpy_start_param_ex.input_param )
			{
				printk("you input handle error line=%d\n",__LINE__);
				goto error;
			}

			ret = dma_memcpy_start_sync_ex(dma_memcpy_dev,&dmamemcpy_start_param_ex);
			if(ret)
			{
				goto error;
			}
		}
		break;
		case DMA_MEMCPY_IOC_DEINIT:
		{
			int ret = -1;
			unsigned long handle = 0;

			ret = copy_from_user(&handle, (void __user *)arg, sizeof(unsigned long));
			if(ret){
				printk("Copy from user failed line=%d\n",__LINE__);
				goto error;
			}

			if(handle == 0)
			{
				printk("handle error line=%d\n",__LINE__);
				goto error;
			}

			ret = dma_memcpy_deinit_node(handle);
			if(ret)
			{
				printk("dma_memcpy_deinit_node failed line=%d\n",__LINE__);
				goto error;
			}

			handle = 0;
			ret = copy_to_user((void __user *)arg, &handle, sizeof(unsigned long));
			if(ret){
				printk("Copy to user failed line=%d\n",__LINE__);
				goto error;
			}
		}
		break;
		default:
			printk("Don't support cmd [%d]\n", cmd);
			break;
	}

	return 0;

error:
	return -1;
}

/*
 *    Kernel Interfaces
 */

static struct file_operations mc_dma64_fops = {
    .owner        = THIS_MODULE,
    .llseek        = no_llseek,
    .write        = mc_dma_memcpy_write,
    .unlocked_ioctl = mc_dma_memcpy_ioctl,
    .open        = mc_dma_memcpy_open,
    .release    = mc_dma_memcpy_release,
};




static int dmamemcpy_dev_detach(struct mc_dma_memcpy_dev *dma_memcpy_dev)
{

	/*delete device*/
	device_destroy(dma_memcpy_dev->dmamemcpy_dev_class, dma_memcpy_dev->dmamemcpy_dev_number);

	/*delete cdev*/
	cdev_del(&dma_memcpy_dev->dmamemcpy_cdev);
	/*free dev_t*/
	unregister_chrdev_region(dma_memcpy_dev->dmamemcpy_dev_number, 1);

	/*delete class*/
	class_destroy(dma_memcpy_dev->dmamemcpy_dev_class);

	return 0;
}


static int dmamemcpy_dev_attach(struct mc_dma_memcpy_dev *dma_memcpy_dev)
{
	int rtn = 0;

	rtn = alloc_chrdev_region(&dma_memcpy_dev->dmamemcpy_dev_number, 0, 1, "dma-memcpy");
	if (rtn < 0) {
		printk("failed to alloc_chrdev_region\n");
		return rtn;
	}

	cdev_init(&dma_memcpy_dev->dmamemcpy_cdev, &mc_dma64_fops);
	dma_memcpy_dev->dmamemcpy_cdev.owner = THIS_MODULE;
	rtn = cdev_add(&dma_memcpy_dev->dmamemcpy_cdev, dma_memcpy_dev->dmamemcpy_dev_number, 1);
	if (rtn < 0) {
		printk("unable to cdev_add dmamemcpy dev\n");
		return rtn;
	}

	dma_memcpy_dev->dmamemcpy_dev_class = class_create(THIS_MODULE, "dma-memcpy-dev");  //create class
	if (IS_ERR(dma_memcpy_dev->dmamemcpy_dev_class)) {
		printk("dmamemcpy unable to class_create()\n");
		return -1;
	}

	dma_memcpy_dev->dmamemcpy_dev_device = device_create(dma_memcpy_dev->dmamemcpy_dev_class, NULL, dma_memcpy_dev->dmamemcpy_dev_number, NULL, DRIVER_NAME);  //dev/dmamemcpy
	if (IS_ERR(dma_memcpy_dev->dmamemcpy_dev_device)) {
		printk("dmamemcpy unable to device_create()\n");
		return -1;
	}

	printk("************ dmamemcpy device node path is \"/dev/dmamemcpy\" **************\n");

	return 0;
}

static void params_init(struct mc_dma_memcpy_dev *dma_memcpy_dev)
{
	sema_init(&dma_memcpy_dev->sem, 1);

	dma_memcpy_dev->wq_ready = 0;
	init_waitqueue_head(&dma_memcpy_dev->wq);

	dma_memcpy_dev->flag = 0;
}



static int dmamemcpy_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct mc_dma_memcpy_dev *dma_memcpy_dev = NULL;
	struct device *dev = &pdev->dev;

	dma_memcpy_dev = devm_kzalloc(dev, sizeof(struct mc_dma_memcpy_dev), GFP_KERNEL);
	if (!dma_memcpy_dev)
	{
		printk ( "devm_kzalloc failes line=%d\n",__LINE__);
		return -1;
	}

	platform_set_drvdata(pdev, dma_memcpy_dev);

	params_init(dma_memcpy_dev);

	ret = dmamemcpy_dev_attach(dma_memcpy_dev);
	if(ret)
	{
		printk("dmamemcpy_dev_attach is failed\n");
	}
	printk("dmamemcpy_dev_attach is success\n");


	return ret;
}


static int dmamemcpy_remove(struct platform_device *pdev)
{
	struct mc_dma_memcpy_dev *dma_memcpy_dev = NULL;
	dma_memcpy_dev = platform_get_drvdata(pdev);

	dmamemcpy_dev_detach(dma_memcpy_dev);

	return 0;
}


static const struct of_device_id dmamemcpy_dt_id[] = {
	{.compatible = "mc,dmamemcpy"},
	{}
};

MODULE_DEVICE_TABLE(of, dmamemcpy_dt_id);
static struct platform_driver dmamemcpy_driver = {
	.probe	= dmamemcpy_probe,
	.remove = dmamemcpy_remove,
	.driver = {
		.name = "dmamemcpy-dev",
		.of_match_table = dmamemcpy_dt_id,
	},
};

static int __init mc_dma64_init(void)
{
	printk("******%s*******\n",__func__);
	return platform_driver_register(&dmamemcpy_driver);
}

static void __exit mc_dma64_exit(void)
{
	printk("******%s*******\n",__func__);
	platform_driver_unregister(&dmamemcpy_driver);
}


module_init(mc_dma64_init);
module_exit(mc_dma64_exit);

MODULE_AUTHOR("molchip");
MODULE_DESCRIPTION("mc Dma64 memcpy Driver");
MODULE_LICENSE("GPL");



