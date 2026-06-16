#ifndef __FY_OSAL_INIT__
#define __FY_OSAL_INIT__

#define fhosal_platform_get_module_param(pdev, name, type, value) \
	fhosal_platform_get_modparam_##type(pdev, name, value)

int fhosal_platform_get_modparam_uint(void *pdev, const char *name, unsigned int *value);
int fhosal_platform_get_modparam_int(void *pdev, const char *name, int *value);
int fhosal_platform_get_modparam_charp(void *pdev, const char *name, char *value);
int fhosal_platform_get_modparam_string(void *pdev, const char *name, unsigned int size, char *value);


/*pinctrl*/
void* fhosal_devm_pinctrl_get(void *dev);
void fhosal_devm_pinctrl_put(void *p);
void* fhosal_pinctrl_lookup_state(void *p, const char *name);
void fhosal_pinctrl_get_select(void *dev, const char *name);
int fhosal_pinctrl_pm_select_default_state(void *dev);
int fhosal_pinctrl_select_state(void *p, void *state);

/*clock function*/
int fhosal_clk_prepare_enable(void *clk);
void fhosal_clk_disable_unprepare(void *clk);
unsigned int fhosal_clk_get_rate(void *clk);
int fhosal_clk_set_rate(void *clk, unsigned int clk_freq);


int fhosal_platform_driver_register(void *drv);
void fhosal_platform_driver_unregister(void *drv);
void* fhosal_platform_get_resource_byname(void *dev, unsigned int type,
    const char *name);
void* fhosal_platform_get_resource(void *dev, unsigned int type,
    unsigned int num);
int fhosal_platform_get_irq(void *dev, unsigned int num);
int fhosal_platform_get_irq_byname(void *dev, const char *name);

#define fhosal_module_driver(fhosal_driver, fhosal_register, fhosal_unregister, ...) \
static int __init fhosal_driver##_init(void) \
{ \
        return fhosal_register(&(fhosal_driver)); \
} \
module_init(fhosal_driver##_init); \
static void __exit fhosal_driver##_exit(void) \
{ \
        fhosal_unregister(&(fhosal_driver)); \
} \
module_exit(fhosal_driver##_exit);

#define fhosal_module_platform_driver(platform_driver) \
        fhosal_module_driver(platform_driver, fhosal_platform_driver_register, \
                        fhosal_platform_driver_unregister)

#endif
