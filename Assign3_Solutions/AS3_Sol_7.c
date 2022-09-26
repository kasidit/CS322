#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char *argv[]){
	int status, i; int n = 2;
	pid_t newpid; 
	int forkflag = 1;

	for(i=0; i < n; i++){
	  if(forkflag == 1) 
		  newpid = fork();
	  else 
		  break;

	  if(newpid < 0){
		printf("cannot fork.. exit\n"); 
		abort(); 
	  }
	  else if(newpid == 0){ 
	        forkflag = 1;
		printf("at i = %d child pid = %ld\n", i, (long)getpid()); 
		fflush(stdout); 
	  }
	  else{
	        forkflag = 0;
	        printf("at i = %d parent pid = %ld wait for child = %ld\n", i, (long)getpid(), (long)newpid);	
	        fflush(stdout); 
	        if(waitpid(newpid, &status, 0) > 0){
		  if(WIFEXITED(status)){
			printf("pid(%ld) exited with status %d\n", 
				(long)newpid, WEXITSTATUS(status));
		  }
	        }
	  }
	}
}
