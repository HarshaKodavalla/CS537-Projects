///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             forkDelegator.c
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
#include <sys/stat.h>
#include "./forkDelegator.h"
#include "./hashTab.h"
#include "./depgraph.h"
#include "./fork.h"


/**
 * Does a post order traversal of a target's dependency tree,
 * starting at the given index. Checks if the target has any dependencies
 * that have more recent modification times. If so, fork the targets commands.
 *
 * @param Target * targets - the hash table of targets
 * @param index - the index of the target to start the build
 * @param int numTargets - number of targets
 */
void forkDelegator(Target * targets, int index, int numTargets) {
	// If the target has target dependencies, traverse its graph
	if (targets[index].numAdj > 0) {
		for (int j = 0; j < targets[index].numAdj; j++) {
			// Checks if the adjacent node hasn't been built yet
			if (!(targets[targets[index].adjList[j]].built)) {
				forkDelegator(targets, targets[index].adjList[j], numTargets);
			}
		}
	}
	// After the post order traversal, checks ALL dependencies - 
	// non-target and target alike for mod times more recent than the target.
	// If any deps are more recent, run the associated command.
	for (int k = 0; k < targets[index].numDeps; k++) {
		if (isModRecent(targets, targets[index].name, targets[index].deps[k], numTargets)) {
			// Fork each command associated with the target
			for (int k = 0; k < targets[index].numCmds; k++) {
				forkProcess(targets[index].cmds[k]);
			}
			targets[index].built = true;
            return;
		}
	}
	// If the target has no dependencies, run its commands.
	if (targets[index].numDeps == 0) {
		for (int k = 0; k < targets[index].numCmds; k++) {
			forkProcess(targets[index].cmds[k]);
		}
	}
	// Mark the current target as built
	targets[index].built = true;
}

/**
 * Compares a target's mod time with a dependency's mod time.
 *
 * @param Target * targets - hash table of targets
 * @param char * target - name of the target file
 * @param char * dep - name of the dep file
 * @param int numTargets - number of targets
 * @return - returns true if the dep is more recent, false otherwise
 */
bool isModRecent(Target * targets, char * target, char * dep, int numTargets) {
    struct stat targStat;
    struct stat depStat;
    
	// Handle non-existent dependency file
	if (!fileExists(dep)) {
		if (searchHashTab(targets, dep, numTargets) == -1) {
			// If the dependency is a non-target, and doesn't exist, throw error
			printf("Non existent dependency file: %s. Terminating.\n", dep);
			exit(0);
		} else {
			return true;
		}
	} 

	// If the target file doesn't exist, assume it has the more recent mod time
	if (!fileExists(target)) {
		return true;
	}

	// Get the modification time
    if (stat(target, &targStat) == -1
		|| stat(dep, &depStat) == -1) {
		printf("Stat error. Terminating.\n");
	}
    
    long long targTime = (long long)targStat.st_mtime;
    long long depTime = (long long)depStat.st_mtime;

    return targTime < depTime;
}

/**
 * Checks if a file exists
 *
 * @param * fname - name of the file
 * @return - true if the file exists, false if it doesn't
 */
bool fileExists(char *fname) {
	struct stat temp;
	return (stat(fname, &temp) == 0);
}

/*
 * Prints an array of strings - seperates each string with a space.
 * 
 * @param char ** line - Array of strings to be printed
 */
 void printCmdLine(char ** line) {
	 printf("%s", line[0]);
	 int i = 1;
	 while (line[i] != 0) {
		 printf(" %s", line[i++]);
	 }
	 printf("\n");
 }