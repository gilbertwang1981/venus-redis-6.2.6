#include "server.h"
#include "venus_def.h"
#include "venus_monitor.h"
#include <unistd.h>

pthread_t monitor_thread_id;

int start_monitor_thread() {
	int ret = pthread_create(&monitor_thread_id , 0 , run_monitor_event_loop , 0);
	if (ret == -1) {
		return -1;
	}

	return 0;
}

void * run_monitor_event_loop(__attribute((unused)) void * args) {
	while (1) {
		usleep(VENUS_MONITOR_CHECK_INTERVAL);
	}

	return 0;
}



