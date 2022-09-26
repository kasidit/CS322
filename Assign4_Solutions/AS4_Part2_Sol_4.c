#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t pid; 

void reaper(int n){
	int status; 

	printf("I got SIGCHLD %d\n", n);
	fflush(stdout);

	if(waitpid(pid, &status, 0) > 0){
		if(WIFEXITED(status)){
			printf("pid(%ld) exited with status %d\n", 
					(long)pid, WEXITSTATUS(status));
		}
	}
	printf("before sleep\n");
	fflush(stdout);
        sleep(10); 
	exit(0);
}

int main(int argc, char *argv[]){

	signal(SIGCHLD, reaper);
	if((pid = fork()) < 0){
		printf("cannot fork.. exit\n"); 
		abort(); 
	}
	else if(pid == 0){ 
		int index = 0; 

		for(;;){
		  sleep(3); 
		  printf("index = %d\n", index++); 
		}
		exit(1); // Actually, we don't need this line.
	}

	while(1); 

}
