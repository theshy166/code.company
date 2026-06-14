#ifndef _VOU_OSAL_
#define _VOU_OSAL_

#include <common.h>

#define REG_TOP_APB_REG_BASE                     0X18000000
#define REG_TOP_CLK_CFG_BASE                     0X28100200
#define REG_ANALOG_BASE                          0X28500000
#define REG_VIO_APB_REG_BASE                     0X34000000
#define REG_VOU_BASE                             0X34100000
#define REG_HDMI_BASE                            0X34200000
#define REG_VOU_QOS_BASE                         0X2C100B00
#define REG_DSI_BASE                             0x34300000
#define REG_DPHY_BASE                            0x34500000

#define ALIGN_TO(x, align) ( (((unsigned int)(x)) + (align) - 1) & (~((align) - 1)) )
#define ALIGN_DOWN_TO(x, align) ( ((unsigned int)(x)) & (~((align) - 1)) )

#ifndef ZERO_FILL_STRUCT
#define ZERO_FILL_STRUCT(v)                                    \
    do {                                                        \
        (void)memset(&(v), 0, sizeof(v));                         \
    } while (0)
#endif /* ZERO_FILL_STRUCT */

#ifndef MEMCPY_STRUCT
#define MEMCPY_STRUCT(to,from)                                    \
    do {                                                        \
        (void)memcpy(&(to), &(from), sizeof(from));                 \
    } while (0)
#endif /* MEMCPY_STRUCT */

#ifndef FREE_POINTER
#define FREE_POINTER(pointer)                                   \
    do {                                                        \
        if (pointer) {                                          \
            free(pointer);                                      \
            pointer = NULL;                                     \
        }                                                       \
    } while (0)
#endif /* FREE_POINTER */


#ifndef ZERO_FILL_POINTER
#define ZERO_FILL_POINTER(v)                                    \
    do {                                                        \
        (void)memset(v, 0, sizeof(*v));                         \
    } while (0)
#endif /* ZERO_FILL_POINTER */


#ifndef MALLOC_POINTER
#define MALLOC_POINTER(v)                                       \
    do {                                                        \
        v = (typeof(v))malloc(sizeof(*v));                      \
    } while (0)
#endif /* MALLOC_POINTER */

#define CLIP(_x, _min, _max) ((_x) > (_max) ? (_max) : ((_x) < (_min) ? (_min) : (_x)))

typedef struct {
	void   *topapb;
	void   *topclk;
	void   *analog;
	void   *base;
	void   *vioapb;
	void   *cvbs;
	void   *hdmi;
} vou_reg_base_t;

uint32_t topapb_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset);
void topapb_write_register_bit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value, uint32_t start, uint32_t bit);
uint32_t topclk_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset);
void topclk_write_register_bit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value, uint32_t start, uint32_t bit);
uint32_t analog_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset);
void analog_write_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value);
uint32_t vou_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset);
void vou_write_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value);
uint32_t cvbs_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset);
void cvbs_write_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value);
uint32_t vioapb_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset);
void vioapb_write_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value);
uint8_t hdmi_read_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset);
void hdmi_write_register_byte(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint8_t value);
void hdmi_write_register_clrbit(vou_reg_base_t *regbase,  uint32_t base_addr, uint32_t offset, uint32_t value);
void hdmi_write_register_byte_setbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint8_t value);
void hdmi_write_register_byte_clrbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint8_t value);
void analog_write_register_setbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value);
void analog_write_register_clrbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value);
void topclk_write_register(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value);
void vioapb_write_register_clrbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value);
void vioapb_write_register_setbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value);
void vioapb_write_register_bit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value, uint32_t start, uint32_t bit);
void topclk_write_register_setbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value);
void topclk_write_register_clrbit(vou_reg_base_t *regbase, uint32_t base_addr, uint32_t offset, uint32_t value);

uint32_t readl(uint32_t addr);
void writel(uint32_t value, uint32_t addr);
void writelc(uint32_t value, uint32_t addr);
void writels(uint32_t value, uint32_t addr);
void writelr(uint32_t value, uint32_t addr, uint32_t start, uint32_t bit);

#endif
