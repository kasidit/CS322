#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define Q_SIZE 5
int Q[Q_SIZE]; 
int Q_len; 
int Q_head, Q_tail; 

pthread_mutex_t mutexQ; 
pthread_cond_t not_full; 
pthread_cond_t not_empty; 

void init_Q(void){
  int i; 
  Q_len = 0; 
  for(i = 0; i < Q_SIZE; i++){
    Q[i] = 0; 
  }
  Q_head = Q_tail = 0; 
  pthread_mutex_init(&mutexQ, NULL);
  pthread_cond_init(&not_full, NULL); 
  pthread_cond_init(&not_empty, NULL); 
}

void print_Q(char * prefix){
  int i; 
  printf("%s: Q[%02d] = ( ", prefix, Q_len); 
  for(i = 0; i < Q_SIZE; i++){
    printf("%d ", Q[i]); 
  }
  printf(") [h %02d:t %02d]\n",Q_head, Q_tail);; 
}

int en_Q(int data){ // enqueue the queue

   pthread_mutex_lock(&mutexQ);  

   while(Q_len == Q_SIZE){ // the Q is full.
     print_Q("F:");
     pthread_cond_wait(&not_full, &mutexQ); 
   }
   Q[Q_tail] = data;
   printf("Q[t%02d] <-- %d\n", Q_tail, data); 
   fflush(stdout); 
   Q_tail++; 
   Q_tail = Q_tail % Q_SIZE; 
   Q_len++;

   pthread_cond_signal(&not_empty); 
   pthread_mutex_unlock(&mutexQ);  
   return data;
}

int de_Q(void){ // dequeue the queue
   int data;
   pthread_mutex_lock(&mutexQ);  

   while(Q_len == 0){ // the Q is empty.
     print_Q("E:");
     pthread_cond_wait(&not_empty, &mutexQ); 
   }
   data = Q[Q_head];
   printf("Q[h%02d] --> %d\n", Q_head, data); 
   fflush(stdout); 
   Q[Q_head] = 0;
   Q_head++; 
   Q_head = Q_head % Q_SIZE; 
   Q_len--;

   pthread_cond_signal(&not_full); 
   pthread_mutex_unlock(&mutexQ);  
   return data;
}

void * producer(void * arg){
   long data = (long) arg;
   int i;  
   for(i = 0; i < 10; i++){
     if(i % Q_SIZE == 0) sleep(2); // slowdown this thr
     printf("p:");
     en_Q(data);
   }
   pthread_exit((void *)NULL); 
}

void * consumer(void * arg){
   int data;
   int i;  
   for(i = 0; i < 10; i++){
     printf("c:");
     data = de_Q();
   }
   pthread_exit((void *)NULL); 
}

#define NUM_THREADS 2

int main(void){

   pthread_t ntid[NUM_THREADS];
   void *tret; 

   init_Q();
    
   pthread_create(&ntid[0], NULL, producer, (void *)1);  
   pthread_create(&ntid[1], NULL, consumer, NULL);  

   pthread_join(ntid[0], tret); 
   pthread_join(ntid[1], tret); 

   pthread_cond_destroy(&not_full); 
   pthread_cond_destroy(&not_empty); 
   pthread_mutex_destroy(&mutexQ); 
}
