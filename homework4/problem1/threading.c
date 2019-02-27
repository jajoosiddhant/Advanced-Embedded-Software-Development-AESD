
/***************************************************
/*
* File Name: Multithreading
* Description: Implemented Multithreading using 2 threads.
* Thread 1 keeps a count of the alphabetic characters in a text file.
* Thread 2 gets the cpu utilization every 100 ms.
* USR1 and USR2 is used to terminate the threads.
* All the output is redirected to a text file. 
* Author: Siddhant Jajoo
* Date: 02/16/2019
/***************************************************/


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define ARR_LENGTH		(26)		//Araay length to store alphabetes count.
#define FILE_READ		("gdb.txt")	//file name to read from
#define ALPHA_FIRST		(97)		//ascii value of 'a'
#define ALPHA_LAST		(122)		//ascii value of 'z'
#define INTERVAL		(100000000)			//Timer Interval in nanoseconds
#define NSEC_PER_MICROSEC	(1000)	


struct file 
{
	FILE *logfile;
	char *filename;
};

//Global Variables
pid_t parent_pid;
pthread_t parent_thread;
pthread_t letter_thread;
pthread_t timer_thread;
pthread_mutex_t mute;
static timer_t timeout;
struct itimerspec trigger;
struct sigevent sev;
struct timespec timestamp;	
static uint32_t arr[ARR_LENGTH];
static char buffer[300];
static uint8_t flag;
char *command = "cat /proc/stat | grep cpu";


//Function declarations
void *letter(void *threadp);
void *timer(void *threadp);
void timer_handler(union sigval sv);
void sig_init(void);
void signal_handler(int signo, siginfo_t *info, void *extra);
void mask1(void);
void wopen(struct file *);
void ropen(char *filename, struct file *);
void file_close(struct file *);
void mlock(pthread_mutex_t mute);
void munlock(pthread_mutex_t mute);


int main(int argc, char *argv[])
{	
	
	//Get Current Time
	clock_gettime(CLOCK_REALTIME,&timestamp);
	printf("\nTimestamp = %ld microseconds\n", timestamp.tv_nsec/NSEC_PER_MICROSEC);
	
	struct file file;			//Local struct object to pass as an argument
	file.filename = argv[1];
	sig_init();
	
	if(pthread_mutex_init(&mute,NULL))
	{
		perror("ERROR: pthread_mutex_init(), mutex not initialized");
		exit(EXIT_FAILURE);
	}
	
	parent_pid = getpid();
	printf("I am the parent Thread with PID: %d.\n", parent_pid);
	
	//Mutex Lock and Unlock,Opening the Log file and printing in the log file
	//mlock(mute);
	pthread_mutex_lock(&mute);
	wopen(&file);
	fprintf(file.logfile,"\nTimestamp = %ld microseconds\n", timestamp.tv_nsec/NSEC_PER_MICROSEC);
	fprintf(file.logfile,"I am the parent Thread with PID: %d.\n", parent_pid);
	file_close(&file);
	//munlock(mute);
	pthread_mutex_unlock(&mute);
	
	//Creating Threads
	if (pthread_create(&letter_thread, NULL, letter, (void *)&file))
	{	
		perror(" ERROR: Child Thread 1 Creation Failed.\n");
		exit(EXIT_FAILURE);	
	}
	if (pthread_create(&timer_thread, NULL, timer, (void *)&file))
	{
		perror(" ERROR: Child Thread 2 Creation Failed.\n");
		exit(EXIT_FAILURE);		
	}
	
	//Wait for Child threads to join the Main thread.	
	if(pthread_join(letter_thread,NULL))
	{
		perror("ERROR : Child 1, pthread_join.\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(timer_thread,NULL))
		{
			perror("ERROR : Child 2, pthread_join.\n");
			exit(EXIT_FAILURE);
		}
	
	//Deleting Timer	
	if(timer_delete(timeout))
	{
		perror("ERROR : timer_delete, cannot delete");
		exit(EXIT_FAILURE);
	}
	
	
	clock_gettime(CLOCK_REALTIME,&timestamp);
	printf("\nTimestamp = %ld microseconds\n", timestamp.tv_nsec/NSEC_PER_MICROSEC);
	printf("Exiting\n");
	
	
	//mlock(mute);
	pthread_mutex_lock(&mute);	
	wopen(&file);
	fprintf(file.logfile,"\nTimestamp = %ld microseconds\n", timestamp.tv_nsec/NSEC_PER_MICROSEC);
	if (flag == 1)
	{
		fprintf(file.logfile,"Signal USR1 Interrupted\n");

	}
	else if (flag == 2)
	{
		fprintf(file.logfile,"Signal USR2 Interrupted\n");

	}
	fprintf(file.logfile,"Exiting");
	file_close(&file);
	//munlock(mute);
	pthread_mutex_unlock(&mute);

	//Destroying Mutexes
	if(pthread_mutex_destroy(&mute))
	{
		perror("ERROR : pthread_mutex_destroy, cannot destroy");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}

/*
 * Thread 1 Starting Function.
 * This function prints/writes timestamp, pid ,tid, posix thread id to a file.
 * It counts the occurences of all the alphabets from a text file and displays/writes the alphabets
 * whos occurences are less than 100 to a text file. 
 */
void *letter(void *threadp)
{	
	//Get current time
	clock_gettime(CLOCK_REALTIME,&timestamp);
	printf("\nTimestamp = %ld microseconds\n", timestamp.tv_nsec/NSEC_PER_MICROSEC);
	
	mask1();	
	
	uint8_t ascii, i,k;
	char c;
	struct file* file = (struct file*)threadp;
	
	pid_t letter_pid = getpid();
	pid_t letter_tid = syscall(SYS_gettid);
	pthread_t letter_posix_id = pthread_self();
	printf("\nChild Thread 1 has been created with PID: %d \t Linux Thread ID: %d \t POSIX Thread ID: %ld.\n", letter_pid, letter_tid, letter_posix_id);	
	printf("Sorting letters in the text file %s......\n", (*file).filename);
	printf("Printing letters whose count is less than 100:\n");
	
	//mlock(mute);
	pthread_mutex_lock(&mute);	
	ropen(FILE_READ, file);
	while((c=fgetc((*file).logfile))!=EOF)
	{
		ascii = (uint8_t)tolower(c);
		if(ALPHA_FIRST<=ascii<=ALPHA_LAST)
		{
		i = ascii - ALPHA_FIRST;
		arr[i]++;
		}
	}
	file_close(file);
	pthread_mutex_unlock(&mute);
	munlock(mute);

	//mlock(mute);
	pthread_mutex_lock(&mute);	
	wopen(file);
	fprintf((*file).logfile,"\nTimestamp = %ld microseconds\n", timestamp.tv_nsec/NSEC_PER_MICROSEC);
	fprintf((*file).logfile,"\nChild Thread 1 has been created with PID: %d \t Linux Thread ID: %d \t POSIX Thread ID: %ld.\n", letter_pid, letter_tid, letter_posix_id);	
	fprintf((*file).logfile,"Sorting letters in the text file %s......\n", (*file).filename);
	fprintf((*file).logfile,"Printing letters whose count is less than 100:\n");

	for(k=0;k<ARR_LENGTH;k++)
	{
		if (arr[k]<100)
		{
		fprintf((*file).logfile,"=>LETTER = %c \t COUNT = %d\n", k+ALPHA_FIRST, arr[k]);
		printf("=>LETTER = %c \t COUNT = %d\n", k+ALPHA_FIRST, arr[k]);
		}
	}
	fprintf((*file).logfile,"Thread 1 Work.\n");
	file_close(file);
	//munlock(mute);
	pthread_mutex_unlock(&mute);
}

/*
 * Thread 2 Starting Function.
 * This function initializes all the required values and prints/writes to a file 
 * timestamp,pid,tid,posix thread id.
 */
void *timer(void *threadp)
{
	//Get Current Time
	clock_gettime(CLOCK_REALTIME,&timestamp);
	printf("\nTimestamp = %ld microseconds\n", timestamp.tv_nsec/NSEC_PER_MICROSEC);

	struct file* file = (struct file*)threadp;
	
	pid_t timer_pid = getpid();
	pid_t timer_tid = syscall(SYS_gettid);
	pthread_t timer_posix_id = pthread_self();
	printf("\nChild Thread 2 has been created with PID: %d \t Linux Thread ID: %d \t POSIX Thread ID: %ld.\n", timer_pid, timer_tid, timer_posix_id);
	
	//mlock(mute);
	pthread_mutex_lock(&mute);	
	wopen(file);
	fprintf((*file).logfile,"\nTimestamp = %ld microseconds\n", timestamp.tv_nsec/NSEC_PER_MICROSEC);
	fprintf((*file).logfile,"\nChild Thread 2 has been created with PID: %d \t Linux Thread ID: %d \t POSIX Thread ID: %ld.\n", timer_pid, timer_tid, timer_posix_id);	
	file_close(file);
	//munlock(mute);
	pthread_mutex_unlock(&mute);

    //Set all `sev` and `trigger` memory to 0 
    memset(&sev, 0, sizeof(struct sigevent));
    memset(&trigger, 0, sizeof(struct itimerspec));
	
	sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = &timer_handler;
    sev.sigev_value.sival_ptr = file;
	timer_create(CLOCK_REALTIME, &sev, &timeout);
	
	//Setting the first timer interval and the repeating timer interval
	//trigger.it_value.tv_nsec = INTERVAL;
	//trigger.it_interval.tv_nsec = INTERVAL;
	trigger.it_value.tv_sec = 2;
	trigger.it_interval.tv_sec = 2;
	
	timer_settime(timeout, 0, &trigger, NULL);
		
	while(1)
	{
	sleep(6);
	}
}

/*
 * Handler Function on Timer Expiration.
 * This function prints and writes the timestamp,pid,tid,posix thread id,CPU utilization
 * to a file after every timer expiration(100 ms).
 */
void timer_handler(union sigval sv)
{
	//Get Current Time
	clock_gettime(CLOCK_REALTIME,&timestamp);
	printf("\nTimestamp = %ld microseconds\n", timestamp.tv_nsec/NSEC_PER_MICROSEC);

	struct file* file = sv.sival_ptr;
	
	pid_t timer_pid = getpid();
	pid_t timer_tid = syscall(SYS_gettid);
	pthread_t timer_posix_id = pthread_self();
	printf("\nChild Thread 2 with PID: %d \t Linux Thread ID: %d \t POSIX Thread ID: %ld.\n", timer_pid, timer_tid, timer_posix_id);

	//mlock(mute);
	pthread_mutex_lock(&mute);
	(*file).logfile = popen(command,"r");
	if ((*file).logfile == NULL)
	{
		perror("ERROR : write popen().\n");
		exit(EXIT_FAILURE);
	}
	fread(buffer,sizeof(buffer),1,(*file).logfile);
	if(pclose((*file).logfile))
	{
		perror("ERROR : fclose()");
		exit(EXIT_FAILURE);
	}
	pthread_mutex_unlock(&mute);	
	//munlock(mute);
	
	printf("Trying to Display CPU Utilization......\n");
	printf("%s\n", buffer);
	
	//mlock(mute);
	pthread_mutex_lock(&mute);	
	wopen(file);
	fprintf((*file).logfile,"\nTimestamp = %ld microseconds\n", timestamp.tv_nsec/NSEC_PER_MICROSEC);
	fprintf((*file).logfile,"\nChild Thread 2 with PID: %d \t Linux Thread ID: %d \t POSIX Thread ID: %ld.\n", timer_pid, timer_tid, timer_posix_id);
	fprintf((*file).logfile,"Trying to Display CPU Utilization......\n");
	fprintf((*file).logfile,"%s\n", buffer);
	file_close(file);
	pthread_mutex_unlock(&mute);	
	munlock(mute);
}

/*
 * Signal Initialization Function.
 */
void sig_init()
{
	struct sigaction send_sig;
	send_sig.sa_flags = SA_SIGINFO;
	send_sig.sa_sigaction = &signal_handler;
	if(sigaction(SIGUSR1, &send_sig, NULL))
	{
		perror("sigaction()\n");
		exit(EXIT_FAILURE);
	}
	
	if(sigaction(SIGUSR2, &send_sig, NULL))
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
	if(signo == 10)
	{
	printf("Signal USR1 Interrupted\n");
	flag = 1;
	}
	
	if(signo == 12)
	{
	printf("Signal USR2 Interrupted\n");
	flag = 2;
	}
	
	if(pthread_cancel(timer_thread))
	{
		printf("ERROR: pthread_cancel.\n");
		exit(EXIT_FAILURE);
	}

}

/*
 * Masks the signals USR1 and USR2 interrupting the thread calling this function. 
 */
void mask1()
{
	sigset_t mask;
	if (sigemptyset(&mask))
	{
		perror("ERROR : sigemptyset().\n");
		exit(EXIT_FAILURE);
	}
	if(sigaddset(&mask, SIGUSR1))
	{
		perror("ERROR : sigemptyset().\n");
		exit(EXIT_FAILURE);
	}
	if(sigaddset(&mask, SIGUSR2))
	{
		perror("ERROR : sigemptyset().\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_sigmask(SIG_BLOCK,&mask,NULL))
	{
		perror("ERROR : pthread_sigmask().\n");
		exit(EXIT_FAILURE);
	}
}

/*
 * Opens file in Append mode with write only capability and error checking. 
 */
void wopen(struct file *file)
{
	(*file).logfile	= fopen((*file).filename, "a");
	if ((*file).logfile == NULL)
	{
		perror("ERROR : write fopen().\n");
		exit(EXIT_FAILURE);
	}
}

/*
 * Closes the open file with error checking. 
 */
void file_close(struct file *file)
{
	if(fclose((*file).logfile))
	{
		perror("ERROR : fclose()");
		exit(EXIT_FAILURE);
	}
}

/*
 * Opens file with read only capability. 
 */
void ropen(char *filename, struct file *file)
{
	(*file).logfile = fopen(filename, "r");
	if ((*file).logfile == NULL)
	{
		perror("ERROR : read fopen().\n");
		exit(EXIT_FAILURE);
	}
}

/*
 * Locks Mutex with error Checking Capability 
 */
void mlock(pthread_mutex_t mute)
{
	pthread_mutex_lock(&mute);
/*	{
		perror("ERROR : pthread_mutex_lock.\n");
		exit(EXIT_FAILURE);
	}
*/
}

/*
 * Unlocks Mutex with error Checking Capability 
 */
void munlock(pthread_mutex_t mute)
{
	pthread_mutex_unlock(&mute);
/*	{
		perror("ERROR : pthread_mutex_unlock.\n");
		exit(EXIT_FAILURE);
	}
*/
}
