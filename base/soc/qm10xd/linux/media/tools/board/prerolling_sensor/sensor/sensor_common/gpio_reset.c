#ifdef OS_LINUX
#include <sys/mman.h>
#endif
#include <fcntl.h>
#include <unistd.h>
#include "sensor_ops.h"

#define mm_dev "/dev/mem"
int gpio_mm_fd = -1;
#ifdef OS_LINUX
static int gpio_mm_init(void)
{
	if (gpio_mm_fd < 0)
	{
		gpio_mm_fd = open(mm_dev, O_RDWR | O_SYNC);
        if(gpio_mm_fd < 0)
        {
			perror("gpio mem open error");
			return -1;
        }

	}
	return 0;
}
static int gpio_mm_close(void)
{
	if (gpio_mm_fd >= 0)
	{
		close(gpio_mm_fd);
		gpio_mm_fd = -1;
	}
	return 0;
}
#endif

static int gpio_mmap(unsigned int FH_UINT32phyAddr,  void** mmapAddr, unsigned int size)
{
	int err = 0;
//|MAP_ANONYMOUS|MAP_LOCKED
#ifdef OS_LINUX
	gpio_mm_init();
	if(*mmapAddr != NULL)
	{
		if (munmap(*mmapAddr, size) == -1)
		{
			perror("gpio mem unmap error\n");
		}
	}
	*mmapAddr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, gpio_mm_fd, FH_UINT32phyAddr);
	if (*mmapAddr == MAP_FAILED)
	{
		err  = -1;//ERROR_CORE_MMAP_FILED;
		perror("mem mmap error\n");
	}
	gpio_mm_close();
#else
	*mmapAddr =(void*)FH_UINT32phyAddr;
#endif
	return err;

}

#define GET_REG_V(addr)                (*((volatile unsigned int *)(addr)))
#define SET_REG_V(addr, data)          (*((volatile unsigned int *)(addr))) = (unsigned int)(data)

#define SET_REG_V_M(addr, data, mask) \
SET_REG_V((addr),((GET_REG_V((addr))&(~(mask))) | ((data)&(mask))))

FH_UINT32 gpio_addr_offset[11][8] = {
    {0x4,  0x8,  0xc,  0x10, 0x14, 0x18, 0x1c, 0x20},  // gpio0
    {0x24, 0x28, 0x2c, 0x30, 0x34, 0x38, 0x3c, 0x48},  // gpio1
    {0x54, 0x58, 0x0,  0x70, 0x78, 0x7c, 0x80, 0x68},  // gpio2
    {0x88, 0x8c, 0x90, 0x94, 0x0,  0xc4, 0xc8, 0xcc},  // gpio3
    {0xd0, 0xd4, 0xd8, 0xdc, 0xe0, 0xe4, 0xe8, 0xec},  // gpio4
    {0xf0, 0xf4, 0xf8, 0xfc, 0x100,0x104,0x108,0x9c},  // gpio5
    {0xa0, 0xa4, 0xa8, 0xac, 0xb0, 0xb4, 0xb8, 0xbc},  // gpio6
    {0xc0, 0x130,0x134,0x10c,0x110,0x114,0x118,0x124},  // gpio7
    {0x128,0x12c,0x138,0x13c,0x140,0x144,0x148,0x14c},  // gpio8
    {0x150,0x154,0x158,0x15c,0x160,0x164,0x168,0x16c},  // gpio9
    {0x170,0x174,0x178,0x17c,0x180,0x184,0x11c,0x0 },  // gpio10
};

#define GPIO_BASE_ADDR    0x1b000000
#define PINMUX_BASE_ADDR  0x28200000
void *gpio_virt_addr = NULL;
void *pinmux_virt_addr = NULL;
int Sensor_Gpio_Reset(unsigned int io_group, unsigned int io_num, unsigned int reset_time_us)
{
    int err;
    //map gpio addr
    err = gpio_mmap(GPIO_BASE_ADDR + (io_group<<16), &gpio_virt_addr, 0x100);
    //map pinmux addr
    err |= gpio_mmap(PINMUX_BASE_ADDR, &pinmux_virt_addr, 0x200);
    // // set io as gpio
    SET_REG_V((FH_UINT32)pinmux_virt_addr+gpio_addr_offset[io_group][io_num], 1);
    // set reset gpio output
    SET_REG_V_M((FH_UINT32)gpio_virt_addr+0x4, 1<<io_num, 1<<io_num);
    SET_REG_V_M((FH_UINT32)gpio_virt_addr+0x28, 0, 1<<io_num);
    SET_REG_V_M((FH_UINT32)gpio_virt_addr+0x8, 1<<io_num, 1<<io_num);
    usleep(10);
    // set reset low
    SET_REG_V_M(gpio_virt_addr, 0, 1<<io_num);
    usleep(reset_time_us);
    // set reset high
    SET_REG_V_M(gpio_virt_addr, 1<<io_num, 1<<io_num);

    return err;
}

void *gpio_pd_virt_addr = NULL;
FH_SINT32 Sensor_Gpio_PowerDown(FH_UINT32 io_group, FH_UINT32 io_num, FH_BOOL level)
{
    int err;
    //map gpio addr
    err = gpio_mmap(GPIO_BASE_ADDR + (io_group<<16), &gpio_pd_virt_addr, 0x100);
    //map pinmux addr
    err |= gpio_mmap(PINMUX_BASE_ADDR, &pinmux_virt_addr, 0x200);
    // // set io as gpio
    SET_REG_V((FH_UINT32)pinmux_virt_addr+gpio_addr_offset[io_group][io_num], 1);
    // set SENSOR POWER DOWN
    SET_REG_V_M((FH_UINT32)gpio_pd_virt_addr+0x4, 1<<io_num, 1<<io_num);
    SET_REG_V_M((FH_UINT32)gpio_pd_virt_addr+0x28, 0, 1<<io_num);
    SET_REG_V_M((FH_UINT32)gpio_pd_virt_addr+0x8, 1<<io_num, 1<<io_num);
    SET_REG_V_M(gpio_pd_virt_addr, level<<io_num, 1<<io_num);

    return err;
}