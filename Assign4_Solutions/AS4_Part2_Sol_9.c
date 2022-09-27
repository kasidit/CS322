#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PARENT_FLOW 0
#define CHILD1_FLOW 1
#define CHILD2_FLOW 2

int main(int argc, char* argv[]){
 
	int n, status;
	int p1fd[2]; 
	int p2fd[2]; 
	pid_t pid1, pid2; 
	int flow_flag = PARENT_FLOW; 

        char buff[100]; 

	if(pipe(p1fd)<0){
		printf("pipe1 error!"); 
		exit(1); 
	}

	if(pipe(p2fd)<0){
		printf("pipe2 error!"); 
		exit(1); 
	}

	if((pid1 = fork())<0){ // fork first child
		printf("fork1 error!"); 
		exit(1); 
	}
	else if(pid1 > 0){ // parent 

	        if((pid2 = fork())<0){ // fork second child
		    printf("fork2 error!"); 
		    exit(1); 
	        }
		else if(pid2 > 0){ // parent 
			flow_flag = PARENT_FLOW; 
		}
		else{
			flow_flag = CHILD2_FLOW; 
		}
	}
	else{ // child 1
		flow_flag = CHILD1_FLOW; 
	}

	switch(flow_flag){
		case PARENT_FLOW: 
                	close(p1fd[0]); 
                	close(p1fd[1]); 
                	close(p2fd[0]); 
                	close(p2fd[1]); 

			waitpid(pid1, &status, 0);
			waitpid(pid2, &status, 0);
			printf("both children terminates\n"); 
			break; 
		case CHILD1_FLOW: 
                	close(p1fd[0]); 
			write(p1fd[1], "ping", sizeof("ping"));

                	close(p2fd[1]); 
			read(p2fd[0], buff, 100);
			printf("child1 receives %s\n", buff); 

			break;
		case CHILD2_FLOW: 
                	close(p1fd[1]); 
			read(p1fd[0], buff, 100);
			printf("child2 receives %s\n", buff); 

                	close(p2fd[0]); 
			if(strncmp(buff, "ping", sizeof(buff)) == 0){
				write(p2fd[1], "pong", sizeof("pong"));
			}
			else{
				printf("child1 did not send ping.\n"); 
			}
			break;
	}

}
