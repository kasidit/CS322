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
#define SERVERNUM 2

int conn_fd[SERVERNUM];
struct sockaddr_in serv_addr;

int main(int argc, char *argv[]){

        int n, m;
        char line[MAXLINE];
	int i; 

	for(i=0; i < SERVERNUM; i++){

	  conn_fd[i] = socket(AF_INET, SOCK_STREAM, 0); 

	  memset(&serv_addr, 0, sizeof(serv_addr));
	  serv_addr.sin_family = AF_INET;
	  serv_addr.sin_port = htons(SERV_PORT+i);
	
	  serv_addr.sin_addr.s_addr = inet_addr(SERV_IP);

          if (connect(conn_fd[i], (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0) { 
            perror("Problem in connecting to the server");
            exit(3);
          }
	}

        while (fgets(line, MAXLINE, stdin) != NULL) {

            for(i=0; i < SERVERNUM; i++){
              n = write(conn_fd[i], line, MAXLINE);
              printf("send %s with n = %d characters to FD  %d \n", line, n, conn_fd[i]);
	    }
            for(i=0; i < SERVERNUM; i++){
              m = read(conn_fd[i], line, MAXLINE);
              printf("receive from conn_fd %d  %s with m = %d characters\n", conn_fd[i], line, m);
              fputs(line, stdout);
	    }
        }

	for(i=0; i < SERVERNUM; i++){
	  close(conn_fd[i]);
	}

}

