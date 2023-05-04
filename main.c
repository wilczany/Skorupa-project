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
int PIPE = 0;
int REDIRECTION = 0;

void handler_quit(int signum);

void pUserDir(){
    char cwd[MAX_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        size_t len = strlen(cwd);
        write(STDOUT_FILENO, cwd, len);
        write(STDOUT_FILENO, ">> ", 3);
    }else perror("getcwd() error");
	
}

char *userPath(char *path){ //obsluga bledow
    char cwd[MAX_SIZE];
    getcwd(cwd, sizeof(cwd));
    strcpy(path,cwd);
}

//znak sigquit (ctrl+\), gdy sie go przekaze do bufora bez dodania zadnych innych znakow (tzn ctrl+\ a nastepnie enter) powoduje segmentation fault
//sProgramBackground dalej powoduje segmentation fault jesli program cokolwiek wypisze, poza tym to nie dziala notabene
int global_hist, h_lines = 0;
int initHistory();
int truncHistory();

int main(int argc, char *argv[]){
    //obsluga skryptow
    if(argc > 1){    
        int fd = open(argv[1], O_RDONLY, 0777);
        dup2(fd, STDIN_FILENO);

    }
    //  else{
        __sighandler_t squit;

        char *path = malloc(MAX_SIZE * sizeof(char));
        userPath(path);
        char *cwd[] = {"cd", path, NULL}; 
        
        //przejscie do folderu domowego
        if ((chdir(homePath())) < 0) {
            fprintf(stderr, "main, chdir(~): %s\n",strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        //otwarcie/stworzenie w nim pliku
        global_hist = open("sforkkorupaHist", O_RDWR | O_APPEND | O_CREAT, 0777);
        if (global_hist < 0){
            fprintf(stderr, "initHistory(), open(skorupaHist): %s\n",strerror(errno));
            exit(EXIT_FAILURE);
        }

        initHistory();

        if((squit = signal(SIGQUIT, handler_quit)) == SIG_ERR){
            fprintf(stderr, "signal(SIGQUIT,..) error\n");
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

            if(argc == 1){
            pUserDir();
            }
            int char_count = 0;
            int arguments_count = 0;
            int pipe_counter = 0;
            buf = readLine(&st, &char_count);
            
            
            if(st == -1){
                break;
            }
            if(strchr(buf, '>') != NULL){
                REDIRECTION = 1;
            }
            if(strchr(buf, '|') != NULL){
                PIPE = 1;
            }
            if(REDIRECTION && PIPE){
                fprintf(stderr, "Nieobsługiwany przypadek\n"); 
                REDIRECTION = 0; PIPE = 0;
                continue;
            }
        

            // if(PIPE){
            // char **program = separate(buf, &pipe_counter, "|");
            // if(pipe_counter>1){
            // pipes_handler(program, pipe_counter);
            // }
            // }
            
            if(strcmp(buf,"")==0 || strcmp(buf," ")==0) continue;


            write(global_hist, buf, char_count);
            write(global_hist, "\n", 1);
            // h_lines++;

            if(h_lines >= 22) truncHistory();
            else{h_lines++;}

            char **program = separate(buf, &arguments_count, " ");

            char* temp = malloc(1 * sizeof(char));
            *temp = program[0][0];
            if (strcmp(temp, "#") == 0)
            continue;

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
                sProgramForeground(program[0],program,NULL,1);
                
            }
            else{
                program[arguments_count-1] = '\0';
                sProgramBackground(program[0],program);
            }

            free(buf); //przy wyjsciu z programu
        }
    close(global_hist);
    //}

    return 0;
}



void handler_quit(int signum){
    unsigned char buffer[16];
	size_t bytes_read;
    lseek(global_hist, 0, SEEK_SET);
    write(STDOUT_FILENO, "\n", 1);
	do{
		bytes_read = read(global_hist, buffer, sizeof(buffer));
        write(STDOUT_FILENO, buffer, bytes_read);
	}while(bytes_read == sizeof(buffer));
    write(STDOUT_FILENO, "\n", 1);
    pUserDir();

    __sighandler_t squit;
    if((squit = signal(SIGQUIT, handler_quit)) == SIG_ERR){
            fprintf(stderr, "signal(SIGQUIT,..) error\n");
            exit(EXIT_FAILURE);
    }
    return;
}

int initHistory(){
    unsigned char buffer[1]; 
	size_t bytes_read;
    do{
        bytes_read = read(global_hist, buffer, sizeof(buffer));
        if(strcmp(buffer, "\n")==0) h_lines++;
    }while(bytes_read == sizeof(buffer));
    if(h_lines==0) h_lines=1;
    return 1;
}

int truncHistory(){ 
    size_t bytes_read;
    int nl_pos=0;
    char buffer[1];
    char * nl;

    //szukanie konca pierwszej linii
    lseek(global_hist, 0, SEEK_SET);
    do{
        bytes_read = read(global_hist, buffer, sizeof(buffer));
        nl_pos++;
        if((nl = strchr(buffer,'\n')) != NULL){
            //
            //fprintf(stderr,"\nznalezione: %i\n", nl_pos);
            //
            break;

        }
        //
        //write(STDOUT_FILENO, buffer, sizeof(buffer));
        //
    }while(bytes_read == sizeof(buffer));

    //przepisanie pliku od drugiej linii
    lseek(global_hist, nl_pos, SEEK_SET);
    int tmpFile = open("tmp", O_CREAT | O_RDWR | O_TRUNC, 0777);
    do{
        bytes_read = read(global_hist, buffer, sizeof(buffer));
        write(tmpFile, buffer, sizeof(buffer));
    }while(bytes_read == sizeof(buffer));

    close(global_hist);
    char *path = malloc(MAX_SIZE * sizeof(char));
    userPath(path);
    char *cwd[] = {"cd", path, NULL}; 
        
    //przejscie do folderu domowego
    if ((chdir(homePath())) < 0) {
        //
        //fprintf(stderr, "main, chdir(~): %s\n",strerror(errno));
        //
        exit(EXIT_FAILURE);
    }
        
    //otwarcie/stworzenie w nim pliku
    global_hist = open("skorupaHist", O_RDWR | O_TRUNC | O_CREAT, 0777);
    if (global_hist < 0){
        //
        //fprintf(stderr, "truncHistory(), open(skorupaHist): %s\n",strerror(errno));
        //
        exit(EXIT_FAILURE);
    }

    char buff[1];
    int pop=0;
    lseek(tmpFile, 0, SEEK_SET);
    do{
        bytes_read = read(tmpFile, buff, sizeof(buff));
        if(strcmp(buff,"\n")==0 && pop) {pop=0; continue;}
        else {write(global_hist, buff, sizeof(buff)); pop=0;}
        if(strcmp(buff,"\n")==0 && pop==0) {pop=1;}

    }while(bytes_read == sizeof(buff));

    cd(cwd, 2);        
    free(path);

    close(tmpFile);
    
    initHistory();
    return 1;
}

