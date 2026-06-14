#ifndef __QUA_CAST_TIMER_H__
#define __QUA_CAST_TIMER_H__
#include <signal.h>
typedef void (*timer_handler_t)(union sigval);

timer_t quacast_create_timer(timer_handler_t handler, int sec, int msec);
void quacast_delete_timer(timer_t timer);
#endif /* __QUA_CAST_TIMER_H__ */