#if defined(__KERNEL__)
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/interrupt.h>
#include <linux/iommu.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/firmware.h>
#include <linux/kobject.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/proc_fs.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/sizes.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of_reserved_mem.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/random.h>
#include <linux/semaphore.h>
#endif

#include "mol_lcm.h"
//#include "fy_common.h"
#include "vou_clk_v01_reg_def.h"
#include "dsi_host_v01_reg_def.h"
#include "dsi_phy_v01_reg_def.h"
#include "dsi_phy_v01_drv_mpi.h"
#include "fy_osal.h"
#include "os.h"

/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/**---------------------------------------------------------------------------*
*                Micro Define                    **
**----------------------------------------------------------------------------*/
#define MC_LCM_DEBUG 0

#define REG_TOP_RST_REG_BASE                     DMC_SYS_RST_BASE_ADDR
#define REG_TOP_CLK_CFG_BASE                     DMC_SYS_CLK_BASE_ADDR
#define REG_TOP_VOU_SEL_BASE                     DMC_VOU_SEL_BASE_ADDR
#define REG_VOU_BASE                             VOU_BASE_ADDR
#define REG_DIS_HOST_2LANE_BASE                  DIS_HOST_2LANE_BASE_ADDR
#define REG_DIS_HOST_4LANE_BASE                  DIS_HOST_4LANE_BASE_ADDR
#define REG_DSI_DPHY_BASE                        DSI_DPHYTX_BASE_ADDR
#define REG_PIN_MUX_BASE                         0x10200000

#ifndef ZERO_FILL_POINTER
#define ZERO_FILL_POINTER(v)                                    \
    do {                                                        \
        (void)memset(v, 0, sizeof(*v));                         \
    } while (0)
#endif /* ZERO_FILL_POINTER */

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
#if defined(OS_RTT)
fhosal_dev_t s_lcm_dev = {0x00};
#endif

lcm_handle_t *gHandle[2] = {NULL, NULL};
/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/
static int lcm0_id_cfg = 1;
static int lcm1_id_cfg = 1;
static int lcm0_id = 0;
static int lcm1_id = 4;

#ifndef MODULE
#if defined(__KERNEL__)
#define LCM_SETUP_CMDLINE_LEN 256
static char __initdata setup_cmd[LCM_SETUP_CMDLINE_LEN+1] = "";
static int __init parse_kern_cmdline(char *line)
{
    int result = 0;
    strncpy(setup_cmd, line, LCM_SETUP_CMDLINE_LEN);
    setup_cmd[LCM_SETUP_CMDLINE_LEN] = '\0';
    bits=NULL;
    result = sscanf(setup_cmd, "lcm0_id_cfg=%d,lcm1_id_cfg=%d,lcm0_id=%d,lcm1_id=%d", &lcm0_id_cfg, &lcm1_id_cfg, &lcm0_id, &lcm1_id);
    printf("lcm0_id_cfg=%d,lcm1_id_cfg=%d,lcm0_id=%d,lcm1_id=%d\n", lcm0_id_cfg, lcm1_id_cfg, lcm0_id, lcm1_id);
    return 1;
}
__setup("lcm=", parse_kern_cmdline);
#endif
#else
module_param(lcm0_id_cfg, int, S_IRUGO);
module_param(lcm1_id_cfg, int, S_IRUGO);
module_param(lcm0_id, int, S_IRUGO);
module_param(lcm1_id, int, S_IRUGO);
#endif

lcm_handle_t *get_lcm_handle(int lcm_id)
{
    lcm_handle_t * lcm_handle = NULL;

    if (1 < lcm_id) {
        printf("%s:%d, lcm_id:%d\n", __FUNCTION__, __LINE__, lcm_id);
    } else {
        lcm_handle = gHandle[lcm_id];
        #if MC_LCM_DEBUG
        printf("get gHandle[%d]:%px, handle:%px\n", lcm_id, gHandle[lcm_id], lcm_handle);
        #endif
    }

    return lcm_handle;
}

static int _set_lcm_handle(int lcm_id, lcm_handle_t *handle)
{
    gHandle[lcm_id] = handle;

    #if MC_LCM_DEBUG
    printf("set gHandle[%d]:%px, handle:%px\n", lcm_id, gHandle[lcm_id], handle);
    #endif

    return 0;
}

static int DRVI_LCM_ProcRead(fhosal_proc_entry_t *sqf)
{
    fhosal_seq_printf(sqf, ">>>----------------------dsi phy info----------------------<<<\n");

    dsi_phy_param_show(sqf, 0);
    dsi_phy_param_show(sqf, 1);

    fhosal_seq_printf(sqf, ">>>----------------------dsi phy info----------------------<<<\n");

    return 0;
}

#if defined(OS_RTT)
static char lcm_buf[128] = {0,};

static int _prase_prarm(char *cnStr, unsigned int *u32Dec)
{
    int s32Ret = 0;
    int i;
    unsigned int value = 0;

    if (NULL == cnStr || NULL == u32Dec) {
        printf("%s Null pointer !\n", __FUNCTION__);
        goto EXIT;
    }

    for (i = 0; i < 4; i++) {
        if (cnStr[i] >= '0' && cnStr[i] <= '9') {
            value = value * 10 + cnStr[i] - '0';
        } else {
        //printf("Invalid number\n");
            break;
        }
    }

    if (u32Dec) {
        *u32Dec = value;
        s32Ret = 0;
    }

EXIT:
    return s32Ret;
}

static int set_lcm(int argc, char *argv[])
{
    char *cmd[4] = {NULL};
    char *input = lcm_buf;
    char *key = NULL;
    int length = 0;
    int i = 0;
    int result = 0;
    lcm_handle_t *lcmHandlePtr = NULL;

    printf("set_lcm argc=%d\n", argc);

    rt_memset(input, 0, sizeof(128));

    if (argc == 1) {
        lcm0_id_cfg = 1;
        lcm1_id_cfg = 1;
        lcm0_id = 0;
        lcm1_id = 4;
    } else {
        cmd[0] = argv[1];
        cmd[1] = argv[2];
        cmd[2] = argv[3];
        cmd[3] = argv[4];

        if (cmd[3] != NULL) {
            length = rt_strlen(cmd[0]) + rt_strlen(cmd[1]) + rt_strlen(cmd[2]) + rt_strlen(cmd[3]) + 1;
            rt_sprintf(input, "%s%s%s%s", cmd[0], cmd[1], cmd[2], cmd[3]);
        } else if (cmd[2] != NULL) {
            length = rt_strlen(cmd[0]) + rt_strlen(cmd[1]) + rt_strlen(cmd[2]) + 1;
            rt_sprintf(input, "%s%s%s", cmd[0], cmd[1], cmd[2]);
        } else if (cmd[1] != NULL) {
            length = rt_strlen(cmd[0]) + rt_strlen(cmd[1]) + 1;
            rt_sprintf(input, "%s%s", cmd[0], cmd[1]);
        } else {
            length = rt_strlen(cmd[0]) + 1;
            rt_sprintf(input, "%s", cmd[0]);
        }

        input[length] = 0;
        printf("%s\n", input);
    }

    key = rt_strstr(input, "lcm0_id_cfg=");
    if (NULL != key) {
        result = _prase_prarm(key + sizeof("lcm0_id_cfg=")-1, (unsigned int *)&lcm0_id_cfg);
        if (result != 0)
            goto EXIT;
    }

    key = rt_strstr(input, "lcm0_id=");
    if (NULL != key) {
        result = _prase_prarm(key + sizeof("lcm0_id=")-1, (unsigned int *)&lcm0_id);
        if (result != 0)
            goto EXIT;
    }

    key = rt_strstr(input, "lcm1_id_cfg=");
    if (NULL != key) {
        result = _prase_prarm(key + sizeof("lcm1_id_cfg=")-1, (unsigned int *)&lcm1_id_cfg);
    if (result != 0)
        goto EXIT;
    }

    key = rt_strstr(input, "lcm1_id=");
    if (NULL != key) {
        result = _prase_prarm(key + sizeof("lcm1_id=")-1, (unsigned int *)&lcm1_id);
    if (result != 0)
        goto EXIT;
    }

    printf("| lcm0_id_cfg=%d, lcm1_id_cfg=%d, lcm0_id=%d, lcm1_id=%d |\n", lcm0_id_cfg, lcm1_id_cfg, lcm0_id, lcm1_id);

EXIT:
    if (1 == lcm0_id_cfg) { //for max loading test
        sfc_write(NULL, LCM_ID_MAIN, lcm0_id);
    }
    if (1 == lcm1_id_cfg) { //for max loading test
        sfc_write(NULL, LCM_ID_SUB,  lcm1_id);
    }

    for (i = 0; i < LCM_ID_MAX; i++) {
        lcmHandlePtr = get_lcm_handle(i);

        lcmHandlePtr->lcm_id_cfg = (0 == i)? lcm0_id_cfg : lcm1_id_cfg;
    }

    return 0;
}

MSH_CMD_EXPORT(set_lcm,  set lcm id);
#endif

static int mc_lcm_probe(void)
{
    int rtn = 0;
    int i = 0;
    lcm_handle_t *lcmHandle[2] = {NULL, NULL};
    fhosal_proc_entry_t * proc = NULL;

    printf("--[LCM]Version:[%s, %s]--\n",__DATE__, __TIME__);
    printf("|----------------------------------------------------|\n");
    printf("| lcm0_id_cfg=%d, lcm1_id_cfg=%d, lcm0_id=%d, lcm1_id=%d |\n", lcm0_id_cfg, lcm1_id_cfg, lcm0_id, lcm1_id);
    printf("| lcm0_id_cfg: 0-normal 1-assign 2-disable           |\n");
    printf("|----------------------------------------------------|\n\n");

    if (1 == lcm0_id_cfg) { //for max loading test
        sfc_write(NULL, LCM_ID_MAIN, lcm0_id);
    }
    if (1 == lcm1_id_cfg) { //for max loading test
        sfc_write(NULL, LCM_ID_SUB,  lcm1_id);
    }

    for (i = 0; i < LCM_ID_MAX; i++) {
        lcmHandle[i] = os_k_malloc(sizeof(lcm_handle_t), GFP_KERNEL);
        if (!lcmHandle[i]) {
            printf("lcm gHandle mem alloc failed\n");
            return -ENOMEM;
        }
        ZERO_FILL_POINTER(lcmHandle[i]);

        lcmHandle[i]->reg = os_k_malloc(sizeof(lcm_reg_base_t), GFP_KERNEL);
        if (!lcmHandle[i]->reg) {
            printf("lcm gHandle regbase mem alloc failed\n");
            return -ENOMEM;
        }
        ZERO_FILL_POINTER(lcmHandle[i]->reg);
        lcmHandle[i]->lcm_id_cfg = (0 == i)? lcm0_id_cfg : lcm1_id_cfg;
        rtn = _set_lcm_handle(i, lcmHandle[i]);
        lcmHandle[i] = NULL;
    }

    lcmHandle[LCM_ID_MAIN] = get_lcm_handle(LCM_ID_MAIN);
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_TOPRST]         = fhosal_ioremap_nocache(REG_TOP_RST_REG_BASE, 0x100);
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_TOPCLK]         = fhosal_ioremap_nocache(REG_TOP_CLK_CFG_BASE, 0x100);
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_VOU_SEL]        = fhosal_ioremap_nocache(REG_TOP_VOU_SEL_BASE, 0x100);

    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_VOU]            = fhosal_ioremap_nocache(REG_VOU_BASE, 0x3000);
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_DPHY]       = fhosal_ioremap_nocache(REG_DSI_DPHY_BASE, 0x2000);
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_HOST_4LANE] = fhosal_ioremap_nocache(REG_DIS_HOST_4LANE_BASE, 0x2000);
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_HOST_2LANE] = fhosal_ioremap_nocache(REG_DIS_HOST_2LANE_BASE, 0x2000);
    lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_PIN_MUX]        = fhosal_ioremap_nocache(REG_PIN_MUX_BASE, 0x1000);

    #if MC_LCM_DEBUG
    printf("LCM...reg top rst    0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_TOPRST], REG_TOP_RST_REG_BASE);
    printf("LCM...reg top clk    0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_TOPCLK], REG_TOP_CLK_CFG_BASE);
    printf("LCM...reg vou sel    0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_VOU_SEL], REG_TOP_VOU_SEL_BASE);
    printf("LCM...reg vou base   0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_VOU], REG_VOU_BASE);
    printf("LCM...reg phy base   0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_DPHY], REG_DSI_DPHY_BASE);
    printf("LCM...reg host0 base 0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_HOST_4LANE], REG_DIS_HOST_4LANE_BASE);
    printf("LCM...reg host1 base 0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_DSI_HOST_2LANE], REG_DIS_HOST_2LANE_BASE);
    printf("LCM...reg pin        0x%px, 0x%08x\n", lcmHandle[LCM_ID_MAIN]->reg->base[LCM_REGBASE_PIN_MUX], REG_PIN_MUX_BASE);
    #endif

    for (i = (LCM_ID_MAIN + 1); i < LCM_ID_MAX; i++) {
        lcmHandle[i] = get_lcm_handle(i);
        if ((NULL != lcmHandle[i])
            && (NULL != lcmHandle[i]->reg)) {
            memcpy(lcmHandle[i]->reg, lcmHandle[LCM_ID_MAIN]->reg, sizeof(lcm_reg_base_t));
        }
    }

    //regist the proc function
    proc = fhosal_create_proc_entry("lcm", NULL);

    if (NULL != proc) {
        proc->read  = DRVI_LCM_ProcRead;
        proc->write = NULL;
    }

    if (rtn) {
    }

    return 0;
}

static int mc_lcm_remove(void)
{
    int rtn = 0x00;
    int32_t i = 0;

    static lcm_handle_t * lcmHandle = NULL;

    for (i = 0; i < LCM_ID_MAX; i++) {
        lcmHandle = get_lcm_handle(i);
        if ( NULL != lcmHandle) {
            fhosal_kfree(lcmHandle->reg);
            fhosal_kfree(lcmHandle);
            lcmHandle = NULL;
            rtn = _set_lcm_handle(i, lcmHandle);
        }
    }

    printf("mc_lcm_remove\n");

    if (rtn) {
    }

    return 0;
}

/*
static const struct of_device_id mc_lcm_match[] = {
    { .compatible = "molchip,mc-lcm", },
    { .compatible = "mmp-lcm", },
    {},
};

MODULE_DEVICE_TABLE(of, mc_lcm_match);

static struct platform_driver mc_lcm_driver = {
    .probe = mc_lcm_probe,
    .remove = mc_lcm_remove,
    .driver = {
        .name = "mc_lcm",
        .of_match_table = mc_lcm_match,
    },
};
*/

#if defined(OS_LINUX)
static int DRVI_LCM_Open(struct inode* inode, struct file *file)
{
    int s32Ret = 0;

    return s32Ret;
}

static int DRVI_LCM_Close(struct inode* inode, struct file *file)
{
    int s32Ret = 0;

    return s32Ret;
}

static int DRVI_LCM_Read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int s32Ret = 0;

    return s32Ret;
}

static long DRVI_LCM_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int s32Ret = 0;

    return s32Ret;
}
#elif defined(OS_RTT)
static int DRVI_LCM_Open(void *inode, void *file)
{
    int s32Ret = 0;

    return s32Ret;
}

static int DRVI_LCM_Close(void *inode, void *file)
{
    int s32Ret = 0;

    return s32Ret;
}

static long DRVI_LCM_Ioctl(void *file, unsigned int cmd, unsigned long arg)
{
    int s32Ret = 0;

    return s32Ret;
}
#endif


#if defined(OS_LINUX)
static struct file_operations s_stLcmFileOp = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = DRVI_LCM_Ioctl,
    .open           = DRVI_LCM_Open,
    .release        = DRVI_LCM_Close,
    .read           = DRVI_LCM_Read,
};

static struct miscdevice lcm_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = "mc_lcm",
    .fops  = &s_stLcmFileOp,
};
#elif defined(OS_RTT)
static struct fhosal_fileops s_stLcmFileOp = {
    .unlocked_ioctl = DRVI_LCM_Ioctl,
    .open           = DRVI_LCM_Open,
    .release        = DRVI_LCM_Close,
//  .read           = DRVI_VO_Read,
};
#endif

int mc_lcm_init(void)
{
    int rtn = 0;
    rtn = mc_lcm_probe();

    #if defined(OS_LINUX)
    rtn = fhosal_misc_register(&lcm_miscdev);
    #elif defined(OS_RTT)
    {
        fhosal_memset(&s_lcm_dev,0,sizeof(s_lcm_dev));
        fhosal_memcpy(s_lcm_dev.name, "lcm", 3);
        s_lcm_dev.fops = &s_stLcmFileOp;
        rtn = fhosal_misc_register(&s_lcm_dev);
    }
    #endif

    EXIT_IF_FAIL(rtn, NULL, "lcm module register ~_~!");

    EXIT:

    return rtn;

}


void mc_lcm_exit(void)
{
    int result = 0;

    result =  mc_lcm_remove();

    #if defined(OS_LINUX)
    fhosal_misc_deregister(&lcm_miscdev);
    #elif defined(OS_RTT)
    fhosal_misc_deregister(&s_lcm_dev);
    #endif

    if (result) {
    }
}

#if defined(OS_LINUX)
module_init(mc_lcm_init);
module_exit(mc_lcm_exit);
//module_platform_driver(mc_lcm_driver);
//MODULE_LICENSE("Dual BSD/GPL");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Molchip VOU driver");
MODULE_ALIAS("vou module");
#else
INIT_COMPONENT_EXPORT(mc_lcm_init);
#endif


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
