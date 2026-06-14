/**
 * @file lv_evdev.c
 *
 */

/**********************
 *      INCLUDES
 **********************/
#include "lv_evdev.h"
#if LV_USE_EVDEV

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/param.h> /*To detect BSD*/
#ifdef BSD
    #include <dev/evdev/input.h>
#else
    #include <linux/input.h>
#endif /*BSD*/
#include "../../misc/lv_assert.h"
#include "../../misc/lv_math.h"
#include "../../stdlib/lv_mem.h"
#include "../../stdlib/lv_string.h"
#include "../../display/lv_display.h"

#define KEY_IPC_RESET        0x1FF
/*
 * video_call.
 */

#define KEY_PICKUP_PHONE        0x1bd   /* Answer incoming call */
#define KEY_HANGUP_PHONE        0x1be   /* Decline incoming call */

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    /*Device*/
    int fd;
    /*Config*/
    bool swap_axes;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    /*State*/
    int root_x;
    int root_y;
    int key;
    lv_indev_state_t state;
} lv_evdev_t;

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int _evdev_process_key(uint16_t code)
{
    switch(code) {
        case KEY_UP:
            return LV_KEY_UP;
        case KEY_DOWN:
            return LV_KEY_DOWN;
        case KEY_RIGHT:
            return LV_KEY_RIGHT;
        case KEY_LEFT:
            return LV_KEY_LEFT;
        case KEY_ESC:
        case KEY_BACK:
            return LV_KEY_ESC;
        case KEY_DELETE:
            return LV_KEY_DEL;
        case KEY_BACKSPACE:
            return LV_KEY_BACKSPACE;
        case KEY_ENTER:
        case KEY_OK:
            return LV_KEY_ENTER;
        case KEY_NEXT:
        case KEY_TAB:
            return LV_KEY_NEXT;
        case KEY_PREVIOUS:
            return LV_KEY_PREV;
        case KEY_HOME:
            return LV_KEY_HOME;
        case KEY_END:
            return LV_KEY_END;
        case KEY_F1:
        case KEY_SETUP:
            return LV_KEY_F1;
        case KEY_F2:
        case KEY_CAMERA:
            return LV_KEY_F2;
        case KEY_F3:
        case KEY_POWER:
            return LV_KEY_F3;
        case KEY_F4:
            return LV_KEY_F4;
        case KEY_F5:
            return LV_KEY_F5;
        case KEY_F6:
            return LV_KEY_F6;
        case KEY_F7:
            return LV_KEY_F7;
        case KEY_F8:
            return LV_KEY_F8;
        case KEY_F9:
            return LV_KEY_F9;
        case KEY_F10:
            return LV_KEY_F10;
        case KEY_F11:
            return LV_KEY_F11;
        case KEY_F12:
            return LV_KEY_F12;
        case KEY_IPC_RESET:
            return LV_KEY_RESET;
        case KEY_PICKUP_PHONE:
            return LV_KEY_PHONE;
        case KEY_HANGUP_PHONE:  
            return LV_KEY_HANGUP;
        case KEY_VOLUMEUP:
            return LV_KEY_VOLUME_UP;
        case KEY_VOLUMEDOWN:
            return LV_KEY_VOLUME_DOWN;
        case KEY_PAGEUP:
            return LV_KEY_PAGE_UP;
        case KEY_PAGEDOWN:
            return LV_KEY_PAGE_DOWN;
        default:
            return 0;
    }
}

static int _evdev_calibrate(int v, int in_min, int in_max, int out_min, int out_max)
{
    if(in_min != in_max) v = (v - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    return LV_CLAMP(out_min, v, out_max);
}

static lv_point_t _evdev_process_pointer(lv_indev_t * indev, int x, int y)
{
    lv_display_t * disp = lv_indev_get_display(indev);
    lv_evdev_t * dsc = lv_indev_get_driver_data(indev);
    lv_display_rotation_t rotation = lv_display_get_rotation(disp);
    LV_ASSERT_NULL(dsc);

    int swapped_x = (rotation == LV_DISPLAY_ROTATION_90 || rotation == LV_DISPLAY_ROTATION_270) ? y : x;
    int swapped_y = (rotation == LV_DISPLAY_ROTATION_90 || rotation == LV_DISPLAY_ROTATION_270) ? x : y;

    int offset_x = lv_display_get_offset_x(disp);
    int offset_y = lv_display_get_offset_y(disp);
    int width = lv_display_get_horizontal_resolution(disp);
    int height = lv_display_get_vertical_resolution(disp);

    lv_point_t p;
    p.x = _evdev_calibrate(swapped_x, dsc->min_x, dsc->max_x, offset_x, offset_x + width - 1);
    p.y = _evdev_calibrate(swapped_y, dsc->min_y, dsc->max_y, offset_y, offset_y + height - 1);

    int unswapped_x = (rotation == LV_DISPLAY_ROTATION_90 || rotation == LV_DISPLAY_ROTATION_270) ? p.y : p.x;
    int unswapped_y = (rotation == LV_DISPLAY_ROTATION_90 || rotation == LV_DISPLAY_ROTATION_270) ? p.x : p.y;

    p.x = dsc->swap_axes ? unswapped_y : unswapped_x;
    p.y = dsc->swap_axes ? unswapped_x : unswapped_y;
    dsc->root_x = p.x;
    dsc->root_y = p.y;
    return p;
}

static void _evdev_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    lv_evdev_t * dsc = lv_indev_get_driver_data(indev);
    LV_ASSERT_NULL(dsc);

    /*Update dsc with buffered events*/
    struct input_event in = { {0}, 0 };
    while(read(dsc->fd, &in, sizeof(in)) > 0) {
        //LV_LOG_USER("read event type=%d code=%d value=%d", in.type, in.code, in.value);
        if(in.type == EV_REL) {
            if(in.code == REL_X) dsc->root_x += in.value;
            else if(in.code == REL_Y) dsc->root_y += in.value;
            //LV_LOG_USER("change to root_x=%d root_y=%d", dsc->root_x, dsc->root_y);
        }
        else if(in.type == EV_ABS) {
            if(in.code == ABS_X || in.code == ABS_MT_POSITION_X) dsc->root_x = in.value;
            else if(in.code == ABS_Y || in.code == ABS_MT_POSITION_Y) dsc->root_y = in.value;
            else if(in.code == ABS_MT_TRACKING_ID) {
                if(in.value == -1) dsc->state = LV_INDEV_STATE_RELEASED;
                else if(in.value >= 0) dsc->state = LV_INDEV_STATE_PRESSED;
            }
        }
        else if(in.type == EV_KEY) {
            if(in.code == BTN_MOUSE || in.code == BTN_TOUCH) {
                if(in.value == 0) dsc->state = LV_INDEV_STATE_RELEASED;
                else if(in.value == 1) dsc->state = LV_INDEV_STATE_PRESSED;
            }
            else {
                dsc->key = _evdev_process_key(in.code);
                if(dsc->key) {
                    dsc->state = in.value ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
                    data->continue_reading = true; /*Keep following events in buffer for now*/
                    LV_LOG_USER("change to state=%d key=%d", dsc->state, dsc->key);
                    break;
                }
            }
        }
    }

    /*Process and store in data*/
    switch(lv_indev_get_type(indev)) {
        case LV_INDEV_TYPE_KEYPAD:
            data->state = dsc->state;
            data->key = dsc->key;
            break;
        case LV_INDEV_TYPE_POINTER:
            data->state = dsc->state;
            data->point = _evdev_process_pointer(indev, dsc->root_x, dsc->root_y);
            break;
        default:
            break;
    }
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_indev_t * lv_evdev_create(lv_indev_type_t indev_type, const char * dev_path)
{
    lv_evdev_t * dsc = lv_malloc_zeroed(sizeof(lv_evdev_t));
    LV_ASSERT_MALLOC(dsc);
    if(dsc == NULL) return NULL;

    dsc->fd = open(dev_path, O_RDONLY | O_NOCTTY | O_CLOEXEC);
    if(dsc->fd < 0) {
        LV_LOG_ERROR("open failed: %s", strerror(errno));
        goto err_after_malloc;
    }

    if(fcntl(dsc->fd, F_SETFL, O_NONBLOCK) < 0) {
        LV_LOG_ERROR("fcntl failed: %s", strerror(errno));
        goto err_after_open;
    }

    lv_indev_t * indev = lv_indev_create();
    if(indev == NULL) goto err_after_open;
    lv_indev_set_type(indev, indev_type);
    lv_indev_set_read_cb(indev, _evdev_read);
    lv_indev_set_driver_data(indev, dsc);
    return indev;

err_after_open:
    close(dsc->fd);
err_after_malloc:
    lv_free(dsc);
    return NULL;
}

void lv_evdev_set_swap_axes(lv_indev_t * indev, bool swap_axes)
{
    lv_evdev_t * dsc = lv_indev_get_driver_data(indev);
    LV_ASSERT_NULL(dsc);
    dsc->swap_axes = swap_axes;
}

void lv_evdev_set_calibration(lv_indev_t * indev, int min_x, int min_y, int max_x, int max_y)
{
    lv_evdev_t * dsc = lv_indev_get_driver_data(indev);
    LV_ASSERT_NULL(dsc);
    dsc->min_x = min_x;
    dsc->min_y = min_y;
    dsc->max_x = max_x;
    dsc->max_y = max_y;
}

void lv_evdev_delete(lv_indev_t * indev)
{
    lv_evdev_t * dsc = lv_indev_get_driver_data(indev);
    LV_ASSERT_NULL(dsc);
    close(dsc->fd);
    lv_free(dsc);

    lv_indev_delete(indev);
}

#endif /*LV_USE_EVDEV*/
