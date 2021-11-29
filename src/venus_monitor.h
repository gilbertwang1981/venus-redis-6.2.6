#ifndef __VENUS_MONITOR_H__
#define __VENUS_MONITOR_H__

#include <pthread.h>

int start_monitor_thread(void);

void * run_monitor_event_loop(void * args);

#endif

