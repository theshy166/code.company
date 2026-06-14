#include "lvgl.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/input.h>

#include "evdev.h"
#include "xos_ability.h"
#include "qm_system.h"

extern void xos_ability_key_power_ctrl(void);

#define MAX_LINE_LEN  256
#define EVDEV_FILE_NAME          "/proc/bus/input/devices"
#define EVDEV_EXISTED_FLAG       (1<<28)

#define KEYPD_HANDLERS_PREFIX    "H: Handlers=kbd event"
#define KEYPD_0_NAME_PREFIX      "N: Name=\"gpio_keys\""

#define MOUSE_WIRED_HANDLERS_PREFIX     "H: Handlers=mouse1 event"
#define MOUSE_WIRELESS_HANDLERS_PREFIX  "H: Handlers=kbd mouse1 event"
#define MOUSE_WIRED_UNIVERSAL_NAME      "USB Optical Mouse"
#define MOUSE_WIRELESS_UNIVERSAL_NAME   "Universal Receiver"


#define TOUCH_HANDLERS_PREFIX    "H: Handlers=event"
#define TOUCH_1_NAME_PREFIX      "N: Name=\"goodix-ts\""


typedef struct {
    int touch_event_id;
    int keypd_event_id;
    int mouse_event_id;
} evdev_event_id_t;

static evdev_event_id_t s_evdev_event = {0};

static void xos_evdev_set_cursor(lv_indev_t * indev_mouse) {
    /*Set a cursor for the mouse*/
    LV_IMG_DECLARE(mouse_cursor_icon);
    /*Create an image object for the cursor */
    lv_obj_t * cursor = lv_img_create(lv_layer_top());
    lv_img_set_src(cursor, &mouse_cursor_icon);
    lv_obj_remove_flag(cursor, LV_OBJ_FLAG_CLICKABLE);
    /*Connect the image object to the driver*/
    lv_indev_set_cursor(indev_mouse, cursor);
}

void xos_evdev_indev_init(void) {

    xos_evdev_mouse_find();
    xos_evdev_mouse_init();

    xos_evdev_keypad_find();
    xos_evdev_keypad_init();

    xos_evdev_touch_find();
    xos_evdev_touch_init();
}

void xos_evdev_touch_find(void) {
    const char * evdev_name = "/dev/input/event0";

    if (0 != access(evdev_name, F_OK)) {
        printf("access %s failed!\n", evdev_name);
        return;
    }

    s_evdev_event.touch_event_id |= (1<<28);
}

void xos_evdev_mouse_find(void) {
    char buffer[MAX_LINE_LEN] = {0};
    char *result = NULL;
    FILE *fp = NULL;
    int _name_matched = 0;

    fp = fopen(EVDEV_FILE_NAME, "r");
    if (fp == NULL) {
        printf("open %s failed!\n", EVDEV_FILE_NAME);
        return;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        //printf("read %s\n", buffer);
        if (strstr(buffer, MOUSE_WIRED_UNIVERSAL_NAME) != NULL) {
            _name_matched = 1;
            printf("%s %s 1\n", __func__, buffer);
        }

        if (strstr(buffer, MOUSE_WIRELESS_UNIVERSAL_NAME) != NULL) {
            _name_matched = 1;
            printf("%s %s 2\n", __func__, buffer);
        }

        if (_name_matched && strncmp(MOUSE_WIRED_HANDLERS_PREFIX, buffer,
            strlen(MOUSE_WIRED_HANDLERS_PREFIX)) == 0) {
            result = strstr(buffer, "event");
            s_evdev_event.mouse_event_id = result[5] - '0';
            s_evdev_event.mouse_event_id |= EVDEV_EXISTED_FLAG;
            printf("%s %s 3\n", __func__, buffer);
            break;
        }

        if (_name_matched && strncmp(MOUSE_WIRELESS_HANDLERS_PREFIX, buffer,
            strlen(MOUSE_WIRELESS_HANDLERS_PREFIX)) == 0) {
            result = strstr(buffer, "event");
            s_evdev_event.mouse_event_id = result[5] - '0';
            s_evdev_event.mouse_event_id |= EVDEV_EXISTED_FLAG;
            printf("%s %s 4\n", __func__, buffer);
            break;
        }
    }

    fclose(fp);
}

void xos_evdev_keypad_find(void) {
    char buffer[MAX_LINE_LEN] = {0};
    char *result = NULL;
    FILE *fp = NULL;
    int _name_matched = 0;

    fp = fopen(EVDEV_FILE_NAME, "r");
    if (fp == NULL) {
        printf("open %s failed!\n", EVDEV_FILE_NAME);
        return;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        //printf("read %s\n", buffer);
        if (strncmp(KEYPD_0_NAME_PREFIX, buffer, strlen(KEYPD_0_NAME_PREFIX)) == 0) {
            _name_matched = 1;
        }

        if (_name_matched && strncmp(KEYPD_HANDLERS_PREFIX, buffer,
            strlen(KEYPD_HANDLERS_PREFIX)) == 0) {
            result = strstr(buffer, "event");
            s_evdev_event.keypd_event_id = result[5] - '0';
            s_evdev_event.keypd_event_id |= EVDEV_EXISTED_FLAG;
            break;
        }
    }

    fclose(fp);
}

void xos_evdev_keypad_init(void) {
    lv_indev_t *indev_kbd = NULL;
    char buffer[20] = {0};

    if((s_evdev_event.keypd_event_id & EVDEV_EXISTED_FLAG) == 0) {
        return;
    }

    snprintf(buffer, sizeof(buffer), "/dev/input/event%d", s_evdev_event.keypd_event_id & 0xf);
    indev_kbd = lv_evdev_create(LV_INDEV_TYPE_KEYPAD, (const char *)buffer);
    if(indev_kbd != NULL) {
        printf("%s lv_evdev_create.\n", buffer);
        lv_indev_set_display(indev_kbd, lv_display_get_default());
        lv_indev_set_group(indev_kbd, lv_group_get_default());
    }
}

void xos_evdev_mouse_init(void) {
    lv_indev_t *indev_mouse = NULL;
    char buffer[20] = {0};
    printf("%s event_id.mouse_event_id:0x%x\n", __func__, s_evdev_event.mouse_event_id);
    if((s_evdev_event.mouse_event_id & EVDEV_EXISTED_FLAG) == 0) {
        return;
    }

    snprintf(buffer, sizeof(buffer), "/dev/input/event%d", s_evdev_event.mouse_event_id & 0xf);
    indev_mouse = lv_evdev_create(LV_INDEV_TYPE_POINTER, (const char *)buffer);
    if(indev_mouse != NULL) {
        printf("%s lv_evdev_create.\n", buffer);
        lv_indev_set_display(indev_mouse, lv_display_get_default());
        lv_indev_set_group(indev_mouse, lv_group_get_default());

        // lv_evdev_set_swap_axes(lv_indev_t * indev, bool swap_axes);
        // lv_evdev_set_calibration(indev_mouse, EVDEV_HOR_MIN, EVDEV_VER_MIN, EVDEV_HOR_MAX, EVDEV_VER_MAX);

        xos_evdev_set_cursor(indev_mouse);
    }
}

void xos_evdev_touch_init(void) {
    char buffer[20] = {0};
    printf("%s event_id.touch_event_id:0x%x\n", __func__, s_evdev_event.touch_event_id);
    if((s_evdev_event.touch_event_id & EVDEV_EXISTED_FLAG) == 0) {
        return;
    }

    snprintf(buffer, sizeof(buffer), "/dev/input/event%d", s_evdev_event.touch_event_id & 0xf);
    lv_indev_t *indev_pointer = lv_evdev_create(LV_INDEV_TYPE_POINTER, (const char *)buffer);
    if(indev_pointer != NULL) {
        printf("%s lv_evdev_create.\n", buffer);
        lv_indev_set_display(indev_pointer, lv_display_get_default());
        lv_indev_set_group(indev_pointer, lv_group_get_default());
    }
}

__attribute__((used)) void indev_hook_handler(lv_indev_t * indev, lv_indev_data_t * data) {
    // printf("%s indev->type:%d data->key:%d data->state:%d indev->long_pr_sent:%d\n", __func__, indev->type, data->key, data->state, indev->long_pr_sent);
    if (indev->type == LV_INDEV_TYPE_KEYPAD && data->key == LV_KEY_F3 &&
        indev->long_pr_sent == 0 && data->state == LV_INDEV_STATE_RELEASED) {
            // qm_system_suspend(SUSPEND_MEM);
    }

    #ifdef CONFIG_PRODUCT_XKPHOTO
    if (indev->type == LV_INDEV_TYPE_KEYPAD && data->key == LV_KEY_F3 &&
        indev->long_pr_sent == 1 && data->state == LV_INDEV_STATE_PRESSED) {
            extern void power_off_page_create(void);
            power_off_page_create();
    }
    #endif
}
