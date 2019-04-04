
/**
 * @file main.c
 * @author Siddhant Jajoo and Satya Mehta
 * @brief This is a multithreaded C code which records temperature and light values at regular intervals
 * of time using a synchronized I2C and sends it to the logger thread using message queues to log into a
 * text file. A socket thread is also created to send data to a remote host on a socket request.  
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

//#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "main.h"
#include "light.h"
#include "temp.h"
#include "logger.h"
#include "sockets.h"
#include "my_signal.h"
#include "queue.h"
#include "gpio.h"
#include "timer.h"

//Global Variables
pthread_t my_thread[4];
pthread_attr_t my_attributes;
volatile static uint32_t temp_hb_value;
volatile static uint32_t light_hb_value;
volatile static uint32_t logger_hb_value;

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("ERROR: Wrong number of parameters.\n");
		printf("Input first parameter = name of log file; second parameter = log level: 'info' or 'warning' or 'error' or 'debug'.\n");
		exit(EXIT_FAILURE);
	}

	if ((strcmp(argv[1], "info") == 0) || (strcmp(argv[1], "warning") == 0) || (strcmp(argv[1], "error") == 0) || (strcmp(argv[1], "debug") == 0))
	{
		printf("ERROR: Filename not valid ; Choose another filename, cannot choose filename same as log levels.\n");
		exit(EXIT_FAILURE);
	}

	if ((strcmp(argv[2], "info") != 0) && (strcmp(argv[2], "warning") != 0) && (strcmp(argv[2], "error") != 0) && (strcmp(argv[2], "debug") != 0))
	{
		printf("ERROR: Invalid Log Level; Valid Log Levels: 'info' or 'warning' or 'error' or 'debug'.\n");
		exit(EXIT_FAILURE);
	}

	filename = argv[1];
	if (!(strcmp(argv[2], "info")))
	{
		g_ll = INFO;
	}
	else if (!(strcmp(argv[2], "debug")))
	{
		g_ll = DEBUG;
	}
	else if (!(strcmp(argv[2], "warning")))
	{
		g_ll = WARNING;
	}
	else if (!(strcmp(argv[2], "error")))
	{
		g_ll = ERROR;
	}

	//Initializing global variables
	temp_timerflag = 0;
	light_timerflag = 0;
	main_exit = 0;
	socket_flag = 0;
	err_t res;
	uint16_t rcv;

	/*Uncomment to test with random numbers*/
	//srand(time(NULL));

	//Initializing GPIOs LEDs
	gpio_init(LED1);
	gpio_init(LED2);
	gpio_init(LED3);
	gpio_init(LED4);

	//Initializing queues
	res = queue_init();
	if (!res)
	{
		printf("BIST: Queue initialization successful.\n");
		msg_log("BIST: Queue initialization successful.\n", DEBUG, P0);
	}
	else
	{
		gpio_ctrl(GPIO53, GPIO53_V, 1);
	}

	//Initializing Signal handler
	res = sig_init();
	if (!res)
	{
		printf("Signal initialization successful.\n");
		msg_log("Signal initialization successful.\n", DEBUG, P0);
	}

	//Initializing I2C
	res = i2c_init();
	if (!res)
	{
		printf("BIST: I2C initialization successful.\n");
		msg_log("BIST: I2C initialization successful.\n", DEBUG, P0);
		gpio_ctrl(GPIO53, GPIO53_V, 0);
	}
	else
	{
		gpio_ctrl(GPIO53, GPIO53_V, 1);
	}

	/*BIST for temperature sensor*/
	write_thigh(24);
	rcv = read_temp_reg(THIGH_REG);
	if (rcv == 0x180)
	{
		printf("BIST: Temprature Sensor working.\n");
		msg_log("BIST: Temperature Sensor Working\n", DEBUG, P0);
		gpio_ctrl(GPIO53, GPIO53_V, 0);
	}
	else
	{
		gpio_ctrl(GPIO53, GPIO53_V, 1);
	//	exit(EXIT_FAILURE);
	}

	/*BIST for light sensor*/

	if ((res = light_id()) == 0x50)
	{
		printf("BIST: Light Sensor Working.\n");
		msg_log("BIST: Light Sensor Working.\n", DEBUG, P0);
	}
	else
	{
		gpio_ctrl(GPIO53, GPIO53_V, 1);
		//exit(EXIT_FAILURE);
	}

	//Initializing Mutexes
	res = mutex_init();
	if (!res)
	{
		printf("BIST: Mutex Initalization successful.\n");
		msg_log("BIST: Mutex initialization successful.\n", DEBUG, P0);
	}
	else
	{
		gpio_ctrl(GPIO53, GPIO53_V, 1);
	}

	//Deleting previous logfile
	if (remove(filename))
	{
		error_log("ERROR: remove(); cannot delete log file", ERROR_DEBUG, P2);
	}
	else
	{
		gpio_ctrl(GPIO53, GPIO53_V, 1);
	}

	//Creating threads
	res = create_threads(filename);
	if (!res)
	{
		msg_log("BIST: Thread Creation successful.\n", DEBUG, P0);
		gpio_ctrl(GPIO53, GPIO53_V, 0);
	}
	else
	{
		gpio_ctrl(GPIO53, GPIO53_V, 1);
	}

	//Initializing Timer
	timer_init(TIMER_HB);
	msg_log("Reached main while loop.\n", DEBUG, P0);

	while (!main_exit)
	{
		hb_handle(hb_receive());
	}

	msg_log("Exiting Main while loop.\n", DEBUG, P0);
	msg_log("Destroying all initialized elements.\n", DEBUG, P0);

	destroy_all();

	for (int i = 0; i < 4; i++)
	{
		pthread_join(my_thread[i], NULL);
	}

	return OK;
}

/**
 * @brief - This thread records temperature from the sensor at regular intervals of time and sends the data
 * 			to the logger thread using message queues.
 * 			It also send data to the socket thread using message queues.
 * 
 * @param filename - This is the textfile name that is passed to the thread. This is obtained as a 
 * 					command line argument.
 * @return void* 
 */
void *temp_thread(void *filename)
{
	msg_log("Entered Temperature Thread.\n", DEBUG, P0);
	timer_init(TIMER_TEMP);

	/*Uncomment to test with random numbers*/
	//sensor_struct data_send;

	msg_log("Entered Temperature Thread.\n", DEBUG, P0);
	timer_init(TIMER_TEMP);

	uint16_t rcv;
	write_thigh(23);
	write_tlow(22);
	rcv = read_config();

	/*Setting THIGH and TLOW for interrupts*/
	rcv = read_temp_reg(THIGH_REG);
	rcv = read_temp_reg(TLOW_REG);

	while (1)
	{
		usleep(10);
		if (temp_timerflag)
		{
			pthread_mutex_lock(&mutex_b);

			//Insert Mutex lock here
			temp_timerflag = 0;
			queue_send(log_mq, read_temp_data(TEMP_UNIT, TEMP_RCV_ID), INFO_DEBUG, P0);

			/*Uncomment to test with random numbers*/
			// data_send.id = SOCK_RCV_ID;
			// data_send.sensor_data.temp_data.temp_c = rand();
			//queue_send(log_mq, data_send, INFO_DEBUG, P0);

			msg_log("Temperature Timer event handled.\n", DEBUG, P0);
			//Insert mutex Unlock here
			pthread_mutex_unlock(&mutex_b);
			hb_send(TEMP_HB);
		}

		if (socket_flag & TC)
		{

			/*Uncomment to test with random numbers*/
			// queue_send(log_mq, data_send, INFO_DEBUG, P0);
			// queue_send(sock_mq, data_send, INFO_DEBUG, P0);

			queue_send(log_mq, read_temp_data(0, SOCK_TEMP_RCV_ID), INFO_DEBUG, P0);
			queue_send(sock_mq, read_temp_data(0, SOCK_TEMP_RCV_ID), INFO_DEBUG, P0);
			msg_log("Temp in celsius socket request event handled", DEBUG, P0);
			pthread_mutex_lock(&mutex_a);
			socket_flag &= (~TC);
			pthread_mutex_unlock(&mutex_a);
		}
		else if (socket_flag & TK)
		{

			/*Uncomment to test with random numbers*/
			//queue_send(log_mq, data_send, INFO_DEBUG);
			//queue_send(sock_mq, data_send, INFO_DEBUG);

			pthread_mutex_lock(&mutex_a);
			socket_flag &= (~TK);
			pthread_mutex_unlock(&mutex_a);
			queue_send(log_mq, read_temp_data(1, SOCK_TEMP_RCV_ID), INFO_DEBUG, P0);
			queue_send(sock_mq, read_temp_data(1, SOCK_TEMP_RCV_ID), INFO_DEBUG, P0);
			msg_log("Temp in kelvin socket request event handled", DEBUG, P0);
		}
		else if (socket_flag & TF)
		{
			/*Uncomment to test with random numbers*/
			//queue_send(log_mq, data_send, INFO_DEBUG);
			//queue_send(sock_mq, data_send, INFO_DEBUG);

			pthread_mutex_lock(&mutex_a);
			socket_flag &= (~TF);
			pthread_mutex_unlock(&mutex_a);
			queue_send(log_mq, read_temp_data(2, SOCK_TEMP_RCV_ID), INFO_DEBUG, P0);
			queue_send(sock_mq, read_temp_data(2, SOCK_TEMP_RCV_ID), INFO_DEBUG, P0);
			msg_log("Temp in fahrenheit socket request event handled", DEBUG, P0);
		}
	}
}

/**
 * @brief - This thread records the lux value from the sensor at regular intervals of time and sends the 
 * 			data to the logger file using message queues.
 * 
 * @param filename - This is the textfile name that is passed to the thread. This is obtained as a 
 * 					command line argument.
 * @return void* 
 */
void *light_thread(void *filename)
{
	msg_log("Entered Light Thread.\n", DEBUG, P0);
	timer_init(TIMER_LIGHT);

	interrupt();

	/*Uncomment to test with random values*/
	//sensor_struct data_send;

	/*Set higher light interrupts for ADC Channel 0*/
	uint16_t ch0, ch1;
	write_int_th(0x60, 1);
	write_int_ctrl(0x00);
	write_int_ctrl(0x11);
	read_light_reg(INT_CTRL);

	while (1)
	{
		/*Polls for interrupts*/
		// int rcv = gpio_poll();
		// if (rcv == 0)
		// {
		// 	write_command(0x40);
		// }

		usleep(1);

		if (light_timerflag)
		{
			pthread_mutex_lock(&mutex_b);
			//Insert Mutex lock here
			light_timerflag = 0;
			queue_send(log_mq, read_light_data(LIGHT_RCV_ID), INFO_DEBUG, P0);

			/*Uncomment to test with random values*/
			// data_send.id = LIGHT_RCV_ID;
			// data_send.sensor_data.light_data.light = rand() % 10;
			//queue_send(log_mq, data_send, INFO_DEBUG, P0);

			msg_log("Light Timer event handled.\n", DEBUG, P0);
			//Insert mutex Unlock here
			pthread_mutex_unlock(&mutex_b);
			hb_send(LIGHT_HB);
		}
		if (socket_flag & L)
		{

			/*Uncomment to test with random numbers*/
			//queue_send(log_mq, data_send, INFO_DEBUG);
			//queue_send(sock_mq, data_send, INFO_DEBUG);

			pthread_mutex_lock(&mutex_a);
			socket_flag &= (~L);
			pthread_mutex_unlock(&mutex_a);
			queue_send(log_mq, read_light_data(SOCK_LIGHT_RCV_ID), INFO_DEBUG, P0);
			queue_send(sock_mq, read_light_data(SOCK_LIGHT_RCV_ID), INFO_DEBUG, P0);
			msg_log("Light socket request event handled", DEBUG, P0);
		}
		if (socket_flag & STATE)
		{

			/*Uncomment to test with random numbers*/
			//queue_send(log_mq, data_send, INFO_DEBUG);
			//queue_send(sock_mq, data_send, INFO_DEBUG);

			pthread_mutex_lock(&mutex_a);
			socket_flag &= (~STATE);
			pthread_mutex_unlock(&mutex_a);
			queue_send(log_mq, read_light_data(SOCK_LIGHT_RCV_ID), INFO_DEBUG, P0);
			queue_send(sock_mq, read_light_data(SOCK_LIGHT_RCV_ID), INFO_DEBUG, P0);
			msg_log("Light socket request event handled", DEBUG, P0);
		}
	}
}

/**
 * @brief - This thread receives data from all the threads using message queues and prints the data to a 
 * 			textfile.
 * 
 * @param filename - This is the textfile name that is passed to the thread. This is obtained as a 
 * 					command line argument.
 * @return void* 
 */
void *logger_thread(void *filename)
{
	msg_log("Entered Logger Thread.\n", DEBUG, P0);
	while (1)
	{
		usleep(1);
		log_data(queue_receive(log_mq));

		hb_send(LOGGER_HB);
	}
}

/**
 * @brief - This thread initializes the socket and waits for a connection to be established from a
 * 			remote host. On socket connection it sends the requested data to the remote host.
 * 
 * @param filename - This is the textfile name that is passed to the thread. This is obtained as a 
 * 					command line argument.
 * @return void* 
 */
void *sock_thread(void *filename)
{
	msg_log("Entered Socket Thread.\n", DEBUG, P0);
	socket_init();
	while (1)
	{
		usleep(1);
		socket_listen();
		handle_socket_req();

		//pthread_mutex_lock(&mutex_b);
		socket_send(queue_receive(sock_mq));
		//pthread_mutex_unlock(&mutex_b);
	}
}

/**
 * @brief - Create 4 different threads.
 * 
 * @param filename - This is the textfile name that is passed to the thread. This is obtained as a 
 * 					command line argument.
 * @return err_t - Returns error value. (0 for success).
 */
err_t create_threads(char *filename)
{
	if (pthread_create(&my_thread[0],		   // pointer to thread descriptor
					   (void *)&my_attributes, // use default attributes
					   temp_thread,			   // thread function entry point
					   (void *)0))			   // parameters to pass in
	{

		perror("ERROR: pthread_create(); in create_threads function, temp_thread not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		mq_close(sock_mq);
		mq_unlink(SOCK_QUEUE);
		close(i2c_open);
		pthread_mutex_destroy(&mutex_a);
		pthread_mutex_destroy(&mutex_b);
		pthread_mutex_destroy(&mutex_error);
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&my_thread[1],		   // pointer to thread descriptor
					   (void *)&my_attributes, // use default attributes
					   light_thread,		   // thread function entry point
					   (void *)0))			   // parameters to pass in

	{
		perror("ERROR: pthread_create(); in create_threads function, light_thread not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		mq_close(sock_mq);
		mq_unlink(SOCK_QUEUE);
		close(i2c_open);
		pthread_mutex_destroy(&mutex_a);
		pthread_mutex_destroy(&mutex_b);
		pthread_mutex_destroy(&mutex_error);
		pthread_cancel(my_thread[0]);
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&my_thread[2],		   // pointer to thread descriptor
					   (void *)&my_attributes, // use default attributes
					   logger_thread,		   // thread function entry point
					   (void *)filename))	  // parameters to pass in

	{
		perror("ERROR: pthread_create(); in create_threads function, logger_thread not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		mq_close(sock_mq);
		mq_unlink(SOCK_QUEUE);
		close(i2c_open);
		pthread_mutex_destroy(&mutex_a);
		pthread_mutex_destroy(&mutex_b);
		pthread_mutex_destroy(&mutex_error);
		pthread_cancel(my_thread[0]);
		pthread_cancel(my_thread[1]);
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&my_thread[3],		   // pointer to thread descriptor
					   (void *)&my_attributes, // use default attributes
					   sock_thread,			   // thread function entry point
					   (void *)filename))	  // parameters to pass in

	{
		perror("ERROR: pthread_create(); in create_threads function, logger_thread not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		mq_close(sock_mq);
		mq_unlink(SOCK_QUEUE);
		close(i2c_open);
		pthread_mutex_destroy(&mutex_a);
		pthread_mutex_destroy(&mutex_b);
		pthread_mutex_destroy(&mutex_error);
		pthread_cancel(my_thread[0]);
		pthread_cancel(my_thread[1]);
		pthread_cancel(my_thread[2]);
		exit(EXIT_FAILURE);
	}

	return OK;
}

/**
 * @brief - This function initializes I2C.
 * 
 * @return err_t
 */
err_t i2c_init(void)
{
	if ((i2c_open = open(I2C_BUS, O_RDWR)) < 0)
	{
		perror("ERROR: i2c_open(); in i2c_init() function");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		mq_close(sock_mq);
		mq_unlink(SOCK_QUEUE);
		exit(EXIT_FAILURE);
	}
	return OK;
}

/**
 * @brief - This function initializes all the mutexes
 * 
 * @return err_t 
 */
err_t mutex_init(void)
{
	if (pthread_mutex_init(&mutex_a, NULL))
	{
		perror("ERROR: pthread_mutex_init(mutex_a); mutex_a not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		mq_close(sock_mq);
		mq_unlink(SOCK_QUEUE);
		close(i2c_open);
		exit(EXIT_FAILURE);
	}

	if (pthread_mutex_init(&mutex_b, NULL))
	{
		perror("ERROR: pthread_mutex_init(mutex_b); mutex_b not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		mq_close(sock_mq);
		mq_unlink(SOCK_QUEUE);
		close(i2c_open);
		pthread_mutex_destroy(&mutex_a);
		exit(EXIT_FAILURE);
	}

	if (pthread_mutex_init(&mutex_error, NULL))
	{
		perror("ERROR: pthread_mutex_init(mutex_error); mutex_error not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		mq_close(sock_mq);
		mq_unlink(SOCK_QUEUE);
		close(i2c_open);
		pthread_mutex_destroy(&mutex_a);
		pthread_mutex_destroy(&mutex_b);
		exit(EXIT_FAILURE);
	}

	return OK;
}

/**
 * @brief - This function reads the error values i.e errno and stores it in a local structure.
 * 
 * @param error_str - The error string that needs to be printed in the text file.
 * @return sensor_struct 
 */
sensor_struct read_error(char *error_str)
{
	sensor_struct read_data;
	read_data.id = ERROR_RCV_ID;

	if (clock_gettime(CLOCK_REALTIME, &read_data.sensor_data.temp_data.data_time))
	{
		error_log("ERROR: clock_gettime(); in read_temp_data() function", ERROR_DEBUG, P2);
	}

	//Errno is thread safe , no mutex required.
	read_data.sensor_data.error_data.error_value = errno;

	strcpy(read_data.sensor_data.error_data.error_str, error_str);

	return read_data;
}

/**
 * @brief - This function stores the string that needs to be printed in the text file in a structure.
 * 
 * @param msg_str - The string that needs to be printed in the text file.
 * @return sensor_struct 
 */
sensor_struct read_msg(char *msg_str)
{
	sensor_struct read_data;
	read_data.id = MSG_RCV_ID;

	strcpy(read_data.sensor_data.msg_data.msg_str, msg_str);

	return read_data;
}

/**
 * @brief - This function sends the error string to be printed in the textfile to the logger thread via
 * 			message queue and prints it in the textile. 
 * 
 * @param error_str - The error string that needs to be printed in the textfile.
 * @param loglevel - The loglevel of the message. Loglevels : INFO, WARNING, ERROR, DEBUG, INFO_DEBUG, ERROR_DEBUG, INFO_ERROR_DEBUG
 * @param prio - Th priorty of the message. Values can be : PO,P1,P2
 */
void error_log(char *error_str, uint8_t loglevel, uint8_t prio)
{
	queue_send(log_mq, read_error(error_str), loglevel, prio);
}

/**
 * @brief - This function sends the message string to be printed in the textfile to the logger thread via
 * 			message queue and prints it in the textile. 
 * 
 * @param str - The message string that needs to be printed in the textfile.
 * @param loglevel - The loglevel of the message. Loglevels : INFO, WARNING, ERROR, DEBUG, INFO_DEBUG, ERROR_DEBUG, INFO_ERROR_DEBUG
 * @param prio - Th priorty of the message . Values can be : PO,P1,P2
 */
void msg_log(char *str, uint8_t loglevel, uint8_t prio)
{
	queue_send(log_mq, read_msg(str), loglevel, prio);
}

/**
 * @brief - This function sends heartbeat to the main loop.
 * 
 * @param hb_value - This value specifies the heartbeat is from which thread. 
 * The values can be : 	TEMP_HB
 * 						LIGHT_HB
 * 						LOGGER_HB
 * 						SOCKET_HB
 * 						CLEAR_HB
 */
void hb_send(uint8_t hb_value)
{
	ssize_t res;
	res = mq_send(heartbeat_mq, (char *)&hb_value, sizeof(uint8_t), 0);
	if (res == -1)
	{
		error_log("ERROR: mq_send(); in queue_send() function", ERROR_DEBUG, P2);
	}
}

/**
 * @brief - This function reads the value received from the messgae queue.
 * 
 * @return uint8_t - Returns the value signifying from which thread it has received the value.
 */
uint8_t hb_receive(void)
{
	ssize_t res;
	uint8_t hb_rcv;
	res = mq_receive(heartbeat_mq, (char *)&hb_rcv, sizeof(sensor_struct), NULL);
	if (res == -1)
	{
		error_log("ERROR: mq_receive(); in queue_receive() function", ERROR_DEBUG, P2);
	}
	return hb_rcv;
}

/**
 * @brief - This function handles the action after receiving the value from the hb_receive() function.
 * 
 * @param hb_rcv - This value is returned from the hb_receive() function.
 */

void hb_handle(uint8_t hb_rcv)
{
	switch (hb_rcv)
	{
	case TEMP_HB:
	{
		temp_hb_value++;
		msg_log("Temperaure Heartbeat received.\n", DEBUG, P0);
		break;
	}

	case LIGHT_HB:
	{
		light_hb_value++;
		msg_log("Light Heartbeat received.\n", DEBUG, P0);
		break;
	}

	case LOGGER_HB:
	{
		logger_hb_value++;
		//msg_log("Logger Heartbeat received.\n", DEBUG, P0);
		break;
	}

	case CLEAR_HB:
	{
		if (temp_hb_value == 0)
		{
			if (pthread_cancel(my_thread[0]))
			{
				error_log("ERROR: pthread_cancel(0); in hb_handle() function", ERROR_DEBUG, P2);
			}
			else
			{
				msg_log("Stopping temperature thread.\n", DEBUG, P0);
			}

			if (pthread_create(&my_thread[0],		   // pointer to thread descriptor
							   (void *)&my_attributes, // use default attributes
							   temp_thread,			   // thread function entry point
							   (void *)0))			   // parameters to pass in
			{
				error_log("ERROR: pthread_create(); in create_threads function, temp_thread not created", ERROR_DEBUG, P2);
			}
			else
			{
				msg_log("Resetting temperature thread.\n", DEBUG, P0);
			}
		}
		else
		{
			temp_hb_value = 0;
		}

		if (light_hb_value == 0)
		{
			if (pthread_cancel(my_thread[1]))
			{
				error_log("ERROR: pthread_cancel(1); in hb_handle() function", ERROR_DEBUG, P2);
			}
			else
			{
				msg_log("Stopping light thread.\n", DEBUG, P0);
			}

			if (pthread_create(&my_thread[1],		   // pointer to thread descriptor
							   (void *)&my_attributes, // use default attributes
							   light_thread,		   // thread function entry point
							   (void *)0))			   // parameters to pass in
			{
				error_log("ERROR: pthread_create(); in create_threads function, temp_thread not created", ERROR_DEBUG, P2);
			}
			else
			{
				msg_log("Resetting light thread.\n", DEBUG, P0);
			}
		}
		else
		{
			light_hb_value = 0;
		}

		if (logger_hb_value == 0)
		{
			if (pthread_cancel(my_thread[2]))
			{
				error_log("ERROR: pthread_cancel(2); in hb_handle() function", ERROR_DEBUG, P2);
			}
			else
			{
				msg_log("Stopping logger thread.\n", DEBUG, P0);
			}

			if (pthread_create(&my_thread[2],		   // pointer to thread descriptor
							   (void *)&my_attributes, // use default attributes
							   logger_thread,		   // thread function entry point
							   (void *)0))			   // parameters to pass in
			{
				error_log("ERROR: pthread_create(); in create_threads function, temp_thread not created", ERROR_DEBUG, P2);
			}
			else
			{
				msg_log("Resetting logger thread.\n", DEBUG, P0);
			}
		}
		else
		{
			logger_hb_value = 0;
		}

		msg_log("Clearing all heartbeat values.\n", DEBUG, P0);
		break;
	}
	}
}

/**
 * @brief - Closes the i2c file descriptor
 * 
 * @return err_t 
 */
err_t i2c_close(void)
{
	if (close(i2c_open))
	{
		perror("ERROR: close(); in i2c_close");
	}
	return OK;
}

/**
 * @brief - This function destroys all the mutexes created at the start.
 * 
 * @return err_t 
 */
err_t mutex_destroy(void)
{
	if (pthread_mutex_destroy(&mutex_a))
	{
		perror("ERROR: pthread_mutex_destroy(mutex_a); cannot destroy mutex_a");
	}

	if (pthread_mutex_destroy(&mutex_b))
	{
		perror("ERROR: pthread_mutex_destroy(mutex_b); cannot destroy mutex_b");
	}

	if (pthread_mutex_destroy(&mutex_error))
	{
		perror("ERROR: pthread_mutex_destroy(mutex_error); cannot destroy mutex_error");
	}

	return OK;
}

err_t thread_destroy(void)
{
	if (pthread_cancel(my_thread[0]))
	{
		perror("ERROR: pthread_cancel(0); in thread_destroy() function");
	}

	if (pthread_cancel(my_thread[1]))
	{
		perror("ERROR: pthread_cancel(1); in thread_destroy() function");
	}

	if (pthread_cancel(my_thread[2]))
	{
		perror("ERROR: pthread_cancel(2); in thread_destroy() function");
	}

	if (pthread_cancel(my_thread[3]))
	{
		perror("ERROR: pthread_cancel(3); in thread_destroy() function");
	}
}

err_t destroy_all(void)
{
	thread_destroy();
	timer_del();
	mutex_destroy();
	queues_close();
	queues_unlink();
	i2c_close();

	FILE *fptr = fopen(filename, "a");
	fprintf(fptr, "Terminating gracefully due to signal.\n");
	printf("\nTerminating gracefully due to signal\n");
	fclose(fptr);
	return OK;
}