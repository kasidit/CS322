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

#include <sys/wait.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_IP		"127.0.0.1"
#define SERV_PORT 	18800

#define MAXLINE	100
#define MAX_CONNECTIONS	100
#define MAX_PROCESSES	50

#include <signal.h>

int lis_fd;
//int pid_array[MAX_PROCESSES];
int pid; 
int conn_fd;
//int global_fd[MAX_CONNECTIONS];
struct sockaddr_in serv_addr;

static void chld_handler(int sig){
  int saved_errno = errno;
  while(waitpid(-1, NULL, WNOHANG) > 0)
    continue;
  printf(" sig = %d\n", sig);
  errno = saved_errno; 
}

void process_request(void *t);
int accept_cr(int fd, struct sockaddr *addr, socklen_t *len);
int write_full(int fd, const void *buf, size_t count);
int read_full(int fd, void *buf, size_t count);

int main(int argc, char *argv[]){

        if(signal(SIGCHLD, chld_handler) == SIG_ERR){
	  printf("sig handler: Error occured\n");
	  exit(1);
        }

	lis_fd = socket(AF_INET, SOCK_STREAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	bind(lis_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)); 

	listen(lis_fd, 5);

        while(1){

	  if((conn_fd = accept_cr(lis_fd, NULL, NULL)) < 0){
		printf("Accept: Error occured\n");
		exit(1);
	  }
       
          if((pid = fork()) == 0){
            close(lis_fd);
            process_request(&conn_fd);
          }
          close(conn_fd);
        }

	close(lis_fd);

}

void process_request(void *t){

    int n, m;
    char line[MAXLINE];

    int param =  *((int *)t); 

    printf(" param = %d tmp_fd = %d\n", param, conn_fd);

    while ((n = read_full(conn_fd, line, MAXLINE)) != 0){
        printf("line = %s with n = %d characters\n", line, n);
        fflush(stdout);
        m = write_full(conn_fd, line, n);
        printf("write line = %s for m = %d characters\n", line, m);
        fflush(stdout);
    }

    close(conn_fd);
}

// -----Basic Communication Utilities-----
int accept_cr(int fd, struct sockaddr *addr, socklen_t *len){
	int ret;
repeat_accept:
        ret = accept(fd, addr, len);
        if (ret < 0) {
            if (errno == EINTR){
                goto repeat_accept;
	    }
	    printf("accept error errno=%d fd=%d\n", errno, fd);
        }
	return ret;
}

int write_full(int fd, const void *buf, size_t count){
    ssize_t ret = 0;
    ssize_t total = 0;

    while (count) {
        ret = write(fd, buf, count);
        if (ret < 0) {
            if (errno == EINTR){
                continue;
	    }
	    printf("write error errno=%d fd=%d\n", errno, fd);
            return ret;
        }
        else if (ret == 0){
            return ret; 
        }

        count -= ret;
        buf += ret;
        total += ret;
    }

    return total;
}

int read_full(int fd, void *buf, size_t count){
    ssize_t ret = 0;
    ssize_t total = 0;

    while (count) {
        ret = read(fd, buf, count);
        if (ret < 0) {
            if (errno == EINTR){ 
	        printf("read INTERRUPTED! %d\n", errno);
                continue;
	    } 
	    printf("read error errno=%d fd=%d\n", errno, fd);
            return ret;
        }
        else if (ret == 0){
            return ret; 
        }

        count -= ret;
        buf += ret;
        total += ret;
    }

    return total;
}
