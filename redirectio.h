#include "headers.h"

#ifndef IO_H_
#define IO_H_

int iofiles(char* tokand, char* rest, char* infile, char* outfile);
void dup_out(char* file, int appendf);
void dup_in(char* file);

#endif