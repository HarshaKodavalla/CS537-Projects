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
// Online sources:   djb2 hash functon by Dan Bernstein
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

#define INPUTBUFFER 1024    // Maximum allowed line size

/**
 * Creates the hash table containing all the targets. Implemented as a dictionary,
 * utilizing linear probing.
 *
 * @param char ***lines - lines representing makefile
 * @param int numlines - number of lines
 * @param int numTargets - number of targets
 */
Target * initHashTable(char ***lines, int numLines, int numTargets) {
    // Initialize the hash table of targets
    Target * targets = malloc(sizeof(Target) * numTargets);
    if (targets == NULL) {
        printf("Malloc error. Terminating.\n");
        exit(1);
    }
    for (int i = 0; i < numTargets; i++) {
        targets[i].name = NULL; 
        targets[i].deps = malloc(sizeof(char*) * INPUTBUFFER);  
        if (targets[i].deps == NULL) {
            printf("Malloc error. Terminating.\n");
            exit(1);
        }
        targets[i].numDeps = 0; 
        targets[i].cmds = NULL; 
        targets[i].numCmds = 0; 
        targets[i].numAdj = 0;
        targets[i].built = false;
        targets[i].visited = false;
        targets[i].dupCheck = false;
    }

    // Iterates through each line from the makefile. If it is a target
    // add it to the hash table. If it is a command, add it to the associated
    // target's cmds field.
    for (int i = 0; i < numLines; i++) {
        // Hash on the name of the target
        int index = (int)(hash(lines[i][0]) % numTargets);
        // Use linear probing to find an empty bucket
        while(targets[index].name != NULL) {
            // Check if a duplicate target has already been added
            if (strcmp(targets[index].name, lines[i][0]) == 0) {
                printf("Duplicate target: %s. Exiting.\n", lines[i][0]);
                exit(0);
            }
            index = (index + 1) % numTargets;
        }
        targets[index].name = lines[i][0];

        // Set the dependencies by iterating through the lines array
        int j = 2;  // index at where the first dependency appears
        while (lines[i][j] != 0) {
            targets[index].deps[j-2] = lines[i][j];
            j++;
        }
        // Set the number of dependencies
        targets[index].numDeps = j - 2;

        // Allocate memory for the adjacecency list using the number of deps
        targets[index].adjList = malloc(sizeof(char*) * targets[index].numDeps);
        if (targets[index].adjList == NULL) {
            printf("Malloc error. Terminating.\n");
            exit(1);
        }

        if (i == numLines - 1) {
            // End of lines reached
            break;
        }

        // Handle a target's associated commands

        // If the next line contains a command it must be linked to the current target
        if (strncmp(lines[i+1][0], "\t", strlen("\t")) == 0) {
            int temp = i; 
            int j = -1;
            // First while loop counts the number of consecutive commands in lines.
            while (strncmp(lines[i+1][0], "\t", strlen("\t")) == 0) {
                targets[index].numCmds++;
                i++;
                if (i == numLines - 1) {
                    // End of lines reached
                    break;
                }
            }
            i = temp;

            // After counting the number of commands, allocate memory.
            targets[index].cmds = malloc(sizeof(char**) * targets[index].numCmds);
            if (targets[index].cmds == NULL) {
                printf("Malloc error. Terminating.\n");
                exit(1);
            }

            // Second while loop initializes the cmds field by getting 
            // the commands from lines.
            while (strncmp(lines[i+1][0], "\t", strlen("\t")) == 0) {
                i++;
                j++;
		
                // Chop the \t that precedes every command
                char *dst = lines[i][0];
                char *src = dst + 1;
                // Copy the command byte by byte
                while (*src != '\0') {
                    *dst++ = *src++;
                }
                *dst = '\0';    // Append terminator
                // Set the targets commands to point to the newly modified commands
                // in the lines array.
                targets[index].cmds[j] = lines[i];
                if (i == numLines - 1) {
                    break;
                }
            }
        }
    }
    return targets;
}

/**
 * The djb2 hash function by Dan Bernstein
 *
 * @param char * key - String to be hashed
 * @return - the result of the hash
 */
unsigned long long hash(char *key) {
    unsigned long long hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

/**
 * Searches the hash table for a particular target.
 *
 * @param Target * targets - hash table of targets
 * @param char * name - the name of a target to be found.
 * @param int numTargets - number of targets
 * @return - Returns the index of the target if it was found. -1 if 
 * it wasn't found.
 */
int searchHashTab(Target * targets, char * name, int numTargets) {
    int startIndex = (int)(hash(name) % numTargets);
    int i = startIndex;

    // Iterate through the hash table searching for the target
    do {
        if (!strcmp(targets[i].name, name)) {
            return i;
        }
        i = (i + 1) % numTargets;
    } while (i != startIndex);

    return -1;
}

