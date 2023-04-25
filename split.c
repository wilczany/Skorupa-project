#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* readLine(){
    char *buf= malloc(sizeof(char)*512);
    int count= 0;
    
    for (;;){
        int c = fgetc (stdin);
        if (c == '\n'){
            buf[count++] = 0;
            break;
        }
        buf[count++] = (char)c;

    }
    return buf;
}

char **separate(char *args){
    char** result = malloc(sizeof(char*));
    char* piece = strtok(args, " ");
    int i = 0;
    while(piece != NULL){
        result = realloc(result, (i+1) * sizeof(char*));
        result[i] = piece;
        piece = strtok(NULL, " ");
        i++;
    }
    result[i]= 0;
    return result;
    }