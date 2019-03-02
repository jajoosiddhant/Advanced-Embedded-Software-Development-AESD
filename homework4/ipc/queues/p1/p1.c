
/***************************************************
/*
* File Name: IPC using queues
* Description: Sending data bidirectionally by creating 2 queues.  
* Author: Siddhant Jajoo
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
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>


#define ELEMENTS			(8)
#define OFF					(0)
#define ON					(1)
#define NSEC_PER_MICROSEC	(1000)	


struct data
{
	char info[25];
	uint32_t length;
	uint8_t LED;
}rs;

char *arr[ELEMENTS] = {"Hello","World","Colorado","Boulder","University of Colorado","Spider Man","Captain Marvel","Bye World"};
pid_t process_pid;
pid_t gchild_pid;
pthread_mutex_t mute;
static int flag;
struct timespec timestamp;
struct data rec;
ssize_t res;
mqd_t qid;
char *x;

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

	if(pthread_mutex_init(&mute,NULL))
	{
		perror("ERROR: pthread_mutex_init(), mutex not initialized");
		exit(EXIT_FAILURE);
	}
	
	char *filename = argv[1];
	struct mq_attr attr;
	
	attr.mq_flags=0;
	attr.mq_maxmsg=10;
	attr.mq_msgsize=100;
	attr.mq_curmsgs=0;
	
	pid_t parent_pid = getpid();
	
	
	pthread_mutex_lock(&mute);
	FILE *datafile1 = fopen(argv[1], "a");
	if (datafile1 == NULL)
	{
		perror("ERROR : fopen(), File not created.\n");
		exit(EXIT_FAILURE);
	}
	fprintf(datafile1,"\nPID Number = %d\n", parent_pid);
	fclose(datafile1);
	pthread_mutex_unlock(&mute);

	
	qid = mq_open("/ipc1", O_RDWR | O_CREAT, 0644, &attr);
	
	if(qid== -1)
	{
		perror("ERROR : mq_open()");
		exit(EXIT_FAILURE);
	}


	//Reading queues
	for (int i = 0; i<(ELEMENTS +2); i++)
	{
		
		mq_receive(qid,(char *)&rec,100,NULL);
		if (res == -1)
		{
			perror("ERROR : mq_receive.\n");
		}
		
		pthread_mutex_lock(&mute);
		FILE *datafile = fopen(argv[1], "a");
		if (datafile == NULL)
		{
			perror("ERROR : fopen(), File not created.\n");
			exit(EXIT_FAILURE);
		}
		clock_gettime(CLOCK_REALTIME,&timestamp);
		fprintf(datafile,"\nTimestamp =  %ld seconds and %ld microseconds.\n", timestamp.tv_sec, timestamp.tv_nsec/NSEC_PER_MICROSEC);
		fprintf(datafile,"Receiving :\n");
		fprintf(datafile,"STRING = %s \t STRING LENGTH = %d \t LED STATUS = %d.\n", rec.info, rec.length, rec.LED);		
		fclose(datafile);
		pthread_mutex_unlock(&mute);

	}


	//Writing Queues
	
	for (int k = 0; k<(ELEMENTS +2); k++)
	{
		
		random_generator();
		int s = mq_send(qid, (char*)&rs, sizeof(struct data), 0);
		if (s == -1)
		{
		perror("ERROR : mq_send.\n");
		}
		sleep(1);
		pthread_mutex_lock(&mute);
		FILE *datafile = fopen(argv[1], "a");
		if (datafile == NULL)
		{
			perror("ERROR : fopen(), File not created.\n");
			exit(EXIT_FAILURE);
		}
		clock_gettime(CLOCK_REALTIME,&timestamp);
		fprintf(datafile,"\nTimestamp =  %ld seconds and %ld microseconds.\n", timestamp.tv_sec, timestamp.tv_nsec/NSEC_PER_MICROSEC);
		fprintf(datafile,"Sending :\n");
		fprintf(datafile,"STRING = %s \t STRING LENGTH = %d \t LED STATUS = %d.\n", rs.info, rs.length, rs.LED);		
		fclose(datafile);
		pthread_mutex_unlock(&mute);
	
	}
	
	//Close queue
	if(mq_close(qid))
	{
		perror("ERROR : mq_close()");
		exit(EXIT_FAILURE);
	}
	
	//Destroying Mutexes
	if(pthread_mutex_destroy(&mute))
	{
		perror("ERROR : pthread_mutex_destroy, cannot destroy");
		exit(EXIT_FAILURE);
	}
	
	//while(1){}		//Uncomment to Check Signal Interruption
	return 0;
}


void random_generator()
{
	
	uint32_t random = rand()%(ELEMENTS+2) ;
	printf("Random = %d",random);
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
	fprintf(datafile2,"Exiting.\n");
	pthread_mutex_unlock(&mute);
	fclose(datafile2);

	//Unlink Queues	
	if(mq_unlink("/ipc1"))
	{
		perror("ERROR : mq_unlink().\n");
		exit(EXIT_FAILURE);
	}
	
	//Closing Queue
	if(mq_close(qid))
	{
		perror("ERROR : mq_close()");
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
