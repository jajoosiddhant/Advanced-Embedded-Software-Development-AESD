/***************************************************/
/*
*File Name: Timer Module
*Description: Module to print name every x period of
*milliseconds and the number of times the timer fired.The  
*name and period are taken as parameters.
*The default period is 500 milliseconds.  
*Author: Siddhant Jajoo
*Date: 02/16/2019
/***************************************************/


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>


#define sec_to_msec	(1000)


struct timer_list mytimer;
static long period = 500;		//period in milliseconds
static int timer_count;
static char *name;
unsigned long ticks;


/*
*Function to execute when the timer expires
*It prints the name provided as the parameter.
*It starts a new timer after the previous timer expires
*and prints the count of the number of times timer has been fired.
*/
void number_timer(unsigned long data)
{
timer_count++;
printk(KERN_ALERT"Name : %s.\n",name);
printk(KERN_ALERT"Number of times timer fired = %d.\n",timer_count);
printk(KERN_ALERT"\n");
mytimer.expires = jiffies + msecs_to_jiffies(period);
add_timer(&mytimer);

}

static int timer_init(void)
{

init_timer(&mytimer);
mytimer.expires = jiffies + msecs_to_jiffies(period);
mytimer.data = 0;
mytimer.function = number_timer;

printk(KERN_ALERT"Timer Started.\n");
add_timer(&mytimer);

printk(KERN_ALERT"\n");
return 0;
}

static void timer_exit(void)
{
del_timer(&mytimer);
printk(KERN_ALERT"Exit Timer Module.\n");
printk(KERN_ALERT"\n");
}


module_init(timer_init);
module_param(period,long,0);
module_param(name,charp,0);
module_exit(timer_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siddhant Jajoo");
MODULE_DESCRIPTION("Test Module");
