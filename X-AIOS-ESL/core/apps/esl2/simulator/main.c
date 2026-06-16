#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "lvgl/lvgl.h"

#define HOR_RES 800
#define VER_RES 480

static uint8_t fb[HOR_RES * VER_RES * 4];

static void mem_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);
    lv_draw_buf_t *draw_buf = lv_display_get_buf_active(disp);
    uint32_t stride = draw_buf->header.stride;
    for (uint32_t y = 0; y < h; y++)
        memcpy(fb + (area->y1 + y) * HOR_RES * 4 + area->x1 * 4, px_map + y * stride, w * 4);
    lv_display_flush_ready(disp);
}

static void save_bmp(const char *path)
{
    FILE *f = fopen(path, "wb");
    if (!f) return;
    uint32_t w = HOR_RES, h = VER_RES, row_size = ((w * 32 + 31) / 32) * 4, img_size = row_size * h;
    uint8_t hdr[54] = {0};
    hdr[0]='B'; hdr[1]='M';
    *(uint32_t*)(hdr+2) = 54 + img_size; *(uint32_t*)(hdr+10) = 54;
    *(uint32_t*)(hdr+14) = 40; *(uint32_t*)(hdr+18) = w; *(uint32_t*)(hdr+22) = h;
    *(uint16_t*)(hdr+26) = 1; *(uint16_t*)(hdr+28) = 32;
    fwrite(hdr, 1, 54, f);
    for (uint32_t y = 0; y < h; y++) {
        uint8_t *row = fb + (h - 1 - y) * w * 4;
        for (uint32_t x = 0; x < w; x++) {
            uint8_t b = row[x*4+0], g = row[x*4+1], r = row[x*4+2];
            uint8_t bgra[4] = {b, g, r, 255};
            fwrite(bgra, 1, 4, f);
        }
    }
    fclose(f);
    fprintf(stderr, "[BMP] saved %s (%ux%u)\n", path, w, h);
}

int main(int argc, char **argv)
{
    (void)argc; (void)argv;
    lv_init();

    lv_display_t *disp = lv_display_create(HOR_RES, VER_RES);
    lv_display_set_flush_cb(disp, mem_flush_cb);
    static uint8_t buf1[HOR_RES * VER_RES * 4];
    lv_display_set_buffers(disp, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_DIRECT);
    lv_display_set_default(disp);
    memset(fb, 0xFF, sizeof(fb));

    lv_obj_t *scr = lv_scr_act();

    /* ?????? */
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xF0F0F0), 0);

    /* ????? */
    lv_obj_t *topbar = lv_obj_create(scr);
    lv_obj_set_size(topbar, HOR_RES, 40);
    lv_obj_set_pos(topbar, 0, 0);
    lv_obj_set_style_bg_color(topbar, lv_color_hex(0x333333), 0);
    lv_obj_set_style_radius(topbar, 0, 0);
    lv_obj_set_style_border_width(topbar, 0, 0);

    lv_obj_t *title = lv_label_create(topbar);
    lv_label_set_text(title, "ESL Price Tag");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_center(title);

    /* ?????? */
    lv_obj_t *card = lv_obj_create(scr);
    lv_obj_set_size(card, 760, 420);
    lv_obj_set_pos(card, 20, 50);
    lv_obj_set_style_bg_color(card, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(card, 12, 0);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, lv_color_hex(0xD0D0D0), 0);
    lv_obj_set_style_pad_all(card, 40, 0);

    /* ??? */
    lv_obj_t *name = lv_label_create(card);
    lv_label_set_text(name, "Coca-Cola");
    lv_obj_set_style_text_color(name, lv_color_hex(0x333333), 0);
    lv_obj_set_pos(name, 40, 40);

    /* ?? */
    lv_obj_t *spec = lv_label_create(card);
    lv_label_set_text(spec, "330ml");
    lv_obj_set_style_text_color(spec, lv_color_hex(0x999999), 0);
    lv_obj_set_pos(spec, 40, 80);

    /* ??? */
    lv_obj_t *line = lv_obj_create(card);
    lv_obj_set_size(line, 680, 1);
    lv_obj_set_pos(line, 40, 130);
    lv_obj_set_style_bg_color(line, lv_color_hex(0xE0E0E0), 0);
    lv_obj_set_style_radius(line, 0, 0);
    lv_obj_set_style_border_width(line, 0, 0);

    /* ???? */
    lv_obj_t *price_label = lv_label_create(card);
    lv_label_set_text(price_label, "Price");
    lv_obj_set_style_text_color(price_label, lv_color_hex(0x999999), 0);
    lv_obj_set_pos(price_label, 40, 160);

    /* ?? - ???? */
    lv_obj_t *price = lv_label_create(card);
    lv_label_set_text(price, "$3.50");
    lv_obj_set_style_text_color(price, lv_color_hex(0xE60012), 0);
    lv_obj_set_pos(price, 40, 200);

    /* ?? */
    lv_obj_t *footer = lv_label_create(card);
    lv_label_set_text(footer, "X-AIOS ESL System");
    lv_obj_set_style_text_color(footer, lv_color_hex(0xCCCCCC), 0);
    lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, -20);

    /* ???? */
    lv_obj_invalidate(scr);
    lv_timer_handler();
    lv_timer_handler();

    save_bmp("/tmp/lvgl_screenshot.bmp");
    return 0;
}
