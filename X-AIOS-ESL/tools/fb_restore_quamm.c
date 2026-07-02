#include <stdio.h>
#include <string.h>

#include "common/qua_sys_platform.h"
#include "display/qua_mm_display.h"
#include "system/qua_mm_system.h"
#include "utils/qua_display_parser.h"

static const char *s_display_ids[] = {
    "id:display0",
    "id:display1",
};

static int restore_fb(const qua_mm_module_t *display_module, const char *display_id)
{
    QUA_CONST_CHAR *vo_id = NULL;
    QUA_CONST_CHAR *fb_id = NULL;
    QUA_S32 vo_num = 0;
    QUA_S32 fb_num = 0;
    QUA_S32 ret = qua_parse_display_id(display_id, &vo_id, &vo_num, &fb_id, &fb_num);
    if (ret != QUA_SUCCESS) {
        fprintf(stderr, "%s parse display failed: %d\n", display_id, ret);
        return -1;
    }

    qua_mm_device_t *mm_device = NULL;
    ret = display_module->open_device(display_module, fb_id, fb_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        fprintf(stderr, "%s open fb failed: %d\n", display_id, ret);
        return -1;
    }

    qua_mm_fb_device_t *fb_dev = (qua_mm_fb_device_t *)mm_device;
    qua_fb_alpha_t alpha;
    memset(&alpha, 0, sizeof(alpha));
    alpha.alpha_enable = QUA_TRUE;
    alpha.alpha_channel = QUA_FALSE;
    alpha.alpha0 = 0;
    alpha.alpha1 = 255;
    alpha.global_alpha = 255;
    fb_dev->put_alpha(fb_dev, &alpha);
    ret = fb_dev->show(fb_dev, QUA_TRUE);
    fb_dev->parent.close((qua_mm_device_t *)fb_dev);
    (void)vo_id;
    (void)vo_num;
    fprintf(stderr, "%s fb show ret=%d\n", display_id, ret);
    return ret == QUA_SUCCESS ? 0 : -1;
}

int main(void)
{
    char platform[32] = {0};
    qua_mm_system_t *system = NULL;
    if (qua_make_platform("qm10xd", "linux", platform, sizeof(platform)) != QUA_SUCCESS) {
        fprintf(stderr, "make platform failed\n");
        return 1;
    }
    if (qua_mm_init(QUA_TRUE, platform, &system) != QUA_SUCCESS || system == NULL) {
        fprintf(stderr, "qua_mm_init failed\n");
        return 1;
    }

    qua_init_display_parser();
    const qua_mm_module_t *display_module = NULL;
    if (qua_mm_load_module(QUA_MM_MODULE_DISPLAY, &display_module) != QUA_SUCCESS ||
        display_module == NULL) {
        fprintf(stderr, "load display module failed\n");
        return 1;
    }

    int failed = 0;
    for (size_t i = 0; i < sizeof(s_display_ids) / sizeof(s_display_ids[0]); i++) {
        if (restore_fb(display_module, s_display_ids[i]) != 0) {
            failed = 1;
        }
    }
    return failed ? 1 : 0;
}
