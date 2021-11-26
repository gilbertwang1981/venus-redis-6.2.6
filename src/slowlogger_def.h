#ifndef __SLOWLOGGER_DEF_H__
#define __SLOWLOGGER_DEF_H__

#define VENUS_REDIS_MSGQ_SLOWLOG_NAME "/tmp/venus-redis-msgq-slowlog"
#define VENUS_REDIS_MSGQ_SLOWLOG_ID 0xe2000001

#define VENUS_REDIS_MSGQ_SLOWLOG_CONTENT_LENGTH 1024

#define VENUS_REDIS_MSGQ_SLOWLOG_ELEM_PID_LENGTH 64 

#define VENUS_REDIS_MSGQ_SLOWLOG_FETCH_USLEEP 20

#define VENUS_SLOWLOG_DB_SQL_LENGTH 4096
#define VENUS_SLOWLOG_STR_LENGTH 256

#define VENUS_MAX_MSG_QUEUE_SIZE 1024 * 1024 * 256

#define VENUS_MAX_SLOWLOG_PARAM 10

#define VENUS_REDIS_COMMON_STR_LENGTH 256
#define VENUS_REDIS_SLOWLOG_RESULT_SIZE 1024 *1024 * 5
#define VENUS_REDIS_SERVER_COMMON_ADDR "0.0.0.0"

#define VENUS_SLOWLOG_DB_USER_NAME_ENV_VAR "VENUS_SLOWLOG_DB_USER_NAME"
#define VENUS_SLOWLOG_DB_PASS_ENV_VAR "VENUS_SLOWLOG_DB_PASS"
#define VENUS_SLOWLOG_DB_HOST_ENV_VAR "VENUS_SLOWLOG_DB_HOST"
#define VENUS_SLOWLOG_DB_PORT_ENV_VAR "VENUS_SLOWLOG_DB_PORT"
#define VENUS_SLOWLOG_DB_PASS_ENV_VAR "VENUS_SLOWLOG_DB_PASS"
#define VENUS_SLOWLOG_DB_NAME_ENV_VAR "VENUS_SLOWLOG_DB_NAME"

#endif

