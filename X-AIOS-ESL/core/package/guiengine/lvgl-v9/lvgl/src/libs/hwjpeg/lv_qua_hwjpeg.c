/*
 * Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#include "../../../lvgl.h"
#if LV_USE_HW_JPEG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "lv_qua_hwjpeg.h"

#include "utils/qua_video_parser.h"
#include "utils/qua_video_utils.h"
#include "video/qua_mm_video.h"
#include "video/qua_mm_video_type.h"
#include "common/qua_mm_common.h"
#include "common/qua_mm_video_common_type.h"
#include "filter/qua_mm_tde.h"
#include "filter/qua_mm_filter.h"
#include "system/qua_mm_system.h"
#include "quagl/qua_gl_buffer.h"

#define MIN_JPEGD_BUF_SIZE (0x8000)

typedef struct qua_hw_jpeg_decoder_instance {
    qua_mm_device_t *device;
    qua_mm_channel_t *channel;
    QUA_S32 channel_id;

    qua_gl_buffer_t *gl_buffer;
} qua_hw_jpeg_decoder_instance_t;

static lv_result_t decoder_info(lv_image_decoder_t *decoder, const void *src, lv_image_header_t *header);
static lv_result_t decoder_open(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc);
static void decoder_close(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc);
static uint8_t *read_file(const char *filename, uint32_t *size);
static lv_draw_buf_t *decode_jpeg_file(const char *filename, lv_image_decoder_dsc_t *dsc);
static lv_draw_buf_t *draw_buf_create(qua_video_frame_info_t *frame_info, qua_hw_jpeg_decoder_instance_t *instance);

/**
 * Register qua hardware jpeg decoder functions in lvgl.
 */
void lv_qua_hwjpeg_init(void) {
    printf("%s %d in\n", __func__, __LINE__);
    lv_image_decoder_t *dec = lv_image_decoder_create();
    lv_image_decoder_set_info_cb(dec, decoder_info);
    lv_image_decoder_set_open_cb(dec, decoder_open);
    lv_image_decoder_set_close_cb(dec, decoder_close);
}

void lv_qua_hwjpeg_deinit(void) {
    printf("%s %d in\n", __func__, __LINE__);
    lv_image_decoder_t *dec = NULL;
    while((dec = lv_image_decoder_get_next(dec)) != NULL) {
        if(dec->info_cb == decoder_info) {
            lv_image_decoder_delete(dec);
            break;
        }
    }
}

/**
 * Get info about a JPEG image
 * @param src can be file name or pointer to a C array
 * @param header store the info here
 * @return LV_RESULT_OK: no error; LV_RESULT_INVALID: can't get the info
 */
static lv_result_t decoder_info(lv_image_decoder_t *decoder, const void *src, lv_image_header_t *header) {
    LV_UNUSED(decoder);
    lv_image_src_t src_type = lv_image_src_get_type(src);

    if (src_type == LV_IMAGE_SRC_FILE) {
        const char *fn = src;
        if (fn[0] == 'H') {
            fn = src + 2;
        }
        if (!qua_is_jpeg_file(fn)) {
            return LV_RESULT_INVALID;
        }

        int32_t fd;
        uint32_t width;
        uint32_t height;
        qua_jpeg_info_entry_t entry;
        fd = open(fn, O_RDONLY);
        if (fd < 0) {
            LV_LOG_WARN("Failed to open file: %s", fn);
            return LV_RESULT_INVALID;
        }

        if (qua_get_jpeg_info_by_fd(fd, &entry) != QUA_SUCCESS) {
            LV_LOG_WARN("Failed to get JPEG info from file: %s", fn);
            close(fd);
            return LV_RESULT_INVALID;
        }

        close(fd);

        /*Save the data in the header*/
        header->cf = LV_COLOR_FORMAT_ARGB8888;
        header->w = entry.width;
        header->h = entry.height;
        return LV_RESULT_OK;
    }

    return LV_RESULT_INVALID;
}

/**
 * Open a JPEG image and return the decided image
 * @param decoder pointer to the decoder
 * @param dsc     pointer to the decoder descriptor
 * @return LV_RESULT_OK: no error; LV_RESULT_INVALID: can't open the image
 */
static lv_result_t decoder_open(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc) {
    LV_UNUSED(decoder); /*Unused*/
    if (dsc->src_type == LV_IMAGE_SRC_FILE) {
        const char *fn = dsc->src;
        lv_draw_buf_t *decoded = decode_jpeg_file(fn, dsc);
        if (decoded == NULL) {
            LV_LOG_WARN("decode jpeg file failed");
            return LV_RESULT_INVALID;
        }

        dsc->decoded = decoded;
        return LV_RESULT_OK;
    }
}

/**
 * Free the allocated resources
 */
static void decoder_close(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc) {
    LV_UNUSED(decoder); /*Unused*/
    qua_hw_jpeg_decoder_instance_t *instance = (qua_hw_jpeg_decoder_instance_t *)dsc->user_data;
    if (instance) {
        if (instance->channel) {
            qua_mm_vdec_channel_t *vdec_chn = (qua_mm_vdec_channel_t *)instance->channel;
            vdec_chn->stop_chn(instance->channel_id);
            instance->channel->release(instance->channel);
            instance->channel = NULL;
        }
        if (instance->device) {
            instance->device->close(instance->device);
            instance->device = NULL;
        }

        if (instance->gl_buffer != NULL) {
            qua_gl_buffer_free(instance->gl_buffer);
            instance->gl_buffer = NULL;
        }
        free(instance);
        instance = NULL;
        dsc->user_data = NULL;
    }

    if (dsc->decoded) {
        lv_free(dsc->decoded);
    }
}

static uint8_t *read_file(const char *filename, uint32_t *size) {
    uint8_t *data = NULL;
    lv_fs_file_t f;
    uint32_t data_size;
    uint32_t rn;
    lv_fs_res_t res;

    *size = 0;

    res = lv_fs_open(&f, filename, LV_FS_MODE_RD);
    if(res != LV_FS_RES_OK) {
        LV_LOG_ERROR("can't open %s", filename);
        return NULL;
    }

    res = lv_fs_seek(&f, 0, LV_FS_SEEK_END);
    if(res != LV_FS_RES_OK) {
        goto failed;
    }

    res = lv_fs_tell(&f, &data_size);
    if(res != LV_FS_RES_OK) {
        goto failed;
    }

    res = lv_fs_seek(&f, 0, LV_FS_SEEK_SET);
    if(res != LV_FS_RES_OK) {
        goto failed;
    }

    /*Read file to buffer*/
    data = lv_malloc(data_size);
    if(data == NULL) {
        LV_LOG_ERROR("malloc failed for data");
        goto failed;
    }

    res = lv_fs_read(&f, data, data_size, &rn);

    if(res == LV_FS_RES_OK && rn == data_size) {
        *size = rn;
    }
    else {
        LV_LOG_ERROR("read file failed");
        lv_free(data);
        data = NULL;
    }

failed:
    lv_fs_close(&f);

    return data;
}

static int32_t decoder_transform(qua_video_frame_info_t *frame_info, qua_hw_jpeg_decoder_instance_t *instance) {
    QUA_S32 ret = QUA_SUCCESS;
    const qua_mm_module_t *mm_mod = NULL;
    qua_mm_device_t *mm_dev = NULL;
    qua_mm_vgs_device_t *vgs_device = NULL;
    qua_video_frame_t frame_in;
    qua_video_frame_t frame_out;
    QUA_VGS_HANDLE handle;
    QUA_U32 size;

    memset(&frame_in, 0, sizeof(qua_video_frame_t));
    memset(&frame_out, 0, sizeof(qua_video_frame_t));
    ret = qua_mm_load_module(QUA_MM_MODULE_FILTER, &mm_mod);
    if (ret != QUA_SUCCESS || mm_mod == NULL)
        return -1;

    ret = mm_mod->open_device(mm_mod, QUA_MM_FILTER_DEV_VGS, 0, &mm_dev);
    if (ret != QUA_SUCCESS || mm_dev == NULL)
        return -1;

    instance->gl_buffer = qua_gl_buffer_alloc("jpeg_dec", frame_info->video_frame.width, frame_info->video_frame.height, QUA_PIXEL_FMT_RGB_8888, 0);
    if (instance->gl_buffer == NULL) {
        ret = QUA_FAILURE;
        LV_LOG_ERROR("Failed alloc mmz jpeg buffer");
        goto failed;
    }

    frame_in = frame_info->video_frame;
    frame_out.width = frame_in.width;
    frame_out.height = frame_in.height;
    frame_out.pixel_fmt = QUA_PIXEL_FMT_RGB_8888;
    frame_out.phy_addr[0] = instance->gl_buffer->phy_addr;
    frame_out.vir_addr[0] = instance->gl_buffer->vir_addr;
    frame_out.stride[0] = qua_gl_buffer_get_hor_stride(instance->gl_buffer) * 4;

    vgs_device = (qua_mm_vgs_device_t *)mm_dev;
    ret = vgs_device->qua_vgs_begin_job(&handle);
    if (ret != QUA_SUCCESS) {
        LV_LOG_ERROR("Failed to begin vgs job");
        goto failed;
    }

    if (vgs_device->qua_vgs_format_convert_new != NULL) {
        ret = vgs_device->qua_vgs_format_convert_new(handle, &frame_in, &frame_out);
    } else {
        qua_tde2_surface_t surface;
        memset(&surface, 0x00, sizeof(surface));
        surface.color_fmt = QUA_TDE2_COLOR_FMT_ARGB8888;
        surface.width = frame_out.width;
        surface.height = frame_out.height;
        surface.stride = qua_gl_buffer_get_hor_stride(instance->gl_buffer) * 4;
        surface.phy_addr = instance->gl_buffer->phy_addr;
        surface.fbc_compress = QUA_FALSE;
        ret = vgs_device->qua_vgs_format_convert(handle, &frame_in, &surface, QUA_TRUE);
    }

    if (ret != QUA_SUCCESS) {
        vgs_device->qua_vgs_cancel_job(handle);
    }

failed:
    if (ret) {
        if (instance->gl_buffer != NULL) {
            qua_gl_buffer_free(instance->gl_buffer);
            instance->gl_buffer = NULL;
        }
    }

    if (vgs_device) {
        vgs_device->qua_vgs_end_job(handle);
    }

    if (mm_dev) {
        mm_dev->close(mm_dev);
        mm_dev = NULL;
    }

    return (ret == QUA_SUCCESS) ? 0 : -1;
}

static lv_draw_buf_t *draw_buf_create(qua_video_frame_info_t *frame_info, qua_hw_jpeg_decoder_instance_t *instance) {
    lv_draw_buf_t *draw_buf = NULL;
    int32_t ret = 0;

    ret = decoder_transform(frame_info, instance);
    if (ret != 0) {
        return NULL;
    }

    draw_buf = lv_malloc_zeroed(sizeof(lv_draw_buf_t));
    LV_ASSERT_MALLOC(draw_buf);
    if (draw_buf == NULL) {
        LV_LOG_ERROR("Failed to allocate memory for draw_buf");
        if (instance->gl_buffer != NULL) {
            qua_gl_buffer_free(instance->gl_buffer);
            instance->gl_buffer = NULL;
        }
        return NULL;
    }

    draw_buf->header.w = frame_info->video_frame.width;
    draw_buf->header.h = frame_info->video_frame.height;
    draw_buf->header.cf = LV_COLOR_FORMAT_ARGB8888;
    draw_buf->header.flags = LV_IMAGE_FLAGS_USER1;
    draw_buf->header.stride = qua_gl_buffer_get_hor_stride(instance->gl_buffer) * 4;
    draw_buf->header.magic = LV_IMAGE_HEADER_MAGIC;
    draw_buf->data = instance->gl_buffer->vir_addr;
    draw_buf->unaligned_data = instance->gl_buffer->vir_addr;
    draw_buf->data_size = draw_buf->header.stride * draw_buf->header.h;
    draw_buf->phy_addr = instance->gl_buffer->phy_addr;
    return draw_buf;
}

static lv_draw_buf_t *decode_jpeg_file(const char *filename, lv_image_decoder_dsc_t *dsc) {
    uint8_t *data = NULL;
    uint32_t data_size = 0;
    int32_t vdec_fd;
    fd_set vdec_read_fds;
    struct timeval timeout_val;

    QUA_S32 ret = QUA_SUCCESS;
    QUA_S32 channel_id = -1;
    const qua_mm_module_t *mm_mod = NULL;
    qua_mm_device_t *mm_dev = NULL;
    qua_mm_channel_t *mm_channel = NULL;
    qua_mm_vdec_channel_t *vdec_chn = NULL;
    qua_mm_vdec_chn_attr_t vdec_attr;
    qua_vdec_stream_t packet;
    qua_video_frame_info_t frame_info;
    lv_draw_buf_t *decoded = NULL;
    qua_hw_jpeg_decoder_instance_t *instance = NULL;

    memset(&packet, 0, sizeof(qua_vdec_stream_t));
    memset(&vdec_attr, 0, sizeof(qua_mm_vdec_chn_attr_t));
    memset(&frame_info, 0, sizeof(qua_video_frame_info_t));

    ret = qua_mm_load_module(QUA_MM_MODULE_VIDEO, &mm_mod);
    if (ret != QUA_SUCCESS)
        return NULL;

    ret = mm_mod->open_device(mm_mod, QUA_MM_VIDEO_DEV_DEC, 0, &mm_dev);
    if (ret != QUA_SUCCESS)
        return NULL;

    data = read_file(filename, &data_size);
    if (data == NULL || data_size == 0) {
        LV_LOG_ERROR("Failed to read file: %s", filename);
        ret = QUA_FAILURE;
        goto failed;
    }

    vdec_attr.chn_attr.pic_width = dsc->header.w;
    vdec_attr.chn_attr.pic_height = dsc->header.h;
    vdec_attr.chn_attr.coding_type = QUA_VIDEO_CodingJpeg;
    vdec_attr.chn_attr.in_stream_buf_size = data_size + 1024;  // decoder will add header data, so larger than actual size
    vdec_attr.chn_attr.vdec_jpeg_attr.mode = QUA_VIDEO_MODE_FRAME;
    vdec_attr.chn_attr.vdec_jpeg_attr.jpeg_fmt = QUA_JPG_COLOR_FMT_YCBCR420;
    vdec_attr.vb_cnt = 1;
    if (vdec_attr.chn_attr.in_stream_buf_size < MIN_JPEGD_BUF_SIZE)
        vdec_attr.chn_attr.in_stream_buf_size = MIN_JPEGD_BUF_SIZE;
    ret = mm_dev->create_channel(mm_dev, &channel_id, &vdec_attr, &mm_channel);
    if (ret != QUA_SUCCESS) {
        LV_LOG_ERROR("Failed to create vdec channel");
        goto failed;
    }

    vdec_chn = (qua_mm_vdec_channel_t *)mm_channel;
    ret = vdec_chn->start_chn(channel_id);
    if (ret != QUA_SUCCESS) {
        LV_LOG_ERROR("Failed to start vdec channel");
        goto failed;
    }

    packet.in_data = (QUA_U64)data;
    packet.in_size = data_size;
    packet.pts = 0;
    packet.end_of_frame = QUA_TRUE;
    packet.end_of_stream = QUA_TRUE;
    ret = vdec_chn->send_stream(channel_id, &packet, 100);
    if (ret != QUA_SUCCESS) {
        LV_LOG_ERROR("Failed to send vdec stream");
        goto failed;
    }

    vdec_fd = vdec_chn->get_chn_fd(channel_id);
    FD_ZERO(&vdec_read_fds);
    FD_SET(vdec_fd, &vdec_read_fds);
    timeout_val.tv_sec = 1;
    timeout_val.tv_usec = 0;

    ret = select(vdec_fd + 1, &vdec_read_fds, NULL, NULL, &timeout_val);
    if (ret <= 0) {
        LV_LOG_ERROR("select failed or timeout");
        goto failed;
    }

    instance = calloc(1, sizeof(qua_hw_jpeg_decoder_instance_t));
    if (instance == NULL) {
        LV_LOG_ERROR("Failed to allocate memory for instance");
        ret = QUA_FAILURE;
        goto failed;
    }

    ret = vdec_chn->get_frame(channel_id, &frame_info, 0);
    if (ret != QUA_SUCCESS) {
        LV_LOG_ERROR("Failed to get vdec frame");
        goto failed;
    }

    decoded = draw_buf_create(&frame_info, instance);
    if (decoded != NULL) {
        instance->device = mm_dev;
        instance->channel = mm_channel;
        instance->channel_id = channel_id;
        dsc->user_data = instance;
    } else {
        ret = QUA_FAILURE;
    }

    vdec_chn->release_frame(instance->channel_id, &frame_info);
failed:
    if (ret) {
        if (mm_channel) {
            mm_channel->release(mm_channel);
            mm_channel = NULL;
        }

        if (mm_dev) {
            mm_dev->close(mm_dev);
            mm_dev = NULL;
        }

        if (instance) {
            free(instance);
            instance = NULL;
        }
    }

    if (data) {
        lv_free(data);
        data = NULL;
    }

    return decoded;
}

#endif