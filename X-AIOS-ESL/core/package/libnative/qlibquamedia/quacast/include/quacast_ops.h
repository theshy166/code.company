#ifndef __QUA_CAST_OPS_H__
#define __QUA_CAST_OPS_H__

#include "quacast.h"

typedef struct {
    const char *name;
    int (*init)(void);
    int (*deinit)(void);
    int (*start)(qua_cast_init_params_t params);
    int (*stop)(void);
    int (*publish_frame)(qua_cast_frame_t *frame);
    int (*handle_touch_event)(int x, int y, int is_touch_down);
    int (*visibility)(int visible);
} qua_cast_ops;

#endif /* __QUA_CAST_OPS_H__ */