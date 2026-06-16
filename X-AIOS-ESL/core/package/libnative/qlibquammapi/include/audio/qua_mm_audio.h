/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_MM_AUDIO_MODULE_H__
#define __QUA_MM_AUDIO_MODULE_H__

#include "common/qua_mm_common.h"

#define QUA_MM_AUDIO_DEV "audio.io"
#define QUA_MM_AUDIO_DEC_DEV "audio.dec"
#define QUA_MM_AUDIO_ENC_DEV "audio.enc"

typedef QUA_S32 QUA_ADEC_CHN;
typedef QUA_S32 QUA_AENC_CHN;

typedef struct _qua_audio_cfg_arg {
    int enc_type;
    int io_type;
    int frame_size;
    int frame_cnt;
    int channels;
    int channel_bit;
    int rate;
} qua_audio_cfg_arg;

typedef struct qua_mm_audio_module_t {
    struct qua_mm_module_t parent;
} qua_mm_audio_module_t;

typedef enum {
    QUA_MIC_IN,
    QUA_LINE_IN,
    QUA_LINE_HPOUT,
    QUA_LINE_OUT
} qua_audio_iotype_t;

typedef enum {
    QUA_CHANNEL_MONO = 1,
    QUA_CHANNEL_STEREO = 2,
    QUA_CHANNEL_QUAD = 4,
} qua_audio_channel_t;

typedef enum {
    QUA_DEV_I2S0 = 0,
    QUA_DEV_I2S1,
    QUA_DEV_I2S2,
    QUA_DEV_I2S3,
    QUA_DEV_I2S4,
    QUA_DEV_I2S_MAX,
    QUA_DEV_ACW0,
    QUA_DEV_ACW1,
    QUA_DEV_ACW_STEREO,
    QUA_DEV_ACW_MAX,
    QUA_DEV_DMIC0,
    QUA_DEV_DMIC1,
    QUA_DEV_DMIC_MAX,
    QUA_DEV_CODEC_OTHERS,
    QUA_DEV_AUDIO_BUTT
} qua_audio_devid_t;

typedef enum {
    QUA_PT_LPCM = 24,
    QUA_PT_AAC = 37,
    QUA_PT_G711A = 19,
    QUA_PT_G711U = 20,
    QUA_PT_G726_16K = 21,
    QUA_PT_G726_32K = 22,
} qua_pt_type_t;

typedef enum {
    QUA_BW_8 = 8,
    QUA_BW_16 = 16,
    QUA_BW_32 = 32
} qua_bit_width_t;

typedef enum {
    QUA_SR_8K = 8000,
    QUA_SR_11K025 = 11025,
    QUA_SR_12K = 12000,
    QUA_SR_16K = 16000,
    QUA_SR_22K05 = 22050,
    QUA_SR_24K = 24000,
    QUA_SR_32K = 32000,
    QUA_SR_44K1 = 44100,
    QUA_SR_48K = 48000,
    QUA_SR_96K = 96000,
    QUA_SR_192K = 192000
} qua_sample_rate_t;

typedef enum qua_adec_mode {
    QUA_ADEC_MODE_PACK = 0,
    QUA_ADEC_MODE_STREAM,
    QUA_ADEC_MODE_BUTT
} qua_adec_mode_t;

typedef enum qua_volume_type_mode {
    QUA_VOLUME_TYPE_MIC,
    QUA_VOLUME_TYPE_ANA,
    QUA_VOLUME_TYPE_DIG,
    QUA_VOLUME_TYPE_VQE,
    QUA_VOLUME_TYPE_MAX
} qua_volume_type_t;

typedef enum qua_audio_algo_type {
    QUA_AUDIO_ALGO_SES_MIC1 = 0,
    QUA_AUDIO_ALGO_SES_MICN,
    QUA_AUDIO_ALGO_MPP_MIC1,
    QUA_AUDIO_ALGO_MOD_MAX,
} qua_audio_algo_type_t;

typedef struct qua_aenc_chn_attr {
    qua_pt_type_t enc_type;
    QUA_U32 frame_cnt;
    QUA_U32 frame_size;
    qua_sample_rate_t sample_rate;
    qua_bit_width_t bit_width;
    qua_audio_channel_t channel;
    QUA_VOID_PTR *value;
} qua_aenc_chn_attr_t;

typedef struct qua_adec_chn_attr {
    qua_pt_type_t enc_type;
    QUA_U32 frame_cnt;
    QUA_U32 frame_size;
    qua_sample_rate_t sample_rate;
    qua_bit_width_t bit_width;
    qua_adec_mode_t mode;
    QUA_VOID_PTR *value;
} qua_adec_chn_attr_t;

typedef struct qua_audio_stream {
    QUA_U8* stream;
    QUA_U32 phy_addr;
    QUA_U32 len;
    QUA_U64 pts;
    QUA_U32 seq;
    qua_sample_rate_t sample_rate;
    qua_bit_width_t bit_width;
    qua_audio_channel_t channel;
} qua_audio_stream_t;

typedef struct qua_audio_frame {
    QUA_VOID_PTR vir_addr;
    QUA_U32 phy_addr;
    QUA_U32 seq;
    QUA_U32 len;
    QUA_U32 timeout;
    QUA_U64 pts;
    qua_sample_rate_t sample_rate;
    qua_bit_width_t bit_width;
    qua_audio_channel_t channel;
    QUA_U32 exp_id;
} qua_audio_frame_t;

typedef struct qua_aec_param {
    QUA_U16 bng_flag;
    QUA_U16 aec_delay_ms;
    QUA_U16 aec_ecstart_thr;
    QUA_U16 aec_miu;
    QUA_U16 nlp_level;
    QUA_U16 nlp_gamma;
    QUA_U16 post_filter_switch;
    QUA_U16 echo_on_fr_switch;
    QUA_U16 erle_thr1;
    QUA_U16 erle_subthr1;
    QUA_U16 erle_thr2;
    QUA_U16 erle_subthr2;
    QUA_U16 pos_s;
    QUA_U16 pos_bit;
    QUA_U16 reverberation;
    QUA_U16 rev_smooth;
    QUA_U16 tx_rx_ratio[12];
    QUA_U16 dtd_switch;
    QUA_U16 dtd_thr;
    QUA_U16 dtd_gain;
    QUA_U16 non_dtd_gain;
} qua_aec_param_t;

typedef struct qua_anc_param {
    QUA_U16 anc_level;
    QUA_U16 bgns_switch;
    QUA_U16 bgns_gain_hf;
    QUA_U16 bgns_gain_lf;
} qua_anc_param_t;

typedef struct qua_agc_param {
    QUA_U16 fixed_gain;
    QUA_U16 tartget_dbfs;
    QUA_U16 gain_db;
    QUA_U16 agc_start_thr;
    QUA_U16 agc_supress;
    QUA_U16 agc_unlimited_flag;
} qua_agc_param_t;

typedef struct qua_doa_param {
    QUA_S16 doa_level;
    QUA_S16 doa_delay[3];
    QUA_S16 doa_mic_loc[12];
} qua_doa_param_t;

typedef struct qua_eqf_param {
    QUA_S16 eqf_bin_g[14];
} qua_eqf_param_t;

typedef struct qua_eqt_subparam {
    QUA_U16 eqt_filter_f0;
    QUA_U16 eqt_filter_q;
    QUA_S16 eqt_filter_gain_db;
} qua_eqt_subparam_t;

typedef struct qua_eqt_param {
    QUA_U16 eqt_filter_num;
    qua_eqt_subparam_t eqt_sub_param[8];
} qua_eqt_param_t;

typedef struct qua_hpf_param {
    QUA_S16 hpf_mode;
} qua_hpf_param_t;

typedef struct qua_howl_param {
    QUA_U16 howl_mc_thr1;
    QUA_U16 howl_mc_thr2;
    QUA_U16 howl_mc_thr3;
    QUA_U16 howl_thr;
} qua_howl_param_t;

typedef struct qua_ains_param {
    QUA_U16 sws_gain_smooth_alpha;
    QUA_U16 sws_gain_beta;
    QUA_U16 sws_gain_min;
} qua_ains_param_t;

typedef struct qua_wns_param {
    QUA_U16 wns_mc_num;
    QUA_U16 wns_mc_thr;
    QUA_U16 wns_bin1_thr;
    QUA_U16 wns_gain_limit;
    QUA_U16 wns_level;
} qua_wns_param_t;

typedef struct qua_alc_param {
    QUA_S16 alc_gain;
} qua_alc_param_t;

typedef struct qua_ac_ses_param {
    QUA_U16 aec_enable;
    QUA_U16 anc_enable;
    QUA_U16 agc_enable;
    QUA_U16 doa_enable;
    QUA_U16 eqf_enable;
    QUA_U16 eqt_enable;
    QUA_U16 hpf_enable;
    QUA_U16 howl_enable;
    QUA_U16 ains_enable;
    QUA_U16 wns_enable;
    QUA_U16 aivad_enable;
    QUA_U16 alc_enable;
    qua_aec_param_t aec_param;
    qua_anc_param_t anc_param;
    qua_agc_param_t agc_param;
    qua_doa_param_t doa_param;
    qua_eqf_param_t eqf_param;
    qua_eqt_param_t eqt_param;
    qua_hpf_param_t  hpf_param;
    qua_howl_param_t howl_param;
    qua_ains_param_t ains_param;
    qua_wns_param_t wns_param;
    qua_alc_param_t alc_param;
} qua_ac_ses_param_t;

typedef struct qua_ai_fhses_param {
    QUA_S16 index;
    QUA_U32 id;
    QUA_S32 ao_chn;
    qua_ac_ses_param_t vqe_attr;
}qua_ai_fhses_param_t;

typedef struct qua_ao_fhses_param {
    QUA_S16 index;
    qua_ac_ses_param_t vqe_attr;
} qua_ao_fhses_param_t;

typedef struct qua_audio_agc_cfg {
    QUA_BOOL usr_mode;
    QUA_S16 output_mode;
    QUA_S32 target_gain;
    QUA_U32 max_adjust_gain;
    QUA_U32 min_adjust_gain;
    QUA_U32 adjust_speed;
    QUA_S32 ignore_db;
    QUA_S32 reserved;
} qua_audio_agc_cfg_t;

typedef struct qua_audio_aec_cfg {
    QUA_BOOL usr_mode;
    QUA_S16 g_mode;
    QUA_S16 nlp_mode;
    QUA_S16 skew_mode;
    QUA_S16 metrics_mode;
    QUA_S16 delay_logging;
    QUA_S32 skew;
    QUA_U32 delay_time;
    QUA_S32 reserved;
} qua_audio_aec_cfg_t;

typedef struct qua_audio_anr_cfg {
    QUA_BOOL usr_mode;
    QUA_S32 anr_level;
    QUA_S32 reserved;
} qua_audio_anr_cfg_t;

typedef struct qua_audio_hpf_cfg {
    QUA_U32 freq;
} qua_audio_hpf_cfg_t;

typedef struct qua_ao_vqe_cfg {
    QUA_S32 hpf_enable;
    QUA_S32 anr_enable;
    QUA_S32 agc_enable;
    QUA_S32 samplate; // only support 8k,16k

    qua_audio_hpf_cfg_t hpf_cfg;
    qua_audio_anr_cfg_t anr_cfg;
    qua_audio_agc_cfg_t agc_cfg;
} qua_ao_vqe_cfg_t;

typedef struct qua_ai_vqe_cfg {
    QUA_S32 hpf_enable;
    QUA_S32 aec_enable;
    QUA_S32 anr_enable;
    QUA_S32 agc_enable;
    QUA_S32 samplate; // only support 8k,16k

    qua_audio_hpf_cfg_t hpf_cfg;
    qua_audio_aec_cfg_t aec_cfg;
    qua_audio_anr_cfg_t anr_cfg;
    qua_audio_agc_cfg_t agc_cfg;
} qua_ai_vqe_cfg_t;

typedef struct qua_mm_audio_device_t {
    struct qua_mm_device_t parent;

    QUA_S32 dev_num;
    QUA_S32 (*play_init)(QUA_U32 id, qua_audio_cfg_arg* param);
    QUA_S32 (*play_deinit)(QUA_U32 id);
    QUA_S32 (*play_start)(QUA_U32 id);
    QUA_S32 (*play_stop)(QUA_U32 id);
    QUA_S32 (*play_volume)(QUA_U32 id, QUA_U32 vol_type, QUA_U32 vol);
    QUA_S32 (*write_frame)(QUA_U32 id, QUA_VOID_PTR buf, QUA_U32 size);
    QUA_S32 (*play_vqe)(QUA_U32 id, qua_audio_algo_type_t mode, QUA_VOID_PTR vqe);

    QUA_S32 (*capture_init)(QUA_U32 id, qua_audio_cfg_arg* param);
    QUA_S32 (*capture_deinit)(QUA_U32 id);
    QUA_S32 (*capture_start)(QUA_U32 id);
    QUA_S32 (*capture_stop)(QUA_U32 id);
    QUA_S32 (*capture_volume)(QUA_U32 id, QUA_U32 vol_type, QUA_U32 vol);
    QUA_S32 (*read_frame)(QUA_U32 id, QUA_VOID_PTR buf, QUA_U32 size, QUA_U64 *pts);
    QUA_S32 (*capture_vqe)(QUA_U32 id, qua_audio_algo_type_t mode, QUA_VOID_PTR vqe);

    QUA_S32 (*exit)(QUA_U32 id);
} qua_mm_audio_device_t;

typedef struct qua_mm_adec_device {
    struct qua_mm_device_t parent;

    QUA_S32 (*bind_init)(QUA_U32 mod_id);
    QUA_S32 (*bind_exit)(QUA_U32 mod_id);
} qua_mm_adec_device_t;

typedef struct qua_mm_adec_channel {
    struct qua_mm_channel_t parent;

    QUA_S32 (*send_stream)(QUA_ADEC_CHN chn_id, qua_audio_stream_t *stream, QUA_S32 timeout);
    QUA_S32 (*clear_chn_buf)(QUA_ADEC_CHN chn_id);
    QUA_S32 (*get_frame)(QUA_ADEC_CHN chn_id, qua_audio_frame_t *frame, QUA_S32 timeout);
    QUA_S32 (*release_frame)(QUA_ADEC_CHN chn_id, qua_audio_frame_t *frame);
    QUA_S32 (*ext_api)(QUA_ADEC_CHN chn_id, QUA_VOID_PTR attr);
} qua_mm_adec_channel_t;

typedef struct qua_mm_aenc_device {
    struct qua_mm_device_t parent;

    QUA_S32 (*bind_init)(QUA_U32 mod_id);
    QUA_S32 (*bind_exit)(QUA_U32 mod_id);
} qua_mm_aenc_device_t;

typedef struct qua_mm_aenc_channel {
    struct qua_mm_channel_t parent;

    QUA_S32 (*get_stream)(QUA_AENC_CHN chn_id, qua_audio_stream_t *stream, QUA_S32 timeout);
    QUA_S32 (*clear_chn_buf)(QUA_AENC_CHN chn_id);
    QUA_S32 (*send_frame)(QUA_AENC_CHN chn_id, qua_audio_frame_t *frame, QUA_S32 timeout);
    QUA_S32 (*release_frame)(QUA_AENC_CHN chn_id, qua_audio_frame_t *frame);
    QUA_S32 (*ext_api)(QUA_AENC_CHN chn_id, QUA_VOID_PTR attr);
} qua_mm_aenc_channel_t;

#endif // __QUA_MM_AUDIO_MODULE_H__
