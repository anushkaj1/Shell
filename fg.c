#include "fg.h"

int fgfn(int* fg, char* comm, char* command[20]) {
    time_t begin, end;
    int time_spent = 0;
    *fg = 1;

    int pid = fork();

    begin = time(NULL);
    if (pid == 0) {
        int err = execvp(comm, command);
        if (err == -1)  printf("\033[0;31mcould not find command to execute\033[0;37m\n");    //red
    }
    else {
        int status;
        //waitpid(pid, &status, WUNTRACED);
        wait(&status);
        end = time(NULL);
                        
        if (WIFEXITED(status)) {
            int statuscode = WEXITSTATUS(status);
            if (statuscode != 0)    {
                perror("\033[0;31m");   //red
                printf("\033[0;37m");   //white
            }
        }
        *fg = 0;

        time_spent = (end - begin);
    }

    return time_spent;
}
