///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             fork.c
// Semester:         CS 537 Fall 2018
//
// Author:           Harsha Kodavalla kodavalla@wisc.edu
// CS Login:         harsha
// Lecturer's Name:  Miller
// Lab Section:      303
//
//////////////////// PAIR PROGRAMMERS COMPLETE THIS SECTION ////////////////////
//
// Pair Partner:     Julius P. Brendle
// Email:            brendle@wisc.edu
// CS Login:         brendle
// Lecturer's Name:  Miller
// Lab Section:      302
//
//////////////////// STUDENTS WHO GET HELP FROM OTHER THAN THEIR PARTNER //////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide //////////////////////////////////
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "./fork.h"
#include "./forkDelegator.h"

/**
 * Forks a child process which executes a command.
 *
 * @param **line - A string array containing the command to be executed
 */
void forkProcess(char **line){
    printCmdLine(line);
    int fValue = fork();
    pid_t cpid;
    int status;

    if(fValue == 0) {
        // child process executes this
        execvp(line[0], line);
        perror("Error executing command");
        exit(1);
    } else if (fValue == -1) {
        perror("Error forking process");
        exit(1);
    } else {
        // parent process executes this
        cpid = wait(&status);
        (void)cpid; // Remove compiler warnings
        
    }
    if (status != 0) {
        printf("This command failed: ");
        printCmdLine(line);
        exit(1);
    }
}


