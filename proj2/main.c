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
#include <pthread.h>
#include "threadDelegator.h"
#include "module.h"
#define QUEUECAPACITY 10	// Size of a single queue

/**
 * Runs the program. Creates four threads to operate on strings
 * that call other functions.
 * 
 */
int main() {
    // Create array of queues
	
	// Queue 0 operates between the reader and munch1
	// Queue 1 operates between munch1 and munch2
	// Queue 3 operates between munch2 and the writer
    Queue** queues = malloc(sizeof(Queue *) * 3);
    if (queues == NULL) {
	    printf("Malloc error. Terminating program.\n");
	    exit(1);
    }
	// Allocate and initialize each queue
    for (int i = 0; i < 3; i++) {
		queues[i] = CreateStringQueue(QUEUECAPACITY);
    }

    // Create four pthreads
    pthread_t reader;
    pthread_t munchThread1;
    pthread_t munchThread2;
    pthread_t writer;
    
    // Create the Reader thread, which reads input from stdin
    if(pthread_create(&reader, NULL, readStdIn, &queues)) {
	    printf("Pthread1 error. Terminating program.\n");
	    exit(1);
    }
    // Create the munch1 thread
    if(pthread_create(&munchThread1, NULL, munch1, &queues)) {
	    printf("Pthread2 error. Could not generate thread. Terminating program.\n");
	    exit(1);
    }
    // Create the munch2 thread
    if((pthread_create(&munchThread2, NULL, munch2, &queues))) {
	    printf("Pthread3 error. Could not generate thread. Terminating program.\n");
	    exit(1);
    }	
    // Create the writer thread
    if((pthread_create(&writer, NULL, writeStdOut, &queues))) {
	    printf("Pthread4 error. Could not generate thread. Terminating program.\n");
	    exit(1);
    }
    
	// Wait for writer to terminate and then go to program completion
    if (pthread_join(writer, NULL)) {
		printf("Pthread join error. Terminating program.\n");
		exit(1);
    }

    // print statistics for each queue
	printf("\n");
	for (int i = 0; i < 3; i++) {
			printf ("Queue %d Stats\n", i+1);
			PrintQueueStats(queues[i]);
			printf("\n");
	} 
}
