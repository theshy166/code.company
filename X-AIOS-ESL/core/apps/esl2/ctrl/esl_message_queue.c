#ifdef CONFIG_XOS_APP_ESL2
#include "esl_message_queue.h"
#include <semaphore.h>
#include "cJSON.h"
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include "../conf/conf.h"
#include "esl_socket_service.h"
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "utils/qua_handler.h"
#include "utils/qua_msg_queue.h"
#endif

#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#define MAX_OTA_STACK_SIZE 200
#define OTA_BATCH_SIZE 3
#define MESSAGE_TIMEOUT 60 // 60秒

typedef struct {
    esl_msg_t *stack[MAX_OTA_STACK_SIZE];
    int top;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} ota_stack_t;

static ota_stack_t ota_stack = {.top = -1};
static pthread_t ota_thread;

static void* handle_ota(void *arg) {
    while (1) {
        QM_ESL2_LOG("handle_ota top:%d\n",ota_stack.top);
        pthread_mutex_lock(&ota_stack.lock);

        // 当栈空时阻塞等待
        while (ota_stack.top == -1) {
            pthread_cond_wait(&ota_stack.cond, &ota_stack.lock);
        }
        //等待5秒内收集一次发送过来的消息
        sleep(5);
        int process_count = (ota_stack.top + 1) > OTA_BATCH_SIZE ? OTA_BATCH_SIZE : (ota_stack.top + 1);
        QM_ESL2_LOG("Processing %d OTA messages\n", process_count);
        for (int i = 0; i < process_count; i++) {
            esl_msg_t *value = ota_stack.stack[ota_stack.top--];
            QM_ESL2_LOG("Processing OTA message: %s,%s\n", value->message, value->device_sn);
            // 处理消息
            create_send_socket(value->device_sn, value->message);

            // 释放资源
            free(value->device_sn);
            free(value->message);
            free(value);
        }
        pthread_mutex_unlock(&ota_stack.lock);
        if(process_count == OTA_BATCH_SIZE){
            sleep(MESSAGE_TIMEOUT); // 每120秒 发送OTA_BATCH_SIZE次消息
        }
    }
    return NULL;
}

static int push_value(esl_msg_t *value) {
    pthread_mutex_lock(&ota_stack.lock);
    if (ota_stack.top >= (MAX_OTA_STACK_SIZE - 1)) {
        pthread_mutex_unlock(&ota_stack.lock);
        QM_ESL2_LOG("OTA stack is full, unable to push value\n");
        return -1; // 栈满
    }
    QM_ESL2_LOG("ota_stack.top = %d\n",ota_stack.top);
    // 深拷贝消息内容
    esl_msg_t *new_value = malloc(sizeof(esl_msg_t));
    if (!new_value) {
        pthread_mutex_unlock(&ota_stack.lock);
        QM_ESL2_LOG("Memory allocation failed\n");
        return -1; // 内存分配失败 
    }
    new_value->device_sn = strdup(value->device_sn);
    new_value->message = strdup(value->message);
    QM_ESL2_LOG("Pushed value: %s,%s\n", new_value->message,new_value->device_sn);
    // 压栈  
    ota_stack.stack[++ota_stack.top] = new_value;
    // 压栈成功后发送信号
    if (ota_stack.top >= 0) {
        pthread_cond_signal(&ota_stack.cond);
    }
    pthread_mutex_unlock(&ota_stack.lock);
    return 0;
}
#endif

#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    static qua_handler_t *handler;
    static sem_t consumer_semaphore; // 信号量
#endif

#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
static void* _create_send_socket_slave(void *arg) {
    qua_msg_t *msg = (qua_msg_t *)arg;
    esl_msg_t *value = *(esl_msg_t **)msg->payload;
    // 解析value->message ，只有dtype为1的才需要创建socket
    // 其他的直接返回   
    cJSON *json = cJSON_Parse(value->message);
    if (json == NULL) {
        QM_ESL2_LOG("Failed to parse JSON message\n");
        free(value->device_sn);
        free(value->message);
        free(value);
        qua_destroy_msg(msg);
        return NULL;
    }
    cJSON *dateTypeItem = cJSON_GetObjectItem(json, "date_type");
    if (dateTypeItem == NULL || !cJSON_IsNumber(dateTypeItem)) {
        QM_ESL2_LOG("_create_send_socket_slave get dt fail!\n");
        cJSON_Delete(json);
        free(value->device_sn);
        free(value->message);
        free(value);
        qua_destroy_msg(msg);
        return NULL;
    }
    int dt = dateTypeItem->valueint;
    /* if(dt == 3 || dt == 4){
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 180; // 设置获取信号量的超时时间为 180 秒
        // 打印 ts 的值
        QM_ESL2_LOG("Current time: %ld seconds, %ld nanoseconds\n", ts.tv_sec, ts.tv_nsec);
        if(sem_timedwait(&consumer_semaphore,&ts) == -1){
            if(errno == ETIMEDOUT){
                QM_ESL2_LOG("Semaphore timed out\n");
                release_consumer_semaphore();
            }
        }
    }else  */
    if(dt == 10)
    {
        cJSON *configItem = cJSON_GetObjectItem(json, "config");
        if (!configItem || !cJSON_IsString(configItem)) {
            QM_ESL2_LOG("_create_send socket_slave get config fail!\n");
            cJSON_Delete(json);
            free(value->device_sn);
            free(value->message);
            free(value);
            qua_destroy_msg(msg);
            return NULL;
        }

        // 解析嵌套的JSON
        cJSON *configJson = cJSON_Parse(configItem->valuestring);
        if (!configJson) {
            QM_ESL2_LOG("Parse config JSON failed\n");
            cJSON_Delete(json);
            free(value->device_sn);
            free(value->message);
            free(value);
            qua_destroy_msg(msg);
            return NULL;
        }

        cJSON *typeItem = cJSON_GetObjectItem(configJson, "type");
        if (!typeItem || !cJSON_IsString(typeItem)) {
            QM_ESL2_LOG("_create_send socket_slave get type fail!\n");
            cJSON_Delete(configJson);
            cJSON_Delete(json);
            free(value->device_sn);
            free(value->message);
            free(value);
            qua_destroy_msg(msg);
            return NULL;
        }

        char *type = typeItem->valuestring;
        QM_ESL2_LOG("type is %s\n", type);

        char type_buffer[64] = {0};
        strncpy(type_buffer, type, sizeof(type_buffer)-1);
        // 释放嵌套JSON
        cJSON_Delete(configJson);
        if(strcmp(type_buffer,"version_update") == 0) {
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            ts.tv_sec += 30;//设置获取信号量的超时时间为 30 秒
            // 打印 ts 的值
            QM_ESL2_LOG("Current time: %ld seconds, %ld nanoseconds\n", ts.tv_sec, ts.tv_nsec);
            if(sem_timedwait(&consumer_semaphore,&ts) == -1) {//等待信号量
                if(errno == ETIMEDOUT){
                    QM_ESL2_LOG("Semaphore timed out\n");
                    release_consumer_semaphore();
                }
            }
        }
    }
    create_send_socket(value->device_sn,value->message);
    cJSON *pushIdItem = cJSON_GetObjectItem(json, "push_id");
    if(pushIdItem == NULL || !cJSON_IsString(pushIdItem)){
        free(value->device_sn);
        free(value->message);
        free(value);
        qua_destroy_msg(msg);
        return NULL;
    }
    char *push_id = strdup(pushIdItem->valuestring);
    char * master_device_no = get_master_device_sn();
    //消息状态上报
    time_t seconds;
    seconds = time(NULL);
    QM_ESL2_LOG("Seconds: %ld\n", seconds);
    // 将 seconds 转换为字符串
    char time_str[20];
    snprintf(time_str, sizeof(time_str), "%ld", seconds);
    QM_ESL2_LOG("Time string: %s\n", time_str);
    char *sign = generate_push_message_report(master_device_no,"push_report",push_id,"1",time_str);
    QM_ESL2_LOG("Sign: %s\n", sign);
    push_message_report(master_device_no,"push_report", push_id, "1", time_str, sign);
    cJSON_Delete(json);
    free(value->device_sn);
    free(value->message);
    free(value);
    qua_destroy_msg(msg);
    return NULL;
}
#endif

#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
static void handle_msg(qua_msg_t *msg) {

    esl_msg_t *value = *(esl_msg_t **)msg->payload;
    // 内存分配失败
    if (value == NULL) {
        QM_ESL2_LOG("handle_msg malloc failed\n");
        return;
    }
    //printf("handle_msg value->message is %s\n", value->message);
    cJSON *json = cJSON_Parse(value->message); 
    if (json == NULL) {
        QM_ESL2_LOG("Failed to parse JSON message\n");
        free(value->device_sn);
        free(value->message);
        free(value);
        qua_destroy_msg(msg);
        return;
    }

    cJSON *typeItem = cJSON_GetObjectItem(json, "type");
    if (typeItem == NULL || !cJSON_IsNumber(typeItem)) {
        QM_ESL2_LOG("cJSON_GetObjectItem type fail!\n");
        cJSON_Delete(json);
        free(value->device_sn);
        free(value->message);
        free(value);
        qua_destroy_msg(msg);
        return;
    }
    int type = typeItem->valueint;
    QM_ESL2_LOG("type is %d\n", type);
    if(type == EslMessageTypeUPDATE) {
        char * master_device_no = get_master_device_sn();
        // 检查master_device_no是否与value->device_sn匹配
        // 如果相等就返回
        if( master_device_no != NULL && \
            strlen(master_device_no) > 2 && \
            strlen(master_device_no) == strlen(value->device_sn) && \
            strncmp(master_device_no,value->device_sn,strlen(master_device_no)-1) == 0)
        {
            QM_ESL2_LOG("master_device_no is match, ignor master device\n");
            cJSON_Delete(json);
            free(value->device_sn);
            free(value->message);
            free(value);
            qua_destroy_msg(msg);
            return;
        }
        //QM_ESL2_LOG("checkUpdate master_device_no:%s\n",master_device_no);
        // 压栈处理
        if (push_value(value) == 0) {
            // 成功入栈后释放资源并返回
            cJSON_Delete(json);
            free(value->device_sn);
            free(value->message);
            free(value);
            qua_destroy_msg(msg);
            return;
        }
        // 栈满处理
        QM_ESL2_LOG("OTA stack is full\n");

        cJSON_Delete(json);
        free(value->device_sn);
        free(value->message);
        free(value);
        qua_destroy_msg(msg);
        return;
    }

    post_push_message("2",json);
    int flag = create_send_socket(value->device_sn,value->message);
    if(flag == 0){
        post_push_message("3",json);
    }
    cJSON_Delete(json);
    free(value->device_sn);
    free(value->message);
    free(value);
    qua_destroy_msg(msg);
}
#endif

int msg_queue_init(int consumer_count) {
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        QM_ESL2_LOG("qua handler test start\n");
        if (handler == NULL) {
            handler = (qua_handler_t *)qua_handler_create(handle_msg, "master_send_");
            if (!handler) {
                QM_ESL2_LOG("create qua handler failed\n");
                return -1;
            }
            // 初始化信号量，允许最多 3 个消费者
            sem_init(&consumer_semaphore, 0, consumer_count);
        }
        // 初始化互斥锁,条件变量 和OTA线程
        QM_ESL2_LOG("handle_ota init\n");
        pthread_cond_init(&ota_stack.cond, NULL);
        pthread_mutex_init(&ota_stack.lock, NULL);
        pthread_create(&ota_thread, NULL, handle_ota, NULL);
        pthread_setname_np(ota_thread, "handle_ota");
        pthread_detach(ota_thread);
    #endif
    return 0;
}


void post_msg_to_queue(int what, char * device_sn, char * message){
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        if (handler == NULL) {
            QM_ESL2_LOG("handler is NULL\n");
            return;
        }
        esl_msg_t * value = malloc(sizeof(esl_msg_t*));
        if (value == NULL) {
            QM_ESL2_LOG("malloc failed\n");
            return;
        }
        memset(value, 0, sizeof(esl_msg_t*));
        value->device_sn = strdup(device_sn);
        value->message = strdup(message);
        qua_msg_t *msg = qua_create_msg(what, &value, sizeof(value), QUA_MSG_PRIORITY_NORMAL, NULL);
        if (msg == NULL) {
            QM_ESL2_LOG("create msg failed\n");
            return;
        }
        qua_handler_post_msg(handler, msg, 0);
        QM_ESL2_LOG("post msg to queue, what=%d, value=%d\n", what, value);
    #endif
}

void release_consumer_semaphore(){
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        int sem_value;
        sem_getvalue(&consumer_semaphore, &sem_value); // 获取信号量的当前值
        QM_ESL2_LOG("Current semaphore value: %d\n", sem_value);
        if (sem_value < MAX_MESSAGE_CONSUMER_COUNT) {
            sem_post(&consumer_semaphore); // 释放信号量
        } else {
            QM_ESL2_LOG("Semaphore is already at maximum value, not releasing.\n");
        }
    #endif
}

#endif
