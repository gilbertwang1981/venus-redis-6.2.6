#include "slowlog_thread.h"
#include "slowlogger.h"
#include "slowlog_mysql.h"

#include <unistd.h>

pthread_t slowlog_thread_id;

int start_slowlog_thread() {
	slowlog_thread_id = pthread_create(&slowlog_thread_id , 0 , run_slowlog_event_loop , 0);
	if (slowlog_thread_id == -1) {
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
			if (-1 == write_slowlog_into_mysql(element)) {
				// TODO 
			}
		}
	}
}


