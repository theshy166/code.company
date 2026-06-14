#ifdef CONFIG_XOS_USE_APP_NATIVE_ESL
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <netdb.h>
#include <ifaddrs.h>
#include "cJSON.h"
#include "esl_socket_udp_service.h"
#include "esl_socket_service.h"
#include "../conf/conf.h"
#include "esl_service.h"
#include "../utils/utils.h"

#define BUFFER_SIZE 256

static int _receive_broadcast_flag = 0;

static int local_group_id_1 = 0;
static int local_group_id_2 = 0;

void send_broadcast_message(char * broadcast_ip,char * msg) {
    printf("Broadcast message sent: %s , %s\n",broadcast_ip, msg);
    int sockfd;
    struct sockaddr_in broadcast_addr;
    int broadcast_enable = 1;

    // 创建UDP套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    // 设置套接字选项以启用广播
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        perror("setsockopt");
        close(sockfd);
        return;
    }

    // 配置广播地址
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(OPEN_BROADCAST_HOST);
    broadcast_addr.sin_addr.s_addr = inet_addr(broadcast_ip);

    // 发送广播消息
    printf("send_broadcast_message sin_port: %d\n",OPEN_BROADCAST_HOST);
    if (sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0) {
        perror("sendto");
        close(sockfd);
        return;
    }

    printf("Broadcast message sent: %s\n", msg);

    close(sockfd);
}


extern char * str_find_next(char *str, char *find, char end, char *temp_string);
extern char * str_find_ip(char *str, char *temp_string);
void * consume_message(void * arg){
    prctl(PR_SET_NAME, "consume_udp_message"); // 设置线程名称
    char *message = (char *)arg;
    char to_send_string[128] = {0};
    char temp_string[128] = {0};
    //{"date_type":998,"master_group_id":274,"master_ip":"172.16.58.96"}
    printf("Consuming message: %s\n", message);

    //获取随机数 随机sleep 1-3000毫秒
    int random_sleep = rand() % 20000 + 1;
    printf("random_sleep: %d =========================\n", random_sleep);
    usleep(random_sleep * 1000);

    char *date_type_ptr = str_find_next(message, "date_type", ',', temp_string);
    if(date_type_ptr == NULL){
        printf("consume_message date_type_ptr is NULL\n");
        free(message);
        return NULL;
    }
    date_type_ptr += 2;
    // printf("consume_message date_type_ptr: %s\n", date_type_ptr);
    if (date_type_ptr[0] != '9' || date_type_ptr[1] != '9' || date_type_ptr[2] != '8'){
        free(message);
        return NULL;
    }

    //广播接收
    printf("广播接收_%d\n", __LINE__);
    memset(temp_string, 0, sizeof(temp_string));
    char *master_group_id_ptr = str_find_next(message, "master_group_id", ',', temp_string);
    if(master_group_id_ptr == NULL){
        printf("consume_message master_group_id_ptr is NULL\n");
        free(message);
        return NULL;
    }
    master_group_id_ptr += 2;
    int master_group_id = atoi(master_group_id_ptr);
    // printf("consume_message master_group_id: %d, local_group_id: %d\n", master_group_id, local_group_id);
    /* if(master_group_id != local_group_id && local_group_id !=0){
        printf("consume_message master_group_id:%d != local_group_id:%d, free message\n", master_group_id, local_group_id);
        free(message);
        return NULL;
    } */
    //
    // printf("consume_message master_group_id_ptr: %s\n", master_group_id_ptr);
    memset(temp_string, 0, sizeof(temp_string));
    char *master_ip_ptr = str_find_ip(message, temp_string);
    if (master_ip_ptr == NULL) {
        printf("consume_message master_ip_ptr is NULL, free message\n");
        free(message);
        return NULL;
    }
    // printf("consume_message master_ip_ptr: %s\n", master_ip_ptr);
   
    // 发送消息给master
    char *master_ip = master_ip_ptr;
    char ip[NI_MAXHOST];
    get_local_ip(ip, NI_MAXHOST);
    if(ip == NULL){
        printf("consume_message get_local_ip is NULL, free message\n");
        free(message);
        return NULL;
    }
    char buffer[16]; // 假设整数不会超过 15 位
    snprintf(buffer, sizeof(buffer), "%d", master_group_id);

    if(local_group_id_1 == 0 || local_group_id_1 == master_group_id){
        char * device_sn = get_device_sn(1);
        //单屏情况下会有问题，做个兼容
        if (!is_string_empty(device_sn)){
            snprintf(to_send_string, sizeof(to_send_string), "{\"date_type\":999,\"device_no\":\"%s\",\"device_host\":\"%s\"}", device_sn, ip);
            create_send_socket_slave(master_ip, to_send_string);
            printf("create_send_socket_slave : %s,%s\n", master_ip, to_send_string);
            printf("consume_message master_group_id:%d == local_group_id_1:%d \n", master_group_id, local_group_id_1);
            put_group_map(buffer,master_ip,1);
        }
    }

    if(local_group_id_2 == 0 || local_group_id_2 == master_group_id){
        char * device_sn = get_device_sn(2);
        //单屏情况下会有问题，做个兼容
        if (!is_string_empty(device_sn)){
            snprintf(to_send_string, sizeof(to_send_string), "{\"date_type\":999,\"device_no\":\"%s\",\"device_host\":\"%s\"}", device_sn, ip);
            create_send_socket_slave(master_ip, to_send_string);
            printf("create_send_socket_slave : %s,%s\n", master_ip, to_send_string);
            printf("consume_message master_group_id:%d == local_group_id_2:%d \n", master_group_id, local_group_id_2);
            put_group_map(buffer,master_ip,2);
        }
    }

    free(message);
    return NULL;
}

void recevice_msg_and_send(int group_id_1,int group_id_2){
    printf("\n\n----------recevice_msg_and_send:%d, %d\n",group_id_1,group_id_2);

    local_group_id_1 = group_id_1;
    local_group_id_2 = group_id_2;

    int sockfd;
    struct sockaddr_in recv_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(recv_addr);

    // 创建UDP套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    // 配置接收地址
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(OPEN_BROADCAST_HOST);
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 绑定套接字到接收地址
    printf("recevice_msg_and_send sin_port: %d\n",OPEN_BROADCAST_HOST);
    if (bind(sockfd, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return;
    }
    while(1){
        printf("================before recvfrom----------------------\n");
        // 接收广播消息
        int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&recv_addr, &addr_len);
        if (recv_len < 0) {
            perror("recvfrom");
            // close(sockfd);//这里不能close，否则continue会死循环
            usleep(100*1000);
            continue;
        }

        buffer[recv_len] = '\0';
        printf("========after recvfrom msg: %s\n", buffer);

        // 创建一个新的线程来消费消息
        char *message = malloc(BUFFER_SIZE);
        if (message == NULL) {
            perror("strdup");
            // close(sockfd);//这里不能close，否则continue会死循环
            usleep(100*1000);
            continue;
        }

        memcpy(message, buffer, recv_len < (BUFFER_SIZE - 1) ? recv_len : (BUFFER_SIZE - 1));
        message[recv_len] = '\0';

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, consume_message, (void *)message) != 0) {
            perror("pthread_create");
            free(message);
            // close(sockfd);//这里不能close，否则continue会死循环
            usleep(100*1000);
            continue;
        }

        // 分离线程
        pthread_detach(thread_id);
    }
    close(sockfd);
}

void set_local_group_id_1(int group_id){
    local_group_id_1 = group_id;
}
void set_local_group_id_2(int group_id){
    local_group_id_2 = group_id;
}

int get_local_group_id(int index){
    if(index == 1){
        return local_group_id_1;
    }else if(index == 2){
        return local_group_id_2;
    }
}
#endif