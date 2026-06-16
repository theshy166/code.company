
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ctype.h>

// 定义一个变量来存储 lcm 参数的值

int32_t qua_lcm_parse_lcm_args(void) {
    static int32_t lcm = -1;

    const char *bootargs = saved_command_line;
    const char *lcm_pos = NULL;

    if (bootargs == NULL) {
        printk("%s Error: bootargs is NULL\n", __func__);
        return -1;
    }

    lcm_pos = strstr(bootargs, "lcm=");
    if (lcm_pos != NULL) {
        // 跳过 "lcm="
        lcm_pos += 4;

        // 检查是否为数字字符, 且后面没有其他字符或空格
        if (isdigit(*lcm_pos) && (lcm_pos[1] == '\0' || lcm_pos[1] == ' ')) {
            lcm = *lcm_pos - '0';
        }
    }

    printk("%s %s Parsed lcm parameter value: %d\n", __func__, lcm_pos, lcm);
    return lcm;
}
