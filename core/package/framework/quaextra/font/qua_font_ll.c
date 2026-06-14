#include <stdio.h>
#include <string.h>
#include "qua_font_ll.h"
#include <stdio.h>
#include <ctype.h>

typedef struct qua_font_node {
    const char* font_path;
    short font_size;
    lv_font_t *font;
}qua_font_node_t;

static lv_ll_t  __font_ll = {0};
static int __font_ll_init = 0;
static int __font_counter = 0;

static int __strcasecmp(const char *s1, const char *s2)
{
    int i = 0;

    while (1) {
        int c1 = tolower((unsigned char)s1[i]);
        int c2 = tolower((unsigned char)s2[i]);

        if (c1 == '\0' && c2 == '\0') {
            return 0;
        } else if (c1 < c2) {
            return -1;
        } else if (c1 > c2) {
            return 1;
        }

        i++;
    }
}


void __init_font_list(void) {
    if (__font_ll_init) return;
    _lv_ll_init(&__font_ll, sizeof(qua_font_node_t));
    __font_ll_init = true;
    __font_counter = 0;
}

void __add_font_list(lv_font_t *font, const char* font_path, short font_size) {
    if (!__font_ll_init) __init_font_list();
    qua_font_node_t *node = _lv_ll_ins_tail(&__font_ll);
    if (node) {
        node->font = font;
        node->font_size = font_size;
        node->font_path = font_path;
        __font_counter++;
    }
}

void __clear_font_list(void) {
    _lv_ll_clear(&__font_ll);
    __font_ll_init = 0;
    __font_counter = 0;
}

lv_font_t* __get_font_by_size(const char* font_path, short font_size) {
    qua_font_node_t *node;
    lv_ll_t *ll_p = &__font_ll;

    _LV_LL_READ(ll_p, node) {
        if((node->font_size == font_size) && (node->font_path) && 
           (0 == __strcasecmp(node->font_path, font_path)))
            return node->font;
    }
    return NULL;
}

void __prt_font_by_size(void) {
    qua_font_node_t *node;
    lv_ll_t *ll_p = &__font_ll;

    int i=0;
    _LV_LL_READ(ll_p, node) {
        printf("%s NO.%d %d %s\n", __func__, i, node->font_size, node->font_path);
        i++;
    }
}
