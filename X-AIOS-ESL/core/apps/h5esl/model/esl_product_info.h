#ifndef __ESL_PRODUCT_INFO_H__
#define __ESL_PRODUCT_INFO_H__

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