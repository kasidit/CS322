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
		int k = 0; 
	        for(;;){  // child run forever (become orphan after parent terminate
		  sleep(1); 
		  printf("child pid = %ld at %d\n", (long)getpid(), k++); 
		  fflush(stdout); 
		}
		exit(0);  
	  }
	  printf("parent pid = %ld child = %ld\n", (long)getpid(), (long)pid[i]);	
	  fflush(stdout); 
	}
	exit(1); 
}
