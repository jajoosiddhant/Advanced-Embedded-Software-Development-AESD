
#include "queue.h"

int queue_init(void)
{
	struct mq_attr attr;

	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(sensor_struct); //Change afterwards
	attr.mq_curmsgs = 0;

	heartbeat_mq = mq_open(HEARTBEAT_QUEUE, O_RDWR | O_CREAT, 0644, &attr);
	if (heartbeat_mq == -1)
	{
		return -1;
	}

	log_mq = mq_open(LOG_QUEUE, O_RDWR | O_CREAT, 0644, &attr);
	if (log_mq == -1)
	{
		return -1;
	}

	sock_mq = mq_open(SOCK_QUEUE, O_RDWR | O_CREAT, 0644, &attr);
	if (sock_mq == -1)
	{
		return -1;
	}

	log_sock_mq = mq_open(LOG_SOCK_QUEUE, O_RDWR | O_CREAT, 0644, &attr);
	if (log_sock_mq == -1)
	{
		return -1;
	}
	return 0;
}

err_t queues_close(void)
{
	if (mq_close(heartbeat_mq))
	{
		perror("ERROR");
	}
	if (mq_close(log_mq))
	{
		perror("ERROR");
	}
	if (mq_close(sock_mq))
	{
		perror("ERROR");
	}
	return OK;
}

/**
 * @brief - This function unlinks all the message queues.
 * 
 * @return err_t 
 */
err_t queues_unlink(void)
{
	if (mq_unlink(HEARTBEAT_QUEUE))
	{
		perror("ERROR");
	}

	if (mq_unlink(LOG_QUEUE))
	{
		perror("ERROR");
	}

	if (mq_unlink(SOCK_QUEUE))
	{
		perror("ERROR");
	}
}
