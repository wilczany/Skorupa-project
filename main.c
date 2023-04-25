#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

#include "fork.h"
#include "read.h"


#include <unistd.h>
#include <string.h>


const int MAX_SIZE = 512;



void pUserDir(){
    
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    printf(" %s>>", cwd); 
	
}

int main(){
    char *const nullek[] = {NULL};
    char *const forkc[] = {"more", "fork.c", NULL};
    
    char *buf = malloc(sizeof(char)*MAX_SIZE);
    char** sep;
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 11);

    while (1) {
        pUserDir();
        int arguments_count = 0;
        buf = readLine();
        char **program = separate(buf, &arguments_count);
        if(strcmp(
            program[arguments_count-1],
            "&"))
            {
            sProgramForeground(program[0],program);
            
        }
        else{
            printf("%s",program[arguments_count-1]);
            program[arguments_count-1] = '\0';
            sProgramBackground(program[0],program);
        }

        free(buf); //przy wyjsciu z programu
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
