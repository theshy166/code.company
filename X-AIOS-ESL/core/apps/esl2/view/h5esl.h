/**
 * @file h5esl.h
 *
 */

#ifndef H5_ESL_H
#define H5_ESL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "xos_app_conf.h"

#if defined(CONFIG_XOS_APP_ESL2)

/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/



/***********************
 * GLOBAL VARIABLES
 ***********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void los_h5esl_entry(void);
void los_h5_esl_quit(void);
#ifdef CONFIG_XOS_FWK_APPMANAGER
void h5esl_init(void);
#endif
/*=====================
 * Setter functions
 *====================*/

/*=====================
 * Getter functions
 *====================*/


/*=====================
 * Other functions
 *====================*/


#endif  /*XOS_USE_APP_H5_ESL*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*H5_QTV_H*/
