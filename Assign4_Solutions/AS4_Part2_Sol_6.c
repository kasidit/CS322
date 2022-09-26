#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>

void f1(), f2(), f3(), f4(), f5(), f6(), f7(), f8(); 

static jmp_buf backto_f1; 
static jmp_buf backto_main; 
int ret; 

void f1(){
    if((ret = setjmp(backto_f1)) == 0){
      f2(); 
    }
    else{
      printf("setjmp return ret=%d after longjmp(backto_f1)\n", ret); 
      f5(); 
    }
}

void f2(){
    f3(); 
}

void f3(){
    f4(); 
}

void f4(){
    longjmp(backto_f1, 1); 
}

void f5(){
    f6();
}

void f6(){
    f7();
}

void f7(){
    longjmp(backto_main, 1); 
}

void f8(){
    exit(0);
}

int main(int argc, char* argv[]){
    if((ret = setjmp(backto_main)) == 0){
      f1(); 
    }
    else{
      printf("setjmp return ret=%d after longjmp(backto_main)\n", ret); 
      f8(); 
    }
}
