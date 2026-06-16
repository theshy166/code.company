
#include "../../lcm_module/include/data_type.h"

/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/**---------------------------------------------------------------------------*
*                Micro Define                    **
**----------------------------------------------------------------------------*/
#define LCM_CTRL_DEBUG 0
/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef struct {
    int32_t lcm_dvdd;
    int32_t lcm_avdd;
    int32_t lcm_iovdd;
    int32_t lcm_vcom;
    int32_t lcm_vgh;
    int32_t lcm_vgl;
    int32_t lcm_reset;
    int32_t lcm_standby;
    int32_t lcm_back_light_gpio;
    //struct pwm_device *lcm_power_pwm;
    //struct pwm_device *lcm_back_light_pwm;
}lcm_ctrl_t;
/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
static lcm_ctrl_t s_lcm_ctrl = {
    .lcm_reset= -1,
    .lcm_dvdd= -1,
    .lcm_avdd= -1,
    .lcm_iovdd= -1,
    .lcm_vcom= -1,
    .lcm_vgh= -1,
    .lcm_vgl= -1,
    .lcm_standby= -1,
    .lcm_back_light_gpio= -1,
    //.lcm_power_pwm= NULL,
    //.lcm_back_light_pwm= NULL,
};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
lcm_ctrl_t *get_lcm_ctrl(uint32_t lcm_id)
{
    return &s_lcm_ctrl;
}

int32_t lcm_dvdd_set(uint32_t lcm_id, uint32_t val)
{
    int32_t rtn = 0;
#if 0
    lcm_ctrl_t *lcm_ctrl = get_lcm_ctrl(lcm_id);

    if(0 > lcm_ctrl->lcm_dvdd)
    {
        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find lcm node ~_~!\n");
        }

        lcm_ctrl->lcm_dvdd = of_get_named_gpio(np, "lcm_dvdd", 0);
        #if LCM_CTRL_DEBUG
        printk("%s:%d, lcm_dvdd:%d\n",__FUNCTION__,__LINE__, lcm_ctrl->lcm_dvdd);
        #endif
        if(0 <= lcm_ctrl->lcm_dvdd) {
            if (0 != gpio_direction_output(lcm_ctrl->lcm_dvdd, val)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set lcm_dvdd gpio dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "get lcm_dvdd gpio(%d) none ~_~!\n", lcm_ctrl->lcm_dvdd);
        }
    }

    if(0 <= lcm_ctrl->lcm_dvdd) {
        __gpio_set_value(lcm_ctrl->lcm_dvdd, val);
    }

EXIT:
#endif

    return rtn;
}

int32_t lcm_avdd_set(uint32_t lcm_id, uint32_t val)
{
    int32_t rtn = 0;
#if 0
    lcm_ctrl_t *lcm_ctrl = get_lcm_ctrl(lcm_id);

    if(0 > lcm_ctrl->lcm_avdd) {
        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find lcm node ~_~!\n");
        }

        lcm_ctrl->lcm_avdd = of_get_named_gpio(np, "lcm_avdd", 0);
        #if LCM_CTRL_DEBUG
        printk("%s:%d, lcm_avdd:%d\n",__FUNCTION__,__LINE__, lcm_ctrl->lcm_avdd);
        #endif
        if(0 <= lcm_ctrl->lcm_avdd) {
            if (0 != gpio_direction_output(lcm_ctrl->lcm_avdd, val)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set lcm_avdd gpio dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "get lcm_avdd gpio(%d) none ~_~!\n", lcm_ctrl->lcm_avdd);
        }
    }

    if(0 <= lcm_ctrl->lcm_avdd) {
        __gpio_set_value(lcm_ctrl->lcm_avdd, val);
    }

EXIT:
#endif

    return rtn;
}


int32_t lcm_iovdd_set(uint32_t lcm_id, uint32_t val)
{
    int32_t rtn = 0;
#if 0
    lcm_ctrl_t *lcm_ctrl = get_lcm_ctrl(lcm_id);

    if(0 > lcm_ctrl->lcm_iovdd) {
        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find lcm node ~_~!\n");
        }

        lcm_ctrl->lcm_iovdd = of_get_named_gpio(np, "lcm_iovdd", 0);
        #if LCM_CTRL_DEBUG
        printk("%s:%d, lcm_iovdd:%d\n",__FUNCTION__,__LINE__, lcm_ctrl->lcm_iovdd);
        #endif
        if(0 <= lcm_ctrl->lcm_iovdd) {
            if (0 != gpio_direction_output(lcm_ctrl->lcm_iovdd, val)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set lcm_iovdd gpio dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "get lcm_iovdd gpio(%d) none ~_~!\n", lcm_ctrl->lcm_iovdd);
        }
    }

    if(0 <= lcm_ctrl->lcm_iovdd) {
        __gpio_set_value(lcm_ctrl->lcm_iovdd, val);
    }

EXIT:
#endif

    return rtn;
}

int32_t lcm_vcom_set(uint32_t lcm_id, uint32_t val)
{
    int32_t rtn = 0;
#if 0
    lcm_ctrl_t *lcm_ctrl = get_lcm_ctrl(lcm_id);

    if(0 > lcm_ctrl->lcm_vcom) {
        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find lcm node ~_~!\n");
        }

        lcm_ctrl->lcm_vcom = of_get_named_gpio(np, "lcm_vcom", 0);
        #if LCM_CTRL_DEBUG
        printk("%s:%d, lcm_vcom:%d\n",__FUNCTION__,__LINE__, lcm_ctrl->lcm_vcom);
        #endif
        if(0 <= lcm_ctrl->lcm_vcom) {
            if (0 != gpio_direction_output(lcm_ctrl->lcm_vcom, val)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set lcm_vcom gpio dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "get lcm_vcom gpio(%d) none ~_~!\n", lcm_ctrl->lcm_vcom);
        }
    }

    if(0 <= lcm_ctrl->lcm_vcom) {
        __gpio_set_value(lcm_ctrl->lcm_vcom, val);
    }

EXIT:
#endif

    return rtn;
}

int32_t lcm_vgh_set(uint32_t lcm_id, uint32_t val)
{
    int32_t rtn = 0;
#if 0
    lcm_ctrl_t *lcm_ctrl = get_lcm_ctrl(lcm_id);

    if(0 > lcm_ctrl->lcm_vgh) {

        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find lcm node ~_~!\n");
        }

        lcm_ctrl->lcm_vgh = of_get_named_gpio(np, "lcm_vgh", 0);
        #if LCM_CTRL_DEBUG
        printk("%s:%d, lcm_vgh:%d\n",__FUNCTION__,__LINE__, lcm_ctrl->lcm_vgh);
        #endif
        if(0 <= lcm_ctrl->lcm_vgh) {
            if (0 != gpio_direction_output(lcm_ctrl->lcm_vgh, val)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set lcm_vgh gpio dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "get lcm_vgh gpio(%d) none ~_~!\n", lcm_ctrl->lcm_vgh);
        }
    }

    if(0 <= lcm_ctrl->lcm_vgh) {
        __gpio_set_value(lcm_ctrl->lcm_vgh, val);
    }

EXIT:
#endif
    return rtn;
}

int32_t lcm_vgl_set(uint32_t lcm_id, uint32_t val)
{
    int32_t rtn = 0;
#if 0
    lcm_ctrl_t *lcm_ctrl = get_lcm_ctrl(lcm_id);

    if(0 > lcm_ctrl->lcm_vgl) {
        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find lcm node ~_~!\n");
        }

        lcm_ctrl->lcm_vgl = of_get_named_gpio(np, "lcm_vgl", 0);
        #if LCM_CTRL_DEBUG
        printk("%s:%d, lcm_vgl:%d\n",__FUNCTION__,__LINE__, lcm_ctrl->lcm_vgl);
        #endif
        if(0 <= lcm_ctrl->lcm_vgl) {
            if (0 != gpio_direction_output(lcm_ctrl->lcm_vgl, val)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set lcm_vgl gpio dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "get lcm_vgl gpio(%d) none ~_~!\n", lcm_ctrl->lcm_vgl);
        }
    }

    if(0 <= lcm_ctrl->lcm_vgl) {
        __gpio_set_value(lcm_ctrl->lcm_vgl, val);
    }

EXIT:
#endif
    return rtn;
}

int32_t lcm_reset_set(uint32_t lcm_id, uint32_t val)
{
    int32_t rtn = 0;
#if 0
    lcm_ctrl_t *lcm_ctrl = get_lcm_ctrl(lcm_id);

    if(0 > lcm_ctrl->lcm_reset) {

        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find lcm node ~_~!\n");
        }

        lcm_ctrl->lcm_reset = of_get_named_gpio(np, "lcd_reset", 0);
        #if LCM_CTRL_DEBUG
        printk("%s:%d, lcm_reset:%d\n",__FUNCTION__,__LINE__, lcm_ctrl->lcm_reset);
        #endif
        if(0 <= lcm_ctrl->lcm_reset) {
            if (0 != gpio_direction_output(lcm_ctrl->lcm_reset, val)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set lcm_reset gpio dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "get lcm_reset gpio(%d) none ~_~!\n", lcm_ctrl->lcm_reset);
        }
    }

    if(0 <= lcm_ctrl->lcm_reset) {
        __gpio_set_value(lcm_ctrl->lcm_reset, val);
    }

EXIT:
#endif
    return rtn;
}

int32_t lcm_standby_set(uint32_t lcm_id, uint32_t val)
{
    int32_t rtn = 0;
#if 0
    lcm_ctrl_t *lcm_ctrl = get_lcm_ctrl(lcm_id);

    if(0 > lcm_ctrl->lcm_standby) {
        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find lcm node ~_~!\n");
        }

        lcm_ctrl->lcm_standby = of_get_named_gpio(np, "lcm_standby", 0);
        #if LCM_CTRL_DEBUG
        printk("%s:%d, lcm_standby:%d\n",__FUNCTION__,__LINE__, lcm_ctrl->lcm_standby);
        #endif

        if(0 <= lcm_ctrl->lcm_standby) {
            if (0 != gpio_direction_output(lcm_ctrl->lcm_standby, val)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set lcm_standby gpio dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "get lcm_standby gpio(%d) none ~_~!\n", lcm_ctrl->lcm_standby);
        }
    }

    if(0 <= lcm_ctrl->lcm_standby) {
        __gpio_set_value(lcm_ctrl->lcm_standby, val);
    }

EXIT:
#endif
    return rtn;
}

int32_t lcm_back_light_gpio_set(uint32_t lcm_id, uint32_t val)
{
    int32_t rtn = 0;
#if 0
    lcm_ctrl_t *lcm_ctrl = get_lcm_ctrl(lcm_id);

    if(0 > lcm_ctrl->lcm_back_light_gpio) {
        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find lcm node ~_~!\n");
        }

        lcm_ctrl->lcm_back_light_gpio = of_get_named_gpio(np, "lcm_back_light", 0);
        #if LCM_CTRL_DEBUG
        printk("%s:%d, lcm_back_light_gpio:%d\n",__FUNCTION__,__LINE__, lcm_ctrl->lcm_back_light_gpio);
        #endif

        if(0 <= lcm_ctrl->lcm_back_light_gpio) {
            if (0 != gpio_direction_output(lcm_ctrl->lcm_back_light_gpio, val)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set lcm_back_light gpio dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "get lcm_back_light gpio(%d) none ~_~!\n", lcm_ctrl->lcm_back_light_gpio);
        }
    }

    if(0 <= lcm_ctrl->lcm_back_light_gpio) {
        __gpio_set_value(lcm_ctrl->lcm_back_light_gpio, val);
    }

EXIT:
#endif
    return rtn;
}

int32_t lcm_back_light_pwm_set(uint32_t lcm_id, uint32_t val)
{
    int32_t rtn = 0;
#if 0
    lcm_ctrl_t *lcm_ctrl = get_lcm_ctrl(lcm_id);

    printk("%s:%d\n",__FUNCTION__,__LINE__);

    if (NULL == lcm_ctrl->lcm_back_light_pwm) {
        lcm_ctrl->lcm_back_light_pwm = pwm_request(1, "lcm_back_light");
    }

    if(IS_ERR(lcm_ctrl->lcm_back_light_pwm)) {
        rtn = FAIL;
        EXIT_IF_FAIL(rtn, NULL, "lcm_back_light pwm(%p) none ~_~!\n", lcm_ctrl->lcm_back_light_pwm);
    }

    rtn = pwm_config(lcm_ctrl->lcm_back_light_pwm, val, 10000);
    EXIT_IF_FAIL(rtn, NULL, "lcm_back_light_cfg ~_~!\n");

    if (0x00 != val) {
        //rtn = pwm_enable(lcm_ctrl->lcm_back_light_pwm);
        EXIT_IF_FAIL(rtn, NULL, "lcm_back_light_enable ~_~!\n");
    } else {
        //rtn = pwm_disable(lcm_ctrl->lcm_back_light_pwm);
        EXIT_IF_FAIL(rtn, NULL, "lcm_back_light_disable ~_~!\n");
    }

EXIT:
#endif

    return rtn;
}

int32_t lcm_power_pwm_set(uint32_t lcm_id, uint32_t val)
{
    int32_t rtn = 0;
#if 0
    lcm_ctrl_t *lcm_ctrl = get_lcm_ctrl(lcm_id);

    printk("%s:%d\n",__FUNCTION__,__LINE__);

    if (NULL == lcm_ctrl->lcm_power_pwm) {
        lcm_ctrl->lcm_power_pwm = pwm_request(1, "lcm_power_pwm");
    }

    if(IS_ERR(lcm_ctrl->lcm_power_pwm)) {
        rtn = FAIL;
        EXIT_IF_FAIL(rtn, NULL, "lcm_power_pwm(%p) none ~_~!\n", lcm_ctrl->lcm_power_pwm);
    }

    rtn = pwm_config(lcm_ctrl->lcm_power_pwm, val, 10000);
    EXIT_IF_FAIL(rtn, NULL, "lcm_power_cfg ~_~!\n");

    if (0x00 != val) {
        //rtn = pwm_enable(lcm_ctrl->lcm_power_pwm);
        EXIT_IF_FAIL(rtn, NULL, "lcm_power_enable ~_~!\n");
    } else {
        //rtn = pwm_disable(lcm_ctrl->lcm_power_pwm);
        EXIT_IF_FAIL(rtn, NULL, "lcm_power_disable ~_~!\n");
    }
/*
    pwm_get
    devm_pwm_get
    of_pwm_get
    devm_of_pwm_get

    struct pwm_device *pwm_get(struct device *dev, const char *con_id);
    struct pwm_device *devm_pwm_get(struct device *dev, const char *con_id);
    struct pwm_device *of_pwm_get(struct device_node *np, const char *con_id);
    struct pwm_device *devm_of_pwm_get(struct device *dev, struct device_node *np, const char *con_id);
    void pwm_put(struct pwm_device *pwm);
    void devm_pwm_put(struct device *dev, struct pwm_device *pwm);
*/



EXIT:
#endif
    return rtn;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

