#ifndef __ESL_SHOW_MODEL_H__
#define __ESL_SHOW_MODEL_H__


typedef struct {
    char * state;//返回结果，Done：成功；Fail：失败
    char * message;//失败原因
    char * data;//json字符串	
} esl_simple_response_t;

typedef struct {
    /* 文本、按钮属性
    text：默认文本内容
    color：文本颜色 如 #333333
    fontSize：文本字号
    fontWeight：文本粗细（normal：默认；bold：加粗）
    textAlign：文本对齐方式（left、center、right）
    fontFamily：字体
    bgColor:背景色
    borderRadius:边框或图形圆角半径 */
    char * text;//文本内容
    char * color;//文本颜色 如 #333333
    char * fontSize;//文本字号
    char * fontWeight;//文本粗细（normal：默认；bold：加粗）
    char * textAlign;//文本对齐方式（left、center、right）
    char * fontFamily;//字体
    char * bgColor;//背景色
    float borderRadius;//边框或图形圆角半径
    float borderWidth;//边框或图形圆角宽度

    /* 价格属性见extension*/

    /* 矩形、圆形属性
    bgColor：背景色
    borderRadius：圆角半径
    */

    /* 图片、条形码、二维码、视频属性
    src：默认链接（封面图）
    borderRadius：圆角半径
    */
    char * src;//默认链接（封面图）
} esl_show_model_child_desc_t;

typedef struct {
    //价格属性
    char * text;// 价格内容
    char * color;// 价格颜色
    int intFontSize;// 整数字号
    int floatFontSize;// 小数字号
    char * fontWeight;//文本粗细（normal：默认；bold：加粗）
    char * fontFamily;// 字体
    char * textAlign;// 文本对齐 left 、 center 、 right
    char * numFloatStyle;// 小数对齐样式 cc(整小数居中) cb(小数在右下角) ct(小数在右上角)
    char * fontStyle;//normal 默认 斜体 italic
    char * textDecoration;//none 默认 underline(下划线) overline(上划线) line-through(删除线)
    char * bgColor;// 背景色
} esl_show_model_child_extension_html_t;

typedef struct {
    //拓展属性
    char * ex_type;//text(文本) price(价格) button(按钮) rect(矩形) circle(圆形) image(图片) bar_code(条形码) qr_code(二维码) video(视频）
    //文本属性
    char * textDecoration;//underline(下划线) overline(上划线) line-through(删除线)
    //价格属性
    char * unit_price;//价格单位 kg、500g
    //图片属性
    char ** swiper_list;//轮播图列表
    int swiper_list_count;//轮播图数量
    //条形码、二维码、视频
    char * value;//默认值
    //绑定属性 bind：绑定属性见下方说明
    char * bind;//$item_unit
    esl_show_model_child_extension_html_t html;//价格属性
} esl_show_model_child_extension_t;

typedef struct {
    char * name;//模板名称
    float x;//x坐标
    float y;//y坐标
    float w;//宽
    float h;//高
    float angle;//旋转角度
    char * type;//元素通用类型搭配extension.ex_type细分元素类型 text:文本、按钮；html：价格；rect：矩形; circle:圆形; image：图片、条形码、二维码、视频
    esl_show_model_child_desc_t desc;//元素描述
    esl_show_model_child_extension_t extension;//元素扩展信息
    //operation 编辑属性 忽略
    char uuid[50];
    int edit;
} esl_show_model_child_t;

typedef struct {
    char * name;//模板名称
    char * resolution;//模板分辨率
    char * type;//模板类型
    int itemNum;//商品数量
    int status;//模板状态
    char * thumbnail;//模板封面图
    int width;//模板宽度 width、contextWidth
    int height;//模板高度 height、contextHeight
    int contextWidth;//模板宽度 width、contextWidth
    int contextHeight;//模板高度 height、contextHeight	
    float devicePixelRatio;//可忽略
    int onlyRender;//可忽略
    esl_show_model_child_t * child_list;//模板子元素集合
    int child_count;//子元素数量
} esl_show_model_t;

/* // 文本绑定对象
const textBindList = [
{
  label:'无',
  value:'',
},
{
  label:'商品名称',
  value:'$item_name',
},
{
  label:'商品条码',
  value:'$item_no',
},
{
  label:'商品自编码',
  value:'$item_subno',
},
{
  label:'规格',
  value:'$item_size',
},
{
  label:'单位',
  value:'$item_unit',
},
{
  label:'供应商',
  value:'$sup_name',
},
{
  label:'促销时间',
  value:'$cu_name',
},
{
  label:'产地',
  value:'$area_address',
},
{
  label:'保质天数',
  value:'$shop_exp',
},
{
  label:'设备序列号(SN)',
  value:'$device_sn',
},
{
  label:'设备ip',
  value:'$device_ip',
},
{
  label:'版本号',
  value:'$pos_version',
},
{
  label:'店名',
  value:'$store_name',
},
{
  label:'提示语',
  value:'$show_msg',
},
{
  label:'分辨率',
  value:'$resolution',
},
{
  label:'设备名称',
  value:'$device_name',
},
{
  label:'WIFI名称',
  value:'$wifi_SSID',
}
]
// 按钮绑定事件
const buttonBindList = [
{
  label:'无',
  value:'',
},
{
  label:'录入会员',
  value:'@vipLogin',
},
]
// 价格绑定对象
const priceBindList = [
{
  label:'不绑定',
  value:'',
},
{
  label:'零售价',
  value:'$sale_price',
},
{
  label:'会员价',
  value:'$vip_price',
},
{
  label:'原价',
  value:'$line_price'
},
{
  label:'促销价',
  value:'$cu_price',
},
{
  label:'进价',
  value:'$price',
}
]
// 图片绑定对象
const imageBindList = [
{
  label:'自定义图片',
  value:'',
},
{
  label:'商品图片',
  value:'$item_image',
},
{
  label:'广告图片',
  value:'$ad_image', // desc => src
},
{
  label:'轮播图',
  value:'$swiper',  // extension => swiper_list
},
]
// 视频绑定对象
const videoBindList = [
{
  label:'自定义视频',
  value:'',
},
{
  label:'商品视频',
  value:'$item_video',
}
]
// 条形码绑定对象
const barCodeBindList = [
{
  label:'自定义',
  value:'',
},
{
  label:'商品条码',
  value:'$bar_item_no',
},
{
  label:'商品自编码',
  value:'$bar_item_subno',
},
]
// 二维码绑定对象
const qrCodeBindList = [
{
  label:'自定义',
  value:'',
},
{
  label:'设备信息',
  value:'$device_info',
},
] */

#endif // __ESL_SHOW_MODEL_H__