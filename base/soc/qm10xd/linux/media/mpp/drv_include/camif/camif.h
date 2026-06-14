#ifndef CAMIF_H
#define CAMIF_H
#include <cam_common.h>
#include <fh_type.h>
#include <fy_osal.h>
#include <camif_api.h>


#define DRV_CAMIF_VERSION "0.37"

typedef enum{
    CAMIF_PIXART_TYPE_PAG7920LT,
    CAMIF_PIXART_TYPE_NUM
}Camif_Pixart_Type_E;

typedef enum{
    CAMIF_BUF_TYPE_AE = 1 << 0,
    CAMIF_BUF_TYPE_AWB = 1 << 1,
    CAMIF_BUF_TYPE_RAWEC = 1 << 2,
}Camif_Buf_Type_E;

typedef struct
{
    UINT8 hsync_pol; //0:high; 1:low
    UINT8 vsync_pol; //0:high; 1:low
    UINT8 dvpin_mode; //0:normal 1:hfp
    UINT32 dvpin_hfp;  //跳过多少个有效数据
    UINT8 dvp_inverted_pad_enable;//inverted_pad使能
}Camif_Dvp_Conf_S;

typedef struct
{
    UINT8 hsync_pol; //0:high; 1:low
    UINT8 vsync_pol; //0:high; 1:low
    UINT8 dvp_inverted_pad_enable;//inverted_pad使能
    UINT8 p2s_pin_num; //多少个pin转8bit;//1,2,4
}Camif_P2s_Conf_S;

typedef struct
{
    UINT8 sck_inv_cfg;//上升沿下降沿取反
    UINT8 sync_code_mid_byte_mode;//mtk模式: 0:8h00, 1:8hff
    UINT8 sync_code_led_byte;
    UINT8 sync_code_lst_byte;
    UINT8 sync_code_fed_byte;
    UINT8 sync_code_fst_byte;
}Camif_Scb_Conf_S;

typedef struct
{
    UINT8 sck_inv_cfg;//上升沿下降沿取反
    UINT8 sync_code_mid_byte_mode;//mtk模式: 0:8h00, 1:8hff
    UINT8 spi_bit_mode;//0:8bits 1:10bits
    UINT8 spi_cap_mode;//spi模式：0单边沿 1双边沿
    UINT8 spi_line_mode;//0:1wire 1:2wire 2:4wire
    //UINT8 mtk_crc_en; //must 0
    UINT8 mtk_mode;
    UINT8 gc_mode;
    UINT8 sst_mode;
    UINT8 sync_code_led_byte;
    UINT8 sync_code_lst_byte;
    UINT8 sync_code_fed_byte;
    UINT8 sync_code_fst_byte;
    UINT32 sync_code_data_byte;
}Camif_Spi_Conf_S;

typedef struct
{
    Camif_Pixart_Type_E pixart_type;//pixart只需要size，驱动底层写死PAG7920LT流程。
}Camif_Pixart_Conf_S;

typedef struct
{
    FH_UINT32 phyAddr;//指向整个buffer
    UINT32 buf_len;
    UINT32 frame_id;
}Camif_Ae_info_S;

typedef enum{
    CAMIF_CMD_SET_AE_STAT_CONF,
    CAMIF_CMD_SET_AE_STAT_BUF_RECV_CALLBACK,
    CAMIF_CMD_SET_AWB_STAT_CONF,
    CAMIF_CMD_SET_AWB_STAT_RECV_CALLBACK,
    CAMIF_CMD_SET_BLC_CONF,
    CAMIF_CMD_SET_CROP_CONF,
    CAMIF_CMD_SET_PORT_CONF,
    CAMIF_CMD_SET_SIZE_CONF,
    CAMIF_CMD_SET_FORMAT_CONF,
    CAMIF_CMD_SET_AXI_PULSE_CONF,
    CAMIF_CMD_SET_SHMM_CONF,
    CAMIF_CMD_SET_SWITCH_CONF,
    CAMIF_CMD_SET_BUF,
    CAMIF_CMD_SET_BUF_RECV_CALLBACK,
    CAMIF_CMD_SET_SKIP_FRAME_CNT,
    CAMIF_CMD_SET_CROP_ENABLE,
    CAMIF_CMD_SET_AE_STAT_ENABLE,
    CAMIF_CMD_SET_AWB_STAT_ENABLE,
    CAMIF_CMD_SET_BLC_ENABLE,
    CAMIF_CMD_SET_RAWEC_ENABLE,
    CAMIF_CMD_SET_AXI_PULSE_ENABLE,
    CAMIF_CMD_SET_SHMM_ENABLE,
    CAMIF_CMD_SET_SWITCH_ENABLE,
    CAMIF_CMD_GET_SWITCH_FLAG,
    CAMIF_CMD_SET_PIXART_IRQ_CALLBACK,
    CAMIF_CMD_GET_STREAM,
    CAMIF_CMD_GET_DUMP,
    CAMIF_CMD_GET_AWB_STAT_CONF,
    CAMIF_CMD_GET_BLC_CONF,
    CAMIF_CMD_NUM
}Camif_Cmd_E;

typedef struct
{
    int ret;
}Camif_Cmd_Common_Output_Param_S;

typedef struct
{
    BOOL enable;//0:disable 1:enable
    UINT32 statwinhc;       //统计窗口水平方向个数减1
    UINT32 statwinvc;       //统计窗口垂直方向个数减1
    UINT32 statwinw;        //统计窗口宽度/2-1
    UINT32 statwinh;        //统计窗口高度/2-1
    UINT32 ae_mode;         //ae模式
    UINT32 statwinly;       //统计窗口起始纵坐标
    UINT32 statwinlx;       //统计窗口起始横坐标
    UINT32 ae_addr;         //ae输出的base地址,并非ae的输出地址
    UINT32 statsamph;       //垂直采样间隔减1
    UINT32 statsampv;       //水平采样间隔减1
    UINT32 statthh;         //饱和像素阈值
    UINT32 statthl;         //饱和像素阈值
    UINT32 stat_picheight;  //STAT的输入图像高
    UINT32 stat_picwidth;   //STAT的输入图像宽
}Camif_Ae_Conf_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Ae_Stat_Conf_Output_Param_S;

typedef struct
{
    int (*buf_recv_callback)(Camif_Ae_info_S *buffer_ptr);
}Camif_Cmd_Set_Ae_Stat_Buf_Recv_Callback_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Ae_Stat_Buf_Recv_Callback_Output_Param_S;

typedef struct
{
    BOOL enable;//0:disable 1:enable
    UINT32 awb_cntshift;     //cnt偏移，cnt >> cntshift
    UINT32 awb_bitmove;      //bit偏移，目标是12bit
    UINT32 awb_flagmode;     //全局统计模式：0考虑白区 1不考虑白区
    UINT32 awb_calgmodex;    //白区计算：0用Y 1用G
    UINT32 awb_calgmodeth;   //白区计算模式：0兼容模式 1新模式
    UINT32 leftsft;          //数据左移信号
    UINT32 shiftall;         //数据移位
    UINT32 high_offset_en;   //ae/awb输入是高位还是低位补0;0:低位补0;1:高位补0
    UINT32 awb_statwinstartx;//水平方向起点/2
    UINT32 awb_statwinstarty;//垂直方向起点/2
    UINT32 awb_v_num;//统计窗垂直个数-1
    UINT32 awb_h_num;//统计窗水平个数-1
    UINT32 awb_z_height;//统计窗口高度/2-1
    UINT32 awb_h_width;//统计窗口宽度/2-1
    UINT32 awb_bx;//B点横坐标
    UINT32 awb_ay;//A点纵坐标
    UINT32 awb_cx;//C点横坐标
    UINT32 awb_by;//B点纵坐标
    UINT32 awb_ex;//e点横坐标
    UINT32 awb_dx;//d点横坐标
    UINT32 awb_fy;//f点纵坐标
    UINT32 awb_ey;//e点纵坐标
    UINT32 awb_gy;//g点纵坐标
    UINT32 awb_gx;//g点横坐标
    UINT32 Awb_Calmode;//白区计算模式，0:兼容模式 1:新模式
    UINT32 awb_th_l;//统计阈值下限
    UINT32 awb_th_h;//统计阈值上限
    UINT32 awb_picheight;//awb的输入图像高
    UINT32 awb_picwidth;//awb的输入图像宽
    UINT32 awb_bayerfmt;//awb的输入raw pattern,0:rggb,1:grbg,2:bggr,3:gbrg
}Camif_Awb_Conf_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Awb_Stat_Conf_Output_Param_S;

typedef struct
{
    UINT32 buf_len;
    FH_UINT32 phyAddr;//指向整个buffer, ddr有效
    UINT32 frame_id;
    UINT32 global_r;
    UINT32 global_g;
    UINT32 global_b;
    UINT32 global_cnt;
}Camif_Awb_info_S;

typedef struct
{
    int (*buf_recv_callback)(Camif_Awb_info_S *awb_callback_info_ptr);
}Camif_Cmd_Set_Awb_Stat_Recv_Callback_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Awb_Stat_Recv_Callback_Ouput_Param_S;

typedef struct
{
    BOOL enable;//0:disable 1:enable
    UINT32 Gain_PicInHeight;
    UINT32 Gain_PicInWidth;
    UINT32 Gaincolor1;
    UINT32 Gaincolor0;
    UINT32 Gaincolor3;
    UINT32 Gaincolor2;
    UINT32 Offsetcolor1;
    UINT32 Offsetcolor0;
    UINT32 Offsetcolor3;
    UINT32 Offsetcolor2;
    UINT32 raw_10bit_en;//blc模块是运行在10bit模式还是12bit模式,1:10bit 0:12bit
    UINT32 gain_low_bit_en;//blc ae awb模块 10bit转12bit是低位补0还是高位补0;1:低位补0，0:高位补0
}Camif_Blc_Conf_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Blc_Conf_Output_Param_S;

typedef struct
{
    BOOL enable;//0:disable 1:enable
    UINT32 crop_x;
    UINT32 crop_y;
    UINT32 crop_width;
    UINT32 crop_height;
}Camif_Crop_Conf_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Crop_Conf_Output_Param_S;

typedef struct
{
    Camif_Port_Type_E port_type;
    UINT32 low_pos_8bit_en;//8bit转10bit补0方法，0:高位补0，1:低位补0
    UINT32 Camif_Data_En;//数据进入rawec时10bit转12方法，0:低位补0 1:高位补0
    union Camif_Port_Config
    {
        Camif_Dvp_Conf_S dvp_conf;
        Camif_P2s_Conf_S p2s_conf;
        Camif_Scb_Conf_S scb_conf;
        Camif_Spi_Conf_S spi_conf;
        Camif_Pixart_Conf_S pixart_conf;
    }conf;
}Camif_Port_Conf_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Port_Conf_Output_Param_S;

typedef struct
{
    UINT16  width; //输入图片的宽度
    UINT16  height; //输入图片的高度
}Camif_Size_Conf_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Size_Conf_Output_Param_S;

typedef struct
{
    //Cam_Pixel_Fmt_E pixel_fmt;//输入pixel的数据格式
    Cam_Bayer_Pattern_E bayer_pattern;
}Camif_Format_Conf_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Format_Conf_Output_Param_S;

typedef struct
{
    UINT32 rawec_base_addr;
    UINT32 rawec_buf_max_addr;
    UINT32 axi_pulse_num;//axi pulse 缓冲buffer大小，单位是64 bytes
}Camif_Cmd_Set_Axi_Pulse_Conf_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Axi_Pulse_Conf_Output_Param_S;

typedef struct
{
    UINT32 sys_addr_level0;
    UINT32 sys_addr_level1;
    UINT32 sys_addr_level2;
    UINT32 sys_addr_level3;
    UINT32 sys_addr_level4;
    UINT32 sys_addr_level5;
}Camif_Cmd_Set_Shmm_Conf_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Shmm_Conf_Output_Param_S;

typedef struct
{
    Camif_Buf_Type_E bufType;
    FH_UINT32 phyAddr;
    UINT32 buf_len;
}Camif_Buf_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Buf_Output_Param_S;

typedef struct
{
    UINT32 is_pulse;//0:表示不是pulse 1：表示pulse
    UINT32 frame_addr;//指向每一个帧实际数据的地址物理地址
    UINT32 bufLen;
    UINT32 frame_id;
    UINT32 register_PPS_0;
    UINT32 register_PPS_1;
    UINT32 register_PPS_2;
    UINT32 register_PPS_3;
    UINT32 register_PPS_4;
    UINT32 register_PPS_5;
    UINT32 register_PPS_6;
    UINT32 register_PPS_7;
    UINT32 register_PPS_8;
    UINT32 register_PPS_9;
    UINT32 register_PPS_10;
    UINT32 register_PPS_11;
    UINT32 register_PPS_12;
    UINT32 register_PPS_13;
    UINT32 register_PPS_14;
    UINT32 register_PPS_15;
    UINT32 register_STAT_0;
    UINT32 register_STAT_1;
    UINT32 register_STAT_2;
    UINT32 register_STAT_3;
}Camif_Frame_info_S;

typedef struct
{
    Camif_Frame_info_S frame_info;
}Camif_Buf_info_S;

typedef struct
{
    int (*buf_recv_callback)(Camif_Buf_info_S *buffer_ptr);
}Camif_Cmd_Set_Buf_Recv_Callback_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Buf_Recv_Callback_Output_Param_S;

typedef struct
{
    UINT16 skip_frame_cnt;
}Camif_Skip_Frame_Cnt_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Skip_Frame_Cnt_Output_Param_S;

typedef struct
{
    UINT16 skip_frame_cnt;
    UINT16 switch_step_flag;//通路切换
}Camif_Cmd_Set_Switch_Conf_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Switch_Conf_Output_Param_S;

typedef struct
{
    BOOL enable;//0:disable 1:enable
}Camif_Cmd_Set_Crop_Enable_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Crop_Enable_Output_Param_S;

typedef struct
{
    BOOL enable;//0:disable 1:enable
}Camif_Cmd_Set_Ae_Stat_Enable_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Ae_Stat_Enable_Output_Param_S;

typedef struct
{
    BOOL enable;//0:disable 1:enable
}Camif_Cmd_Set_Awb_Stat_Enable_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Awb_Stat_Enable_Output_Param_S;

typedef struct
{
    BOOL enable;//0:disable 1:enable
}Camif_Cmd_Set_Blc_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Blc_Output_Param_S;

typedef struct
{
    BOOL rawecOutputBypass;//rawec通道是否打开 0:disable 1:enable
    BOOL rawecCompressBypass;//rawec压缩是否打开
}Camif_Cmd_Set_Rawec_Enable_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Rawec_Enable_Output_Param_S;

typedef struct
{
    BOOL enable;//0:disable 1:enable
}Camif_Cmd_Set_Axi_Pulse_Enable_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Axi_Pulse_Enable_Output_Param_S;

typedef struct
{
    BOOL enable;//0:disable 1:enable
}Camif_Cmd_Set_Shmm_Enable_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Shmm_Enable_Output_Param_S;

typedef struct
{
    BOOL enable;//0:disable 1:enable
}Camif_Cmd_Set_Switch_Enable_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Switch_Enable_Output_Param_S;

typedef struct
{
    int (*pixart_irq_callback)(void);
}Camif_Cmd_Set_Pixart_Irq_Callback_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Set_Pixart_Irq_Callback_Output_Param_S;

typedef struct
{
    fhosal_proc_entry_t *sqf;
}Camif_Cmd_Get_Dump_Input_Param_S;

typedef struct
{
    int ret;
}Camif_Cmd_Get_Dump_Output_Param_S;

typedef struct
{
	Camif_Crop_Conf_S cropConf;
	Camif_Size_Conf_S sizeConf;
	Camif_Format_Conf_S formatConf;
}Camif_Viattr;

typedef struct
{
	Camif_Ae_Conf_S aeStat;
	Camif_Awb_Conf_S awbStat;
	Camif_Blc_Conf_S blcConf;
}Camif_2aconfig;

typedef struct
{
    Camif_Buf_Type_E type;
    FH_UINT32  u32TimeOut;//ms
    union{
        Camif_Ae_info_S aeInfo;
        Camif_Buf_info_S bufInfo;
        Camif_Awb_info_S awbInfo;
    }info;
}Camif_GetStream_S;

typedef struct
{
    int ret;
}Camif_Cmd_GetStream_Output_Param_S;

#endif
