#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_ARGUMENT_COUNT 32

char* readLine(int *status, int *ct){
    char *buf= malloc(sizeof(char)*512);
    int count= 0;
    size_t bytes_read; 
    char c;

    for (;;){
        bytes_read = read(STDIN_FILENO, &c, 1);
        if (bytes_read == 0 && count == 0){
            exit(EXIT_SUCCESS);
        }
        if( c == '\n'){
            buf[count++] = '\0';
            break;
        }
        buf[count++] = c;

    }
    *ct = count;
    return buf;
}

char **separate (char *args, int *count, char *delim){
    char** result = malloc(sizeof(char*) * MAX_ARGUMENT_COUNT);
    char* piece = strtok(args, delim);
    
    while(piece != NULL){
    
        result = realloc(result, ((*count)+1) * sizeof(char*));
        result[(*count)] = piece;
        piece = strtok(NULL, delim);
        (*count)++;
        
    }
    result[(*count)]= NULL;
    return result;
    }