#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int status, i; 
	pid_t pid[100]; 
	int n = 2;

	for(i=0; i < n; i++){
	  if((pid[i] = fork()) < 0){
		printf("cannot fork.. exit\n"); 
		abort(); 
	  }
	  else if(pid[i] == 0){ 
		printf("child pid = %ld\n", (long)getpid()); 
		fflush(stdout); 
		sleep(3); 
		exit(0);  
	  }
	  printf("parent pid = %ld child = %ld\n", (long)getpid(), (long)pid[i]);	
	  fflush(stdout); 
	}

	for(i=0; i < n; i++){
	  if(waitpid(pid[i], &status, 0) > 0){
		if(WIFEXITED(status)){
			printf("pid[%d](%ld) exited with status %d\n", 
				i, (long)pid[i], WEXITSTATUS(status));
		}
	  }
	}
}
