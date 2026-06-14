#ifndef _VIO_APB_V01_REG_DEF_H_
#define _VIO_APB_V01_REG_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
#if 0
#define CPU_SYS_CLK_BASE_ADDR      (0x0c000100)
#define VOU_RST         (0x00b4)
#define VOU_EN          (0x00c0)


#define CPU_SYS_MIF_BASE_ADDR      (0x09200200)
#define VOU_MIF_SEL         (0x0088)

#define VOU_BASE_ADDR      (0X0C800000)
#define VOU_DHD_CLK         (0x0004)
#endif

/*******************************************/
#define DMC_SYS_CLK_BASE_ADDR      (0x24000300)
#define VOU_CLK_EN                 (0x0008)

#define DMC_SYS_RST_BASE_ADDR      (0x24000400)
#define VOU_RST                    (0x0004)

#define DMC_VOU_SEL_BASE_ADDR      (0x24000e00)
#define VOU_CTRL_0                 (0x000c)

#define VOU_BASE_ADDR              (0X26000000)
#define VOU_DHD_CLK                (0x0004)
#define VOU_DISPLAY_FORMAT         (0x0040)

#define VOU_DHD_OFFSET             (0x1000)
#define VOU_DHD_CHN_WORK_CTRL      (0x000C)

union dmc_sys_vou_clk_tag {
    struct dmc_sys_vou_clk_map {
        volatile unsigned int reserved0                 :11;
        volatile unsigned int ckg_vou_bus_force_en      :1;
        volatile unsigned int ckg_vou_bus_auto_gate_sel :1;
        volatile unsigned int reserved1                 :2;
        volatile unsigned int ckg_vou_force_en          :1;
        volatile unsigned int ckg_vou_auto_gate_sel     :1;
        volatile unsigned int reserved2                 :1;
        volatile unsigned int ckg_apb_dsi_cfg_en        :1;
        volatile unsigned int ckg_apb_dphy_cfg_en       :1;
        volatile unsigned int reserved3                 :4;
        volatile unsigned int ckg_vou_en                :1;
        volatile unsigned int reserved4                 :2;
        volatile unsigned int ckg_vou_bus_div           :3; //N+1
        volatile unsigned int ckg_vou_bus_sel           :2; //0:24M  1:120M 2:150M 3:200M
    }mBits;
    volatile unsigned int dwValue;
};

union dmc_sys_vou_rst_tag {
    struct dmc_sys_vou_rst_map {
        volatile unsigned int reserved0             :22;
        volatile unsigned int vou_dhd1_soft_rst     :1;
        volatile unsigned int vou_dhd0_soft_rst     :1;
        volatile unsigned int dsi_phy_soft_rst      :1;
        volatile unsigned int dsi_soft_rst          :1;
        volatile unsigned int vou_mif1_soft_rst     :1;
        volatile unsigned int vou_mif0_soft_rst     :1;
        volatile unsigned int vou_pub_soft_rst      :1;
        volatile unsigned int reserved1             :3;
    }mBits;
    volatile unsigned int dwValue;
};

union vou_display_format_tag {
    struct vou_display_format_map {
        volatile unsigned int dhd0_diaplay_format   :3;
        volatile unsigned int reserved0             :1;
        volatile unsigned int dhd1_diaplay_format   :3;
        volatile unsigned int reserved1             :25;
    }mBits;
    volatile unsigned int dwValue;
};

union vou_chn_work_ctrl_tag {
    struct vou_chn_work_ctrl_map {
        volatile unsigned int chn_open           :1;
        volatile unsigned int chn_shutdown       :1;
        volatile unsigned int reserved0          :6;
        volatile unsigned int update_mode        :1;
        volatile unsigned int force_update       :1;
        volatile unsigned int all_update_mode    :1;
        volatile unsigned int reserved1          :21;
    }mBits;
    volatile unsigned int dwValue;
};

union vou_dhd_chn_clk_tag {
    struct vou_dhd_chn_clk_map {
        volatile unsigned int dhd0_en          :1;
        volatile unsigned int dhd1_en          :1;
        volatile unsigned int reserved0        :10;
        volatile unsigned int dhd0_div         :4;
        volatile unsigned int dhd1_div         :4;
        volatile unsigned int reserved1        :11;
        volatile unsigned int eco_en           :1;
    }mBits;
    volatile unsigned int dwValue;
};

union vou_ctrl0_tag {
    struct vou_ctrl0_map {
        volatile unsigned int vou_dpi0_shutdn_in        :1;
        volatile unsigned int vou_dpi0_colorm_in        :1;
        volatile unsigned int vou_dpi1_shutdn_in        :1;
        volatile unsigned int vou_dpi1_colorm_in        :1;
        volatile unsigned int dhd0_src_sel              :1;  // dhd0 sel 1:sel lcm0  0:sel bt
        volatile unsigned int dhd1_src_sel              :1;  // dhd1 sel 1:sel lcm1  0:sel bt
        volatile unsigned int lcd_src_sel               :1;  // rgb/mcu src sel 0:from dhd0 1:from dhd1
        volatile unsigned int reserved                  :25;
    }mBits;
    volatile unsigned int dwValue;
};


#if 0
union cpu_sys_vou_clk_rst_tag {
    struct cpu_sys_vou_clk_rst_map {
        volatile unsigned int rst               :1;
        volatile unsigned int reserved          :31;
    }mBits;
    volatile unsigned int dwValue;
};

union cpu_sys_vou_clk_en_tag {
    struct cpu_sys_vou_clk_en_map {
        volatile unsigned int vo_en             :1;
        volatile unsigned int mif_en            :1;
        volatile unsigned int reserved          :30;
    }mBits;
    volatile unsigned int dwValue;
};

union cpu_sys_clk_vou_mif_sel_tag {
    struct cpu_sys_clk_vou_mif_sel_map {
        volatile unsigned int sel              :2;
        volatile unsigned int reserved0        :6;
        volatile unsigned int div              :8;
        volatile unsigned int reserved1        :16;
    }mBits;
    volatile unsigned int dwValue;
};

union vou_dhd_chn_clk_tag {
    struct vou_dhd_chn_clk_map {
        volatile unsigned int en               :1;
        volatile unsigned int reserved0        :15;
        volatile unsigned int div              :4;
        volatile unsigned int reserved1        :11;
        volatile unsigned int eco_en           :1;
    }mBits;
    volatile unsigned int dwValue;
};
#endif
/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
