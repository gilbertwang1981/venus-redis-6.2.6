#include "server.h"
#include "slowlog_thread.h"
#include "slowlogger.h"
#include "slowlog_mysql.h"
#include "venus_db_keepalive_thread.h"

#include <unistd.h>

pthread_t slowlog_thread_id;

int start_slowlog_thread() {
	int ret = pthread_create(&slowlog_thread_id , 0 , run_slowlog_event_loop , 0);
	if (ret == -1) {
		return -1;
	}

	return 0;
}

void * run_slowlog_event_loop(__attribute((unused)) void * args) {
	while (1) {
		slowlogQElement element;

		int ret = -1;
		if (1 == (ret = getq(&element))) {
			usleep(VENUS_REDIS_MSGQ_SLOWLOG_FETCH_USLEEP);
		} else if (ret == 0) {
			if (strcmp(element.command , VENUS_SLOWLOG_KEEPALIVE_DB_COMMAND_NAME) == 0) {
				if (-1 == keep_alive()) {
					serverLog(LL_WARNING , "keepalive to MYSQL(SLOWLOG) failed.");
				}
			} else if (-1 == write_slowlog_into_mysql(element)) {
				serverLog(LL_WARNING , "write slowlog into db failed.");
			}
		}
	}
}


