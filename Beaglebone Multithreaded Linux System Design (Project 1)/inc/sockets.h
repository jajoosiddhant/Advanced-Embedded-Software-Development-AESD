/**
 * @file sockets.h
 * @author Siddhant Jajoo and Satya Mehta
 * @brief Header file of sockets.c
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _SOCKETS_H
#define _SOCKETS_H

#include <stdio.h>
#include "main.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "queue.h"

#define PORT 3124   /* server's port number */
#define MAX_SIZE    0x01
#define TC          0x02
#define TF          0x04
#define TK          0x08
#define L           0x10
#define STATE       0x12
#define TFL         0x14
#define TKL         0x18     

//Variable Declarations
int serv, ser, client_len, port;
struct sockaddr_in serv_addr, client_addr;
struct hostent *hptr;

//Function Declarations
void socket_init(void);
int socket_recv(void);
void handle_socket_req(void);
void socket_send(sensor_struct);
void socket_listen(void);

#endif
