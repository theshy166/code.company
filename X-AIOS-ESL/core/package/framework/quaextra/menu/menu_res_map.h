#ifndef __MENU_RES_MAP_H__
#define __MENU_RES_MAP_H__

#include "xos_string_ids.h"

typedef struct {
    int textid;
    const char* text;
} text_id_mapping;

typedef struct {
    const void* imgsrc;
    const char* name;
} image_src_mapping;

static const image_src_mapping image_src_default_mapping_table[] = {
    {NULL, NULL}
};

static const text_id_mapping text_id_default_mapping_table[] = {
    {0, NULL}
};
#endif // __MENU_RES_MAP_H__
