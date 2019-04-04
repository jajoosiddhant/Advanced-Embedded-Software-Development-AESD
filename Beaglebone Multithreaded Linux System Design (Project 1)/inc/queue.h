/**
 * @file queue.h
 * @author Siddhant Jajoo and Satya Mehta
 * @brief Header file for queue.c
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef _QUEUE_H
#define _QUEUE_H

#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <errno.h>
#include "main.h"

//Names of the different queues.
#define HEARTBEAT_QUEUE			("/mq1")
#define LOG_QUEUE				("/mq2")
#define SOCK_QUEUE				("/mq3")

//Message Queue handles
mqd_t heartbeat_mq;
mqd_t log_mq;
mqd_t sock_mq;

//Function declarations
int queue_init(void);
void queue_send(mqd_t mq, sensor_struct data_send, uint8_t loglevel, uint8_t prio);
sensor_struct queue_receive(mqd_t mq);
err_t queues_close(void);
err_t queues_unlink(void);


#endif
