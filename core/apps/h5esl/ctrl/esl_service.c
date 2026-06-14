#ifdef CONFIG_XOS_USE_APP_NATIVE_ESL
#include "esl_service.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <curl/curl.h>
#include "../conf/conf.h"
#include "http_utils.h"
#include "cJSON.h"
#include "../model/esl_global_model.h"
#include "esl_file.h"
#include "../view/page_esl.h"
#include "../utils/esl_sign.h"
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "logo_img.h"
#endif


#define KbaseUrl "http://open.maolink.cn/openapi/tag/device/label"

esl_global_model_t g_esl_global_model = {0};

static float _round_to_two_decimal_places(float value) {
    return roundf(value * 100) / 100;
}

static size_t _write_data(void *contents, size_t size, size_t nmemb, void *userp) {
    struct response_data *data = (struct response_data *)userp;
    size_t realsize = size * nmemb;
    //printf("realsize %d. data->data=%p\n", realsize, data->data);

    char *ptr = realloc(data->data, data->size + realsize + 1);
    //char *ptr = malloc(data->size + realsize + 1);
    if(ptr == NULL) {
        // 内存分配失败
        printf("内存分配失败\n");
        if (data->data) {
            free(data->data);
            data->data = NULL;
        }
        data->size = 0;
        return 0;
    }
    //printf("ptr %p , %s\n", ptr, ptr);
    data->data = ptr;
    memcpy(&(data->data[data->size]), contents, realsize);
    data->size += realsize;
    data->data[data->size] = '\0';
    //printf("realsize %d. data->data=%p\n", realsize, data->data);

    return realsize;

}

static char * _post_remote_url_with_form_only_esl(const char *url,const char * device_no, const char *type , const char * sign,const char * app_id,const char * protocol_type,const char * local_ip,const char * device_list,const char * time,const char * version_no, const char *push_id, const char *push_type) {
    if (!url) {
        fprintf(stderr, "无效的URL或POST数据\n");
        return NULL;
    }
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Curl initialization failed\n");
        return NULL;
    }
    struct response_data data = {NULL, 0};
    struct curl_slist *headers = NULL;
    CURLcode res;
    char *result = NULL;

    headers = curl_slist_append(headers, "Content-Type: multipart/form-data");
    if (!headers) {
        fprintf(stderr, "创建请求头失败\n");
        goto cleanup;
    }
    // 设置要请求的 URL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // 设置回调函数，用于处理接收到的数据
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_data);
    // 将缓冲区指针传递给回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    // 设置请求头
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    // 设置超时时间
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L); // 连接超时时间10秒
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);       // 总超时时间30秒
    // Create the form
    curl_mime *form = curl_mime_init(curl);
    curl_mimepart *field = NULL;

    // Add APP_ID field
    field = curl_mime_addpart(form);
    curl_mime_name(field, "appid");
    curl_mime_data(field, app_id, CURL_ZERO_TERMINATED);

    // Add device_no field
    field = curl_mime_addpart(form);
    curl_mime_name(field, "deviceno");
    curl_mime_data(field, device_no, CURL_ZERO_TERMINATED);

    // Add type field
    field = curl_mime_addpart(form);
    curl_mime_name(field, "type");
    curl_mime_data(field, type, CURL_ZERO_TERMINATED);

    // Add sign field
    field = curl_mime_addpart(form);
    curl_mime_name(field, "sign");
    curl_mime_data(field, sign, CURL_ZERO_TERMINATED);

    // Add protocol_type field
    if (protocol_type) {
        field = curl_mime_addpart(form);
        curl_mime_name(field, "protocol_type");
        curl_mime_data(field, protocol_type, CURL_ZERO_TERMINATED);
    }

    // Add local_ip field
    if (local_ip) {
        field = curl_mime_addpart(form);
        curl_mime_name(field, "local_ip");
        curl_mime_data(field, local_ip, CURL_ZERO_TERMINATED);
    }

    // Add device_list field
    if (device_list) {
        field = curl_mime_addpart(form);
        curl_mime_name(field, "device_list");
        curl_mime_data(field, device_list, CURL_ZERO_TERMINATED);
    }

    // Add time field
    if (time) {
        field = curl_mime_addpart(form);
        curl_mime_name(field, "time");
        curl_mime_data(field, time, CURL_ZERO_TERMINATED);
    }

    if (version_no){
        field = curl_mime_addpart(form);
        curl_mime_name(field, "version_no");
        curl_mime_data(field, version_no, CURL_ZERO_TERMINATED);
    }

    if (push_id) {
        field = curl_mime_addpart(form);
        curl_mime_name(field, "push_id");
        curl_mime_data(field, push_id, CURL_ZERO_TERMINATED);
    }

    if (push_type) {
        field = curl_mime_addpart(form);
        curl_mime_name(field, "push_type");
        curl_mime_data(field, push_type, CURL_ZERO_TERMINATED);
    }

    // 设置 POST 数据
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
    // 执行请求
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "POST请求失败: %s\n", curl_easy_strerror(res));
        goto cleanup;
    }
    result = data.data;

cleanup:
    if (headers) {
        curl_slist_free_all(headers);
    }
    if (form) {
        curl_mime_free(form);
    }
    if (curl) {
        curl_easy_cleanup(curl);
    }
    if (!result && data.data) {
        free(data.data);
    }
    return result;
}

esl_push_server_data * query_push_server(const char * device_no, const char *type , const char * sign,const char *protocol_type,const char * local_ip,const char * version_no){
    printf("query_push_server! device_no:%s,type:%s,sign:%s\n",device_no,type,sign);
    char *response_content = _post_remote_url_with_form_only_esl(KbaseUrl,device_no,type,sign,APP_ID,protocol_type,local_ip,NULL,NULL,version_no,NULL,NULL);
    printf("response_content:%s",response_content);
    cJSON *json = cJSON_Parse(response_content);
    if (json == NULL) {
        printf("Error parsing JSON\n");
        free(response_content);
        return NULL;
    }

    cJSON *stateItem = cJSON_GetObjectItem(json, "State");
    if (stateItem != NULL && cJSON_IsString(stateItem)) {
        if(strcmp(stateItem->valuestring,"Fail") == 0){
            esl_push_server_data *data = (esl_push_server_data *)malloc(sizeof(esl_push_server_data));
            memset(data, 0, sizeof(esl_push_server_data));
            data->query_success = -1;
            cJSON_Delete(json);
            free(response_content);
            return data;
        }
    }else{
        if(strcmp(stateItem->valuestring,"Done") != 0){
            printf("Failed to get product_info JSON State.\n");
            cJSON_Delete(json);
            free(response_content);
            return NULL;
        }
    }

    esl_push_server_data *data = (esl_push_server_data *)malloc(sizeof(esl_push_server_data));
    memset(data, 0, sizeof(esl_push_server_data));
    if (data == NULL) {
        printf("Memory allocation failed\n");
        cJSON_Delete(json);
        free(response_content);
        return NULL;
    }

    cJSON *data_json = cJSON_GetObjectItem(json, "Data");
    if (data_json == NULL) {
        printf("Error getting Data object\n");
        free(data);
        cJSON_Delete(json);
        free(response_content);
        return NULL;
    }

    cJSON *item;

    item = cJSON_GetObjectItem(data_json, "update_version");
    data->update_version = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "update_url");
    data->update_url = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "update_file_md5");
    data->update_file_md5 = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "boot_logo_url");
    data->boot_logo_url = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "boot_logo_md5");
    data->boot_logo_md5 = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "bind_logo_url");
    data->bind_logo_url = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "bind_logo_md5");
    data->bind_logo_md5 = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "host");
    data->host = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "port");
    if(item){
        if(cJSON_IsNumber(item)){
            data->port = item->valueint;
        }else if(cJSON_IsString(item)){
            data->port = atoi(item->valuestring);
        }else{
            data->port = 0;
        }
    }else{
        data->port = 0;
    }

    item = cJSON_GetObjectItem(data_json, "username");
    data->username = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "password");
    data->password = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "protocol");
    data->protocol = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "client_id");
    data->client_id = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "topic");
    data->topic = item ? strdup(item->valuestring) : strdup("");

    item = cJSON_GetObjectItem(data_json, "master");
    data->master = item ? item->valueint : 0;

    item = cJSON_GetObjectItem(data_json, "group_id");
    data->group_id = item ? item->valueint : 0;

    data->query_success = 1;

    save_update(data);
    cJSON_Delete(json);
    free(response_content);
    return data;
}

esl_show_model_t * parse_product_template(cJSON *data){
    if (!cJSON_IsString(data)) {
        printf("Failed to get JSON Data.\n");
        return NULL;
    }

    char *data_str = strdup(data->valuestring);
    cJSON *template_data_json = cJSON_Parse(data_str);
    if (template_data_json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf("Error before: %s\n", error_ptr);
        }
        free(data_str);
        return NULL;
    }
    esl_show_model_t *model = (esl_show_model_t *)malloc(sizeof(esl_show_model_t));
    memset(model, 0, sizeof(esl_show_model_t));

    cJSON *name = cJSON_GetObjectItemCaseSensitive(template_data_json, "name");
    //model->name = cJSON_IsString(name) && (name->valuestring != NULL) ? strdup(name->valuestring) : strdup("");
    if(cJSON_IsString(name) && (name->valuestring != NULL)){
        model->name = strdup(name->valuestring);
    }

    cJSON *resolution = cJSON_GetObjectItemCaseSensitive(template_data_json, "resolution");
    //model->resolution = cJSON_IsString(resolution) && (resolution->valuestring != NULL) ? strdup(resolution->valuestring) : strdup("");
    if(cJSON_IsString(resolution) && (resolution->valuestring != NULL)){
        model->resolution = strdup(resolution->valuestring);
    }

    cJSON *type = cJSON_GetObjectItemCaseSensitive(template_data_json, "type");
    //model->type = cJSON_IsString(type) && (type->valuestring != NULL) ? strdup(type->valuestring) : strdup("");
    if(cJSON_IsString(type) && (type->valuestring != NULL)){
        model->type = strdup(type->valuestring);
    }

    cJSON *itemNum = cJSON_GetObjectItemCaseSensitive(template_data_json, "itemNum");
    model->itemNum = cJSON_IsNumber(itemNum) ? itemNum->valueint : 0;

    cJSON *status = cJSON_GetObjectItemCaseSensitive(template_data_json, "status");
    model->status = cJSON_IsNumber(status) ? status->valueint : 0;
   

    cJSON *thumbnail = cJSON_GetObjectItemCaseSensitive(template_data_json, "thumbnail");
    //model->thumbnail = cJSON_IsString(thumbnail) && (thumbnail->valuestring != NULL) ? strdup(thumbnail->valuestring) : strdup("");
    if (cJSON_IsString(thumbnail) && (thumbnail->valuestring != NULL)) {
        model->thumbnail = strdup(thumbnail->valuestring);
    }

    cJSON *width = cJSON_GetObjectItemCaseSensitive(template_data_json, "width");
    model->width = cJSON_IsNumber(width) ? width->valueint : 0;

    cJSON *height = cJSON_GetObjectItemCaseSensitive(template_data_json, "height");
    model->height = cJSON_IsNumber(height) ? height->valueint : 0;

    cJSON *contextWidth = cJSON_GetObjectItemCaseSensitive(template_data_json, "contextWidth");
    //model->contextWidth = cJSON_IsNumber(contextWidth) ? contextWidth->valueint : 0;
    if (cJSON_IsNumber(contextWidth)) {
        model->contextWidth = contextWidth->valueint;
    }

    cJSON *contextHeight = cJSON_GetObjectItemCaseSensitive(template_data_json, "contextHeight");
    //model->contextHeight = cJSON_IsNumber(contextHeight) ? contextHeight->valueint : 0;
    if (cJSON_IsNumber(contextHeight)) {
        model->contextHeight = contextHeight->valueint;
    }

    cJSON *devicePixelRatio = cJSON_GetObjectItemCaseSensitive(template_data_json, "devicePixelRatio");
    model->devicePixelRatio = cJSON_IsNumber(devicePixelRatio) ? _round_to_two_decimal_places((float)devicePixelRatio->valuedouble) : 1.0f;

    cJSON *onlyRender = cJSON_GetObjectItemCaseSensitive(template_data_json, "onlyRender");
    model->onlyRender = cJSON_IsBool(onlyRender) ? cJSON_IsTrue(onlyRender) : false;

    cJSON *child_list = cJSON_GetObjectItemCaseSensitive(template_data_json, "child_list");
    if (cJSON_IsArray(child_list)) {
        model->child_count = cJSON_GetArraySize(child_list);
        model->child_list = malloc(model->child_count * sizeof(esl_show_model_child_t));
        memset(model->child_list, 0, model->child_count * sizeof(esl_show_model_child_t));
        for (int i = 0; i < model->child_count; i++) {
            cJSON *child = cJSON_GetArrayItem(child_list, i);
            esl_show_model_child_t *child_model = &model->child_list[i];

            cJSON *child_name = cJSON_GetObjectItemCaseSensitive(child, "name");
            //child_model->name = cJSON_IsString(child_name) && (child_name->valuestring != NULL) ? strdup(child_name->valuestring) : strdup("");
            if(cJSON_IsString(child_name) && (child_name->valuestring != NULL)){
                child_model->name = strdup(child_name->valuestring);
            }

            cJSON *child_x = cJSON_GetObjectItemCaseSensitive(child, "x");
            child_model->x = cJSON_IsNumber(child_x) ? _round_to_two_decimal_places((float)child_x->valuedouble) : 0.0f;

            cJSON *child_y = cJSON_GetObjectItemCaseSensitive(child, "y");
            child_model->y = cJSON_IsNumber(child_y) ? _round_to_two_decimal_places((float)child_y->valuedouble) : 0.0f;

            cJSON *child_w = cJSON_GetObjectItemCaseSensitive(child, "w");
            child_model->w = cJSON_IsNumber(child_w) ? _round_to_two_decimal_places((float)child_w->valuedouble) : 0.0f;

            cJSON *child_h = cJSON_GetObjectItemCaseSensitive(child, "h");
            child_model->h = cJSON_IsNumber(child_h) ? _round_to_two_decimal_places((float)child_h->valuedouble) : 0.0f;

            cJSON *child_angle = cJSON_GetObjectItemCaseSensitive(child, "angle");
            child_model->angle = cJSON_IsNumber(child_angle) ? _round_to_two_decimal_places((float)child_angle->valuedouble) : 0.0f;

            cJSON *child_type = cJSON_GetObjectItemCaseSensitive(child, "type");
            //child_model->type = cJSON_IsString(child_type) && (child_type->valuestring != NULL) ? strdup(child_type->valuestring) : strdup("");
            if(cJSON_IsString(child_type) && (child_type->valuestring != NULL)){
                child_model->type = strdup(child_type->valuestring);
            }

            cJSON *child_uuid = cJSON_GetObjectItemCaseSensitive(child, "uuid");
            if (cJSON_IsString(child_uuid) && (child_uuid->valuestring != NULL)) {
                strncpy(child_model->uuid, child_uuid->valuestring, sizeof(child_model->uuid) - 1);
            }

            cJSON *child_edit = cJSON_GetObjectItemCaseSensitive(child, "edit");
            child_model->edit = cJSON_IsBool(child_edit) ? cJSON_IsTrue(child_edit) : false;

            // Parse esl_show_model_child_desc_t
            cJSON *desc = cJSON_GetObjectItemCaseSensitive(child, "desc");
            if (cJSON_IsObject(desc)) {
                cJSON *src = cJSON_GetObjectItemCaseSensitive(desc, "src");
                //child_model->desc.src = cJSON_IsString(src) && (src->valuestring != NULL) ? strdup(src->valuestring) : strdup("");
                if (cJSON_IsString(src) && (src->valuestring != NULL)){
                    child_model->desc.src = strdup(src->valuestring);
                }

                cJSON *text = cJSON_GetObjectItemCaseSensitive(desc, "text");
                //child_model->desc.text = cJSON_IsString(text) && (text->valuestring != NULL) ? strdup(text->valuestring) : strdup("");
                if (cJSON_IsString(text) && (text->valuestring != NULL)){
                    child_model->desc.text = strdup(text->valuestring);
                }

                cJSON *color = cJSON_GetObjectItemCaseSensitive(desc, "color");
                //child_model->desc.color = cJSON_IsString(color) && (color->valuestring != NULL) ? strdup(color->valuestring) : strdup("");
                if (cJSON_IsString(color) && (color->valuestring != NULL)){
                    child_model->desc.color = strdup(color->valuestring);
                }

                cJSON *fontSize = cJSON_GetObjectItemCaseSensitive(desc, "fontSize");
                if(cJSON_IsNumber(fontSize)){
                    child_model->desc.fontSize = malloc(10);
                    sprintf(child_model->desc.fontSize,"%d",fontSize->valueint);
                }else{
                    //child_model->desc.fontSize = cJSON_IsString(fontSize) && (fontSize->valuestring != NULL) ? strdup(fontSize->valuestring) : strdup("");
                    if (cJSON_IsString(fontSize) && (fontSize->valuestring != NULL)){
                        child_model->desc.fontSize = strdup(fontSize->valuestring);
                    }
                }

                cJSON *fontWeight = cJSON_GetObjectItemCaseSensitive(desc, "fontWeight");
                //child_model->desc.fontWeight = cJSON_IsString(fontWeight) && (fontWeight->valuestring != NULL) ? strdup(fontWeight->valuestring) : strdup("");
                if (cJSON_IsString(fontWeight) && (fontWeight->valuestring != NULL)){
                    child_model->desc.fontWeight = strdup(fontWeight->valuestring);
                }

                cJSON *textAlign = cJSON_GetObjectItemCaseSensitive(desc, "textAlign");
                //child_model->desc.textAlign = cJSON_IsString(textAlign) && (textAlign->valuestring != NULL) ? strdup(textAlign->valuestring) : strdup("");
                if (cJSON_IsString(textAlign) && (textAlign->valuestring != NULL)){
                    child_model->desc.textAlign = strdup(textAlign->valuestring);
                }

                cJSON *fontFamily = cJSON_GetObjectItemCaseSensitive(desc, "fontFamily");
                //child_model->desc.fontFamily = cJSON_IsString(fontFamily) && (fontFamily->valuestring != NULL) ? strdup(fontFamily->valuestring) : strdup("");
                if (cJSON_IsString(fontFamily) && (fontFamily->valuestring != NULL)){
                    child_model->desc.fontFamily = strdup(fontFamily->valuestring);
                }

                cJSON *bgColor = cJSON_GetObjectItemCaseSensitive(desc, "bgColor");
                //child_model->desc.bgColor = cJSON_IsString(bgColor) && (bgColor->valuestring != NULL) ? strdup(bgColor->valuestring) : strdup("");
                if (cJSON_IsString(bgColor) && (bgColor->valuestring != NULL)){
                    child_model->desc.bgColor = strdup(bgColor->valuestring);
                }

                cJSON *borderRadius = cJSON_GetObjectItemCaseSensitive(desc, "borderRadius");
                child_model->desc.borderRadius = cJSON_IsNumber(borderRadius) ? _round_to_two_decimal_places((float)borderRadius->valuedouble) : 0.0f;

                cJSON *borderWidth = cJSON_GetObjectItemCaseSensitive(desc, "borderWidth");
                child_model->desc.borderWidth = cJSON_IsNumber(borderWidth) ? _round_to_two_decimal_places((float)borderWidth->valuedouble) : 0.0f;
            }

            // Parse esl_show_model_child_extension_t
            cJSON *extension = cJSON_GetObjectItemCaseSensitive(child, "extension");
            if (cJSON_IsObject(extension)) {
                cJSON *ex_type = cJSON_GetObjectItemCaseSensitive(extension, "ex_type");
                //child_model->extension.ex_type = cJSON_IsString(ex_type) && (ex_type->valuestring != NULL) ? strdup(ex_type->valuestring) : strdup("");
                if (cJSON_IsString(ex_type) && (ex_type->valuestring != NULL)){
                    child_model->extension.ex_type = strdup(ex_type->valuestring);
                }

                cJSON *textDecoration = cJSON_GetObjectItemCaseSensitive(extension, "textDecoration");
                //child_model->extension.textDecoration = cJSON_IsString(textDecoration) && (textDecoration->valuestring != NULL) ? strdup(textDecoration->valuestring) : strdup("");
                if (cJSON_IsString(textDecoration) && (textDecoration->valuestring != NULL)){
                    child_model->extension.textDecoration = strdup(textDecoration->valuestring);
                }

                cJSON *unit_price = cJSON_GetObjectItemCaseSensitive(extension, "unit_price");
                //child_model->extension.unit_price = cJSON_IsString(unit_price) && (unit_price->valuestring != NULL) ? strdup(unit_price->valuestring) : strdup("");
                if (cJSON_IsString(unit_price) && (unit_price->valuestring != NULL)){
                    child_model->extension.unit_price = strdup(unit_price->valuestring);
                }

                cJSON *swiper_list = cJSON_GetObjectItemCaseSensitive(extension, "swiper_list");
                if (cJSON_IsArray(swiper_list)) {
                    int swiper_list_size = cJSON_GetArraySize(swiper_list);
                    child_model->extension.swiper_list = malloc((swiper_list_size + 1) * sizeof(char *));
                    memset(child_model->extension.swiper_list, 0, (swiper_list_size + 1) * sizeof(char *));
                    for (int j = 0; j < swiper_list_size; j++) {
                        cJSON *swiper_item = cJSON_GetArrayItem(swiper_list, j);
                        //child_model->extension.swiper_list[j] = cJSON_IsString(swiper_item) && (swiper_item->valuestring != NULL) ? strdup(swiper_item->valuestring) : strdup("");
                        if (cJSON_IsString(swiper_item) && (swiper_item->valuestring != NULL)){
                            child_model->extension.swiper_list[j] = strdup(swiper_item->valuestring);
                        }
                    }
                    child_model->extension.swiper_list[swiper_list_size] = NULL; // Null-terminate the array
                    child_model->extension.swiper_list_count = swiper_list_size;
                } else {
                    child_model->extension.swiper_list = NULL;
                    child_model->extension.swiper_list_count = 0;
                }

                cJSON *value = cJSON_GetObjectItemCaseSensitive(extension, "value");
                //child_model->extension.value = cJSON_IsString(value) && (value->valuestring != NULL) ? strdup(value->valuestring) : strdup("");
                if (cJSON_IsString(value) && (value->valuestring != NULL)){
                    child_model->extension.value = strdup(value->valuestring);
                }

                cJSON *bind = cJSON_GetObjectItemCaseSensitive(extension, "bind");
                //child_model->extension.bind = cJSON_IsString(bind) && (bind->valuestring != NULL) ? strdup(bind->valuestring) : strdup("");
                if (cJSON_IsString(bind) && (bind->valuestring != NULL)){
                    child_model->extension.bind = strdup(bind->valuestring);
                }

                cJSON *html = cJSON_GetObjectItemCaseSensitive(extension, "html");
                if (cJSON_IsObject(html)) {
                    cJSON *text = cJSON_GetObjectItemCaseSensitive(html, "text");
                    //child_model->extension.html.text = cJSON_IsString(text) && (text->valuestring != NULL) ? strdup(text->valuestring) : strdup("");
                    if (cJSON_IsString(text) && (text->valuestring != NULL)){
                        child_model->extension.html.text = strdup(text->valuestring);
                    }

                    cJSON *color = cJSON_GetObjectItemCaseSensitive(html, "color");
                    //child_model->extension.html.color = cJSON_IsString(color) && (color->valuestring != NULL) ? strdup(color->valuestring) : strdup("");
                    if (cJSON_IsString(color) && (color->valuestring != NULL)){
                        child_model->extension.html.color = strdup(color->valuestring);
                    }

                    cJSON *intFontSize = cJSON_GetObjectItemCaseSensitive(html, "intFontSize");
                    if (cJSON_IsString(intFontSize) && intFontSize->valuestring != NULL) {
                        child_model->extension.html.intFontSize = atoi(intFontSize->valuestring);
                    } else if (cJSON_IsNumber(intFontSize)) {
                        child_model->extension.html.intFontSize = intFontSize->valueint;
                    } else {
                        child_model->extension.html.intFontSize = 0;
                    }

                    cJSON *floatFontSize = cJSON_GetObjectItemCaseSensitive(html, "floatFontSize");
                    if (cJSON_IsString(floatFontSize) && floatFontSize->valuestring != NULL) {
                        child_model->extension.html.floatFontSize = atoi(floatFontSize->valuestring);
                    } else if (cJSON_IsNumber(floatFontSize)) {
                        child_model->extension.html.floatFontSize = floatFontSize->valuedouble;
                    } else {
                        child_model->extension.html.floatFontSize = 0;
                    }

                    cJSON *fontWeight = cJSON_GetObjectItemCaseSensitive(html, "fontWeight");
                    //child_model->extension.html.fontWeight = cJSON_IsString(fontWeight) && (fontWeight->valuestring != NULL) ? strdup(fontWeight->valuestring) : strdup("");
                    if (cJSON_IsString(fontWeight) && (fontWeight->valuestring != NULL)){
                        child_model->extension.html.fontWeight = strdup(fontWeight->valuestring);
                    }

                    cJSON *textAlign = cJSON_GetObjectItemCaseSensitive(html, "textAlign");
                    //child_model->extension.html.textAlign = cJSON_IsString(textAlign) && (textAlign->valuestring != NULL) ? strdup(textAlign->valuestring) : strdup("");
                    if (cJSON_IsString(textAlign) && (textAlign->valuestring != NULL)){
                        child_model->extension.html.textAlign = strdup(textAlign->valuestring);
                    }

                    cJSON *fontFamily = cJSON_GetObjectItemCaseSensitive(html, "fontFamily");
                    //child_model->extension.html.fontFamily = cJSON_IsString(fontFamily) && (fontFamily->valuestring != NULL) ? strdup(fontFamily->valuestring) : strdup("");
                    if (cJSON_IsString(fontFamily) && (fontFamily->valuestring != NULL)){
                        child_model->extension.html.fontFamily = strdup(fontFamily->valuestring);
                    }

                    cJSON *numFloatStyle = cJSON_GetObjectItemCaseSensitive(html, "numFloatStyle");
                    //child_model->extension.html.numFloatStyle = cJSON_IsString(numFloatStyle) && (numFloatStyle->valuestring != NULL) ? strdup(numFloatStyle->valuestring) : strdup("");
                    if (cJSON_IsString(numFloatStyle) && (numFloatStyle->valuestring != NULL)){
                        child_model->extension.html.numFloatStyle = strdup(numFloatStyle->valuestring);
                    }

                    cJSON *fontStyle = cJSON_GetObjectItemCaseSensitive(html, "fontStyle");
                    //child_model->extension.html.fontStyle = cJSON_IsString(fontStyle) && (fontStyle->valuestring != NULL) ? strdup(fontStyle->valuestring) : strdup("");
                    if (cJSON_IsString(fontStyle) && (fontStyle->valuestring != NULL)){
                        child_model->extension.html.fontStyle = strdup(fontStyle->valuestring);
                    }

                    cJSON *textDecoration = cJSON_GetObjectItemCaseSensitive(html, "textDecoration");
                    //child_model->extension.html.textDecoration = cJSON_IsString(textDecoration) && (textDecoration->valuestring != NULL) ? strdup(textDecoration->valuestring) : strdup("");
                    if (cJSON_IsString(textDecoration) && (textDecoration->valuestring != NULL)){
                        child_model->extension.html.textDecoration = strdup(textDecoration->valuestring);
                    }

                    cJSON *bgColor = cJSON_GetObjectItemCaseSensitive(html, "bgColor");
                    //child_model->extension.html.bgColor = cJSON_IsString(bgColor) && (bgColor->valuestring != NULL) ? strdup(bgColor->valuestring) : strdup("");
                    if (cJSON_IsString(bgColor) && (bgColor->valuestring != NULL)){
                        child_model->extension.html.bgColor = strdup(bgColor->valuestring);
                    }
                }
            }
        }
    }
    cJSON_Delete(template_data_json);
    free(data_str);
    return model;
}

esl_show_model_t *query_product_template(const char * device_no, const char *query_type , const char * sign){
    printf("query_product_template! device_no:%s,query_type:%s,sign:%s\n",device_no,query_type,sign);
    char *response_content = _post_remote_url_with_form_only_esl(KbaseUrl,device_no,query_type,sign,APP_ID,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    cJSON *template_json = cJSON_Parse(response_content);
    if (template_json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf("Error before: %s\n", error_ptr);
        }
        free(response_content);
        return NULL;
    }

    cJSON *stateItem = cJSON_GetObjectItem(template_json, "State");
    if (stateItem == NULL || !cJSON_IsString(stateItem) || strcmp(stateItem->valuestring,"Done") != 0) {
        printf("Failed to get product_info JSON State.\n");
        cJSON_Delete(template_json);
        free(response_content);
        return NULL;
    }

    cJSON *data = cJSON_GetObjectItemCaseSensitive(template_json, "Data");
    esl_show_model_t * model = parse_product_template(data);
    cJSON_Delete(template_json);
    free(response_content);
    return model;
}

esl_product_info_t ** parse_product_infos(cJSON *dataItem,int * count){
    if (dataItem == NULL || !cJSON_IsArray(dataItem)) {
        printf("Failed to get product_info JSON Data.\n");
        return NULL;
    }
    int array_size = cJSON_GetArraySize(dataItem);
    esl_product_info_t **product_info_list = (esl_product_info_t **)malloc(array_size * sizeof(esl_product_info_t *));
    if (product_info_list == NULL) {
        printf("Failed to allocate memory for product_info_list.\n");
        return NULL;
    }
    printf("product_info_list:%p \n", product_info_list);
    *count = array_size;

    for (int i = 0; i < array_size; i++) {
        esl_product_info_t *product_info = (esl_product_info_t *)malloc(sizeof(esl_product_info_t));
        if (product_info == NULL) {
            printf("Failed to allocate memory for product_info.\n");
            for (int j = 0; j < i; j++) {
                free(product_info_list[j]);
            }
            free(product_info_list);
            return NULL;
        }
        cJSON *product_info_item = cJSON_GetArrayItem(dataItem, i);

        cJSON *item_no = cJSON_GetObjectItemCaseSensitive(product_info_item, "item_no");
        if (item_no && cJSON_IsString(item_no) && item_no->valuestring) {
            product_info->item_no = strdup(item_no->valuestring);
        } else {
            product_info->item_no = strdup("");
        }

        cJSON *item_subno = cJSON_GetObjectItemCaseSensitive(product_info_item, "item_subno");
        product_info->item_subno = cJSON_IsString(item_subno) && (item_subno->valuestring != NULL) ? strdup(item_subno->valuestring) : strdup("");

        cJSON *item_name = cJSON_GetObjectItemCaseSensitive(product_info_item, "item_name");
        product_info->item_name = cJSON_IsString(item_name) && (item_name->valuestring != NULL) ? strdup(item_name->valuestring) : strdup("");

        cJSON *item_subname = cJSON_GetObjectItemCaseSensitive(product_info_item, "item_subname");
        product_info->item_subname = cJSON_IsString(item_subname) && (item_subname->valuestring != NULL) ? strdup(item_subname->valuestring) : strdup("");

        cJSON *main_supcust = cJSON_GetObjectItemCaseSensitive(product_info_item, "main_supcust");
        product_info->main_supcust = cJSON_IsString(main_supcust) && (main_supcust->valuestring != NULL) ? strdup(main_supcust->valuestring) : strdup("");

        cJSON *item_clsno = cJSON_GetObjectItemCaseSensitive(product_info_item, "item_clsno");
        product_info->item_clsno = cJSON_IsString(item_clsno) && (item_clsno->valuestring != NULL) ? strdup(item_clsno->valuestring) : strdup("");

        cJSON *combine_sta = cJSON_GetObjectItemCaseSensitive(product_info_item, "combine_sta");
        product_info->combine_sta = cJSON_IsString(combine_sta) && (combine_sta->valuestring != NULL) ? strdup(combine_sta->valuestring) : strdup("");

        cJSON *item_sup_flag = cJSON_GetObjectItemCaseSensitive(product_info_item, "item_sup_flag");
        product_info->item_sup_flag = cJSON_IsString(item_sup_flag) && (item_sup_flag->valuestring != NULL) ? strdup(item_sup_flag->valuestring) : strdup("");

        cJSON *item_stock = cJSON_GetObjectItemCaseSensitive(product_info_item, "item_stock");
        product_info->item_stock = cJSON_IsString(item_stock) && (item_stock->valuestring != NULL) ? strdup(item_stock->valuestring) : strdup("");

        cJSON *price = cJSON_GetObjectItemCaseSensitive(product_info_item, "price");
        product_info->price = cJSON_IsString(price) && (price->valuestring != NULL) ? strdup(price->valuestring) : strdup("0.00");

        cJSON *base_price = cJSON_GetObjectItemCaseSensitive(product_info_item, "base_price");
        product_info->base_price = cJSON_IsNumber(base_price) ? _round_to_two_decimal_places(base_price->valuedouble) : 0.0f;

        cJSON *sale_price = cJSON_GetObjectItemCaseSensitive(product_info_item, "sale_price");
        product_info->sale_price = cJSON_IsString(sale_price) && (sale_price->valuestring != NULL) ? strdup(sale_price->valuestring) : strdup("0.00");

        cJSON *vip_price = cJSON_GetObjectItemCaseSensitive(product_info_item, "vip_price");
        product_info->vip_price = cJSON_IsString(vip_price) && (vip_price->valuestring != NULL) ? strdup(vip_price->valuestring) : strdup("0.00");

        cJSON *num2 = cJSON_GetObjectItemCaseSensitive(product_info_item, "num2");
        product_info->num2 = cJSON_IsNumber(num2) ? num2->valueint : 0;

        cJSON *num3 = cJSON_GetObjectItemCaseSensitive(product_info_item, "num3");
        product_info->num3 = cJSON_IsNumber(num3) ? num3->valueint : 0;

        cJSON *item_rem = cJSON_GetObjectItemCaseSensitive(product_info_item, "item_rem");
        product_info->item_rem = cJSON_IsString(item_rem) && (item_rem->valuestring != NULL) ? strdup(item_rem->valuestring) : strdup("");

        cJSON *unit_no = cJSON_GetObjectItemCaseSensitive(product_info_item, "unit_no");
        product_info->unit_no = cJSON_IsString(unit_no) && (unit_no->valuestring != NULL) ? strdup(unit_no->valuestring) : strdup("");

        cJSON *item_size = cJSON_GetObjectItemCaseSensitive(product_info_item, "item_size");
        product_info->item_size = cJSON_IsString(item_size) && (item_size->valuestring != NULL) ? strdup(item_size->valuestring) : strdup("");

        cJSON *product_area = cJSON_GetObjectItemCaseSensitive(product_info_item, "product_area");
        product_info->product_area = cJSON_IsString(product_area) && (product_area->valuestring != NULL) ? strdup(product_area->valuestring) : strdup("");

        cJSON *build_date = cJSON_GetObjectItemCaseSensitive(product_info_item, "build_date");
        product_info->build_date = cJSON_IsString(build_date) && (build_date->valuestring != NULL) ? strdup(build_date->valuestring) : strdup("");

        cJSON *modify_date = cJSON_GetObjectItemCaseSensitive(product_info_item, "modify_date");
        product_info->modify_date = cJSON_IsString(modify_date) && (modify_date->valuestring != NULL) ? strdup(modify_date->valuestring) : strdup("");

        cJSON *item_url = cJSON_GetObjectItemCaseSensitive(product_info_item, "item_url");
        product_info->item_url = cJSON_IsString(item_url) && (item_url->valuestring != NULL) ? strdup(item_url->valuestring) : strdup("");

        cJSON *purchase_spec = cJSON_GetObjectItemCaseSensitive(product_info_item, "purchase_spec");
        product_info->purchase_spec = cJSON_IsNumber(purchase_spec) ? purchase_spec->valueint : 0;

        cJSON *status = cJSON_GetObjectItemCaseSensitive(product_info_item, "status");
        product_info->status = cJSON_IsString(status) && (status->valuestring != NULL) ? strdup(status->valuestring) : strdup("");

        cJSON *min_opqty = cJSON_GetObjectItemCaseSensitive(product_info_item, "min_opqty");
        product_info->min_opqty = cJSON_IsNumber(min_opqty) ? min_opqty->valueint : 0;

        cJSON *item_video = cJSON_GetObjectItemCaseSensitive(product_info_item, "item_video");
        product_info->item_video = cJSON_IsString(item_video) && (item_video->valuestring != NULL) ? strdup(item_video->valuestring) : strdup("");

        cJSON *main_supname = cJSON_GetObjectItemCaseSensitive(product_info_item, "main_supname");
        product_info->main_supname = cJSON_IsString(main_supname) && (main_supname->valuestring != NULL) ? strdup(main_supname->valuestring) : strdup("");

        product_info_list[i] = product_info;
    }
    return product_info_list;
}

esl_product_info_t ** query_product_infos(const char * device_no, const char *type , const char * sign, int *count){
    printf("query_product_template! device_no:%s,type:%s,sign:%s\n",device_no,type,sign);
    char *response_content = _post_remote_url_with_form_only_esl(KbaseUrl,device_no,type,sign,APP_ID,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    if (response_content == NULL) {
        return NULL;
    }
    cJSON *product_info_array = cJSON_Parse(response_content);
    if (product_info_array == NULL) {
        printf("Failed to parse product_info JSON array.\n");
        free(response_content);
        return NULL;
    }
    cJSON *stateItem = cJSON_GetObjectItem(product_info_array, "State");
    if (stateItem == NULL || !cJSON_IsString(stateItem) || strcmp(stateItem->valuestring,"Done") != 0) {
        printf("Failed to get product_info JSON State.\n");
        cJSON_Delete(product_info_array);
        free(response_content);
        return NULL;
    }
    cJSON *dataItem = cJSON_GetObjectItem(product_info_array, "Data");
    esl_product_info_t ** product_info_list = parse_product_infos(dataItem,count);
    cJSON_Delete(product_info_array);
    free(response_content);
    return product_info_list;
}

esl_device_setting_t * parse_device_setting(cJSON *data){
    if (!cJSON_IsString(data)) {
        printf("Failed to get JSON Data.\n");
        return NULL;
    }

    char *data_str = strdup(data->valuestring);
    cJSON *template_data_json = cJSON_Parse(data_str);
    if (template_data_json == NULL) {
        free(data_str);
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf("Error before: %s\n", error_ptr);
        }
        return NULL;
    }
    esl_device_setting_t *device_setting = (esl_device_setting_t *)malloc(sizeof(esl_device_setting_t));
    if (!device_setting) {
        cJSON_Delete(template_data_json);
        free(data_str);
        return NULL;
    }

    cJSON *SN_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "SN");
    device_setting->SN = SN_item && cJSON_IsString(SN_item) ? strdup(SN_item->valuestring) : strdup("");

    cJSON *pos_ip_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "pos_ip");
    device_setting->pos_ip = pos_ip_item && cJSON_IsString(pos_ip_item) ? strdup(pos_ip_item->valuestring) : strdup("");

    cJSON *pos_id_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "pos_id");
    device_setting->pos_id = pos_id_item && cJSON_IsString(pos_id_item) ? strdup(pos_id_item->valuestring) : strdup("");

    cJSON *server_ip_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "server_ip");
    device_setting->server_ip = server_ip_item && cJSON_IsString(server_ip_item) ? strdup(server_ip_item->valuestring) : strdup("");

    cJSON *speech_play_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "speech_play");
    device_setting->speech_play = speech_play_item && cJSON_IsString(speech_play_item) ? strdup(speech_play_item->valuestring) : strdup("");

    cJSON *store_name_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "store_name");
    device_setting->store_name = store_name_item && cJSON_IsString(store_name_item) ? strdup(store_name_item->valuestring) : strdup("");

    cJSON *store_id_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "store_id");
    device_setting->store_id = store_id_item && cJSON_IsString(store_id_item) ? strdup(store_id_item->valuestring) : strdup("");

    cJSON *show_msg_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "show_msg");
    device_setting->show_msg = show_msg_item && cJSON_IsString(show_msg_item) ? strdup(show_msg_item->valuestring) : strdup("");

    cJSON *pos_version_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "pos_version");
    device_setting->pos_version = pos_version_item && cJSON_IsString(pos_version_item) ? strdup(pos_version_item->valuestring) : strdup("");

    cJSON *video_muted_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "video_muted");
    device_setting->video_muted = video_muted_item && cJSON_IsString(video_muted_item) ? strdup(video_muted_item->valuestring) : strdup("");

    cJSON *show_led_id_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "show_led_id");
    device_setting->show_led_id = show_led_id_item && cJSON_IsString(show_led_id_item) ? strdup(show_led_id_item->valuestring) : strdup("");

    cJSON *device_tpl_id_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "device_tpl_id");
    device_setting->device_tpl_id = device_tpl_id_item && cJSON_IsString(device_tpl_id_item) ? strdup(device_tpl_id_item->valuestring) : strdup("");

    cJSON *swiper_time_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "swiper_time");
    device_setting->swiper_time = swiper_time_item && cJSON_IsString(swiper_time_item) ? strdup(swiper_time_item->valuestring) : strdup("");

    cJSON *other_1_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "other_1");
    device_setting->other_1 = other_1_item && cJSON_IsString(other_1_item) ? strdup(other_1_item->valuestring) : strdup("");

    cJSON *other_2_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "other_2");
    device_setting->other_2 = other_2_item && cJSON_IsString(other_2_item) ? strdup(other_2_item->valuestring) : strdup("");

    cJSON *other_3_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "other_3");
    device_setting->other_3 = other_3_item && cJSON_IsString(other_3_item) ? strdup(other_3_item->valuestring) : strdup("");

    cJSON *other_4_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "other_4");
    device_setting->other_4 = other_4_item && cJSON_IsString(other_4_item) ? strdup(other_4_item->valuestring) : strdup("");

    cJSON *other_5_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "other_5");
    device_setting->other_5 = other_5_item && cJSON_IsString(other_5_item) ? strdup(other_5_item->valuestring) : strdup("");

    cJSON *other_6_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "other_6");
    device_setting->other_6 = other_6_item && cJSON_IsString(other_6_item) ? strdup(other_6_item->valuestring) : strdup("");

    cJSON *merchant_id_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "merchant_id");
    device_setting->merchant_id = merchant_id_item && cJSON_IsNumber(merchant_id_item) ? merchant_id_item->valueint : 0;

    cJSON *local_up_tag_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "local_up_tag");
    device_setting->local_up_tag = local_up_tag_item && cJSON_IsString(local_up_tag_item) ? strdup(local_up_tag_item->valuestring) : strdup("");

    cJSON *branch_no_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "branch_no");
    device_setting->branch_no = branch_no_item && cJSON_IsString(branch_no_item) ? strdup(branch_no_item->valuestring) : strdup("");

    cJSON *branch_name_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "branch_name");
    device_setting->branch_name = branch_name_item && cJSON_IsString(branch_name_item) ? strdup(branch_name_item->valuestring) : strdup("");

    cJSON *wifi_SSID_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "wifi_SSID");
    device_setting->wifi_SSID = wifi_SSID_item && cJSON_IsString(wifi_SSID_item) ? strdup(wifi_SSID_item->valuestring) : strdup("");

    cJSON *wifi_pwd_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "wifi_pwd");
    device_setting->wifi_pwd = wifi_pwd_item && cJSON_IsString(wifi_pwd_item) ? strdup(wifi_pwd_item->valuestring) : strdup("");

    cJSON *screen_direction_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "screen_direction");
    device_setting->screen_direction = screen_direction_item && cJSON_IsString(screen_direction_item) ? strdup(screen_direction_item->valuestring) : strdup("");

    cJSON *video_url_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "video_url");
    device_setting->video_url = video_url_item && cJSON_IsString(video_url_item) ? strdup(video_url_item->valuestring) : strdup("");

    cJSON *wxwork_role_item = cJSON_GetObjectItemCaseSensitive(template_data_json, "wxwork_role");
    device_setting->wxwork_role = wxwork_role_item && cJSON_IsString(wxwork_role_item) ? strdup(wxwork_role_item->valuestring) : strdup("");

    cJSON_Delete(template_data_json);
    free(data_str);
    return device_setting;
}

esl_device_setting_t * query_device_setting(const char * device_no, const char *type , const char * sign){
    printf("query_device_setting! device_no:%s,type:%s,sign:%s\n",device_no,type,sign);
    char *response_content = _post_remote_url_with_form_only_esl(KbaseUrl,device_no,type,sign,APP_ID,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    if (response_content == NULL) {
        return NULL;
    }
    cJSON *device_setting_json = cJSON_Parse(response_content);
    if (device_setting_json == NULL) {
        printf("Failed to parse device_setting JSON.\n");
        free(response_content);
        return NULL;
    }
    cJSON *stateItem = cJSON_GetObjectItem(device_setting_json, "State");
    if (stateItem == NULL || !cJSON_IsString(stateItem) || strcmp(stateItem->valuestring,"Done") != 0) {
        printf("Failed to get device_setting JSON State.\n");
        cJSON_Delete(device_setting_json);
        free(response_content);
        return NULL;
    }
    cJSON *dataItem = cJSON_GetObjectItem(device_setting_json, "Data");
    esl_device_setting_t * device_setting = parse_device_setting(dataItem);
    cJSON_Delete(device_setting_json);
    free(response_content);
    return device_setting;
}

void push_online_report(const char * device_no,const char * type,const char * device_list,const char * time,const char * sign){
    printf("push_online_report! device_no:%s,type:%s,device_list:%s,time:%s,sign:%s\n",device_no,type,device_list,time,sign);
    char *response_content = _post_remote_url_with_form_only_esl(KbaseUrl,device_no,type,sign,APP_ID,NULL,NULL,device_list,time,NULL,NULL,NULL);
    printf("response_content:%s\n",response_content);
    if(response_content != NULL){
        free(response_content);
    }
}

void push_message_report(const char * device_no, const char *type, const char *push_id, const char *push_type, const char *time,const char * sign){
    printf("push_message_report! device_no:%s,type:%s,push_id:%s,push_type:%s,time:%s,sign:%s\n",device_no,type,push_id,push_type,time,sign);
    char *response_content = _post_remote_url_with_form_only_esl(KbaseUrl,device_no,type,sign,APP_ID,NULL,NULL,NULL,time,NULL,push_id,push_type);
    printf("response_content:%s\n",response_content);
    if(response_content != NULL){
        free(response_content);
    }
}


void free_esl_push_server_data(esl_push_server_data *data) {
    if (data) {
        if (data->update_version) free(data->update_version);
        if (data->update_url) free(data->update_url);
        if (data->update_file_md5) free(data->update_file_md5);
        if (data->boot_logo_url) free(data->boot_logo_url);
        if (data->boot_logo_md5) free(data->boot_logo_md5);
        if (data->bind_logo_url) free(data->bind_logo_url);
        if (data->bind_logo_md5) free(data->bind_logo_md5);
        if (data->host) free(data->host);
        if (data->username) free(data->username);
        if (data->password) free(data->password);
        if (data->protocol) free(data->protocol);
        if (data->client_id) free(data->client_id);
        if (data->topic) free(data->topic);
        free(data);
    }
}

static void _free_esl_product_info(esl_product_info_t *product_info) {
    if (product_info == NULL) {
        return;
    }

    if (product_info->item_no) free(product_info->item_no);
    if (product_info->item_subno) free(product_info->item_subno);
    if (product_info->item_name) free(product_info->item_name);
    if (product_info->item_subname) free(product_info->item_subname);
    if (product_info->main_supcust) free(product_info->main_supcust);
    if (product_info->item_clsno) free(product_info->item_clsno);
    if (product_info->combine_sta) free(product_info->combine_sta);
    if (product_info->item_sup_flag) free(product_info->item_sup_flag);
    if (product_info->item_stock) free(product_info->item_stock);
    if (product_info->price) free(product_info->price);
    if (product_info->sale_price) free(product_info->sale_price);
    if (product_info->vip_price) free(product_info->vip_price);
    if (product_info->item_rem) free(product_info->item_rem);
    if (product_info->unit_no) free(product_info->unit_no);
    if (product_info->item_size) free(product_info->item_size);
    if (product_info->product_area) free(product_info->product_area);
    if (product_info->build_date) free(product_info->build_date);
    if (product_info->modify_date) free(product_info->modify_date);
    if (product_info->item_url) free(product_info->item_url);
    if (product_info->status) free(product_info->status);
    if (product_info->item_video) free(product_info->item_video);
    if (product_info->main_supname) free(product_info->main_supname);

    free(product_info);
}

void free_esl_product_info_array(esl_product_info_t **product_info_list, int count) {
    if (product_info_list == NULL) {
        return;
    }

    for (int i = 0; i < count; i++) {
        _free_esl_product_info(product_info_list[i]);
    }

    free(product_info_list);
}

void free_esl_show_model(esl_show_model_t *model) {
    if (model == NULL) {
        return;
    }

    if (model->name) free(model->name);
    if (model->resolution) free(model->resolution);
    if (model->type) free(model->type);
    if (model->thumbnail) free(model->thumbnail);

    for (int i = 0; i < model->child_count; i++) {
        esl_show_model_child_t *child = &model->child_list[i];
        char *type = child->type;
        if (strcmp(type, "image") == 0) {
            if (child->desc.src) free(child->desc.src);
            if (child->extension.swiper_list) {
                for (int j = 0; j < child->extension.swiper_list_count; j++) {
                    if (child->extension.swiper_list[j]) free(child->extension.swiper_list[j]);
                }
                free(child->extension.swiper_list);
            }
            if (child->extension.value) free(child->extension.value);
        } else if (strcmp(type, "text") == 0) {
            if (child->desc.text) free(child->desc.text);
            if (child->desc.color) free(child->desc.color);
            if (child->desc.fontSize) free(child->desc.fontSize);
            if (child->desc.fontWeight) free(child->desc.fontWeight);
            if (child->desc.textAlign) free(child->desc.textAlign);
            if (child->desc.fontFamily) free(child->desc.fontFamily);
            if (child->desc.bgColor) free(child->desc.bgColor);
            if (child->extension.textDecoration) free(child->extension.textDecoration);
        } else if (strcmp(type, "rect") == 0 || strcmp(type, "circle") == 0) {
            if (child->desc.bgColor) free(child->desc.bgColor);
        } else if (strcmp(type, "html") == 0 ){
            if (child->extension.html.text) free(child->extension.html.text);
            if (child->extension.html.color) free(child->extension.html.color);
            if (child->extension.html.fontWeight) free(child->extension.html.fontWeight);
            if (child->extension.html.textAlign) free(child->extension.html.textAlign);
            if (child->extension.html.fontFamily) free(child->extension.html.fontFamily);
            if (child->extension.html.numFloatStyle) free(child->extension.html.numFloatStyle);
            if (child->extension.html.fontStyle) free(child->extension.html.fontStyle);
            if (child->extension.html.textDecoration) free(child->extension.html.textDecoration);
            if (child->extension.html.bgColor) free(child->extension.html.bgColor);
            if (child->extension.unit_price) free(child->extension.unit_price);
        }

        if (child->extension.ex_type) free(child->extension.ex_type);
        if (child->extension.bind) free(child->extension.bind);
        if (child->name) free(child->name);
        if (child->type) free(child->type);
    }

    if (model->child_list) free(model->child_list);
    free(model);
}

void free_device_setting(esl_device_setting_t *device_setting) {
    if (device_setting == NULL) {
        return;
    }

    if (device_setting->SN) free(device_setting->SN);
    if (device_setting->pos_ip) free(device_setting->pos_ip);
    if (device_setting->pos_id) free(device_setting->pos_id);
    if (device_setting->server_ip) free(device_setting->server_ip);
    if (device_setting->speech_play) free(device_setting->speech_play);
    if (device_setting->store_name) free(device_setting->store_name);
    if (device_setting->store_id) free(device_setting->store_id);
    if (device_setting->show_msg) free(device_setting->show_msg);
    if (device_setting->pos_version) free(device_setting->pos_version);
    if (device_setting->video_muted) free(device_setting->video_muted);
    if (device_setting->show_led_id) free(device_setting->show_led_id);
    if (device_setting->device_tpl_id) free(device_setting->device_tpl_id);
    if (device_setting->swiper_time) free(device_setting->swiper_time);
    if (device_setting->other_1) free(device_setting->other_1);
    if (device_setting->other_2) free(device_setting->other_2);
    if (device_setting->other_3) free(device_setting->other_3);
    if (device_setting->other_4) free(device_setting->other_4);
    if (device_setting->other_5) free(device_setting->other_5);
    if (device_setting->other_6) free(device_setting->other_6);
    if (device_setting->local_up_tag) free(device_setting->local_up_tag);
    if (device_setting->branch_no) free(device_setting->branch_no);
    if (device_setting->branch_name) free(device_setting->branch_name);
    if (device_setting->wifi_SSID) free(device_setting->wifi_SSID);
    if (device_setting->wifi_pwd) free(device_setting->wifi_pwd);
    if (device_setting->screen_direction) free(device_setting->screen_direction);
    if (device_setting->video_url) free(device_setting->video_url);
    if (device_setting->wxwork_role) free(device_setting->wxwork_role);

    free(device_setting);
}

void set_bg(lv_obj_t * bg,int index){
    if(index == 1){
        LV_LOG_USER("set_bg 1. bg=%p\n",bg);
        g_esl_global_model.bg_first = bg;
    }else{
        LV_LOG_USER("set_bg 2. bg=%p\n",bg);
        g_esl_global_model.bg_second = bg;
    }
}

void set_top(lv_obj_t * top,int index){
    if(index == 1){
        LV_LOG_USER("set_top 1. top=%p\n",top);
        if (g_esl_global_model.top_first == NULL){
            g_esl_global_model.top_first = top;
        }
    }else{
        LV_LOG_USER("set_top 2. top=%p\n",top);
        if (g_esl_global_model.top_second == NULL){
            g_esl_global_model.top_second = top;
        }
    }
}

void set_ip(char * ip){
    g_esl_global_model.ip = ip;
}

/* void set_product_info_list(esl_product_info_t ** product_info_list,int index){
    if(index == 1){
        if(g_esl_global_model.product_info_list_first != NULL){
            free_esl_product_info_array(g_esl_global_model.product_info_list_first,g_esl_global_model.product_count_first);
        }
        g_esl_global_model.product_info_list_first = product_info_list;
    }else{
        if(g_esl_global_model.product_info_list_second != NULL){
            free_esl_product_info_array(g_esl_global_model.product_info_list_second,g_esl_global_model.product_count_second);
        }
        g_esl_global_model.product_info_list_second = product_info_list;
    }
}

void set_product_model(esl_show_model_t * product_model,int index){
    if(index == 1){
        if(g_esl_global_model.product_model_first != NULL){
            free_esl_show_model(g_esl_global_model.product_model_first);
        }
        g_esl_global_model.product_model_first = product_model;
    }else{
        if(g_esl_global_model.product_model_second != NULL){
            free_esl_show_model(g_esl_global_model.product_model_second);
        }
        g_esl_global_model.product_model_second = product_model;
    }
}

void set_product_count(int product_count,int index){
    if(index == 1){
        g_esl_global_model.product_count_first = product_count;
    }else{
        g_esl_global_model.product_count_second = product_count;
    }
}

void set_device_setting(esl_device_setting_t *device_setting, int index){
    if(index == 1){
        if(g_esl_global_model.device_setting_first != NULL){
            free_device_setting(g_esl_global_model.device_setting_first);
        }
        g_esl_global_model.device_setting_first = device_setting;
    }else{
        if(g_esl_global_model.device_setting_second != NULL){
            free_device_setting(g_esl_global_model.device_setting_second);
        }
        g_esl_global_model.device_setting_second = device_setting;
    }
} */

void set_device_sn(char * device_sn,int index){
    if(index == 1){
        if(g_esl_global_model.device_sn_first == NULL){
            g_esl_global_model.device_sn_first = strdup(device_sn);
        }
    }else{
        if(g_esl_global_model.device_sn_second == NULL){
            g_esl_global_model.device_sn_second = strdup(device_sn);
        }
    }
}

void set_master_device_sn(char * master_device_sn){
    g_esl_global_model.master_device_sn = master_device_sn;
}

lv_obj_t * get_bg(int index){
    if(index == 1){
        return g_esl_global_model.bg_first;
    }else{
        return g_esl_global_model.bg_second;
    }
}
lv_obj_t * get_top(int index){
    if(index == 1){
        return g_esl_global_model.top_first;
    }else{
        return g_esl_global_model.top_second;
    }
}
/* esl_product_info_t ** get_product_info_list(int index){
    if(index == 1){
        return g_esl_global_model.product_info_list_first;
    }else{
        return g_esl_global_model.product_info_list_second;
    }
}
int get_product_count(int index){
    if(index == 1){
        return g_esl_global_model.product_count_first;
    }else{
        return g_esl_global_model.product_count_second;
    }
}
esl_show_model_t * get_product_model(int index){
    if(index == 1){
        return g_esl_global_model.product_model_first;
    }else{
        return g_esl_global_model.product_model_second;
    }
}

esl_device_setting_t * get_device_setting(int index){
    if(index == 1){
        return g_esl_global_model.device_setting_first;
    }else{
        return g_esl_global_model.device_setting_second;
    }
} */

char * get_device_sn(int index){
    if(index == 1){
        return g_esl_global_model.device_sn_first;
    }else{
        return g_esl_global_model.device_sn_second;
    }
}

char * get_master_device_sn(){
    return g_esl_global_model.master_device_sn;
}

static void _refresh_ui(int index,esl_product_info_t ** product_info_list,int count,esl_show_model_t * model,esl_device_setting_t * device_setting){
    lv_obj_t * bg = get_bg(index);
    lv_obj_t * top = get_top(index);
    if(model != NULL && product_info_list != NULL){
        if(bg != NULL && lv_obj_is_valid(bg)){
            lv_obj_delete(bg);
            set_bg(NULL,index);
        }
        async_show_ui(model,device_setting,product_info_list,count,top,index);
    }
}

void* reboot_thread(void* arg) {
    sleep(10);
    system("reboot");
    return NULL;
}

static void _delete_ui_and_local_data(int index,int need_restart){
    lv_obj_t * bg = get_bg(index);
    lv_obj_t * top = get_top(index);
    char * device_sn = get_device_sn(index);
    if(bg != NULL && lv_obj_is_valid(bg)){
        lv_obj_delete(bg);
        set_bg(NULL,index);
    }
    int display_id = index - 1;
    los_videoplayer_quit(display_id);
    //删除本地信息
    delete_local_data(index,device_sn);
    if(index == 1){
        if(logo_image_is_exist(k_path_boot_bg_img_1)){
            delete_local_logo_image(index);
            #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
            extern int xos_logo_reset(uint8_t index);
            xos_logo_reset(index - 1);
            #endif
        }
    }else if(index == 2){
        if(logo_image_is_exist(k_path_boot_bg_img_2)){
            delete_local_logo_image(index);
            #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
            extern int xos_logo_reset(uint8_t index);
            xos_logo_reset(index - 1);
            #endif
        }
    }
    qme_set_render_status(display_id, false);
    if (index == 1) {
        set_local_group_id_1(0);
        show_empty_ui_1(top);
    } else if (index == 2) {
        set_local_group_id_2(0);
        show_empty_ui_2(top);
    }
    qme_set_render_status(display_id, true);
    if(need_restart  == 1){
        char * master_device_sn = get_master_device_sn();
        LV_LOG_USER("================master_device_sn:%s,device_sn:%s  ===============",master_device_sn,device_sn);
        if(!is_string_empty(master_device_sn) && strcmp(master_device_sn,device_sn) == 0){
            LV_LOG_USER("==========true======master_device_sn:%s,device_sn:%s  ===============",master_device_sn,device_sn);
            system("reboot");
        }else{
            LV_LOG_USER("=========false=======master_device_sn:%s,device_sn:%s  ===============",master_device_sn,device_sn);
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, reboot_thread, NULL);
            pthread_detach(thread_id);
        }
    }
}

void refresh_ui_1(void *user_data){
    show_ui_args_t *args = (show_ui_args_t *)user_data;
    _refresh_ui(1,args->product_info_list,args->count,args->model,args->device_setting);
    free(args);
}

void delete_ui_and_local_data_1(void *user_data){
    if(user_data == NULL){
        _delete_ui_and_local_data(1,1);
    }else{
        _delete_ui_and_local_data(1,0);
    }
}

void refresh_ui_2(void *user_data){
    show_ui_args_t *args = (show_ui_args_t *)user_data;
    _refresh_ui(2,args->product_info_list,args->count,args->model,args->device_setting);
    free(args);
}

void delete_ui_and_local_data_2(void *user_data){
    if(user_data == NULL){
        _delete_ui_and_local_data(2,1);
    }else{
        _delete_ui_and_local_data(2,0);
    }
}

void update_product_info_batch(esl_product_info_t ** new_infos, int new_count, esl_product_info_t ** old_infos, int old_count) {
    if (new_infos == NULL || old_infos == NULL || new_count <= 0 || old_count <= 0) {
        LV_LOG_WARN("update_product_info_batch: new_infos or old_infos is NULL");
        return;
    }

    for (int i = 0; i < new_count; i++) {
        if (new_infos[i] == NULL) {
            LV_LOG_WARN("update_product_info_batch: new_infos[%d] is NULL", i);
            continue;
        }

        for (int j = 0; j < old_count; j++) {
            if (old_infos[j] == NULL) {
                LV_LOG_WARN("update_product_info_batch: old_infos[%d] is NULL", j);
                continue;
            }

            if (strcmp(new_infos[i]->item_no, old_infos[j]->item_no) == 0) {
                // 释放老的数据
                if (old_infos[j]->item_no) free(old_infos[j]->item_no);
                if (old_infos[j]->item_subno) free(old_infos[j]->item_subno);
                if (old_infos[j]->item_name) free(old_infos[j]->item_name);
                if (old_infos[j]->item_subname) free(old_infos[j]->item_subname);
                if (old_infos[j]->main_supcust) free(old_infos[j]->main_supcust);
                if (old_infos[j]->item_clsno) free(old_infos[j]->item_clsno);
                if (old_infos[j]->combine_sta) free(old_infos[j]->combine_sta);
                if (old_infos[j]->item_sup_flag) free(old_infos[j]->item_sup_flag);
                if (old_infos[j]->item_stock) free(old_infos[j]->item_stock);
                if (old_infos[j]->price) free(old_infos[j]->price);
                if (old_infos[j]->sale_price) free(old_infos[j]->sale_price);
                if (old_infos[j]->vip_price) free(old_infos[j]->vip_price);
                if (old_infos[j]->item_rem) free(old_infos[j]->item_rem);
                if (old_infos[j]->unit_no) free(old_infos[j]->unit_no);
                if (old_infos[j]->item_size) free(old_infos[j]->item_size);
                if (old_infos[j]->product_area) free(old_infos[j]->product_area);
                if (old_infos[j]->build_date) free(old_infos[j]->build_date);
                if (old_infos[j]->modify_date) free(old_infos[j]->modify_date);
                if (old_infos[j]->item_url) free(old_infos[j]->item_url);
                if (old_infos[j]->status) free(old_infos[j]->status);
                if (old_infos[j]->item_video) free(old_infos[j]->item_video);
                if (old_infos[j]->main_supname) free(old_infos[j]->main_supname);

                // 覆盖老的数据，逐个属性复制
                old_infos[j]->item_no = new_infos[i]->item_no ? strdup(new_infos[i]->item_no) : strdup("");
                old_infos[j]->item_subno = new_infos[i]->item_subno ? strdup(new_infos[i]->item_subno) : strdup("");
                old_infos[j]->item_name = new_infos[i]->item_name ? strdup(new_infos[i]->item_name) : strdup("");
                old_infos[j]->item_subname = new_infos[i]->item_subname ? strdup(new_infos[i]->item_subname) : strdup("");
                old_infos[j]->main_supcust = new_infos[i]->main_supcust ? strdup(new_infos[i]->main_supcust) : strdup("");
                old_infos[j]->item_clsno = new_infos[i]->item_clsno ? strdup(new_infos[i]->item_clsno) : strdup("");
                old_infos[j]->combine_sta = new_infos[i]->combine_sta ? strdup(new_infos[i]->combine_sta) : strdup("");
                old_infos[j]->item_sup_flag = new_infos[i]->item_sup_flag ? strdup(new_infos[i]->item_sup_flag) : strdup("");
                old_infos[j]->item_stock = new_infos[i]->item_stock ? strdup(new_infos[i]->item_stock) : strdup("");
                old_infos[j]->price = new_infos[i]->price ? strdup(new_infos[i]->price) : strdup("0.00");
                old_infos[j]->base_price = new_infos[i]->base_price;
                old_infos[j]->sale_price = new_infos[i]->sale_price ? strdup(new_infos[i]->sale_price) : strdup("0.00");
                old_infos[j]->vip_price = new_infos[i]->vip_price ? strdup(new_infos[i]->vip_price) : strdup("0.00");
                old_infos[j]->num2 = new_infos[i]->num2;
                old_infos[j]->num3 = new_infos[i]->num3;
                old_infos[j]->item_rem = new_infos[i]->item_rem ? strdup(new_infos[i]->item_rem) : strdup("");
                old_infos[j]->unit_no = new_infos[i]->unit_no ? strdup(new_infos[i]->unit_no) : strdup("");
                old_infos[j]->item_size = new_infos[i]->item_size ? strdup(new_infos[i]->item_size) : strdup("");
                old_infos[j]->product_area = new_infos[i]->product_area ? strdup(new_infos[i]->product_area) : strdup("");
                old_infos[j]->build_date = new_infos[i]->build_date ? strdup(new_infos[i]->build_date) : strdup("");
                old_infos[j]->modify_date = new_infos[i]->modify_date ? strdup(new_infos[i]->modify_date) : strdup("");
                old_infos[j]->item_url = new_infos[i]->item_url ? strdup(new_infos[i]->item_url) : strdup("");
                old_infos[j]->purchase_spec = new_infos[i]->purchase_spec;
                old_infos[j]->status = new_infos[i]->status ? strdup(new_infos[i]->status) : strdup("");
                old_infos[j]->min_opqty = new_infos[i]->min_opqty;
                old_infos[j]->item_video = new_infos[i]->item_video ? strdup(new_infos[i]->item_video) : strdup("");
                old_infos[j]->main_supname = new_infos[i]->main_supname ? strdup(new_infos[i]->main_supname) : strdup("");

                free_esl_product_info_array(new_infos, new_count);
                break; // 假设每个item_no是唯一的，找到匹配后可以跳出内层循环
            }
        }
    }
}

#endif
