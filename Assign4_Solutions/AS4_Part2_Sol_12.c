#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
 
	int n, status;
	int fd[2]; 
	int fd2[2]; 
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

	if(pipe(fd2)<0){
		printf("pipe error!"); 
		exit(1); 
	}

	if((pid = fork())<0){
		printf("fork error!"); 
		exit(1); 
	}
	else if(pid > 0){ // parent 
                close(fd[0]); 
                close(fd2[1]); 

		while(fgets(line, 100, fp) != NULL){
			n = strlen(line); 
			write(fd[1], line, n);
		}

		close(fd[1]); 
	
		while(read(fd2[0], line, 100)!=0){
			n = strlen(line); 
			//printf("parent receive: %s\n", line);
			write(STDOUT_FILENO, line, n);
		}

/*		
		if(fd2[0] != STDOUT_FILENO){
			if(dup2(fd2[0], STDOUT_FILENO) != STDOUT_FILENO){ 
				printf("dup2 error!"); 
				exit(1); 
			}
			close(fd2[0]); 
		}
*/		
	        waitpid(pid, &status, 0);
	}
	else{ // child
		close(fd[1]); 
		close(fd2[0]); 

		if(fd[0] != STDIN_FILENO){
			if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO){ 
				printf("dup2 error!"); 
				exit(1); 
			}
			close(fd[0]); 
		}

		if(fd2[1] != STDOUT_FILENO){
			/*
			if(dup2(STDOUT_FILENO, fd2[1]) != fd2[1]){ 
				printf("dup2 error!"); 
				exit(1); 
			}
			close(STDOUT_FILENO); 
			*/
			if(dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO){ 
				printf("dup2 error!"); 
				exit(1); 
			}
			close(fd2[1]); 
		}
		
		if(execl("/usr/bin/wc", "wc", "-l", (char *)0) < 0){
			printf("exec error!"); 
			exit(1); 
		}
		exit(1); 
	}

}
