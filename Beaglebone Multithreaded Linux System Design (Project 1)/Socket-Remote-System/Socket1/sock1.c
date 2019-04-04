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


int main()
{
	port = PORT;
	uint32_t temp;
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

	socket_request();
	float data;
	if (read(client_fd, (void *)&data, sizeof(data)) < 0)
	{
		perror("Read failed\n\n");
	}
	fprintf(stdout, "Data rcvd %f\n\n", data);
	close(client_fd);
}
