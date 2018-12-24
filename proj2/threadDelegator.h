///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  threadDelegator.c
// File:             threadDelegator.h
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
#ifndef THREADDELEGATOR_H
#define THREADDELEGATOR_H

/**
 * Reads Stdin and appends each line as a string to the first queue
 * in the given queue q array. Enqueues a Null String as flag for completion.
 * 
 *
 * @param (*q) (A queue array pointer, that points to an array of queues)
 * @return (returns NULL upon completion)
 */
void *readStdIn(void *q);

/**
 * Dequeues string from the first queue in a given queue array.
 * Replaces every white space with an aterisk. Enqueues strings to the second
 * queue in the given queue array.
 *
 * @param (*q) (A queue array pointer, that points to an array of queues)
 * @return (returns NULL upon completion)
 */
void *munch1(void *q);

/**
 * Dequeues string from the second queue in a given queue array.
 * Replaces every lower case character with an upper case char.
 * Enqueues strings to the third queue in the given queue array.
 *
 * @param (*q) (A queue array pointer, that points to an array of queues)
 * @return (returns NULL upon completion)
 */
void *munch2(void *q);

/**
 * Dequeues string from the third queue in a given queue array.
 * Prints the strings to stdOut.
 *
 * @param (*q) (A queue array pointer, that points to an array of queues)
 * @return (returns NULL upon completion)
 */
void *writeStdOut(void *q);

#endif /* THREADDELEGATOR_H */