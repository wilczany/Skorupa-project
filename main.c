#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
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

char * homePath(){
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
}

int cd(char **args, int arguments_count);

int main(){
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
            program[0],"cd")==0)
            {
            if(arguments_count == 2 || arguments_count == 3) cd(program, arguments_count);
        }
        else if(strcmp(
            program[arguments_count-1],
            "&"))
            {
            sProgramForeground(program[0],program);
            
        }
        else{
            program[arguments_count-1] = '\0';
            sProgramBackground(program[0],program);
        }

        free(buf); //przy wyjsciu z programu
    }
    return 0;
}

int cd(char **args, int arguments_count){
    char first = args[1][0];
    char sciezka[128];
    switch(first){
        case '~': strcpy(sciezka,homePath()); break; //roboczo
        default: strcpy(sciezka,args[1]); printf("\n%c\n", first);
    }

    DIR* dir = opendir(sciezka);
    if (dir) {
        closedir(dir);
        chdir(sciezka);
    } else if (ENOENT == errno) {
        fprintf(stderr, "Brak takiej ścieżki: %s", sciezka);
    } else {
        fprintf(stderr, "cd, opendir(): %s",strerror(errno));
        exit(EXIT_FAILURE);
    }    
}