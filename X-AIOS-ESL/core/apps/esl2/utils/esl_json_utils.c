#include "esl_json_utils.h"
#include "../conf/conf.h"

static char* format_price(const char* price_str) {
    if (!price_str || !*price_str) return strdup("");
    double price = atof(price_str);
    char buf[32];
    snprintf(buf, sizeof(buf), "%.2f", price);
    return strdup(buf);
}

esl_product_info_t ** parse_product_list_second(const char *json_string,int * count){
    cJSON *dataItem = cJSON_Parse(json_string);
    if (dataItem == NULL || !cJSON_IsArray(dataItem)) {
        QM_ESL2_LOG("Failed to get product_info JSON Data.");
        return NULL;
    }
    int array_size = cJSON_GetArraySize(dataItem);
    esl_product_info_t **product_info_list = (esl_product_info_t **)malloc(array_size * sizeof(esl_product_info_t *));
    if (product_info_list == NULL) {
        QM_ESL2_LOG("Failed to allocate memory for product_info_list.");
        return NULL;
    }
    memset(product_info_list, 0, array_size * sizeof(esl_product_info_t *)); // 初始化内存
    QM_ESL2_LOG("product_info_list:%p ", product_info_list);
    *count = array_size;

    for (int i = 0; i < array_size; i++) {
        esl_product_info_t *product = (esl_product_info_t *)malloc(sizeof(esl_product_info_t));
        if (product == NULL) {
            QM_ESL2_LOG("Failed to allocate memory for product_info.\n");
            for (int j = 0; j < i; j++) {
                free(product_info_list[j]);
            }
            free(product_info_list);
            return NULL;
        }
        memset(product, 0, sizeof(esl_product_info_t)); // 初始化内存
        cJSON *item = cJSON_GetArrayItem(dataItem, i);

        product->mch_id = strdup(cJSON_GetObjectItem(item, "mch_id") ? cJSON_GetObjectItem(item, "mch_id")->valuestring : "");
        product->mall_id = strdup(cJSON_GetObjectItem(item, "mall_id") ? cJSON_GetObjectItem(item, "mall_id")->valuestring : "");
        product->pro_id = strdup(cJSON_GetObjectItem(item, "pro_id") ? cJSON_GetObjectItem(item, "pro_id")->valuestring : "");
        product->pro_name = strdup(cJSON_GetObjectItem(item, "pro_name") ? cJSON_GetObjectItem(item, "pro_name")->valuestring : "");
        product->pro_unit = strdup(cJSON_GetObjectItem(item, "pro_unit") ? cJSON_GetObjectItem(item, "pro_unit")->valuestring : "");
        product->pro_spec = strdup(cJSON_GetObjectItem(item, "pro_spec") ? cJSON_GetObjectItem(item, "pro_spec")->valuestring : "");
        product->pro_adr = strdup(cJSON_GetObjectItem(item, "pro_adr") ? cJSON_GetObjectItem(item, "pro_adr")->valuestring : "");
        product->sale_price = format_price(cJSON_GetObjectItem(item, "sale_price") ? cJSON_GetObjectItem(item, "sale_price")->valuestring : "0");
        product->vip_price = format_price(cJSON_GetObjectItem(item, "vip_price") ? cJSON_GetObjectItem(item, "vip_price")->valuestring : "0");
        product->pf_price = format_price(cJSON_GetObjectItem(item, "pf_price") ? cJSON_GetObjectItem(item, "pf_price")->valuestring : "0");
        product->dis_price = format_price(cJSON_GetObjectItem(item, "dis_price") ? cJSON_GetObjectItem(item, "dis_price")->valuestring : "0");
        product->crt_day = strdup(cJSON_GetObjectItem(item, "crt_day") ? cJSON_GetObjectItem(item, "crt_day")->valuestring : "");
        product->exp_day = strdup(cJSON_GetObjectItem(item, "exp_day") ? cJSON_GetObjectItem(item, "exp_day")->valuestring : "");
        product->exp_day_num = strdup(cJSON_GetObjectItem(item, "exp_day_num") ? cJSON_GetObjectItem(item, "exp_day_num")->valuestring : "");
        product->img_urls = strdup(cJSON_GetObjectItem(item, "img_urls") ? cJSON_GetObjectItem(item, "img_urls")->valuestring : "");
        product->video_urls = strdup(cJSON_GetObjectItem(item, "video_urls") ? cJSON_GetObjectItem(item, "video_urls")->valuestring : "");
        product->ctm_txt1 = strdup(cJSON_GetObjectItem(item, "ctm_txt1") ? cJSON_GetObjectItem(item, "ctm_txt1")->valuestring : "");
        product->ctm_txt2 = strdup(cJSON_GetObjectItem(item, "ctm_txt2") ? cJSON_GetObjectItem(item, "ctm_txt2")->valuestring : "");
        product->ctm_txt3 = strdup(cJSON_GetObjectItem(item, "ctm_txt3") ? cJSON_GetObjectItem(item, "ctm_txt3")->valuestring : "");
        product->ctm_txt4 = strdup(cJSON_GetObjectItem(item, "ctm_txt4") ? cJSON_GetObjectItem(item, "ctm_txt4")->valuestring : "");
        product->ctm_txt5 = strdup(cJSON_GetObjectItem(item, "ctm_txt5") ? cJSON_GetObjectItem(item, "ctm_txt5")->valuestring : "");
        product->ctm_txt6 = strdup(cJSON_GetObjectItem(item, "ctm_txt6") ? cJSON_GetObjectItem(item, "ctm_txt6")->valuestring : "");
        product->state_id = strdup(cJSON_GetObjectItem(item, "state_id") ? cJSON_GetObjectItem(item, "state_id")->valuestring : "");
        product->state_lng = strdup(cJSON_GetObjectItem(item, "state_lng") ? cJSON_GetObjectItem(item, "state_lng")->valuestring : "");
        product->remark = strdup(cJSON_GetObjectItem(item, "remark") ? cJSON_GetObjectItem(item, "remark")->valuestring : "");
        product->sort_id = strdup(cJSON_GetObjectItem(item, "sort_id") ? cJSON_GetObjectItem(item, "sort_id")->valuestring : "");
        product->rec_yn_id = strdup(cJSON_GetObjectItem(item, "rec_yn_id") ? cJSON_GetObjectItem(item, "rec_yn_id")->valuestring : "");
        product->rec_yn_lng = strdup(cJSON_GetObjectItem(item, "rec_yn_lng") ? cJSON_GetObjectItem(item, "rec_yn_lng")->valuestring : "");
        product->hot_yn_id = strdup(cJSON_GetObjectItem(item, "hot_yn_id") ? cJSON_GetObjectItem(item, "hot_yn_id")->valuestring : "");
        product->hot_yn_lng = strdup(cJSON_GetObjectItem(item, "hot_yn_lng") ? cJSON_GetObjectItem(item, "hot_yn_lng")->valuestring : "");
        product->chg_time = strdup(cJSON_GetObjectItem(item, "chg_time") ? cJSON_GetObjectItem(item, "chg_time")->valuestring : "");
        product->RC = strdup(cJSON_GetObjectItem(item, "RC") ? cJSON_GetObjectItem(item, "RC")->valuestring : "");
        product->select = cJSON_IsTrue(cJSON_GetObjectItem(item, "select"));
        
        product_info_list[i] = product;
    }
    return product_info_list;
}

esl_other_info_t *parse_other_info_second(const char *json_string){
    cJSON *root = cJSON_Parse(json_string);
    if (!root) {
        QM_ESL2_LOG("Failed to parse JSON\n");
        return NULL;
    }

    esl_other_info_t *other_info = (esl_other_info_t *)malloc(sizeof(esl_other_info_t));
    if (!other_info) {
        QM_ESL2_LOG("Failed to allocate memory for esl_other_info_t\n");
        cJSON_Delete(root);
        return NULL;
    }
    memset(other_info, 0, sizeof(esl_other_info_t)); // 初始化内存

    // 解析 bg 部分
    cJSON *priceType = cJSON_GetObjectItem(root, "priceType");
    other_info->priceType = (priceType && cJSON_IsString(priceType) && priceType->valuestring) ? strdup(priceType->valuestring) : strdup("");

    cJSON *priceUnit = cJSON_GetObjectItem(root, "priceUnit");
    other_info->priceUnit = (priceUnit && cJSON_IsString(priceUnit) && priceUnit->valuestring) ? strdup(priceUnit->valuestring) : strdup("");

    cJSON *priceColor = cJSON_GetObjectItem(root, "priceColor");
    other_info->priceColor = (priceColor && cJSON_IsString(priceColor) && priceColor->valuestring) ? strdup(priceColor->valuestring) : strdup("");

    cJSON *txt1 = cJSON_GetObjectItem(root, "txt1");
    other_info->txt1 = (txt1 && cJSON_IsString(txt1) && txt1->valuestring) ? strdup(txt1->valuestring) : strdup("");

    cJSON *txt2 = cJSON_GetObjectItem(root, "txt2");
    other_info->txt2 = (txt2 && cJSON_IsString(txt2) && txt2->valuestring) ? strdup(txt2->valuestring) : strdup("");

    cJSON *txt3 = cJSON_GetObjectItem(root, "txt3");
    other_info->txt3 = (txt3 && cJSON_IsString(txt3) && txt3->valuestring) ? strdup(txt3->valuestring) : strdup("");

    cJSON *txt4 = cJSON_GetObjectItem(root, "txt4");
    other_info->txt4 = (txt4 && cJSON_IsString(txt4) && txt4->valuestring) ? strdup(txt4->valuestring) : strdup("");

    cJSON *txt5 = cJSON_GetObjectItem(root, "txt5");
    other_info->txt5 = (txt5 && cJSON_IsString(txt5) && txt5->valuestring) ? strdup(txt5->valuestring) : strdup("");

    cJSON *txt6 = cJSON_GetObjectItem(root, "txt6");
    other_info->txt6 = (txt6 && cJSON_IsString(txt6) && txt6->valuestring) ? strdup(txt6->valuestring) : strdup("");

    cJSON *ctmTxt = cJSON_GetObjectItem(root, "ctmTxt");
    other_info->ctmTxt = (ctmTxt && cJSON_IsString(ctmTxt) && ctmTxt->valuestring) ? strdup(ctmTxt->valuestring) : strdup("");

    cJSON_Delete(root);
    return other_info;
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

    // 释放其他字段
    if (product_info->mch_id) free(product_info->mch_id);
    if (product_info->mall_id) free(product_info->mall_id);
    if (product_info->pro_id) free(product_info->pro_id);
    if (product_info->pro_name) free(product_info->pro_name);
    if (product_info->pro_unit) free(product_info->pro_unit);
    if (product_info->pro_spec) free(product_info->pro_spec);
    if (product_info->pro_adr) free(product_info->pro_adr);
    if (product_info->pf_price) free(product_info->pf_price);
    if (product_info->dis_price) free(product_info->dis_price);
    if (product_info->crt_day) free(product_info->crt_day);
    if (product_info->exp_day) free(product_info->exp_day);
    if (product_info->exp_day_num) free(product_info->exp_day_num);
    if (product_info->img_urls) free(product_info->img_urls);
    if (product_info->video_urls) free(product_info->video_urls);
    if (product_info->ctm_txt1) free(product_info->ctm_txt1);
    if (product_info->ctm_txt2) free(product_info->ctm_txt2);
    if (product_info->ctm_txt3) free(product_info->ctm_txt3);
    if (product_info->ctm_txt4) free(product_info->ctm_txt4);
    if (product_info->ctm_txt5) free(product_info->ctm_txt5);
    if (product_info->ctm_txt6) free(product_info->ctm_txt6);
    if (product_info->state_id) free(product_info->state_id);
    if (product_info->state_lng) free(product_info->state_lng);
    if (product_info->remark) free(product_info->remark);
    if (product_info->sort_id) free(product_info->sort_id);
    if (product_info->rec_yn_id) free(product_info->rec_yn_id);
    if (product_info->rec_yn_lng) free(product_info->rec_yn_lng);
    if (product_info->hot_yn_id) free(product_info->hot_yn_id);
    if (product_info->hot_yn_lng) free(product_info->hot_yn_lng);
    if (product_info->chg_time) free(product_info->chg_time);
    if (product_info->RC) free(product_info->RC);
    // 释放 product_info 本身

    free(product_info);
}

void free_esl_product_info_array(esl_product_info_t **product_info_list, int count){
    if (product_info_list == NULL) {
        return;
    }

    for (int i = 0; i < count; i++) {
        _free_esl_product_info(product_info_list[i]);
    }

    free(product_info_list);
}

void free_esl_other_info(esl_other_info_t *info){
    if (!info) return;
    if (info->priceType) free(info->priceType);
    if (info->priceUnit) free(info->priceUnit);
    if (info->priceColor) free(info->priceColor);
    if (info->txt1) free(info->txt1);
    if (info->txt2) free(info->txt2);
    if (info->txt3) free(info->txt3);
    if (info->txt4) free(info->txt4);
    if (info->txt5) free(info->txt5);
    if (info->txt6) free(info->txt6);
    if (info->ctmTxt) free(info->ctmTxt);
    free(info);
}