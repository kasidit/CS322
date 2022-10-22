#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int A[8] = {1, 1, 1, 1, 1, 1, 1, 1}; 
int start; // starting array index for each child process
int size = 4; // size of the slice of the array given to a child process
int x[2]; 

int main(int argc, char *argv[]){
	int status, i, k; 
	pid_t pid[100]; 
	int n = 2;

	for(i=0; i < n; i++){
	  if((pid[i] = fork()) < 0){
		printf("cannot fork.. exit\n"); 
		abort(); 
	  }
	  else if(pid[i] == 0){ 

		start = i*size; 
                for(k = start; k < start+size; k++)
	          x[i] += A[i]; 
                printf("child pid = %ld: x%d = %d\n", (long)getpid(), i+1, x[i]); 		
		fflush(stdout); 

		exit(0);  
	  }
	}

	for(i=0; i < n; i++){
	  waitpid(pid[i], &status, 0);
	}
}
