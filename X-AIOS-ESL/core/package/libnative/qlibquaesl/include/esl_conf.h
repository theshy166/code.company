
/**
 * @file esl_conf.h
 * @brief ESL (Electronic Shelf Label) configuration definitions
 */
#ifndef __H5_ESL_CONF_H__
#define __H5_ESL_CONF_H__

/**
 * @brief Resource free rate threshold for clearing unused resources
 * Value: 0.4f (40% free rate)
 */
#define CLEAR_RESOURCE_FREE_RATE 0.4f

/**
 * @brief Path configurations for different build environments
 */
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
    /** Path configurations for simulator build */
    #define k_path_prefix        "H:./out/simulator/xos/res"   /**< Base resource path for simulator */
    #define k_path_ttf           "H:./out/simulator/xos/res/ttf/2312_v9.ttf"  /**< Default TTF font path for simulator */
    #define k_path_impact_ttf    "H:./out/simulator/xos/res/ttf/impact-2.ttf"  /**< Impact TTF font path for simulator */
    #define k_path_img_prefix    "./out/simulator/xos/res/img/"  /**< Image resource prefix path for simulator */

#else
    /** Path configurations for target device build */
    #define k_path_prefix        "H:/res"   /**< Base resource path for target device */
    #define k_path_ttf           "H:/res/ttf/2312_v9.ttf"  /**< Default TTF font path for target device */
    #define k_path_impact_ttf    "H:/res/esl2/ttf/impact-2.ttf"  /**< Impact TTF font path for target device */
    #define k_path_img_prefix    "/data/res/img/"  /**< Image resource prefix path for target device */
#endif

/**
 * @brief Template image paths for Mid-Autumn Festival theme
 */
#define k_path_template_midautumn_0    k_path_prefix"/esl2/skin/216c0779ea7118825ca42478ff3a018b_0.png"  /**< Mid-Autumn theme template image 0 */
#define k_path_template_midautumn_1    k_path_prefix"/esl2/skin/216c0779ea7118825ca42478ff3a018b_1.png"  /**< Mid-Autumn theme template image 1 */

#endif
