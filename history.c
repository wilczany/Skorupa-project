#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
//#include
#include "builtin.h"
#include "history.h"

int global_hist = 0;
int h_lines = 0;


int initHistory(){
    unsigned char buffer[1]; 
	size_t bytes_read;
    do{
        bytes_read = read(global_hist, buffer, sizeof(buffer));
        if(strcmp(buffer, "\n")==0) h_lines++;
    }while(bytes_read == sizeof(buffer));
    if(h_lines==0) h_lines=1;

    if(h_lines>21) truncHistory();
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
            break;

        }
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
        exit(EXIT_FAILURE);
    }
        
    //otwarcie/stworzenie w nim pliku
    global_hist = open("skorupaHist", O_RDWR | O_TRUNC | O_CREAT, 0777);
    if (global_hist < 0){
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
    h_lines--;
    initHistory();

    return 1;


}