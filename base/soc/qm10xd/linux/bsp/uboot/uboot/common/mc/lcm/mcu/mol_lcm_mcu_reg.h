#ifndef _LCM_MCU_REG_H_
#define _LCM_MCU_REG_H_

#ifdef __cplusplus
extern "c"
{
#endif
/**---------------------------------------------------------------------------*
**                               micro define                                **
**----------------------------------------------------------------------------*/

#define DHD_DISPLAY_FROAMT 0x0040
#define DHD_INTERRUPT_CLR  0x1018
#define DHD_INTERRUPT_RAW  0x101C
#define DHD_DISPLAY_CFG    0x10B4 //??
#define DHD_DISPLAY_DELAY  0x10B8
#define LCM_CFG            0x10BC
#define LCM_BITMAP0        0x10C0
#define LCM_BITMAP1        0x10C4
#define LCM_BITMAP2        0x10C8
#define LCM_BITMAP3        0x10CC
#define LCM_BITMAP4        0x10D0
#define LCM_BITMAP5        0x10D4
#define LCM_STEPCYCLE      0x10D8 //??
#define LCM_STAGE          0x10E0
#define LCM_CMD_RUN        0x10E4
#define LCM_CMD_WDATA      0x10E8
#define LCM_CMD_RDATA      0x10EC
#define LCM_CMD_ADDR       0x10F0

#define DHD1_INTERRUPT_CLR  0x2018
#define DHD1_INTERRUPT_RAW  0x201C
#define DHD1_DISPLAY_CFG    0x20B4 //??
#define DHD1_DISPLAY_DELAY  0x20B8
#define LCM1_CFG            0x20BC
#define LCM1_BITMAP0        0x20C0
#define LCM1_BITMAP1        0x20C4
#define LCM1_BITMAP2        0x20C8
#define LCM1_BITMAP3        0x20CC
#define LCM1_BITMAP4        0x20D0
#define LCM1_BITMAP5        0x20D4
#define LCM1_STEPCYCLE      0x20D8 //??
#define LCM1_STAGE          0x20E0
#define LCM1_CMD_RUN        0x20E4
#define LCM1_CMD_WDATA      0x20E8
#define LCM1_CMD_RDATA      0x20EC
#define LCM1_CMD_ADDR       0x20F0
/**---------------------------------------------------------------------------*
**                               data prototype                              **
**----------------------------------------------------------------------------*/

union _lcm_mcu_int_tag { /* 0x10B4 */
    struct _lcm_mcu_int_map {
        volatile unsigned int Reserved1                 :26;  // default:h0
        volatile unsigned int LCM_CMD_DONE              :1;
        volatile unsigned int LCM_STAGE_SWITCH_DONE     :1;
        volatile unsigned int LCM_TE                    :1;
        volatile unsigned int Reserved0                 :3;
    }mBits;
    volatile unsigned int dwValue;
};

union _lcm_display_cfg_tag { /* 0x10B4 */
    struct _lcm_display_cfg_map {
        volatile unsigned int DISPLAY_BIT_DEPTH             :2;  // default:h0    For DSI/LCM 00: 24bits 888 01: 18bits 666 10: 16bits 565
        volatile unsigned int DISPLAY_UV_ORDER              :1;  // default:b0    for AVE/BT656/1120 0: {VU} 1: {UV}
        volatile unsigned int DISPLAY_RGB_ORDER             :3;  // default:h0    for DSI/LCM 0: {BGR} 1: {GBR} 2: {RBG} 3: {BRG} 4: {GRB} 5: {RGB} Others: reserved
        volatile unsigned int DSI_CMD_mode                  :1;  // default:b0    0: VIDEO_mode 1: CMD mode
        volatile unsigned int LCM_MCU_mode                  :1;  // default:b0    0: RGB_mode 1: MCU mode
        volatile unsigned int BT_1120_mode                  :1;  // default:b0    0: BT656 1: BT1120
        volatile unsigned int DISPLAY_DATAEN_ACTIVE_LOW     :1;  // default:b0    For DSI/LCM, 数据使能极性 0：数据有效为高电平；1：数据有效为低电平
        volatile unsigned int DISPLAY_HSYNC_ACTIVE_LOW      :1;  // default:b0    For DSI/LCM,水平同步极性 0：消隐为高电平；1：消隐为低电平
        volatile unsigned int DISPLAY_VSYNC_ACTIVE_LOW      :1;  // default:b0    For DSI/LCM,垂直同步极性 0：消隐为高电平；1：消隐为低电平
        volatile unsigned int DPI_SHUTDN_ACTIVE_LOW         :1;  // default:b0    For DSI,关闭信号极性 0：高有效；1：低有效
        volatile unsigned int DPI_COLORM_ACTIVE_LOW         :1;  // default:b0    For DSI,COLORM极性: 0：高有效；1：低有效
        volatile unsigned int DISPLAY_TE_EN                 :1;  // default:b1    For DSI/LCM, Tearing Effect使能: 0：关闭；1：使能
        volatile unsigned int DPI_VIDEO_MODE_WAIT_HALT      :1;  // default:b1    For DSI, DPI Video模式下，每帧内数据发送是否要受HALT信号影响
        volatile unsigned int Dithering_en                  :1;  // default:b1    打开Dithering
        volatile unsigned int Bitswap_en                    :1;  // default:b0    RGB内部bit位置Swap
        volatile unsigned int USE_CLK_NEGEDGE               :1;  // default:b0    目前LCM Only 0:在时钟上升沿输出 1:在时钟下降沿输出
        volatile unsigned int USE_TE_NEGEDGE                :1;  // default:b0    目前LCM Only 0:在TE上升沿后开始输出下一帧 1:在TE下降沿后开始输出下一帧
        volatile unsigned int Reserved                      :4;  // default:h0    Reserved
        volatile unsigned int DISPLAY_RGB2YUV_matrix_sel    :2;  // default:h0    for AVE/BT656/1120，00:BT601 01:BT709  10:BT2020 11:reserved
        volatile unsigned int DISPLAY_RGB2YUV_clip_eb       :1;  // default:h0    for AVE/BT656/1120，0: output is full range 1：clip to Narrow range
        volatile unsigned int DISPLAY_RGB2YUV_clip_mode     :1;  // default:h0    for AVE/BT656/1120，0: 直接clip 1：进行拉伸操作
        volatile unsigned int DISPLAY_RGB_MAP               :4;  // default:h0    For DSI 16bits and 18bits 0000: 有效数据向低位集中 0001: 有效数据分别向8bit低位集中 0010: 有效数据分别向8bit高位集中 0011: 在0001基础上bit数较少分量空出低位以补足位宽到最大位宽
    }mBits;
    volatile unsigned int dwValue;
};

union _lcm_display_delay_tag { /* 0x10B8 */
    struct _lcm_display_delay_map {
        volatile unsigned int DISPLAY_DELAY_CYCLE          :32;  // default:h0 For AVE/DSI/LCM 在给定时刻后延迟Cycle数后输出有效数据
    }mBits;
    volatile unsigned int dwValue;
};

union _VOU_CHN_INT_STS_tag {
     struct _VOU_CHN_INT_STS_map {
        volatile unsigned int chn_sof                :1;
        volatile unsigned int chn_eof                :1;
        volatile unsigned int chn_underrun           :1;
        volatile unsigned int reg_update_done        :1;
        volatile unsigned int reserved0              :3;
        volatile unsigned int reg_rdy_update_done    :1;
        volatile unsigned int reserved1              :8;
        volatile unsigned int img_state_err          :1;
        volatile unsigned int reserved2              :9;
        volatile unsigned int lcm_cmd_done           :1;
        volatile unsigned int lcm_stage_switch_done  :1;
        volatile unsigned int lcm_te                 :1;
        volatile unsigned int internal_reset_done    :1;
        volatile unsigned int chn_open               :1;
        volatile unsigned int chn_shutdown           :1;
    }mBits ;
    volatile unsigned int dwValue;
};

union _lcm_cfg_tag { /* 0x10BC */
    struct _lcm_cfg_map {
        volatile unsigned int Reserved2                :4;  // default:h0
        volatile unsigned int LCM_SYS_IF_TYPE          :2;  // default:h0    //00:18bit interface  01:16bit interface 10: 9bit interface 11: 8bit interface
        volatile unsigned int LCM_RGB_IF_TYPE          :2;  // default:h0 //00:18bit RGB interface  01:16bit RGB interface     10: 6bit RGB interface     11:24bit RGB interface
        volatile unsigned int LCM_MCU_TIMING_TYPE      :1;  // default:b0 //0: Data Active at rising edge of WR/RD 1: Data Active at rising edge of CS
        volatile unsigned int LCM_SW_USE_TE            :1;  // default:b0 //0: HW use TE to control Frame refresh 1: SW use TE to control Frame refresh, and HW stop after each Frame to wait SW Launch refresh again
        volatile unsigned int LCM_MCU_HIGHBYTE_FIRST   :1;  // default:b0 //Only used when Transfer_Num=2   0: Low 8bits/9bits Transfer First  1: High 8bits/9bits Transfer First
        volatile unsigned int LCM_MCU_3_TRANS          :1;  // default:b0 //Transfer Number is 3, and R/G/B each time
        volatile unsigned int Reserved1                :2;  // default:h0
        volatile unsigned int LCM_WAIT_FIFO            :1;  // default:b0 //1: LCM数据传输需要检查FIFO Ready
        volatile unsigned int Reserved0                :16; // default:h0
    }mBits;
    volatile unsigned int dwValue;
};

union _lcm_bitmap_tag { /* 0x10C0 ~ 0x10D4 0~31 */
    struct _lcm_bitmap_map {
        volatile unsigned int LCM_BIT_SRC_SEL0       :5;  // default:h0  //LCM输出数据Bitmap
        volatile unsigned int LCM_BIT_SRC_SEL1       :5;  // default:h1  //LCM输出数据Bitmap
        volatile unsigned int LCM_BIT_SRC_SEL2       :5;  // default:h2  //LCM输出数据Bitmap
        volatile unsigned int LCM_BIT_SRC_SEL3       :5;  // default:h3  //LCM输出数据Bitmap
        volatile unsigned int LCM_BIT_SRC_SEL4       :5;  // default:h4  //LCM输出数据Bitmap
        volatile unsigned int LCM_BIT_SRC_SEL5       :5;  // default:h5  //LCM输出数据Bitmap
        volatile unsigned int Reserved               :2;  // default:h0
    }mBits;
    volatile unsigned int dwValue;
};

union _lcm_step_cycle_tag { /* 0x10D8 */
    struct _lcm_step_cycle_map {
        volatile unsigned int LCM_WSTEP0_CYCLE     :5;  // default:h1
        volatile unsigned int LCM_WSTEP1_CYCLE     :5;  // default:h1
        volatile unsigned int LCM_WSTEP2_CYCLE     :5;  // default:h1
        volatile unsigned int LCM_RSTEP0_CYCLE     :5;  // default:h1
        volatile unsigned int LCM_RSTEP1_CYCLE     :5;  // default:h1
        volatile unsigned int LCM_RSTEP2_CYCLE     :5;  // default:h1
        volatile unsigned int Reserved             :2;  // default:h0
    }mBits;
    volatile unsigned int dwValue;
};

union _lcm_stage_tag { /* 0x10E0 */
    struct _lcm_stage_map {
        volatile unsigned int LCM_STAGE_SWITCH    :1;  // default:h0   //执行LCM STAGE切换，自清单周期脉冲
        volatile unsigned int Reserved2           :7;  // default:h0
        volatile unsigned int LCM_STAGE_SET       :2;  // default:h0   // 0:IDLE状态 1:设置LCM进入发CMD阶段 2:设置LCM进入发WR DATA阶段 3:设置LCM进入发RD DATA阶段
        volatile unsigned int Reserved1           :2;  // default:h0
        volatile unsigned int LCM_STAGE_STATUS    :2;  // default:h0   // 确认切换成功 1:LCM进入发CMD阶段 2:LCM进入发WR DATA阶段 3:LCM进入发RD DATA阶段  note:软件需（polling或中断）确认切换成功后才可发送命令。
        volatile unsigned int Reserved0           :20; // default:h0
    }mBits;
    volatile unsigned int dwValue;
};

union _lcm_cmd_run_tag { /* 0x10E4 */
    struct _lcm_bitmap0_map {
        volatile unsigned int LCM_CMD_START          :1;  // default:h0   // 使能VOU执行命令，自清单周期脉冲
        volatile unsigned int LCM_CMD_FIFO_CLR       :1;  // default:h0   // 清除内部CMD FIFO状态，自清单周期脉冲
        volatile unsigned int Reserved4              :2;  // default:h0
        volatile unsigned int LCM_CMD_RW             :1;  // default:h0   // 0:Read 1:Write
        volatile unsigned int Reserved3              :1;  // default:h0
        volatile unsigned int LCM_CMD_DUMMY_NUM      :2;  // default:h0   // 读操作时Dummy Read Num, 0 到3
        volatile unsigned int LCM_CMD_DATA_NUM       :5;  // default:h0   // CMD_DATA_NUM，0到16个
        volatile unsigned int Reserved2              :2;  // default:h0
        volatile unsigned int LCM_CMD_ADDR_NUM       :1;  // default:h0   // CMD_ADDR_NUM，0或1
        volatile unsigned int LCM_CMD_WFIFO_DEPTH    :5;  // default:h0   // CMD WFIFO DEPTH,最大16
        volatile unsigned int Reserved1              :2;  // default:h0
        volatile unsigned int LCM_CMD_WFIFO_FULL     :1;  // default:h0
        volatile unsigned int LCM_CMD_RFIFO_DEPTH    :5;  // default:h0   // CMD RFIFO DEPTH,最大16
        volatile unsigned int Reserved0              :2;  // default:h0
        volatile unsigned int LCM_CMD_RFIFO_EMPTY    :1;  // default:h0
    }mBits;
    volatile unsigned int dwValue;
};

union _lcm_data_tag { /* 0x10E8/0x10EC/0x10F0 */
    struct _lcm_data_map {
        volatile unsigned int data             :32;  // default:h0
    }mBits;
    volatile unsigned int dwValue;
};

/**----------------------------------------------------------------------------*
**                         compiler flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// end