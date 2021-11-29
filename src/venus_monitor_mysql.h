#ifndef __VENUS_MONITOR_MYSQL_H__
#define __VENUS_MONITOR_MYSQL_H__

#include "venus_def.h"

int init_monitor_mysql_connection(void);

int write_monitor_into_mysql(venus_monitor_elem elem);

void close_monitor_mysql_connection(void);

int reconnect_to_monitor_db(void);

#endif

