/*******************************************************************************************
#  Copyright (c) 2023~2025 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_MM_CAMERA_H__
#define __QUA_MM_CAMERA_H__

#include "common/qua_mm_common.h"
#include "common/qua_mm_video_common_type.h"
#include "isp/isp_common.h"
#include "isp/isp_api.h"
#include "isp/isp_enum.h"
#include "isp/isp_sensor_if.h"

#define QUA_MM_CAMERA_DEV_VI  "camera.vi"
#define QUA_MM_CAMERA_DEV_ISP "camera.isp"
#define QUA_MM_CAMERA_DEV_SENSOR "camera.sensor"

typedef enum
{
    QUA_VI_SNS0 = 0, //!< sensor0的序号
    QUA_VI_SNS1 = 1, //!< sensor1的序号
    QUA_VI_SNS2 = 2, //!< sensor2的序号
    QUA_VI_SNS3 = 3, //!< sensor3的序号
}qua_vi_sns_idx_t;

typedef enum 
{
    QUA_VI_PIPELINE_MIPI0 = 0,   //!< MIPI0序号，本项目对应QUA_VI_SNS0
    QUA_VI_PIPELINE_MIPI1 = 1,   //!< MIPI1序号，本项目对应QUA_VI_SNS1
    QUA_VI_PIPELINE_DVP = 2,     //!< DVP序号，本项目对应QUA_VI_SNS2
    QUA_VI_PIPELINE_TESTMODE = 3,    //!< 在线模式QUA_VI测试模式，离线模式下不支持
}qua_vi_idx_t;

typedef enum 
{
    QUA_VI_STITCH_FREE_RUN     = 0,     //!< sns非同步模式时的拼接
    QUA_VI_STITCH_SYNC_MODE     = 1,     //!< sns同步模式时的拼接
    QUA_VI_STITCH_LARGE_PIC_MODE = 2,     //!< 硬件大图处理模式
    QUA_VI_STITCH_BUTT,
}qua_vi_stitch_mode_t;

typedef enum 
{
    QUA_VI_WORK_MODE_ONLINE     = 0,     //!< QUA_VI工作在在线模式
    QUA_VI_WORK_MODE_OFFLINE    = 1,     //!< QUA_VI工作在离线模式
    QUA_VI_WORK_MODE_BUTT,
}qua_vi_work_mode_t;

typedef enum 
{
    QUA_VI_SFRAME = 0,   //!< 短帧通道号
    QUA_VI_LFRAME = 1,   //!< 长帧通道号
}qua_vi_wdr_chan_t;


typedef enum  {
    QUA_VI_BAYER_RGGB = 0x0, //!< 色彩模式RGGB
    QUA_VI_BAYER_GRBG = 0x1, //!< 色彩模式GRBG
    QUA_VI_BAYER_BGGR = 0x2, //!< 色彩模式BGGR
    QUA_VI_BAYER_GBRG = 0x3, //!< 色彩模式GBRG
    QUA_VI_BAYER_TYPE_DUMMY=0xffffffff,
} qua_vi_bayer_type_t;

typedef enum  {
    QUA_VI_VSYNC_ONLY = 0,  //!< 只有垂直同步信号
    QUA_VI_VSYNC_HSYNC = 1,  //!< 垂直同步信号 + 水平同步信号
} qua_vi_sync_mode_t;

typedef enum  {
    QUA_VI_VSYNC_TRIGGER_INTER = 0,  //!< 周期性发送vsync和hsync
    QUA_VI_VSYNC_TRIGGER_EXTER = 1,  //!< 外部触发发送一个周期的vsync和hsync
} qua_vi_sync_trigger_mode_t;

typedef enum  {
    QUA_VI_DATA_IN_RAW12 = 0,      //!< 输入的数据格式为RAW12
    QUA_VI_DATA_IN_RAW16 = 1,      //!< 输入的数据格式为RAW16
    QUA_VI_DATA_IN_YUV16 = 2,      //!< 输入的数据格式为YUV16
    QUA_VI_DATA_IN_YUYV = 3,      //!< 输入的数据格式为YUYV
    QUA_VI_DATA_IN_UYVY = 4,      //!< 输入的数据格式为UYVY
    QUA_VI_DATA_IN_BUTT,
} qua_vi_input_data_type_t;

typedef enum  {
    QUA_VI_DVP_BIT_MODE_16BIT_YUV = 0,    //!< DVP输入格式为YUV16
    QUA_VI_DVP_BIT_MODE_12BIT_RAW = 1,    //!< DVP输入格式为RAW12
    QUA_VI_DVP_BIT_MODE_10BIT_RAW = 2,    //!< DVP输入格式为RAW10
}qua_vi_dvp_in_bit_mode_t;

typedef enum  {
    QUA_VI_BT656_BIT_MODE_16BIT_YUV = 0,    //!< BT656输入格式为YUV16
    QUA_VI_BT656_BIT_MODE_8BIT_YUV = 1,    //!< BT656输入格式为RAW12
}qua_vi_bit656_in_bit_mode_t;


typedef enum 
{
    QUA_VI_DVP_SYNC_POL_HIGH = 0,     //!< 同步信号高有效
    QUA_VI_DVP_SYNC_POL_LOW  = 1,     //!< 同步信号低有效
}qua_vi_sync_pol_t;


typedef struct
{
    QUA_BOOL      cut_enable;  //!< 裁剪的使能寄存器。取值范围：[0~1]
    qua_rect_t    rect;      //!< 裁剪的偏移及裁剪后的宽高。取值范围：[qua_rect_t]
}qua_vi_crop_cfg_t;

typedef struct
{
    QUA_U8 priority;    //!< 离线遍历优先级。取值范围：[0~0xf]
}qua_vi_offline_cfg_t;

typedef struct
{
    qua_vi_bayer_type_t     bayer_type;   //!< bayer pattern。取值范围：[qua_vi_bayer_type_t]
    qua_vi_work_mode_t      work_mode;    //!< 配置当前QUA_VI的工作模式。取值范围：[qua_vi_work_mode_t]
    QUA_U8                  wdr_mode;     //!< 输入sensor的线性还是wdr配置。取值范围：[0~1]
    qua_size_t              in_size;      //!< 输入幅面。取值范围：[qua_size_t]
    qua_vi_crop_cfg_t       crop_size;    //!< 裁剪后幅面。取值范围：[qua_vi_crop_cfg_t]
    qua_vi_offline_cfg_t    offline_cfg;  //!< 离线下的配置。取值范围：[qua_vi_offline_cfg_t]
    QUA_U8                  bit_width;     // 输入的pixel宽度 | [FH_UINT8]
}qua_vi_dev_attr_t;

typedef struct
{
    QUA_BOOL                   using_vb;       //!< 是否使用VB缓存逻辑。取值范围：[0~1]
    qua_vi_input_data_type_t   data_type_in;   //!< SENSOR输入该模块的数据格式，本芯片仅DVP有效。取值范围： [qua_vi_input_data_type_t]
    qua_vi_work_mode_t         work_mode;     //!< 配置当前QUA_VI的工作模式，影响内存分配。取值范围：[qua_vi_work_mode_t]
    qua_size_t                 size;         //!< 图像幅面, 会影响内存分配, 故需按应用需要的最大幅面配置。取值范围：[qua_size_t]
}qua_vi_attr_t;

typedef struct
{
    qua_vi_dvp_in_bit_mode_t    bit_mode;        //!< 配置当前输出模式是raw还是yuv等。取值范围：[qua_vi_dvp_in_bit_mode_t]
    qua_vi_sync_pol_t           hsync_polarity;  //!< DVP水平同步信号极性。取值范围：[qua_vi_sync_pol_t]
    qua_vi_sync_pol_t           vsync_polarity;  //!< DVP垂直同步信号极性。取值范围：[qua_vi_sync_pol_t]
    QUA_U8                      io_remap[16];    //!< 对应的16个管脚的映射关系。取值范围：[0~0xf]
}qua_vi_dvp_cfg_t;

typedef struct
{
    qua_vi_bit656_in_bit_mode_t    bit_mode;        //!< 配置当前输出模式是raw还是yuv等。取值范围：[qua_vi_dvp_in_bit_mode_t]
    qua_vi_sync_pol_t              hsync_polarity;  //!< DVP水平同步信号极性。取值范围：[qua_vi_sync_pol_t]
    qua_vi_sync_pol_t              vsync_polarity;  //!< DVP垂直同步信号极性。取值范围：[qua_vi_sync_pol_t]
    QUA_U8                         io_remap[16];    //!< 对应的16个管脚的映射关系。取值范围：[0~0xf]
}qua_vi_bt656_cfg_t;

typedef struct
{
    qua_vi_sync_pol_t           hsync_polarity;  //!< DVP水平同步信号极性。取值范围：[qua_vi_sync_pol_t]
    qua_vi_sync_pol_t           vsync_polarity;  //!< DVP垂直同步信号极性。取值范围：[qua_vi_sync_pol_t]
    QUA_U8                      io_revert;       //!< dvp管脚是否revert
    QUA_U32 ir_fs_high_period;                    //!<fs高电平持续时间
    QUA_U32 ir_fs_how_period;                     //!<fs低电平持续时间
    QUA_U32 ir_reg_output_delay_period;           //!<ir静态寄存器输出延迟时间
    QUA_U32 ir_ooc_output_delay_period;           //!<ir ooc输出延迟时间
    QUA_U32 ir_ooc_tline_active_pixel;            //!<ir ooc行有效时间
    QUA_U32 ir_ooc_tline_inactive_period;         //!<ir ooc行间隔时间
    QUA_U32 ir_ooc_tline_active_line;             //!<ir ooc行有效行数
    QUA_U32 ir_clk_tx_inv_en;                     //!<输出给IR传感器的时钟是否反向
    QUA_U32 en_ir_gray2bin;                       //!<grap码转二进制使能
}qua_vi_ir_cfg_t;


typedef struct
{	
	QUA_U8    group_id; 
    QUA_U8    stitch_mode;  //!< 该路的拼接模式。取值范围：[0~2]
    QUA_BOOL  main_dev;      //!< 该路是否为mainPipe。取值范围：[0~1]
    QUA_BOOL  stitch_en;     //!< 该路是否开启拼接时能。取值范围：[0~1]
}qua_vi_stitch_attr_t;

typedef struct
{
    QUA_U8    stitch_mode;  //!< 拼接模式
    QUA_BOOL  stitch_en;     //!< 是否拼接
    QUA_U32   main_dev_id;  //!< 主通路的id
    QUA_U32   dev_num;     //!< 拼接组的通路个数
    QUA_U32   dev_id[4];    //!<各个通路的id
}qua_vi_stitch_grp_attr_t;

typedef struct
{
    QUA_U8 dev_id;	 //!< 设备号。取值范围：[0~4]
    qua_vi_attr_t dev_attr;  //!< 设备参数。取值范围：[qua_vi_dev_attr_t]
    QUA_U32 buf_num; // 获取到的缓存块数 | [0~0xffffffff]
    QUA_U32 buf_size; // 获取到的缓存总大小 | [0~0xffffffff]
} qua_vi_buf_cfg;

typedef struct
{
    QUA_U8 dev_num;         //!< 总共的QUA_VI通路。取值范围：[0~4]
    qua_vi_buf_cfg buf_cfg[4];
}qua_vi_buf_size_attr_t;

typedef struct
{
    qua_vi_sync_mode_t sync_mode;         //!< 选择vsync only还是vsync + hsync
    QUA_U16   vsync_period;              //!< 垂直触发信号的一个周期的cycle数。取值范围：[0~0x1fff]
    QUA_U16   vsync_duty;                //!< 垂直触发信号的有效电平cycle数  。取值范围：[0~0x1fff]
    QUA_BOOL   vsync_polatiry;              //!< 垂直有效电平的极性。取值范围：[0~1]
    QUA_U16   hsync_period;              //!< 水平触发信号的一个周期的cycle数。取值范围：[0~0x1fff]
    QUA_U16   hsync_duty;                //!< 水平触发信号的有效电平cycle数  。取值范围：[0~0x1fff]
    QUA_BOOL  hsync_polatiry;              //!< 水平有效电平的极性。取值范围：[0~1] 0:高电平有效、1:低电平有效
    qua_vi_sync_trigger_mode_t sync_trigger_mode;//!<0:根据htotal/vtotal产生hsync和vysnc 1:根据外部时钟产生hsync和vsync
}qua_vi_frame_seq_cfg_t;

typedef struct
{
    QUA_BOOL ready;
    QUA_ULONG data_buf_addr;
    QUA_U32 check_sum;
    QUA_U32 start_line;
    QUA_U32 fifo_depth;
}qua_isp_ecdc_info_t;

typedef struct
{
    QUA_U64 time_stamp;     //!< QUA_VI启动帧的时间戳
    QUA_U64 frame_id;       //!< 传递过来的frame id
    QUA_U32 ec_para;
    qua_size_t pic;
    qua_isp_ecdc_info_t sf_info;
    qua_isp_ecdc_info_t lf_info;
    QUA_U32  reserved[16];
}qua_isp_src_info_t;


typedef struct
{
    qua_isp_ecdc_info_t isp_ecdc_info;//!<ecdc信息
    QUA_ULONG buf_addr_phy; //!<指向整个buf的头
    QUA_ULONG  buf_vir_addr; //!< 整个buf的头的虚拟地址，同一个进程空间可直接使用。取值范围：[QUA_U32]
    QUA_ULONG addr_phy;       //!< 数据的物理地址。取值范围：[QUA_ULONG]
    QUA_ULONG  vir_addr;    //!< 数据应用层的虚拟地址，同一个进程空间可直接使用。取值范围：[QUA_U32]
    QUA_U32  crc;        //!< 数据校验值  。取值范围：[0~0xffffffff]
    QUA_U64  time_stamp;  //!< 数据获取的时间戳。取值范围：[0~0xffffffffffffffff]
    QUA_U32  buf_size;    //!< 获取到的离线数据的缓存大小。取值范围：[0~0xffffffff]
    QUA_U32 line_stride;  // 一行的数据量，单位是字节 | [0~0xffffffff]
}qua_vi_stream_com_t;

typedef struct
{
    QUA_U8 dev_id;
    QUA_BOOL v2;        //!< 是否使用升级版获取缓存，未及时取走时下次可立刻获取到。取值范围：[0~1]
    QUA_BOOL lock;       //!< 是否加锁。取值范围：[0~1]
    QUA_BOOL block;      //!< 是否使用阻塞，阻塞支持线程调度。取值范围：[0~1]
    QUA_U32 time_out;  //!< 超时时间，阻塞和非阻塞都可以配置超时时间，单位是ms。取值范围：[0~0xffffffff]
    QUA_U32 pool_id;
    QUA_U32 line_stride;   //!< 一行的数据量，单位是字节。取值范围：[0~0xffffffff]
    QUA_U64 frm_cnt;   //!< 帧号。取值范围：[0~0xffffffff]
    qua_size_t pic;      //!< 图像幅面相关的配置。取值范围：[QUA_VI_SIZE_S]
    qua_vi_stream_com_t sf;  //!< 短帧的数据流。取值范围：[qua_vi_stream_com_t]
    qua_vi_stream_com_t lf;  //!< 长帧的数据流。取值范围：[qua_vi_stream_com_t]
    QUA_U32 subscribe_info;
}qua_vi_stream_t;

typedef struct
{
    QUA_BOOL  enable;        //!< 当前通道是否使能。取值范围：[0~1]
    QUA_U32   sof_cnt;      //!< 帧开始信号计数。取值范围：[0~0xffffffff]
    QUA_U32   eof_cnt;      //!< 帧结束信号计数。取值范围：[0~0xffffffff]
    QUA_U32   wr_done_cnt;   //!< 帧写完计数。取值范围：[0~0xffffffff]
    QUA_U16   frame_rate;   //!< sensor输出帧率。取值范围：[0~0xffff]
}qua_vi_status_t;

typedef struct
{
    QUA_U32 ir_config_reg0_31_0;
    QUA_U32 ir_config_reg0_63_32;
    QUA_U32 ir_config_reg0_95_64;
    QUA_U32 ir_config_reg0_127_96;
    QUA_U32 ir_config_reg1_31_0;
    QUA_U32 ir_config_reg1_63_32;
    QUA_U32 ir_config_reg1_95_64;
    QUA_U32 ir_config_reg1_127_96;
    QUA_U32 ir_config_reg2_31_0;
    QUA_U32 ir_config_reg2_63_32;
    QUA_U32 ir_config_reg2_95_64;
    QUA_U32 ir_config_reg2_127_96;
}qua_vi_ir_regs_t;

typedef struct
{
    QUA_U16 buffer_cnt;//!<最大10，默认3
}qua_vi_buf_cnt_t;

typedef struct
{
    QUA_U32 subscribe_info;//!<最大10，默认3
}qua_vi_subscribe_info_t;

typedef struct
{
    QUA_U32 pause;
}qua_vi_pause_t;

typedef struct
{
    QUA_U32 valid_cnt;
    QUA_ULONG phy_addr[20];
}qua_vi_free_buf_t;


enum{
    QUA_VI_PEAKLOAD_0 = 16000000,
    QUA_VI_PEAKLOAD_1 = 66000000,
    QUA_VI_PEAKLOAD_2 = 110000000,
    QUA_VI_PEAKLOAD_3 = 133000000,
    QUA_VI_PEAKLOAD_4 = 198000000,
    QUA_VI_PEAKLOAD_5 = 266000000,
    QUA_VI_PEAKLOAD_6 = 333000000,
    QUA_VI_PEAKLOAD_7 = 396000000,
};

typedef struct{
    QUA_U32 clk_rate[8];
}qua_vi_scenario_attr_t;

typedef struct
{
    QUA_U32 cur_scenario_id;
    qua_vi_scenario_attr_t scenario_data[4];
}qua_vi_low_power_info_t;

typedef struct
{
    QUA_U32 width;
    QUA_U32 height;
    QUA_U32 fps;
    QUA_U32 pclk;
}qua_vi_size_fps_t;

typedef struct
{
    QUA_U32 pclk_set;//blank is set;0:not value 1: valid value
    qua_vi_size_fps_t size_fps_info[3];//mipi0 mipi1 dvp
}qua_vi_peak_load_info_t;


typedef struct
{
    QUA_U8 vi_offline_enable;  //0: disable; 1:enable
}qua_vi_offline_t;

typedef struct
{
    QUA_U32 buf_size;
}qua_vi_buf_size_t;

typedef struct qua_mm_camera_module_t {
    struct qua_mm_module_t parent;
} qua_mm_camera_module_t;

typedef struct qua_mm_vi_device_t {
    struct qua_mm_device_t parent;

    QUA_S32 (*set_peak_load)(qua_vi_peak_load_info_t *peak_load);
    QUA_S32 (*set_buf_size)(QUA_U32 dev_id, qua_vi_buf_size_t *buf_size_attr);
    QUA_S32 (*get_buf_size)(QUA_U32 dev_id, qua_vi_buf_size_attr_t *buf_size_attr);	
    QUA_S32 (*set_buf_cnt)(QUA_U32 dev_id, qua_vi_buf_cnt_t *buf_cnt);
    QUA_S32 (*set_scribe_info)(QUA_U32 dev_id, qua_vi_subscribe_info_t *subscribe_info);
    QUA_S32 (*set_pause)(QUA_U32 dev_id, qua_vi_pause_t *pause_info);
    QUA_S32 (*get_free_buf)(QUA_U32 dev_id, qua_vi_free_buf_t *free_buf);
    QUA_S32 (*set_offline)(QUA_U32 dev_id, qua_vi_offline_t *offline);
    QUA_S32 (*set_phy_lane)(QUA_U32 dev_id, int num);

    QUA_S32 (*init_dev)(QUA_U32 dev_id, qua_vi_attr_t *dev_attr);
    QUA_S32 (*exit_dev)(QUA_U32 dev_id);
    QUA_S32 (*set_attr)(QUA_U32 dev_id, qua_vi_dev_attr_t *vi_attr);
    QUA_S32 (*get_attr)(QUA_U32 dev_id, qua_vi_dev_attr_t *vi_attr);
    QUA_S32 (*set_pipe_crop)(QUA_U32 dev_id, qua_vi_wdr_chan_t wdr_chn, qua_vi_crop_cfg_t *crop_cfg);
    QUA_S32 (*get_pipe_crop)(QUA_U32 dev_id, qua_vi_wdr_chan_t wdr_chn, qua_vi_crop_cfg_t *crop_cfg);
    QUA_S32 (*set_dvp_cfg)(QUA_U32 dev_id, qua_vi_dvp_cfg_t *dvp_cfg);
    QUA_S32 (*get_dvp_cfg)(QUA_U32 dev_id, qua_vi_dvp_cfg_t *dvp_cfg);
    QUA_S32 (*set_bt656_cfg)(QUA_U32 dev_id, qua_vi_bt656_cfg_t *bt656_cfg);
    QUA_S32 (*get_bt656_cfg)(QUA_U32 dev_id, qua_vi_bt656_cfg_t *bt656_cfg);
    QUA_S32 (*set_ir_cfg)(QUA_U32 dev_id, qua_vi_ir_cfg_t *ir_cfg);
    QUA_S32 (*get_ir_cfg)(QUA_U32 dev_id, qua_vi_ir_cfg_t *ir_cfg);
    QUA_S32 (*register_stitch_dev)(QUA_U32 dev_id, qua_vi_stitch_attr_t *stitch_attr);
    QUA_S32 (*unregister_stitch_dev)(QUA_U32 dev_id);
    QUA_S32 (*set_stitch_grp_attr)(QUA_U8 group_id, qua_vi_stitch_grp_attr_t *stitch_attr);

    QUA_S32 (*set_sns_clk)(qua_vi_sns_idx_t sns_idx, QUA_U32 clk);
    QUA_S32 (*set_sns_clk_en)(qua_vi_sns_idx_t sns_idx, QUA_BOOL en);
    QUA_S32 (*set_frame_seq_gen_cfg)(qua_vi_sns_idx_t sns_idx, qua_vi_frame_seq_cfg_t *frame_seq_cfg);
    QUA_S32 (*get_frame_seq_gen_cfg)(qua_vi_sns_idx_t sns_idx, qua_vi_frame_seq_cfg_t *frame_seq_cfg);
    QUA_S32 (*set_frame_seq_gen_en)(QUA_U16 en_cfg);
    QUA_S32 (*get_frame_seq_gen_en)(QUA_U16 *en_cfg);
    QUA_S32 (*get_stream)(QUA_U32 dev_id, qua_vi_stream_t *stream);
    QUA_S32 (*release_stream)(QUA_U32 dev_id, qua_vi_stream_t *stream);
    QUA_S32 (*get_status)(qua_vi_sns_idx_t sns_idx, qua_vi_status_t *status);
    QUA_S32 (*set_ir_regs)(qua_vi_ir_regs_t *ir_regs);
    QUA_S32 (*get_ir_regs)(qua_vi_ir_regs_t *ir_regs);
    QUA_S32 (*set_ir_ooc_addr)(QUA_ULONG addr_phy);
    QUA_S32 (*get_ir_ooc_addr)(QUA_ULONG *addr_phy);

} qua_mm_vi_device_t;

typedef struct qua_mm_isp_device_t {
    struct qua_mm_device_t parent;
    struct isp_sensor_if *sensor_if;

    QUA_S32 (*init_mem)(QUA_U32 dev_id, ISP_MEM_INIT *mem_info);
    QUA_S32 (*get_bin_addr)(QUA_U32 dev_id, ISP_PARAM_CONFIG *param_config);
    QUA_S32 (*set_cis_clk)(QUA_U32 dev_id, QUA_U32 cis_clk);
    QUA_S32 (*en_cis_clk)(QUA_U32 dev_id, QUA_BOOL en);
    QUA_S32 (*set_vi_attr)(QUA_U32 dev_id, const ISP_VI_ATTR_S *vi_attr);
    QUA_S32 (*get_vi_attr)(QUA_U32 dev_id, ISP_VI_ATTR_S *vi_attr);
    QUA_S32 (*init)(QUA_U32 dev_id);
    QUA_S32 (*load_isp_param)(QUA_U32 dev_id, QUA_CHAR *isp_param_buff);
    QUA_S32 (*pause)(QUA_U32 dev_id);
    QUA_S32 (*resume)(QUA_U32 dev_id);
    QUA_S32 (*run)(QUA_U32 dev_id);
    QUA_S32 (*exit)(QUA_U32 dev_id);
    QUA_S32 (*reg_sensor_cb)(QUA_U32 dev_id, QUA_U32 sensor_id, struct isp_sensor_if *sns_if);
    QUA_S32 (*unreg_sensor_cb)(QUA_U32 dev_id, QUA_U32 sensor_id);
    QUA_S32 (*init_sensor)(QUA_U32 u32DevId, Sensor_Init_t *init_cfg);
    QUA_S32 (*set_sensor_fmt)(QUA_U32 dev_id, QUA_U32 format);
    QUA_S32 (*kick_sensor)(QUA_U32 dev_id);
    QUA_S32 (*set_sensor_reg)(QUA_U32 dev_id, QUA_U16 addr,QUA_U16 data);
    QUA_S32 (*get_sensor_reg)(QUA_U32 dev_id, QUA_U16 addr,QUA_U16 *data);

    QUA_S32 (*adv_init_sensor)(QUA_U32 dev_id, struct isp_sensor_if* sns_if);
    QUA_S32 (*adv_init)(QUA_U32 dev_id);
    QUA_S32 (*adv_set_awb_gain)(QUA_U32 dev_id,QUA_U32 mode);
    QUA_S32 (*adv_get_awb_gain)(QUA_U32 dev_id,QUA_U32 *awb_gain);
    QUA_S32 (*adv_set_ae_mode)(QUA_U32 dev_id, QUA_U32 mode, QUA_U32 intt_us, QUA_U32 gain_level);
    QUA_S32 (*adv_get_ae_mode)(QUA_U32 dev_id, QUA_U32 *intt_us, QUA_U32 *gain_level);
    QUA_S32 (*adv_set_ltm_cfg)(QUA_U32 dev_id, QUA_U32 mode, QUA_U32 level);
    QUA_S32 (*adv_get_ltm_cfg)(QUA_U32 dev_id, QUA_U32 *mode, QUA_U32 *level);
    QUA_S32 (*adv_set_brightness)(QUA_U32 dev_id, QUA_U32 value);
    QUA_S32 (*adv_get_brightness)(QUA_U32 dev_id, QUA_U32 *pValue);
    QUA_S32 (*adv_set_contrast)(QUA_U32 dev_id, QUA_U32 mode, QUA_U32 value);
    QUA_S32 (*adv_get_contrast)(QUA_U32 dev_id, QUA_U32 *mode, QUA_U32 *value);
    QUA_S32 (*adv_set_saturation)(QUA_U32 dev_id, QUA_U32 mode, QUA_U32 value);
    QUA_S32 (*adv_get_saturation)(QUA_U32 dev_id, QUA_U32 *mode, QUA_U32 *value);
    QUA_S32 (*adv_set_sharpeness)(QUA_U32 dev_id, QUA_U32 mode, QUA_U32 value);
    QUA_S32 (*adv_get_sharpeness)(QUA_U32 dev_id, QUA_U32 *mode, QUA_U32 *value);
    QUA_S32 (*adv_set_chroma)(QUA_U32 dev_id, QUA_U32 value);
    QUA_S32 (*adv_get_chroma)(QUA_U32 dev_id, QUA_U32 *pValue);
    QUA_S32 (*adv_set_mirror_flip)(QUA_U32 dev_id, QUA_U32 grp_idx, QUA_U32 mirror, QUA_U32 flip);
    QUA_S32 (*adv_get_mirror_flip)(QUA_U32 dev_id, QUA_U32 *mirror, QUA_U32 *flip);
    QUA_S32 (*adv_set_color_mode)(QUA_U32 dev_id, QUA_U32 mode);
    QUA_S32 (*adv_get_color_mode)(QUA_U32 dev_id, QUA_U32 *mode);
    QUA_CHAR* (*adv_version)(QUA_U32 print_enable);
    QUA_S32 (*adv_set_3d_denoise)(QUA_U32 dev_id, QUA_U32 mode, QUA_U32 value);
    QUA_S32 (*adv_get_3d_denoise)(QUA_U32 dev_id, QUA_U32 *mode, QUA_U32 *value);
    QUA_S32 (*adv_set_2d_denoise)(QUA_U32 dev_id, QUA_U32 mode, QUA_U32 value);
    QUA_S32 (*adv_get_2d_denoise)(QUA_U32 dev_id, QUA_U32 *mode, QUA_U32 *value);
    QUA_S32 (*get_awb_default_cfg)(QUA_U32 dev_id, AWB_DEFAULT_CFG *awb_cfg);
    QUA_S32 (*set_awb_default_cfg)(QUA_U32 dev_id, AWB_DEFAULT_CFG *awb_cfg);
    QUA_S32 (*get_stream)(QUA_U32 dev_id, QUA_S32 timeout, qua_video_frame_info_t *stream);
    QUA_S32 (*release_stream)(QUA_U32 dev_id, qua_video_frame_info_t *stream);

} qua_mm_isp_device_t;

typedef struct qua_mm_sensor_device_t {
    qua_mm_device_t parent;

    QUA_S32 (*open_sensor)(QUA_U32 dev_id, const char *sensor_name, struct isp_sensor_if **sns_if);
    QUA_S32 (*close_sensor)(QUA_U32 dev_id);
} qua_mm_sensor_device_t;
#endif // __QUA_MM_CAMERA_H__