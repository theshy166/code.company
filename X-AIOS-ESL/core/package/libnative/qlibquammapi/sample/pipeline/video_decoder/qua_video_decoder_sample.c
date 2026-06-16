#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "pipeline/video_decoder/qua_vdec_pipeline.h"
#include "common/qua_sys_platform.h"
#include "common/qua_mm_common.h"
#include "system/qua_mm_system.h"

static void print_usage(char* prog) {
    printf("Usage: %s -p --primary-user -c --chip-name -o --os-name -i stream_file -w img-width -h img-height -f coding_format\n", prog);
    printf("\t -p --primary-user   : whether this sample is primary user(1: primary user; 0: non-primary user)\n");
    printf("\t -c --chip-name      : chip name, such as mc6870 or mc331x\n");
    printf("\t -o --os-name        : os name, such as android or rtt\n");
    printf("\t -i --stream_file    : the full path of the stream file\n");
    printf("\t -w --img-width      : the width of image\n");
    printf("\t -h --img-height     : the height of image\n");
    printf("\t -f --coding_format  : coding type. such as h264\n");
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

static int32_t qua_h2645_split_codec_nal(QUA_U8 *data, QUA_U32 size, QUA_U32 *nalSize) {
    if (data == NULL || size < 4) {
        return -1;
    }

    QUA_U8 nal_length = 4;
    if (memcmp(data, "\x00\x00\x00\x01", 4)) {
        if (memcmp(data, "\x00\x00\x01", 3)) {
            return -1;
        }
        nal_length = 3;
    }

    if (size < 10) {
        *nalSize = size;
        return 0;
    }

    const QUA_U8 *nalu_data = data + nal_length;
    QUA_U32 nalu_size = size - nal_length;
    QUA_U8 *nextStart = NULL;

    nextStart = (QUA_U8 *)memmem(nalu_data, nalu_size - 1, "\x00\x00\x00\x01", 4);
    if (nextStart != NULL && nextStart < nalu_data + nalu_size - 4) {
        *nalSize = nextStart - nalu_data + 4;
    } else {
        *nalSize = size;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    QUA_S32 ret;
    QUA_CHAR tmp_buf[16];
    FILE *fp_strm = NULL;
    FILE *fp_frm = NULL;
    QUA_CHAR platform[32];

    QUA_BOOL primary;
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_PRIMARY_USER, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get primary value error!\n");
        print_usage(argv[0]);
        return ret;
    }
    primary = atoi(tmp_buf);

    QUA_CHAR chip_name[16];
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_CHIP_NAME, chip_name, sizeof(chip_name));
    if (ret == QUA_FAILURE) {
        printf("get chip name error!\n");
        print_usage(argv[0]);
        return ret;
    }

    QUA_CHAR os_name[16];
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_OS_NAME, os_name, sizeof(os_name));
    if (ret == QUA_FAILURE) {
        printf("get os name error!\n");
        print_usage(argv[0]);
        return ret;
    }

    QUA_S32 width;
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_IMAGE_WIDTH, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get image width error!\n");
        print_usage(argv[0]);
        return ret;
    }
    width = atoi(tmp_buf);

    QUA_S32 height;
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_IMAGE_HEIGHT, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get image height error!\n");
        print_usage(argv[0]);
        return ret;
    }
    height = atoi(tmp_buf);

    QUA_CHAR stream_path[256];
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_INPUT_FILE, stream_path, sizeof(stream_path));
    if (ret == QUA_FAILURE) {
        printf("get input file error!\n");
        print_usage(argv[0]);
        return ret;
    }

    QUA_CHAR frame_path[256];
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_OUTPUT_FILE, frame_path, sizeof(frame_path));
    if (ret == QUA_FAILURE) {
        printf("get ouput file error!\n");
    }

    qua_coding_type_t coding_type;
    ret = qua_get_cmdline_arg(argc, argv, QUA_ARG_CODING_FORMAT, tmp_buf, sizeof(tmp_buf));
    if (ret == QUA_FAILURE) {
        printf("get coding format error!\n");
        print_usage(argv[0]);
        return ret;
    }
    coding_type = get_coding_type(tmp_buf);

    fp_strm = fopen(stream_path, "rb");
    if (fp_strm == NULL) {
        printf("can't open stream %s (%s)\n", stream_path, strerror(errno));
        return -1;
    }

    fp_frm = fopen(frame_path, "wb");
    if (fp_frm == NULL) {
        printf("can't open frm %s (%s)\n", frame_path, strerror(errno));
    }

    qua_mm_system_t *system;
    qua_mm_system_ops_t *ops;
    QUA_U32 blk_size;
    qua_vb_config_t vb_cfg;
    sprintf(platform, "%s_%s", chip_name, os_name);
    printf("platform %s\n", platform);
    qua_mm_init(primary, platform, &system);
    ops = (qua_mm_system_ops_t *)system;

    memset(&vb_cfg, 0, sizeof(qua_vb_config_t));
    blk_size = 1024*600*2;
    vb_cfg.max_pool_cnt = QUA_VB_MAX_POOLS;
    vb_cfg.common_pools[0].block_size = blk_size;
    vb_cfg.common_pools[0].block_cnt = 4;

    if (primary) {
        ops->sys_init(&vb_cfg);
    }

    QUA_U32 output_width = 3840;
    QUA_U32 output_height = 2160;
    qua_pixel_fmt_t output_format = QUA_PIXEL_FMT_RGB_8888;
    qua_vdec_pipeline_attr_t attr;
    attr.stream_width = width;
    attr.stream_height = height;
    attr.coding_type = coding_type;
    attr.output_width = output_width;
    attr.output_height = output_height;
    attr.output_format = output_format;
    QUA_U32 buf_len = width * height;
    QUA_U8 *pu8_buf = malloc(buf_len);

    qua_vdec_pipeline_ops_t pipeline_ops;
    ret = qua_vdec_pipeline_init(&pipeline_ops);
    if (ret != QUA_SUCCESS)
        return -1;
    qua_vdec_pipeline_t pipeline = NULL;
    pipeline = pipeline_ops.pipeline_create(&pipeline, &attr);
    if (pipeline == NULL)
        return -1;

    ret = pipeline_ops.pipeline_start(&pipeline);
    if (ret != QUA_SUCCESS)
        goto start_err;

    QUA_U32 used_bytes = 0;
    QUA_U32 read_len = 0;
    QUA_U64 pts = 0;
    QUA_BOOL is_jpeg = (coding_type == QUA_VIDEO_CodingJpeg);
    if (is_jpeg) {
        fseek(fp_strm, 0, SEEK_END);
        buf_len = ftell(fp_strm);
    }
    while(1) {
        fseek(fp_strm, used_bytes, SEEK_SET);
        read_len = fread(pu8_buf, 1, buf_len, fp_strm);
        if (read_len == 0)
            break;
        QUA_U32 nalu_size;
        if (is_jpeg)
            nalu_size = buf_len;
        else
            qua_h2645_split_codec_nal(pu8_buf, buf_len, &nalu_size);
        qua_vdec_stream_t packet;
        packet.pts = pts;
        packet.in_data = (QUA_U64)(pu8_buf);
        packet.in_size = nalu_size;
        packet.end_of_frame = QUA_TRUE;
        packet.end_of_stream = QUA_FALSE;
        ret = pipeline_ops.pipeline_send_stream(&pipeline, &packet, -1);
        if (ret != QUA_SUCCESS)
            goto send_err;
        used_bytes += nalu_size;
        pts += 16666;

        qua_vdec_pipeline_frame_info_t frame;
        ret = pipeline_ops.pipeline_get_frame(&pipeline, &frame);
        if (!ret) {
            if (fp_frm) {
                fwrite(frame.frame.video_frame.vir_addr[0], 1, output_width * output_height * 4, fp_frm);
                fflush(fp_frm);
            }
            pipeline_ops.pipeline_release_frame(&pipeline, &frame);
        }
    }
send_err:
    pipeline_ops.pipeline_stop(&pipeline);
start_err:
    pipeline_ops.pipeline_destroy(pipeline);
    if (pu8_buf)
        free(pu8_buf);
    if (fp_strm)
        fclose(fp_strm);
    if (fp_frm)
        fclose(fp_frm);
    return 0;
}
