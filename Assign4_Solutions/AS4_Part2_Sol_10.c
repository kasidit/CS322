#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PARENT_FLOW 0
#define CHILD_FLOW 1
#define GRANDCHILD_FLOW 2

int main(int argc, char* argv[]){
 
	int n, status;
	int p1fd[2]; 
	pid_t pid; 
	int flow_flag = PARENT_FLOW; 

        char buff[100]; 

	if(pipe(p1fd)<0){
		printf("pipe1 error!"); 
		exit(1); 
	}

	if((pid = fork())<0){ // fork a child
		printf("fork1 error!"); 
		exit(1); 
	}
	else if(pid > 0){ // parent 
		flow_flag = PARENT_FLOW; 
	}
	else{ // child 

	        if((pid = fork())<0){ // fork a grandchild
		    printf("fork2 error!"); 
		    exit(1); 
	        }
		else if(pid > 0){ // the child 
			flow_flag = CHILD_FLOW; 
		}
		else{ // the grandchild
			flow_flag = GRANDCHILD_FLOW; 
		}
	}

	switch(flow_flag){
		case PARENT_FLOW: 
                	close(p1fd[0]); 
			write(p1fd[1], "hello grandchild", sizeof("hello grandchild"));

			waitpid(pid, &status, 0);
			printf("parent %ld detects child(%ld)'s termination\n", (long)getpid(), (long)pid); 
			break; 
		case CHILD_FLOW: 
                	close(p1fd[0]); 
                	close(p1fd[1]); 

			waitpid(pid, &status, 0);
			printf("child %ld detects grandchaild(%ld)'s termination\n", (long)getpid(), (long)pid); 

			break;
		case GRANDCHILD_FLOW: 
                	close(p1fd[1]); 
			read(p1fd[0], buff, 100);
			printf("granchild(%ld) receives %s\n", (long)getpid(), buff); 

			break;
	}

}
