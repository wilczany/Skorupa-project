#ifndef _read_h
#define _read_h

char *readLine(int *status);

char **separate(char *args, int* count, char *delim);

char **seperatePipe(char *args,int *count);
#endif