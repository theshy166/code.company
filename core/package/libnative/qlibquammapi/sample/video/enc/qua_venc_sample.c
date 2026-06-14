/*
 *  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "video/qua_mm_video.h"
#include "system/qua_mm_system.h"

#define ALIGNTO(x, align) (((x) + (align) - 1) & ~((align) - 1))

static void print_usage(char* prog) {
    printf("Usage: %s -p primary -c chip-name -o os-name -i stream_file -w img-width -h img-height -f coding_format -n channels -v vb_cnt -d yuv_file\n",prog);
    printf("\t -p --primary-user   : whether this sample is primary user(1: primary user; 0: non-primary user)\n");
    printf("\t -c --chip-name      : chip name, such as mc6870 or mc331x\n");
    printf("\t -o --os-name        : os name, such as android or rtt\n");
    printf("\t -i --stream_file    : the full path of the stream file\n");
    printf("\t -w --img-width      : the width of image\n");
    printf("\t -h --img-height     : the height of image\n");
    printf("\t -f --coding_format  : coding type. such as h264/h265/jpeg\n");
    printf("\t -n --channels       : channel of encoder.\n");
    printf("\t -d --output         : the full path of the output file\n");
}

static qua_venc_type_t get_enc_type(char *stype) {
    qua_venc_type_t enc_type = QUA_VENC_TYPE_DUMMY;

    if(strcasecmp(stype, "jpeg") == 0) {
        enc_type = QUA_JPEG;
    } else if(strcasecmp(stype, "h264") == 0) {
        enc_type = QUA_NORMAL_H264;
    } else if(strcasecmp(stype, "h265") == 0) {
        enc_type = QUA_NORMAL_H265;
    } else if(strcasecmp(stype, "s264") == 0) {
        enc_type = QUA_SMART_H264;
    } else if(strcasecmp(stype, "s265") == 0) {
        enc_type = QUA_SMART_H265;
    }
    return enc_type;
}

static void qua_mm_get_sys_ops(const char *platform, QUA_BOOL primary, qua_mm_system_ops_t **sys_ops) {
    qua_mm_system_t *system;
    qua_mm_system_ops_t *ops;
    QUA_U32 blk_size;
    qua_vb_config_t vb_cfg;
    qua_mm_init(primary, platform, &system);
    ops = (qua_mm_system_ops_t *)system;

    memset(&vb_cfg, 0, sizeof(qua_vb_config_t));
    blk_size = 640*480*2;
    vb_cfg.max_pool_cnt = QUA_VB_MAX_POOLS;
    vb_cfg.common_pools[0].block_size = blk_size;
    vb_cfg.common_pools[0].block_cnt = 6;

    if (primary) {
        ops->sys_init(&vb_cfg);
    }

    *sys_ops = ops;
}

void read_yuv(QUA_U32 idx, QUA_U32 width, QUA_U32 height, QUA_U32 stride, void *y_addr, void *uv_addr, FILE *fp) {
    QUA_U32 i = 0;
    QUA_U32 seek = idx * width * height * 3 / 2;

    // copy y
    fseek(fp, 0, SEEK_SET);
    for(; i < height; i++) {
        fseek(fp, seek, SEEK_SET);
        fread(y_addr, 1, width, fp);
        seek += width;
        y_addr += stride;
    }

    // copy uv
    i = 0;
    seek = idx * width * height * 3 / 2 + width * height;
    for(; i < height / 2; i++) {
        fseek(fp, seek, SEEK_SET);
        fread(uv_addr, 1, width, fp);
        seek += width;
        uv_addr += stride;
    }
}

int main(int argc, char *argv[]) {
    QUA_S32 chn_id;
    QUA_S32 ret;

    QUA_CHAR stream_path[256];
    FILE *fp_strm=NULL;
    QUA_CHAR frame_path[256];
    FILE *fp_frm=NULL;
    QUA_CHAR chip_name[16];
    QUA_CHAR os_name[16];
    QUA_CHAR platform[32];
    QUA_CHAR tmp_buf[8];
    QUA_U32 vb_cnt = 0;
    QUA_U32 width;
    QUA_U32 height;
    qua_venc_type_t enc_type;
    qua_venc_chn_attr_t chn_attr;
    qua_venc_chn_cap_t chn_cap;
    qua_venc_attr_jpeg_t jpeg_attr;
    qua_venc_stream_t packet;
    qua_video_frame_info_t frame_info;

    int32_t venc_fd;
    fd_set venc_read_fds;
    struct timeval timeout_val;
    struct timeval start;
    struct timeval end;

    QUA_U64 phy_addr;
    QUA_VOID *vir_addr;
    QUA_U32 yuv_align_size;
    QUA_U32 hor_stride, ver_stride;
    QUA_U32 y_align_size;
    QUA_BOOL primary;
    QUA_U32 i;
    QUA_U64 time_stamp = 0;
    QUA_U32 encode_cnt = 0;
    QUA_BOOL is_jpeg = QUA_FALSE;

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_CHIP_NAME, chip_name, sizeof(chip_name));
    if (ret == QUA_FAILURE) {
        printf("get chip name error!\n");
        print_usage(argv[0]);
        return ret;
    }

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_OS_NAME, os_name, sizeof(os_name));
    if (ret == QUA_FAILURE) {
        printf("get os name error!\n");
        print_usage(argv[0]);
        return ret;
    }

    ret = qua_make_platform(chip_name, os_name, platform, sizeof(platform));
    if (ret == QUA_FAILURE) {
        printf("make platform name error!\n");
        return ret;
    }

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_IMAGE_WIDTH, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get image width error!\n");
        print_usage(argv[0]);
        return ret;
    }
    width = atoi(tmp_buf);

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_IMAGE_HEIGHT, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get image height error!\n");
        print_usage(argv[0]);
        return ret;
    }
    height = atoi(tmp_buf);

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_INPUT_FILE, frame_path, sizeof(frame_path));
    if (ret == QUA_FAILURE) {
        printf("get input file error!\n");
        print_usage(argv[0]);
        return ret;
    }

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_OUTPUT_FILE, stream_path, sizeof(stream_path));
    if (ret == QUA_FAILURE) {
        printf("get ouput file error!\n");
    }

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_CHANNEL_ID, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get channel id error!\n");
        print_usage(argv[0]);
        return ret;
    }
    chn_id = atoi(tmp_buf);

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_CODING_FORMAT, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get coding format error!\n");
        print_usage(argv[0]);
        return ret;
    }
    enc_type = get_enc_type(tmp_buf);

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_PRIMARY_USER, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get primary value error!\n");
        print_usage(argv[0]);
        return ret;
    }
    primary = atoi(tmp_buf);

    printf("==============start qua mm venc test============\n");
    printf("frame file:          %s\n", frame_path);
    printf("stream file:         %s\n", stream_path);
    printf("enc type:            %d\n", enc_type);
    printf("number of channels:  %d\n", chn_id);
    printf("vb_cnt:              %d\n", vb_cnt);
    printf("venc dimention:      %d x %d\n", width, height);

    fp_frm = fopen(frame_path, "rb");
    if (fp_frm == NULL) {
        printf("can't open frame %s (%s)\n", frame_path, strerror(errno));
        return -1;
    }

    fp_strm = fopen(stream_path, "wb");
    if (fp_strm == NULL) {
        printf("can't open stream %s (%s)\n", stream_path, strerror(errno));
    }

    qua_mm_system_ops_t *sys_ops = NULL;
    qua_mm_get_sys_ops(platform, primary, &sys_ops);

    const qua_mm_module_t *video_module = NULL;
    ret = qua_mm_load_module(QUA_MM_MODULE_VIDEO, &video_module);
    if (ret != QUA_SUCCESS || video_module == NULL)
        return -1;

    printf("Module %s, API version %d\n", video_module->id, video_module->api_version);
    qua_mm_device_t *mm_device = NULL;
    ret = video_module->open_device(video_module, QUA_MM_VIDEO_DEV_ENC, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL)
        return -1;
    qua_mm_venc_device_t *venc_device = (qua_mm_venc_device_t *)mm_device;
    printf("Device %s\n", venc_device->parent.id);
    is_jpeg = enc_type == QUA_JPEG;

    qua_mm_channel_t *mm_chn;
    qua_mm_venc_chn_attr_t attr;
    qua_mm_venc_channel_t *venc_chn;
    qua_mm_jpege_channel_t *jpege_chn;
    if (!is_jpeg) {
        chn_cap.support_type = enc_type;
        chn_cap.max_size.width = width;
        chn_cap.max_size.height = height;
        attr.chn_cap = chn_cap;
        attr.video_chn = QUA_MM_VIDEO_VENC_CHN;
        encode_cnt = 30;

        ret = venc_device->parent.create_channel(&venc_device->parent, &chn_id, &attr, &mm_chn);
        if (ret != QUA_SUCCESS) {
            printf("venc can't create channel %d ret 0x%x\n", chn_id, ret);
            goto EXIT;
        }

        venc_chn = (qua_mm_venc_channel_t *)mm_chn;
        qua_venc_chn_attr_t venc_attr;
        switch (enc_type) {
            case QUA_NORMAL_H264: {
                packet.chn_id = chn_id;
                packet.stream_type = QUA_STREAM_H264;

                venc_attr.venc_attr.enc_type = QUA_NORMAL_H264;
                venc_attr.venc_attr.attr_u.attr_h264.profile = QUA_H264_PROFILE_MAIN;
                venc_attr.venc_attr.attr_u.attr_h264.i_frame_interval = 50;
                venc_attr.venc_attr.attr_u.attr_h264.pic_width = width;
                venc_attr.venc_attr.attr_u.attr_h264.pic_height = height;

                venc_attr.venc_rc_attr.rc_mode = QUA_VENC_RC_MODE_H264VBR;
                venc_attr.venc_rc_attr.attr_h264_vbr.max_bitRate = 8 * 1000 * 1000;
                venc_attr.venc_rc_attr.attr_h264_vbr.init_qp = 35;
                venc_attr.venc_rc_attr.attr_h264_vbr.max_iqp = 50;
                venc_attr.venc_rc_attr.attr_h264_vbr.min_iqp = 28;
                venc_attr.venc_rc_attr.attr_h264_vbr.max_qp = 50;
                venc_attr.venc_rc_attr.attr_h264_vbr.min_qp = 28;
                venc_attr.venc_rc_attr.attr_h264_vbr.src_frmrate = 30;
                venc_attr.venc_rc_attr.attr_h264_vbr.max_rate_percent = 200;
                venc_attr.venc_rc_attr.attr_h264_vbr.i_frm_max_bits = 0;
                venc_attr.venc_rc_attr.attr_h264_vbr.ip_qp_delta = 5;
                venc_attr.venc_rc_attr.attr_h264_vbr.i_bit_prop = 15;
                venc_attr.venc_rc_attr.attr_h264_vbr.p_bit_prop = 1;
                venc_attr.venc_rc_attr.attr_h264_vbr.fluctuate_level = 0;
            } break;
            case QUA_NORMAL_H265: {
                packet.chn_id = chn_id;
                packet.stream_type = QUA_STREAM_H265;

                venc_attr.venc_attr.enc_type = QUA_NORMAL_H265;
                venc_attr.venc_attr.attr_u.attr_h265.profile = QUA_H265_PROFILE_MAIN;
                venc_attr.venc_attr.attr_u.attr_h265.i_frame_interval = 50;
                venc_attr.venc_attr.attr_u.attr_h265.pic_width = width;
                venc_attr.venc_attr.attr_u.attr_h265.pic_height = height;

                venc_attr.venc_rc_attr.rc_mode = QUA_VENC_RC_MODE_H265VBR;
                venc_attr.venc_rc_attr.attr_h265_vbr.max_bitRate = 8 * 1000 * 1000;
                venc_attr.venc_rc_attr.attr_h265_vbr.init_qp = 35;
                venc_attr.venc_rc_attr.attr_h265_vbr.max_iqp = 50;
                venc_attr.venc_rc_attr.attr_h265_vbr.min_iqp = 28;
                venc_attr.venc_rc_attr.attr_h265_vbr.max_qp = 50;
                venc_attr.venc_rc_attr.attr_h265_vbr.min_qp = 28;
                venc_attr.venc_rc_attr.attr_h265_vbr.src_frmrate = 30;
                venc_attr.venc_rc_attr.attr_h265_vbr.max_rate_percent = 200;
                venc_attr.venc_rc_attr.attr_h265_vbr.i_frm_max_bits = 0;
                venc_attr.venc_rc_attr.attr_h265_vbr.ip_qp_delta = 5;
                venc_attr.venc_rc_attr.attr_h265_vbr.i_bit_prop = 15;
                venc_attr.venc_rc_attr.attr_h265_vbr.p_bit_prop = 1;
                venc_attr.venc_rc_attr.attr_h265_vbr.fluctuate_level = 0;
            } break;
            default:
                break;
        }

        ret = venc_chn->set_chn_attr(chn_id, &venc_attr);
        if (ret != QUA_SUCCESS) {
            printf("venc set_chn_attr %d failed ret 0x%x\n", chn_id, ret);
            goto EXIT;
        }
    } else {
        chn_attr.venc_attr.enc_type = enc_type;
        jpeg_attr.max_pic_width  = width;
        jpeg_attr.max_pic_height = height;
        jpeg_attr.pic_width  = width;
        jpeg_attr.pic_height = height;
        jpeg_attr.stream_buf_size = width * height;
        jpeg_attr.by_frame = QUA_TRUE;
        jpeg_attr.support_dcf = QUA_FALSE;
        memcpy(&chn_attr.venc_attr.attr_u.jpeg_u.attr_jpeg, &jpeg_attr, sizeof(qua_venc_attr_jpeg_t));
        attr.chn_attr = chn_attr;
        attr.video_chn = QUA_MM_VIDEO_JPEGE_CHN;
        encode_cnt = 1;

        qua_mm_channel_t *mm_chn;
        ret = venc_device->parent.create_channel(&venc_device->parent, &chn_id, &attr, &mm_chn);
        if (ret != QUA_SUCCESS) {
            printf("venc can't create channel %d ret 0x%x\n", chn_id, ret);
            goto EXIT;
        }

        jpege_chn = (qua_mm_jpege_channel_t *)mm_chn;
        packet.chn_id = chn_id;
        packet.stream_type = QUA_STREAM_JPEG;
    }

    ret = is_jpeg ? jpege_chn->start_recv_pic(chn_id) :
                    venc_chn->start_recv_pic(chn_id);
    if (ret != QUA_SUCCESS) {
        printf("venc channel %d start receive picture failed ret 0x%x\n", chn_id, ret);
        goto START_FAILED;
    }

    QUA_U32 align = is_jpeg ? 16 : 64;
    hor_stride = ALIGNTO(width, align);
    ver_stride = ALIGNTO(height, align);
    y_align_size = hor_stride * ver_stride;
    yuv_align_size = hor_stride * ver_stride * 3 / 2;
    sys_ops->sys_mmz_alloc("test_enc_buffer", NULL, yuv_align_size, &phy_addr, &vir_addr);

    for (i = 0; i < encode_cnt; i++) {
        memset(&frame_info, 0, sizeof(qua_video_frame_info_t));
        frame_info.pool_id = -1;
        frame_info.video_frame.width = width;
        frame_info.video_frame.height = height;
        frame_info.video_frame.field = QUA_VIDOE_FIELD_FRAME;
        frame_info.video_frame.pixel_fmt = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
        frame_info.video_frame.video_fmt = QUA_VIDEO_FORMAT_LINEAR;
        frame_info.video_frame.compress_mode = QUA_COMPRESS_MODE_NONE;
        frame_info.video_frame.stride[0]= hor_stride;
        frame_info.video_frame.phy_addr[0]= phy_addr;
        frame_info.video_frame.phy_addr[1]= phy_addr + y_align_size;
        frame_info.video_frame.vir_addr[0]= (QUA_U64)vir_addr;
        frame_info.video_frame.vir_addr[1]= (QUA_U64)vir_addr + y_align_size;
        frame_info.video_frame.pts = time_stamp;
        time_stamp += 33333;

        printf("read yuv idx %d\n", i);

        read_yuv(i, width, height, hor_stride,
            frame_info.video_frame.vir_addr[0],
            frame_info.video_frame.vir_addr[1],
            fp_frm);

        ret = is_jpeg ? jpege_chn->send_frame(chn_id, &frame_info, -1) :
                        venc_chn->send_frame(chn_id, &frame_info, -1);
        if (ret != QUA_SUCCESS) {
            goto ENCODE_FAILED;
        }

        gettimeofday(&start, NULL);
        if (is_jpeg) {
            venc_fd = jpege_chn->get_chn_fd(chn_id);
            FD_ZERO(&venc_read_fds);
            FD_SET(venc_fd, &venc_read_fds);
            timeout_val.tv_sec = 1;
            timeout_val.tv_usec = 0;

            ret = select(venc_fd + 1, &venc_read_fds, NULL, NULL, &timeout_val);
            if (ret <= 0) {
                printf("select timeout\n");
                goto ENCODE_FAILED;
            }
        }

        ret = is_jpeg ? jpege_chn->get_stream(chn_id, &packet, 0) :
                        venc_chn->get_stream(chn_id, &packet, 1000);
        if (ret != QUA_SUCCESS) {
            printf("venc %d get Stream failed ret 0x%x\n", chn_id, ret);
            goto ENCODE_FAILED;
        }
        gettimeofday(&end, NULL);
        printf("cost %ld ms\n", (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000);

        if (fp_strm) {
            if (enc_type == QUA_JPEG)
                fwrite(packet.jpeg_stream.vir_addr, 1, packet.jpeg_stream.length, fp_strm);
            else if (enc_type == QUA_NORMAL_H264 || enc_type == QUA_SMART_H264)
                fwrite(packet.h264_stream.start, 1, packet.h264_stream.length, fp_strm);
            else
                fwrite(packet.h265_stream.start, 1, packet.h265_stream.length, fp_strm);
            fflush(fp_strm);
        }

        if (is_jpeg) {
            ret = jpege_chn->release_stream(chn_id, &packet);
            printf("stream addr 0x%x size %d\n", packet.jpeg_stream.vir_addr, packet.jpeg_stream.length);
        } else {
            ret = venc_chn->release_stream(chn_id, &packet);
            printf("stream chn_id %d addr 0x%x size %d frame type %d nalu cnt %d time_stamp %d\n", packet.chn_id, packet.h264_stream.start,
                packet.h264_stream.length, packet.h264_stream.frame_type, packet.h264_stream.nalu_cnt, packet.h264_stream.time_stamp);
        }
    }

    sys_ops->sys_mmz_free(phy_addr, vir_addr);
ENCODE_FAILED:
    ret = is_jpeg ? jpege_chn->stop_recv_pic(chn_id) : venc_chn->stop_recv_pic(chn_id);
    if (ret != QUA_SUCCESS) {
        printf("venc channel %d stop failed ret 0x%x\n", chn_id, ret);
        return  -1;
    }
START_FAILED:
    ret = is_jpeg ? jpege_chn->parent.release(&jpege_chn->parent) :
                    venc_chn->parent.release(&venc_chn->parent);
    if (ret != QUA_SUCCESS) {
        printf("venc channel %d destroy failed ret 0x%x\n", chn_id, ret);
        return  -1;
    }

    venc_device->parent.close(&venc_device->parent);
EXIT:
    if (fp_strm)
        fclose(fp_strm);

    if (fp_frm)
        fclose(fp_frm);

    printf("==============qua mm venc test done============\n");
}
