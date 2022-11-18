/*
 A simple TCP client program 
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

#define	MAXLINE	100

int sockfd;
struct sockaddr_in serv_addr;

int main(int argc, char *argv[]){

        int n, m;
        char line[MAXLINE];
        char recvline[MAXLINE];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = inet_addr(SERV_IP);

        while (fgets(line, MAXLINE, stdin) != NULL) {

            n = sendto(sockfd, line, strlen(line), 0, 
		(struct sockaddr *) &serv_addr, sizeof(serv_addr));
            printf("send %s with n = %d characters\n", line, n);
            m = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
            printf("receive %s with m = %d characters\n", recvline, m);

            fputs(recvline, stdout);

        }

	close(sockfd);

}

