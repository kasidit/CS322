#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
 
	int n, status;
	int fd[2]; 
	pid_t pid; 
	int x = 0; 

	if(pipe(fd)<0){
		printf("pipe error!"); 
		exit(1); 
	}

	if((pid = fork())<0){
		printf("fork error!"); 
		exit(1); 
	}
	else if(pid > 0){ // parent 
                close(fd[1]); 
		read(fd[0], &x, sizeof(x));
		printf("x = %d\n", x); 

	        waitpid(pid, &status, 0);
	}
	else{ // child
		close(fd[0]); 
		x = 2; 
		write(fd[1], &x, sizeof(x));
	}
}
