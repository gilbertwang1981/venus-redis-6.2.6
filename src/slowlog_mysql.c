#include "slowlog_mysql.h"
#include "slowlogger_def.h"

#include <mysql.h>
#include <stdlib.h>

MYSQL * connection = 0;

char * get_env_value_by_name(char * name) {
	return getenv(name);
}

int init_mysql_connection() {
	connection = mysql_init(0);
	if (connection == 0) {
		return -1;
	}

	if (mysql_real_connect(connection , get_env_value_by_name(VENUS_SLOWLOG_DB_HOST_ENV_VAR) , get_env_value_by_name(VENUS_SLOWLOG_DB_USER_NAME_ENV_VAR) , 
          get_env_value_by_name(VENUS_SLOWLOG_DB_PASS_ENV_VAR) , 0 , atoi(get_env_value_by_name(VENUS_SLOWLOG_DB_PORT_ENV_VAR)) , 0 , 0) == 0) {
    	return -1;
	}

	char db_name[VENUS_SLOWLOG_STR_LENGTH] = {0};
	sprintf(db_name , "use %s" , get_env_value_by_name(VENUS_SLOWLOG_DB_NAME_ENV_VAR));

	if (mysql_query(connection , db_name)) {
		return -1;
	}

	return 0;
}

int write_slowlog_into_mysql(slowlogQElement elem) {
	char sql[VENUS_SLOWLOG_DB_SQL_LENGTH] = {0};
	(void)sprintf(sql , "insert into redis_log(entry_id , duration , time , client_id , command) values (%ld,%ld,%ld,\'%s\',\'%s\')" , 
		elem.id , elem.duration , elem.time , elem.peerid , elem.command);

	if (mysql_query(connection , sql)) {
		return -1;
	}

	return 0;
}

void close_mysql_connection() {
	if (connection != 0) {
		(void)mysql_close(connection);
	}
}



