///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Title:            537ps
// Files:            main.c, parser.c, searchproc.c, processopts.c, ps537.h
// Semester:         CS 537 Fall 2018
//
// Author:           Harsha Kodavalla
// Email:            kodavalla@wisc.edu
// CS Login:         harsha
// Lecturer's Name:  Miller
// Lab Section:      303
//
//////////////////// PAIR PROGRAMMERS COMPLETE THIS SECTION ////////////////////
//
//                   CHECK ASSIGNMENT PAGE TO see IF PAIR-PROGRAMMING IS ALLOWED
//                   If pair programming is allowed:
//                   1. Read PAIR-PROGRAMMING policy (in cs302 policy) 
//                   2. choose a partner wisely
//                   3. REGISTER THE TEAM BEFORE YOU WORK TOGETHER 
//                      a. one partner creates the team
//                      b. the other partner must join the team
//                   4. complete this section for each program file.
//
// Pair Partner:     Julius P. Brendle
// Email:            brendle@wisc.edu
// CS Login:         brendle
// Lecturer's Name:  Miller
// Lab Section:      (your partner's lab section number)
//
//////////////////// STUDENTS WHO GET HELP FROM OTHER THAN THEIR PARTNER //////
//                   must fully acknowledge and credit those sources of help.
//                   Instructors and TAs do not have to be credited here,
//                   but tutors, roommates, relatives, strangers, etc do.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide //////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "ps537.h"

/**
 * Runs the program. Instantiates and intializes the options class. Calls
 * various functions to exectute the ps command.
 *
 * @param int argc - the number of command line arguments
 * @param char *argv - the command line arguments
 */
int main(int argc, char *argv[]) {
    // Contains the command line options specified by the user
  Options *options = (Options *)malloc(sizeof(Options));
    if (options == NULL) {
        printf("Malloc error. Terminating program.\n");
        exit(1);
    }

    // Get and store command line options
    processOpts(argc, argv, options);
    
    if (options->pidDisplay) {
        // Print information for single process
        searchPID(options);
    } else {
        // Print information for all processes belonging to user
        searchProc(options);
    } 

    free(options);

    return 0;
}
