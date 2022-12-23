#include "cd.h"

int cd(char* locn, char* cwd, char* directory, char* memory) {
    int x = 0;
    if (locn == NULL || strcmp(locn, "~") == 0) x = chdir(cwd);
    else if (strcmp(locn, "-") == 0) {
        x = chdir(memory);
        printf("%s\n", memory);
    }
    else    x = chdir(locn);
    return x;
}

char* chpath(char* cwd) {
    char* dir = (char*) malloc(sizeof(char) * 200);
    
    char pwd[PATH_MAX];
    getcwd(pwd, sizeof(pwd));

    int p = strlen(pwd), c = strlen(cwd);

    if (c > p)  strcpy(dir, pwd);
    else {
        int flag = 0;
        for(int i = 0; i < c; i++) {
            if (pwd[i] != cwd[i]) {
                flag = 1;
                break;
            }
        }
        if (flag == 1)  strcpy(dir, pwd);
        else {
            char dir1[100] = "";
            strncpy(dir1, pwd + c, p);
            strcpy(dir, "~");
            strcat(dir, dir1);
        }
    }

    return dir;
}
