#ifndef __rpc_hdr_h__
#define __rpc_hdr_h__

struct xbus_ioc_hdr
{
    unsigned int   out_len_min; /*let compiler 4Bytes aligned*/
    unsigned short out_len_max;
    unsigned short in_len;
};

#define RPC_TABLE_ID_ACW      (0)
#define RPC_TABLE_ID_HUMAN    (1)
#define RPC_TABLE_ID_VMM      (2)
#define RPC_TABLE_ID_CONTROL  (3)
#define RPC_TABLE_ID_TINY_NN  (4)
#define RPC_TABLE_ID_ISP0     (5)
#define RPC_TABLE_ID_ISP1     (6)
#define RPC_TABLE_ID_ISP2     (7)
#define RPC_TABLE_ID_ISP3     (8)
#define RPC_TABLE_ID_ISP4     (9)
#define RPC_TABLE_ID_DSP0     (10)
#define RPC_TABLE_ID_DSP1     (11)
#define RPC_TABLE_ID_DSP2     (12)
#define RPC_TABLE_ID_DSP3     (13)
#define RPC_TABLE_ID_DSP4     (14)
#define RPC_TABLE_ID_DSP5     (15)
#define RPC_TABLE_ID_DSP6     (16)
#define RPC_TABLE_ID_DSP7     (17)
#define RPC_TABLE_ID_DSP8     (18)
#define RPC_TABLE_ID_DSP9     (19)
#define RPC_TABLE_ID_MAX      (20) /*not included, don't use it...*/

#define RPC_GRP_ID_NONE        (0)
#define RPC_GRP_ID_ACW_IOCTL   (1)
#define RPC_GRP_ID_HUMAN_IOCTL (2)
#define RPC_GRP_ID_MAX         (3) /*not included, don't use it...*/

#define RPC_PRIO_DEFAULT      (0) /*now, same with RPC_PRIO_MID*/
#define RPC_PRIO_HIGHEST      (1)
#define RPC_PRIO_HIGH         (2)
#define RPC_PRIO_MID          (3)
#define RPC_PRIO_LOW          (4)
#define RPC_PRIO_LOWEST       (5)
#define RPC_PRIO_EXT_SERV     (6) /*special for audio service,because it need large stack*/
#define RPC_PRIO_LVL_MAX      (31) /*include...*/


#define RET_NO_FUNC          (-38)  /* Invalid system call number */


#if 1

/*
 * version:  2bit
 * src:      3bit // 1:ARM,2:ARC0,3:ARC1
 * tgt:      3bit // 0:unkonw, 1:ARM,2:ARC0,3:ARC1

 * grp_id:   3bit
 * priority: 5bit

 * table_id: 7bit ==> MOD_ID_E
 * func_id:  9bit
 */


#define RPC_VERSION_BITS       (2)
#define RPC_FUNC_ID_BITS       (9)

#define ID_UNKOWN  0
#define ID_ARM  1
#define ID_ARC0 2
#define ID_ARC1 3
#define ID_MAX  3

#undef RPC_TABLE_ID_MAX
#define RPC_TABLE_ID_MAX      127 /* mod_id: not included, don't use it...*/


#define RPC_XID(version, src,tgt, grp_id, priority, mod, func_id) (((version)<<30) | ((src)<<27) | ((tgt)<<24)  | ((grp_id)<<21) | ((priority)<<16) | ((mod)<<9) | (func_id))
#if(defined(CONFIG_ARM))
#define RPC_ID(version, grp_id, table_id, func_id) RPC_XID(version, 1,0, grp_id, RPC_PRIO_DEFAULT, table_id, func_id)
#elif(defined(ARC0))
#define RPC_ID(version, grp_id, table_id, func_id) RPC_XID(version, 2,0, grp_id, RPC_PRIO_DEFAULT, table_id, func_id)
#elif(defined(ARC1))
#define RPC_ID(version, grp_id, table_id, func_id) RPC_XID(version, 3,0, grp_id, RPC_PRIO_DEFAULT, table_id, func_id)
#else
#error "Unkown SRC"
#endif

#define RPC_VERSIONX(ID)       (((unsigned int)(ID) >> 30) & 0x3)
#define RPC_SRC(ID)             (((unsigned int)(ID) >> 27) & 0x7)
#define RPC_TGT(ID)             (((unsigned int)(ID) >> 24) & 0x7)
#define RPC_GRP_ID(ID)         (((unsigned int)(ID) >> 21) & 0x7)
#define RPC_PRIO(ID)           (((unsigned int)(ID) >> 16) & 0x1f)
#define RPC_MOD_ID(ID)         (((unsigned int)(ID) >> 9)  & 0x7f)
#define RPC_TABLE_ID(ID)       RPC_MOD_ID(ID)

#define RPC_FUNC_ID(ID)        ((ID) & 0x1ff)
#define RPC_TABLE_FUNC_ID(ID)  ((ID) & 0xffff)

#else

/*
 * version:  8bit
 * grp_id:   6bit
 * priority: 4bit
 * table_id: 5bit
 * func_id:  9bit
 */

#define RPC_VERSION_BITS       (8)
#define RPC_FUNC_ID_BITS       (9)

#define RPC_XID(version, grp_id, priority, table_id, func_id) (((version)<<24) | ((grp_id)<<18) | ((priority)<<14) | ((table_id)<<9) | (func_id))
#define RPC_ID(version, grp_id, table_id, func_id) RPC_XID(version, grp_id, RPC_PRIO_DEFAULT, table_id, func_id)
#define RPC_VERSIONX(ID)       (((unsigned int)(ID) >> 24) & 0xff)
#define RPC_GRP_ID(ID)         (((unsigned int)(ID) >> 18) & 0x3f)
#define RPC_PRIO(ID)           (((unsigned int)(ID) >> 14) & 0x0f)
#define RPC_TABLE_ID(ID)       (((unsigned int)(ID) >> 9)  & 0x1f)
#define RPC_FUNC_ID(ID)        ((ID) & 0x1ff)
#define RPC_TABLE_FUNC_ID(ID)  ((ID) & 0x3fff)
#endif

struct rpc_hdr
{
    /*function ID to excecute on peer*/
    unsigned int ID;
    int          ret;

    unsigned char param[0];
};

typedef enum
{
    XBUS_PORT_INVALID = 0,

    XBUS_PORT_SYS = 1, // port for sys bind/vb ARM<=>ARC0<=>ARC1

    //the following defined ports for arc=>ARM only
    XBUS_PORT_A_2ARM = 2,
    XBUS_PORT_B_2ARM = 3,
    XBUS_PORT_C_2ARM = 4,
    XBUS_PORT_D_2ARM = 5,

    // port 3/4/5/6 port(ARM=>ARC) are reserved for xbus driver ioctrl(which isp/senser call it)


    //ports define for each driver(ARM=>ARC)
    XBUS_PORT_VOU = 7,
    XBUS_PORT_VOU_TIMEOUT = 8,
    XBUS_PORT_DMIC = 7,
    XBUS_PORT_DMIC_TIMEOUT = 9,
    XBUS_PORT_ACW = 7,
    XBUS_PORT_ACW_TIMEOUT = 10,
    XBUS_PORT_VPPU = 11,
    XBUS_PORT_VPPU_TIMEOUT = 12,
    XBUS_PORT_VGS = 13,
    XBUS_PORT_VPU = 15,
    XBUS_PORT_VPU_TIMEOUT = 16,
    XBUS_PORT_ACW_TIMEOUT_AO = 17,
    XBUS_PORT_JPEG = 14,
    XBUS_PORT_JPEGE_TIMEOUT = 18,

    XBUS_PORT_VICAP = 21,
    XBUS_PORT_VICAP_TIMEOUT = 23,

    XBUS_PORT_VENC = 22,
    XBUS_PORT_VENC_TIMEOUT  = 24,

    XBUS_PORT_I2S = 25,
    XBUS_PORT_I2S_RX_TIMEOUT = 26,
    XBUS_PORT_I2S_TX_TIMEOUT  = 27,
    
	XBUS_PORT_SRC = 28,
    XBUS_PORT_MAX=31
}XBUS_RPC_PORT;

#endif /*__rpc_hdr_h__*/
