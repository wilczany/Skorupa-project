#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    result[i]= NULL;
    return result;
    }