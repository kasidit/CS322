#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>

static jmp_buf env_alrm; 
int ret; 

static void sig_alrm(int signo){
    printf("sig_alrm(%d) is called\n", signo); 
    fflush(stdout); 
    longjmp(env_alrm, 1); 
}

unsigned int sleep2(int sec){ 
    if(signal(SIGALRM, sig_alrm) == SIG_ERR)
      return(sec); 
    if((ret = setjmp(env_alrm)) == 0){
      alarm(sec); 
      pause(); 
    }
    else{
      printf("setjmp return %d after sig_alrm()\n", ret); 
    }
    return(alarm(0)); 
}

int main(int argc, char* argv[]){
    printf("bef sleep2\n"); 
    sleep2(10); 
    printf("after sleep2\n"); 
}
