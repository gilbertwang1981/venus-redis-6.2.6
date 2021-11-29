#include "server.h"
#include "venus_def.h"
#include "venus_monitor.h"
#include "venus_monitor_mysql.h"

#include <unistd.h>

pthread_t monitor_thread_id;

int start_monitor_thread() {
	if (-1 == init_monitor_mysql_connection()) {
		return -1;
	}

	int ret = pthread_create(&monitor_thread_id , 0 , run_monitor_event_loop , 0);
	if (ret == -1) {
		return -1;
	}

	return 0;
}

void * run_monitor_event_loop(__attribute((unused)) void * args) {
	while (1) {
		venus_monitor_elem elem;
		(void)strcpy(elem.cluster_name , server.venus_redis_cluster_name);
		(void)strcpy(elem.host , server.venus_redis_server_host);
		elem.port = server.port;
	
		if (-1 == write_monitor_into_mysql(elem)) {
			serverLog(LL_WARNING , "health checking failed.");
		}
	
		usleep(VENUS_MONITOR_CHECK_INTERVAL);
	}

	return 0;
}



