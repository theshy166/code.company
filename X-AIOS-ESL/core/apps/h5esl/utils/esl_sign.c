#ifdef CONFIG_XOS_USE_APP_NATIVE_ESL
#include "esl_sign.h"
#include <stdio.h>
#include <string.h>
#include "../utils/md5.h"
#include "../conf/conf.h"

void to_uppercase(char *str) {
    for (; *str; ++str) {
        *str = toupper(*str);
    }
}

char * generate_sign(const char *deviceno, const char *type) {
    char sign_temp[256];
    snprintf(sign_temp, sizeof(sign_temp), "appid=%s&deviceno=%s&type=%s&key=%s", APP_ID, deviceno, type, APP_SECRET);
    
    char *sign = MD5Char(sign_temp);
    if (sign == NULL) {
        return NULL;
    }
    to_uppercase(sign);
    return sign;
}

char *generate_push_server_sign(const char *deviceno, const char *type, const char *protocol_type, const char *local_ip,const char *version_no){
    char sign_temp[256];
    snprintf(sign_temp, sizeof(sign_temp), "appid=%s&deviceno=%s&local_ip=%s&protocol_type=%s&type=%s&version_no=%s&key=%s", APP_ID, deviceno, local_ip, protocol_type, type, version_no, APP_SECRET);
    printf("sign_temp:%s\n",sign_temp);
    char *sign = MD5Char(sign_temp);
    if (sign == NULL) {
        return NULL;
    }
    to_uppercase(sign);
    return sign;
}

char *generate_check_life_sign(const char *deviceno, const char *type, const char *device_list, const char *time){
    size_t sign_temp_size = snprintf(NULL, 0, "appid=%s&device_list=%s&deviceno=%s&time=%s&type=%s&key=%s", APP_ID, device_list, deviceno, time, type, APP_SECRET) + 1;
    char *sign_temp = (char *)malloc(sign_temp_size);
    if (sign_temp == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    snprintf(sign_temp, sign_temp_size, "appid=%s&device_list=%s&deviceno=%s&time=%s&type=%s&key=%s", APP_ID, device_list, deviceno, time, type, APP_SECRET);
    printf("sign_temp: %s\n", sign_temp);

    char *sign = MD5Char(sign_temp);
    free(sign_temp); // 释放动态分配的内存

    if (sign == NULL) {
        return NULL;
    }

    to_uppercase(sign);
    return sign;
}

char *generate_push_message_report(const char *deviceno, const char *type, const char *push_id, const char *push_type,const char *time){
    size_t sign_temp_size = snprintf(NULL, 0, "appid=%s&deviceno=%s&push_id=%s&push_type=%s&time=%s&type=%s&key=%s", APP_ID, deviceno, push_id, push_type,time, type, APP_SECRET) + 1;
    char *sign_temp = (char *)malloc(sign_temp_size);
    if (sign_temp == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    snprintf(sign_temp, sign_temp_size, "appid=%s&deviceno=%s&push_id=%s&push_type=%s&time=%s&type=%s&key=%s", APP_ID, deviceno, push_id, push_type,time, type, APP_SECRET);
    printf("sign_temp: %s\n", sign_temp);

    char *sign = MD5Char(sign_temp);
    free(sign_temp); // 释放动态分配的内存

    if (sign == NULL) {
        return NULL;
    }

    to_uppercase(sign);
    return sign;
}

#endif // CONFIG_XOS_USE_APP_NATIVE_ESL

