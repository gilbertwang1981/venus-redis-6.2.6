#ifndef __SLOWLOGGER_H__
#define __SLOWLOGGER_H__

#include <sys/types.h>
#include <sys/ipc.h>

#include "slowlogger_def.h"

typedef struct tag_venus_msg_buffer {
	long int mtype;
	char mtext[VENUS_REDIS_MSGQ_SLOWLOG_CONTENT_LENGTH];
} venus_msg_buf;

typedef struct slowlogQElement {
    long id;       /* Unique entry identifier. */
    long duration; /* Time spent by the query, in microseconds. */
    long time;        /* Unix time at which the query was executed. */
    char peerid[VENUS_REDIS_MSGQ_SLOWLOG_ELEM_PID_LENGTH];         /* Client network address. */
	char command[VENUS_SLOWLOG_DB_SQL_LENGTH];
}slowlogQElement;

key_t create_key(void);
int createq(void);
int removeq(void);

int putq(slowlogQElement * element);
int getq(slowlogQElement * element);

#endif

