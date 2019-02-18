/***************************************************/
/*
*File Name: Sorting Module
*Description: Module to print name every x period of
*seconds and the number of times the timer fired.The  
*name and period are taken as parameters.
*The default period is 1 second.  
*Author: Siddhant Jajoo
*Date: 02/16/2019
/***************************************************/


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/sort.h>
#include <linux/slab.h>
#include <linux/time.h>


#define	arr_length	(50)

static int arr_l = 50;
static int nodes1;
static int nodes2;
static long count = 0;		//period in seconds
static char *animal = "none";
static char *arr[50] = {"fish","cat","bat","spider","ant","tortoise","frog","gorilla","dog","rabbit","octopus","platypus","rat","lion","monkey","cheetah","penguin","dolphin","cow","panda","bear","mosquito","snake","raccon","tortoise","cayote","fish","cat","bat","spider","ant","wasp","spider","gorilla","cow","panda","bear","mosquito","snake","monkey","cheetah","penguin","fish","cat","bat","spider","ant","ant","frog","gorilla"};

struct animal_type
{
char *name;		//animal name
uint32_t name_count;	//number of duplicates in the array
struct list_head list;
};

struct animal_type *animal_t1,*one;	//Objects for Linked list 1
struct animal_type *animal_t2,*two;
struct list_head *head1, *head2;
struct animal_type *temp;
struct timeval start = {0,0};
struct timeval stop = {0,0};
struct timeval diff = {0,0};

/*
*Comparator function for Sort.
*/
int compare(const void *p, const void *q)
{
return (strcmp(*(const char**)p,*(const char**)q));
}


static int sort_init(void)
{
int i;
do_gettimeofday(&start);
printk(KERN_ALERT"Starting Sorting Module.\n");

//Sorting and Printing the entire Static array
sort(arr,arr_length,sizeof(char*),compare,NULL);
for (i=0;i<arr_length;i++)
{
printk(KERN_ALERT"%s\n",arr[i]);
}

animal_t1 = kmalloc(sizeof(struct animal_type),GFP_KERNEL);
INIT_LIST_HEAD(&(animal_t1->list));
head1 = &(animal_t1->list);		//Creating reference for the first element in linked list 1


animal_t2 = kmalloc(sizeof(struct animal_type),GFP_KERNEL);
INIT_LIST_HEAD(&(animal_t2->list));
head2 = &(animal_t2->list);		//Creating reference for the first element in linked list 2


/*
*Removing duplicates from the array and creating one sorted linked list with all the elements and one linked list with filtered elements according to the parameters
*/
for(i=0;i<arr_length;i++)
{
one = kmalloc(sizeof(struct animal_type),GFP_KERNEL);
one->name = arr[i];
(one->name_count)=0;
while(i!=(arr_length-1) && !(strcmp(arr[i],arr[i+1])))
{
i++;
(one->name_count)++;
}

if(i==(arr_length-1))
{
if(!(strcmp(arr[i],arr[i-1])));
else
{
one = kmalloc(sizeof(struct animal_type),GFP_KERNEL);
one->name = arr[i];
(one->name_count)=0;
i++;
}
}

(one->name_count)++;
list_add_tail(&(one->list),&(animal_t1->list));

//Filtering elements to add in second linked list based on input parameters
if((!(strcmp(animal,one->name))||!(strcmp(animal,"none"))) && count < one->name_count)
{
two = kmalloc(sizeof(struct animal_type),GFP_KERNEL);
two->name = one->name;
two->name_count = one->name_count;
list_add_tail(&(two->list),&(animal_t2->list));
}
}

//Traversing Sorted Linked List
list_for_each_entry(temp,head1,list)
{
nodes1++;
printk(KERN_ALERT"SORTED LINKED LIST => NAME:%s \t COUNT:%d.\n",temp->name,temp->name_count);
}
printk(KERN_ALERT"Number of Sorted Linked list Nodes: %d.\n",nodes1);

//Traversing Filtered Linked List
list_for_each_entry(temp,head2,list)
{
nodes2++;
printk(KERN_ALERT"FILTERED LINKED LIST => NAME:%s \t COUNT:%d.\n",temp->name,temp->name_count);
}
printk(KERN_ALERT"Number of Filtered Linked list Nodes: %d.\n",nodes2);

if(!nodes2)
{
printk(KERN_ALERT"No Animals Found");
} 
printk(KERN_ALERT"The total memory dynamically allocated is %d bytes.\n",sizeof(struct animal_type)*(nodes1+nodes2));
do_gettimeofday(&stop);
printk(KERN_ALERT"Time required to execute function = %ld seconds and %ld microseconds.\n",(stop.tv_sec-start.tv_sec),(stop.tv_usec-start.tv_usec));
printk(KERN_ALERT"\n");

return 0;
}



static void sort_exit(void)
{
do_gettimeofday(&start);
printk(KERN_ALERT"Exit Sorting Module.\n");
//Freeing dynamically allocated memory
list_for_each_entry(temp,head1,list)
{
kfree(temp);
}
list_for_each_entry(temp,head2,list)
{
kfree(temp);
}
printk(KERN_ALERT"The total memory freed is %d bytes",sizeof(struct animal_type)*(nodes1+nodes2));
do_gettimeofday(&stop);
printk(KERN_ALERT"Time required to execute function = %ld seconds and %ld microseconds.\n",(stop.tv_sec-start.tv_sec),(stop.tv_usec-start.tv_usec));
printk(KERN_ALERT"\n");
}

module_init(sort_init);
module_exit(sort_exit);
module_param(count,long,0);
module_param(animal,charp,0);
module_param_array(arr,charp,&arr_l,0);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siddhant Jajoo");
MODULE_DESCRIPTION("Test Module");
