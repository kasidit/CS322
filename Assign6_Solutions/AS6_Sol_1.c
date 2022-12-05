#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void * foo(void * arg){
   pthread_t mtid;
   mtid = pthread_self();
   printf("Hello World from %lu\n", (unsigned long) mtid); 
   pthread_exit((void *)NULL); 
}

#define NUMTHREADS  4

int main(void){

   pthread_t mtid, ntid[NUMTHREADS];
   void *tret; 
   int i; 
    
   for (i=0; i < NUMTHREADS; i++){
     pthread_create(&ntid[i], NULL, foo, NULL);  
   }

   for (i=0; i < NUMTHREADS; i++){
     pthread_join(ntid[i], &tret); 
   }
}
