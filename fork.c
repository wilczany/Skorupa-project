#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "read.h"

int sProgramForeground(const char* progName, char *const args[]){
        pid_t chpid = fork();
        if(chpid < 0){
            fprintf(stderr, "sProgramForeground, fork(): %s",strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        else if(chpid == 0){
            int status = execvp(progName, args);
            if(status == -1){
                fprintf(stderr, "sProgramForeground, execvp(...): %s\n", strerror(errno));
                //nieznane/nieprawidlowe polecenie
            }
            exit(EXIT_FAILURE);
        }

        else if(chpid > 0){ 
            while(wait(NULL) > 0); //czeka na zakonczenie procesu dziecka, a nawet wielu procesow dziecka(chociaz uzywamy tylko jednego) 
            //nie dziala to w przypadku odpalenia skryptem,,,
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
            int out = dup(STDOUT_FILENO);
            if(out == -1){
                fprintf(stderr,"sProgramBackground, dup(): %s",strerror(errno));
                exit(EXIT_FAILURE);
            }

            int null = open("/dev/null",O_WRONLY);
            if(null == -1){
                fprintf(stderr,"sProgramBackground, open(): %s",strerror(errno));
                exit(EXIT_FAILURE);
            }

            int dup2status = dup2(null, STDERR_FILENO);
            if(dup2status == -1) {
                fprintf(stderr,"sProgramBackground, dup2(): %s",strerror(errno));
                exit(EXIT_FAILURE);
            }

            int status = execvp(progName, args);
            if(status == -1){
                fprintf(stderr, "sProgramBackground, execvp(...): %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        } 

        return(0);


}

void pipes_handler(char **progs, int pipes_count){
    //int* pipes = malloc((count - 1) * 2 * sizeof(int));
        for(int i = 0; i< pipes_count; i++)
        printf ("%i", pipes_count);
    
        int* fd = malloc(sizeof(fd));

        
        //int fd[pipes_count-1][2];
        
        // for (int i = 0; i < pipes_count ;i += 1) {
        //     if(pipe(fd[i]) < 0)
        //         printf("FAILURE");
        // }
        // int i = 0;
        // while(fd[i] != '\0'){
        //     printf("xd");
        //     i++;
        // }
        // // pętla{
        // // int counter = 0;
        // // char **xddd = separate(progs[0],&counter);
        // // sProgramForeground(xddd[0],xddd,fd);
        
        // // }
        // //sProgramForeground();
        // for(int i = 0; i< pipes_count-1; i++){
        //     close(fd[i][0]);
        //     close(fd[i][1]);
        // }
        
    }