#ifndef __ESL_MESSAGE_QUEUE_H__
#define __ESL_MESSAGE_QUEUE_H__

typedef struct {
    char * device_sn;   // Message type
    char * message;  // Message value
} esl_msg_t;

int msg_queue_init(int consumer_count);

void post_msg_to_queue(int what, char * device_sn, char * message);

void release_consumer_semaphore();

#endif