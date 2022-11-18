/*
 A simple echo server program 
 Kasidit Chanchio (kasiditchanchio@gmail.com)
*/
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_IP		"127.0.0.1"
#define SERV_PORT 	18800

#define MAXLINE	100

#include <signal.h>

int sockfd;
struct sockaddr_in serv_addr, cli_addr;

int main(int argc, char *argv[]){

        int m, n;
        char line[MAXLINE];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)); 

        for(;;){
          int len = sizeof(cli_addr);
          n = recvfrom(sockfd, line, MAXLINE, 0, (struct sockaddr *)&cli_addr, &len);
          printf("line = %s with n = %d characters\n", line, n);
          fflush(stdout);
        //if(strncmp(line, "ENDOFMESSAGE", sizeof("ENDOFMESSAGE")) ==0)
	//	break;
          n = sendto(sockfd, line, n, 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));

        }
	close(sockfd);

}

