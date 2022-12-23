#include "ls.h"
#include "cd.h"

int comparator(const void *p, const void *q) {
    const char *s1 = ((struct dirent *)p)->d_name;
    const char *s2 = ((struct dirent *)q)->d_name;
    return (strcmp(s1, s2));
}

int ls(char* tolist, int flags, char* cwd) {
    struct passwd *pw;
    struct group *gp;
    struct stat file_stat;
    struct dirent *d;
    DIR *dir;
    char pwd[100] = "", *bleh, *bleh2;
    getcwd(pwd, sizeof(pwd));
    int x = 0;

    if (tolist != NULL) x = cd(tolist, cwd, bleh, bleh2);
    if (x == -1) {
        printf("\e[0;31m%s : directory not found\033[0;37m\n", tolist);
        return -1;
    }

    dir = opendir(".");

    d = readdir(dir);
    while(d != NULL) {
        //taking care of -a
        if ((flags == 0 || flags == 1) && d->d_name[0] == '.');
        else {
            stat(d->d_name, &file_stat);

            if (S_ISDIR(file_stat.st_mode))  printf("\033[0;35m"); //purple for directories
            else if (file_stat.st_mode & S_IXUSR) printf("\e[0;31m"); //red for executables
            else printf("\033[0;37m");      //white for files

            if (flags == 0 || flags == 2)   printf("%s\n", d->d_name);
            else {
                (S_ISDIR(file_stat.st_mode)) ? printf("d") : printf("-");
                (file_stat.st_mode & S_IRUSR) ? printf("r") : printf("-");
                (file_stat.st_mode & S_IWUSR) ? printf("w") : printf("-");
                (file_stat.st_mode & S_IXUSR) ? printf("x") : printf("-");
                (file_stat.st_mode & S_IRGRP) ? printf("r") : printf("-");
                (file_stat.st_mode & S_IWGRP) ? printf("w") : printf("-");
                (file_stat.st_mode & S_IXGRP) ? printf("x") : printf("-");
                (file_stat.st_mode & S_IROTH) ? printf("r") : printf("-");
                (file_stat.st_mode & S_IWOTH) ? printf("w") : printf("-");
                (file_stat.st_mode & S_IXOTH) ? printf("x") : printf("-");
                printf(" ");

                printf("%d ",file_stat.st_nlink);
                pw=getpwuid(file_stat.st_uid);
                printf("%s ",pw->pw_name);
                gp=getgrgid(file_stat.st_gid);
                printf("%s ",gp->gr_name);
                printf("%4lld ",file_stat.st_size);
                char *c = ctime(&file_stat.st_mtime);
                for(int i = 4; i <= 15; i++)    printf("%c", c[i]);
                printf(" ");

                printf(" %s\n", d->d_name);
            }
        }
        d = readdir(dir);
    }

    printf("\033[0;37m");
    cd(pwd, cwd, bleh, bleh2);

    return 0;
}


int lsall(char** tolist, int flags, char* cwd, int size) {
    struct passwd *pw;
    struct group *gp;
    struct stat file_stat;
    struct dirent *d;
    DIR *dir;

    if (size == 0)  (ls(NULL, flags, cwd));
    else {
        for(int i = 0; i < size; i++) {
            ls(tolist[i], flags, cwd);
        }
    }

    return 0;
}
