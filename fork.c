//nazwa robocza, kod z demona lekko zmieniony

//ifguardy trzeba przemyslec jak zrobic
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


//reminder ze przekazane args[] ma konczyc sie NULLem
int sProgramForeground(const char progName, char *const args[]){
        int status = execvp(progName, args);
        if(status == -1){
            fprintf(stderr, "sProgramForeground: %s\n", strerror(errno));
        }

        //debug v
		printf("jak sie to wyswietli po ctrl+c? to wrocilo do naszej powloki");
        //jak nie to trzeba bardziej przekminic

        return(0);

}

//reminder ze przekazane args[] ma konczyc sie NULLem
int sProgramBackground(const char progName, char *const args[]){
        pid_t chpid = fork();
        if(chpid < 0){
            fprintf(stderr, "sProgramBackground, fork(): %s",strerror(errno));
        }
        
        else if(chpid == 0){
            int status = execvp(progName, args);
            if(status == -1){
                fprintf(stderr, "sProgramBackground, execvp(...): %s\n", strerror(errno));
            }
            abort();
        } 

        return(0);

}