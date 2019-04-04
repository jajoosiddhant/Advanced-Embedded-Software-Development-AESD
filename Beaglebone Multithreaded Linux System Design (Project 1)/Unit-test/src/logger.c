/**
 * @file logger.c
 * @author Siddhant Jajoo
 * @brief This file consists of the logger function which prints the data to the logfile.
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "logger.h"

bool previous_state;

/**
 * @brief - This function logs data to the textfile depending on the id field obtained from the structure sensor_struct
 * 			upon dequeuing the data.
 * 
 * @param data_rcv - This is the local object of the structure sensor_struct. This is obtained from function queue_receive().
  
 */
void log_data(sensor_struct data_rcv)
{
	switch (data_rcv.id)
	{

	case TEMP_RCV_ID:
	{
		FILE *logfile = fopen(filename, "a");
		fprintf(stdout, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.temp_data.data_time.tv_sec, data_rcv.sensor_data.temp_data.data_time.tv_nsec);
		fprintf(stdout, "In logger thread temperature Value: %f %s.\n", data_rcv.sensor_data.temp_data.temp_c, UNIT);
		fprintf(stdout, "\n***********************************\n\n");
		fprintf(logfile, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.temp_data.data_time.tv_sec, data_rcv.sensor_data.temp_data.data_time.tv_nsec);
		fprintf(logfile, "In logger Thread temperature Value: %f %s.\n", data_rcv.sensor_data.temp_data.temp_c, UNIT);
		fprintf(logfile, "\n***********************************\n\n");
		fclose(logfile);
		break;
	}

	case LIGHT_RCV_ID:
	{
		FILE *logfile = fopen(filename, "a");
		fprintf(stdout, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.light_data.data_time.tv_sec, data_rcv.sensor_data.light_data.data_time.tv_nsec);
		fprintf(stdout, "In logger thread Light Value: %f.\n", data_rcv.sensor_data.light_data.light);
		fprintf(stdout, "In logger thread Light State: %s.\n", (data_rcv.sensor_data.light_data.light_state)? "LIGHT":"DARK");
		fprintf(stdout, "\n***********************************\n\n");
		fprintf(logfile, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.light_data.data_time.tv_sec, data_rcv.sensor_data.light_data.data_time.tv_nsec);
		fprintf(logfile, "In logger Thread Light Value: %f.\n", data_rcv.sensor_data.light_data.light);
		fprintf(logfile, "In logger thread Light State: %s.\n", (data_rcv.sensor_data.light_data.light_state)? "LIGHT":"DARK");
		if(previous_state != data_rcv.sensor_data.light_data.light_state)
		{
			fprintf(stdout, "LIGHT STATE CHANGED FROM %s to %s\n", (previous_state)? "'LIGHT'": "'DARK'", (data_rcv.sensor_data.light_data.light_state)? "'LIGHT'":"'DARK'");
			fprintf(logfile, "LIGHT STATE CHANGED FROM %s to %s\n", (previous_state)? "'LIGHT'": "'DARK'", (data_rcv.sensor_data.light_data.light_state)? "'LIGHT'":"'DARK'");
			previous_state = data_rcv.sensor_data.light_data.light_state;
		}
		fprintf(logfile, "\n***********************************\n\n");
		fclose(logfile);
		break;
	}

	case ERROR_RCV_ID:
	{
		FILE *logfile = fopen(filename, "a");
		fprintf(stdout, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.error_data.data_time.tv_sec, data_rcv.sensor_data.error_data.data_time.tv_nsec);
		fprintf(stdout, "%s.\n", data_rcv.sensor_data.error_data.error_str);
		fprintf(stdout, "%s.\n", strerror(data_rcv.sensor_data.error_data.error_value));
		fprintf(stdout, "\n***********************************\n\n");
		fprintf(logfile, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.error_data.data_time.tv_sec, data_rcv.sensor_data.error_data.data_time.tv_nsec);
		fprintf(logfile, "%s.\n", data_rcv.sensor_data.error_data.error_str);
		fprintf(logfile, "%s.\n", strerror(data_rcv.sensor_data.error_data.error_value));
		fprintf(logfile, "\n***********************************\n\n");
		fclose(logfile);
		break;
	}

	case MSG_RCV_ID:
	{
		FILE *logfile = fopen(filename, "a");

		fprintf(stdout, "%s", data_rcv.sensor_data.msg_data.msg_str);

		fprintf(logfile, "%s", data_rcv.sensor_data.msg_data.msg_str);

		fclose(logfile);
		break;
	}
	case SOCK_TEMP_RCV_ID:
	{
		pthread_mutex_lock(&mutex_error);
		FILE *logfile = fopen(filename, "a");
		fprintf(logfile, "SOCKET REQUEST RECEIVED\n");
		fprintf(stdout, "SOCKET REQUEST RECEIVED\n");
		fprintf(stdout, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.temp_data.data_time.tv_sec, data_rcv.sensor_data.temp_data.data_time.tv_nsec);
		fprintf(stdout, "In logger thread temperature Value: %f.\n", data_rcv.sensor_data.temp_data.temp_c);
		fprintf(stdout, "\n***********************************\n\n");
		fprintf(logfile, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.temp_data.data_time.tv_sec, data_rcv.sensor_data.temp_data.data_time.tv_nsec);
		fprintf(logfile, "In logger Thread temperature Value: %f.\n", data_rcv.sensor_data.temp_data.temp_c);
		fprintf(logfile, "\n***********************************\n\n");
		fclose(logfile);
		pthread_mutex_unlock(&mutex_error);
		break;
	}

	case SOCK_LIGHT_RCV_ID:
	{
		FILE *logfile = fopen(filename, "a");
		fprintf(logfile, "SOCKET REQUEST RECEIVED\n");
		fprintf(stdout, "SOCKET REQUEST RECEIVED\n");
		fprintf(stdout, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.light_data.data_time.tv_sec, data_rcv.sensor_data.light_data.data_time.tv_nsec);
		fprintf(stdout, "In logger thread Light Value: %f.\n", data_rcv.sensor_data.light_data.light);
		fprintf(stdout, "\n***********************************\n\n");
		fprintf(logfile, "Timestamp: %lu seconds and %lu nanoseconds.\n", data_rcv.sensor_data.light_data.data_time.tv_sec, data_rcv.sensor_data.light_data.data_time.tv_nsec);
		fprintf(logfile, "In logger Thread Light Value: %f.\n", data_rcv.sensor_data.light_data.light);
		fprintf(logfile, "\n***********************************\n\n");
		fclose(logfile);
		break;
	}
	default:
		break;
	}
}
