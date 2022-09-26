#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char* argv[]){
 
    char *ptr = malloc(10*sizeof(char)); 
    char *nptr; 

    memset(ptr, 'a', 10); 

    printf("%s\n", ptr);

    char *ptr2 = calloc(10, sizeof(char)); 

    int clear_cnt = 0;
    int i; 

    for(i=0;i < 10; i++){
	    if(*(ptr2+i) == 0) clear_cnt++; 
    }
    printf("clear_cnt = %d\n", clear_cnt);

    char add_space_str[100]; 
    long add_space = 0; 
    long total_space = 10; 
    char *endptr; 

    while(fgets(add_space_str, 100, stdin) > 0){
	    printf("%s\n", add_space_str);
/* 
 * We should use strtol() because we can check input errors.  
 * However, for this assignment, atoi() is good enough.
 *
 */
/*
	   add_space = strtol(add_space_str, &endptr, 10);  
	    
	   if ((errno == ERANGE && (add_space == LONG_MAX || add_space == LONG_MIN))
                   || (errno != 0 && add_space == 0)) {
               perror("strtol");
               exit(EXIT_FAILURE);
           }

           if (endptr == add_space_str) {
               fprintf(stderr, "No digits were found\n");
               exit(EXIT_FAILURE);
           }
*/
	   add_space = atoi(add_space_str); 
	   
	   printf("%ld\n", add_space); 

	   if(add_space <= -1) break; 
	    
	   if(add_space > 0){ 
             total_space += add_space; 
	     nptr = realloc(ptr, total_space); 

	     if(nptr == NULL) break;
	     
	     ptr = nptr; 
	     memset(ptr, 'a', total_space); 

             printf("%s\n", ptr);
	   }  

    }
}
