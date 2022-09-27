#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
 
	int n, status;
	int fd[2]; 
	pid_t pid; 
        char line[100]; 
	FILE *fp; 

	if((fp = fopen(argv[1], "r")) == NULL){
		printf("file error!"); 
		exit(1); 
	}	

	if(pipe(fd)<0){
		printf("pipe error!"); 
		exit(1); 
	}

	if((pid = fork())<0){
		printf("fork error!"); 
		exit(1); 
	}
	else if(pid > 0){ // parent 
                close(fd[0]); 

		while(fgets(line, 100, fp) != NULL){
			n = strlen(line); 
			write(fd[1], line, n);
		}

		close(fd[1]); 
	        waitpid(pid, &status, 0);
	}
	else{ // child
		close(fd[1]); 

		if(fd[0] != STDIN_FILENO){
			if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO){ 
				printf("dup2 error!"); 
				exit(1); 
			}
			close(fd[0]); 
		}
		
		if(execl("/usr/bin/wc", "wc", "-l", (char *)0) < 0){
			printf("exec error!"); 
			exit(1); 
		}
		exit(1); 
	}

}
