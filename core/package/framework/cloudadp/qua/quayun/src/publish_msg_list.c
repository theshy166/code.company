#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <string.h>
#include <quaMqtt.h>
#include "lvgl/lvgl.h"
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>


int _strlen(const char* str)
{
    int count = 0;
    while (*str != '\0')
    {
        count++;
        str++;
    }
    return count;
}
// 队列初始化  
void queue_init(PbQueue* q) {  
    q->front = q->rear = NULL;  
    q->size = 0;  
    pthread_mutex_init(&q->lock, NULL);  
}  
  
// 队列销毁  
void queue_destroy(PbQueue* q) {  
    PbNode* current = q->front;  
    while (current != NULL) {  
        PbNode* next = current->next;  
        free(current->str1);  
        free(current->str2);  
        free(current);  
        current = next;  
    }  
    pthread_mutex_destroy(&q->lock);  
}
  
// 入队  
void queue_enqueue(PbQueue* q, const char* str1, const char* str2) {  
    pthread_mutex_lock(&q->lock);  
    if (q->size >= 10) { // 假设队列最大容量为100  
        pthread_mutex_unlock(&q->lock);  
        fprintf(stderr, "Queue is full\n");  
        return;  
    }  
  
    PbNode* newNode = (PbNode*)malloc(sizeof(PbNode));  
    newNode->str1 = (char*)malloc(_strlen(str1)+1);  
    newNode->str2 = (char*)malloc(_strlen(str2)+1);  
    strcpy(newNode->str1,str1);
    strcpy(newNode->str2,str2);
    newNode->next = NULL;  
    //printf("queue_enqueue newNode->str1=%s newNode->str2=%s\n", newNode->str1, newNode->str2);
    if (q->rear == NULL) {  
        q->front = q->rear = newNode;  
    } else {  
        q->rear->next = newNode;  
        q->rear = newNode;  
    }  
    q->size++;  
  
    pthread_mutex_unlock(&q->lock);  
}  
  
#if 0
PbNode* queue_dequeue(PbQueue* q) {  
    pthread_mutex_lock(&q->lock);  
    if (q->front == NULL) {  
        pthread_mutex_unlock(&q->lock);  
        return NULL;  
    }  
  
    PbNode* temp = q->front;  

    printf("queue_dequeue temp->str1=%s temp->str2=%s,point %d\n", temp->str1, temp->str2,temp);

    q->front = q->front->next;  
  
    if (q->front == NULL) {  
        q->rear = NULL;  
    }  
    q->size--;  
    pthread_mutex_unlock(&q->lock);  
    printf("queue_dequeue temp->str1=%d temp->str2=%d,point %d\n", temp->str1, temp->str2,temp);
    return temp;  
}  

#else


int queue_dequeue(PbQueue* q,char* out_str1,char* out_str2) {  
    pthread_mutex_lock(&q->lock);  
    if (q->front == NULL) {  
        pthread_mutex_unlock(&q->lock);  
        return -1;  
    }  
    PbNode* temp = q->front;  
    strcpy(out_str1,temp->str1);
    strcpy(out_str2,temp->str2);
    q->front = q->front->next;  
  
    if (q->front == NULL) {  
        q->rear = NULL;  
    }  
    q->size--;  
    pthread_mutex_unlock(&q->lock);  
    free(temp->str1);
    free(temp->str2);
    free(temp);

    return 0;
}  

#endif