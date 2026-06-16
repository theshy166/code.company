#ifdef CONFIG_XOS_APP_ESL2
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>
#include "esl_file.h"
#include "cJSON.h"
#include "../utils/esl_ui_utils.h"
#include "../conf/conf.h"
#include "utils.h"


// 判断文件是否存在，存在则删除
static void _check_and_delete_file(const char *filename) {
    // 检查文件是否存在
    if (access(filename, F_OK) == 0) {
        // 文件存在，删除文件
        if (remove(filename) == 0) {
            QM_ESL2_LOG("File deleted successfully: %s\n", filename);
        } else {
            QM_ESL2_LOG("Failed to delete file: %s\n", filename);
        }
    } else {
        QM_ESL2_LOG("File does not exist: %s\n", filename);
    }
}

// 读取 JSON 文件内容并返回字符串
static char* _read_json_file(const char *filename) {
    // 检查文件是否存在
    if (access(filename, F_OK) != 0) {
        QM_ESL2_LOG("File does not exist: %s\n", filename);
        return NULL;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        QM_ESL2_LOG("Failed to open file: %s\n", filename);
        return NULL;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 分配内存以存储文件内容
    char *content = (char *)malloc(file_size + 1);
    if (content == NULL) {
        QM_ESL2_LOG("Failed to allocate memory for file content\n");
        fclose(file);
        return NULL;
    }

    // 读取文件内容
    size_t read_size = fread(content, 1, file_size, file);
    if (read_size != file_size) {
        QM_ESL2_LOG("Failed to read file content\n");
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
static void _save_json_to_file(const char *filename, char *json_str) {
    // 检查文件是否存在
    if (access(filename, F_OK) == 0) {
        // 文件存在，删除旧的文件
        if (remove(filename) != 0) {
            QM_ESL2_LOG("Failed to delete old file: %s\n", filename);
            return;
        }
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        QM_ESL2_LOG("Failed to open file: %s\n", filename);
        free(json_str);
        return;
    }

    fprintf(file, "%s", json_str);
    fclose(file);
}

static char * _els_base_model_to_json(esl_base_model_t *model){
    if (!model) return NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "width", model->width);
    cJSON_AddNumberToObject(root, "height", model->height);
    cJSON_AddStringToObject(root, "color", model->color ? model->color : "");
    cJSON_AddNumberToObject(root, "opacity", model->opacity);
    cJSON_AddStringToObject(root, "bgColor", model->bgColor ? model->bgColor : "");

    // 图片列表
    cJSON *img_arr = cJSON_CreateArray();
    if (model->img_urls) {
        for (int i = 0; i < model->img_count; ++i) {
            cJSON *img = cJSON_CreateObject();
            cJSON_AddStringToObject(img, "img_url", model->img_urls[i].img_url ? model->img_urls[i].img_url : "");
            cJSON_AddNumberToObject(img, "width", model->img_urls[i].width);
            cJSON_AddNumberToObject(img, "height", model->img_urls[i].height);
            cJSON_AddItemToArray(img_arr, img);
        }
    }
    cJSON_AddItemToObject(root, "img_urls", img_arr);

    cJSON *video_arr = cJSON_CreateArray();
    if (model->video_urls) {
        for (int i = 0; i < model->video_count; ++i) {
            cJSON_AddItemToArray(video_arr, cJSON_CreateString(model->video_urls[i] ? model->video_urls[i] : ""));
        }
    }
    cJSON_AddItemToObject(root, "video_urls", video_arr);

    // 子元素
    cJSON *child_arr = cJSON_CreateArray();
    if (model->child_list) {
        for (int i = 0; i < model->child_count; ++i) {
            esl_base_model_child_t *child = &model->child_list[i];
            cJSON *child_obj = cJSON_CreateObject();
            cJSON_AddNumberToObject(child_obj, "type", child->type);
            cJSON_AddStringToObject(child_obj, "content", child->content ? child->content : "");
            cJSON_AddStringToObject(child_obj, "color", child->color ? child->color : "");
            cJSON_AddNumberToObject(child_obj, "fontSize", child->fontSize);
            cJSON_AddStringToObject(child_obj, "textAlign", child->textAlign ? child->textAlign : "");
            cJSON_AddStringToObject(child_obj, "fontFamily", child->fontFamily ? child->fontFamily : "");
            cJSON_AddStringToObject(child_obj, "bgColor", child->bgColor ? child->bgColor : "");
            cJSON_AddStringToObject(child_obj, "textDecoration", child->textDecoration ? child->textDecoration : "");
            cJSON_AddNumberToObject(child_obj, "borderRadius", child->borderRadius);
            cJSON_AddNumberToObject(child_obj, "borderWidth", child->borderWidth);
            cJSON_AddStringToObject(child_obj, "borderColor", child->borderColor ? child->borderColor : "");
            cJSON_AddNumberToObject(child_obj, "x", child->x);
            cJSON_AddNumberToObject(child_obj, "y", child->y);
            cJSON_AddNumberToObject(child_obj, "w", child->w);
            cJSON_AddNumberToObject(child_obj, "h", child->h);
            cJSON_AddNumberToObject(child_obj, "angle", child->angle);
            cJSON_AddNumberToObject(child_obj, "opacity", child->opacity);
            cJSON_AddNumberToObject(child_obj, "padding", child->padding);
            // 价格相关
            cJSON_AddNumberToObject(child_obj, "intFontSize", child->intFontSize);
            cJSON_AddNumberToObject(child_obj, "floatFontSize", child->floatFontSize);
            cJSON_AddStringToObject(child_obj, "numFloatStyle", child->numFloatStyle ? child->numFloatStyle : "");
            cJSON_AddItemToArray(child_arr, child_obj);
        }
    }
    cJSON_AddItemToObject(root, "child_list", child_arr);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str; // 用完后记得 free
}

void save_data_thread(esl_file_save_info2_t *args){
    char * device_sn = args->device_sn;
    esl_base_model_t * model = args->model;
    int index = args->index;

    char * els_base_model_str = _els_base_model_to_json(model);

    char model_filename[256];
    snprintf(model_filename, sizeof(model_filename), k_path_json_product_base_model, args->device_sn);
    _save_json_to_file(model_filename, els_base_model_str);
    free(els_base_model_str);
}

esl_base_model_t * query_esl_base_model_local(int bg_index,char * device_sn){
    char esl_base_model_filename[256];
    snprintf(esl_base_model_filename, sizeof(esl_base_model_filename), k_path_json_product_base_model, device_sn);
    char * esl_base_model_str = _read_json_file(esl_base_model_filename);
    if(is_string_empty(esl_base_model_str)){
        return NULL;
    }
    esl_base_model_t * model = parse_esl_base_model(esl_base_model_str);
    free(esl_base_model_str);
    return model;
}

void *delete_local_data(int bg_index,char * device_sn){
    char model_filename[256];
    snprintf(model_filename, sizeof(model_filename), k_path_json_product_base_model, device_sn);
    _check_and_delete_file(model_filename);
    return NULL;
}

void *delete_local_logo_image(int bg_index){
    if(bg_index == 1){
        _check_and_delete_file(k_path_boot_bg_img_1);
    }else if(bg_index == 2){
        _check_and_delete_file(k_path_boot_bg_img_2);
    }
}

int save_update(version_info *data)
{
    if(data == NULL)
        return -1;

    FILE *fp = fopen("/data/update.ini", "w");
    if(!fp) {
        perror("Failed to open update file");
        return -2;
    }

    // 写入版本信息
    if(data->version && strlen(data->version) > 0) {
        fprintf(fp, "update_version=%s\n", data->version);
    }
    // 写入URL
    if(data->url && strlen(data->url) > 0) {
        fprintf(fp, "update_url=%s\n", data->url);
    }
    // 写入MD5
    if(data->md5 && strlen(data->md5) > 0) {
        fprintf(fp, "update_md5=%s\n", data->md5);
    }
    // 写入大小
    if(data->size > 0) {
        fprintf(fp, "update_size=%d\n", data->size);
    }

    fclose(fp);
    system("sync");
    return 0;
}


int sparse_update(version_info *data) {
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
            data->version = strdup(value);
        } else if(strcmp(line, "update_url") == 0) {
            data->url = strdup(value);
        } else if(strcmp(line, "update_md5") == 0) {
            data->md5 = strdup(value);
        } else if(strcmp(line, "update_size") == 0) {
            data->size = atoi(value);
        }
    }

    fclose(fp);
    return 0;
}

#endif
