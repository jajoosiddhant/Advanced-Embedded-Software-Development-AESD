
/***************************************************
/*
* File Name: IPC using sockets
* Description: Sending data bidirectionally by sockets  
* Author: Siddhant Jajoo
* Reference: www.it.uom.gr/teaching/distributedSite/dsldaLiu/labs/labs2_1/Sockets.html
* Date: 02/28/2019
/***************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <wait.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/stat.h>
#include <netdb.h>


#define ELEMENTS			(8)
#define OFF					(0)
#define ON					(1)
#define NSEC_PER_MICROSEC	(1000)	
#define PORT				(8000)


struct data
{
	char info[25];
	uint32_t length;
	uint8_t LED;
}rs, rec;

char *arr[ELEMENTS] = {"Hello","World","Colorado","Boulder","University of Colorado","Spider Man","Captain Marvel","Bye World"};
pid_t process_pid;
pthread_mutex_t mute;
struct timespec timestamp;
char *x;
int soc, new_soc, client_len;

//Function Declarations
void random_generator();
void sig_init();
void signal_handler(int signo, siginfo_t *info, void *extra);


int main(int argc, char *argv[])
{		
	printf("Welcome\n");
	if(ELEMENTS <= 0)
	{
		printf("ERROR : Number of Elements in the array cannot be zero");
		exit(EXIT_FAILURE);
	}
	
	sig_init();
	srand(time(0));
	x = argv[1];	

	char *filename = argv[1];
	char *server_host = "localhost";
	struct sockaddr_in server_add;
	struct hostent *host;
	int len, port;
	port = PORT;

	if(pthread_mutex_init(&mute,NULL))
	{
		perror("ERROR: pthread_mutex_init(), mutex not initialized");
		exit(EXIT_FAILURE);
	}
		
	
	//Get PID 	
	pid_t parent_pid = getpid();
	pthread_mutex_lock(&mute);
	FILE *datafile1 = fopen(argv[1], "a");
	if (datafile1 == NULL)
	{
		perror("ERROR : fopen(), File not created.\n");
		exit(EXIT_FAILURE);
	}
	fprintf(datafile1,"\nPID Number = %d\n", parent_pid);
	fprintf(datafile1, "IPC Method used is Sockets.\n");
	fclose(datafile1);
	pthread_mutex_unlock(&mute);


	//Get address of host
	host = gethostbyname(server_host);
	if(host ==NULL)
	{
		perror("ERROR : gethostbyname().\n");
		exit(EXIT_FAILURE);
	}
	
	if(host->h_addrtype != AF_INET)
	{
		perror("Unknown address type.\n");
		exit(EXIT_FAILURE);
	}
	
	bzero((char *)&server_add,sizeof(server_add));
	server_add.sin_family = AF_INET;
	server_add.sin_addr.s_addr = ((struct in_addr *)host->h_addr_list[0])->s_addr;
	server_add.sin_port = htons(port);

	
	//Open a Socket
	if ((soc = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("ERROR : socket().\n");
		exit(EXIT_FAILURE);
	}
	
		
	if((connect(soc, (struct sockaddr *)&server_add, sizeof(server_add)))<0)
	{
		perror("ERROR : Connect().\n");
		exit(EXIT_FAILURE);
	}
	
	//Write data
	pthread_mutex_lock(&mute);
	datafile1 = fopen(argv[1], "a");
	if (datafile1 == NULL)
	{
		perror("ERROR : fopen(), File not created.\n");
		exit(EXIT_FAILURE);
	}
	
	for (int i =0; i <(ELEMENTS+2); i++)
	{
		random_generator();
		
		if ((write(soc, &rs,sizeof(struct data))) < 0)
		{
			perror("ERROR : write().\n");
			exit(EXIT_FAILURE);
		}
		clock_gettime(CLOCK_REALTIME,&timestamp);
		fprintf(datafile1,"\nTimestamp =  %ld seconds and %ld microseconds.\n", timestamp.tv_sec, timestamp.tv_nsec/NSEC_PER_MICROSEC);
		fprintf(datafile1,"Sending:\n");
		fprintf(datafile1,"STRING = %s \t LENGTH = %d \t LED = %d.\n", rs.info, rs.length, rs.LED);
	}
	
	fclose(datafile1);
	pthread_mutex_unlock(&mute);

	
	//Reading data
	pthread_mutex_lock(&mute);
	datafile1 = fopen(argv[1], "a");
	if (datafile1 == NULL)
	{
		perror("ERROR : fopen(), File not created.\n");
		exit(EXIT_FAILURE);
	}
	
	for (int k =0; k <(ELEMENTS+2); k++)
	{
		len = read(soc, &rec, sizeof(struct data));
		if(len<0)
		{
			perror("ERROR : read().\n");
			exit(EXIT_FAILURE);
		}
		
		clock_gettime(CLOCK_REALTIME,&timestamp);
		fprintf(datafile1,"\nTimestamp =  %ld seconds and %ld microseconds.\n", timestamp.tv_sec, timestamp.tv_nsec/NSEC_PER_MICROSEC);
		fprintf(datafile1,"Receiving:\n");
		fprintf(datafile1,"STRING = %s \t LENGTH = %d \t LED = %d.\n", rec.info, rec.length, rec.LED);
	}
	
	fclose(datafile1);
	pthread_mutex_unlock(&mute);
	
	//Closing Socket
	if(close(soc))
	{
		perror("ERROR : close().\n");
		exit(EXIT_FAILURE);		
	}
	
	//Destroying Mutexes
	if(pthread_mutex_destroy(&mute))
	{
		perror("ERROR : pthread_mutex_destroy, cannot destroy");
		exit(EXIT_FAILURE);
	}
	
	
	//while(1){}		//To check signal interruption
	
	return 0;
}


void random_generator()
{
	
	uint32_t random = rand()%(ELEMENTS+2) ;
	//printf("Random = %d",random);
	switch(random)
	{
		case 0:
		{	
			strcpy(rs.info, arr[random]);
			rs.length = strlen(rs.info);
			rs.LED = OFF;
			break;
		}
		
		case 1:
		{
			strcpy(rs.info, arr[random]);
			rs.length = strlen(rs.info);
			rs.LED = OFF;
			break;
		}
		
		case 2:
		{
			strcpy(rs.info, arr[random]);
			rs.length = strlen(rs.info);
			rs.LED = OFF;
			break;
		}

		case 3:
		{
			strcpy(rs.info, arr[random]);
			rs.length = strlen(rs.info);
			rs.LED = OFF;
			break;
		}

		case 4:
		{
			strcpy(rs.info, arr[random]);
			rs.length = strlen(rs.info);
			rs.LED = OFF;
			break;
		}

		case 5:
		{
			strcpy(rs.info, arr[random]);
			rs.length = strlen(rs.info);
			rs.LED = OFF;
			break;
		}

		case 6:
		{
			strcpy(rs.info, arr[random]);
			rs.length = strlen(rs.info);
			rs.LED = OFF;
			break;
		}

		case 7:
		{
			strcpy(rs.info, arr[random]);
			rs.length = strlen(rs.info);
			rs.LED = OFF;
			break;
		}

		case 8:
		{
			rs.LED = OFF;
			break;
		}

		case 9:
		{
			rs.LED = ON;
			break;
		}

	}

}

/*
 * Signal Initialization Function.
 */
void sig_init()
{
	struct sigaction send_sig;
	send_sig.sa_flags = SA_SIGINFO;
	send_sig.sa_sigaction = &signal_handler;
	if(sigaction(SIGINT, &send_sig, NULL))
	{
		perror("sigaction()\n");
		exit(EXIT_FAILURE);
	}
}


/*
 * Function called on Signal Interruption.
 * Checks which signal has interrupted the program.
 */
void signal_handler(int signo, siginfo_t *info, void *extra)
{
	
	printf("Exit\n");
	FILE *datafile2;
	pthread_mutex_lock(&mute);
	datafile2 = fopen(x, "a");
	if (datafile2 == NULL)
	{
		perror("ERROR : fopen(), File not created.");
		exit(EXIT_FAILURE);
	}
	fprintf(datafile2,"\nSIGTERM Interrupted.\n");
	fprintf(datafile2,"\nExiting.\n");
	pthread_mutex_unlock(&mute);
	fclose(datafile2);

	//Closing Socket
	if(close(soc))
	{
		perror("ERROR : close().\n");
		exit(EXIT_FAILURE);		
	}

	//Destroying Mutexes
	if(pthread_mutex_destroy(&mute))
	{
		perror("ERROR : pthread_mutex_destroy, cannot destroy");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_FAILURE);

}
