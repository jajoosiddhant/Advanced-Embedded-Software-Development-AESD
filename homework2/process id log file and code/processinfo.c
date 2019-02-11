#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#define arr_size	(256)

int32_t arr_length = arr_size;
int32_t *arr;
int32_t *sorted_arr;

int main()
{

clock_t start,end;
double elapsed_time;

pid_t id;
uid_t userid;

time_t ctime;
struct tm *timeinfo;

id = getpid();
printf("\nProcess ID : %d.\n",id);

userid = getuid();
printf("User ID: %d.\n",userid);

time(&ctime);
timeinfo =  localtime(&ctime);

printf("Current time and date: %s.\n", asctime(timeinfo));

printf("Trying to sort the array and in descending order and copying it to the kernel space and then back to user space.\n");
arr = malloc(sizeof(int32_t)*arr_size);
sorted_arr = malloc(sizeof(int32_t)*arr_size);
printf("Dynamically Allocated Arrays and now generating random numbers to store in array.\n");
srand(time(0));
for (int i=0; i<arr_size;i++)
{
arr[i] = rand();
printf("Random number at Position %d is %d.\n", i, arr[i]);
}

start = clock();
syscall(398,arr,arr_length,sorted_arr);
end = clock();

for (int k=0; k<arr_size;k++)
{
printf("Sorted Random number at Position %d is %d.\n", k, sorted_arr[k]);
}


elapsed_time = ((double)(end-start))/CLOCKS_PER_SEC;
printf("Time taken by system call: %f.\n", elapsed_time); 


return 0;
}
