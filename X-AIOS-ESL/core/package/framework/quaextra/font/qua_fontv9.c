
#include "qua_font.h"
#include "lvgl/lvgl.h"
#include "qua_font_ll.h"

#if defined(CONFIG_LVGL_V9) && CONFIG_LVGL_V9

lv_font_t * ft_font_ttf_20;
lv_font_t * ft_font_ttf_30;
lv_font_t * ft_font_ttf_12;
lv_font_t * ft_font_ttf_14;
lv_font_t * ft_font_ttf_40;

bool g_font_inited = false;

#if LV_USE_FREETYPE || LV_USE_TINY_TTF

#include "lang/xos_strid.h"

#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR
#define k_path_free_ttf     "./out/simulator/xos/res/ttf/2312.ttf"
#define k_path_tiny_ttf     "H:./out/simulator/xos/res/ttf/2312_v9.ttf"
#else
#define k_path_free_ttf     "/res/ttf/2312.ttf"
#define k_path_tiny_ttf     "H:/res/ttf/2312_v9.ttf"
#endif

static void * get_font_file_data(const char *font_path, uint32_t *size)
{
    static void * g_font_file_data = NULL;
    static uint32_t g_font_file_size = 0;

    if (font_path != NULL && strcmp(font_path, k_path_tiny_ttf) != 0)
        return NULL;

    if (g_font_file_data) {
        *size = g_font_file_size;
        return g_font_file_data;
    }

    lv_fs_file_t f;
    lv_fs_res_t res = lv_fs_open(&f, k_path_tiny_ttf, LV_FS_MODE_RD);
    if(res != LV_FS_RES_OK)
        return NULL;

    if(lv_fs_seek(&f, 0, LV_FS_SEEK_END) != 0) {
        lv_fs_close(&f);
        return NULL;
    }

    lv_fs_tell(&f, &g_font_file_size);
    if(lv_fs_seek(&f, 0, LV_FS_SEEK_SET) != 0) {
        lv_fs_close(&f);
        return NULL;
    }

    g_font_file_data = malloc(g_font_file_size);
    if (g_font_file_data == NULL) {
        lv_fs_close(&f);
        return NULL;
    }

    uint32_t br;
    res = lv_fs_read(&f, g_font_file_data, g_font_file_size, &br);
    if(res != LV_FS_RES_OK || br != g_font_file_size) {
        free(g_font_file_data);
        lv_fs_close(&f);
        return NULL;
    }

    lv_fs_close(&f);
    LV_LOG_WARN("g_font_file_size=%u\n", g_font_file_size);
    *size = g_font_file_size;
    return g_font_file_data;
}

static int __ft_font_init(lv_font_t ** pOutfont, uint16_t font_size, const char *font_path, uint16_t font_style)
{
    lv_font_t *font = NULL;
    const int path_count = 1;
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR
    #if LV_USE_FREETYPE
        char *font_path_array[1] = {"./out/simulator/xos/res/ttf/2312.ttf"};
    #else
        char *font_path_array[1] = {"H:./out/simulator/xos/res/ttf/2312_v9.ttf"};
    #endif
#else
    #if LV_USE_FREETYPE
        char *font_path_array[1] = {"/res/ttf/2312.ttf"};
    #else
        char *font_path_array[1] = {"H:/res/ttf/2312_v9.ttf"};
    #endif
#endif

#if LV_USE_FREETYPE
    int render_mode = LV_FREETYPE_FONT_RENDER_MODE_OUTLINE;//BITMAP;
    for (int i = 0; i < path_count; i++) {
        font = lv_freetype_font_create(font_path != NULL ? font_path : font_path_array[i], render_mode,
             font_size > 0 ? font_size : 10, 
             font_style? font_style : LV_FREETYPE_FONT_STYLE_NORMAL);
        if (font != NULL) {
            break;
        }
    }
#elif LV_TINY_TTF_FILE_SUPPORT
    void * data;
    uint32_t size;
    for (int i = 0; i < path_count; i++) {
        data = get_font_file_data(font_path != NULL ? font_path : font_path_array[i], &size);
        if (data)
            font = lv_tiny_ttf_create_data(data, size, font_size > 0 ? font_size : 10);
        else
            font = lv_tiny_ttf_create_file(font_path != NULL ? font_path : font_path_array[i], font_size > 0 ? font_size : 10);

        if (font != NULL) {
            break;
        }
    }
#endif

    *pOutfont = font;
    return font != NULL ? 0 : -1;
}

//old
void qua_los_ft_font_init(void)
{
    qua_xos_ft_init_with_path(NULL);
}

//new
void qua_xos_ft_init_with_path(const char *ttf_path)
{
    int rtn = 0;
 
    if (g_font_inited) return;

    rtn = __ft_font_init(&ft_font_ttf_12, 12, ttf_path, 0);
    if(rtn) LV_LOG_ERROR("los_ft_font_init ft_font_ttf_12 failed.");
    rtn = __ft_font_init(&ft_font_ttf_14, 14, ttf_path, 0);
    if(rtn) LV_LOG_ERROR("los_ft_font_init ft_font_ttf_14 failed.");
    rtn = __ft_font_init(&ft_font_ttf_20, 20, ttf_path, 0);
    if(rtn) LV_LOG_ERROR("los_ft_font_init ft_font_ttf_20 failed.");
    rtn = __ft_font_init(&ft_font_ttf_30, 30, ttf_path, 0);
    if(rtn) LV_LOG_ERROR("los_ft_font_init ft_font_ttf_30 failed.");
    rtn = __ft_font_init(&ft_font_ttf_40, 40, ttf_path, 0);
    if(rtn) LV_LOG_ERROR("los_ft_font_init ft_font_ttf_40 failed.");

    __add_font_list(ft_font_ttf_12, ttf_path, 12);
    __add_font_list(ft_font_ttf_14, ttf_path, 14);
    __add_font_list(ft_font_ttf_20, ttf_path, 20);
    __add_font_list(ft_font_ttf_30, ttf_path, 30);
    __add_font_list(ft_font_ttf_40, ttf_path, 40);

    g_font_inited = true;
}

void qua_los_ft_init_with_size(lv_font_t **ppFont, short font_size, short style)
{
#if LV_USE_FREETYPE
    const char *ttf_path = k_path_free_ttf;
#else
    const char *ttf_path = k_path_tiny_ttf;
#endif
    qua_xos_ft_init_with_size_path(ppFont, font_size, ttf_path, style);
}

void qua_xos_ft_init_with_size_path(lv_font_t **ppFont, short font_size, const char *ttf_path, short style)
{
    int rtn = 0;
    lv_font_t * _font = NULL;

    //LV_LOG_USER("font_size:%d ttf_path:%s.", font_size, ttf_path);
    if(ttf_path == NULL)
    {
        LV_LOG_ERROR("ERROR！font_size:%d ttf_path:%s is NULL.", font_size, ttf_path);
        *ppFont = NULL;
        goto __exit;
    }

    _font = __get_font_by_size(ttf_path, font_size);
    if (NULL != _font) {
        *ppFont = _font;
        goto __exit;
    }

    rtn = __ft_font_init(ppFont, font_size, ttf_path, 0);
    if(rtn) {
        LV_LOG_ERROR("ERROR！font_size:%d ttf_path:%s init failed.", font_size, ttf_path);
        *ppFont = NULL;
        goto __exit;
    }

    __add_font_list(*ppFont, ttf_path, font_size);

__exit:
    return;
}

#else
//impossible situation.

void qua_los_ft_font_init(void)
{
    qua_xos_ft_init_with_path(NULL);
}

//new
void qua_xos_ft_init_with_path(const char *ttf_path)
{
    #if !defined(CONFIG_CHIPSET_QM10XD)
    if (!g_font_inited) {
        g_font_inited = true;
        ft_font_ttf_12 = (lv_font_t *)&lv_font_montserrat_12;
        ft_font_ttf_14 = (lv_font_t *)&lv_font_montserrat_14;
        ft_font_ttf_20 = (lv_font_t *)&lv_font_montserrat_20;
        ft_font_ttf_30 = (lv_font_t *)&lv_font_montserrat_28;
        ft_font_ttf_40 = (lv_font_t *)&lv_font_montserrat_48;
    }
    #endif
}

#endif /*LV_USE_FREETYPE*/

#endif //#if defined(CONFIG_LVGL_V9) && CONFIG_LVGL_V9
