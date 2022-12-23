#include "discover.h"
#include "cd.h"

int discover(int flags, char* directory, char* fil, char* cwd, char* toprintpre) {
    char pwd[100] = "", *bleh, *bleh2;
    getcwd(pwd, sizeof(pwd));
    cd(directory, cwd, bleh, bleh2);
    
    DIR *dir = opendir(".");
    struct dirent *d = readdir(dir);
    struct stat file_stat;
    
    while(d != NULL) {
        if (d->d_name[0] == '.') {
            d = readdir(dir);
            continue;
        }

        stat(d->d_name, &file_stat);
        char toprint[100] = "";
        strcpy(toprint, toprintpre);
        strcat(toprint, "/");
        strcat(toprint, d->d_name);

        if (strlen(fil) == 0) {
            if (flags != 2 && S_ISDIR(file_stat.st_mode))   printf("%s\n", toprint);
            if (flags != 1 && !(S_ISDIR(file_stat.st_mode)))    printf("%s\n", toprint);
        }

        else { 
            char check[100] = "";
            strncpy(check, fil+1, strlen(fil) - 2);
            if (strcmp(d->d_name, check) == 0)   printf("%s\n", toprint);
        }
        if (S_ISDIR(file_stat.st_mode)) discover(flags, d->d_name, fil, cwd, toprint);

        d = readdir(dir);
    }

    cd(pwd, cwd, bleh2, bleh);
    return 1;
}
