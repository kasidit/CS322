#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void scrapper(void){
    system("rm commandstr.txt"); 
}

int main(int argc, char* argv[]){
 
    char cmdstring[100]; 
    int  option = 0; 

    if(argc == 3){
	    option = atoi(argv[2]); 
    }

    if(option == 1){
	    printf("register atexit()\n");
	    atexit(scrapper); 
    }

    FILE *fp = fopen("commandstr.txt", "w+"); 

    fprintf(fp, "%s\n", argv[1]); 
    printf("%s\n", argv[1]); 

    fclose(fp); 

    system(argv[1]); 
    exit(0); 

}
