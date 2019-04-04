#ifndef _QUEUE_H
#define _QUEUE_H


#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <errno.h>
#include "main.h"

#define HEARTBEAT_QUEUE			("/mq1")
#define LOG_QUEUE				("/mq2")
#define SOCK_QUEUE				("/mq3")
#define LOG_SOCK_QUEUE			("/mq4")


mqd_t heartbeat_mq;
mqd_t log_mq;
mqd_t sock_mq;
mqd_t log_sock_mq;
char x[100];

//function declarations
int queue_init(void);
void queue_send(mqd_t mq, sensor_struct data_send, uint8_t loglevel);
sensor_struct queue_receive(mqd_t mq);
err_t queues_close(void);
err_t queues_unlink(void);


#endif
