#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
 
	int n, status;
	int fd[2]; 
	pid_t pid; 
        char line[100]; 

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
		read(fd[0], line, 100);
		printf("%s\n", line); 

	        waitpid(pid, &status, 0);
	}
	else{ // child
                close(fd[0]); 
		write(fd[1], "hello from child", sizeof("hello from child"));
	}

}
