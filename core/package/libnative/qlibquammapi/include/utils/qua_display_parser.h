/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_DISPLAY_PARSER_H__
#define __QUA_DISPLAY_PARSER_H__

#include "display/qua_mm_display.h"

#ifdef __cplusplus
extern "C" {
#endif

QUA_S32 qua_init_display_parser();
QUA_S32 qua_display_parser_getint(QUA_CONST_CHAR *key);
QUA_CONST_CHAR * qua_display_parser_getstring(QUA_CONST_CHAR *key);
QUA_S32 qua_parse_display_id(QUA_CONST_CHAR *display_id, QUA_CONST_CHAR **vo_id, QUA_S32 *vo_num, QUA_CONST_CHAR **fb_id, QUA_S32 *fb_num);
QUA_S32 qua_parse_display_resolution(qua_vo_intf_sync_t intf_sync, qua_size_t *size);

#ifdef __cplusplus
}
#endif

#endif /* __QUA_DISPLAY_PARSER_H__ */
