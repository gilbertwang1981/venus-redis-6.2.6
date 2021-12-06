#include "server.h"
#include "venus_db_keepalive_thread.h"
#include "slowlogger_def.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

pthread_t venus_db_ka_slowlog_thread_id;

int start_venus_db_keepalive_thread() {
	int ret = pthread_create(&venus_db_ka_slowlog_thread_id , 0 , run_venus_db_keepalive_thread_event_loop , 0);
	if (ret == -1) {
		return -1;
	}

	return 0;
}

void * run_venus_db_keepalive_thread_event_loop(__attribute((unused)) void * args) {
	while (1) {
		usleep(VENUS_REDIS_MYSQL_KEEPALVE_INTERVAL);

		int sock = socket(AF_INET , SOCK_STREAM , 0);
		if (-1 == sock) {
			serverLog(LL_WARNING , "create socket failed. %s" , strerror(errno));
		
			return 0;
		}

		struct sockaddr_in sockaddr;
		memset(&sockaddr , 0x00 , sizeof(sockaddr));
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = htons(server.port);
		inet_pton(AF_INET ,  server.venus_redis_server_host , &sockaddr.sin_addr);
		if(connect(sock , (struct sockaddr*)&sockaddr , sizeof(sockaddr)) == -1) {
			serverLog(LL_WARNING , "connect to redis failed. %s" , strerror(errno));
		
			close(sock);

			return 0;
		}

		char command[VENUS_SLOWLOG_STR_LENGTH] = {0};
		sprintf(command , "venus-db-keepalive\r\n");
		if(send(sock , command , strlen(command) , 0) == -1) {
			serverLog(LL_WARNING , "send keepalive to redis failed. %s" , strerror(errno));
		
			close(sock);

			return 0;
		}

		close(sock);

		serverLog(LL_NOTICE , "send keepalive to MYSQL(SLOWLOG).");
	}

	return 0;
}


