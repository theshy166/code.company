
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "esl_ui_utils.h"
#include "../conf/conf.h"
#include "cJSON.h"
#include "esl_json_utils.h"
#include "utils.h"
#include "../ctrl/esl_service.h"
#include "pic_utils.h"

#define IMG_COUNT 20
#define MIN(a,b) (((a) > (b))? (b) : (a))

static void _get_ohter_info_text(esl_product_info_t *info,char * value,char *result){
    if(info == NULL || is_string_empty(value)){
        return;
    }
    if(strcmp(value,"规格")){
        snprintf(result, 256, "规格:%s", info->pro_spec);
    }else if(strcmp(value,"产地")){
        snprintf(result, 256, "产地:%s", info->pro_adr);
    }else if(strcmp(value,"生产日期")){
        snprintf(result, 256, "生产日期:%s", info->crt_day);
    }else if(strcmp(value,"保质到期")){
        snprintf(result, 256, "保质到期:%s", info->exp_day);
    }else if(strcmp(value,"保质天数")){
        snprintf(result, 256, "保质天数:%s", info->exp_day_num);
    }else if(strcmp(value,"零售价")){
        snprintf(result, 256, "零售价:%s", info->sale_price);
    }else if(strcmp(value,"会员价")){
        snprintf(result, 256, "会员价:%s", info->vip_price);
    }else if(strcmp(value,"批发价")){
        snprintf(result, 256, "批发价:%s", info->pf_price);
    }else if(strcmp(value,"促销价")){
        snprintf(result, 256, "促销价:%s", info->dis_price);
    }else if(strcmp(value,"商品自定1")){
        snprintf(result, 256, "商品自定1:%s", info->ctm_txt1);
    }else if(strcmp(value,"商品自定2")){
        snprintf(result, 256, "商品自定2:%s", info->ctm_txt2);
    }else if(strcmp(value,"商品自定3")){
        snprintf(result, 256, "商品自定3:%s", info->ctm_txt3);
    }else if(strcmp(value,"商品自定4")){
        snprintf(result, 256, "商品自定4:%s", info->ctm_txt4);
    }else if(strcmp(value,"商品自定5")){
        snprintf(result, 256, "商品自定5:%s", info->ctm_txt5); 
    }else if(strcmp(value,"商品自定6")){
        snprintf(result, 256, "商品自定6:%s", info->ctm_txt6);
    }
}

static void _get_product_info_text(esl_product_info_t ** product_infos,esl_other_info_t * other_info,char * propAssPro,char *propAssProAttr,int count,char *result){
    if (product_infos == NULL || propAssPro == NULL || propAssProAttr == NULL || count <= 0) {
        return;
    }
    int index = 0;
    if (starts_with(propAssPro, "pro")) {
        // 处理以 "pro" 开头的属性
        index = atoi(propAssPro + 3); // 假设属性名格式为 "proX"
    }
    if (index <= 0){
        return; // 如果索引无效，返回 NULL
    }
    if (index > count) {
        index = count; // 确保索引不超过数组长度
    }
    index--; // 转换为 0 基索引
    esl_product_info_t *info = product_infos[index];
    if (info == NULL) {
        return; // 如果信息为空，返回 NULL
    }
    if(strcmp(propAssProAttr,"Name") == 0){
        strcpy(result,info->pro_name); // 返回商品名称
    }else if(strcmp(propAssProAttr,"Unit") == 0){
        strcpy(result,info->pro_unit); // 返回商品单位
    }else if(strcmp(propAssProAttr,"Img") == 0){
        strcpy(result,info->img_urls); // 返回商品图片地址
    }else if(strcmp(propAssProAttr,"PriceBig") == 0){
        char * price = info->sale_price;
        if(is_string_empty(price)){
            return;
        }
        if(strstr(price,".") == NULL){
            strcpy(result, price); // 价格整数
            return;
        }
        char *dot_pos = strchr(price, '.');
        char int_part[50];
        strncpy(int_part, price, dot_pos - price + 1); // Include the dot
        int_part[dot_pos - price + 1] = '\0';
        strcpy(result, int_part); // 价格整数
    }else if(strcmp(propAssProAttr,"PriceDcm") == 0){
        char * price = info->sale_price;
        if(is_string_empty(price) || strstr(price,".") == NULL){
            return;
        }
        char *dot_pos = strchr(price, '.');
        strcpy(result, dot_pos + 1); // 返回商品价格小数部分
    }else if(strcmp(propAssProAttr,"VipPriceBig") == 0){
        char * price = info->vip_price;
        if(is_string_empty(price)){
            return;
        }
        if(strstr(price,".") == NULL){
            strcpy(result, price); // 价格整数
            return;
        }
        char *dot_pos = strchr(price, '.');
        char int_part[50];
        strncpy(int_part, price, dot_pos - price + 1); // Include the dot
        int_part[dot_pos - price + 1] = '\0';
        strcpy(result, int_part); // 价格整数
    }else if(strcmp(propAssProAttr,"VipPriceDcm") == 0){
        char * price = info->vip_price;
        if(is_string_empty(price) || strstr(price,".") == NULL){
            return;
        }
        char *dot_pos = strchr(price, '.');
        strcpy(result, dot_pos + 1); // 返回商品价格小数部分
    }else if(strcmp(propAssProAttr,"PfPriceBig") == 0){
        char * price = info->pf_price;
        if(is_string_empty(price)){
            return;
        }
        if(strstr(price,".") == NULL){
            strcpy(result, price); // 价格整数
            return;
        }
        char *dot_pos = strchr(price, '.');
        char int_part[50];
        strncpy(int_part, price, dot_pos - price + 1); // Include the dot
        int_part[dot_pos - price + 1] = '\0';
        strcpy(result, int_part); // 价格整数
    }else if(strcmp(propAssProAttr,"PfPriceDcm") == 0){
        char * price = info->pf_price;
        if(is_string_empty(price) || strstr(price,".") == NULL){
            return;
        }
        char *dot_pos = strchr(price, '.');
        strcpy(result, dot_pos + 1); // 返回商品价格小数部分
    }else if(strcmp(propAssProAttr,"DisPriceBig") == 0){
        char * price = info->dis_price;
        if(is_string_empty(price)){
            return;
        }
        if(strstr(price,".") == NULL){
            strcpy(result, price); // 价格整数
            return;
        }
        char *dot_pos = strchr(price, '.');
        char int_part[50];
        strncpy(int_part, price, dot_pos - price + 1); // Include the dot
        int_part[dot_pos - price + 1] = '\0';
        strcpy(result, int_part); // 价格整数
    }else if(strcmp(propAssProAttr,"DisPriceDcm") == 0){
        char * price = info->dis_price;
        if(is_string_empty(price) || strstr(price,".") == NULL){
            return;
        }
        char *dot_pos = strchr(price, '.');
        strcpy(result, dot_pos + 1); // 返回商品价格小数部分
    }else if(strcmp(propAssProAttr,"Id") == 0){
        strcpy(result,info->pro_id); // 返回商品id
     }else if(strcmp(propAssProAttr,"1") == 0){
        char * value = other_info->txt1;
        _get_ohter_info_text(info,value,result);
     }else if(strcmp(propAssProAttr,"2") == 0){
        char * value = other_info->txt2;
        _get_ohter_info_text(info,value,result);
     }else if(strcmp(propAssProAttr,"3") == 0){
        char * value = other_info->txt3;
        _get_ohter_info_text(info,value,result);
     }else if(strcmp(propAssProAttr,"4") == 0){
        char * value = other_info->txt4;
        _get_ohter_info_text(info,value,result);
     }else if(strcmp(propAssProAttr,"5") == 0){
        char * value = other_info->txt5;
        _get_ohter_info_text(info,value,result);
     }else if(strcmp(propAssProAttr,"6") == 0){
        char * value = other_info->txt6;
        _get_ohter_info_text(info,value,result);
     } else {
        return ; // 如果属性不匹配，返回 NULL   
    }
}

//目前只支持 rgba 和 #333333格式
static void _color_convert(char * source_color,char * result){
    if (is_string_empty(source_color)){
        strcpy(result,source_color);
        return;
    }
    if (starts_with(source_color, "rgba")){
        int r, g, b;
        float a;
        // 解析字符串
        sscanf(source_color, "rgba(%d, %d, %d, %f)", &r, &g, &b, &a);
        uint32_t temp = rgba_to_hex(r,g,b,a);
        sprintf(result, "#%06x", temp & 0xFFFFFF);
    } else{
        strcpy(result,source_color);
    }
}

esl_base_model_t * the_second_to_model(const char * product_template,const char * product_list,const char * other_info_str,const char * tpl_bg_url,int index,int display_product_num){
    if(is_string_empty(product_template)){
        return NULL;
    }
    int count = 0;
    esl_product_info_t **  product_infos = parse_product_list_second(product_list,&count);
    esl_other_info_t  * other_info = parse_other_info_second(other_info_str);

    cJSON *root = cJSON_Parse(product_template);
    if (!root) {
        QM_ESL2_LOG("Failed to parse JSON\n");
        free_esl_product_info_array(product_infos,count);
        free_esl_other_info(other_info);
        return NULL;
    }
    esl_base_model_t *model = (esl_base_model_t *)malloc(sizeof(esl_base_model_t));
    if (!model) {
        QM_ESL2_LOG("Failed to allocate memory for product_template_t\n");
        cJSON_Delete(root);
        free_esl_product_info_array(product_infos,count);
        free_esl_other_info(other_info);
        return NULL;
    }
    memset(model, 0, sizeof(esl_base_model_t)); // 初始化内存
    int img_count = 0;
    // 解析 bg 部分
    cJSON *bg = cJSON_GetObjectItem(root, "bg");

    cJSON *width = cJSON_GetObjectItem(bg, "width");
    model->width = (width && cJSON_IsNumber(width)) ? width->valueint : 0;

    cJSON *height = cJSON_GetObjectItem(bg, "height");
    model->height = (height && cJSON_IsNumber(height)) ? height->valueint : 0;

    cJSON *color = cJSON_GetObjectItem(bg, "color");
    if(color && cJSON_IsString(color) && color->valuestring){
        char result[10];
        char * temp = color->valuestring;
        _color_convert(temp,&result);
        model->color = strdup(result);
    }

    cJSON *opacity = cJSON_GetObjectItem(bg, "opacity");
    model->opacity = (opacity && cJSON_IsNumber(opacity)) ? opacity->valuedouble : 1.0;

    cJSON *background = cJSON_GetObjectItem(bg, "background");
    if(background && cJSON_IsString(background) && background->valuestring){
        char result[10];
        char * temp = background->valuestring;
        _color_convert(temp,&result);
        model->bgColor = strdup(result);
    }

    // 解析 body 部分
    cJSON *body = cJSON_GetObjectItem(root, "body");
    if (body && cJSON_IsArray(body)) {
        // 存在  
        model->child_count = cJSON_GetArraySize(body) + 2;//背景+二维码
        model->child_list = (esl_base_model_child_t *)malloc(sizeof(esl_base_model_child_t) * model->child_count);
        model->img_urls = (esl_img_t *)malloc(sizeof(esl_img_t) * IMG_COUNT);
        if (!model->child_list) {
            QM_ESL2_LOG("Failed to allocate memory for body components\n");
            cJSON_Delete(root);
            free(model);
            free_esl_product_info_array(product_infos,count);
            free_esl_other_info(other_info);
            return NULL;
        }
        memset(model->child_list, 0, sizeof(esl_base_model_child_t) * model->child_count); // 初始化内存
        //背景
        esl_base_model_child_t *bg_child = &model->child_list[0];
        if(!is_string_empty(tpl_bg_url)){
            bg_child->content = strdup(tpl_bg_url);
            bg_child->type = EslCompentTypeIMAGE;
            esl_img_t * img_url = &model->img_urls[img_count];
            img_url->img_url = strdup(tpl_bg_url);
            img_url->width = model->width;
            img_url->height = model->height;
            img_count++;
        }else{
            cJSON *backgroundImage = cJSON_GetObjectItem(bg, "backgroundImage");
            if(backgroundImage && cJSON_IsString(backgroundImage) && backgroundImage->valuestring){
                bg_child->content = strdup(backgroundImage->valuestring);
                bg_child->type = EslCompentTypeIMAGE;
                esl_img_t * img_url = &model->img_urls[img_count];
                img_url->img_url = strdup(backgroundImage->valuestring);
                img_url->width = model->width;
                img_url->height = model->height;
                img_count++;
            }else{
                bg_child->content = strdup("");
                bg_child->type = EslCompentTypeTEXT;
            }
        }
        bg_child->w = model->width;
        bg_child->h = model->height;
        bg_child->x = 0;
        bg_child->y = 0;


        for (int i = 1; i < model->child_count - 1; i++) {
            cJSON *item = cJSON_GetArrayItem(body, i - 1);
            esl_base_model_child_t *child = &model->child_list[i];

            cJSON *component_item = cJSON_GetObjectItem(item, "component");
            if(component_item && cJSON_IsString(component_item) && component_item->valuestring){
                char *component = component_item->valuestring;
                if (strcmp(component, "Picture") == 0) {
                    child->type = EslCompentTypeIMAGE;
                } else if (strcmp(component, "VText") == 0) {
                    child->type = EslCompentTypeTEXT;
                } else if (strcmp(component, "RectShape") == 0) {
                    child->type = EslCompentTypeRECT;
                } else if (strcmp(component, "CircleShape") == 0) {
                    child->type = EslCompentTypeCIRCLE;
                } else if (strcmp(component, "LineShape") == 0) {
                    child->type = EslCompentTypeLINE;
                } else if (strcmp(component, "VQrCode") == 0) {
                    child->type = EslCompentTypeQRCODE;
                } else if (strcmp(component, "VBarCode") == 0) {
                    child->type = EslCompentTypeBARCODE;
                } else if (strcmp(component, "VVideo") == 0) {
                    child->type = EslCompentTypeVIDEO;
                }
            }else{
                //默认为文本
                child->type = EslCompentTypeTEXT;
            }
            
            cJSON *label = cJSON_GetObjectItem(item, "label");
            char * label_temp = (label && cJSON_IsString(label) && label->valuestring) ? label->valuestring : "";

            char * proValue_temp = "";
            cJSON *propValue = cJSON_GetObjectItem(item, "propValue");
            if( child->type == EslCompentTypeIMAGE){
                cJSON *url = cJSON_GetObjectItem(propValue,"url");
                proValue_temp = (url && cJSON_IsString(url) && url->valuestring) ? url->valuestring : "";
            }else{
                if(propValue && cJSON_IsString(propValue) && propValue->valuestring){
                    proValue_temp = propValue->valuestring;
                    remove_html_entities(proValue_temp);
                    char *p = proValue_temp;
                    while ((p = strstr(p, "<br>")) != NULL) {
                        *p = '\n';
                        memmove(p + 1, p + 4, strlen(p + 4) + 1); // +1 for '\0'
                        p++; // 跳过刚替换的 \n
                    }
                }else{
                    proValue_temp = "";
                }
            }

            cJSON *propAssPro = cJSON_GetObjectItem(item, "propAssPro");
            char *propAssPro_temp = (propAssPro && cJSON_IsString(propAssPro) && propAssPro->valuestring) ? propAssPro->valuestring : "";

            cJSON *propAssProAttr = cJSON_GetObjectItem(item, "propAssProAttr");
            char *propAssProAttr_temp = (propAssProAttr && cJSON_IsString(propAssProAttr) && propAssProAttr->valuestring) ? propAssProAttr->valuestring : "";

            cJSON *usePropValue = cJSON_GetObjectItem(item, "usePropValue");
            char *usePropValue_temp = (usePropValue && cJSON_IsString(usePropValue) && usePropValue->valuestring) ? usePropValue->valuestring : "";
            
            cJSON *isAttr = cJSON_GetObjectItem(item, "isAttr");
            bool isAttr_temp = (isAttr && cJSON_IsBool(isAttr)) ? cJSON_IsTrue(isAttr) : false;

            if(usePropValue_temp != NULL && strcmp(usePropValue_temp,"Y") == 0){
                char text[256]= {0};
                _get_product_info_text(product_infos,other_info,propAssPro_temp,propAssProAttr_temp,count,&text);
                if(!is_string_empty(text)){
                    child->content = strdup(text);
                }else{
                    if(child->type == EslCompentTypeIMAGE || child->type == EslCompentTypeVIDEO){
                        child->content = strdup(proValue_temp);
                    }else{
                        child->content = strdup(text);
                    }
                }
            }else{
                child->content = strdup(proValue_temp);
            }

            // 解析 style
            cJSON *style = cJSON_GetObjectItem(item, "style");
            if (style) {
                cJSON *style_color = cJSON_GetObjectItem(style, "color");
                if(style_color && cJSON_IsString(style_color) && style_color->valuestring){
                    char result[10];
                    char * temp = style_color->valuestring;
                    _color_convert(temp,&result);
                    child->color = strdup(result);
                }

                cJSON *style_fontSize = cJSON_GetObjectItem(style, "fontSize");
                child->fontSize = (style_fontSize && cJSON_IsNumber(style_fontSize)) ? style_fontSize->valueint : 0;

                cJSON *style_textAlign = cJSON_GetObjectItem(style, "textAlign");
                child->textAlign = (style_textAlign && cJSON_IsString(style_textAlign) && style_textAlign->valuestring) ? strdup(style_textAlign->valuestring) : strdup("");

                cJSON *style_fontFamily = cJSON_GetObjectItem(style, "fontFamily");
                child->fontFamily = (style_fontFamily && cJSON_IsString(style_fontFamily) && style_fontFamily->valuestring) ? strdup(style_fontFamily->valuestring) : strdup("");

                cJSON *style_backgroundColor = cJSON_GetObjectItem(style, "backgroundColor");
                if(style_backgroundColor && cJSON_IsString(style_backgroundColor) && style_backgroundColor->valuestring){
                    char result[10];
                    char * temp = style_backgroundColor->valuestring;
                    _color_convert(temp,&result);
                    child->bgColor = strdup(result);
                }

                cJSON *style_borderRadius = cJSON_GetObjectItem(style, "borderRadius");
                if(style_borderRadius){
                    if(cJSON_IsString(style_borderRadius)){
                        child->borderRadius = (style_borderRadius->valuestring) ? atof(style_borderRadius->valuestring) : 1.0;
                    }else if(cJSON_IsNumber(style_borderRadius)){
                        child->borderRadius = style_borderRadius->valueint;
                    }else{
                        child->borderRadius = 1.0;
                    }
                }else{
                    child->borderRadius = 1.0;
                }

                cJSON *style_borderWidth = cJSON_GetObjectItem(style, "borderWidth");
                child->borderWidth = (style_borderWidth && cJSON_IsNumber(style_borderWidth)) ? style_borderWidth->valueint : 0;
                
                cJSON *style_borderColor = cJSON_GetObjectItem(style, "borderColor");
                if(style_borderColor && cJSON_IsString(style_borderColor) && style_borderColor->valuestring){
                    char result[10];
                    char * temp = style_borderColor->valuestring;
                    _color_convert(temp,&result);
                    child->borderColor = strdup(result);
                }

                cJSON *style_left = cJSON_GetObjectItem(style, "left");
                child->x = (style_left && cJSON_IsNumber(style_left)) ? style_left->valueint : 0;
                
                cJSON *style_top = cJSON_GetObjectItem(style, "top");
                child->y = (style_top && cJSON_IsNumber(style_top)) ? style_top->valueint : 0;
                
                cJSON *style_width = cJSON_GetObjectItem(style, "width");
                child->w = (style_width && cJSON_IsNumber(style_width)) ? style_width->valueint : 0;

                cJSON *style_height = cJSON_GetObjectItem(style, "height");
                child->h = (style_height && cJSON_IsNumber(style_height)) ? style_height->valueint : 0;

                cJSON *style_rotate = cJSON_GetObjectItem(style, "rotate");
                child->angle = (style_rotate && cJSON_IsNumber(style_rotate)) ? style_rotate->valueint : 0;

                cJSON *style_opacity = cJSON_GetObjectItem(style, "opacity");
                child->opacity = (style_opacity && cJSON_IsNumber(style_opacity)) ? style_opacity->valuedouble : 1.0;
                
                cJSON *style_padding = cJSON_GetObjectItem(style, "padding");
                child->padding = (style_padding && cJSON_IsNumber(style_padding)) ? style_padding->valueint : 0;
                
                cJSON * style_line_height = cJSON_GetObjectItem(style,"lineHeight");
                if(style_line_height){
                    float lineHeight = 0.0;
                    if(cJSON_IsString(style_line_height)){
                        lineHeight = (is_string_empty(style_line_height->valuestring) == 0) ? atof(style_line_height->valuestring) : 1.2;
                        QM_ESL2_LOG("lineHeight : %f\n",lineHeight);
                    }else if(cJSON_IsNumber(style_line_height)){
                        lineHeight = style_line_height->valuedouble;
                    }else{
                        lineHeight = 1.0;
                    }
                    child->lineSpace = lineHeight * child->fontSize - child->fontSize;
                    QM_ESL2_LOG("lineSpace : %d\n",child->lineSpace);
                }else{
                    child->lineSpace = 0;
                }

                cJSON * style_letter_spacing = cJSON_GetObjectItem(style,"letterSpacing");
                child->letterSpace = (style_letter_spacing && cJSON_IsNumber(style_letter_spacing)) ? style_letter_spacing->valuedouble : 0;

            }
            if(child->type == EslCompentTypeIMAGE){
                esl_img_t * img_url = &model->img_urls[img_count];
                img_url->img_url = strdup(child->content);
                img_url->width = child->w;
                img_url->height = child->h;
                img_count++;
            }

            if(child->type == EslCompentTypeVIDEO){
                int video_count = 0;

                int loop_count = MIN(count,display_product_num);
                
                char ** video_urls = (char **)calloc(loop_count, sizeof(char *));

                if(loop_count > 0 && product_infos != NULL){
                    for(int j = 0 ; j < loop_count ; j++){
                        video_urls[video_count] = strdup(product_infos[j]->video_urls);
                        video_count ++;
                    }
                }
                model->video_urls = video_urls;
                model->video_count = video_count;
            }
        }
        //二维码
        esl_base_model_child_t *child = &model->child_list[model->child_count - 1];
        child->type = EslCompentTypeQRCODE;
        child->content = strdup(get_device_sn(index));
        child->w = 60;
        child->h = 60;
        child->x = 10;
        child->y = 1210;
        child->padding = 5;
    }else{
        //不存在 只画背景+二维码
        model->child_count = 2;//背景+二维码
        model->child_list = (esl_base_model_child_t *)malloc(sizeof(esl_base_model_child_t) * model->child_count);
        if (!model->child_list) {
            QM_ESL2_LOG("Failed to allocate memory for body components\n");
            cJSON_Delete(root);
            free(model);
            free_esl_product_info_array(product_infos,count);
            free_esl_other_info(other_info);
            return NULL;
        }
        memset(model->child_list, 0, sizeof(esl_base_model_child_t) * model->child_count); // 初始化内存
        //背景
        esl_base_model_child_t *bg_child = &model->child_list[0];
        if(!is_string_empty(tpl_bg_url)){
            bg_child->content = strdup(tpl_bg_url);
            bg_child->type = EslCompentTypeIMAGE;
            esl_img_t * img_url = &model->img_urls[img_count];
            img_url->img_url = strdup(tpl_bg_url);
            img_url->width = model->width;
            img_url->height = model->height;
            img_count++;
        }else{
            cJSON *backgroundImage = cJSON_GetObjectItem(bg, "backgroundImage");
            if(backgroundImage && cJSON_IsString(backgroundImage) && backgroundImage->valuestring){
                bg_child->content = strdup(backgroundImage->valuestring);
                bg_child->type = EslCompentTypeIMAGE;
                esl_img_t * img_url = &model->img_urls[img_count];
                img_url->img_url = strdup(backgroundImage->valuestring);
                img_url->width = model->width;
                img_url->height = model->height;
                img_count++;
            }else{
                bg_child->content = strdup("");
                bg_child->type = EslCompentTypeTEXT;
            }
        }
        bg_child->w = model->width;
        bg_child->h = model->height;
        bg_child->x = 0;
        bg_child->y = 0;

        //二维码
        esl_base_model_child_t *child = &model->child_list[model->child_count - 1];
        child->content = strdup(get_device_sn(index));
        child->w = 60;
        child->h = 60;
        child->x = 10;
        child->y = 1210;
        child->padding = 5;
    }
    model->img_count = img_count;

    cJSON_Delete(root);
    free_esl_product_info_array(product_infos,count);
    free_esl_other_info(other_info);
    return model;
}

void free_esl_base_model(esl_base_model_t *model) {
    if (!model) return;
    if (model->color) free(model->color);
    if (model->bgColor) free(model->bgColor);

    // 释放图片列表
    if (model->img_urls) {
        for (int i = 0; i < model->img_count; ++i) {
            if (model->img_urls[i].img_url) free(model->img_urls[i].img_url);
        }
        free(model->img_urls);
    }

    // 释放视频列表
    if (model->video_urls) {
        for (int i = 0; i < model->video_count; ++i) {
            if (model->video_urls[i]) free(model->video_urls[i]);
        }
        free(model->video_urls);
    }

    // 释放子元素
    if (model->child_list) {
        for (int i = 0; i < model->child_count; ++i) {
            esl_base_model_child_t *child = &model->child_list[i];
            if (child->content) free(child->content);
            if (child->textAlign) free(child->textAlign);
            if (child->fontFamily) free(child->fontFamily);
            if (child->textDecoration) free(child->textDecoration);
            if (child->numFloatStyle) free(child->numFloatStyle);
            if (child->color) free(child->color);
            if (child->bgColor) free(child->bgColor);
            if (child->borderColor) free(child->borderColor);
        }
        free(model->child_list);
    }

    free(model);
}

esl_base_model_t * parse_esl_base_model(const char * json_str){
    if (is_string_empty(json_str)){
        return NULL;
    } 
    cJSON *root = cJSON_Parse(json_str);
    if (!root){
        return NULL;
    } 

    esl_base_model_t *model = (esl_base_model_t*)calloc(1, sizeof(esl_base_model_t));
    if (!model) {
        cJSON_Delete(root);
        return NULL;
    }
    memset(model, 0, sizeof(esl_base_model_t)); // 初始化内存

    cJSON *width = cJSON_GetObjectItem(root, "width");
    model->width = (width && cJSON_IsNumber(width)) ? width->valueint : 0;

    cJSON *height = cJSON_GetObjectItem(root, "height");
    model->height = (height && cJSON_IsNumber(height)) ? height->valueint : 0;

    cJSON *color = cJSON_GetObjectItem(root, "color");
    model->color = (color && cJSON_IsString(color)) ? strdup(color->valuestring) : strdup("");

    cJSON *opacity = cJSON_GetObjectItem(root, "opacity");
    model->opacity = (opacity && cJSON_IsNumber(opacity)) ? opacity->valueint : 0;

    cJSON *bgColor = cJSON_GetObjectItem(root, "bgColor");
    model->bgColor = (bgColor && cJSON_IsString(bgColor)) ? strdup(bgColor->valuestring) : strdup("");

    // video_urls
    cJSON *video_arr = cJSON_GetObjectItem(root, "video_urls");
    if (video_arr && cJSON_IsArray(video_arr)) {
        model->video_count = cJSON_GetArraySize(video_arr);
        if (model->video_count > 0) {
            model->video_urls = (char **)calloc(model->video_count, sizeof(char*));
            for (int i = 0; i < model->video_count; i++) {
                cJSON *item = cJSON_GetArrayItem(video_arr, i);
                if(item && cJSON_IsString(item)){
                    char * temp = item->valuestring;
                    if (!starts_with(temp, "http")) {
                        // 处理以 "pro" 开头的属性
                        char url[256];
                        memset(url,0,256);
                        char * http_url = get_http_url();
                        strcat(url, http_url);
                        strcat(url, "/");
                        strcat(url, temp);
                        QM_ESL2_LOG("url : %s ===========================================",url);
                        model->video_urls[i] = strdup(url);
                    }else{
                        model->video_urls[i] = strdup(item->valuestring);
                    }
                } else {
                    model->video_urls[i] = strdup(""); // 如果不是字符串，设置为空字符串
                }
            }
        }
    }

    // img_urls
    cJSON *img_arr = cJSON_GetObjectItem(root, "img_urls");
    if (img_arr && cJSON_IsArray(img_arr)) {
        model->img_count = cJSON_GetArraySize(img_arr);
        if (model->img_count > 0) {
            model->img_urls = (esl_img_t*)calloc(model->img_count, sizeof(esl_img_t));
            for (int i = 0; i < model->img_count; i++) {
                cJSON *img = cJSON_GetArrayItem(img_arr, i);
                if (img && cJSON_IsObject(img)) {
                    cJSON *img_url = cJSON_GetObjectItem(img, "img_url");
                    if(img_url && cJSON_IsString(img_url)){
                        char * temp = img_url->valuestring;
                        if (!starts_with(temp, "http")) {
                            // 处理以 "pro" 开头的属性
                            char url[256];
                            memset(url,0,256);
                            char * http_url = get_http_url();
                            strcat(url, http_url);
                            strcat(url, "/");
                            strcat(url, temp);
                            QM_ESL2_LOG("url : %s ===========================================",url);
                            model->img_urls[i].img_url = strdup(url);
                        }else{
                            model->img_urls[i].img_url = strdup(img_url->valuestring);
                        }
                    } else {
                        model->img_urls[i].img_url = strdup(""); // 如果不是字符串，设置为空字符串
                    }
                    cJSON *w = cJSON_GetObjectItem(img, "width");
                    model->img_urls[i].width = (w && cJSON_IsNumber(w)) ? w->valueint : 0;
                    cJSON *h = cJSON_GetObjectItem(img, "height");
                    model->img_urls[i].height = (h && cJSON_IsNumber(h)) ? h->valueint : 0;
                } else {
                    model->img_urls[i].img_url = NULL;
                    model->img_urls[i].width = 0;
                    model->img_urls[i].height = 0;
                }
            }
        }
    }

    // child_list
    cJSON *child_arr = cJSON_GetObjectItem(root, "child_list");
    if (child_arr && cJSON_IsArray(child_arr)) {
        model->child_count = cJSON_GetArraySize(child_arr);
        if (model->child_count > 0) {
            model->child_list = (esl_base_model_child_t*)calloc(model->child_count, sizeof(esl_base_model_child_t));
            for (int i = 0; i < model->child_count; ++i) {
                cJSON *child = cJSON_GetArrayItem(child_arr, i);
                esl_base_model_child_t *c = &model->child_list[i];
                if (child && cJSON_IsObject(child)) {
                    cJSON *type = cJSON_GetObjectItem(child, "type");
                    c->type = (type && cJSON_IsNumber(type)) ? type->valueint : 0;

                    cJSON *content = cJSON_GetObjectItem(child, "content");
                    if(c->type == 1){
                        if(content && cJSON_IsString(content)){
                            char * temp = content->valuestring;
                            if (!starts_with(temp, "http")) {
                                // 处理以 "pro" 开头的属性
                                char url[256];
                                memset(url,0,256);
                                char * http_url = get_http_url();
                                strcat(url, http_url);
                                strcat(url, "/");
                                strcat(url, temp);
                                QM_ESL2_LOG("url : %s ===========================================",url);
                                c->content = strdup(url);
                            }else{
                                c->content = strdup(content->valuestring);
                            }
                        } else {
                            c->content = strdup(""); // 如果不是字符串，设置为空字符串
                        }
                    }else{
                        c->content = (content && cJSON_IsString(content)) ? strdup(content->valuestring) : strdup("");
                    }

                    cJSON *color = cJSON_GetObjectItem(child, "color");
                    c->color = (color && cJSON_IsString(color)) ? strdup(color->valuestring) : strdup("");

                    cJSON *fontSize = cJSON_GetObjectItem(child, "fontSize");
                    c->fontSize = (fontSize && cJSON_IsNumber(fontSize)) ? fontSize->valueint : 0;

                    cJSON *textAlign = cJSON_GetObjectItem(child, "textAlign");
                    c->textAlign = (textAlign && cJSON_IsString(textAlign)) ? strdup(textAlign->valuestring) : strdup("");

                    cJSON *fontFamily = cJSON_GetObjectItem(child, "fontFamily");
                    c->fontFamily = (fontFamily && cJSON_IsString(fontFamily)) ? strdup(fontFamily->valuestring) : strdup("");

                    cJSON *bgColor = cJSON_GetObjectItem(child, "bgColor");
                    c->bgColor = (bgColor && cJSON_IsString(bgColor)) ? strdup(bgColor->valuestring) : strdup("");

                    cJSON *textDecoration = cJSON_GetObjectItem(child, "textDecoration");
                    c->textDecoration = (textDecoration && cJSON_IsString(textDecoration)) ? strdup(textDecoration->valuestring) : strdup("");

                    cJSON *borderRadius = cJSON_GetObjectItem(child, "borderRadius");
                    c->borderRadius = (borderRadius && cJSON_IsNumber(borderRadius)) ? (float)borderRadius->valuedouble : 0;

                    cJSON *borderWidth = cJSON_GetObjectItem(child, "borderWidth");
                    c->borderWidth = (borderWidth && cJSON_IsNumber(borderWidth)) ? (float)borderWidth->valuedouble : 0;

                    cJSON *borderColor = cJSON_GetObjectItem(child, "borderColor");
                    c->borderColor = (borderColor && cJSON_IsString(borderColor)) ? strdup(borderColor->valuestring) : strdup("");

                    cJSON *x = cJSON_GetObjectItem(child, "x");
                    c->x = (x && cJSON_IsNumber(x)) ? x->valueint : 0;

                    cJSON *y = cJSON_GetObjectItem(child, "y");
                    c->y = (y && cJSON_IsNumber(y)) ? y->valueint : 0;

                    cJSON *w = cJSON_GetObjectItem(child, "w");
                    c->w = (w && cJSON_IsNumber(w)) ? w->valueint : 0;

                    cJSON *h = cJSON_GetObjectItem(child, "h");
                    c->h = (h && cJSON_IsNumber(h)) ? h->valueint : 0;

                    cJSON *angle = cJSON_GetObjectItem(child, "angle");
                    c->angle = (angle && cJSON_IsNumber(angle)) ? (float)angle->valuedouble : 0;

                    cJSON *opacity = cJSON_GetObjectItem(child, "opacity");
                    c->opacity = (opacity && cJSON_IsNumber(opacity)) ? (float)opacity->valuedouble : 0;

                    cJSON *padding = cJSON_GetObjectItem(child, "padding");
                    c->padding = (padding && cJSON_IsNumber(padding)) ? padding->valueint : 0;

                    cJSON *intFontSize = cJSON_GetObjectItem(child, "intFontSize");
                    c->intFontSize = (intFontSize && cJSON_IsNumber(intFontSize)) ? intFontSize->valueint : 0;

                    cJSON *floatFontSize = cJSON_GetObjectItem(child, "floatFontSize");
                    c->floatFontSize = (floatFontSize && cJSON_IsNumber(floatFontSize)) ? floatFontSize->valueint : 0;

                    cJSON *numFloatStyle = cJSON_GetObjectItem(child, "numFloatStyle");
                    c->numFloatStyle = (numFloatStyle && cJSON_IsString(numFloatStyle)) ? strdup(numFloatStyle->valuestring) : strdup("");
                } else {
                    memset(c, 0, sizeof(esl_base_model_child_t));
                }
            }
        }
    }

    cJSON_Delete(root);
    return model;
}


esl_message_model_t * the_second_to_message_model(const char * message){
     if(is_string_empty(message)){
        return NULL;
    }

    cJSON *root = cJSON_Parse(message);
    if (!root) {
        QM_ESL2_LOG("Failed to parse JSON\n");
        return NULL;
    }
    esl_message_model_t *model = (esl_message_model_t *)malloc(sizeof(esl_message_model_t));
    if (!model) {
        QM_ESL2_LOG("Failed to allocate memory for esl_message_model_t\n");
        cJSON_Delete(root);
        return NULL;
    }
    memset(model, 0, sizeof(esl_message_model_t)); //初始化内存

    cJSON *id = cJSON_GetObjectItem(root, "id");
    model->message_id = (id && cJSON_IsString(id)) ? strdup(id->valuestring) : strdup("");

    cJSON *deviceId = cJSON_GetObjectItem(root, "deviceId");
    model->device_no = (deviceId && cJSON_IsString(deviceId)) ? strdup(deviceId->valuestring) : strdup("");

    cJSON *data = cJSON_GetObjectItem(root, "data");
    model->data = (data) ? cJSON_PrintUnformatted(data) : strdup("");

    cJSON *type = cJSON_GetObjectItem(root, "type");
    char *temp = (type && cJSON_IsString(type)) ? type->valuestring : "";
    if(strcmp(temp,"content") == 0){
        cJSON *unbind = cJSON_GetObjectItem(data,"unbind");
        if(unbind != NULL && cJSON_IsNumber(unbind) && unbind->valueint == 1){
            //设备解绑 删除数据，还需要删除本地文件
            model->type = EslMessageTypeUNBIND;
            model->ignore = 0;
        }else{
            //模版更新
            model->type = EslMessageTypeCONTENT;
            model->ignore = 0;
        }
    }else if(strcmp(temp,"restart") == 0){
        //设备重启
        model->type = EslMessageTypeRESTRAT;
        model->ignore = 0;
    }else if(strcmp(temp,"refresh") == 0){
        //刷新
        model->type = EslMessageTypeREFRESH;
        model->ignore = 0;
    }else if(strcmp(temp,"wifi") == 0){
        //wifi修改
        model->type = EslMessageTypeWIFI;
        model->ignore = 0;
    }else if(strcmp(temp,"group") == 0){
        //分组修改
        model->type = EslMessageTypeGROUP;
        model->ignore = 0;
    }else if(strcmp(temp,"checkUpdate") == 0){
        //更新
        model->type = EslMessageTypeUPDATE;
        model->ignore = 0;
    }else if(strcmp(temp,"unbind") == 0){
        //解绑
        model->type = EslMessageTypeUNBIND;
        model->ignore = 0;
    }else if(strcmp(temp,"baseUrl") == 0){
        //基础url修改 暂不支持
        model->type = EslMessageTypeURL;
        model->ignore = 1;
    }else if(strcmp(temp,"brightness") == 0){
        //亮度 暂不支持
        model->type = EslMessageTypeBRIGHTNESS;
        model->ignore = 1;
    }else if(strcmp(temp,"screenSwitch") == 0){
        //屏幕开关 暂不支持
        model->type = EslMessageTypeSCREENSWITCH;
        model->ignore = 0;
    }else if(strcmp(temp,"language") == 0){
        //语言 暂不支持
        model->type = EslMessageTypeLANGUAGE;
        model->ignore = 1;
    }else{
        //全都不匹配
        model->type = EslMessageTypeLANGUAGE;
        model->ignore = 1;
    }
    

    cJSON_Delete(root);
    return model;
}

void free_esl_message_model(esl_message_model_t *model){
    if (!model) return;
    if (model->message_id) free(model->message_id);
    if (model->device_no) free(model->device_no);
    if (model->data) free(model->data);
    free(model);
}

esl_message_model_t * parse_esl_message_model(const char * message){
    if (is_string_empty(message)){
        return NULL;
    } 
    cJSON *root = cJSON_Parse(message);
    if (!root){
        return NULL;
    } 

    esl_message_model_t *model = (esl_message_model_t*)calloc(1, sizeof(esl_message_model_t));
    if (!model) {
        cJSON_Delete(root);
        return NULL;
    }
    memset(model, 0, sizeof(esl_message_model_t)); // 初始化内存

    cJSON *deviceHost = cJSON_GetObjectItem(root, "device_host");
    if(deviceHost && cJSON_IsString(deviceHost)){
        //说明是探活消息
        model -> type = EslMessageTypeCHECKLIFE;
        model -> data = strdup(deviceHost->valuestring);
        model -> ignore = 0;

        cJSON *deviceNo = cJSON_GetObjectItem(root, "device_no");
        model->device_no = (deviceNo && cJSON_IsString(deviceNo)) ? strdup(deviceNo->valuestring) : strdup("");
    }else{

        cJSON *messageId = cJSON_GetObjectItem(root, "message_id");
        model->message_id = (messageId && cJSON_IsString(messageId)) ? strdup(messageId->valuestring) : strdup("");
        
        cJSON *deviceNo = cJSON_GetObjectItem(root, "device_no");
        model->device_no = (deviceNo && cJSON_IsString(deviceNo)) ? strdup(deviceNo->valuestring) : strdup("");

        cJSON *type = cJSON_GetObjectItem(root, "type");
        model->type = (type && cJSON_IsNumber(type)) ? type->valueint : 0;

        cJSON *data = cJSON_GetObjectItem(root, "data");
        model->data = (data && cJSON_IsString(data)) ? strdup(data->valuestring) : strdup("");

        cJSON *ignore = cJSON_GetObjectItem(root, "ignore");
        model->ignore = (ignore && cJSON_IsNumber(ignore)) ? ignore->valueint : 0;
    }

    cJSON_Delete(root);
    return model;
}

char * els_message_model_to_json(esl_message_model_t * model){
    if (!model) return NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "type", model->type);
    cJSON_AddNumberToObject(root, "ignore", model->ignore);
    cJSON_AddStringToObject(root, "message_id", model->message_id ? model->message_id : "");
    cJSON_AddStringToObject(root, "device_no", model->device_no ? model->device_no : "");
    cJSON_AddStringToObject(root, "data", model->data ? model->data : "");
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str; // 用完后记得 free
}