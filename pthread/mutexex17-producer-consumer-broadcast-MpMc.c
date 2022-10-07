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

int items_per_prod;
int items_per_cons;

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

void print_Q(int prefix){
  int i; 
  printf("%d: Q[%02d] = ( ", prefix, Q_len); 
  for(i = 0; i < Q_SIZE; i++){
    printf("%d ", Q[i]); 
  }
  printf(") [h %02d:t %02d]\n",Q_head, Q_tail);; 
}

int en_Q(int data){ // enqueue the queue

   pthread_mutex_lock(&mutexQ);  

   while(Q_len == Q_SIZE){ // the Q is full.
     print_Q(1);
     pthread_cond_wait(&not_full, &mutexQ); 
   }
   Q[Q_tail] = data;
   printf("Q[t%02d]Ql=%d <-- %d\n", Q_tail, Q_len, data); 
   Q_tail = (Q_tail + 1) % Q_SIZE; 
   Q_len++;
   pthread_cond_broadcast(&not_empty); 
   pthread_mutex_unlock(&mutexQ);  
   return data;
}

int de_Q(void){ // dequeue the queue
   int data;
   pthread_mutex_lock(&mutexQ);  

   while(Q_len == 0){ // the Q is full.
     print_Q(0);
     pthread_cond_wait(&not_empty, &mutexQ); 
   }
   data = Q[Q_head];
   printf("Q[h%02d]Ql=%d --> %d\n", Q_head, Q_len, data); 
   Q[Q_head] = 0;
   Q_head = (Q_head + 1) % Q_SIZE; 
   Q_len--;

   pthread_cond_broadcast(&not_full); 
   pthread_mutex_unlock(&mutexQ);  
   return data;
}

void * producer(void * arg){
   long id = (long) arg;
   int data = 1;
   int i;  
   for(i = 0; i < items_per_prod; i++){
     printf("p%ld(%d):", id, i);
     en_Q(data);
   }
}

void * consumer(void * arg){
   long id = (long) arg;
   int data;
   int i;  
   for(i = 0; i < items_per_cons; i++){
     printf("c%ld(%d):", id, i);
     data = de_Q();
   }
}

#define CONSUME_UNIT 10

int main(int argc, char * argv[]){
   int i; 
   int num_prod = 1;
   int num_cons = 3;
   pthread_t *ptid;
   pthread_t *ctid;

   printf("argc = %d\n", argc); 
   if(argc == 1){
     num_prod = 1;
     num_cons = 3; 
     items_per_prod = num_cons * CONSUME_UNIT; 
     items_per_cons = CONSUME_UNIT;
   }
   else if(argc == 2){
     num_prod = 1;
     num_cons = atoi(argv[1]); 
     items_per_prod = num_cons * CONSUME_UNIT; 
     items_per_cons = CONSUME_UNIT;
   }
   else if(argc == 3){
     num_prod = atoi(argv[1]);
     num_cons = atoi(argv[2]); 
     items_per_cons = CONSUME_UNIT;
     items_per_prod = num_cons * CONSUME_UNIT; 
     if(items_per_prod % num_prod == 0){
       items_per_prod = (int)(items_per_prod/num_prod); 
       printf("items per producer = %d, items per consumers = %d\n",
             items_per_prod, items_per_cons);  
     }
     else{
       printf("Since each consumer consume %d items,\nthe total number of items needed by consumers (i.e. %d)\nis not divisible by number of producers (%d). Abort!\n",
               items_per_cons, items_per_prod, num_prod); 
       exit(1); 
     }
   }
   else{
     printf("wrong input parameters\n");
     exit(1); 
   }

   ptid = (pthread_t *)malloc(num_prod*sizeof(pthread_t)); 
   ctid = (pthread_t *)malloc(num_cons*sizeof(pthread_t)); 
   
   void *tret; 

   init_Q();

   for(i=0; i < num_prod; i++){
     pthread_create(ptid+i, NULL, producer, (void *)(long)i);  
   }
   for(i=0; i < num_cons; i++){
     pthread_create(ctid+i, NULL, consumer, (void *)(long)i);  
   }
   
   for(i=0; i < num_prod; i++){
     pthread_join(ptid[i], NULL); 
   }
   for(i=0; i < num_cons; i++){
     pthread_join(ctid[i], NULL); 
   }

   pthread_cond_destroy(&not_full); 
   pthread_cond_destroy(&not_empty); 
   pthread_mutex_destroy(&mutexQ); 

}
