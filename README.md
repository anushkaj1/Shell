# OSN
# Assignment 3 - Mid Submission
## Anushka Jain
## 2021111008

- This is an implementation of the command line. Various shell commands have been coded manually.
- The coded commands can be executed to implement in a way similar to that of the Linuz shell.
- To execute the program, run the command `make` followed by `./jezsh`.
- The program contains all the functionality required for assignment 2 and also supports
input/oputput redirection, i.e. specification 1 for assignment 3.

## bg.c
Creates a new background process.

## cd.c
Handles all the functionalities of the `cd` command. The command changes location to the specified directory.

## discover.c
Handles all the functionalities of the `discover` command. Shows all the cascading files and folders present inside the selected directory. Flag `-d` can be used to print only directories and `-f` to print only files.

## fg.c
Creates new foreground process, used to run commands executed by `execvp`.

## history.c
Handles all the functionalities of the `history` command and stores data in a file called `history.txt`. When the history command is called, the latest 10 commands given to the shell are printed.

## ls.c
Handles all the functionalities of the `ls` command. Shows contents of the specified directory. Flag `-l` can be used to show permissions given to the files. Flag `-a` can be used to display hidden files as well.

## prompt.c
Creates the shell prompt which will be printed in the terminal.

## pwdfn.c
Handles all the functionalities of the `pwd` command. Prints the path to the current directory.

## redirectio.c
Takes care of input/output redirection to files. 

## shell.c
This file contains the `main()` function.
