#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

void pUserDir(){
    
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\ %s", cwd);

}
void main(){
    
    pUserDir();

    return 0;
}