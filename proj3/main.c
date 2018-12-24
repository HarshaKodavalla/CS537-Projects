///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Title:            537ps
// Files:            main.c, depgraph.c, fork.c, forkDelegator.c, hashTab.c
//                   inputparser.c
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
// Lab Section:      302
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
#include "./inputparser.h"
#include "./fork.h"
#include "./hashTab.h"
#include "./depgraph.h"
#include "./forkDelegator.h"

/**
 * Runs the program. Calls the parser, then creates the hash table,
 * then the dependency graph, then builds. If a build specification
 * is requested, it will build only that specification. Otherwise
 * it will build the first specification.
 *
 * @param int argc - the number of command line arguments
 * @param char *argv - the command line arguments
 */
int main(int argc, char** argv) {
    // Read from the makefile and create an array of lines.
    // Each line is an array of strings, representing one line of the 
    // makefile.
    int numLines;
    int numTargets;
    // Representation of the makefile
    char ***lines = readMakefileLines(&numLines, &numTargets);

    // Create a hash table of all the targets
    Target * targets = initHashTable(lines, numLines, numTargets);

    // Create the dependency graph of all the targets
    initGraph(targets, numTargets);

    // Retrieve the index in the hash table of the first build specification
    int index = searchHashTab(targets, lines[0][0], numTargets);

    // To assist with debugging
    if (index == -1) {
        printf("Bug detected when retrieving index of first build spec.\n");
        exit(0);
    }

    // Checks the makefile for cycles. Terminates if one exists.
    checkCyclic(targets, index);

    // If a particular build spec is requested, retrieve the index of that
    // build spec. 
    
    if (argc == 2) {
        // If a command line arg is given, it should be a build spec
        index = searchHashTab(targets, argv[1], numTargets);
        if (index == -1) {
            // User gave invalid input
            printf("Build specification is invalid. Exiting.\n");
            exit(0);
        }
    }
    
    // Build
    forkDelegator(targets, index, numTargets);
}

