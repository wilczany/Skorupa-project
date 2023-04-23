#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

#include "fork.h"
#include "split.h"


#include <unistd.h>





void pUserDir(){
    
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf(" %s \n\n", cwd); 
	
}
int main(){
    char *const nullek[] = {NULL};
    char *const forkc[] = {"more", "fork.c", NULL};
    
    char buf[32] = { 0 };
    char** sep;
    // CTRL + D wywala nieskończoną pentlę
    while (buf[0] != 'q' ) {
        pUserDir();
        printf("ps\n");
        printf("\n%i\n",sProgramForeground("ps", nullek));
        fgets(buf, sizeof(buf), stdin); 
        
    }
    


    // do{
    //     // pUserDir();
    //     // printf("more fork.c\n");
    //     // sProgramForeground("more", forkc);

    //     pUserDir();
    //     printf("ps\n");
    //     printf("\n%i\n",sProgramForeground("ps", nullek));

    //     //pUserDir();
    //     //printf("firefox&\n");
    //     //sProgramBackground("firefox", nullek);

    // }while(getc(stdin)!='q');
    return 0;
}
