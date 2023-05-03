#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>
#include <fcntl.h>

#include "fork.h"
#include "read.h"
#include "builtin.h"

#include <unistd.h>
#include <string.h>


const int MAX_SIZE = 512;

void handler_quit(int signum);

void pUserDir(){
    char *cwd = get_current_dir_name();
    //getcwd(cwd, sizeof(cwd)); //obsluga bledu
    //write(STDOUT_FILENO, cwd, 2 * sizeof(&cwd));
    printf(" %s>>", cwd); 
	
}

char *userPath(char *path){ //obsluga bledow
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    strcpy(path,cwd);
}

//int cd(char **args, int arguments_count);


//znak sigquit (ctrl+\), gdy sie go przekaze do bufora bez dodania zadnych innych znakow (tzn ctrl+\ a nastepnie enter) powoduje segmentation fault
//sProgramBackground dalej powoduje segmentation fault jesli program cokolwiek wypisze, poza tym to nie dziala notabene

int main(int argc, char *argv[]){
    fprintf(stderr,"halo zyjesz?");

    
    //obsluga skryptow
    if(argc > 1){    
        // fprintf(stderr,"docieramy tu?");
        // char** args = malloc((argc+1) * sizeof *args);
        // for(int i = 0; i < argc-1; i++)
        // {
        //     args[i] = strdup(argv[i+1]);
        // }
        // args[argc] = NULL;
        // fprintf(stderr,"kupa 1\n");
        // //if(strcmp(argv[argc-1],"&")==0){
        // //    sProgramBackground(args[0],args);
        // //}else{
        // sProgramBackground(args[0],args);
        // fprintf(stderr,"kupa 2\n");
        // //}

        // for(int i = 0; i < argc; i++)
        // {
        //     free(args[i]);
        // }
        // free(args);
        int fd = open(argv[1], O_RDONLY, 0777);
        dup2(fd, STDIN_FILENO);

   //     return 0;
    }

    //  else{
        __sighandler_t sint, squit, skill;

        char *path = malloc(1024 * sizeof(char));
        userPath(path);
        char *cwd[] = {"cd", path, NULL}; 
        
        if((squit = signal(SIGQUIT, handler_quit)) == SIG_ERR){
            fprintf(stderr, "signal(SIGQUIT,..) error\n");
            exit(EXIT_FAILURE);
        }
        
        //historia poleceń
        //przejscie do folderu domowego
        if ((chdir(homePath())) < 0) {
            fprintf(stderr, "main, chdir(~): %s\n",strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        //otwarcie/stworzenie w nim pliku
        int hist = open("skorupaHist", O_WRONLY | O_APPEND | O_CREAT, 0777);
        if (hist < 0){
            fprintf(stderr, "main, open(history): %s\n",strerror(errno));
            exit(EXIT_FAILURE);
        }

        cd(cwd, 2);        
        free(path);
        char *buf = malloc(sizeof(char)*MAX_SIZE);
        char** sep;

        const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 11);
        
        int st = 1;
        while (st>0) {

            // CHWILOWO NIE DZIALA XDD
            // if(argc == 1){
            // pUserDir();
            // }
        sleep(0);
            int arguments_count = 0;
            buf = readLine(&st);
            //
            // DEBUG XDD

            // printf("funkcja: %s\n\n",buf);
            // printf("status:\t%i\n\nWywołanie:\n",st);
            //
            //
            write(hist, buf, MAX_SIZE);
            if(st == -1){
                break;
            }
            char **program = separate(buf, &arguments_count);
            if(strcmp(
                program[0],"cd")==0)
                {
                    if(arguments_count == 2 || arguments_count == 3) cd(program, arguments_count);
                    else if(arguments_count == 1) fprintf(stderr, "cd: Zbyt mało argumentów\n");
                    else if(arguments_count > 3) fprintf(stderr,"cd: Zbyt wiele argumentów\n");
            }
            else if(strcmp(
                program[0],"exit")==0 ||
                strcmp(
                program[0],"\\")==0
            )
                {
                    st = -1;
                    return 0;
            }
            else if(strcmp(
                program[arguments_count-1],
                "&"))
                {
                sProgramForeground(program[0],program);
                
            }
            // else{
            //     program[arguments_count-1] = '\0';
            //     sProgramBackground(program[0],program);
            // }

            free(buf); //przy wyjsciu z programu
        }
        close(hist);
    //}

    return 0;
}



void handler_quit(int signum){
    //fprintf(stdout, "\ntutaj wyswietlic historie polecen\n");
    return;
}
