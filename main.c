#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

#include "fork.h"
#include "split.h"


#include <unistd.h>





void pUserDir(){
    
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    printf(" %s>>", cwd); 
	
}

int main(){
    char *const nullek[] = {NULL};
    char *const forkc[] = {"more", "fork.c", NULL};
    
    char *buf;
    int count= 0;
    char** sep;
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 11);

    while (1) {
        pUserDir();
        
        buf = readLine();
        char **program = separate(buf);

        sProgramForeground(program[0],program);
        
        // free(buf); przy wyjsciu z programu
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
