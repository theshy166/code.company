#if defined(__KERNEL__)
#include <linux/of.h>
#include <linux/of_gpio.h>
#endif

#include "pin_lcd_v01_drv.h"
#include "data_type.h"
#include "mol_lcm_serial_bus.h"
#include "mol_lcm_spi_param.h"
#include "os.h"

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
#define GPIO_SPI_FUN_DEBUG 0
#define GPIO_SPI_DEBUG 0

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/
typedef enum {
    GPIO_OUTPUT = 0,
    GPIO_INPUT = 1,
    GPIO_DIR_MAX,
}gpio_dir_e;

typedef struct {
    uint32_t pin_base_addr;
    int32_t mode;

    int32_t baudRate;

    int32_t cs;
    int32_t scl;
    int32_t sda;

    int32_t cs_idle;
    int32_t scl_idle;
    int32_t sda_idle;

    uint32_t rd8_start_dummy_cycle;
    uint32_t rd_start_dummy_cycle;
    uint32_t rd_end_dummy_cycle;


    int32_t delay;
}gpio_spi_t;

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
static gpio_spi_t s_gpio_spi = {
    .cs = -1,
    .scl = -1,
    .sda = -1,

    .cs_idle = 1,
    .scl_idle = -1,
    .sda_idle = -1,

    .delay = 1,
};

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/
static gpio_spi_t *_get_gpio_spi_context(void)
{
    return &s_gpio_spi;
}

static int32_t _gpio_spi_identify(void)
{
    int32_t rtn = SUCCESS;

#if defined(__KERNEL__)
    struct device_node *np = NULL;
    int32_t gpio_valid = -1;

    #if GPIO_SPI_FUN_DEBUG
    printk("%s:%d-----------start--------------\n",__FUNCTION__,__LINE__);
    #endif

    np = of_find_node_by_name(NULL, "lcm");
    if (!np) {
        rtn = FAIL;
        EXIT_IF_FAIL(rtn, NULL, "find gpio spi node ~_~!\n");
    }

    gpio_valid = of_get_named_gpio(np, "gpio_spi_cs", 0);
    if(0 > gpio_valid) {
        rtn = FAIL;
        EXIT_IF_FAIL(rtn, NULL, "gpio spi cs invalid ~_~!\n");
    }
    #if GPIO_SPI_DEBUG
    printk("gpio_cs :%d \n",gpio_valid);
    #endif

    gpio_valid = of_get_named_gpio(np, "gpio_spi_scl", 0);
    if(0 > gpio_valid) {
        rtn = FAIL;
        EXIT_IF_FAIL(rtn, NULL, "gpio spi scl invalid ~_~!\n");
    }
    #if GPIO_SPI_DEBUG
    printk("gpio_scl :%d \n",gpio_valid);
    #endif

    gpio_valid = of_get_named_gpio(np, "gpio_spi_sda", 0);
    if(0 > gpio_valid) {
        rtn = FAIL;
        EXIT_IF_FAIL(rtn, NULL, "gpio spi sda invalid ~_~!\n");
    }
    #if GPIO_SPI_DEBUG
    printk("gpio_sda :%d \n",gpio_valid);
    #endif

EXIT:

    #if GPIO_SPI_FUN_DEBUG
    printk("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif
#endif
    return rtn;
}

static int32_t _gpio_spi_init_fail_proc(void)
{
    int32_t rtn = SUCCESS;
    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    spi_ctrl->cs = -1;
    spi_ctrl->scl = -1;
    spi_ctrl->sda = -1;

    spi_ctrl->scl_idle = -1;
    spi_ctrl->sda_idle = -1;

    spi_ctrl->delay = 0;

    return rtn;
}

static int32_t _gpio_spi_bund_rate_cfg(    int32_t baudRate)
{
    int32_t rtn = SUCCESS;
    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    #if GPIO_SPI_FUN_DEBUG
    printk("%s:%d-----------start--------------\n",__FUNCTION__,__LINE__);
    #endif

    spi_ctrl->delay = 140000 / baudRate;

    if (0 >= spi_ctrl->delay) {
        spi_ctrl->delay = 0x01;
    }

    #if GPIO_SPI_FUN_DEBUG
    printk("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

static int32_t _gpio_spi_mode_cfg(    int32_t mode)
{
    int32_t rtn = SUCCESS;
    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    switch (mode) {
        case SPI_MODE0:
        case SPI_MODE1:
            spi_ctrl->scl_idle = 0;
        break;

        case SPI_MODE2:
        case SPI_MODE3:
            spi_ctrl->scl_idle = 1;
        break;

        default :
            spi_ctrl->scl_idle = 0;
        break;
    }

    spi_ctrl->sda_idle = 1;

    return rtn;
}

static int32_t _gpio_spi_param_cfg(lcm_spi_init_in_t *inParam)
{
    int32_t rtn = SUCCESS;
    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();
    serial_bus_info *busInf = inParam->info;

    #if GPIO_SPI_FUN_DEBUG
    printk("%s:%d-----------start--------------\n",__FUNCTION__,__LINE__);
    #endif

    spi_ctrl->pin_base_addr = inParam->pin_base_addr;
    spi_ctrl->baudRate = busInf->baudRate;
    spi_ctrl->mode = busInf->timing.spi.mode;

    spi_ctrl->rd8_start_dummy_cycle = busInf->timing.spi.rd8_start_dummy_cycle;
    spi_ctrl->rd_start_dummy_cycle = busInf->timing.spi.rd_start_dummy_cycle;
    spi_ctrl->rd_end_dummy_cycle = busInf->timing.spi.rd_end_dummy_cycle;

    rtn = _gpio_spi_bund_rate_cfg(spi_ctrl->baudRate);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_bund_rate_cfg ~_~!\n");

    rtn = _gpio_spi_mode_cfg(spi_ctrl->mode);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_mode_cfg ~_~!\n");


    #if GPIO_SPI_DEBUG
    printk("spi_ctrl->pin_base_addr        :0x%08x\n",spi_ctrl->pin_base_addr);
    printk("spi_ctrl->mode                 :%d\n",spi_ctrl->mode);
    printk("spi_ctrl->baudRate             :%d\n",spi_ctrl->baudRate);
    printk("spi_ctrl->cs                   :%d\n",spi_ctrl->cs);
    printk("spi_ctrl->scl                  :%d\n",spi_ctrl->scl);
    printk("spi_ctrl->sda                  :%d\n",spi_ctrl->sda);
    printk("spi_ctrl->cs_idle              :%d\n",spi_ctrl->cs_idle);
    printk("spi_ctrl->scl_idle             :%d\n",spi_ctrl->scl_idle);
    printk("spi_ctrl->sda_idle             :%d\n",spi_ctrl->sda_idle);
    printk("spi_ctrl->rd8_start_dummy_cycle:%d\n",spi_ctrl->rd8_start_dummy_cycle);
    printk("spi_ctrl->rd_start_dummy_cycle :%d\n",spi_ctrl->rd_start_dummy_cycle);
    printk("spi_ctrl->rd_end_dummy_cycle   :%d\n",spi_ctrl->rd_end_dummy_cycle);
    printk("spi_ctrl->delay                :%d\n",spi_ctrl->delay);
    #endif


EXIT:

    #if GPIO_SPI_FUN_DEBUG
    printk("%s:%d-----------end--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}


static int32_t _gpio_spi_cs_init(lcm_spi_init_in_t *inParam)
{
    int32_t rtn = SUCCESS;
#if defined(__KERNEL__)
    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    if(0 > spi_ctrl->cs) {
        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find gpio spi cs node ~_~!\n");
        }

        spi_ctrl->cs = of_get_named_gpio(np, "gpio_spi_cs", 0);
        if(0 <= spi_ctrl->cs) {
            rtn = lcm_gpio_pin_cfg(inParam->pin_base_addr, spi_ctrl->cs);
            if (0 != gpio_direction_output(spi_ctrl->cs, spi_ctrl->cs_idle)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set gpio cs dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "gpio spi cs invalid (%d) ~_~!\n", spi_ctrl->cs);
        }
    }

EXIT:
#endif
    return rtn;
}

static int32_t _gpio_spi_scl_init(lcm_spi_init_in_t *inParam)
{
    int32_t rtn = SUCCESS;
#if defined(__KERNEL__)
    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    if(0 > spi_ctrl->scl) {
        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find gpio spi scl node ~_~!\n");
        }

        spi_ctrl->scl = of_get_named_gpio(np, "gpio_spi_scl", 0);
        if(0 <= spi_ctrl->scl) {
            rtn = lcm_gpio_pin_cfg(inParam->pin_base_addr, spi_ctrl->scl);
            if (0 != gpio_direction_output(spi_ctrl->scl, spi_ctrl->scl_idle)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set gpio scl dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "gpio spi scl invalid (%d) ~_~!\n", spi_ctrl->scl);
        }

    }

EXIT:
#endif
    return rtn;
}

static int32_t _gpio_spi_sda_init(lcm_spi_init_in_t *inParam)
{
    int32_t rtn = SUCCESS;
#if defined(__KERNEL__)

    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    if(0 > spi_ctrl->sda) {
        struct device_node *np = NULL;

        np = of_find_node_by_name(NULL, "lcm");
        if (!np) {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "find gpio spi sda node ~_~!\n");
        }

        spi_ctrl->sda = of_get_named_gpio(np, "gpio_spi_sda", 0);
        if(0 <= spi_ctrl->sda) {
            rtn = lcm_gpio_pin_cfg(inParam->pin_base_addr, spi_ctrl->sda);
            if (0 != gpio_direction_output(spi_ctrl->sda, spi_ctrl->sda_idle)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set gpio sda dir ~_~!\n");
            }
        } else {
            rtn = FAIL;
            EXIT_IF_FAIL(rtn, NULL, "gpio spi sda invalid (%d) ~_~!\n", spi_ctrl->sda);
        }

    }

EXIT:
#endif

    return rtn;
}


static int32_t _gpio_spi_sda_dir_set(int32_t dir)
{
    int32_t rtn = SUCCESS;
#if defined(__KERNEL__)

    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    if(0 <= spi_ctrl->sda) {
        if (0 == dir) {
            if (0 != gpio_direction_output(spi_ctrl->sda, spi_ctrl->sda_idle)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set gpio sda dir ~_~!\n");
            }
        } else if (1 == dir) {
            if (0 != gpio_direction_input(spi_ctrl->sda)) {
                rtn = FAIL;
                EXIT_IF_FAIL(rtn, NULL, "set gpio sda dir ~_~!\n");
            }
        }
    }

EXIT:
#endif
    return rtn;
}


static int32_t _gpio_spi_cs_set(int32_t val)
{
    int32_t rtn = SUCCESS;
#if defined(__KERNEL__)

    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    if(0 <= spi_ctrl->cs) {
        __gpio_set_value(spi_ctrl->cs, val);
    } else {
        rtn = FAIL;
        EXIT_IF_FAIL(rtn, NULL, "gpio spi cs invalid (%d) ~_~!\n", spi_ctrl->cs);
    }

EXIT:
#endif
    return rtn;
}

static int32_t _gpio_spi_scl_set(int32_t val)
{
    int32_t rtn = SUCCESS;
#if defined(__KERNEL__)
    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    if(0 <= spi_ctrl->scl) {
        __gpio_set_value(spi_ctrl->scl, val);
    } else {
        rtn = FAIL;
        EXIT_IF_FAIL(rtn, NULL, "gpio spi scl invalid (%d) ~_~!\n", spi_ctrl->scl);
    }

EXIT:
#endif
    return rtn;
}

static int32_t _gpio_spi_sda_set(int32_t val)
{
    int32_t rtn = SUCCESS;
#if defined(__KERNEL__)
    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    if(0 <= spi_ctrl->sda) {
        __gpio_set_value(spi_ctrl->sda, val);
    } else {
        rtn = FAIL;
        EXIT_IF_FAIL(rtn, NULL, "gpio spi sda invalid (%d) ~_~!\n", spi_ctrl->sda);
    }

EXIT:
#endif
    return rtn;
}

static int32_t _gpio_spi_sda_get(void)
{
    int32_t val = SUCCESS;
#if defined(__KERNEL__)
    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    if(0 <= spi_ctrl->sda) {
        val = __gpio_get_value(spi_ctrl->sda);
    } else {
        EXIT_IF_FAIL(FAIL, NULL, "gpio spi sda invalid (%d) ~_~!\n", spi_ctrl->sda);
    }

EXIT:
#endif
    return val;
}

int32_t gpio_spi_init(lcm_spi_init_in_t *inParam)
{
    int32_t rtn = SUCCESS;

    #if GPIO_SPI_FUN_DEBUG
    printk("%s:%d-----------start--------------\n",__FUNCTION__,__LINE__);
    #endif

    rtn = _gpio_spi_identify();
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_identify ~_~!\n");

    rtn = _gpio_spi_cs_init(inParam);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_cs_init ~_~!\n");

    rtn = _gpio_spi_scl_init(inParam);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_scl_init ~_~!\n");

    rtn = _gpio_spi_sda_init(inParam);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_sda_init ~_~!\n");

    rtn = _gpio_spi_param_cfg(inParam);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_sda_init ~_~!\n");

EXIT:

    if (SUCCESS != rtn) {
        _gpio_spi_init_fail_proc();
    }

    #if GPIO_SPI_FUN_DEBUG
    printk("%s:%d-----------_end_--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

int32_t gpio_spi_deinit(lcm_spi_deinit_in_t *inParam)
{
    int32_t rtn = SUCCESS;

    #if GPIO_SPI_FUN_DEBUG
    printk("%s:%d-----------start--------------\n",__FUNCTION__,__LINE__);
    #endif

    rtn = _gpio_spi_init_fail_proc();
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_init_fail_proc ~_~!\n");

EXIT:

    #if GPIO_SPI_FUN_DEBUG
    printk("%s:%d-----------_end_--------------\n",__FUNCTION__,__LINE__);
    #endif

    return rtn;
}

static int32_t _gpio_spi_write(int32_t dc, int32_t data, int32_t cs_end, int32_t *end_scl)
{
    int32_t rtn = SUCCESS;
    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    int32_t scl = spi_ctrl->scl_idle;
    int32_t sda = 0x00;
    int32_t val = 0x00;
    int32_t i = 0;

    val = ((dc & 0x01) << 0x08) | (data & 0xff);

    _gpio_spi_sda_set(spi_ctrl->sda_idle);
    _gpio_spi_scl_set(spi_ctrl->scl_idle);
    _gpio_spi_cs_set((~spi_ctrl->cs_idle) & 0x01);

    for (i = 0x08; i >= 0x00; i--) {

        if ((0 == spi_ctrl->mode)
            || (2 == spi_ctrl->mode)) {
            sda = (val >> i) & 0x01;
            _gpio_spi_sda_set(sda);
        }
        delayUs(spi_ctrl->delay);

        scl = (~scl) & 0x01;
        _gpio_spi_scl_set(scl);

        if ((1 == spi_ctrl->mode)
            || (3 == spi_ctrl->mode)) {
            sda = (val >> i) & 0x01;
            _gpio_spi_sda_set(sda);
        }

        delayUs(spi_ctrl->delay);
        scl = (~scl) & 0x01;
        _gpio_spi_scl_set(scl);

    }

    *end_scl = scl;

    delayUs(spi_ctrl->delay);
    if (1 == cs_end) {
        _gpio_spi_sda_set(spi_ctrl->sda_idle);
        _gpio_spi_scl_set(spi_ctrl->scl_idle);
        _gpio_spi_cs_set(spi_ctrl->cs_idle);
    }

//EXIT:

    return rtn;
}

static int32_t _gpio_spi_read(uint8_t *data, uint32_t num, int32_t start_scl)
{
    int32_t rtn = SUCCESS;
    gpio_spi_t *spi_ctrl = _get_gpio_spi_context();

    int32_t scl = start_scl;
    uint32_t sda = 0x00;
    uint32_t readBits = num * 8;
    int32_t i = 0;

    rtn = _gpio_spi_sda_dir_set(GPIO_INPUT);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_sda_dir_set ~_~!\n");

    scl = (~scl) & 0x01;
    if (((1 == num) && (1 == spi_ctrl->rd8_start_dummy_cycle))
        || ((1 < num) && (1 == spi_ctrl->rd_start_dummy_cycle))) {
        _gpio_spi_scl_set(scl);
        delayUs(spi_ctrl->delay);
        scl = (~scl) & 0x01;
        _gpio_spi_scl_set(scl);
    }

    for (i = 0; i < readBits; i++) {

        sda <<= 1;
        _gpio_spi_scl_set(scl);
        sda |= _gpio_spi_sda_get();
        delayUs(spi_ctrl->delay);
        scl = (~scl) & 0x01;

        _gpio_spi_scl_set(scl);
        delayUs(spi_ctrl->delay);
        scl = (~scl) & 0x01;
    }

    if (1 == spi_ctrl->rd_end_dummy_cycle) {
        _gpio_spi_scl_set(scl);
        delayUs(spi_ctrl->delay);

        scl = (~scl) & 0x01;
        _gpio_spi_scl_set(scl);
        delayUs(spi_ctrl->delay);
    }

    _gpio_spi_scl_set(spi_ctrl->scl_idle);
    _gpio_spi_cs_set(spi_ctrl->cs_idle);

    rtn = _gpio_spi_sda_dir_set(GPIO_OUTPUT);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_sda_dir_set ~_~!\n");
    _gpio_spi_sda_set(spi_ctrl->sda_idle);

    for(i = 0; i < num; i++) {
        data[i] = (sda >> (((num -1)- i) * 8)) & 0xff;
    }

EXIT:

    return rtn;
}

int32_t gpio_spi_write_cmd(serial_bus_info *info, int32_t cmd)
{
    int32_t rtn = SUCCESS;
    int32_t scl = 0x00;

    rtn = _gpio_spi_write(0, cmd, 1, &scl);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_write ~_~!\n");
    delayUs(info->timing.spi.delay);

EXIT:

    return rtn;
}

int32_t gpio_spi_write_data(serial_bus_info *info, int32_t data)
{
    int32_t rtn = SUCCESS;
    int32_t scl = 0x00;

    rtn = _gpio_spi_write(1, data, 1, &scl);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_write ~_~!\n");
    delayUs(info->timing.spi.delay);


EXIT:

    return rtn;
}

int32_t gpio_spi_read(serial_bus_info *info, int32_t cmd, uint8_t *data, int32_t read_len)
{
    int32_t rtn = SUCCESS;
    int32_t scl = 0x00;

    rtn = _gpio_spi_write(0, cmd, 0, &scl);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_write ~_~!\n");

    rtn = _gpio_spi_read(data, read_len, scl);
    EXIT_IF_FAIL(rtn, NULL, "_gpio_spi_read ~_~!\n");
    delayUs(info->timing.spi.delay);

EXIT:

    return rtn;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
