#include "server.h"

#include "slowlog_mysql.h"
#include "slowlogger_def.h"

#include <mysql.h>
#include <stdlib.h>
#include <errmsg.h>
#include <string.h> 

MYSQL * db_connection = 0;
int is_connected = 0;

char * get_env_value_by_name(char * name) {
	char *env = 0;
	if ((env = getenv(name)) == 0) {
		serverPanic("please set env var such as VENUS_SLOWLOG_DB_HOST,VENUS_SLOWLOG_DB_USER_NAME,VENUS_SLOWLOG_DB_PASS,VENUS_SLOWLOG_DB_PORT and VENUS_SLOWLOG_DB_NAME");
	}

	return env;
}

int reconnect_to_db() {
	db_connection = mysql_init(0);
	if (db_connection == 0) {
		serverLog(LL_WARNING , "init db connection failed. errno:%d" , mysql_errno(db_connection));
		
		return -1;
	}

	if (mysql_real_connect(db_connection , get_env_value_by_name(VENUS_SLOWLOG_DB_HOST_ENV_VAR) , get_env_value_by_name(VENUS_SLOWLOG_DB_USER_NAME_ENV_VAR) , 
          get_env_value_by_name(VENUS_SLOWLOG_DB_PASS_ENV_VAR) , 0 , atoi(get_env_value_by_name(VENUS_SLOWLOG_DB_PORT_ENV_VAR)) , 0 , 0) == 0) {
        serverLog(LL_WARNING , "connect to db failed. errno:%d" , mysql_errno(db_connection));
		
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
		serverLog(LL_WARNING , "executing sql failed. errno:%d" , err);
		if (err == 2013 || (err >= 1158 && err <= 1161)) {
			close_mysql_connection();
		}
		
		return -1;
	}

	return 0;
}

int get_slowlog_records(char * slowlogs , long int offset) {
	char sql[VENUS_SLOWLOG_DB_SQL_LENGTH] = {0};
	(void)sprintf(sql , "select command , duration from venus_redis_slowlog where id >= %ld limit 50" , offset);
	if (mysql_query(db_connection , sql)) {
		int err = mysql_errno(db_connection);
		serverLog(LL_WARNING , "executing sql failed. errno:%d" , err);
		if (err == 2013 || (err >= 1158 && err <= 1161)) {
			close_mysql_connection();
			reconnect_to_db();
		}

		return -1;	
	}

	MYSQL_RES * result = mysql_store_result(db_connection);
	if (result) {
		int num_fields = mysql_num_fields(result);
		MYSQL_ROW row = 0;
		int offset = 0;
		while ((row = mysql_fetch_row(result)) != 0) {
			int i = 0;
			for(i = 0;i < num_fields;i++) {
				offset += sprintf(slowlogs + offset , "%s " , row[i]); 
			}
			
			offset += sprintf(slowlogs + offset , "\n");
		}

		mysql_free_result(result);
	}
	
	return 0;
}

int is_enable_db_persistence() {
	char *env = 0;
	if ((env = getenv(VENUS_REDIS_ENABLE_PERSISTENCE_ENV_VAR_NAME)) == 0) {
		return 1;
	}

	return 0;
}

int write_slowlog_into_mysql(slowlogQElement elem) {
	if (is_enable_db_persistence() == 0) {
		return 0;
	}

	char * ptr = strtok(elem.peerid , ":");
	int port = -1;
	char ip[VENUS_REDIS_COMMON_STR_LENGTH] = {0};
	if (ptr) {
		(void)strcpy(ip , ptr);
		ptr = strtok(0 , ":");
		if (ptr) {
			port = atoi(ptr);
		}
	}

	char sql[VENUS_SLOWLOG_DB_SQL_LENGTH] = {0};
	(void)sprintf(sql , 
		"insert into venus_redis_slowlog(entry_id , duration , time , ip , port , command , create_time , redis_host , redis_port , redis_run_id , redis_cluster_name , msg_queue_id) values (%ld,%ld,%ld,\'%s\',%d,\'%s\',now(),\'%s\',%d,\'%s\',\'%s\',%d)" , 
		elem.id , elem.duration , elem.time , ip , port , elem.command , server.venus_redis_server_host , server.port , server.runid , server.venus_redis_cluster_name , server.slowlog_message_queue_id);

	if (mysql_query(db_connection , sql)) {
		int err = mysql_errno(db_connection);
		serverLog(LL_WARNING , "executing sql failed. errno:%d" , err);
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



