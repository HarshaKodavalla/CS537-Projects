///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  inputparser.c
// File:             inputparser.h
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
#ifndef PARSER_H
#define PARSER_H


/**
 * Reads a makefile called 'makefile' or 'Makefile' in the same folder
 * this exectuable is located in. Returns the array of makefile lines as arrays.
 *
 * @param int *numLines - a pointer to an int that stores the number of
 *                        content lines (lines that contain makefile)
 * @param int *numTargets - a pointer to an int that store the number of
 *                          targets in the makefile
 * @return char ***makefileLines - return a (clean) array of the makefiles lines
 *                                 as arrays of strings
 */
char *** readMakefileLines(int * numLines, int * numTargets);

/**
 * Reads the next line in the makefile and returns it as an array of strings.
 * Modifies the array by filtering out for a makefile redundant spaces and tabs
 *
 * @param FILE *makefile - pointer to the makefile from which we want to read
 *                         the next line from
 *
 * @return char **makefileLine - returns an array of strings with all
 *                               information containing words in a makefile
 */
char ** readMakefileLine(FILE *makefile);

/**
 * Allocates a new string in memory at returns a pointer to it.
 *
 * @param size_t buffersize - how much memory we want to allocate for the string
 *
 * @return char *allocatedString - returns a pointer to the allocated memory
 *                                 structure
 */
char * allocateString(size_t buffersize);

/*
* Checks if a line is a correct makefile line (contains a tab as the first char
* or a colon as the second element and is thus a target line.
* @param FILE **line - pointer to the line that we check for correctness
*
* @return int - returns 0 if line is incorrect. returns 1 if it is a target
*               line, returns 2 if it is not a command line.
*/
int lineIsCorrect(char **line);

#endif /* PARSER_H */