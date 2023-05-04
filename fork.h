#ifndef _fork_h
#define _fork_h

int sProgramForeground(const char* progName, char *const args[]);

int sProgramBackground(const char* progName, char *const args[]);

void pipes_handler(char **progs, int count);


#endif