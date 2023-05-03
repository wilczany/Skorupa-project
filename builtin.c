#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>

#include <errno.h>

#include <sys/types.h>
#include <pwd.h>

#include "builtin.h"

int cd(char **args, int arguments_count){
    char first = args[1][0];
    char sciezka[4096]; //linuksowy limit sciezki to 4095 czy 4096, ale moze to byc leeekki overkill w malym projekcie
    switch(first){
        case '-': fprintf(stderr, "cd: Funkcja powrotu (jeszcze) niedostępna\n"); return 2;
        case '~': strcpy(sciezka,homePath()); break; //roboczo
        default: strcpy(sciezka,args[1]);
    }

    DIR* dir = opendir(sciezka);
    if (dir) {
        if(closedir(dir) == -1){
            fprintf(stderr, "cd, closedir(): %s",strerror(errno));
            return -1;
        }
        if(chdir(sciezka) == -1){
            fprintf(stderr, "cd, chdir(): %s",strerror(errno));
            return -1;
        }
    } else if (ENOENT == errno) { //kod bledu braku podanej sciezki
        fprintf(stderr, "cd: Brak takiej ścieżki: %s\n", sciezka);
        return 0;
    } else {
        fprintf(stderr, "cd, opendir(): %s\n",strerror(errno));
        return -1;
    }    

    return 1;
}


char *homePath(){ //obsluga bledow
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
}