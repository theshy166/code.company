#ifndef __PRE_ROLLING_H__
#define __PRE_ROLLING_H__
#include "isp460/isp_sensor_if.h"

#define MAX_AE_INFO_NODE_NUM            (20)
#define MAX_AWB_INFO_NODE_NUM           (20)
#define MAX_CTRL_INFO_NODE_NUM          (20)
#define PPS_INFO_NUM                    (20)

typedef enum {
	SENSOR_POWER_ALWAYS_ON = 1,
	SENSOR_POWER_STANBY,
	SENSOR_POWER_OFF
} SENSOR_POWER_MODE_E;

typedef enum {
	PRG_NORMAL_MODE = 0,
	PRG_TUNNING_MODE,
	PRG_INVALID_WORK_MODE
} PRG_WORK_MODE;

/*
 * @pic_enc_mode, picture encode mode in pre_rolling system.
 *     =0, Raw, output RawData.
 *     =1, no encode, output YData directly.
 *     =2, JPEG, output jpeg picture.
 *     =3, H264, output h264 picture.
 * @frame_rate, frame rate to share memory in pre-rolling system.
 * @pic_continue_out, indicate whether continue output picture while
 *     pre_rolling exit.
 * @pic_abort, indicate whether PIR event abot pre_rolling process.
 * @main_sensor_preconfig, incidate whether to pre-config main sensor.
 * @sensor_power_mode.
 *     =0, always on.
 *     =1, standby.
 *     =2, power off.
 */
struct prg_system_info {
	unsigned int pic_enc_mode;
	unsigned int main_sensor_preconfig;
	unsigned int pic_continue_out;
	unsigned int pic_abort;
	unsigned int sensor_power_mode;
	unsigned int prg_power_mode;
	unsigned int stanby_time;
	//unsigned int frame_rate;
};

/*
 * @phy_base/buf_size, the DDR memory space to store picture data & control information.
 * @pre_data_offset/len, picture data during pre-rolling.
 * @pre_ctrl_offset/len, control information during pre-rolling.
 * @pre_ctrl_num, control information number during pre-rolling.
 * @extra_data_len, picture data during the whole system boot. base is pys_base.
 * @extra_ctrl_offset/len, control information during system boot.
 * @extra_ctrl_number, control information number during system boot.
 */
struct prg_picture_info {
	unsigned int phy_base;
	unsigned int buf_size;

	unsigned int pre_data_offset;
	unsigned int pre_data_len;
	unsigned int pre_ctrl_offset;
	unsigned int pre_ctrl_len;
	unsigned int pre_ctrl_num;
	unsigned int ae_ctrl_offset;
	unsigned int ae_ctrl_num;
	unsigned int awb_ctrl_offset;
	unsigned int awb_ctrl_num;

	unsigned int extra_data_offset;
	unsigned int extra_data_len;
	unsigned int extra_ctrl_offset;
	unsigned int extra_ctrl_len;
	unsigned int extra_ctrl_num;
	unsigned int extra_ae_ctrl_offset;
	unsigned int extra_ae_ctrl_num;
	unsigned int extra_awb_ctrl_offset;
	unsigned int extra_awb_ctrl_num;
};

/*
 * @frame_id, picture id
 * @frame_addr, picture phy addr
 * @frame_size, picture real size
 * @pps, rawec picture pps info, This will be left blank when h264 and jpeg mode
 */
typedef struct {
	unsigned int frame_id;
	unsigned int frame_addr;
	unsigned int frame_size;
	unsigned int PPS_0;
	unsigned int PPS_1;
	unsigned int PPS_2;
	unsigned int PPS_3;
	unsigned int PPS_4;
	unsigned int PPS_5;
	unsigned int PPS_6;
	unsigned int PPS_7;
	unsigned int PPS_8;
	unsigned int PPS_9;
	unsigned int PPS_10;
	unsigned int PPS_11;
	unsigned int PPS_12;
	unsigned int PPS_13;
	unsigned int PPS_14;
	unsigned int PPS_15;
	unsigned int STAT_0;
	unsigned int STAT_1;
	unsigned int STAT_2;
	unsigned int STAT_3;
}Prg_Pps_Info_Node_S;

/*
 * @info
 * @valid, picture valid flag.
 *     =1, picture is complete.
 *     =0, picture is not complete.
 */
struct prg_ctrl_info_node {
	Prg_Pps_Info_Node_S info;
	unsigned int valid;
};

typedef struct {
	unsigned int rGain;
	unsigned int gGain;
	unsigned int bGain;
	unsigned int CT;
	unsigned int framId;
}awb_info_t;

typedef struct {
	unsigned int exposureTime;
	unsigned int aGain;
	unsigned int dGain;
	unsigned int EV;
	unsigned int framId;
}ae_info_t;

struct prg_buffer {
	unsigned int base;
	unsigned int size;
};

struct prg_image_info {
	unsigned int phy_base;
	unsigned int size;
};

struct prg_sensor_info {
	Sensor_Param_S sensor;
	void *hex_virt;
	unsigned int hex_len;
};

int pre_rolling_init(int mode, struct prg_system_info *info);

int pre_rolling_deinit(int mode);

int pre_rolling_get_frame(struct prg_picture_info *image);

int pre_rolling_get_system_info(struct prg_system_info *info);

int pre_rolling_set_system_info(struct prg_system_info *info);

int pre_rolling_get_image_info(struct prg_image_info *image);

int pre_rolling_set_sensor_info(Sensor_Param_S *sensor, void *hex, unsigned int len);

int pre_rolling_get_prg_state(unsigned int *state);

int pre_rolling_set_ppbuf(void);

int pre_rolling_start(void);

#endif
