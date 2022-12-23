#include "headers.h"

#ifndef PROCESSES_H_
#define PROCESSES_H_

struct process {
    int pro_no;
    int pro_id;
    char* pro_name;
    struct process* next;
    struct process* head;
};

#endif