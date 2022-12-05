#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t v1, v2, v3, v4, v5;
int a, b, c, d, e; 

void * t(void * arg){
   long int id = (long) arg; 
   if(id == 1){
     pthread_mutex_lock(&v1); 
     pthread_mutex_lock(&v2); 
     pthread_mutex_lock(&v3); 
     pthread_mutex_lock(&v4); 
     pthread_mutex_lock(&v5); 
     printf("enter critical section of t(%ld)\n", id); 
     a = b + c + d + e; 
     pthread_mutex_unlock(&v5); 
     pthread_mutex_unlock(&v4); 
     pthread_mutex_unlock(&v3); 
     pthread_mutex_unlock(&v2); 
     pthread_mutex_unlock(&v1); 
   }
   else if(id == 2){
     pthread_mutex_lock(&v1); 
     pthread_mutex_lock(&v2); 
     pthread_mutex_lock(&v3); 
     pthread_mutex_lock(&v4); 
     pthread_mutex_lock(&v5); 
     printf("enter critical section of t(%ld)\n", id); 
     b = a + c + d + e; 
     pthread_mutex_unlock(&v5); 
     pthread_mutex_unlock(&v4); 
     pthread_mutex_unlock(&v3); 
     pthread_mutex_unlock(&v2); 
     pthread_mutex_unlock(&v1); 
   }
   else if(id == 3){
     pthread_mutex_lock(&v1); 
     pthread_mutex_lock(&v2); 
     pthread_mutex_lock(&v3); 
     pthread_mutex_lock(&v4); 
     pthread_mutex_lock(&v5); 
     printf("enter critical section of t(%ld)\n", id); 
     c = a + b + d + e; 
     pthread_mutex_unlock(&v5); 
     pthread_mutex_unlock(&v4); 
     pthread_mutex_unlock(&v3); 
     pthread_mutex_unlock(&v2); 
     pthread_mutex_unlock(&v1); 
   }
   else if(id == 4){
     pthread_mutex_lock(&v4); 
     printf("enter critical section of t(%ld)\n", id); 
     d = 100; 
     pthread_mutex_unlock(&v4); 
   }
   else if(id == 5){
     pthread_mutex_lock(&v1); 
     pthread_mutex_lock(&v4); 
     pthread_mutex_lock(&v5); 
     printf("enter critical section of t(%ld)\n", id); 
     e = a + d; 
     pthread_mutex_unlock(&v5); 
     pthread_mutex_unlock(&v4); 
     pthread_mutex_unlock(&v1); 
   }
   pthread_exit((void *)NULL); 
}

int main(void){

   pthread_t ntid1, ntid2, ntid3, ntid4, ntid5;

   a = b = c = d = e = 1; 

   pthread_mutex_init(&v1, NULL); 
   pthread_mutex_init(&v2, NULL); 
   pthread_mutex_init(&v3, NULL); 
   pthread_mutex_init(&v4, NULL); 
   pthread_mutex_init(&v5, NULL); 
    
   pthread_create(&ntid1, NULL, t, (void *)1);  
   pthread_create(&ntid2, NULL, t, (void *)2);  
   pthread_create(&ntid3, NULL, t, (void *)3);  
   pthread_create(&ntid4, NULL, t, (void *)4);  
   pthread_create(&ntid5, NULL, t, (void *)5);  

   pthread_join(ntid1, NULL);
   pthread_join(ntid2, NULL);
   pthread_join(ntid3, NULL);
   pthread_join(ntid4, NULL);
   pthread_join(ntid5, NULL);

   printf("a = %d b = %d c = %d d = %d e = %d\n", a,b,c,d,e); 
}
