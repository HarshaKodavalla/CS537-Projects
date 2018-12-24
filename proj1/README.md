## 537ps - Implementing a simple version of the ps command

This program will display information about all processes belonging to the user who invokes the program. 
It can also display information about a specific process if the -p option is activiated and a valid pid is given. 

It will display:
- single-character state information, option -s which defaults to false. 
- amount of user time consumed by the process, option -U which defaults to true. -U- turns this off.
- amount of system time conusmed, option -S which defaults to false. 
- amount of virtual memory currently being used in pages, option -v which defaults to false.
- the command-line that started the program, option -c which defaults to true. -c- turns this off.

You will need the following files:
- makefile
- main.c
- processopts.c
- parser.c
- searchproc.
- ps537.h

To run the program:

1. >make compile
2. >537ps

Usage: 537ps [-p <pid>] [-s] [-U-] [-S] [-v] [-c-]

Authors:

Harsha Kodavalla
Julius P. Brendle

Copyright 2018
