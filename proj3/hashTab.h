///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             hashTab.h
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
// Online sources:
//////////////////////////// 80 columns wide //////////////////////////////////

#include <stdbool.h>
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
#ifndef HASHTAB_H
#define HASHTAB_H

/**
 * Represents a single target in the makefile
 */
typedef struct Target {
    char *name;
    // A list of the name of all the target's dependencies
    char **deps;
    int numDeps;
    // A list of all the target lines associated with the target
    char ***cmds;
    int numCmds;
    // A list representing the dependency graph for a single target.
    // Contains the index, in the hash table, of every target
    // dependency for this target.
    int *adjList;
    int numAdj;
    bool built;
    // Used when checking for cycles
    bool visited;
    // Used when searching for duplicates
    bool dupCheck;
} Target;

/**
 * Creates the hash table containing all the targets. Implemented as a dictionary,
 * utilizing linear probing. 
 *
 * @param char ***lines - lines representing makefile
 * @param int numlines - number of lines
 * @param int numTargets - number of targets
 */
Target * initHashTable(char ***lines, int numLines, int numTargets);

/**
 * The djb2 hash function by Dan Bernstein
 *
 * @param char * key - String to be hashed
 * @return - the result of the hash
 */
unsigned long long hash(char * name);

/**
 * Searches the hash table for a particular target.
 *
 * @param Target * targets - hash table of targets
 * @param char * name - the name of a target to be found.
 * @param int numTargets - number of targets
 * @return - Returns the index of the target if it was found. -1 if 
 * it wasn't found.
 */
int searchHashTab(Target * targets, char * name, int numTargets);

#endif /* HASHTAB_H */