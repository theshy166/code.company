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
#include "mc_dma_lib.h"

#define DRIVER_NAME 				"dma64"


 /*dma channels is not pre-allocate, they are dynamic
 *			application
 */
#define MC_DMA_MAX_MEMORY_CHANNEL		32


#define DMA_CHN_UNUSED 				0
#define DMA_CHN_USED 				1
#define MC_DMA_IOC_MAGIC 			'A'
#define MC_DMA_IOCGETCHN			_IO(MC_DMA_IOC_MAGIC, 0)
#define MC_DMA_IOCCFGANDSTART 			_IO(MC_DMA_IOC_MAGIC, 1)
#define MC_DMA_IOCGETSTATUS 			_IO(MC_DMA_IOC_MAGIC, 2)
#define MC_DMA_IOCRELEASECHN 			_IO(MC_DMA_IOC_MAGIC, 3)
#define MMAP_MEM_SIZE				0x4000

#define ARRAY_COUNT				512
#define SRC_ADDR  				1
#define DST_ADDR				2
#define KPAGE_SIZE				0x1000

/*communication with user level pragram*/
struct mc_dma_chncfg{
	/*va 2 pa in user space */
	unsigned long src_addr[ARRAY_COUNT];
	unsigned long dst_addr[ARRAY_COUNT];
	unsigned long src_len[ARRAY_COUNT];
	unsigned long dst_len[ARRAY_COUNT];
	/*common items*/
	unsigned long scnt;
	unsigned long dcnt;
	unsigned long chn_num;//dma channel number used
	unsigned long status;
};
struct mc_dma_chns{
	struct dma_chan *dma_chan;
	unsigned char used;
#define DMA_STATUS_UNFINISHED	0
#define DMA_STATUS_FINISHED 	1
	unsigned char status;//dma complete status
	unsigned char reserved[2];
};
static struct mc_dma_chns channels[MC_DMA_MAX_MEMORY_CHANNEL];

struct scatterlist 	src_sg_table[ARRAY_COUNT];  //record src physical address and length
struct scatterlist 	dst_sg_table[ARRAY_COUNT];  //recode dest physical address and length

/*dma_complete_func: dma callback function
 *@status dma complete status
 * */
static void dma_complete_func(void *status){

	*(u32 *)status = DMA_STATUS_FINISHED;
	printk(KERN_INFO"complete status = 0x%x\r\n",*(u32*)status);
}
#if 0
static void debug_dump1(struct mc_dma_chncfg *chncfg){
	int i;
	for(i = 0;i<chncfg->scnt;i++){
		printk(KERN_DEBUG"src_addr[%d]=0x%x\r\n",i,chncfg->src_addr[i]);
		printk(KERN_DEBUG"src_len[%d]=0x%x\r\n",i,chncfg->src_len[i]);
	}
	for(i = 0;i<chncfg->dcnt;i++){
		printk(KERN_DEBUG"dst_addr[%d]=0x%x\r\n",i,chncfg->dst_addr[i]);
		printk(KERN_DEBUG"dst_len[%d]=0x%x\r\n",i,chncfg->dst_len[i]);
	}
		printk(KERN_DEBUG"chncfg->scnt=0x%x\r\n",chncfg->scnt);
		printk(KERN_DEBUG"chncfg->dcnt=0x%x\r\n",chncfg->dcnt);
}
#endif

static int mc_dma_open(struct inode *inode, struct file *file){

	file->private_data = (struct mc_dma_chncfg*)kmalloc(sizeof(struct mc_dma_chncfg),GFP_ATOMIC);
	if(!file->private_data){
		printk(KERN_ERR"kalloc mc_dma_chncfg pointer NULL\r\n");
	}

	return 0;
}

static int mc_dma_release(struct inode *inode, struct file *file){
	kfree(file->private_data);
	return 0;
}

static ssize_t mc_dma_write(struct file *file, const char __user *data, size_t len, loff_t *ppos){
	return 0;
}

static long mc_dma_ioctl(struct file *file, u32 cmd, unsigned long arg){

	struct dma_device *dma_dev;
	struct dma_async_tx_descriptor *tx = NULL;
	struct dma_chan *chan;
	dma_cap_mask_t mask;
	dma_cookie_t cookie;
	enum dma_ctrl_flags flags;
	struct mc_dma_chncfg *chncfg;
	int ret = -1;
	int i;
	int j;
	chncfg = (struct mc_dma_chncfg *)file->private_data;

	switch(cmd){
		case MC_DMA_IOCGETCHN:
		{
			dma_cap_zero(mask);
			dma_cap_set(DMA_SG, mask);
			chan = dma_request_channel(mask, NULL,NULL); //get a dma channel on free application
			if(!chan){
				printk(KERN_ERR"dma request channel failed\n");
				goto error;
			}

			/*get the channel index you use ,and you use this value
			 * to find the pointer of the struct dma_chan,and dma complete
			 * status value range 0~31*/
			chncfg->chn_num = chan->chan_id;
			channels[chncfg->chn_num].dma_chan = chan;
			channels[chncfg->chn_num].used = DMA_CHN_USED;
			ret = copy_to_user((void __user *)arg, chncfg, sizeof(*chncfg));
			if(ret){
				printk(KERN_ERR"Copy to user failed\n");
				goto error;
			}
		}
		break;
		case MC_DMA_IOCCFGANDSTART:
		{
			ret = copy_from_user(chncfg, (void __user *)arg, sizeof(*chncfg));
			if(ret){
				printk(KERN_ERR"Copy from user failed\n");
				goto error;
			}


		//	debug_dump1(chncfg);
			dma_dev = channels[chncfg->chn_num].dma_chan->device;

			/*init sg table according scnt/dcnt value*/
			sg_init_table(src_sg_table,chncfg->scnt);
			sg_init_table(dst_sg_table,chncfg->dcnt);

			for(i = 0;i < chncfg->scnt;i++){
				sg_dma_address(&src_sg_table[i]) = chncfg->src_addr[i];
				sg_dma_len(&src_sg_table[i]) = chncfg->src_len[i];
				dma_sync_single_for_device(dma_dev->dev, chncfg->src_addr[i],
						chncfg->src_len[i],DMA_TO_DEVICE);
			}

			for(j = 0;j < chncfg->dcnt;j++)
			{
				sg_dma_address(&dst_sg_table[j]) = chncfg->dst_addr[j];
				sg_dma_len(&dst_sg_table[j]) = chncfg->dst_len[j];
				dma_sync_single_for_device(dma_dev->dev, chncfg->dst_addr[j],
						chncfg->dst_len[j], DMA_TO_DEVICE);
			}
			/*generate linklist desc*/
			flags = DMA_PREP_INTERRUPT;
			tx = dma_dev->device_prep_dma_sg(channels[chncfg->chn_num].dma_chan, dst_sg_table,chncfg->dcnt,src_sg_table,chncfg->scnt, flags);
			if(!tx)
			{
				printk(KERN_ERR"Failed to prepare DMA memcpy\n");
				goto error;
			}

			/*register callback fun*/
			tx->callback = dma_complete_func;
			channels[chncfg->chn_num].status = DMA_STATUS_UNFINISHED;
			tx->callback_param = &channels[chncfg->chn_num].status;

			/*add desc to active list*/
			cookie = tx->tx_submit(tx);
			if(dma_submit_error(cookie)){
				printk(KERN_ERR"Failed to dma tx_submit\n");
				goto error;
			}
			/*dma transfer start*/
			dma_async_issue_pending(channels[chncfg->chn_num].dma_chan);
		}
		break;
		case MC_DMA_IOCGETSTATUS:
		{
			dma_dev = channels[chncfg->chn_num].dma_chan->device;
			ret = copy_from_user(chncfg, (void __user *)arg, sizeof(struct mc_dma_chncfg));
			if(ret){
				printk(KERN_ERR"Copy from user failed\n");
				goto error;
			}

			if(chncfg->chn_num >= MC_DMA_MAX_MEMORY_CHANNEL){
				printk(KERN_ERR"chn_num[%ld] is invalid\n", chncfg->chn_num);
				goto error;
			}
			chncfg->status = channels[chncfg->chn_num].status;
			ret = copy_to_user((void __user *)arg, chncfg, sizeof(struct mc_dma_chncfg));
			if(ret){
				printk(KERN_ERR"Copy to user failed\n");
				goto error;
			}
			for(j = 0;j < chncfg->dcnt;j++){
				dma_sync_single_for_cpu(dma_dev->dev, chncfg->dst_addr[j],
						chncfg->dst_len[j], DMA_FROM_DEVICE);
			}
		}
		break;
		case MC_DMA_IOCRELEASECHN:
		{
			ret = copy_from_user(chncfg, (void __user *)arg, sizeof(struct mc_dma_chncfg));
			if(ret){
				printk(KERN_ERR"Copy from user failed\n");
				goto error;
			}
			if((chncfg->chn_num >= MC_DMA_MAX_MEMORY_CHANNEL) || (!channels[chncfg->chn_num].dma_chan)){
				printk(KERN_ERR"chn_num[%ld] is invalid\n", chncfg->chn_num);
				goto error;
			}
			dma_release_channel(channels[chncfg->chn_num].dma_chan);
			channels[chncfg->chn_num].used = DMA_CHN_UNUSED;
			channels[chncfg->chn_num].status = DMA_STATUS_UNFINISHED;
		}
		break;
		default:
			printk(KERN_ERR"Don't support cmd [%d]\n", cmd);
			break;
	}
	return 0;
error:
	return -EFAULT;
}

/*
 *    Kernel Interfaces
 */

static struct file_operations mc_dma64_fops = {
    .owner        = THIS_MODULE,
    .llseek        = no_llseek,
    .write        = mc_dma_write,
    .unlocked_ioctl = mc_dma_ioctl,
    .open        = mc_dma_open,
    .release    = mc_dma_release,
};

static struct miscdevice mc_dma64_miscdev = {
    .minor        = MISC_DYNAMIC_MINOR,
    .name        = DRIVER_NAME,
    .fops        = &mc_dma64_fops,
};

static int __init mc_dma64_init(void){
    int ret = 0;
    printk(KERN_INFO"dma ioctl init\r\n");

    ret = misc_register(&mc_dma64_miscdev);
    if(ret) {
        printk (KERN_ERR "cannot register miscdev (err=%d)\n", ret);
		return ret;
    }

	return 0;
}

static void __exit mc_dma64_exit(void){
    misc_deregister(&mc_dma64_miscdev);
}

module_init(mc_dma64_init);
module_exit(mc_dma64_exit);

MODULE_AUTHOR("molchip");
MODULE_DESCRIPTION("mc Dma64 ioctrl Driver");
MODULE_LICENSE("GPL");

