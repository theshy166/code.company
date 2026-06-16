#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "lvgl/lvgl.h"
#include "logo_img.h"

/*
 *
 * ./logo_test.elf -l logo_0.jpg
 * ./logo_test.elf -u 0 -p /data/res/logo/logo_test_1.jpg
 * ./logo_test.elf -r 0
*/
int main(int argc, char *argv[]) {
    int opt;
    const char *filename = NULL;
    int r_flag = 0;
    int u_flag = 0;
    int l_flag = 0;
    int p_flag = 0;
    int r_value = 0;
    int u_value = 0;
    char *l_value = NULL;
    char *p_value = NULL;

    // 解析命令行参数
    while ((opt = getopt(argc, argv, "r:u:l:p:")) != -1) {
        switch (opt) {
            case 'r':
                // 将 -r 后面的参数转换为整数
                r_flag = 1;
                r_value = atoi(optarg);
                break;
            case 'u':
                // 将 -u 后面的参数转换为整数
                u_flag = 1;
                u_value = atoi(optarg);
                break;
            case 'l':
                // 直接存储 -l 后面的字符串
                l_flag = 1;
                l_value = optarg;
                break;
            case 'p':
                // 直接存储 -p 后面的字符串
                p_flag = 1;
                p_value = optarg;
                break;
            default:
                // 处理未知参数
                fprintf(stderr, "Usage: %s -r <integer> -u <integer> -l <string> -p <string>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // 根据标志执行相应操作
    if (l_flag) {
        load_file(l_value);
    } else if (u_flag && p_flag) {
        xos_logo_update(u_value, p_value);
        printf("u_value: %d, p_value: %s\n", u_value, p_value);
    } else if (r_flag) {
        xos_logo_reset(r_value);
    } else {
        fprintf(stderr, "No valid operation specified\n");
        return 1;
    }

    return 0;
}