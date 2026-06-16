#include "xos_conf.h"

#if CONFIG_XOS_USE_INNER_RES != 0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include "lvgl/lvgl.h"

//#include "zlib.h"
#include "lang/QuaResTypes.h"
// #include "lang/QuaResPackage.h"
#include "lang/QuaResManager.h"
#include "xos_string_ids.h"
#include "xos_language_ids.h"

extern int xos_qua_uncompress_strings(unsigned char*, unsigned long*, const unsigned char*, unsigned long);

static int __is_lang_id_valild(int langId);

static char* resPkgStringsBucket = NULL;
static const char* resPkgStrings [MAX_STRID];
static int m_lang = 0;

#define _DBG_PRINTF(fmt, ...)   do {printf (fmt, ##__VA_ARGS__);} while (0)
#define _ERR_PRINTF(fmt, ...)   do {printf (fmt, ##__VA_ARGS__);} while (0)

#define __XOS_USE_ZIPPED_STRING__   1

static int __is_lang_id_valild(int langId)
{
    if (langId <= LANGID_AUTOMATIC || langId >= MAX_LANGID)
        return FALSE;

#if __XOS_USE_ZIPPED_STRING__
    if (__xos_map_language_to_zipped_bytes [langId].zipped_bytes == NULL) {
        _ERR_PRINTF ("%s: no strings for language: %d.\n", __func__, langId);
        return FALSE;
    }
#else
    if ( __xos_map_language_to_raw_strings [langId].raw_strings == NULL) {
        _ERR_PRINTF ("%s: no strings for language: %d.\n", __func__, langId);
        return FALSE;
    }
#endif
    return TRUE;
}

QBool ResPkgManagerSetCurrentLang (int langId)
{
    if (!__is_lang_id_valild(langId))
        return FALSE;

#if __XOS_USE_ZIPPED_STRING__
    if (resPkgStringsBucket == NULL) {
        unsigned int max_bucket_size = 0;
        for (int i = 1; i < MAX_LANGID; i++) {
            unsigned int current_size = __xos_map_language_to_zipped_bytes [i].origin_size;
            if (current_size > max_bucket_size) {
                max_bucket_size = current_size;
            }
        }

        if (max_bucket_size == 0) {
            _ERR_PRINTF ("%s: no any language defined.\n", __func__);
            return FALSE;
        }
        _DBG_PRINTF("max_bucket_size=%d", max_bucket_size);
        resPkgStringsBucket = (char*)malloc (max_bucket_size);
        if (resPkgStringsBucket == NULL) {
            return FALSE;
        }
    }

    unsigned int len_uncompressed = __xos_map_language_to_zipped_bytes [langId].origin_size;
    int ret = xos_qua_uncompress_strings ((unsigned char*)resPkgStringsBucket, (unsigned long*)&len_uncompressed, 
            (const unsigned char*)__xos_map_language_to_zipped_bytes [langId].zipped_bytes,
            (unsigned long)__xos_map_language_to_zipped_bytes [langId].zipped_size);
    if (ret) {
        _ERR_PRINTF ("%s: failed when calling uncompress: %d.\n", __func__, ret);
        return FALSE;
    }

    unsigned int offset = 0;
    for (int i = 0; i < MAX_STRID; i++) {
        resPkgStrings [i] = resPkgStringsBucket + offset;
        offset += strlen (resPkgStrings [i]);
        offset += 1;
    }
#else
    for (int i = 0; i < MAX_STRID; i++) {
        resPkgStrings [i] = &__xos_map_language_to_raw_strings [langId].raw_strings[i];
    }
#endif
    m_lang = langId;
    return TRUE;
}

int ResPkgManagerGetCurrentLang (void)
{
    return m_lang;
}

const char *ResPkgManagerGetLangSelfName(int langId)
{
    if (!__is_lang_id_valild(langId))
        return FALSE;

#if __XOS_USE_ZIPPED_STRING__
    return (const char *)__xos_map_language_to_zipped_bytes [langId].self_name;
#else
    return (const char *) __xos_map_language_to_raw_strings [langId].self_name;
#endif
}

int ResPkgManagerGetImeLangId(int langId)
{
    if (!__is_lang_id_valild(langId))
        return FALSE;
#if __XOS_USE_ZIPPED_STRING__
    return __xos_map_language_to_zipped_bytes [langId].ime_lang_id;
#else
    return __xos_map_language_to_raw_strings  [langId].ime_lang_id;
#endif
}

const char * GetTextRes(ResID id)
{
#if 0
    if( id >= STRID_TONEVOICE_TYPE_LOCAL_ECL_QTN_BEGIN &&
        id < (STRID_TONEVOICE_TYPE_LOCAL_ECL_QTN_BEGIN+RING_FILE_NUMBER))
    {
        return (NGChar *)(&sound_ring_name[id-STRID_TONEVOICE_TYPE_LOCAL_ECL_QTN_BEGIN][0]);
    }
#endif
    if (id >= MAX_STRID) {
        _ERR_PRINTF("%s:BadID STRID:%d.\n\n", __func__, id);
        return "BadID";
    }

    const char* text = resPkgStrings [id];
    if (text == NULL) {
        return "";
    }

    return text;
}

#endif /*CONFIG_XOS_USE_INNER_RES*/
