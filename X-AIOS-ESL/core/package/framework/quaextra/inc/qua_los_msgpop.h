
#ifndef __QUA_XOS_MSGPOP_H__
#define __QUA_XOS_MSGPOP_H__

#include "lvgl/lvgl.h"

#define XOS_USE_MSGPOP_WIDTH   400
#define XOS_USE_MSGPOP_HEIGHT  320

#define XOS_USE_MSGPOP_PANEL_WIDTH   (XOS_USE_MSGPOP_WIDTH - 80)
#define XOS_USE_MSGPOP_PANEL_HEIGHT   (XOS_USE_MSGPOP_HEIGHT - 80)

#define XOS_USE_MSGPOP_BUTTON_WIDTH   40
#define XOS_USE_MSGPOP_BUTTON_HEIGHT   40

/**
 * @brief Display a message popup with configurable buttons.
 *
 * @param title The title of the message popup.
 * @param content The content of the message popup.
 * @param ok_btn_cb Callback function to be called when the OK button is clicked.
 *                  If this parameter is set, an OK button will be displayed.
 * @param ok_btn_data User data to be passed to the `ok_btn_cb` callback function.
 * @param has_close_btn If true, a close button will be displayed in addition to the OK button
 */

void qua_los_msgpop(char *title, char *content, lv_event_cb_t ok_btn_cb, void *ok_btn_data, bool has_close_btn);

/**
 * @brief Close the msgpop by force
 */
void qua_los_msgpop_close(void);

#endif //__QUA_XOS_MSGPOP_H__
