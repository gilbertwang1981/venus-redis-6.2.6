#include "server.h"
#include "venus_db_keepalive_thread.h"
#include "slowlogger_def.h"
#include "slowlogger.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

pthread_t venus_db_ka_slowlog_thread_id;

int start_venus_db_keepalive_thread() {
	int ret = pthread_create(&venus_db_ka_slowlog_thread_id , 0 , run_venus_db_keepalive_thread_event_loop , 0);
	if (ret == -1) {
		return -1;
	}

	return 0;
}

void * run_venus_db_keepalive_thread_event_loop(__attribute((unused)) void * args) {
	while (1) {
		usleep(VENUS_REDIS_MYSQL_KEEPALVE_INTERVAL);

		slowlogQElement elem;
		(void)memset(elem.command , 0x00 , VENUS_SLOWLOG_DB_SQL_LENGTH);
		(void)strcpy(elem.command , VENUS_SLOWLOG_KEEPALIVE_DB_COMMAND_NAME);
		elem.duration = 0;
		elem.id = -1;
		(void)sprintf(elem.peerid , "%s:%d" , server.venus_redis_server_host , server.port);
		elem.time = time(0);
		if (-1 == putq(&elem)) {
			serverLog(LL_WARNING , "keepalive failed, write to message queue failed.");
		} else {
			serverLog(LL_NOTICE , "TX to self [%s - %s:%d] (keepalive) success." , server.venus_redis_cluster_name , server.venus_redis_server_host , server.port);
		}
	}

	return 0;
}


