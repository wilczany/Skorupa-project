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

#include <unistd.h>
#include <string.h>


const int MAX_SIZE = 512;

void handler_quit(int signum);

void pUserDir(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd)); //obsluga bledu

    printf(" %s>>", cwd); 
	
}

char *userPath(){ //obsluga bledow
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
}

char *homePath(){ //obsluga bledow
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
}

int cd(char **args, int arguments_count);


//znak sigquit (ctrl+\), gdy sie go przekaze do bufora bez dodania zadnych innych znakow (tzn ctrl+\ a nastepnie enter) powoduje segmentation fault
//sProgramBackground dalej powoduje segmentation fault jesli program cokolwiek wypisze, poza tym to nie dziala notabene

int main(int argc, char *argv[]){
    fprintf(stderr,"halo zyjesz?");
    //obsluga skryptow
    if(argc > 1){    
        fprintf(stderr,"docieramy tu?");
        char** args = malloc((argc+1) * sizeof *args);
        for(int i = 0; i < argc-1; i++)
        {
            args[i] = strdup(argv[i+1]);
        }
        args[argc] = NULL;
        fprintf(stderr,"kupa 1\n");
        //if(strcmp(argv[argc-1],"&")==0){
        //    sProgramBackground(args[0],args);
        //}else{
        sProgramBackground(args[0],args);
        fprintf(stderr,"kupa 2\n");
        //}

        for(int i = 0; i < argc; i++)
        {
            free(args[i]);
        }
        free(args);
        return 0;
    }

    else{
        fprintf(stderr,"zagłada");
        __sighandler_t sint, squit, skill;
        char *cwd[] = {"cd", userPath(), NULL}; 
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

        char *buf = malloc(sizeof(char)*MAX_SIZE);
        char** sep;
        const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 11);

        int st = 1;
        while (st>0) {
            pUserDir();
            int arguments_count = 0;
            buf = readLine(&st);
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
                program[0],"exit")==0)
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
            else{
                program[arguments_count-1] = '\0';
                sProgramBackground(program[0],program);
            }

            free(buf); //przy wyjsciu z programu
        }
        close(hist);
    }

    return 0;
}

int cd(char **args, int arguments_count){
    char first = args[1][0];
    char sciezka[4096]; //linuksowy limit sciezki to 4095 czy 4096, ale moze to byc leeekki overkill w malym projekcie
    switch(first){
        case '-': fprintf(stderr, "cd: Funkcja powrotu (jeszcze) niedostępna\n"); return 2;
        case '~': strcpy(sciezka,homePath()); break; //roboczo
        default: strcpy(sciezka,args[1]);
    }

    DIR* dir = opendir(sciezka);
    if (dir) {
        if(closedir(dir) == -1){
            fprintf(stderr, "cd, closedir(): %s",strerror(errno));
            return -1;
        }
        if(chdir(sciezka) == -1){
            fprintf(stderr, "cd, chdir(): %s",strerror(errno));
            return -1;
        }
    } else if (ENOENT == errno) { //kod bledu braku podanej sciezki
        fprintf(stderr, "cd: Brak takiej ścieżki: %s\n", sciezka);
        return 0;
    } else {
        fprintf(stderr, "cd, opendir(): %s\n",strerror(errno));
        return -1;
    }    

    return 1;
}

void handler_quit(int signum){
    //fprintf(stdout, "\ntutaj wyswietlic historie polecen\n");
    return;
}
