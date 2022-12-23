#include "bg.h"

void bg(char* comm, char** command, int* processids, int processno, int* pr_stats) {
    int pid = fork();
    int x;
    if (pid == 0) {
        setpgid(0,0);
        processids[processno] = x = getpid();
        printf("[%d]\t%d\n", processno+1, processids[processno]);
        fflush(stdout);
        pr_stats[processno] = 1;
        int err = execvp(comm, command);
        if (err == -1)  printf("\033[0;31mcould not find command to execute\033[0;37m\n");    //red
        return;
    }
}
