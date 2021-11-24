#include "slowlog_mysql.h"
#include "slowlogger_def.h"

#include <mysql.h>
#include <stdlib.h>
#include <errmsg.h>

MYSQL * db_connection = 0;
int is_connected = 0;

char * get_env_value_by_name(char * name) {
	char *env = 0;
	if ((env = getenv(name)) == 0) {
		exit(-1);
	}

	return env;
}

int reconnect_to_db() {
	db_connection = mysql_init(0);
	if (db_connection == 0) {
		return -1;
	}

	if (mysql_real_connect(db_connection , get_env_value_by_name(VENUS_SLOWLOG_DB_HOST_ENV_VAR) , get_env_value_by_name(VENUS_SLOWLOG_DB_USER_NAME_ENV_VAR) , 
          get_env_value_by_name(VENUS_SLOWLOG_DB_PASS_ENV_VAR) , 0 , atoi(get_env_value_by_name(VENUS_SLOWLOG_DB_PORT_ENV_VAR)) , 0 , 0) == 0) {		
    	return -1;
	}

	is_connected = 1;

	return 0;
}

int init_mysql_connection() {
	if (reconnect_to_db() == -1) {
		return -1;
	}

	char db_name[VENUS_SLOWLOG_STR_LENGTH] = {0};
	sprintf(db_name , "use %s" , get_env_value_by_name(VENUS_SLOWLOG_DB_NAME_ENV_VAR));

	if (mysql_query(db_connection , db_name)) {
		int err = mysql_errno(db_connection);
		if (err == 2013 || (err >= 1158 && err <= 1161)) {
			close_mysql_connection();
		}
		
		return -1;
	}

	return 0;
}

int write_slowlog_into_mysql(slowlogQElement elem) {
	char sql[VENUS_SLOWLOG_DB_SQL_LENGTH] = {0};
	(void)sprintf(sql , "insert into redis_log(entry_id , duration , time , client_id , command) values (%ld,%ld,%ld,\'%s\',\'%s\')" , 
		elem.id , elem.duration , elem.time , elem.peerid , elem.command);

	if (mysql_query(db_connection , sql)) {
		int err = mysql_errno(db_connection);
		if (err == 2013 || (err >= 1158 && err <= 1161)) {
			close_mysql_connection();
			reconnect_to_db();
		}

		return -1;
	}

	return 0;
}

void close_mysql_connection() {
	is_connected = 0;

	if (db_connection != 0) {		
		(void)mysql_close(db_connection);
		db_connection = 0;
	}
}



