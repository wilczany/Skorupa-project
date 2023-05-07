#ifndef _fork_h
#define _fork_h

typedef struct pipes_struct{
    int size;
    int potoki[];
} P_S;

int sProgramForeground(const char* progName, char *const args[], P_S *p, int seq);

int sProgramBackground(const char* progName, char *const args[]);

void pipes_handler(char *progs);

void przekierowanie_xd(char *prog, char *const args[], P_S *p, char *path);

#endif