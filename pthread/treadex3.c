#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void * foo(void * arg){
   printf("Hello World\n"); 
   pthread_exit((void *)NULL); 
}

int main(void){

   pthread_t mtid, ntid;
   void *tret; 
    
   mtid = pthread_self(); 
   pthread_create(&ntid, NULL, foo, NULL);  

   pthread_join(ntid, tret); 
   printf("main thread id = %lu foo thread id = %lu\n", 
                                    (unsigned long) mtid, 
                                    (unsigned long) ntid);

}
