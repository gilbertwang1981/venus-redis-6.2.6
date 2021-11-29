#ifndef __VENUS_DEF_H__
#define __VENUS_DEF_H__

#define VENUS_MONITOR_CHECK_INTERVAL 15000000

#define VENUS_MONITOR_COMMON_STR_LEN 256

typedef struct tag_venus_monitor_elem {
	char cluster_name[VENUS_MONITOR_COMMON_STR_LEN];
	char host[VENUS_MONITOR_COMMON_STR_LEN];
	int port;
}venus_monitor_elem;

#define VENUS_MONITOR_DB_USER_NAME_ENV_VAR "VENUS_MONITOR_DB_USER_NAME"
#define VENUS_MONITOR_DB_PASS_ENV_VAR "VENUS_MONITOR_DB_PASS"
#define VENUS_MONITOR_DB_HOST_ENV_VAR "VENUS_MONITOR_DB_HOST"
#define VENUS_MONITOR_DB_PORT_ENV_VAR "VENUS_MONITOR_DB_PORT"
#define VENUS_MONITOR_DB_NAME_ENV_VAR "VENUS_MONITOR_DB_NAME"

#endif
