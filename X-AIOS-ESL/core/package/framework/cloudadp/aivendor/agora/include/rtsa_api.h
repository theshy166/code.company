
#ifndef _RTSA_API_H
#define _RTSA_API_H


/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "xos_app_conf.h"



#ifdef CONFIG_XOS_FWK_CADP_AGORA
//


//function api
void reset_app_t_structure(void);
int rtsa_api_go(int argc, char **argv, void * callback);
//sync function
int rtsa_api_stop(void);

#ifdef CONFIG_XOS_USE_APP_MOMO_TALK
#include "qua_camera.h"
int rtsa_send_audio(int channel, qua_steram_type_t stream_type, qua_camera_stream_info_t *stream_info);
#endif //CONFIG_XOS_USE_APP_MOMO_TALK

#endif //CONFIG_XOS_FWK_CADP_AGORA

#endif
