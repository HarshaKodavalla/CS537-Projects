///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             forkDelegator.h
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
#include "./hashTab.h"
#include "./depgraph.h"
#ifndef FORKDELEGATOR_H
#define FORKDELEGATOR_H

/**
 * Does a post order traversal of a target's dependency tree,
 * starting at the given index. Checks if the target has any dependencies
 * that have more recent modification times. If so, fork the targets commands.
 *
 * @param Target * targets - the hash table of targets
 * @param index - the index of the target to start the build
 * @param int numTargets - number of targets
 */
void forkDelegator(Target * targets, int index, int numTargets);

/**
 * Compares a target's mod time with a dependency's mod time.
 *
 * @param Target * targets - hash table of targets
 * @param char * target - name of the target file
 * @param char * dep - name of the dep file
 * @param int numTargets - number of targets
 * @return - returns true if the dep is more recent, false otherwise
 */
bool isModRecent(Target * targets, char * target, char * dep, int numTargets);

/**
 * Checks if a file exists
 *
 * @param * fname - name of the file
 * @return - true if the file exists, false if it doesn't
 */
bool fileExists(char *fname);

/*
 * Prints an array of strings - seperates each string with a space.
 * 
 * @para char ** line - Array of strings to be printed
 */
 void printCmdLine(char ** line);

#endif /* FORKDELEGATOR_H */