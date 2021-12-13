#include "server.h"
#include "venus_monitor_mysql.h"

#include <mysql.h>
#include <stdlib.h>
#include <errmsg.h>
#include <string.h> 

MYSQL * db_monitor_connection = 0;
int is_monitor_connected = 0;

char * get_monitor_env_value_by_name(char * name) {
	char *env = 0;
	if ((env = getenv(name)) == 0) {
		serverPanic("please set env var such as VENUS_MONITOR_DB_HOST,VENUS_MONITOR_DB_USER_NAME,VENUS_MONITOR_DB_PASS,VENUS_MONITOR_DB_PORT and VENUS_MONITOR_DB_NAME");
	}

	return env;
}

int init_monitor_mysql_connection() {
	if (reconnect_to_monitor_db() == -1) {
		return -1;
	}

	char db_name[VENUS_SLOWLOG_STR_LENGTH] = {0};
	sprintf(db_name , "use %s" , get_monitor_env_value_by_name(VENUS_MONITOR_DB_NAME_ENV_VAR));

	if (mysql_query(db_monitor_connection , db_name)) {
		serverLog(LL_WARNING , "executing sql(use db) failed. errno:%d" , mysql_errno(db_monitor_connection));
		
		close_monitor_mysql_connection();
		
		return -1;
	}

	return 0;

}

int write_monitor_into_mysql(venus_monitor_elem elem) {
	if (strlen(elem.cluster_name) == 0) {
		return 0;
	}

	char sql[VENUS_SLOWLOG_DB_SQL_LENGTH] = {0};
	(void)sprintf(sql , 
		"INSERT INTO venus_redis_cluster_instance(cluster_name , instance_host , instance_port , create_time,role) VALUES (\'%s\',\'%s\',%d,now(),%d) ON DUPLICATE KEY UPDATE update_time=now(),role=%d" , 
		elem.cluster_name , elem.host , elem.port , (server.master == server.current_client)?0:1,(server.master == server.current_client)?0:1);

	if (mysql_query(db_monitor_connection , sql)) {
		serverLog(LL_WARNING , "executing sql(%s) failed. errno:%d" , sql , mysql_errno(db_monitor_connection));
	
		close_monitor_mysql_connection();
		reconnect_to_monitor_db();

		return -1;
	}

	return 0;

}

void close_monitor_mysql_connection() {
	is_monitor_connected = 0;

	if (db_monitor_connection != 0) {		
		(void)mysql_close(db_monitor_connection);
		db_monitor_connection = 0;
	}
}

int reconnect_to_monitor_db() {
	db_monitor_connection = mysql_init(0);
	if (db_monitor_connection == 0) {
		serverLog(LL_WARNING , "init db connection failed. errno:%d" , mysql_errno(db_monitor_connection));
		
		return -1;
	}

	if (mysql_real_connect(db_monitor_connection , get_monitor_env_value_by_name(VENUS_MONITOR_DB_HOST_ENV_VAR) , get_monitor_env_value_by_name(VENUS_MONITOR_DB_USER_NAME_ENV_VAR) , 
          get_monitor_env_value_by_name(VENUS_MONITOR_DB_PASS_ENV_VAR) , 0 , atoi(get_monitor_env_value_by_name(VENUS_MONITOR_DB_PORT_ENV_VAR)) , 0 , 0) == 0) {
        serverLog(LL_WARNING , "connect to db failed. errno:%d" , mysql_errno(db_monitor_connection));

		close_monitor_mysql_connection();
		
    	return -1;
	}

	is_monitor_connected = 1;

	return 0;

}

