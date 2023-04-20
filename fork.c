//nazwa robocza, kod z demona lekko zmieniony

//ifguardy trzeba przemyslec jak zrobic
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


//reminder ze przekazane args[] ma zaczynac sie PROGRAMEM i konczyc sie NULLem
//juz dziala
int sProgramForeground(const char* progName, char *const args[]){
        pid_t chpid = fork();
        if(chpid < 0){
            fprintf(stderr, "sProgramBackground, fork(): %s",strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        else if(chpid == 0){
            int status = execvp(progName, args);
            if(status == -1){
                fprintf(stderr, "sProgramBackground, execvp(...): %s\n", strerror(errno));
            }
            exit(EXIT_FAILURE);
        }

        else if(chpid > 0){ 
            wait(NULL); //to czeka na zakonczenie procesu dziecka bodajze 
            return(0);
        }

        return(-1); //tego returna nie chcemy
}

//reminder ze przekazane args[] ma zaczynac sie PROGRAMEM i konczyc sie NULLem
//chyba dziala (y)
int sProgramBackground(const char* progName, char *const args[]){
        pid_t chpid = fork();
        if(chpid < 0){
            fprintf(stderr, "sProgramBackground, fork(): %s",strerror(errno));
        }
        
        else if(chpid == 0){
            int status = execvp(progName, args);
            if(status == -1){
                fprintf(stderr, "sProgramBackground, execvp(...): %s\n", strerror(errno));
            }
            exit(EXIT_FAILURE);
        } 

        return(0);

}