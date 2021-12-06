#ifndef __VENUS_DB_KEEPALIVE_THREAD_H__
#define __VENUS_DB_KEEPALIVE_THREAD_H__

int start_venus_db_keepalive_thread(void);

void * run_venus_db_keepalive_thread_event_loop(void * args);

#endif

