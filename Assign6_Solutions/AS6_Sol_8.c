#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex_var; 
pthread_cond_t cv; 

#define NUM_THREADS 4
int global_var = 0; 

void * foo(void * arg){

   pthread_mutex_lock(&mutex_var);  
   global_var++; 
   printf("foo() critical 1: block on global_var == %d\n"
                                        , global_var); 
   while(global_var < NUM_THREADS){
     pthread_cond_wait(&cv, &mutex_var); 
   }
   printf("foo() critical 2: done global_var to %d\n"
                                        , global_var); 
   pthread_cond_signal(&cv);
   pthread_mutex_unlock(&mutex_var);  

   pthread_exit((void *)NULL); 
}

int main(void){

   pthread_t ntid[NUM_THREADS];
   void *tret; 
   int i; 
    
   pthread_mutex_init(&mutex_var, NULL); 
   pthread_cond_init(&cv, NULL); 

   for(i=0; i< NUM_THREADS; i++){
     pthread_create(&ntid[i], NULL, foo, NULL);  
   }

   for(i=0; i< NUM_THREADS; i++){
     pthread_join(ntid[i], &tret); 
   }

   pthread_cond_destroy(&cv); 
   pthread_mutex_destroy(&mutex_var); 
   printf("main() global_var is %d\n", global_var); 
}
