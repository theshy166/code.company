#ifdef CONFIG_XOS_USE_APP_NATIVE_ESL
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>
#include "esl_file.h"
#include "cJSON.h"
#include "esl_service.h"
#include "../conf/conf.h"
#include "../utils/utils.h"


// 判断文件是否存在，存在则删除
static void _check_and_delete_file(const char *filename) {
    // 检查文件是否存在
    if (access(filename, F_OK) == 0) {
        // 文件存在，删除文件
        if (remove(filename) == 0) {
            printf("File deleted successfully: %s\n", filename);
        } else {
            printf("Failed to delete file: %s\n", filename);
        }
    } else {
        printf("File does not exist: %s\n", filename);
    }
}

// 读取 JSON 文件内容并返回字符串
static char* _read_json_file(const char *filename) {
    // 检查文件是否存在
    if (access(filename, F_OK) != 0) {
        printf("File does not exist: %s\n", filename);
        return NULL;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return NULL;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 分配内存以存储文件内容
    char *content = (char *)malloc(file_size + 1);
    if (content == NULL) {
        printf("Failed to allocate memory for file content\n");
        fclose(file);
        return NULL;
    }

    // 读取文件内容
    size_t read_size = fread(content, 1, file_size, file);
    if (read_size != file_size) {
        printf("Failed to read file content\n");
        free(content);
        fclose(file);
        return NULL;
    }

    // 添加字符串终止符
    content[file_size] = '\0';

    fclose(file);
    return content;
}


// 将数据转换为JSON并保存到文件
static void _save_json_to_file(const char *filename, cJSON *json) {
    char *json_str = cJSON_PrintUnformatted(json);
    if (json_str == NULL) {
        printf("Failed to print JSON to string\n");
        return;
    }

    // 检查文件是否存在
    if (access(filename, F_OK) == 0) {
        // 文件存在，删除旧的文件
        if (remove(filename) != 0) {
            printf("Failed to delete old file: %s\n", filename);
            free(json_str);
            return;
        }
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        free(json_str);
        return;
    }

    fprintf(file, "%s", json_str);
    fclose(file);
    free(json_str);
}

static int _save_template_model_data(esl_file_save_info_t *args){
    // 转换 model 为 JSON
    cJSON *model_json = cJSON_CreateObject();
    if (model_json == NULL) {
        printf("Failed to create JSON object for model\n");
        return 0;
    }

    cJSON_AddStringToObject(model_json, "name", args->model->name);
    cJSON_AddStringToObject(model_json, "resolution", args->model->resolution);
    cJSON_AddStringToObject(model_json, "type", args->model->type);
    cJSON_AddNumberToObject(model_json, "itemNum", args->model->itemNum);
    cJSON_AddNumberToObject(model_json, "status", args->model->status);
    cJSON_AddStringToObject(model_json, "thumbnail", args->model->thumbnail);
    cJSON_AddNumberToObject(model_json, "width", args->model->width);
    cJSON_AddNumberToObject(model_json, "height", args->model->height);
    cJSON_AddNumberToObject(model_json, "contextWidth", args->model->contextWidth);
    cJSON_AddNumberToObject(model_json, "contextHeight", args->model->contextHeight);
    cJSON_AddNumberToObject(model_json, "devicePixelRatio", args->model->devicePixelRatio);
    cJSON_AddNumberToObject(model_json, "onlyRender", args->model->onlyRender);

    // 转换 child_list 为 JSON
    cJSON *child_list_json = cJSON_CreateArray();
    if (child_list_json == NULL) {
        printf("Failed to create JSON array for child_list\n");
        cJSON_Delete(model_json);
        return 0;
    }

    for (int i = 0; i < args->model->child_count; i++) {
        cJSON *child_item = cJSON_CreateObject();
        if (child_item == NULL) {
            printf("Failed to create JSON object for child_item\n");
            continue;
        }
        cJSON_AddStringToObject(child_item, "name", args->model->child_list[i].name);
        cJSON_AddNumberToObject(child_item, "x", args->model->child_list[i].x);
        cJSON_AddNumberToObject(child_item, "y", args->model->child_list[i].y);
        cJSON_AddNumberToObject(child_item, "w", args->model->child_list[i].w);
        cJSON_AddNumberToObject(child_item, "h", args->model->child_list[i].h);
        cJSON_AddNumberToObject(child_item, "angle", args->model->child_list[i].angle);
        cJSON_AddStringToObject(child_item, "type", args->model->child_list[i].type);
        // 转换 desc 为 JSON
        cJSON *desc_json = cJSON_CreateObject();
        if (desc_json == NULL) {
            printf("Failed to create JSON object for desc\n");
            cJSON_Delete(child_item);
            continue;
        }
        
        // 转换 extension 为 JSON
        cJSON *extension_json = cJSON_CreateObject();
        if (extension_json == NULL) {
            printf("Failed to create JSON object for extension\n");
            cJSON_Delete(child_item);
            continue;
        }
        // 转换 html 为 JSON
        cJSON *html_json = cJSON_CreateObject();
        if (html_json == NULL) {
            printf("Failed to create JSON object for html\n");
            cJSON_Delete(extension_json);
            continue;
        }
        if (strcmp(args->model->child_list[i].type, "image") == 0) {
            // 转换 swiper_list 为 JSON
            cJSON *swiper_list_json = cJSON_CreateArray();
            if (swiper_list_json == NULL) {
                printf("Failed to create JSON array for swiper_list\n");
                cJSON_Delete(extension_json);
                continue;
            }
            cJSON_AddNumberToObject(desc_json, "borderRadius", args->model->child_list[i].desc.borderRadius);
            cJSON_AddNumberToObject(desc_json, "borderWidth", args->model->child_list[i].desc.borderWidth);
            cJSON_AddStringToObject(extension_json, "ex_type", args->model->child_list[i].extension.ex_type);
            cJSON_AddStringToObject(extension_json, "bind", args->model->child_list[i].extension.bind);
            cJSON_AddStringToObject(desc_json, "src", args->model->child_list[i].desc.src);
            for (int j = 0; j < args->model->child_list[i].extension.swiper_list_count; j++) {
                cJSON_AddItemToArray(swiper_list_json, cJSON_CreateString(args->model->child_list[i].extension.swiper_list[j]));
            }
            cJSON_AddItemToObject(extension_json, "swiper_list", swiper_list_json);
            cJSON_AddStringToObject(extension_json, "value", args->model->child_list[i].extension.value);
        }else if (strcmp(args->model->child_list[i].type, "text") == 0) {
            cJSON_AddStringToObject(desc_json, "text", args->model->child_list[i].desc.text);
            cJSON_AddStringToObject(desc_json, "color", args->model->child_list[i].desc.color);
            cJSON_AddStringToObject(desc_json, "fontSize", args->model->child_list[i].desc.fontSize);
            cJSON_AddStringToObject(desc_json, "fontWeight", args->model->child_list[i].desc.fontWeight);
            cJSON_AddStringToObject(desc_json, "textAlign", args->model->child_list[i].desc.textAlign);
            cJSON_AddStringToObject(desc_json, "fontFamily", args->model->child_list[i].desc.fontFamily);
            cJSON_AddStringToObject(desc_json, "bgColor", args->model->child_list[i].desc.bgColor);
            cJSON_AddNumberToObject(desc_json, "borderRadius", args->model->child_list[i].desc.borderRadius);
            cJSON_AddNumberToObject(desc_json, "borderWidth", args->model->child_list[i].desc.borderWidth);
            cJSON_AddStringToObject(extension_json, "ex_type", args->model->child_list[i].extension.ex_type);
            cJSON_AddStringToObject(extension_json, "bind", args->model->child_list[i].extension.bind);
            cJSON_AddStringToObject(extension_json, "textDecoration", args->model->child_list[i].extension.textDecoration);
        }else if (strcmp(args->model->child_list[i].type, "rect") == 0 || strcmp(args->model->child_list[i].type, "circle") == 0) {
            cJSON_AddStringToObject(desc_json, "bgColor", args->model->child_list[i].desc.bgColor);
            cJSON_AddNumberToObject(desc_json, "borderRadius", args->model->child_list[i].desc.borderRadius);
            cJSON_AddNumberToObject(desc_json, "borderWidth", args->model->child_list[i].desc.borderWidth);
            cJSON_AddStringToObject(extension_json, "ex_type", args->model->child_list[i].extension.ex_type);
            cJSON_AddStringToObject(extension_json, "bind", args->model->child_list[i].extension.bind);
        }else if (strcmp(args->model->child_list[i].type, "html") == 0 ){
            cJSON_AddStringToObject(extension_json, "ex_type", args->model->child_list[i].extension.ex_type);
            cJSON_AddStringToObject(extension_json, "bind", args->model->child_list[i].extension.bind);
            cJSON_AddStringToObject(extension_json, "unit_price", args->model->child_list[i].extension.unit_price);
            cJSON_AddStringToObject(html_json, "text", args->model->child_list[i].extension.html.text);
            cJSON_AddStringToObject(html_json, "color", args->model->child_list[i].extension.html.color);
            cJSON_AddNumberToObject(html_json, "intFontSize", args->model->child_list[i].extension.html.intFontSize);
            cJSON_AddNumberToObject(html_json, "floatFontSize", args->model->child_list[i].extension.html.floatFontSize);
            cJSON_AddStringToObject(html_json, "fontWeight", args->model->child_list[i].extension.html.fontWeight);
            cJSON_AddStringToObject(html_json, "fontFamily", args->model->child_list[i].extension.html.fontFamily);
            cJSON_AddStringToObject(html_json, "textAlign", args->model->child_list[i].extension.html.textAlign);
            cJSON_AddStringToObject(html_json, "numFloatStyle", args->model->child_list[i].extension.html.numFloatStyle);
            cJSON_AddStringToObject(html_json, "fontStyle", args->model->child_list[i].extension.html.fontStyle);
            cJSON_AddStringToObject(html_json, "textDecoration", args->model->child_list[i].extension.html.textDecoration);
            cJSON_AddStringToObject(html_json, "bgColor", args->model->child_list[i].extension.html.bgColor);
        }
        cJSON_AddItemToObject(extension_json, "html", html_json);
        cJSON_AddItemToObject(child_item, "extension", extension_json);
        cJSON_AddItemToObject(child_item, "desc", desc_json);
        cJSON_AddItemToArray(child_list_json, child_item);
    }
    cJSON_AddItemToObject(model_json, "child_list", child_list_json);

    cJSON *data_json = cJSON_CreateObject();
    if (data_json == NULL) {
        printf("Failed to create JSON object for data_json\n");
        cJSON_Delete(model_json);
        return 0;
    }
    char * model_json_str = cJSON_PrintUnformatted(model_json);
    cJSON_AddStringToObject(data_json,"data",model_json_str);

    char model_filename[256];
    snprintf(model_filename, sizeof(model_filename), k_path_json_product_model, args->device_sn);
    _save_json_to_file(model_filename, data_json);

    free(model_json_str);
    cJSON_Delete(data_json);
    cJSON_Delete(model_json);

    return 1;
}

static int _save_product_info_data(esl_file_save_info_t *args){
    // 转换 product_info_list 为 JSON
    cJSON *product_info_json = cJSON_CreateArray();
    if (product_info_json == NULL) {
        printf("Failed to create JSON array for product_info_list\n");
        return 0;
    }
    printf("args->product_info_count :%d\n",args->product_info_count);
    for (int i = 0; i < args->product_info_count; i++) {
        cJSON *product_info_item = cJSON_CreateObject();
        if (product_info_item == NULL) {
            printf("Failed to create JSON object for product_info_item\n");
            continue;
        }
        // 假设 product_info 有一些字段需要转换为 JSON
        cJSON_AddStringToObject(product_info_item, "item_no", args->product_info_list[i]->item_no);
        cJSON_AddStringToObject(product_info_item, "item_subno", args->product_info_list[i]->item_subno);
        cJSON_AddStringToObject(product_info_item, "item_name", args->product_info_list[i]->item_name);
        cJSON_AddStringToObject(product_info_item, "item_subname", args->product_info_list[i]->item_subname);
        cJSON_AddStringToObject(product_info_item, "main_supcust", args->product_info_list[i]->main_supcust);
        cJSON_AddStringToObject(product_info_item, "item_clsno", args->product_info_list[i]->item_clsno);
        cJSON_AddStringToObject(product_info_item, "combine_sta", args->product_info_list[i]->combine_sta);
        cJSON_AddStringToObject(product_info_item, "item_sup_flag", args->product_info_list[i]->item_sup_flag);
        cJSON_AddStringToObject(product_info_item, "item_stock", args->product_info_list[i]->item_stock);
        cJSON_AddStringToObject(product_info_item, "price", args->product_info_list[i]->price);
        cJSON_AddNumberToObject(product_info_item, "base_price", args->product_info_list[i]->base_price);
        cJSON_AddStringToObject(product_info_item, "sale_price", args->product_info_list[i]->sale_price);
        cJSON_AddStringToObject(product_info_item, "vip_price", args->product_info_list[i]->vip_price);
        cJSON_AddNumberToObject(product_info_item, "num2", args->product_info_list[i]->num2);
        cJSON_AddNumberToObject(product_info_item, "num3", args->product_info_list[i]->num3);
        cJSON_AddStringToObject(product_info_item, "item_rem", args->product_info_list[i]->item_rem);
        cJSON_AddStringToObject(product_info_item, "unit_no", args->product_info_list[i]->unit_no);
        cJSON_AddStringToObject(product_info_item, "item_size", args->product_info_list[i]->item_size);
        cJSON_AddStringToObject(product_info_item, "product_area", args->product_info_list[i]->product_area);
        cJSON_AddStringToObject(product_info_item, "build_date", args->product_info_list[i]->build_date);
        cJSON_AddStringToObject(product_info_item, "modify_date", args->product_info_list[i]->modify_date);
        cJSON_AddStringToObject(product_info_item, "item_url", args->product_info_list[i]->item_url);
        cJSON_AddNumberToObject(product_info_item, "purchase_spec", args->product_info_list[i]->purchase_spec);
        cJSON_AddStringToObject(product_info_item, "status", args->product_info_list[i]->status);
        cJSON_AddNumberToObject(product_info_item, "min_opqty", args->product_info_list[i]->min_opqty);
        cJSON_AddStringToObject(product_info_item, "item_video", args->product_info_list[i]->item_video);
        cJSON_AddStringToObject(product_info_item, "main_supname", args->product_info_list[i]->main_supname);
        cJSON_AddItemToArray(product_info_json, product_info_item);
    }

    char product_info_filename[256];
    snprintf(product_info_filename, sizeof(product_info_filename), k_path_json_product_info, args->device_sn);
    _save_json_to_file(product_info_filename, product_info_json);

    cJSON_Delete(product_info_json);
    return 1;
}

static int _save_device_setting_data(esl_file_save_info_t *args){
    cJSON *device_setting_json = cJSON_CreateObject();
    if (device_setting_json == NULL) {
        printf("Failed to create JSON object for model\n");
        return 0;
    }

    cJSON_AddStringToObject(device_setting_json, "SN", args->device_setting->SN ? args->device_setting->SN : "");
    cJSON_AddStringToObject(device_setting_json, "pos_ip", args->device_setting->pos_ip ? args->device_setting->pos_ip : "");
    cJSON_AddStringToObject(device_setting_json, "pos_id", args->device_setting->pos_id ? args->device_setting->pos_id : "");
    cJSON_AddStringToObject(device_setting_json, "server_ip", args->device_setting->server_ip ? args->device_setting->server_ip : "");
    cJSON_AddStringToObject(device_setting_json, "speech_play", args->device_setting->speech_play ? args->device_setting->speech_play : "");
    cJSON_AddStringToObject(device_setting_json, "store_name", args->device_setting->store_name ? args->device_setting->store_name : "");
    cJSON_AddStringToObject(device_setting_json, "store_id", args->device_setting->store_id ? args->device_setting->store_id : "");
    cJSON_AddStringToObject(device_setting_json, "show_msg", args->device_setting->show_msg ? args->device_setting->show_msg : "");
    cJSON_AddStringToObject(device_setting_json, "pos_version", args->device_setting->pos_version ? args->device_setting->pos_version : "");
    cJSON_AddStringToObject(device_setting_json, "video_muted", args->device_setting->video_muted ? args->device_setting->video_muted : "");
    cJSON_AddStringToObject(device_setting_json, "show_led_id", args->device_setting->show_led_id ? args->device_setting->show_led_id : "");
    cJSON_AddStringToObject(device_setting_json, "device_tpl_id", args->device_setting->device_tpl_id ? args->device_setting->device_tpl_id : "");
    cJSON_AddStringToObject(device_setting_json, "swiper_time", args->device_setting->swiper_time ? args->device_setting->swiper_time : "");
    cJSON_AddStringToObject(device_setting_json, "other_1", args->device_setting->other_1 ? args->device_setting->other_1 : "");
    cJSON_AddStringToObject(device_setting_json, "other_2", args->device_setting->other_2 ? args->device_setting->other_2 : "");
    cJSON_AddStringToObject(device_setting_json, "other_3", args->device_setting->other_3 ? args->device_setting->other_3 : "");
    cJSON_AddStringToObject(device_setting_json, "other_4", args->device_setting->other_4 ? args->device_setting->other_4 : "");
    cJSON_AddStringToObject(device_setting_json, "other_5", args->device_setting->other_5 ? args->device_setting->other_5 : "");
    cJSON_AddStringToObject(device_setting_json, "other_6", args->device_setting->other_6 ? args->device_setting->other_6 : "");
    cJSON_AddNumberToObject(device_setting_json, "merchant_id", args->device_setting->merchant_id);
    cJSON_AddStringToObject(device_setting_json, "local_up_tag", args->device_setting->local_up_tag ? args->device_setting->local_up_tag : "");
    cJSON_AddStringToObject(device_setting_json, "branch_no", args->device_setting->branch_no ? args->device_setting->branch_no : "");
    cJSON_AddStringToObject(device_setting_json, "branch_name", args->device_setting->branch_name ? args->device_setting->branch_name : "");
    cJSON_AddStringToObject(device_setting_json, "wifi_SSID", args->device_setting->wifi_SSID ? args->device_setting->wifi_SSID : "");
    cJSON_AddStringToObject(device_setting_json, "wifi_pwd", args->device_setting->wifi_pwd ? args->device_setting->wifi_pwd : "");
    cJSON_AddStringToObject(device_setting_json, "screen_direction", args->device_setting->screen_direction ? args->device_setting->screen_direction : "");
    cJSON_AddStringToObject(device_setting_json, "video_url", args->device_setting->video_url ? args->device_setting->video_url : "");
    cJSON_AddStringToObject(device_setting_json, "wxwork_role", args->device_setting->wxwork_role ? args->device_setting->wxwork_role : "");

    cJSON *device_setting_data_json = cJSON_CreateObject();
    if (device_setting_data_json == NULL) {
        printf("Failed to create JSON object for data_json\n");
        cJSON_Delete(device_setting_json);
        return 0;
    }
    char * device_setting_str = cJSON_PrintUnformatted(device_setting_json);
    cJSON_AddStringToObject(device_setting_data_json,"data",device_setting_str);

    // 根据 index 保存到不同的文件
    char device_setting_filename[256];
    snprintf(device_setting_filename, sizeof(device_setting_filename), k_path_json_device_setting, args->device_sn);
    _save_json_to_file(device_setting_filename, device_setting_data_json);

    free(device_setting_str);
    cJSON_Delete(device_setting_json);
    cJSON_Delete(device_setting_data_json);

    return 1;
}

// 线程函数
void * save_data_thread(void *arg) {
    esl_file_save_info_t *args = (esl_file_save_info_t *)arg;
    prctl(PR_SET_NAME, "save_data_to_local"); // 设置线程名称
    // 转换 model 为 JSON
    int model_flag = _save_template_model_data(args);
    if(model_flag == 0){
        free(args);
        return NULL;
    }
    // 转换 product_info_list 为 JSON
    int product_info_flag = _save_product_info_data(args);
    if(product_info_flag == 0){
        free(args);
        return NULL;
    }

    _save_device_setting_data(args);     
    free(args);
    return NULL;
}

esl_product_info_t ** query_product_infos_local(int bg_index,char * device_sn,int * count){
    char product_info_filename[256];
    snprintf(product_info_filename, sizeof(product_info_filename), k_path_json_product_info, device_sn);
    char * product_infos = _read_json_file(product_info_filename);
    if(is_string_empty(product_infos)){
        *count = 0;
        return NULL;
    }
    cJSON *result_json = cJSON_Parse(product_infos);
    if(!result_json){
        free(product_infos);
        printf("query_product_infos_local convert json fail!\n");
        return NULL;
    }
    esl_product_info_t ** product_info_list = parse_product_infos(result_json,count);

    cJSON_Delete(result_json);
    free(product_infos);
    return product_info_list;
}

esl_show_model_t * query_product_template_local(int bg_index,char * device_sn){
    char model_filename[256];
    snprintf(model_filename, sizeof(model_filename), k_path_json_product_model, device_sn);
    char * product_template = _read_json_file(model_filename);
    if(is_string_empty(product_template)){
        return NULL;
    }
    cJSON *result_json = cJSON_Parse(product_template);
    if(!result_json){
        free(product_template);
        //printf("query_product_template_local convert json fail! content :%s\n",product_template);
        return NULL;
    }
    cJSON *data = cJSON_GetObjectItem(result_json, "data");
    esl_show_model_t * model = parse_product_template(data);

    cJSON_Delete(result_json);
    free(product_template);
    return model;
}

esl_device_setting_t * query_device_setting_local(int bg_index,char * device_sn){
    char device_setting_filename[256];
    snprintf(device_setting_filename, sizeof(device_setting_filename), k_path_json_device_setting, device_sn);
    char * device_setting_str = _read_json_file(device_setting_filename);
    if(is_string_empty(device_setting_str)){
        return NULL;
    }
    cJSON *result_json = cJSON_Parse(device_setting_str);
    if(!result_json){
        free(device_setting_str);
        printf("query_device_setting_local convert json fail! content :%s\n",device_setting_str);
        return NULL;
    }
    cJSON *data = cJSON_GetObjectItem(result_json, "data");
    esl_device_setting_t * device_setting = parse_device_setting(data);

    cJSON_Delete(result_json);
    free(device_setting_str);
    return device_setting;
}

void *delete_local_data(int bg_index,char * device_sn){
    char model_filename[256];
    char product_info_filename[256];
    char device_setting_filename[256];
    snprintf(model_filename, sizeof(model_filename), k_path_json_product_model, device_sn);
    snprintf(product_info_filename, sizeof(product_info_filename), k_path_json_product_info, device_sn);
    snprintf(device_setting_filename, sizeof(device_setting_filename), k_path_json_device_setting, device_sn);

    _check_and_delete_file(model_filename);
    _check_and_delete_file(product_info_filename);
    _check_and_delete_file(device_setting_filename);

    return NULL;
}

void *delete_local_logo_image(int bg_index){
    if(bg_index == 1){
        _check_and_delete_file(k_path_boot_bg_img_1);
    }else if(bg_index == 2){
        _check_and_delete_file(k_path_boot_bg_img_2);
    }
}

int save_update(esl_push_server_data *data)
{
    if(data == NULL)
        return -1;

    FILE *fp = fopen("/data/update.ini", "w");
    if(!fp) {
        perror("Failed to open update file");
        return -2;
    }

    // 写入版本信息
    if(data->update_version && strlen(data->update_version) > 0) {
        fprintf(fp, "update_version=%s\n", data->update_version);
    }

    // 写入URL和MD5校验信息
    if(data->update_url && strlen(data->update_url) > 0) {
        fprintf(fp, "update_url=%s\n", data->update_url);
    }
    if(data->update_file_md5 && strlen(data->update_file_md5) > 0) {
        fprintf(fp, "update_md5=%s\n", data->update_file_md5);
    }

    // 写入启动相关资源
    if(data->boot_logo_url && strlen(data->boot_logo_url) > 0) {
        fprintf(fp, "boot_logo_url=%s\n", data->boot_logo_url);
    }
    if(data->boot_logo_md5 && strlen(data->boot_logo_md5) > 0) {
        fprintf(fp, "boot_logo_md5=%s\n", data->boot_logo_md5);
    }

    // 写入绑定相关资源
    if(data->bind_logo_url && strlen(data->bind_logo_url) > 0) {
        fprintf(fp, "bind_logo_url=%s\n", data->bind_logo_url);
    }
    if(data->bind_logo_md5 && strlen(data->bind_logo_md5) > 0) {
        fprintf(fp, "bind_logo_md5=%s\n", data->bind_logo_md5);
    }

    // 写入服务器配置
    if(data->host && strlen(data->host) > 0) {
        fprintf(fp, "server_host=%s\n", data->host);
    }
    fprintf(fp, "server_port=%d\n", data->port);

    // 写入认证信息
    if(data->username && strlen(data->username) > 0) {
        fprintf(fp, "auth_user=%s\n", data->username);
    }
    if(data->password && strlen(data->password) > 0) {
        fprintf(fp, "auth_pass=%s\n", data->password);
    }

    // 写入协议相关配置
    if(data->protocol && strlen(data->protocol) > 0) {
        fprintf(fp, "conn_protocol=%s\n", data->protocol);
    }
    if(data->client_id && strlen(data->client_id) > 0) {
        fprintf(fp, "client_id=%s\n", data->client_id);
    }
    if(data->topic && strlen(data->topic) > 0) {
        fprintf(fp, "mqtt_topic=%s\n", data->topic);
    }

    // 写入状态信息
    fprintf(fp, "master_node=%d\n", data->master);
    fprintf(fp, "group_id=%d\n", data->group_id);

    fclose(fp);
    system("sync");
    return 0;
}


int sparse_update(esl_push_server_data *data) {
    if(data == NULL) return -1;

    FILE *fp = fopen("/data/update.ini", "r");
    if(!fp) {
        perror("Failed to open update file");
        return -2;
    }

    char line[512];
    while(fgets(line, sizeof(line), fp)) {
        // 去除换行符
        line[strcspn(line, "\n")] = 0;

        char *value = strchr(line, '=');
        if(!value) continue;
        *value++ = '\0';  // 分割键值对

        // 根据键名填充结构体
        if(strcmp(line, "update_version") == 0) {
            data->update_version = strdup(value);
        } else if(strcmp(line, "update_url") == 0) {
            data->update_url = strdup(value);
        } else if(strcmp(line, "update_md5") == 0) {
            data->update_file_md5 = strdup(value);
        } else if(strcmp(line, "boot_logo_url") == 0) {
            data->boot_logo_url = strdup(value);
        } else if(strcmp(line, "boot_logo_md5") == 0) {
            data->boot_logo_md5 = strdup(value);
        } else if(strcmp(line, "bind_logo_url") == 0) {
            data->bind_logo_url = strdup(value);
        } else if(strcmp(line, "bind_logo_md5") == 0) {
            data->bind_logo_md5 = strdup(value);
        } else if(strcmp(line, "server_host") == 0) {
            data->host = strdup(value);
        } else if(strcmp(line, "server_port") == 0) {
            data->port = atoi(value);
        } else if(strcmp(line, "auth_user") == 0) {
            data->username = strdup(value);
        } else if(strcmp(line, "auth_pass") == 0) {
            data->password = strdup(value);
        } else if(strcmp(line, "conn_protocol") == 0) {
            data->protocol = strdup(value);
        } else if(strcmp(line, "client_id") == 0) {
            data->client_id = strdup(value);
        } else if(strcmp(line, "mqtt_topic") == 0) {
            data->topic = strdup(value);
        } else if(strcmp(line, "master_node") == 0) {
            data->master = atoi(value);
        } else if(strcmp(line, "group_id") == 0) {
            data->group_id = atoi(value);
        }
    }

    fclose(fp);
    return 0;
}

#endif
