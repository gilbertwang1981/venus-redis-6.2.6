#ifndef __SLOWLOG_THREAD_H__
#define __SLOWLOG_THREAD_H__

#include <pthread.h>

int start_slowlog_thread();

void * run_slowlog_event_loop(void * args);

#endif

