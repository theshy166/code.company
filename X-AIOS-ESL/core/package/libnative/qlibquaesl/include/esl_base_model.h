/**
 * @file esl_base_model.h
 * @brief Basic ESL (Electronic Shelf Label) model definitions
 */
#ifndef __ESL_BASE_MODEL__
#define __ESL_BASE_MODEL__
#include "lvgl/lvgl.h"

/**
 * @brief ESL component types enumeration
 */
typedef enum{
    EslCompentTypeIMAGE = 1,    /**< Image component */
    EslCompentTypeVIDEO = 2,    /**< Video component */
    EslCompentTypeQRCODE = 3,   /**< QR code component */
    EslCompentTypeBARCODE = 4,  /**< Barcode component */
    EslCompentTypeRECT = 5,     /**< Rectangle component */
    EslCompentTypeCIRCLE = 6,   /**< Circle component */
    EslCompentTypeLINE = 7,     /**< Line component */
    EslCompentTypePRICE = 8,    /**< Price component */
    EslCompentTypeTEXT = 9,     /**< Text component */
} EslCompentType;

/**
 * @brief ESL base model child element structure
 * Represents individual components in an ESL display (text, price, buttons, shapes, images, etc.)
 */
typedef struct esl_base_model_child{
    int type;                   /**< Component type: 1.image 2.video 3.qr_code 4.bar_code 5.rect 6.circle 7.line 8.price 9.text */
    char *content;              /**< Content value for non-graphic components */
    char *color;                /**< Text color in hex format (e.g., #333333) */
    int fontSize;               /**< Text font size */
    char *textAlign;            /**< Text alignment: left, center, right */
    char *fontFamily;           /**< Font family name */
    char *bgColor;              /**< Background color in hex format (e.g., #333333) */
    char *textDecoration;       /**< Text decoration: none, underline, overline, line-through */
    float borderRadius;         /**< Border or shape radius */
    float borderWidth;          /**< Border or shape width */
    char *borderColor;          /**< Border color in hex format (e.g., #333333) */
    int x;                      /**< X coordinate */
    int y;                      /**< Y coordinate */
    int w;                      /**< Width */
    int h;                      /**< Height */
    float angle;                /**< Rotation angle in degrees (e.g., 90 for clockwise 90 degrees) */
    float opacity;              /**< Opacity value (0.0 to 1.0) */
    int padding;                /**< Padding */
    int lineSpace;              /**< Line spacing */
    int letterSpace;            /**< Letter spacing */

    // Price-specific properties
    int intFontSize;            /**< Font size for integer part of price */
    int floatFontSize;          /**< Font size for decimal part of price */
    char *numFloatStyle;        /**< Decimal alignment style: cc(center-center), cb(center-bottom), ct(center-top) */

}esl_base_model_child_t;

/**
 * @brief ESL image structure
 */
typedef struct esl_img{
    char *img_url;              /**< Image URL */
    int width;                  /**< Image width */
    int height;                 /**< Image height */
}esl_img_t;

/**
 * @brief ESL base model structure
 * Represents the overall ESL display template
 */
typedef struct esl_base_model{
    int width;                  /**< Template width */
    int height;                 /**< Template height */
    char *color;                /**< Color in hex format (e.g., #333333) */
    int opacity;                /**< Opacity value (0.0 to 1.0) */
    char *bgColor;              /**< Background color in hex format (e.g., #333333) */
    char **video_urls;          /**< Video URLs (non-null when video template exists) */
    int video_count;            /**< Number of videos */
    esl_img_t *img_urls;        /**< List of images to download */
    int img_count;              /**< Number of images */
    int child_count;            /**< Number of child components */
    esl_base_model_child_t *child_list;  /**< List of child components */
}esl_base_model_t;

#endif //__ESL_BASE_MODEL__