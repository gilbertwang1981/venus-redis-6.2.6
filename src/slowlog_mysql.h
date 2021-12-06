#ifndef __SLOWLOG_MYSQL_H__
#define __SLOWLOG_MYSQL_H__

#include "slowlogger.h"

int init_mysql_connection(void);

int write_slowlog_into_mysql(slowlogQElement elem);

int get_slowlog_records(char * slowlogs , long int offset);

void close_mysql_connection(void);

int reconnect_to_db(void);

int is_enable_db_persistence(void);

int start_keepalive_slowlog_thread(void); 

void * run_keepalive_slowlog_event_loop(void *args);

int keep_alive(void);

#endif

