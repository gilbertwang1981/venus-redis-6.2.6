#include "slowlog_mysql.h"
#include "slowlogger_def.h"

#include <mysql.h>

MYSQL * connection = 0;

int init_mysql_connection() {
	connection = mysql_init(0);
	if (connection == 0) {
		return -1;
	}

	if (mysql_real_connect(connection , "rm-wz9z9bk07qr61w1f7.mysql.rds.aliyuncs.com", "root", 
          "ne2FRwzOQjONzEOmx0oZ4uh9", 0 , 3306 , 0 , 0) == 0) {
    	return -1;
	}

	if (mysql_query(connection , "use abtest")) {
		printf("%s\n" , mysql_error(connection));
		return -1;
	}

	return 0;
}

int write_slowlog_into_mysql(slowlogQElement elem) {
	char sql[VENUS_SLOWLOG_DB_SQL_LENGTH] = {0};
	(void)sprintf(sql , "insert into redis_log(entry_id , duration , time , client_id , command) values (%ld,%ld,%ld,\'%s\',\'%s\')" , 
		elem.id , elem.duration , elem.time , elem.peerid , elem.command);

	if (mysql_query(connection , sql)) {
		printf("%s\n" , mysql_error(connection));
		return -1;
	}

	return 0;
}

void close_mysql_connection() {
	if (connection != 0) {
		(void)mysql_close(connection);
	}
}



