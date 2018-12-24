///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             depgraph.c
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
// Lab Section:      303
//
//////////////////// STUDENTS WHO GET HELP FROM OTHER THAN THEIR PARTNER //////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   djb2 by Dan Bernstein
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
#include "./hashTab.h"
#include "./depgraph.h"

/**
 * Creates the dependency graph by making an adjacency list for each target.
 * The graph's nodes are targets. Non-target dependencies do not appear in the graph.
 *
 * @param Target * targets - hash table of targets
 * @param int numTargets - number of total targets
 */
void initGraph(Target * targets, int numTargets) {
    int index;
    int count;  // Tracks how many target deps are found
    
    // Iterate through the hash table of targets setting the adj list
    for (int i = 0; i < numTargets; i++) {
        count = 0;
        // Check if any of the target's dependencies are targets, and add them
        // to the adjacency list.
        for (int j = 0; j < targets[i].numDeps; j++) {
            index = searchHashTab(targets, targets[i].deps[j], numTargets);
            if (index != -1) {
                // The dependency was found in the table
                targets[i].adjList[count++] = index;
                targets[i].numAdj = count;
            }
        }
    }
}

/**
 * Checks if the graph is cyclic. If it is, the program is terminated.
 * Uses a Depth First Traversal to search for back edges.
 *
 * @param Target * targets - hash table of targets
 * @param int index - the first target's index in the hash table
 */
void checkCyclic(Target * targets, int index) {
    targets[index].visited = true;
    // Check if any of the children have already been visited.
    for (int i = 0; i < targets[index].numAdj; i++) {
        if (targets[targets[index].adjList[i]].visited) {
            printf("Graph contains cycle at this target: %s. ", 
                    targets[targets[index].adjList[i]].name);
            printf("Exiting.\n");
            exit(0);
        }
    }
    // Recursively traverse the graph
    for (int j = 0; j < targets[index].numAdj; j++) {
        checkCyclic(targets, targets[index].adjList[j]);    
    }
    targets[index].visited = false;
}