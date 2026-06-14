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

#define INPUT_FRAME_NUM     300
#define ALIGNTO(x, align) (((x) + (align) - 1) & ~((align) - 1))

static void print_usage(char* prog) {
    printf("Usage: %s -p primary -c --chip-name -o --os-name -i stream_file -w img-width -h img-height -f coding_format -n channels -v vb_count --d yuv_file\n",prog);
    printf("\t -p --primary-user   : whether this sample is primary user(1: primary user; 0: non-primary user)\n");
    printf("\t -c --chip-name      : chip name, such as mc6870 or mc331x\n");
    printf("\t -o --os-name        : os name, such as android or rtt\n");
    printf("\t -i --stream_file    : the full path of the stream file\n");
    printf("\t -w --img-width      : the width of image\n");
    printf("\t -h --img-height     : the height of image\n");
    printf("\t -f --coding_format  : coding type. such as h264\n");
    printf("\t -n --channels       : channel of decoder.\n");
    printf("\t -v --vb_count       : vb cnt. \n");
    printf("\t -d --output         : the full path of the output file\n");
}

static qua_coding_type_t get_coding_type(char *stype) {
    qua_coding_type_t coding_type = QUA_VIDEO_CodingUnused;

    if(strcasecmp(stype, "h264") == 0) {
        coding_type = QUA_VIDEO_CodingAVC;
    } else if(strcasecmp(stype, "h265") == 0) {
        coding_type = QUA_VIDEO_CodingHEVC;
    } else if(strcasecmp(stype, "jpeg") == 0) {
        coding_type = QUA_VIDEO_CodingJpeg;
    } else if(strcasecmp(stype, "mjpeg") == 0) {
        coding_type = QUA_VIDEO_CodingMJpeg;
    }
    return coding_type;
}

QUA_S32 findStreamFrame(QUA_U8* pu8Buf, QUA_S32 bufLen, qua_coding_type_t codingType, QUA_S32* s32Start) {
    QUA_BOOL bFindStart = QUA_FALSE;
    QUA_BOOL bFindEnd = QUA_FALSE;
    QUA_S32 i, s32FrameLen = -1;
    QUA_U32 u32Len = 0;
    switch(codingType) {
        case QUA_VIDEO_CodingAVC: {
            *s32Start = 0;
            for (i=0; i<bufLen - 9; i++) {
                int tmp = pu8Buf[i+4] & 0x1F;
                if (pu8Buf[i] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 0 && pu8Buf[i+3] == 1 &&
                       (tmp == 7 || tmp == 8 || tmp == 6 || tmp == 5 ||tmp == 1)
                   ) {
                    bFindStart = QUA_TRUE;
                    i += 9;
                    break;
                }
            }

            for (; i<bufLen-9; i++) {
                int tmp = pu8Buf[i+4] & 0x1F;
                if (  pu8Buf[i] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 0 && pu8Buf[i+3] == 1 &&
                   (tmp == 7 || tmp == 8 || tmp == 6 || tmp == 5 ||tmp == 1)
                   ) {
                    bFindEnd = QUA_TRUE;
                    break;
                }
            }

            if(i > 0) s32FrameLen = i;
            if (bFindStart == QUA_FALSE)
            {
                printf("can not find H264 start code\n");
                s32FrameLen = -1;
            }
            if (bFindEnd == QUA_FALSE)
            {
                s32FrameLen = i + 9;
            }
        } break;
        case QUA_VIDEO_CodingHEVC: {
            *s32Start = 0;
            QUA_BOOL  bNewPic = QUA_FALSE;
            for (i = 0; i < bufLen - 7; i++) {
                QUA_U32 tmp = (pu8Buf[i+4]&0x7E)>>1;
                bNewPic = (QUA_BOOL)( pu8Buf[i+0] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 0 && pu8Buf[i+3] == 1
                            && (tmp <= 21) && ((pu8Buf[i+6]&0x80) == 0x80) );
                if (bNewPic) {
                    bFindStart = QUA_TRUE;
                    i += 7;
                    break;
                }
            }

            for (; i<bufLen-7; i++) {
                QUA_U32 tmp = (pu8Buf[i+4]&0x7E)>>1;
                bNewPic = (QUA_BOOL)(pu8Buf[i+0] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 0 && pu8Buf[i+3] == 1
                            &&( tmp == 32 || tmp == 33 || tmp == 34 || tmp == 39 || tmp == 40 || ((tmp <= 21) && (pu8Buf[i+6]&0x80) == 0x80) )
                             );
                if (bNewPic) {
                    bFindEnd = QUA_TRUE;
                    break;
                }
            }
            if(i>0)s32FrameLen = i;
            if (bFindStart == QUA_FALSE) {
                printf("can not find H265 start code!\n");
                s32FrameLen = -1;
            }
            if (bFindEnd == QUA_FALSE) {
                s32FrameLen = i + 7;
            }
        } break;
        case QUA_VIDEO_CodingJpeg:
        case QUA_VIDEO_CodingMJpeg: {
            for (i = 0; i< bufLen - 1; i++) {
                if (pu8Buf[i] == 0xFF && pu8Buf[i+1] == 0xD8) {
                    *s32Start = i;
                    bFindStart = QUA_TRUE;
                    i = i + 2;
                    break;
                }
            }
            for (; i< bufLen - 3; i++) {
                if ((pu8Buf[i] == 0xFF) && (pu8Buf[i+1]& 0xF0) == 0xE0) {
                     u32Len = (pu8Buf[i+2]<<8) + pu8Buf[i+3];
                     i += 1 + u32Len;
                } else {
                    break;
                }
            }

            for (; i<bufLen-1; i++) {
                if (pu8Buf[i] == 0xFF && pu8Buf[i+1] == 0xD9) {
                    bFindEnd = QUA_TRUE;
                    break;
                }
            }
            s32FrameLen = i + 2;
            if (bFindStart == QUA_FALSE) {
                printf("can not find JPEG start code!\n");
                s32FrameLen = -1;
            }
        } break;
        default:
            break;
    }
    return s32FrameLen;
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

int main(int argc, char* argv[]) {
    QUA_CHAR stream_path[256];
    FILE *fp_strm = NULL;
    QUA_CHAR frame_path[256];
    FILE *fp_frm = NULL;
    QUA_CHAR chip_name[16];
    QUA_CHAR os_name[16];
    QUA_CHAR platform[32];
    QUA_CHAR tmp_buf[8];
    QUA_S32 vb_cnt = 0;
    QUA_S32 chn_id;
    QUA_U32 width;
    QUA_U32 height;
    QUA_U8 *pu8_buf = NULL;
    QUA_U32 buf_len = 0;
    QUA_U32 used_bytes = 0;
    QUA_U32 read_len = 0;
    QUA_U32 frame_len = 0;
    QUA_S32 start = 0;
    QUA_U32 frame_cnt = 0;
    QUA_U32 max_input_cnt = 0;
    qua_coding_type_t coding_type;
    qua_vdec_stream_t packet;
    qua_video_frame_info_t out_frame;
    QUA_S32 err = 0;
    QUA_BOOL primary;

    QUA_S32 vdec_fd;
    fd_set vdec_read_fds;
    struct timeval timeout_val;
    QUA_S32 ret;

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

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_INPUT_FILE, stream_path, sizeof(stream_path));
    if (ret == QUA_FAILURE) {
        printf("get input file error!\n");
        print_usage(argv[0]);
        return ret;
    }

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_OUTPUT_FILE, frame_path, sizeof(frame_path));
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

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_VB_COUNT, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get channel id error!\n");
        print_usage(argv[0]);
        return ret;
    }
    vb_cnt = atoi(tmp_buf);

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_CODING_FORMAT, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get coding format error!\n");
        print_usage(argv[0]);
        return ret;
    }
    coding_type = get_coding_type(tmp_buf);

    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_PRIMARY_USER, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get primary value error!\n");
        print_usage(argv[0]);
        return ret;
    }
    primary = atoi(tmp_buf);

    printf("==============start qua mm vdec test============\n");
    printf("stream file:        %s\n", stream_path);
    printf("frame file:         %s\n", frame_path);
    printf("coding_type:        %d\n", coding_type);
    printf("number of channels: %d\n", chn_id);
    printf("vb_cnt:             %d\n", vb_cnt);
    printf("vdec dimention:     %d x %d\n", width, height);

    fp_strm = fopen(stream_path, "rb");
    if (fp_strm == NULL) {
        printf("can't open stream %s (%s)\n", stream_path, strerror(errno));
        return -1;
    }

    fp_frm = fopen(frame_path, "wb");
    if (fp_frm == NULL) {
        printf("can't open frm %s (%s)\n", frame_path, strerror(errno));
    }

    printf("platform: %s\n", platform);
    qua_mm_system_ops_t *sys_ops = NULL;
    qua_mm_get_sys_ops(platform, primary, &sys_ops);

    const qua_mm_module_t *video_module = NULL;
    ret = qua_mm_load_module(QUA_MM_MODULE_VIDEO, &video_module);
    if (ret != QUA_SUCCESS || video_module == NULL)
        return -1;

    printf("Module %s, API version %d\n", video_module->id, video_module->api_version);
    qua_mm_device_t *mm_device = NULL;
    ret = video_module->open_device(video_module, QUA_MM_VIDEO_DEV_DEC, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL)
        return -1;
    qua_mm_vdec_device_t *vdec_device = (qua_mm_vdec_device_t *)mm_device;
    printf("Device %s\n", vdec_device->parent.id);

    QUA_U32 bufSize = width * height;
    if (width * height > 3840 * 2160)
        bufSize = bufSize * 2;

    qua_vdec_chn_attr_t chn_attr;
    chn_attr.coding_type = coding_type;
    chn_attr.in_stream_buf_size = bufSize;
    chn_attr.priority = 5;
    chn_attr.pic_width = width;
    chn_attr.pic_height = height;
    chn_attr.vdec_video_attr.mode = QUA_VIDEO_MODE_FRAME;

    if (coding_type == QUA_VIDEO_CodingAVC) {
        chn_attr.vdec_video_attr.temporal_mvp_enable = 0;
    } else if (coding_type == QUA_VIDEO_CodingHEVC) {
        chn_attr.vdec_video_attr.temporal_mvp_enable = 1;
    } else if (coding_type == QUA_VIDEO_CodingJpeg || coding_type == QUA_VIDEO_CodingMJpeg) {
        chn_attr.vdec_jpeg_attr.mode = QUA_VIDEO_MODE_FRAME;
        chn_attr.vdec_jpeg_attr.jpeg_fmt = QUA_JPG_COLOR_FMT_YCBCR420;
    }

    qua_mm_channel_t *mm_channel;
    qua_mm_vdec_chn_attr_t vdec_chn_attr;
    vdec_chn_attr.chn_attr = chn_attr;
    vdec_chn_attr.vb_cnt = vb_cnt;
    vdec_device->parent.create_channel(mm_device, &chn_id, (QUA_VOID_PTR)&vdec_chn_attr, &mm_channel);

    qua_vdec_chn_param_t chn_param;
    qua_mm_vdec_channel_t *vdec_chn = (qua_mm_vdec_channel_t*)mm_channel;
    vdec_chn->get_chn_param(chn_id, &chn_param);

    chn_param.dec_order_output = 0; // display order
    chn_param.chan_err_thr = 1;
    vdec_chn->set_chn_param(chn_id, &chn_param);
    vdec_chn->start_chn(chn_id);

    buf_len = width * height;
    pu8_buf = (QUA_U8 *)malloc(buf_len);
    max_input_cnt = (coding_type == QUA_VIDEO_CodingJpeg) ? 1 : INPUT_FRAME_NUM;
    while(frame_cnt++ < max_input_cnt) {
        fseek(fp_strm, used_bytes, SEEK_SET);
        read_len = fread(pu8_buf, 1, buf_len, fp_strm);
        if (read_len == 0)
            break;

        frame_len = findStreamFrame(pu8_buf, read_len, coding_type, &start);
        memset(&packet, 0, sizeof(qua_vdec_stream_t));
        packet.pts = 0;
        packet.in_data = (QUA_U64)(pu8_buf + start);
        packet.in_size = frame_len;
        packet.end_of_frame = QUA_TRUE;
        packet.end_of_stream = QUA_FALSE;
        printf("sendStream bufLen:%d, s32ReadLen:%d, s32FrameLen:%d, s32Start:%d\n", buf_len, read_len, frame_len, start);
        used_bytes += start + frame_len;
        vdec_chn->send_stream(chn_id, &packet, -1);

        vdec_fd = vdec_chn->get_chn_fd(chn_id);
        FD_ZERO(&vdec_read_fds);
        FD_SET(vdec_fd, &vdec_read_fds);
        timeout_val.tv_sec = 0;
        timeout_val.tv_usec = 20000;

        ret = select(vdec_fd + 1, &vdec_read_fds, NULL, NULL, &timeout_val);
        if (ret <= 0) {
            usleep(100000);
            continue;
        }

        err = vdec_chn->get_frame(chn_id, &out_frame, 0);
        if (err != QUA_SUCCESS) {
            printf("getFrame error %d", __LINE__);
            break;
        }

        QUA_S32 hor_stride;
        QUA_S32 ver_stride;
        QUA_S32 align = (coding_type == QUA_VIDEO_CodingJpeg) ? 16 : 64;
        hor_stride = ALIGNTO(out_frame.video_frame.width, align);
        ver_stride = ALIGNTO(out_frame.video_frame.height, align);
        QUA_VOID *vaddr = sys_ops->sys_mmap(out_frame.video_frame.phy_addr[0], hor_stride * ver_stride * 3/2);
        printf("mmap vaddr %p compress 0x%x wxh %dx%d stride 0 %d u32PhyAddr 0 %lld\n", vaddr, out_frame.video_frame.compress_mode,
                out_frame.video_frame.width, out_frame.video_frame.height, out_frame.video_frame.stride[0],out_frame.video_frame.phy_addr[0]);
        if (fp_frm) {
            fwrite(vaddr, 1, hor_stride * ver_stride * 3 / 2, fp_frm);
            fflush(fp_frm);
        }
        sys_ops->sys_munmap(vaddr, hor_stride * ver_stride * 3 / 2);
        vdec_chn->release_frame(chn_id, &out_frame);
        printf("getFrame %dx%d addr %lld\n", out_frame.video_frame.width, out_frame.video_frame.height, out_frame.video_frame.phy_addr[0]);
    }

    if (coding_type != QUA_VIDEO_CodingJpeg) {
        memset(&packet, 0, sizeof(qua_vdec_stream_t));
        packet.end_of_stream = QUA_TRUE;
        vdec_chn->send_stream(chn_id, &packet, -1);
    }

    vdec_chn->stop_chn(chn_id);
    mm_channel->release(mm_channel);
    vdec_device->parent.close(mm_device);
    if (fp_strm)
        fclose(fp_strm);
    if (fp_frm)
        fclose(fp_frm);
    if (pu8_buf)
        free(pu8_buf);

    printf("==============qua mm vdec test done============\n");
}
