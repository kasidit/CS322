#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler(int n){
	printf("I got SIGCHLD %d\n", n);
	fflush(stdout);
}
int main(int argc, char *argv[]){
	int status; 
	pid_t pid; 

	signal(SIGCHLD, handler);
	if((pid = fork()) < 0){
		printf("cannot fork.. exit\n"); 
		abort(); 
	}
	else if(pid == 0){ 
		sleep(3); 
		exit(0);  // NORMAL EXIT
	}
	if(waitpid(pid, &status, 0) > 0){
		if(WIFEXITED(status)){
			printf("pid(%ld) exited with status %d\n", 
					(long)pid, WEXITSTATUS(status));
		}
	}
}
