#include "lvgl/lvgl.h"

#ifdef CONFIG_XOS_RES_800x1280
#define __FBT_IMG_SRC_1         raw_800x1280_4
#define __FBT_IMG_SRC_2         raw_800x1280_5
#else
#define __FBT_IMG_SRC_1         raw_240x320_1_32
#define __FBT_IMG_SRC_2         raw_240x320_2_32
#endif

static lv_timer_t * sg_timer = NULL;
static int sg_counter = 0;
static lv_obj_t * sg_image = NULL;
LV_IMG_DECLARE(__FBT_IMG_SRC_1);
LV_IMG_DECLARE(__FBT_IMG_SRC_2);

static void __1st_scr_update_image (lv_timer_t *t) {
    //LV_LOG_USER("update image.");
    sg_counter++;
    if(sg_counter%2)
        lv_img_set_src(sg_image, &__FBT_IMG_SRC_1);
    else
        lv_img_set_src(sg_image, &__FBT_IMG_SRC_2);
}

void __1st_lcm_test(void) {
    lv_obj_t * _image = lv_image_create(lv_scr_act());
	lv_obj_set_size(_image, LV_USE_HOR_SIZE, LV_USE_VER_SIZE);
    lv_img_set_src(_image, &__FBT_IMG_SRC_1);
    lv_obj_align(_image, LV_ALIGN_CENTER, 0, 0);

    lv_timer_t * t = lv_timer_create(__1st_scr_update_image, 3000, NULL);
    lv_timer_set_repeat_count(t, -1);

    sg_timer = t;
    sg_image = _image;
    sg_counter = 0;
}


static lv_timer_t * sg_timer_2nd = NULL;
static int sg_counter_2nd = 0;
static lv_obj_t * sg_image_2nd = NULL;

static void __2nd_scr_update_image (lv_timer_t *t) {
    //LV_LOG_USER("__2nd_scr_update_image.");
    sg_counter_2nd++;
    if(sg_counter_2nd%2)
        lv_img_set_src(sg_image_2nd, &__FBT_IMG_SRC_1);
    else
        lv_img_set_src(sg_image_2nd, &__FBT_IMG_SRC_2);
}

void __2nd_scr_test(void) {
    LV_LOG_USER("1 ");
    extern lv_display_t * lv_display_get_2nd_scr(void);
    lv_display_t * _disp = lv_display_get_2nd_scr();
    LV_LOG_USER("2 %p", _disp);
    LV_LOG_USER("2 %p", _disp->top_layer);
    lv_obj_t * _image = lv_image_create(_disp->top_layer);
    LV_LOG_USER("3.");
    lv_obj_set_size(_image, 800, 1280);
    LV_LOG_USER("4.");
    lv_img_set_src(_image, &__FBT_IMG_SRC_2);
    LV_LOG_USER("5.");
    lv_obj_align(_image, LV_ALIGN_CENTER, 0, 0);
    LV_LOG_USER("6.");

    lv_timer_t * t = lv_timer_create(__2nd_scr_update_image, 33, NULL);
    lv_timer_set_repeat_count(t, -1);

    sg_timer_2nd = t;
    sg_image_2nd = _image;
    sg_counter_2nd = 0;
    LV_LOG_USER("8.");
}

__attribute__((weak))  void app_manager_run(void) { ;/* dummy function link image */ }
__attribute__((weak)) int am_message_queue_init(int buffer_len){ return 0;/* dummy function link image */ }
