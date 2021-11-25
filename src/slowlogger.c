#include "server.h"
#include "slowlogger.h"

#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>
#include <errno.h>  
#include <unistd.h>  
#include <string.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int msg_queue_id = 0;

key_t create_key() {
	key_t key = ftok(VENUS_REDIS_MSGQ_SLOWLOG_NAME , VENUS_REDIS_MSGQ_SLOWLOG_ID);
	if (-1 == key) {
		serverLog(LL_WARNING , "create key of the queue failed.");
		
		return -1;
	}

	return key;
}

int createq() {
	key_t key = create_key();
	if (key == -1) {
		serverLog(LL_WARNING , "create key of message queue failed. %s" , strerror(errno));
		
		return -1;
	}
	
	msg_queue_id = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
	if (msg_queue_id == -1) {
		serverLog(LL_WARNING , "create message queue failed, will be retry. %s" , strerror(errno));
		
		if (errno == EEXIST) {
			key = create_key();
			if (key == -1) {
				serverLog(LL_WARNING , "create key of message queue failed. %s" , strerror(errno));
				
				return -1;
			}
			msg_queue_id = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
		}
	}

	struct msqid_ds ds;
	ds.msg_qbytes = VENUS_MAX_MSG_QUEUE_SIZE;
	if (-1 == msgctl(msg_queue_id , IPC_SET , &ds)) {
		serverLog(LL_WARNING , "set msgctl failed. %s" , strerror(errno));
	
		return -1;
	}

	return msg_queue_id;
}

int putq(slowlogQElement * element) {
	if (msg_queue_id == -1) {
		return -1;
	}

	venus_msg_buf msgbuf;
	
	char slowlog[VENUS_REDIS_MSGQ_SLOWLOG_CONTENT_LENGTH] = {0};
	sprintf(slowlog , "%ld|%ld|%ld|%s|%s" , element->id , element->time , element->duration , element->peerid , element->command);

	(void)memset(msgbuf.mtext , 0x00 , VENUS_REDIS_MSGQ_SLOWLOG_CONTENT_LENGTH);
	
	msgbuf.mtype = getpid();
	(void)strcpy(msgbuf.mtext , slowlog);

	int ret = msgsnd(msg_queue_id , &msgbuf, sizeof(msgbuf) , IPC_NOWAIT);
	if (ret == -1) {
		serverLog(LL_WARNING , "slowlog write into message queue failed. %s" , strerror(errno));
	
		return -1;
	}
	
	return 0;
}

int getq(slowlogQElement * element) {
	venus_msg_buf msgbuf;
	(void)memset(msgbuf.mtext , 0x00 , VENUS_REDIS_MSGQ_SLOWLOG_CONTENT_LENGTH);

	int ret = msgrcv(msg_queue_id , &msgbuf, sizeof(msgbuf) , getpid() , IPC_NOWAIT);
	if (ret == -1) {
		if (errno == ENOMSG) {
			return 1;
		}
		
		return -1;
	}

	char * ptr = strtok(msgbuf.mtext , "|");
	if (ptr) {
		element->id = atol(ptr);
		ptr = strtok(0 , "|");
		if (ptr) {
			element->time = atol(ptr);
			ptr = strtok(0 , "|");
			if (ptr) {
				element->duration = atol(ptr);
				ptr = strtok(0 , "|");
				if (ptr) {
					(void)memset(element->peerid , 0x00 , VENUS_REDIS_MSGQ_SLOWLOG_ELEM_PID_LENGTH);
					(void)strcpy(element->peerid , ptr);

					ptr = strtok(0 , "|");
					if (ptr) {
						(void)memset(element->command , 0x00 , VENUS_SLOWLOG_DB_SQL_LENGTH);
						(void)strcpy(element->command , ptr);

						return 0;
					}
				}
			}
		}
	}

	serverLog(LL_WARNING , "slowlog gets from message queue failed. %s" , strerror(errno));

	return -1;
}


