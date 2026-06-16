#ifndef _FH_VGS_H_
#define _FH_VGS_H_

#include "fh_drv_common.h"
#include "fh_vgs2_mpipara.h"


#define VGS_IOC_MAGIC              'G'
#define IOC_VGS_QUERY_SIZE        _IOWR(VGS_IOC_MAGIC, 0 , FH_UINT32)
#define IOC_VGS_CREATE            _IOWR(VGS_IOC_MAGIC, 1 , VGS_SYS_INIT)
#define IOC_VGS_DESTORY           _IOWR(VGS_IOC_MAGIC, 2 , FH_VOID*)
#define IOC_VGS_SUBMIT            _IOWR(VGS_IOC_MAGIC, 3 , VGS_USR_CMD)
#define IOC_VGS_QUERY_HANDLE      _IOWR(VGS_IOC_MAGIC, 4 , VGS_USR_QUERY)
#define IOC_VGS_HASCREATE         _IOWR(VGS_IOC_MAGIC, 5 , FH_UINT32)
#define IOC_VGS_SET_MRGW          _IOWR(VGS_IOC_MAGIC, 6 , VGS_USR_MRG_W)
#define IOC_VGS_SET_OSD_LUT       _IOWR(VGS_IOC_MAGIC, 7 , VGS_USR_OSD_LUT)


typedef enum
{
	VGS_CMDID_CVT     = 0,
	VGS_CMDID_PM      = 1,
	VGS_CMDID_ROT     = 2,
	VGS_CMDID_JNT     = 3,
	VGS_CMDID_SCL     = 4,
	VGS_CMDID_CNE     = 5,
	VGS_CMDID_INTG    = 6,
	VGS_CMDID_ADD     = 7,
	VGS_CMDID_MRG     = 8,
	VGS_CMDID_OSD_CVT = 9,
	VGS_CMDID_OSD_OL  = 10,
	VGS_CMDID_BUTT,
}VGS_USR_CMDID;

typedef struct{
	unsigned int    width;        /*Image width*/
	unsigned int    height;       /*Image height*/
}PIC_SIZE;

typedef struct
{
	int x;
	int y;
}POINT;

typedef struct
{
	int x;
	int y;
	unsigned int width;
	unsigned int height;
}AREA;

typedef struct
{
	PIC_SIZE  max_size;
	MEM_INFO  mem_info;
}VGS_SYS_INIT;

typedef struct
{
	MEM_INFO  phyaddr;
	MEM_INFO  phyaddr_u;
	MEM_INFO  phyaddr_v;
	PIC_SIZE  size;
	POINT     start;
	FH_UINT32 pool_id;
}VGS_DATA;

typedef struct
{
	MEM_INFO     ydata;
	MEM_INFO     udata;
	MEM_INFO     vdata;
	PIC_SIZE     size;
	POINT        start;
	FH_UINT32    pool_id;
}VGS_DATA_DIV;

typedef struct
{
	VGS_DATA_DIV phyaddr_src;
	VGS_DATA_DIV phyaddr_dst;
	PIC_SIZE     op_size;
	FH_UINT32    src_format;
	FH_UINT32    src_comp;
	FH_UINT32    dst_format;
}VGS_USR_CVT;

typedef struct
{
	VGS_DATA     phyaddr_src;
	VGS_DATA     phyaddr_dst;
	FH_UINT32    src_format;
	FH_UINT32    src_comp;
	FH_UINT32    mode;		/* 0: pure color 1: mosaic */
	FH_UINT32    size;		/* size 0: 16x16，1: 32x32，2: 64x64，3: 128x128 */
	AREA         mask;
	FH_UINT32    comp[3];
}VGS_USR_PM;

typedef struct
{
	VGS_DATA     phyaddr_src;
	VGS_DATA     phyaddr_dst;
	PIC_SIZE     op_size;
	FH_UINT32    src_format;
	FH_UINT32    src_comp;
	FH_UINT32    src_pixw;
	FH_UINT32    rotang;
	FH_UINT32    rotmode;
	FH_UINT32    comp[3];
}VGS_USR_ROT;

typedef struct
{
	VGS_DATA  pic_src0;
	PIC_SIZE  op_size0;
	VGS_DATA  pic_src1;
	PIC_SIZE  op_size1;
	VGS_DATA  pic_dst;
	PIC_SIZE  op_size;
	FH_UINT32 src_format;
	FH_UINT32 src_comp;
	FH_UINT32 comp[3];
}VGS_USR_JNT;

typedef struct
{
	VGS_DATA  pic_src;
	PIC_SIZE  op_size_src;
	VGS_DATA  pic_dst;
	PIC_SIZE  op_size_dst;
	FH_UINT32 src_format;
	FH_UINT32 src_comp;
	FH_UINT32 comp[3];
}VGS_USR_SCL;

typedef struct
{
	VGS_DATA  pic_src;
	VGS_DATA  pic_dst;
	PIC_SIZE  op_size;
	FH_UINT32 src_format;
	FH_UINT32 mode;
	FH_UINT32 masken;
}VGS_USR_CNE;

typedef struct
{
	VGS_DATA  pic_src;
	VGS_DATA  pic_dst;
	PIC_SIZE  op_size;
	FH_UINT32 src_format;
	FH_UINT32 src_comp;
	FH_UINT32 comp[3];
}VGS_USR_INTG;

typedef struct
{
	VGS_DATA  pic_src0;
	VGS_DATA  pic_src1;
	VGS_DATA  pic_src2;
	VGS_DATA  pic_dst;
	PIC_SIZE  op_size;
	FH_UINT32 src_format;
	FH_UINT32 src_comp;
	FH_UINT32 mode;
	FH_UINT32 comp[3];
}VGS_USR_ADD;

typedef struct
{
	VGS_DATA  pic_src0;
	VGS_DATA  pic_src1;
	VGS_DATA  pic_src2;
	VGS_DATA  pic_w1;
	VGS_DATA  pic_w2;
	VGS_DATA  pic_dst;
	PIC_SIZE  op_size;
	FH_UINT32 src_format;
	FH_UINT32 src_comp;
	FH_UINT32 mode;
	FH_UINT32 avgrad;
	FH_UINT32 avgmode;
	FH_UINT32 wtype;
	FH_UINT32 absign;
	FH_UINT32 abmode;
	FH_UINT32 mulmode;
}VGS_USR_MRG;

typedef struct
{
	FH_UINT32 w1[128];
	FH_UINT32 w2[128];
}VGS_USR_MRG_W;

typedef struct
{
	FH_UINT32 color[256];
}VGS_USR_OSD_LUT;

typedef struct
{
	VGS_DATA  pic_src;
	VGS_DATA  pic_dst;
	PIC_SIZE  op_size;
	POINT     osd_start;
	FH_UINT32 pixw;
	FH_UINT32 rotang;
}VGS_USR_OSD_CVT;

typedef struct
{
	VGS_DATA_DIV  pic_src;
	VGS_DATA      osd;
	VGS_DATA_DIV  pic_dst;
	PIC_SIZE      op_size;
	FH_UINT32     src_format;
	FH_UINT32     src_comp;
	FH_UINT32     osdpixw;
	FH_UINT32     extmode;
	FH_UINT32     magiccoloren;
	FH_UINT32     alphaen;
	FH_UINT32     magiccolor;
	FH_UINT32     alpha;
}VGS_USR_OSD_OL;

#pragma pack (4)
typedef struct
{
	VGS_USR_CMDID  cmdid;
	FH_UINT32      prior;
	FH_BOOL        instant;
	FH_UINT64      handle;
	FH_UINT32      module;
	union
	{
		VGS_USR_PM pm;
		VGS_USR_CVT cvt;
		VGS_USR_ROT rot;
		VGS_USR_JNT jnt;
		VGS_USR_SCL scl;
		VGS_USR_CNE cne;
		VGS_USR_INTG intg;
		VGS_USR_ADD add;
		VGS_USR_MRG mrg;
		VGS_USR_OSD_CVT osd_cvt;
		VGS_USR_OSD_OL osd_ol;
	};
}VGS_USR_CMD;

typedef struct
{
	FH_UINT64      handle;
	FH_UINT32      module;
	FH_BOOL        block;
	FH_BOOL        finish;
	FH_UINT32      prior;
}VGS_USR_QUERY;
#pragma pack ()

#endif /*_FH_VGS_H_*/
