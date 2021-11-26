#include "venus_util.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>

int get_local_ip_using_ifaddrs(char *str_ip) {
	struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=0;
	int status = -1;
 
    getifaddrs(&ifAddrStruct);
 
    while (ifAddrStruct != 0) {
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { 
            tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
			if(inet_ntop(AF_INET, tmpAddrPtr, str_ip, INET_ADDRSTRLEN)) {
				status = 0;
				if(strcmp("127.0.0.1",str_ip)) {
					 break;
				}
			}  
        }else if(ifAddrStruct->ifa_addr->sa_family==AF_INET6){
		}
		
        ifAddrStruct=ifAddrStruct->ifa_next;
    }
	return status;
}

void get_local_binded_addr(char * addr) {
	get_local_ip_using_ifaddrs(addr);
}

