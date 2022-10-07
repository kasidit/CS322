#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex_var; 
pthread_cond_t cv; 

int global_var = 0; 

void * foo(void * arg){

   pthread_mutex_lock(&mutex_var);  
   printf("foo() critical 1: block on global_var == %d\n"
                                        , global_var); 
   while(global_var <= 0){
     pthread_cond_wait(&cv, &mutex_var); 
   }
   printf("foo() critical 2: new global_var is %d\n"
                                        , global_var); 
   global_var--;
   printf("foo() critical 2: update global_var to %d\n"
                                        , global_var); 
   pthread_mutex_unlock(&mutex_var);  

   pthread_exit((void *)NULL); 
}

void * bar(void * arg){
   sleep(3);
   pthread_mutex_lock(&mutex_var);  
   global_var ++;
   printf("bar() update global_var to %d\n", global_var); 
   pthread_cond_broadcast(&cv);
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

   pthread_join(ntid[0], tret); 
   pthread_join(ntid[1], tret); 

   pthread_cond_destroy(&cv); 
   pthread_mutex_destroy(&mutex_var); 
   printf("main() global_var is %d\n", global_var); 
}
