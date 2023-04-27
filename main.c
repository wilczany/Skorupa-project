#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>

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

char *homePath(){ //obsluga bledow
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
}

int cd(char **args, int arguments_count);


//znak konca pliku (ctrl+d) powoduje segmentation fault gdy wprowadza sie go jako pierszy znak
//dodatkowo nawet wprowadzony pozniej nie jest obslugiwany jakkolwiek i powoduje ponownie segmentation fault przy dalszym przekazaniu do funkcji

//tak samo znak sigquit (ctrl+\), gdy sie go przekaze do bufora bez dodania zadnych innych znakow (tzn ctrl+\ a nastepnie enter)
int main(){
    __sighandler_t sint, squit, skill;
    if((squit = signal(SIGQUIT, handler_quit)) == SIG_ERR){
    	fprintf(stderr, "signal(SIGQUIT,..) error\n");
    	exit(EXIT_FAILURE);
    }
    
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
            else if(arguments_count == 1) fprintf(stderr, "cd: Zbyt mało argumentów\n");
            else if(arguments_count > 3) fprintf(stderr,"cd: Zbyt wiele argumentów\n");
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
    fprintf(stdout, "\ntutaj wyswietlic historie polecen\n");
    pUserDir(); //to sie wykonac nie chce bruh kocham c
    return;
}