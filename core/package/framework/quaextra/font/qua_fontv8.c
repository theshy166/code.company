
#include "qua_font.h"
#include "lvgl/lvgl.h"

#if !defined(CONFIG_LVGL_V9) || CONFIG_LVGL_V9 == 0

lv_font_t * ft_font_ttf_20;
lv_font_t * ft_font_ttf_30;
lv_font_t * ft_font_ttf_12;
lv_font_t * ft_font_ttf_14;
lv_font_t * ft_font_ttf_40;

bool g_font_inited = false;

#if LV_USE_FREETYPE != 0

#include "lang/xos_strid.h"

static int __ft_font_init(lv_font_t ** font, const char * font_path, uint16_t font_size, uint16_t font_style)
{
    LV_LOG_INFO("e.");
    /*Create a font*/
    lv_ft_info_t info1={0};

#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR
    info1.name = "./out/simulator/xos/res/ttf/2312.ttf";
    info1.weight = font_size? font_size : 10;
    info1.style = font_style? font_style : FT_FONT_STYLE_NORMAL;
    info1.mem = NULL;
    info1.font = NULL;
    if(lv_ft_font_init(&info1)) {
        goto __exit;
    }
    info1.name = "../res/ttf/2312.ttf";
    info1.weight = font_size? font_size : 10;
    info1.style = font_style? font_style : FT_FONT_STYLE_NORMAL;
    info1.mem = NULL;
    info1.font = NULL;
    if(!lv_ft_font_init(&info1)) {
        LV_LOG_ERROR("lv_ft_font_init failed.");
        return -1;
    }
#else
    /*FreeType uses C standard file system, so no driver letter is required.*/
    info1.name = font_path;
    info1.weight = font_size? font_size : 10;
    info1.style = font_style? font_style : FT_FONT_STYLE_NORMAL;
    info1.mem = NULL;
    info1.font = NULL;
    if(!lv_ft_font_init(&info1)) {
        LV_LOG_ERROR("lv_ft_font_init failed.");
        return -1;
    }
#endif

__exit:
    *font = (lv_font_t *)info1.font;
    LV_LOG_INFO("x.");
    return 0;
}

void qua_los_ft_font_init(void)
{
    if (!g_font_inited) {
        g_font_inited = true;
    }

    int rtn = 0;
    rtn = __ft_font_init(&ft_font_ttf_12, "/res/2312.ttf", 12, FT_FONT_STYLE_NORMAL);
    if(rtn) LV_LOG_ERROR("los_ft_font_init ft_font_ttf_12 failed.");
    rtn = __ft_font_init(&ft_font_ttf_14, "/res/2312.ttf", 14, FT_FONT_STYLE_NORMAL);
    if(rtn) LV_LOG_ERROR("los_ft_font_init ft_font_ttf_14 failed.");
    rtn = __ft_font_init(&ft_font_ttf_20, "/res/2312.ttf", 20, FT_FONT_STYLE_NORMAL);
    if(rtn) LV_LOG_ERROR("los_ft_font_init ft_font_ttf_20 failed.");
    rtn = __ft_font_init(&ft_font_ttf_30, "/res/2312.ttf", 30, FT_FONT_STYLE_NORMAL);
    if(rtn) LV_LOG_ERROR("los_ft_font_init ft_font_ttf_30 failed.");
    rtn = __ft_font_init(&ft_font_ttf_40, "/res/2312.ttf", 40, FT_FONT_STYLE_NORMAL);
    if(rtn) LV_LOG_ERROR("los_ft_font_init ft_font_ttf_40 failed.");
}

void qua_los_ft_init_with_size(lv_font_t **ppFont, short font_size, short style)
{
    int rtn = 0;
    rtn = __ft_font_init(ppFont, "/res/2312.ttf", font_size, style);
    if(rtn) LV_LOG_ERROR("los_ft_font_init ft_font_ttf_12 failed.");
}

#else

void qua_los_ft_font_init(void)
{
    if (!g_font_inited) {
        g_font_inited = true;
        ft_font_ttf_20 = (lv_font_t * )&lv_font_montserrat_20;
        ft_font_ttf_30 = (lv_font_t * )&lv_font_montserrat_28;
        ft_font_ttf_12 = (lv_font_t * )&lv_font_montserrat_12;
        ft_font_ttf_14 = (lv_font_t * )&lv_font_montserrat_14;
        ft_font_ttf_40 = (lv_font_t * )&lv_font_montserrat_48;
    }
}

#endif /*LV_USE_FREETYPE*/

#endif //#if defined(CONFIG_LVGL_V9) && CONFIG_LVGL_V9
