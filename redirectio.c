#include "redirectio.h"

//overwritten for > and appended for >>
int iofiles(char* tokand, char* rest, char* infile, char* outfile) {
    //char* piece = (char*) malloc(sizeof(char) * BUFFER);
    int i = 0;
    while(i != strlen(tokand) && tokand[i] != '>' && tokand[i] != '<') {
        rest[i] = tokand[i];
        i++;
    }
    int appendf;

    if (i != strlen(tokand)) {
        if (tokand[i] == '>') {
            if (tokand[i+1] == '>') {
                appendf = 1;
                i++;
            }
            else appendf = 0;
            i++;

            char* x = (char*) malloc(sizeof(char) * 50);
            int j = 0;
            while(i != strlen(tokand)) {
                x[j] = tokand[i];
                i++;
                j++;
            }

            char* o = strtok_r(x, " <", &x);
            char* i = strtok_r(x, " <", &x);

            if (o == NULL)  outfile = NULL;
            else    strcpy(outfile, o);
            if (i == NULL)  infile = NULL;
            else    strcpy(infile, i);
        }
        else if (tokand[i] == '<') {
            i++;
            char* x = (char*) malloc(sizeof(char) * 50);
            int j = 0;
            while(i != strlen(tokand)) {
                x[j] = tokand[i];
                i++;
                j++;
            }

            char* i = strtok_r(x, " >", &x);
            char* o = strtok_r(x, " >", &x);
            
            if (i == NULL)  infile = NULL;
            else    strcpy(infile, i);
            if (o == NULL)  outfile = NULL;
            else    strcpy(outfile, o);
        }
    }
    else {
        infile = NULL;
        outfile = NULL;
    }

    return appendf;
}

void dup_out(char* file, int appendf) {
    int f;
    if (appendf == 0) {
        f = open(file, O_WRONLY | O_CREAT | O_EXCL, 0644);
        if (f == -1 && EEXIST == errno) {
            f = open(file, O_WRONLY);
        }
    }
    else {
        f = open(file, O_WRONLY | O_CREAT | O_EXCL | O_APPEND, 0644);
        if (f == -1 && EEXIST == errno) {
            f = open(file, O_WRONLY | O_APPEND);
        }
    }
    
    if (f < 0) {
        perror("\033[0;31m");   //red
        printf("\033[0;37m");
    }
    dup2(f, STDOUT_FILENO);
    close(f);
}

void dup_in(char* file) {
    int f = open(file, O_RDONLY | O_EXCL);
    if (f < 0) {
        perror("\033[0;31m");   //red
        printf("\033[0;37m");
    }
    dup2(f, STDIN_FILENO);
    close(f);
}
