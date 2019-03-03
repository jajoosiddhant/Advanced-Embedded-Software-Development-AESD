
/***************************************************
/*
* File Name: IPC using Pipes
* Description: Sending data bidirectionally between a parent process and 
* a child process by creating 2 pipes.*  
* Author: Siddhant Jajoo
* Date: 02/16/2019
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

#define ELEMENTS			(8)
#define OFF					(0)
#define ON					(1)
#define NSEC_PER_MICROSEC	(1000)	

struct data
{
	char *info;
	uint32_t length;
	uint8_t LED;
}rs;

char *arr[ELEMENTS] = {"Hello","World","Colorado","Boulder","University of Colorado","Spider Man","Captain Marvel","Bye World"};
pid_t process_pid;
pid_t gchild_pid;
pthread_mutex_t mute;
char *x;

//Function Declarations
void random_generator();
void sig_init();
void signal_handler(int signo, siginfo_t *info, void *extra);

int main(int argc, char *argv[])
{		
	
	if(ELEMENTS <= 0)
	{
		printf("ERROR : Number of Elements in the array cannot be zero");
		exit(EXIT_FAILURE);
	}
	
	x=argv[1]; 
	sig_init();
	srand(time(0));
	
	if(pthread_mutex_init(&mute,NULL))
	{
		perror("ERROR: pthread_mutex_init(), mutex not initialized");
		exit(EXIT_FAILURE);
	}
	
	char *filename = argv[1];
	rs.LED = OFF;
	printf("led = %d\n", rs.LED);
	int pone[2];	//Two ends of first pipe => pone[0] = read side, pone[1] = write side
	int ptwo[2];	//Two ends of second pipe => ptwo[0] = read side, ptwo[1] = write side
			
	//Creating Pipe One
	if(pipe(pone))
	{
		perror("ERROR : Pipe One Creation Failed.\n");
		exit(EXIT_FAILURE);
	}

	//Creating Pipe Two
	if(pipe(ptwo))
	{
		perror("ERROR : Pipe Two Creation Failed.\n");
		exit(EXIT_FAILURE);
	}
	
	process_pid = fork();

	if(process_pid == -1)
	{
		perror("ERROR : Fork Failed");
		exit(EXIT_FAILURE);
	}
	else if(process_pid > 0)		//Code for Parent Process i.e Process 1 
	{
		
		struct timespec timestamp;
		pid_t parent_pid = getpid();
		
		printf("\nParent PID Number = %d\n", parent_pid);
		printf("Child PID Number = %d.\n", process_pid);
		gchild_pid = process_pid;
		printf("IPC Method used is PIPES.\n");
		
				
		//Close pipe1 read side and pipe2 write side
		close(pone[0]);
		close(ptwo[1]);

		//Creating a new file and writing data to pipe 1
		pthread_mutex_lock(&mute);
		FILE *datafile = fopen(argv[1], "a");
		if (datafile == NULL)
		{
			perror("ERROR : fopen(), File not created");
			exit(EXIT_FAILURE);
		}
		fprintf(datafile, "\nParent PID Number = %d.\n", parent_pid);
		fprintf(datafile, "Writing Data to Pipe 1.\n");
		fprintf(datafile, "IPC Method used is PIPES.\n");
		
		for (int i = 0; i<(ELEMENTS+2);i++)
		{			printf("Before randommmm = %d\n\n", rs.LED);
			random_generator();
			printf("After randommmm = %d\n\n", rs.LED);
			int x = write(pone[1], &rs, sizeof(struct data));
			if(x == -1)
			{
				perror("ERROR : write pone[1]");
				exit(EXIT_FAILURE);
			}
			else if(x < sizeof(struct data))
			{
				printf("All data not written\n");
			}
			
			clock_gettime(CLOCK_REALTIME,&timestamp);
			fprintf(datafile,"\nTimestamp =  %ld seconds and %ld microseconds.\n", timestamp.tv_sec, timestamp.tv_nsec/NSEC_PER_MICROSEC);
			fprintf(datafile,"Sending :\n");
			fprintf(datafile,"STRING = %s \t STRING LENGTH = %d \t LED STATUS = %d.\n", rs.info, rs.length, rs.LED);
			printf("%d", rs.LED);

		}
		fclose(datafile);
		pthread_mutex_unlock(&mute);
		wait(NULL);

	
		//Reading from pipe 2
		//initialization for read
		char read_arr[sizeof(struct data)];
		struct data *ptr = (struct data *)read_arr;
		struct timespec timestamp1;
		
		pthread_mutex_lock(&mute);
		datafile = fopen(argv[1], "a");
		if (datafile == NULL)
		{
			perror("ERROR : fopen(), File not created");
			exit(EXIT_FAILURE);
		}
		
		fprintf(datafile, "\nParent PID Number = %d.\n", parent_pid);
		fprintf(datafile, "Reading Data from Pipe 2.\n");
		fprintf(datafile, "IPC Method used is PIPES.\n");
		
		for (int i =0; i<(ELEMENTS+2);i++)
		{
			int y = read(ptwo[0], read_arr, sizeof(read_arr));
			if(y == -1)
			{
				perror("ERROR : read pone[0]");
				exit(EXIT_FAILURE);
			}
			
			printf("\nTimestamp =  %ld seconds and %ld microseconds.\n", timestamp1.tv_sec, timestamp1.tv_nsec/NSEC_PER_MICROSEC);
			printf("Receiving :\n");
			printf("STRING = %s \t STRING LENGTH = %d \t LED STATUS = %d.\n", ptr->info, ptr->length, ptr->LED);
			
			clock_gettime(CLOCK_REALTIME,&timestamp1);
			fprintf(datafile,"\nTimestamp =  %ld seconds and %ld microseconds\n", timestamp.tv_sec, timestamp.tv_nsec/NSEC_PER_MICROSEC);
			fprintf(datafile,"Receiving :\n");
			fprintf(datafile,"STRING = %s \t STRING LENGTH = %d \t LED STATUS = %d.\n", ptr->info, ptr->length, ptr->LED);
		}
		fclose(datafile);
		pthread_mutex_unlock(&mute);
	}
	
	else if(process_pid == 0)	//Code for Child Process i.e Process 2
	{
		
		//Close pipe1 write side and pipe2 read side
		close(pone[1]);
		close(ptwo[0]);
		
		//initialization for read
		char read_arr[sizeof(struct data)];
		struct data *ptr = (struct data *)read_arr;
		struct timespec timestamp1;
		
		pthread_mutex_lock(&mute);
		FILE *datafile1 = fopen(argv[1], "a");
		if (datafile1 == NULL)
		{
			perror("ERROR : fopen(), File not created.\n");
			exit(EXIT_FAILURE);
		}
	
		gchild_pid = getpid();
		printf("\nChild PID Number = %d\n", gchild_pid);
		printf("Reading from pipe 1\n");
		fprintf(datafile1,"\nChild PID Number = %d.\n",gchild_pid);
		fprintf(datafile1,"Reading from pipe 1.\n");
		fprintf(datafile1, "IPC Method used is PIPES.\n");
		
		for (int i =0; i<(ELEMENTS+2);i++)
		{
			int y = read(pone[0], read_arr, sizeof(read_arr));
			if(y == -1)
			{
				perror("ERROR : read pone[0]");
				exit(EXIT_FAILURE);
			}
		
			printf("\nTimestamp =  %ld seconds and %ld microseconds.\n", timestamp1.tv_sec, timestamp1.tv_nsec/NSEC_PER_MICROSEC);
			printf("Receiving :\n");
			printf("STRING = %s \t STRING LENGTH = %d \t LED STATUS = %d.\n", ptr->info, ptr->length, ptr->LED);
			
			clock_gettime(CLOCK_REALTIME,&timestamp1);
			fprintf(datafile1,"\nTimestamp =  %ld seconds and %ld microseconds.\n", timestamp1.tv_sec, timestamp1.tv_nsec/NSEC_PER_MICROSEC);
			fprintf(datafile1,"Receiving :\n");
			fprintf(datafile1,"STRING = %s \t STRING LENGTH = %d \t LED STATUS = %d.\n", ptr->info, ptr->length, ptr->LED);
		}
		fclose(datafile1);
		pthread_mutex_unlock(&mute);
		
	
		//Writing in PIPE 2 
		//Creating a new file 
		pthread_mutex_lock(&mute);
		datafile1 = fopen(argv[1], "a");
		if (datafile1 == NULL)
		{
			perror("ERROR : fopen(), File not created.");
			exit(EXIT_FAILURE);
		}

		fprintf(datafile1,"\nChild PID Number = %d.\n", gchild_pid);
		fprintf(datafile1,"Writing to pipe 2.\n");
		fprintf(datafile1, "IPC Method used is PIPES.\n");
		
		for (int i = 0; i<(ELEMENTS+2);i++)
		{
			random_generator();
			printf("%d\n\n", rs.LED);
			int x = write(ptwo[1], &rs, sizeof(struct data));
			if(x == -1)
			{
				perror("ERROR : write ptwo[1]");
				exit(EXIT_FAILURE);
			}
			else if(x < sizeof(struct data))
			{
				printf("All data not written.\n");
			}
			
			clock_gettime(CLOCK_REALTIME,&timestamp1);
			//fprintf(datafile1,"\nChild PID Number = %d\n", gchild_pid);
			//fprintf(datafile1,"Writing to pipe 2\n");
			fprintf(datafile1,"\nTimestamp =  %ld seconds and %ld microseconds.\n", timestamp1.tv_sec, timestamp1.tv_nsec/NSEC_PER_MICROSEC);
			fprintf(datafile1,"Sending :\n");
			fprintf(datafile1,"STRING = %s \t STRING LENGTH = %d \t LED STATUS = %d.\n", rs.info, rs.length, rs.LED);
			printf("%d", rs.LED);

		}
		fclose(datafile1);
		pthread_mutex_unlock(&mute);
		wait(NULL);
	}
	
	//Destroying Mutexes
	if(pthread_mutex_destroy(&mute))
	{
		perror("ERROR : pthread_mutex_destroy, cannot destroy");
		exit(EXIT_FAILURE);
	}
	
	//while(1)		//To check signal Interruption
	//{}
	
		
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
			rs.info = arr[random];
			rs.length = strlen(arr[random]);
			rs.LED = OFF;
			break;
		}
		
		case 1:
		{
			rs.info = arr[random];
			rs.length = strlen(arr[random]);
			rs.LED = OFF;
			break;
		}
		
		case 2:
		{
			rs.info = arr[random];
			rs.length = strlen(arr[random]);
			rs.LED = OFF;
			break;
		}

		case 3:
		{
			rs.info = arr[random];
			rs.length = strlen(arr[random]);
			rs.LED = OFF;
			break;
		}

		case 4:
		{
			rs.info = arr[random];
			rs.length = strlen(arr[random]);
			rs.LED = OFF;
			break;
		}

		case 5:
		{
			rs.info = arr[random];
			rs.length = strlen(arr[random]);
			rs.LED = OFF;
			break;
		}

		case 6:
		{
			rs.info = arr[random];
			rs.length = strlen(arr[random]);
			rs.LED = OFF;
			break;
		}

		case 7:
		{
			rs.info = arr[random];
			rs.length = strlen(arr[random]);
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

	//Destroying Mutexes
	if(pthread_mutex_destroy(&mute))
	{
		perror("ERROR : pthread_mutex_destroy, cannot destroy");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_FAILURE);

		
}
