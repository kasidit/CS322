#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void * hello_world(void * arg){
	printf("Hello World!\n"); 
	pthread_exit((void *) NULL);
}

int main(int argc, char *argv[]){
	pthread_t ntid;
	void *tret; 

	pthread_create(&ntid, NULL, hello_world, NULL);  
	pthread_join(ntid, tret);
}
