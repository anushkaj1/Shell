#include "prompt.h"

char* makeprompt(char* prompt, char* directory, int time_spent) {
    printf("\033[0;36m%s", prompt);   //cyan
    printf("\033[0;32m%s", directory);   //green
    if (time_spent >= 1)    printf(" took %ds", time_spent);
    printf("\033[0;36m>");   //cyan
    time_spent = 0;

    printf("\033[0;37m");   //white
    size_t bufsize = 200;
    char *s = (char*) malloc(sizeof(char) * bufsize);
    size_t characters;
    //scanf("%[^\n]%*c", s);

    characters = getline(&s, &bufsize, stdin);

    return s;
}
