///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  module.c
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
// Online sources:   http://pages.cs.wisc.edu/~bart/537/programs/program2.html
//                   - copied variable and function descriptions -
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
#include <pthread.h>
#ifndef MODULE_H
#define MODULE_H

/**
 * The queue struct contains an array of strings, as well as variables containing
 * statistics about operations done on the queue and its array. It also contains
 * a mutex and two monitors to allow multi-threading.
 */
typedef struct Queue {
    // Mutex for the queue
    pthread_mutex_t lock;
  
    // Monitor to signal when queue is non-full
    pthread_cond_t nonFull;
  
    // Monitor to signal when queue is non-empty
    pthread_cond_t nonEmpty;
  
    // Array of strings
    char** strings;
  
    // indexes of the head and tail
    int head;
    int tail;
  
    // A count of the number of strings enqueued on this queue.
    int enqueueCount;
  
    // A count of the number of strings dequeued on this queue.
    // We would expect that when the program exits, enqueue and dequeue are equal.
    int dequeueCount;
  
    // A count of the number of times that an enqueue was attempted but blocked
    // due to a full queue.
    int enqueueBlockCount;
  
    // A count of the number of times that a dequeue was attempted but blocked
    // due to an empty queue.
    int dequeueBlockCount;
  
} Queue;

/**
 * Creates a single queue, and initializes its members.
 *
 * @param int size - how many pointers the queue stores
 * @return - the queue that was created.
 */
Queue *CreateStringQueue(int size);

/**
 * Enqueues a string to the tail of the queue. If the queue is full,
 * the thread is put to sleep until it is notified that a spot has opened.
 *
 * @param Queue *q - the queue where the string will be put.
 * @param char *string - the string to be placed in the queue
 */
void EnqueueString(Queue *q, char *string);

/**
 * Removes a string from the tail of the queue. If the queue is empty,
 * the thread is to put to sleep until it is notified that a string was added.
 *
 * @param Queue *q - the queue where the string will be put.
 * @return - the string that was dequeued.
 */
char * DequeueString(Queue *q);

/**
 * Prints out the statistics of a queue.
 *
 * @param Queue *q - the queue where the strings are contained.
 */
void PrintQueueStats(Queue *q);

#endif /* MODULE_H */
