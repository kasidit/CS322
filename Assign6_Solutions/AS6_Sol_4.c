#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex_var; 
pthread_cond_t cv; 

int count = 0; 

void * foo(void * arg){

   pthread_mutex_lock(&mutex_var);  
   printf("foo() critical 1: block on count == %d\n"
                                        , count); 
   while(count <= 0){
     pthread_cond_wait(&cv, &mutex_var); 
   }
   printf("foo() critical 2: new count is %d\n"
                                        , count); 
   count--;
   printf("foo() critical 2: update count to %d\n"
                                        , count); 
   pthread_mutex_unlock(&mutex_var);  

   pthread_exit((void *)NULL); 
}

void * bar(void * arg){
   sleep(3);
   pthread_mutex_lock(&mutex_var);  
   count ++;
   printf("bar() update count to %d\n", count); 
   pthread_cond_signal(&cv);
   pthread_mutex_unlock(&mutex_var);  

   pthread_exit((void *)NULL); 
}

#define NUM_THREADS 2

int main(void){

   pthread_t ntid[NUM_THREADS];
   void *tret; 
    
   pthread_mutex_init(&mutex_var, NULL); 
   pthread_cond_init(&cv, NULL); 

   pthread_create(&ntid[0], NULL, foo, NULL);  
   pthread_create(&ntid[1], NULL, bar, NULL);  

   pthread_join(ntid[0], NULL); 
   pthread_join(ntid[1], NULL); 

   pthread_cond_destroy(&cv); 
   pthread_mutex_destroy(&mutex_var); 
   printf("main() count is %d\n", count); 
}
