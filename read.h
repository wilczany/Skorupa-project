#ifndef _read_h
#define _read_h

char *readLine(int *status, int *ct);

char **separate(char *args, int* count, char *delim);

char *trim(char *toTrim); 
#endif