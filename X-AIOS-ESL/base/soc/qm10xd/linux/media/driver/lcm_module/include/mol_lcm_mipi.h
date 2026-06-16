#ifndef _MOL_LCM_MIPI_H_
#define _MOL_LCM_MIPI_H_

#include "dsi_com_param.h"

typedef struct {
    dsi_mode_e work_mode; /*command_mode, video_mode*/
    dsi_lane_num_e lane_num;
    uint32_t packet_size; // 0x34
    dsi_format_e format;
    dsi_timing_t *timing;
}lcm_mipi_info;

typedef struct {
    int32_t (*set_cmd_mode)(void *param);
    int32_t (*set_video_mode)(void *param);
    int32_t (*lp_mode)(void *param);
    int32_t (*hs_mode)(void *param);
    int32_t (*generic_write)(uint8_t *param, uint16_t param_length);
    int32_t (*generic_read)(uint8_t *param, uint16_t param_length, uint8_t bytes_to_read, uint8_t *read_buffer);
    int32_t (*dcs_write)(uint32_t hostId, uint8_t *param, uint16_t param_length);
    int32_t (*dcs_read)(uint32_t hostId, uint8_t data_type, uint8_t command, uint8_t bytes_to_read, uint8_t *read_buffer);
    int32_t (*push_tab)(uint32_t hostId, void *param, uint32_t count);
    int32_t (*bus_init)(void *param, void *lcm_cfg);
    int32_t (*bus_deinit)(void *param, void *lcm_cfg);
}lcm_mipi_dsi_fun;

#endif
