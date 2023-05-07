#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "read.h"


typedef struct pipes_struct{
    int size;
    int potoki[];
} P_S;

int sProgramForeground(const char* progName, char *const args[], P_S *p, int seq){

    // struct p posiada w sobie zaiiniciowane potoki
    // jezeli zamiast niego zostal podany NULL to funkcja nie wywola ich obslugi

    int status = 0;
    
        //tworzymy proces potomny
        pid_t chpid = fork();

        // obsluga bledow
        if(chpid < 0){
            fprintf(stderr, "sProgramForeground, fork(): %s",strerror(errno));
            exit(EXIT_FAILURE);
        }

         // jezeli jest to proces potomny
         else if(chpid == 0){
            
            // jezeli i/o procesu nalezy przekierowac
            if (p != NULL){
                    
                // tutaj odbywa sie laczenie potokow oraz zamykanie nieuzywanych polaczen
                // zmienna pomocnicza seq odpowiada za kolejnośc łączenia potoków z procesami, liczenie zaczynamy od -1
                if(seq == -1){
                
                dup2(p->potoki[1], STDOUT_FILENO);
               

                }else if( seq == p->size - 2){
                    
                dup2(p->potoki[(seq) * 2], STDIN_FILENO);
               
                    

                }else {
                dup2(p->potoki[0 + (seq * 2)], STDIN_FILENO);
                dup2(p->potoki[3 + (seq * 2)], STDOUT_FILENO);
                
               


                }

            }

            for(int i =0;i < (p->size-1)*2; i++){
                close(p->potoki[i]);
                    }

            //uruchamiamy zadany program
            int status = execvp(progName, args);


            if(status == -1){
                if(errno == ENOENT) fprintf(stderr, "Brak takiego polecenia lub ścieżki.\n");
                else fprintf(stderr, "sProgramForeground, execvp(...): %s\n", strerror(errno));
                //nieznane/nieprawidlowe polecenie
                exit(EXIT_FAILURE);

            }

            exit(EXIT_FAILURE);
        }

        // jezeli jest to proces rodzica
        if(chpid > 0){ 

                // jezeli zostaly uzyte potoki

            if (p != NULL && seq == p->size - 2){

                for(int i =0;i < (p->size-1)*2; i++){
                close(p->potoki[i]);
                    }

            pid_t wpid;
            while ((wpid = wait(&status)) > 0);  //czeka na zakonczenie procesu dziecka, a nawet wielu procesow dziecka
            
            

            }else if (p != NULL && seq < p->size - 2){
            

        }else waitpid (chpid, NULL, 0);
            return(0);
        
        
        }

        return(-1); //tego returna nie chcemy
}


int sProgramBackground(const char* progName, char *const args[]){
        pid_t chpid = fork();
        if(chpid < 0){
            fprintf(stderr, "sProgramBackground, fork(): %s",strerror(errno));
        }
        
        if(chpid) fprintf(stderr, "[Uruchomiono proces %i]\n",chpid);
        
        if(chpid == 0){
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

            int dup2status = dup2(null, STDOUT_FILENO);
            if(dup2status == -1) {
                fprintf(stderr,"sProgramBackground, dup2(): %s",strerror(errno));
                exit(EXIT_FAILURE);
            }

            int status = execvp(progName, args);
            if(status == -1){
                if(errno == ENOENT) fprintf(stderr, "Brak takiego polecenia lub ścieżki.\n");
                else fprintf(stderr, "sProgramBackground, execvp(...): %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        } 

        return(0);


}

void pipes_handler(char **progs, int pipes_count){


        P_S *pp = malloc(sizeof(P_S) + (pipes_count-1) *2 * sizeof(int));
        
        
        //int *fd = malloc((pipes_count - 1) * 2 * sizeof(int));
            
        for (int i = 0; i < (pipes_count-1) * 2 ;i  += 2) {
            if(pipe(pp->potoki + i) < 0){
                fprintf(stderr,"\n\nFAILURE\n\n");
                exit(EXIT_FAILURE);}
        }
    
        pp->size = pipes_count;

       for (int i = -1 ; i < (pipes_count-1); i++){

         int arguments_count = 0;
         char **program = separate(progs[i+1], &arguments_count, " ");

         
         sProgramForeground(program[0], program, pp, i);
         
       }
        
    }
