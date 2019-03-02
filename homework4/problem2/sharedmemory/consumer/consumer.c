
/***************************************************
/*
* File Name: IPC using shared memory
* Description: Sending data bidirectionally using shared memory.  
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
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define ELEMENTS			(8)
#define OFF					(0)
#define ON					(1)
#define NSEC_PER_MICROSEC	(1000)	


struct data
{
	char info[25];
	uint32_t length;
	uint8_t LED;
};
struct data rs, rec;
void *shared_mem;


char *arr[ELEMENTS] = {"Hello","World","Colorado","Boulder","University of Colorado","Spider Man","Captain Marvel","Bye World"};
const char *name = "/memory";
const char *semname1 = "/sem1";
const char *semname2 = "/sem2";

pid_t process_pid;
sem_t *sem1;
sem_t *sem2;
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
	 
	sig_init();
	srand(time(0));
	process_pid = getpid();
	x=argv[1];
		
		
	struct timespec timestamp;
	uint32_t shared_size;
		
	//file descriptor
	int shm_consumer;
	
	sem1 = sem_open(semname1, O_EXCL, 0777, 0);
	if(sem1 == NULL)
	{
		perror("ERROR : Semaphore not created.\n");
		exit(EXIT_FAILURE);
	}
	
	sem2 = sem_open(semname2, O_EXCL, 0777, 0);
	if(sem2 == NULL)
	{
		perror("ERROR : Semaphore not created.\n");
		exit(EXIT_FAILURE);
	}
	
	//Open shared memory object
	shm_consumer = shm_open(name,O_CREAT | O_RDWR, 0666);
	if(shm_consumer<0)
	{
		perror("ERROR: shm_open");
		exit(EXIT_FAILURE);
	}
	
		
	shared_size = sizeof(struct data) * (ELEMENTS+2);
	
	
	//Truncate
	ftruncate(shm_consumer, shared_size);
	
	shared_mem = mmap(0, shared_size, PROT_WRITE, MAP_SHARED, shm_consumer, 0);
	if(shared_mem<0)
	{
		perror("ERROR : mmap().\n");
		exit(EXIT_FAILURE);
	}



	//Read Data
	sem_wait(sem1);
	
	FILE *datafile = fopen(argv[1], "a");
	if (datafile == NULL)
	{
		perror("ERROR : fopen(), File not created");
		exit(EXIT_FAILURE);
	}	
	fprintf(datafile,"\nPID Number = %d\n", process_pid);
	fprintf(datafile,"IPC Method used is Shared Memory.\n");
	for (int i = 0; i<(ELEMENTS+2); i++)
	{	
		memcpy((void *)&rec, shared_mem, sizeof(struct data));
		shared_mem += sizeof(struct data);
		printf("%s\n", rec.info);
		printf("%d\n", rec.length);
		printf("%d\n", rec.LED);
		clock_gettime(CLOCK_REALTIME,&timestamp);
		fprintf(datafile,"\nTimestamp =  %ld seconds and %ld microseconds.\n", timestamp.tv_sec, timestamp.tv_nsec/NSEC_PER_MICROSEC);
		fprintf(datafile,"Receiving:\n");
		fprintf(datafile,"STRING = %s \t STRING LENGTH = %d \t LED STATUS = %d.\n", rec.info, rec.length, rec.LED);		
		
	}
	fclose(datafile);

	
	//To get the starting memory address of shared memory
	shared_mem = mmap(0, shared_size, PROT_WRITE, MAP_SHARED, shm_consumer, 0);
	if(shared_mem<0)
	{
		perror("ERROR : mmap().\n");
		exit(EXIT_FAILURE);
	}
	
	//Write Data to shared Memory
	
	datafile = fopen(argv[1], "a");
	if (datafile == NULL)
	{
		perror("ERROR : fopen(), File not created");
		exit(EXIT_FAILURE);
	}	
	fprintf(datafile,"\nPID Number = %d\n", process_pid);
	fprintf(datafile,"IPC Method used is Shared Memory.\n");
	for (int k = 0; k<(ELEMENTS+2); k++)
	{	
		random_generator();
		
		memcpy((struct data *)shared_mem, &rs, sizeof(struct data));
		shared_mem += sizeof(struct data);
		
		printf("%s\n", rs.info);
		printf("%d\n", rs.length);
		printf("%d\n", rs.LED);
		clock_gettime(CLOCK_REALTIME,&timestamp);
		fprintf(datafile,"\nTimestamp =  %ld seconds and %ld microseconds.\n", timestamp.tv_sec, timestamp.tv_nsec/NSEC_PER_MICROSEC);
		fprintf(datafile,"Sending:\n");
		fprintf(datafile,"STRING = %s \t STRING LENGTH = %d \t LED STATUS = %d.\n", rs.info, rs.length, rs.LED);		
		
	}
	fclose(datafile);
	
	sem_post(sem2);
	
	//Unlink SHM
	shm_unlink(name);
	
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
	datafile2 = fopen(x, "a");
	if (datafile2 == NULL)
	{
		perror("ERROR : fopen(), File not created.");
		exit(EXIT_FAILURE);
	}
	fprintf(datafile2,"\nSIGTERM Interrupted.\n");
	fprintf(datafile2,"\nExiting.\n");
	fclose(datafile2);
	
	//Unlink SHM
	shm_unlink(name);
	
	//Unlink Semaphore 1
	if(sem_unlink(semname1))
	{
		perror("ERROR : Cannot Unlink Semaphore 1,\n");
		exit(EXIT_FAILURE);
	}
	
	//Unlink Semaphore 2
	if(sem_unlink(semname2))
	{
		perror("ERROR : Cannot Unlink Semaphore 2,\n");
		exit(EXIT_FAILURE);
	}
	
	//Destroying Semaphore 1
	if(sem_destroy(sem1))
	{
		perror("Cannot Destroy Semaphore 1.\n");
		exit(EXIT_FAILURE);
	}

	//Destroying Semaphore 2
	if(sem_destroy(sem2))
	{
		perror("Cannot Destroy Semaphore 2.\n");
		exit(EXIT_FAILURE);
	}

	
	exit(EXIT_FAILURE);

}

