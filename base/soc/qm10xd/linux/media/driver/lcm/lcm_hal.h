#ifndef _LCM_HAL_H_
#define _LCM_HAL_H_

typedef struct {
    int width;
    int height;
    int frame_rate;
} lcm_output_info_t;

typedef enum {
    LCM_IDLE,
    LCM_CMD,
    LCM_WR_DATA,
    LCM_RD_DATA,
}lcm_stage_type_t;

typedef enum {
    LCM_MAIN = 0,
    LCM_SUB,
    LCM_ID_IDX_MAX,
}lcm_id_e;

typedef enum {
    _LCM_TYPE_MCU = 0,
    _LCM_TYPE_RGB,
    _LCM_TYPE_MIPI,
    _LCM_TYPE_SPI,
    _LCM_TYPE_MAX
}_lcm_type_e;

typedef enum {
    LCM_CMD_SUSPEND = 0,
    LCM_CMD_RESUME,
    LCM_CMD_MAX
}lcm_cmd_e;

typedef struct {
    char     *Name;
    int Hactvie;
    int Vactive;
    int FrameMode;
    int FrameRate;
    int PixelRate;
    int Htotal;
    int Vtotal;
    int Hblank;
    int Vblank;
    int Hfront;
    int Hsync;
    int Hback;
    int Hpol;
    int Vfront;
    int Vsync;
    int Vback;
    int Vpol;
    int Alen;
    int Aline;
    int Hlen;
    int StartBlen;
    int EndBlen;
    int TVLine;
    int TBLineOfs;
    int TEndBLine;
    int BVLine;
    int BBLineOfs;
    int BEndBLine;
    int HSyncLength;
    int VSyncLength0;
} vou_sync_info_t;

typedef struct {
    int hor_sync_active;
    int hor_back_porch;
    int hor_active_pixel;
    int hor_front_porch;

    int ver_sync_active;
    int ver_back_porch;
    int ver_active_line;
    int ver_front_porch;
} dp_sync_info_t;

typedef struct {
    int h_sync_pol;
    int v_sync_pol;
    int de_pol;
}rgb_display_cfg_t;

typedef struct {
    int trans_mode; // 8/9bit lsb/hsb first
    int cs_type;
    int te_eb;
    int te_mode;
    int te_edge;
}mcu_display_cfg_t;

typedef struct {
    int bit_depth;
    int if_type;
    int rgb_order;
    int dithering_en;
    int bitswap_en;
    union {
        rgb_display_cfg_t rgb;
        mcu_display_cfg_t mcu;
    }info;
}lcm_display_cfg_t;

typedef struct {
    lcm_id_e lcm_id;
    _lcm_type_e type;
    lcm_output_info_t output;
    dp_sync_info_t dp_sync;
    lcm_display_cfg_t lcm_config;
} lcm_probe_info_t;

typedef struct {
    int reserved;
}lcm_probe_in_t;

typedef struct {
    lcm_probe_info_t *prob_info[LCM_ID_IDX_MAX];
}lcm_probe_out_t;

typedef struct {
    lcm_id_e lcm_id;
}lcm_init_in_t;

typedef struct {
    int reserved;
}lcm_init_out_t;

typedef struct {
    lcm_id_e lcm_id;
}lcm_deinit_in_t;

typedef struct {
    int reserved;
}lcm_deinit_out_t;

typedef struct {
    lcm_id_e lcm_id;
}lcm_suspend_in_t;

typedef struct {
    int reserved;
}lcm_suspend_out_t;

typedef struct {
    lcm_id_e lcm_id;
}lcm_resume_in_t;

typedef struct {
    int reserved;
}lcm_resume_out_t;

typedef struct {
    lcm_id_e lcm_id;
}lcm_close_in_t;

typedef struct {
    int reserved;
}lcm_close_out_t;

typedef struct {
    lcm_id_e lcm_id;
}lcm_reopen_in_t;

typedef struct {
    int reserved;
}lcm_reopen_out_t;

typedef struct {
    lcm_id_e lcm_id;
    lcm_cmd_e cmd;
    void *param;
}lcm_ioctrl_in_t;

typedef struct {
    void *param;
}lcm_ioctrl_out_t;

typedef struct {
	int (*probe)(lcm_probe_in_t * inParam, lcm_probe_out_t *outParam);
	int (*init)(lcm_init_in_t *inParam, lcm_init_out_t *outParam);
	int (*deinit)(lcm_deinit_in_t *inParam, lcm_deinit_out_t *outParam);
	int (*suspend)(lcm_suspend_in_t *inParam, lcm_suspend_out_t *outParam);
	int (*resume)(lcm_resume_in_t *inParam, lcm_resume_out_t *outParam);
	int (*lcm_ioctrl)(lcm_ioctrl_in_t *inParam, lcm_ioctrl_out_t *outParam);
} lcm_api_t;

int lcm_func_probe(lcm_probe_in_t * inParam, lcm_probe_out_t *outParam);
int lcm_func_init(lcm_init_in_t *inParam, lcm_init_out_t *outParam);
int lcm_func_deinit(lcm_deinit_in_t *inParam, lcm_deinit_out_t *outParam);
int lcm_func_suspend(lcm_suspend_in_t *inParam, lcm_suspend_out_t *outParam);
int lcm_func_resume(lcm_resume_in_t *inParam, lcm_resume_out_t *outParam);
int lcm_ioctrl(lcm_ioctrl_in_t *inParam, lcm_ioctrl_out_t *outParam);

#endif
