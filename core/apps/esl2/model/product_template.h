#ifndef __PRODUCT_TEMPLATE_H__
#define __PRODUCT_TEMPLATE_H__

#include <stdbool.h>

// 定义背景样式结构体
typedef struct {
    int width;                // 背景宽度
    int height;               // 背景高度
    int scale;                // 缩放比例
    char *color;              // 背景颜色
    float opacity;            // 背景透明度
    char *background;         // 背景类型
    int fontSize;             // 字体大小
    char *backgroundRepeat;   // 背景重复方式
    char *backgroundSize;     // 背景尺寸
    char *backgroundPosition; // 背景位置
    char *backgroundImage;    // 背景图片链接
} product_template_bg_t;

// 定义链接样式结构体
typedef struct {
    char *key;   // 样式键
    char *value; // 样式值
} product_template_linkage_style_t;

typedef struct {
    char *id;                       // 链接ID
    char *label;                    // 链接标签
    char *event;                    // 链接事件
    product_template_linkage_style_t *style; // 样式数组
    int style_count;                // 样式数量
} product_template_linkage_t;

// 定义组件样式结构体
typedef struct {
    int rotate;              // 旋转角度
    float opacity;           // 透明度
    float transform;         // 透明度
    int width;               // 宽度
    int height;              // 高度
    int fontSize;            // 字体大小
    int fontWeight;          // 字体粗细
    char *fontFamily;        // 字体粗细
    int padding;             // 字体粗细
    char *lineHeight;        // 行高
    int letterSpacing;       // 字母间距
    char *textAlign;         // 文本对齐方式
    char *color;             // 颜色
    char *borderColor;       // 边框颜色
    int borderWidth;         // 边框宽度
    char *backgroundColor;   // 背景颜色
    char *borderStyle;       // 边框样式
    char *borderRadius;      // 圆角半径
    char *verticalAlign;     // 垂直对齐方式
    int top;                 // 顶部位置
    int left;                // 左侧位置
} product_template_component_style_t;

// 定义组件结构体
typedef struct {
    char *component;                          // 组件类型
    char *label;                              // 组件标签
    char *propValue;                          // 属性值
    char *icon;                               // 图标
    char *propAssPro;                         // 属性绑定
    char *propAssProAttr;                     // 属性绑定细节
    char *usePropValue;                       // 是否使用属性值
    bool isAttr;                              // 是否为属性
    product_template_component_style_t style; // 样式
    char *id;                                 // 组件ID
    product_template_linkage_t linkage;       // 链接信息
} product_template_body_t;

// 定义产品模板结构体
typedef struct {
    product_template_bg_t bg;           // 背景样式
    product_template_body_t *body;      // 组件数组
    int body_count;                     // 组件数量
} product_template_t;

#endif // __PRODUCT_TEMPLATE_H__