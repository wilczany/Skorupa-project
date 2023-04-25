#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* readLine(){
    char *buf= malloc(sizeof(char)*512);
    int count= 0;
    
    for (;;){
        int c = fgetc (stdin);
        if (c == '\n'){
            buf[count++] = '\0';
            break;
        }
        buf[count++] = (char)c;

    }
    return buf;
}

char **separate (char *args, int *count){

    char** result = malloc(sizeof(char*));
    char* piece = strtok(args, " ");
    
    while(piece != NULL){
        result = realloc(result, ((*count)+1) * sizeof(char*));
        result[(*count)] = piece;
        piece = strtok(NULL, " ");
        (*count)++;
        
    }
    result[(*count)]= 0;
    return result;
    }