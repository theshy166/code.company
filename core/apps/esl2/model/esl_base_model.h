#ifndef __ESL_BASE_MODEL__
#define __ESL_BASE_MODEL__
#include "lvgl/lvgl.h"

typedef enum{
    EslCompentTypeIMAGE = 1,
    EslCompentTypeVIDEO = 2,
    EslCompentTypeQRCODE = 3,
    EslCompentTypeBARCODE = 4,
    EslCompentTypeRECT = 5,
    EslCompentTypeCIRCLE = 6,
    EslCompentTypeLINE = 7,
    EslCompentTypePRICE = 8,
    EslCompentTypeTEXT = 9,
} EslCompentType;

typedef struct esl_base_model_child{
    //text(文本) price(价格) button(按钮) rect(矩形) circle(圆形) image(图片) bar_code(条形码) qr_code(二维码) video(视频）
    // Picture VText RectShape CircleShape LineShape VQrCode VBarCode VVideo
    int type;//1.image 2.video 3.qr_code 4.bar_code 5.rect 6.circle 7.line 8.price 9.text
    char *content;//除了图形和背景 其他都应该存在这个值
    char * color;//文本颜色 #333333
    int fontSize;//文本字号
    char *textAlign;//文本对齐方式(left\center\right)
    char *fontFamily;//字体
    char * bgColor;//背景颜色 #333333
    char *textDecoration;//none 默认 underline(下划线) overline(上划线) line-through(删除线)
    float borderRadius;//边框或图形圆角半径
    float borderWidth;//边框或图形圆角宽度
    char * borderColor;// 边框颜色 #333333
    int x;//x坐标
    int y;//y坐标
    int w;//宽
    int h;//高
    float angle;//旋转角度 按正常度数显示  如 顺时针旋转90度  即90, 不满足的自行转换
    float opacity;// 透明度 取值 0~1，不满足的自行转换
    int padding;
    int lineSpace;//行间距
    int letterSpace;//字符间距


    //针对价格 使用到的属性
    int intFontSize;// 整数字号
    int floatFontSize;// 小数字号
    char * numFloatStyle;// 小数对齐样式 cc(整小数居中) cb(小数在右下角) ct(小数在右上角)

}esl_base_model_child_t;

typedef struct esl_img{
    char * img_url;
    int width;
    int height;
}esl_img_t;

typedef struct esl_base_model{
    int width;// 模版宽度
    int height;// 模版高度
    char * color;//颜色 #333333
    int opacity;//透明度 取值 0~1，不满足的自行转换
    char * bgColor;//背景颜色 #333333
    char **video_urls;//存在视频模版时  该值不为空
    int video_count;//视频数量
    esl_img_t * img_urls;//需要下载的图片列表
    int img_count;//图片的数据
    int child_count;//子元素集合
    esl_base_model_child_t * child_list;
}esl_base_model_t;

#endif //__ESL_BASE_MODEL__