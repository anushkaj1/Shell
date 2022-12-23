#include "bg.h"
#include "cd.h"
#include "discover.h"
#include "fg.h"
#include "headers.h"
#include "history.h"
// #include "jobs.h"
#include "ls.h"
#include "pipeline.h"
#include "prompt.h"
#include "pwdfn.h"
#include "redirectio.h"

#define BUFFER 200

int fg = 0;
char processnames[5000][100];
int processids[5000];
int processno = 0;
int pr_stats[5000];
char cwd[PATH_MAX];
char directory[100];
int lines;
int time_spent;
int leave;
int pipeline;

// void handle_sigtstp(int sig) {
//     if (fg == 0)    return;
//     int id = getpid();
//     kill(SIGTSTP, getpid);
//     fg = 0;

// }

void loop_shell(char* rest, char* mem, int history) {
    char *piece = strtok_r(rest, " \t", &rest);
    while(piece != NULL) {
        //pwd command
        if (strcmp(piece, "pwd") == 0) {
            piece = strtok_r(rest, " ", &rest);
            pwdf(piece);
        }

        //cd command
        else if (strcmp(piece, "cd") == 0) {
            //for memory
            char pwd2[PATH_MAX];
            getcwd(pwd2, sizeof(pwd2));

            char *piece1 = strtok_r(rest, " ", &rest);
            if (piece1 != NULL) piece = strtok_r(rest, " ", &rest);
            else    piece = NULL;

            //error
            if (piece != NULL){
                printf("\033[0;31mcd: string not in pwd: %s\033[0;37m\n", piece1);   //red
                break;
            }

            //actual computation
            int succ = cd(piece1, cwd, directory, mem);
            if (succ == -1) {
                perror("\033[0;31mjezsh");  //red
                printf("\033[0;37m");    //white
            }   //red
            else    strcpy(directory, chpath(cwd));

            strcpy(mem, pwd2);
        }

        //echo command
        else if (strcmp(piece, "echo") == 0) {
            piece = strtok_r(rest, " ", &rest);
            while (piece != NULL) {
                //printf("%s ", piece);
                strcat(piece, " ");
                write(STDOUT_FILENO, piece, strlen(piece));
                piece = strtok_r(rest, " ", &rest);
            }
            printf("\n");
        }

        //ls command
        else if (strcmp(piece, "ls") == 0) {
            //handling flags
            int flags = 0, flagl = 0, flaga = 0;      //1 for l, 2 for a, 3 for la

            char **directories = (char**) malloc(sizeof(char*) * BUFFER);
            int i = 0;

            //finding directories and flags
            piece = strtok_r(rest, " ", &rest);
            while (piece != NULL) {
                if (strcmp(piece, "-l") == 0)   flagl = 1;
                else if (strcmp(piece, "-a") == 0) flaga = 1;
                else if (strcmp(piece, "-la") == 0 || strcmp(piece, "-al") == 0)    flagl = flaga = 1;
                else {
                    directories[i] = piece;
                    i++;
                }
                piece = strtok_r(rest, " ", &rest);
            }

            if (flagl == 1) flags += 1;
            if (flaga == 1) flags += 2;

            //calling function
            lsall(directories, flags, cwd, i);
        }

        //discover command
        else if (strcmp(piece, "discover") == 0) {
            int x = 0;
            //handling flags
            int flags = 0, flagd = 0, flagf = 0;      //1 for d, 2 for f, 3 for df
                    
            char dir[100] = "";
            char fil[100] = "";

            //finding directories and flags
            piece = strtok_r(rest, " ", &rest);
            while (piece != NULL && piece[0] != ';') {
                if (strcmp(piece, "-f") == 0)   flagf = 1;
                else if (strcmp(piece, "-d") == 0)   flagd = 1;
                else if (strcmp(piece, "-df") == 0 || strcmp(piece, "-fd") == 0)   flagd = flagf = 1;
                else if (piece[0] == '"') {
                    if (strlen(fil) == 0)    strcpy(fil, piece);
                    else x = -1;
                }
                else {
                    if (strlen(dir) == 0)    strcpy(dir, piece);
                    else x = -1;
                }
                piece = strtok_r(rest, " ", &rest);
            }

            if (flagf == 1) flags += 2;
            if (flagd == 1) flags += 1;

            if (x == -1) {
                printf("\033[0;31mtoo many arguments\033[0;37m\n");   //red
                break;
            }
                    
            char* passit = (char*) malloc(sizeof(char) * BUFFER);
            if (strcmp(dir, "") == 0)    strcpy(dir, ".");
            strcpy(passit, dir);
            discover(flags, dir, fil, cwd, passit);
        }

        //history command
        else if (strcmp(piece, "history") == 0) {
            piece = strtok_r(rest, " ", &rest);
            if (piece != NULL) {
                while (piece != NULL) piece = strtok(NULL, " ");
                printf("\033[0;31mtoo many arguments\033[0;37m\n");     //red
                break;
            }
            printlines(lines, history);
        }

        //jobs command
        else if (strcmp(piece, "jobs") == 0) {
            int flags = 0;
            piece = strtok_r(rest, " ", &rest);

            //flag = 1 means running only
            if (piece != NULL) {
                if (strcmp(piece, "-r") == 0)   flags = 1;
                else if (strcmp(piece, "-s") == 0)  flags = 2;
                piece = strtok_r(rest, " ", &rest);
            }

            for (int i = 0; i < processno; i++) if (pr_stats[i] != -1) {
                if (flags != 1 && pr_stats[i] == 0) {
                    printf("[%d] Stopped %s [%d]\n", i+1, processnames[i], processids[i]);
                }
                else if (flags != 2 && pr_stats[i] == 1) {
                    printf("[%d] Running %s [%d]\n", i+1, processnames[i], processids[i]);
                }
            }
        }

        //sig command
        else if (strcmp(piece, "sig") == 0) {
            piece = strtok_r(rest, " ", &rest);
            if (piece == NULL) {
                printf("\033[0;31msig: too few arguments\033[0;37m\n");     //red
                break;
            }
            int sig_to_id = atoi(piece);
            if (sig_to_id > processno || pr_stats[sig_to_id] == -1) {
                printf("\033[0;31mprocess id does not exist\033[0;37m\n");     //red
                break;
            }
            int sig_num = atoi(piece);
            piece = strtok_r(rest, " ", &rest);
            if (piece == NULL) {
                printf("\033[0;31msig: too few arguments\033[0;37m\n");     //red
                break;
            }
            piece = strtok_r(rest, " ", &rest);
            if (piece != NULL) {
                printf("\033[0;31msig: too many arguments\033[0;37m\n");     //red
                break;
            }
            kill(sig_num, processids[sig_to_id-1]);
        }

        // //fg command
        // else if (strcmp(piece, "fg") == 0) {
            
        // }

        //bg command
        else if (strcmp(piece, "bg") == 0) {
            piece = strtok_r(rest, " ", &rest);
            if (piece == NULL) {
                printf("\033[0;31msig: too few arguments\033[0;37m\n");     //red
                break;
            }
            int bg_id = atoi(piece);
            piece = strtok_r(rest, " ", &rest);
            if (piece != NULL) {
                printf("\033[0;31msig: too many arguments\033[0;37m\n");     //red
                break;
            }
            kill(SIGCONT, processids[bg_id-1]);
        }

        //exiting the program
        else if (strcmp(piece, "exit") == 0) {
            leave = 1;
            break;
        }

        //system commands/wrong command
        else {
            char* command[20];
            char* comm = piece;
            int i = 0;

            while (piece != NULL) {
                command[i] = piece;
                i++;
                piece = strtok_r(rest, " ", &rest);
            }
            command[i] = NULL;
                    
            time_spent = fgfn(&fg, comm, command);
        }
    }

    return;
}

void createbg(char* tokand) {
    char *rest = (char*) malloc(sizeof(char) * BUFFER);
    strcpy(rest, tokand);

    char *piece = strtok_r(rest, " \t", &rest);
    char* command[20];
    char* comm = piece;
    int i = 0;

    while (piece != NULL) {
        command[i] = piece;
        i++;
        piece = strtok_r(rest, " ", &rest);
    }

    command[i] = NULL;
    strcpy(processnames[processno], comm);
    bg(comm, command, processids, processno, pr_stats);
    processids[processno] = getpid();
    pr_stats[processno] = 1;
    processno++;
    //printf("%d %s %d %d\n", processno, processnames[processno-1], processids[processno-1], pr_stats[processno-1]);
}

void sigchild_end() {
    if (fg == 1) return;
    int status;
    int pid = waitpid(-1, &status, WNOHANG);

    if (pid > 0) {
        int i = 0;
        while (processids[i] != pid && i < processno - 1) {
            //printf("%d\n", processids[i]);
            i++;
        }

        if (WIFSTOPPED(status)) return;

        char name[100];
        strcpy(name, processnames[i]);

        if (WIFEXITED(status)) printf("%s with pid %d exited normally\n", name, pid);
        else printf("%s with pid %d exited abnormally\n", name, pid);

        pr_stats[i] = -1;

        // while (i != processno - 1) {
        //     strcpy(processnames[i], processnames[i+1]);
        //     processids[i] = processids[i+1];
        //     pr_stats[i] = pr_stats[i+1];
        // }

        // processno--;
    }
}


int main()
{
    //making shell prompt
    char hostname[256];
    int host = gethostname(hostname, sizeof(hostname));

    struct passwd *systems;
    systems = (struct passwd*) malloc(sizeof(struct passwd));
    uid_t uid = getuid();
    systems = getpwuid(uid);

    getcwd(cwd, sizeof(cwd));

    char prompt[200] = "";
    strcat(prompt, "<");
    strcat(prompt, systems->pw_name);
    strcat(prompt, "@");
    strcat(prompt, hostname);
    strcat(prompt, ":");

    strcpy(directory, "~");
    char mem[100];
    strcpy(mem, cwd);

    //making history file
    int history = open("history.txt", O_RDWR | O_CREAT, 0777);
    int sizeoffile = lseek(history, 0, SEEK_END);
    if (sizeoffile == 0)    write(history, "00\n", strlen("00\n"));
    lseek(history, 0, 0);

    //finding out current number of lines in history
    lines = getlines(history);

    leave = 0;
    time_spent = 0;

    while(1) {
        signal(SIGCHLD, sigchild_end);

        // struct sigaction sa;
        // sa.sa_handler = &handle_sigtstp;
        // sa.sa_flags = SA_RESTART;
        // sigaction(SIGTSTP, &sa, NULL);

        pipeline = 0;   // 0-no pipe, 1-outgoing pipe, 2-incoming pipe, 3-outgoing+incoming pipe
        
        //printing prompt and taking new input
        char *s = makeprompt(prompt, directory, time_spent);    //remember strings contain a '\n' at end
        char *s1 = (char*) malloc(sizeof(char) * BUFFER);
        char *s2 = (char*) malloc(sizeof(char) * BUFFER);
        int x = strlen(s);
        strncpy(s1, s, x-1);
        strcpy(s2, s1);
        lines = addlines(lines, s2, history);

        char *token = strtok(s1, ";");
        time_spent = 0;

        while(token != NULL) {
            printf("\033[0;37m");   //white
            char *infile = (char*) malloc(sizeof(char) * 100);
            char *outfile = (char*) malloc(sizeof(char) * 100);
            int appendf = 0;

            char* rest2 = (char*) malloc(sizeof(char) * BUFFER);
            strcpy(rest2, token);
            char* tokand = strtok_r(rest2, "&", &rest2);
            int length = strlen(tokand);
            
            //finding bg processes
            while(tokand != NULL && length != strlen(token)) {
                createbg(tokand);
                tokand = strtok_r(rest2, "&", &rest2);
                if (tokand == NULL) length += 1;
                else    length += strlen(tokand) + 1;
            }
            free(rest2);

            // //pipelining
            // if (pipeline == 1 || pipeline == 3) pipeline = 2;   //incoming pipe
            // else    pipeline = 0;

            // char* piper = (char*) malloc(sizeof(char) * BUFFER);
            // strcpy(piper, tokand);
            // char* piped = strtok_r(piper, "|", &piper);
            // if (strlen(piped) != strlen(tokand)) pipeline++;     //there's an outgoing pipe
            // //pipeline = find_pipe_num(pipeline, tokand);
            // printf("%s %d\n", piped, pipeline);
            
            //taking care of input and output redirection
            char *rest = (char*) malloc(sizeof(char) * BUFFER);
            if (tokand == NULL) rest = NULL;
            else appendf = iofiles(tokand, rest, infile, outfile);


            int old_stdin = dup(STDIN_FILENO);
            int old_stdout = dup(STDOUT_FILENO);

            // int dealer = 0;
            // if (pipeline == 2 || pipeline == 3) {
            //     strcpy(infile, "text.txt");
            // }
            if (strlen(infile) > 0)     dup_in(infile);

            // if ((pipeline == 1 || pipeline == 3)) {
            //     strcpy(outfile, "text.txt");
            // }
            if (strlen(outfile) > 0)    dup_out(outfile, appendf);

            if (infile != NULL) free(infile);
            if (outfile != NULL) free(outfile);

            //actual stuff
            loop_shell(rest, mem, history);
 
            dup2(old_stdout, STDOUT_FILENO);
            dup2(old_stdin, STDIN_FILENO);

            close(old_stdin);
            close(old_stdout);

            token = strtok(NULL, ";|");
            free(rest);
            if (leave == 1) break;
        }

        free(token);
        if (leave == 1) break;
    }

    close(history);

    return 0;
}
