#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t v1;
pthread_mutex_t v2;

void * t(void * arg){
   long int id = (long) arg; 
   pthread_t tid = pthread_self(); 
   printf("this is tr%ld tid = %lu \n", id, (unsigned long)tid); 
   
   if(id == 1){
     pthread_mutex_lock(&v1); 
     sleep(1);
     printf("before enter csection of t(1)\n"); 
     pthread_mutex_lock(&v2); 
     printf("enter critical section of t(1)\n"); 
     pthread_mutex_unlock(&v2); 
     pthread_mutex_unlock(&v1); 
   }
   else if(id == 2){
     pthread_mutex_lock(&v2); 
     sleep(1);
     printf("before enter csection of t(2)\n"); 
     pthread_mutex_lock(&v1); 
     printf("enter critical section of t(2)\n"); 
     pthread_mutex_unlock(&v1); 
     pthread_mutex_unlock(&v2); 
   }

   pthread_exit((void *)NULL); 
}

int main(void){

   pthread_t ntid1, ntid2;

   pthread_mutex_init(&v1, NULL); 
   pthread_mutex_init(&v2, NULL); 
    
   pthread_create(&ntid1, NULL, t, (void *)1);  
   pthread_create(&ntid2, NULL, t, (void *)2);  

   pthread_join(ntid1, NULL);
   pthread_join(ntid2, NULL);

}
