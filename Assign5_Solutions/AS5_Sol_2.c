#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void * foo(void * arg){
   pthread_t mtid;
   long id = (long) arg; 
   mtid = pthread_self();
   if(id % 2 == 0){
	   pthread_detach(mtid); 
   }
   printf("Hello World from (%ld) %lu\n", id, (unsigned long) mtid); 
   pthread_exit((void *)NULL); 
}

#define NUMTHREADS  50

int main(void){

   pthread_t mtid, ntid[NUMTHREADS];
   void *tret; 
   long i; 
    
   for (i=0; i < NUMTHREADS; i++){
     pthread_create(&ntid[i], NULL, foo, (void *) i);  
   }

   for (i=1; i < NUMTHREADS; i+=2){
     pthread_join(ntid[i], &tret); 
   }
}
