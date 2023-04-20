#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include "fork.h"

void pUserDir(){
    
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\ %s ", cwd);
	
}
int main(){
    char *const nullek[] = {NULL};
    char *const forkc[] = {"more", "fork.c", NULL};
    
    char buf[32] = { 0 };

    while (buf[0] != 'q') {
        pUserDir();
        printf("ps\n");
        printf("\n%i\n",sProgramForeground("ps", nullek));

        fgets(buf, sizeof(buf), stdin);
    }

/*
zrobilem nieskonczona petle do testow funkcji z jakimkolwiek inputem do przejscia dalej 
jesli chodzi o uzycie bufora i fgetsa to odsylam tutaj 
https://stackoverflow.com/questions/15167390/getcstdin-returns-two-characters-is-there-a-better-way-to-handle-this
lub do przetestowania tego jak to dziala z getc(stdin) na ponizszej petli, w skrocie dziala, ale kilkukrotnie
wywal jak informacja dojdzie do ciebie dziena
*/


    // do{
    //     // pUserDir();
    //     // printf("more fork.c\n");
    //     // sProgramForeground("more", forkc);

    //     pUserDir();
    //     printf("ps\n");
    //     printf("\n%i\n",sProgramForeground("ps", nullek));

    //     //pUserDir();
    //     //printf("firefox&\n");
    //     //sProgramBackground("firefox", nullek);

    // }while(getc(stdin)!='q');
    return 0;
}
