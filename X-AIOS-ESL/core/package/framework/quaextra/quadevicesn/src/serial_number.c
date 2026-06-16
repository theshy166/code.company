#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 2048
#define SN_DEFAULT  "D1S920H73ODI667"
char* deviceSerialNumber = NULL;
char* clientid = NULL;

static char * _query_deviceSN();

static int isStringEmpty(const char *str) {
    return str == NULL || str[0] == '\0';
}

void initializeSerialNumber(){
    if (deviceSerialNumber == NULL) {
        deviceSerialNumber = _query_deviceSN();
        if(isStringEmpty(deviceSerialNumber)){
            printf("SN value: %s\n", deviceSerialNumber);
            //设备号就取默认值 测试专用
            deviceSerialNumber = strdup(SN_DEFAULT);
        }
        printf("SN value: %s\n", deviceSerialNumber);
    }

    // 先释放之前可能存在的clientid
    if (clientid != NULL) {
        free(clientid);
        clientid = NULL;
    }

    clientid = malloc(strlen(deviceSerialNumber) + strlen("client-id-") + 1);
    if (clientid == NULL) {
        printf("Failed to allocate memory for clientid\n");
        return;
    }
    clientid[0] = '\0';
    strcat(clientid, "client-id-");
    strcat(clientid, deviceSerialNumber);
}

bool CheckSerialNumber(){
    if(isStringEmpty(deviceSerialNumber)) {
        return false;
    }
    //对比deviceSerialNumber 是否等于"D1S920H73ODI667"
    //如果相等，就是没有在cmdline里找到sn, 就return false
    //否则，就是在cmdline里找到了sn，就return true
    if(strcmp(deviceSerialNumber,SN_DEFAULT) == 0) {
        return false;
    }
    return true;
}

static char* findSN(const char* content) {
    char* saveptr;
    char* content_copy = strdup(content); // 创建内容的副本
    if (content_copy == NULL) {
        return NULL;
    }

    char* line = strtok_r(content_copy, "\n", &saveptr); // 首先按行分割
    while (line != NULL) {
        char* token;
        char* word_saveptr;
        token = strtok_r(line, " ", &word_saveptr); // 然后按空格分割每一行
        
        while (token != NULL) {
            if (strstr(token, "sn=") != NULL) {
                char* result = strdup(token + 3); // 复制找到的SN值
                free(content_copy);
                return result;
            }
            token = strtok_r(NULL, " ", &word_saveptr);
        }
        line = strtok_r(NULL, "\n", &saveptr);
    }
    
    free(content_copy);
    return NULL;
}

static char * _query_deviceSN() {
    FILE *file = fopen("/proc/cmdline", "r");
    //FILE *file = fopen("./temp", "r");
    if (file == NULL) {
        perror("Error opening /proc/cmdline");
        return NULL;
    }

    char buffer[BUFFER_SIZE] = {0};
    size_t total_read = 0;
    size_t bytes_read;
    
    // 读取整个文件内容
    while ((bytes_read = fread(buffer + total_read, 1, 
           BUFFER_SIZE - total_read - 1, file)) > 0) {
        total_read += bytes_read;
        if (total_read >= BUFFER_SIZE - 1) break;
    }
    
    buffer[total_read] = '\0';
    fclose(file);

    return findSN(buffer);
}

// 添加清理函数
void cleanupSerialNumber() {
    if (clientid != NULL) {
        free(clientid);
        clientid = NULL;
    }
    if (deviceSerialNumber != NULL){
        free(deviceSerialNumber);
        deviceSerialNumber = NULL;
    }
    
}