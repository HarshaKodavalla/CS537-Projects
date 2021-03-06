///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             module.h
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
#include "hashTab.h"
#ifndef GRAPH_H
#define GRAPH_H

/**
 * Creates the dependency graph by making an adjacency list for each target.
 *
 * @param Target * targets - hash table of targets
 * @param int numTargets - number of total targets
 */
void initGraph(Target * targets, int numTargets);

/**
 * Checks if the graph is cyclic. If it is, the program is terminated.
 * Uses a Depth First Traversal to search for back edges.
 *
 * @param Target * targets - hash table of targets
 * @param int index - the first target's index in the hash table
 */
void checkCyclic(Target * targets, int index);

#endif /* GRAPH_H */