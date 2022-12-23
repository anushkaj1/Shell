#include "pwdfn.h"

void pwdf(char* piece) {
    if (piece != NULL)  printf("\033[0;31mpwd: too many arguments\033[0;37m\n");   //red

    char pwd[PATH_MAX];
    getcwd(pwd, sizeof(pwd));
    printf("%s\n", pwd);   //white
}
