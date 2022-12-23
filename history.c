#include "history.h"

int getlines(int history) {
    char c;
    read(history, &c, 1);
    int i = -1;

    while(c != '\n') {
        i++;
        read(history, &c, 1);
    }

    lseek(history, 0, 0);
    int size = 0;

    read(history, &c, 1);
    while(c != '\n') {
        int x = c - 48;
        size += x * pow(10, i);
        i--;
        read(history, &c, 1);
    }

    return size;
}

void printlines(int lines, int history) {
    //printf("here\n");
    //print in opposite order maybe if time permits
    lseek(history, 0, 0);
    char c;
    do read(history, &c, 1);
    while (c != '\n');
    
    for(int i = 1; i < lines + 1; i++) {
        if (i > lines - 10) printf("%d ", i);
        do {
            read(history, &c, 1);
            if (i > lines - 10) printf("%c", c);
        }
        while (c != '\n');
    }
}

int addlines(int lines, char* s, int history) {
    //checking if the last statement was the same
    char command[100] = "";
    lseek(history, 0, 0);
    for(int i = 0; i < lines+1; i++) {
        strcpy(command, "");
        char* c = (char*) malloc(sizeof(char));
        
        read(history, c, 1);
        while (strcmp(c, "\n") != 0) {
            strcat(command, c);
            read(history, c, 1);
        }
    }

    if (strcmp(command, s) == 0)    return lines;

    lseek(history, 0, SEEK_END);
    write(history, s, strlen(s));
    write(history, "\n", 1);

    if (lines == 20) {
        lseek(history, 0, 0);
        char c;
        do read(history, &c, 1);
        while (c != '\n');

        do {
            read(history, &c, 1);
            lseek(history, -1, SEEK_CUR);
            write(history, "", 1);
        }
        while (c != '\n');
    }
    else {
        lines++;
        int x = lines / 10;
        char c = x + '0';
        lseek(history, 0, 0);
        write(history, &c, 1);
        x = lines % 10;
        c = x + '0';
        write(history, &c, 1);
    }
    
    return lines;
}
