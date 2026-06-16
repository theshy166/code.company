#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/vfs.h>
#include "../conf/conf.h"

int create_directory(const char *path){
    char dir_path[256] = {0};
    char *p = NULL;
    int len = strlen(path);
    
    strncpy(dir_path, path, sizeof(dir_path)-1);
    
    // 去掉末尾的斜杠
    if (dir_path[len-1] == '/' || dir_path[len-1] == '\\') {
        dir_path[len-1] = '\0';
    }
    
    // 逐级创建目录
    for (p = dir_path + 1; *p; p++) {
        if (*p == '/' || *p == '\\') {
            *p = '\0';  // 临时截断
            
            #ifdef _WIN32
            if (mkdir(dir_path) != 0) {
            #else
            if (mkdir(dir_path, 0755) != 0) {
            #endif
                if (errno != EEXIST) { // 忽略目录已存在的错误
                    return -1;
                }
            }
            
            *p = '/';  // 恢复斜杠
        }
    }
    
    // 创建最后一级目录
    #ifdef _WIN32
    if (mkdir(dir_path) != 0) {
    #else
    if (mkdir(dir_path, 0755) != 0) {
    #endif
        if (errno != EEXIST) {
            return -1;
        }
    }
    
    return 0;
}


char* transform_path(const char* input_path) {
    // 计算处理后的路径长度，去除 "H:." 后的长度
    size_t new_length = strlen(input_path);
    if (strncmp(input_path, "H:", 2) == 0) {
        new_length -= 2;
    }

    // 分配足够的内存来存储处理后的路径
    char* output_path = (char*)malloc(new_length + 1);
    if (output_path == NULL) {
        // 内存分配失败，返回 NULL
        return NULL;
    }

    // 复制处理后的路径内容
    if (strncmp(input_path, "H:", 2) == 0) {
        strcpy(output_path, input_path + 2);
    } else {
        strcpy(output_path, input_path);
    }

    return output_path;
}

int is_string_empty(const char *str) {
    return str == NULL || str[0] == '\0' || strcmp(str,"") == 0;
}

int ends_with(const char *str, const char *suffix) {
    int strLen = strlen(str);
    int suffixLen = strlen(suffix);
    if (suffixLen > strLen) {
        return 0;
    }
    return strcmp(str + strLen - suffixLen, suffix) == 0;
}

int is_string_ends_with(const char **arr, const char *target, int size){
    if(size < 0){
        return 0;
    }
    for (int i = 0; i < size; i++) {
        if (ends_with(arr[i], target)) {
            return 1;  // 找到了，返回1
        }
    }
    return 0;  // 遍历完没找到，返回0
}

int starts_with(const char *str, const char *prefix) {
    size_t str_len = strlen(str);
    size_t prefix_len = strlen(prefix);
    // 如果前缀的长度大于要检查的字符串长度，直接返回 0
    if (prefix_len > str_len) {
        return 0;
    }
    // 使用 strncmp 函数比较 str 的前 prefix_len 个字符和 prefix
    // strncmp 函数返回 0 表示相等
    if (strncmp(str, prefix, prefix_len) == 0) {
        return 1;
    }
    return 0;
}

char** convert_string_to_char_array(const char *str, int *count) {
    if (str == NULL || count == NULL) {
        return NULL;
    }

    // 计算逗号的数量以确定数组的大小
    int num_commas = 0;
    for (const char *p = str; *p != '\0'; p++) {
        if (*p == ',') {
            num_commas++;
        }
    }

    // 数组的大小是逗号数量加1
    *count = num_commas + 1;

    // 分配数组
    char **array = (char **)malloc(*count * sizeof(char *));
    if (array == NULL) {
        return NULL;
    }

    // 复制字符串以便使用strtok
    char *str_copy = strdup(str);
    if (str_copy == NULL) {
        free(array);
        return NULL;
    }

    // 使用strtok分割字符串并存储在数组中
    char *token = strtok(str_copy, ",");
    int index = 0;
    while (token != NULL) {
        array[index++] = strdup(token);
        token = strtok(NULL, ",");
    }

    free(str_copy);
    return array;
}


struct android_bootloader_message {
    char command[32];
    char status[32];
    /* Number of times left attempting to boot this slot. */
    char tries_counter[2];
    /* 1 if this slot has booted successfully, 0 otherwise. */
    char successful_boot[2];
    char recovery[764];//reserved

    /* The 'recovery' field used to be 1024 bytes.  It has only ever
     * been used to store the recovery command line, so 768 bytes
     * should be plenty.  We carve off the last 256 bytes to store the
     * stage string (for multistage packages) and possible future
     * expansion. */
    char stage[32];//reserved

    /* The 'reserved' field used to be 224 bytes when it was initially
     * carved off from the 1024-byte recovery field. Bump it up to
     * 1184-byte so that the entire bootloader_message struct rounds up
     * to 2048-byte. */
    char reserved[1184];
};

#define MISC_DEVICE "/dev/mtd5"
#define BCB_SIZE    sizeof(struct android_bootloader_message)

int readmisc(struct android_bootloader_message *bcb) {
    char cmd[128];
    FILE *fp;

    // 使用nanddump读取misc分区
    snprintf(cmd, sizeof(cmd), "nanddump --bb=skipbad -l %zu -s 0 %s | head -c %zu > /tmp/misc_bcb",
		BCB_SIZE, MISC_DEVICE, BCB_SIZE);
    printf("cmd:%s\n",cmd);
    if(system(cmd) != 0) {
	    printf("readmisc %s error\n",cmd);
	    return -1;
    }

    // 读取临时文件
    if((fp = fopen("/tmp/misc_bcb", "rb")) == NULL) {
	    printf("readmisc fopen error\n");
	    return -2;
    }
    if(fread(bcb, BCB_SIZE, 1, fp) != 1) {
	printf("readmisc fread error\n");
        fclose(fp);
        return -3;
    }
    fclose(fp);
    remove("/tmp/misc_bcb");
    return 0;
}

int writemisc(struct android_bootloader_message *bcb) {
    char cmd[256];
    FILE *fp;

    // 写入临时文件
    if((fp = fopen("/tmp/misc_bcb", "wb")) == NULL) return -1;
    if(fwrite(bcb, BCB_SIZE, 1, fp) != 1) {
        fclose(fp);
        return -2;
    }
    fclose(fp);

    // 使用flash_erase nandwrite直接写入块设备
    snprintf(cmd, sizeof(cmd), "flash_erase %s 0 0",MISC_DEVICE);
    int ret = system(cmd);
    printf("cmd:%s\n",cmd);
    snprintf(cmd, sizeof(cmd), "nandwrite -p %s /tmp/misc_bcb",
		MISC_DEVICE);
    ret = system(cmd);
    printf("cmd:%s\n",cmd);

    remove("/tmp/misc_bcb");
    return (ret == 0) ? 0 : -3;
}

int boot2linux() {
    struct android_bootloader_message bcb;
    if(readmisc(&bcb) != 0) {
        printf("Failed to read misc partition\n");
        return -1;
    }
    printf("boot2linux bcb.command:%s\n",bcb.command);
    printf("boot2linux bcb.tries_counter:%d\n",bcb.tries_counter[0]);
    printf("boot2linux bcb.successful_boot:%d\n",bcb.successful_boot[0]);
    if(strlen(bcb.command) == 0 && bcb.tries_counter[0] == 0 && bcb.successful_boot[0] == 1) {
        return 0;
    }
    memset(bcb.command, 0, sizeof(bcb.command));
    memset(bcb.tries_counter, 0, sizeof(bcb.tries_counter));
    bcb.successful_boot[0] = 1;
    if(writemisc(&bcb) != 0) {
        printf( "Failed to write misc partition\n");
        return -2;
    }
    return 0;
}

long long get_milliseconds() {
    struct timeval tv;
    // 获取当前时间，精确到微秒
    gettimeofday(&tv, NULL);
    // 计算从1970年1月1日开始的毫秒数
    return (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
}

int is_need_delete_resource(){
    struct statfs diskInfo;
    statfs("/data", &diskInfo);
    unsigned long long totalBlocks = diskInfo.f_bsize;
    unsigned long long totalSize = totalBlocks * diskInfo.f_blocks;
    size_t mbTotalsize = totalSize>>20;
    unsigned long long freeDisk = diskInfo.f_bfree*totalBlocks;
    size_t mbFreedisk = freeDisk>>20;
    printf ("/data  total=%dMB, free=%dMB\n", mbTotalsize, mbFreedisk);
    size_t mbFreedisk_base = mbTotalsize * CLEAR_RESOURCE_FREE_RATE;
    if (mbFreedisk < mbFreedisk_base){
        return 1;
    }else{
        return 0;
    }
}
