/**
 * @file sockets.c
 * @Satya Mehta and Siddhant Jajoo
 * @Functions supporting sockets initialization.
 * @date 2019-03-22
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "sockets.h"

struct sock_send
{
    uint8_t id;
    float data;
};

/**
 * @Initializes socket and opens port 3124 
 */
void socket_init(void)
{
    port = PORT;
    int opt = 1;
    char string[40];
    if ((serv = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error_log("ERROR: socket() initialization; in socket_init()", ERROR_DEBUG, P2);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    if (setsockopt(serv, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
    {
        error_log("ERROR: setsockopt() in socket_init() function", ERROR_DEBUG, P2);
    }
    if (bind(serv, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error_log("ERROR: bind() failed in socket_init() function", ERROR_DEBUG, P2);
    }
}


/**
 * @brief Used to listen on port again
 * 
 */
void socket_listen()
{
    if (listen(serv, 5) == -1)
    {
        error_log("ERROR: listen(); in sockekt_init() function", ERROR_DEBUG, P2);
    }
    client_len = sizeof(client_addr);
    if ((ser = accept(serv, (struct sockaddr *)&client_addr, &client_len)) == -1)
    {
        error_log("ERROR: accept() in socket_init() function", ERROR_DEBUG, P2);
    }
    else
    {
        msg_log("Connected to remote Host\n", INFO_DEBUG, P0);
    }
    
    
}
/**
 * @brief Send data via socket  depending on light and temperature id's.
 * 
 * @param data_send 
 */

void socket_send(sensor_struct data_send)
{
    struct sock_send a; 
    if(data_send.id == 5)
    {
        a.id = 5;
        a.data = data_send.sensor_data.temp_data.temp_c;
        //send(ser, (void *)&data_send.sensor_data.temp_data.temp_c, sizeof(sensor_struct), 0);
        send(ser, (void *)&a, sizeof(struct sock_send), 0);
    }
    if(data_send.id == 6)
    {
        send(ser, (void *)&data_send.sensor_data.light_data.light, sizeof(sensor_struct), 0);
    }
}

/**
 * @Read data from the socket request
 * 
 * @return int 
 */

int socket_recv(void)
{
    int len, data;
    // len = recv(ser, (void *)&data, sizeof(data), MSG_WAITALL);
    len = read(ser, (void *)&data, sizeof(data));
    if (len == 0)
    {
        return 0;
    }
    printf("Receieved %d bytes\n\n", len);
    printf("Received String %d\n", data);
    fflush(stdout);
    return data;
}
/**
 * @brief Calls socket receive function and sets the flag based on 
 * the request received from the remote machine
 * 
 */

void handle_socket_req()
{
    switch (socket_recv())
    {
    case 100:
        socket_flag |= TC;
        break;
    case 101:
        socket_flag |= TF;
        break;
    case 102:
        socket_flag |= TK;
        break;
    case 103:
        socket_flag |= L;
        break;
    case 104:
        socket_flag |= STATE;
        break;
    case 105:
        socket_flag |= TFL;
        break;
    case 106:
        socket_flag |= TKL;
        break;
    default:
        socket_flag = 0;
        break;
    }
}