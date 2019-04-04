/*Inter process communication using sockets.
Author: Satya Mehta. 
This is a client. 
Referred from:- http://www.it.uom.gr/teaching/distrubutedSite/dsIdaLiu/labs/lab2_1/sockets.html 
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

#define PORT 3124 /* server's port number */
#define MAX_SIZE 100
#define TEMP_RCV_ID (1)
#define LIGHT_RCV_ID (2)
#define ERROR_RCV_ID (3)
#define MSG_RCV_ID (4)
#define SOCK_TEMP_RCV_ID (5)
#define SOCK_LIGHT_RCV_ID (6)

int len;
struct sockaddr_in client_addr;
char *serv_host = "192.168.0.41";
struct hostent *hptr;
int buff_size = 0;
int client_fd, client_f, clilen, port;
char string[MAX_SIZE];
typedef uint32_t err_t;
char *filename = "log_system.txt";

struct temp_struct
{
	float temp_c;
	struct timespec data_time;
	uint8_t logger_level;
};

struct light_struct
{
	float light;
	struct timespec data_time;
	uint8_t logger_level;
};

struct error_struct
{
	struct timespec data_time;
	err_t error_value;
	char error_str[50];
};

struct msg_struct
{
	char msg_str[50];
};

typedef struct
{
	uint8_t id;
	union sensor_data {
		struct temp_struct temp_data;
		struct light_struct light_data;
		struct error_struct error_data;
		struct msg_struct msg_data;

	} sensor_data;

} sensor_struct;

struct sock_send
{
    uint8_t id;
    float data;
};

void signal_handler(int signo, siginfo_t *info, void *extra)
{
	if (signo == 2)
	{
		close(client_fd);
		printf("\nTerminating due to signal number = %d.\n", signo);
		//The below command should come after printing terminating in the text file.
		exit(EXIT_SUCCESS);
	}
}

err_t sig_init()
{
	struct sigaction send_sig;
	send_sig.sa_flags = SA_SIGINFO;
	send_sig.sa_sigaction = &signal_handler;
	if (sigaction(SIGINT, &send_sig, NULL))
	{
		perror("ERROR: sigaction(); in sig_init() function");
	}
}

void socket_request(void)
{
	const char *strings[7] = {"TC", "TF", "TK", "L", "TCL", "TKL", "TFL"};
	int strings_define[7] = {100, 101, 102, 103, 104, 105, 106};
	printf("Client fd %d\n", client_fd);
	char data[3];
	printf("\nEnter one of the available commands\n\n");
	printf("Press TC and enter to request temperature in Celsius\n");
	printf("Press TF and enter to request temperature in Fahrenheit\n");
	printf("Press TK and enter to request temperature in Kelvin\n");
	printf("Press L and enter to request Light intensity in Lux\n");
	scanf("%s", data);
	if (strcmp(data, strings[0]) == 0)
	{
		if (send(client_fd, (void *)&strings_define[0], sizeof(strings_define[0]), 0) == -1)
		{
			perror("send failed");
		}
	}
	else if (strcmp(data, strings[1]) == 0)
	{
		if (send(client_fd, (void *)&strings_define[1], sizeof(strings_define[1]), 0) == -1)
		{
			perror("send failed");
		}
	}

	else if (strcmp(data, strings[2]) == 0)
	{
		if (send(client_fd, (void *)&strings_define[2], sizeof(strings_define[2]), 0) == -1)
		{
			perror("send failed");
		}
	}

	else if (strcmp(data, strings[3]) == 0)
	{
		if (send(client_fd, (void *)&strings_define[3], sizeof(strings_define[3]), 0) == -1)
		{
			perror("send failed");
		}
	}

	else if (strcmp(data, strings[4]) == 0)
	{
		if (send(client_fd, (void *)&strings_define[4], sizeof(strings_define[4]), 0) == -1)
		{
			perror("send failed");
		}
	}

	else if (strcmp(data, strings[5]) == 0)
	{
		if (send(client_fd, (void *)&strings_define[5], sizeof(strings_define[5]), 0) == -1)
		{
			perror("send failed");
		}
	}
	else
	{
		printf("Wrong Input\n\n");
		socket_request();
	}
}

void log_data(struct sock_send data_rcv)
{
	printf("%d\n", data_rcv.id);
	printf("%f", data_rcv.data);
	switch (data_rcv.id)
	{
	// case TEMP_RCV_ID:
	// {
	// 	FILE *logfile = fopen(filename, "a");
	// 	fprintf(stdout,"DATA RECEIVED\n");
	// 	fprintf(stdout, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.temp_data.data_time.tv_sec, data_rcv.sensor_data.temp_data.data_time.tv_nsec);
	// 	fprintf(stdout, "In logger thread temperature Value: %f.\n", data_rcv.sensor_data.temp_data.temp_c);
	// 	fprintf(stdout, "\n***********************************\n\n");
	// 	fprintf(logfile,"DATA RECEIVED\n");
	// 	fprintf(logfile, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.temp_data.data_time.tv_sec, data_rcv.sensor_data.temp_data.data_time.tv_nsec);
	// 	fprintf(logfile, "In logger Thread temperature Value: %f.\n", data_rcv.sensor_data.temp_data.temp_c);
	// 	fprintf(logfile, "\n***********************************\n\n");
	// 	break;
	// }

	// case LIGHT_RCV_ID:
	// {
	// 	FILE *logfile = fopen(filename, "a");
	// 	fprintf(stdout,"DATA RECEIVED\n");
	// 	fprintf(stdout, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.light_data.data_time.tv_sec, data_rcv.sensor_data.light_data.data_time.tv_nsec);
	// 	fprintf(stdout, "In logger thread Light Value: %f.\n", data_rcv.sensor_data.light_data.light);
	// 	fprintf(stdout, "\n***********************************\n\n");
	// 	fprintf(logfile,"DATA RECEIVED\n");
	// 	fprintf(logfile, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.light_data.data_time.tv_sec, data_rcv.sensor_data.light_data.data_time.tv_nsec);
	// 	fprintf(logfile, "In logger Thread Light Value: %f.\n", data_rcv.sensor_data.light_data.light);
	// 	fprintf(logfile, "\n***********************************\n\n");
	// 	fclose(logfile);
	// 	break;
	// }

	// case ERROR_RCV_ID:
	// {
	// 	FILE *logfile = fopen(filename, "a");
	// 	fprintf(stdout,"DATA RECEIVED\n");
	// 	fprintf(stdout, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.error_data.data_time.tv_sec, data_rcv.sensor_data.error_data.data_time.tv_nsec);
	// 	fprintf(stdout, "%s.\n", data_rcv.sensor_data.error_data.error_str);
	// 	fprintf(stdout, "%s.\n", strerror(data_rcv.sensor_data.error_data.error_value));
	// 	fprintf(stdout, "\n***********************************\n\n");
	// 	fprintf(logfile,"DATA RECEIVED\n");
	// 	fprintf(logfile, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.error_data.data_time.tv_sec, data_rcv.sensor_data.error_data.data_time.tv_nsec);
	// 	fprintf(logfile, "%s.\n", data_rcv.sensor_data.error_data.error_str);
	// 	fprintf(logfile, "%s.\n", strerror(data_rcv.sensor_data.error_data.error_value));
	// 	fprintf(logfile, "\n***********************************\n\n");
	// 	fclose(logfile);
	// 	break;
	// }

	// case MSG_RCV_ID:
	// // {
	// // 	FILE *logfile = fopen(filename, "a");
	// // 	fprintf(stdout,"DATA RECEIVED\n");
	// // 	fprintf(stdout, "%s", data_rcv.sensor_data.msg_data.msg_str);

	// // 	fprintf(logfile,"DATA RECEIVED\n");
	// // 	fprintf(logfile, "%s", data_rcv.sensor_data.msg_data.msg_str);

	// // 	fclose(logfile);
	// 	break;
	// }
	case SOCK_TEMP_RCV_ID:
	{
		// FILE *logfile = fopen(filename, "a");
		// fprintf(stdout,"Temperature %f.\n", data_rcv.sensor_data.temp_data.temp_c);
		// fprintf(logfile,"Temperature %f.\n", data_rcv.sensor_data.temp_data.temp_c);
		// fprintf(logfile, "\n***********************************\n\n");
		// fclose(logfile);
		break;
	}
	case SOCK_LIGHT_RCV_ID:
	{
		// FILE *logfile = fopen(filename, "a");
		// fprintf(stdout, "Light Value: %f.\n", data_rcv.sensor_data.light_data.light);
		// fprintf(stdout, "\n***********************************\n\n");
		// fprintf(logfile, "Light Value: %f.\n", data_rcv.sensor_data.light_data.light);
		// fprintf(logfile, "\n***********************************\n\n");
		// fclose(logfile);
		break;
	}

	default:
		break;
	}
}

int main()
{
	port = PORT;
	uint32_t temp;
	sensor_struct rcv_data;
	sig_init();
	if ((hptr = gethostbyname(serv_host)) == NULL)
	{
		perror("gethostbyname error");
		exit(1);
	}

	if (hptr->h_addrtype != AF_INET)
	{
		perror("unknown address type");
		exit(1);
	}

	bzero((char *)&client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = ((struct in_addr *)hptr->h_addr_list[0])->s_addr;
	client_addr.sin_port = htons(port);
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("CAN'T OPEN SOCKET");
		exit(EXIT_FAILURE);
	}
	if (connect(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
	{
		perror("can't connect to server");
		exit(1);
	}
	//socket_request();
	//while (1)
	{
		socket_request();
		float data;
		struct sock_send a;
		if(recv(client_fd, (void *)&a, sizeof(struct sock_send), 0)<0)
		//if (read(client_fd, (void *)&data, sizeof(rcv_data)) < 0)
		{
			perror("Read failed\n\n");
		}
		log_data(a);
		fprintf(stdout,"Data rcvd %f\n\n",data);
		//if(read(client_fd, (void *)&rcv_data, sizeof(rcv_data)) < 0)
		// {
		// 	perror("Read failed\n\n");
		// }
		// log_data(rcv_data);
	}
	close(client_fd);
}
