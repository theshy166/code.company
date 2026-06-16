#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
// #include "lvgl/lvgl.h"

#define DMA_ZONE "DMA"
#define NORMAL_ZONE "Normal"
#define BUDDYINFO_PATH "/proc/buddyinfo"
#define MAX_ORDER 10
#define PAGE_SIZE 4096

/**
 * cat /proc/buddyinfo
 * Node 0, zone   Normal     51     38     28      3      1      1      1      3      0      1      0
 * 数据解释：
 * Order 0 4KB    Free 51
 * Order 1 8KB    Free 38
 * Order 2 16KB   Free 28
 * Order 3 32KB   Free 3
 * Order 4 64KB   Free 1
 * Order 5 128KB  Free 1
 * Order 6 256KB  Free 1
 * Order 7 512KB  Free 3
 * Order 8 1024KB Free 0
 * Order 9 2048KB Free 1
 * Order 10 4096KB Free 0
*/
// 查找指定 zone 和 order 对应的可用内存块数量
static int __query_buddyinfo(const char *zone, int order) {
    if (order < 0 || order > MAX_ORDER) {
        fprintf(stderr, "Order should be between 0 and %d.\n", MAX_ORDER);
        return -1;
    }

    FILE *fp = fopen(BUDDYINFO_PATH, "r");
    if (fp == NULL) {
        perror("Failed to open /proc/buddyinfo");
        return -1;
    }

    char line[256] = {0};
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, zone) != NULL) {
            char *token;
            int field_index = 0;

            // 找到 zone 后，跳过 zone 本身
            token = strstr(line, zone);
            token = strtok(token + strlen(zone), " \t\n");

            while (token != NULL) {
                if (field_index == order) {
                    int result = atoi(token);
                    fclose(fp);
                    return result;
                }
                token = strtok(NULL, " \t\n");
                field_index++;
            }
        }
    }

    fclose(fp);
    fprintf(stderr, "Zone %s not found or insufficient data.\n", zone);
    return -1;
}

bool query_memfree(unsigned int size)
{
    double result = log2(size / PAGE_SIZE);
    int order = (int)ceil(result);
    return __query_buddyinfo(NORMAL_ZONE, order) > 0;
}