#ifndef __ESL_PRODUCT_INFO_H__
#define __ESL_PRODUCT_INFO_H__
#include <stdbool.h>

typedef struct {
    char * item_no;//商品编号
    char * item_subno;//商品子编号
    char * item_name;//商品名称
    char * item_subname;//商品子名称
    char * main_supcust;//主供应商
    char * item_clsno;//商品分类
    char * combine_sta;//组合状态
    char * item_sup_flag;//商品供应商标识
    char * item_stock;//商品库存
    char * price;//商品价格
    float base_price;//商品基础价格
    char * sale_price;//商品销售价格
    char * vip_price;//商品会员价格
    int num2;//商品数量2
    int num3;//商品数量3
    char * item_rem;//商品备注
    char * unit_no;//商品单位
    char * item_size;//商品规格
    char * product_area;//商品产地
    char * build_date;//商品创建时间
    char * modify_date;//商品修改时间
    char * item_url;//商品图片
    int purchase_spec;//商品采购规格
    char * status;//商品状态
    int min_opqty;//商品最小采购量
    char * item_video;//商品视频
    char * main_supname;//自采默认


    char *mch_id;          // 商户ID
    char *mall_id;         // 商场ID
    char *pro_id;          // 产品ID
    char *pro_name;        // 产品名称
    char *pro_unit;        // 产品单位
    char *pro_spec;        // 产品规格
    char *pro_adr;         // 产品产地
    //char *sale_price;      // 销售价
    //char *vip_price;       // 会员价
    char *pf_price;        // 批发价
    char *dis_price;       // 折扣价
    char *crt_day;         // 创建日期
    char *exp_day;         // 到期日期
    char *exp_day_num;     // 保质期天数
    char *img_urls;        // 图片链接
    char *video_urls;      // 视频链接
    char *ctm_txt1;        // 自定义文本1
    char *ctm_txt2;        // 自定义文本2
    char *ctm_txt3;        // 自定义文本3
    char *ctm_txt4;        // 自定义文本4
    char *ctm_txt5;        // 自定义文本5
    char *ctm_txt6;        // 自定义文本6
    char *state_id;        // 状态ID
    char *state_lng;       // 状态描述
    char *remark;          // 备注
    char *sort_id;         // 排序ID
    char *rec_yn_id;       // 推荐状态ID
    char *rec_yn_lng;      // 推荐状态描述
    char *hot_yn_id;       // 热销状态ID
    char *hot_yn_lng;      // 热销状态描述
    char *chg_time;        // 修改时间
    char *RC;              // RC字段
    bool select;           // 是否选中
} esl_product_info_t;

struct esl_product_item_key_t{
    char * product_item_key;
    int count;
};

struct hash_map_key_t{
    char * key;
    char * value;
};

#endif //__ESL_PRODUCT_INFO_H__