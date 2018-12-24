///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             processopts.c
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
#include <pthread.h>
#include "module.h"
#define QUEUECAPACITY 10
#define BUFFERSIZE 1023

/**
 * Creates a single queue, and initializes its members.
 *
 * @param int size - how many pointers the queue stores
 * @return - the queue that was created.
 */
Queue * CreateStringQueue(int size) {
    // Allocate a single queue
    Queue *q = malloc(sizeof(Queue));
    if (q == NULL) {
        printf("Malloc error. Terminating program.\n");
        exit(1);
    }

    // Allocate the array of char pointers
    q->strings = malloc(size * sizeof(char*));
    if (q->strings == NULL) {
        printf("Malloc error. Terminating program.\n");
        exit(1);
    }

    // Initialize the queue struct's fields
    q->enqueueCount = 0;
    q->dequeueCount = 0;
    q->enqueueBlockCount = 0;
    q->dequeueBlockCount = 0;
    q->head = 0;
    q->tail = 0;

    // Initialize the locks and monitors
    q->lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    q->nonFull = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    q->nonEmpty = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    return q;
}

/**
 * Enqueues a string to the tail of the queue. If the queue is full,
 * the thread is put to sleep until it is notified that a spot has opened.
 *
 * @param Queue *q - the queue where the string will be put.
 * @param char *string - the string to be placed in the queue
 */
void EnqueueString(Queue *q, char *string) {
    int wakeCheck = 0;  // Used to check for spurious wakeup

    // Acquire the lock
    if (pthread_mutex_lock(&q->lock)) {
        printf("Pthread_mutex_lock error. Terminating program.\n");
        exit(1);
    }
    
    // If the queue is full, put the thread to sleep and wait for a string
    // to be dequeued.
    while ( (q->enqueueCount - q->dequeueCount) >= QUEUECAPACITY ) {
        // If a spurious wakeup occurred, do not increment block counter
        if (!wakeCheck) {
            q->enqueueBlockCount++;
            wakeCheck = 1;
        }
        if (pthread_cond_wait(&q->nonFull, &q->lock)) {
            printf("Pthread_cond_wait error. Terminating program.\n");
            exit(1);
        }
    }

    // Append string to the tail of the queue and increment count
    q->strings[q->tail] = string;
    q->enqueueCount++;

    // Wraparound to ensure circular queue
    q->tail = (q->tail + 1) % QUEUECAPACITY;

    // Mutex unlock 
    if (pthread_mutex_unlock(&q->lock)) {
        printf("Pthread_mutex_unlock error. Terminating program.\n");
        exit(1);
    }

    // Signal that the queue is non empty
    if (pthread_cond_signal(&q->nonEmpty)) {
        printf("Pthread_cond_signal error. Terminating program.\n");
        exit(1);
    };
    return;
}
/**
 * Removes a string from the tail of the queue. If the queue is empty,
 * the thread is to put to sleep until it is notified that a string was added.
 *
 * @param Queue *q - the queue where the string will be put.
 * @return - the string that was dequeued.
 */
char* DequeueString(Queue *q) {
    int wakeCheck = 0;  // Used to check for spurious wakeup

    // Acquire the lock
    if (pthread_mutex_lock(&q->lock)) {
        printf("Pthread_mutex_lock error. Terminating program.\n");
        exit(1);
    }

    // If the queue is empty, put the thread to sleep and wait for a string
    // to be enqueued.
    while (q->enqueueCount == q->dequeueCount) {
        // If a spurious wakeup occurred, do not increment block counter
        if (!wakeCheck) {
            q->dequeueBlockCount++;
            wakeCheck = 1;
        }
        if (pthread_cond_wait(&q->nonEmpty, &q->lock)) {
            printf("Pthread_cond_wait error. Terminating program.\n");
            exit(1);
        }
    }

    // Get the string from the head of the queue and decrement count
    char *str = q->strings[q->head];
    q->dequeueCount++;

    // Wraparound to ensure circular queue
    q->head = (q->head + 1) % QUEUECAPACITY;

    // Mutex unlock
    if (pthread_mutex_unlock(&q->lock)) {
        printf("Pthread_mutex_unlock error. Terminating program.\n");
        exit(1);
    }

    // Signal that the queue is no longer full
    if (pthread_cond_signal(&q->nonFull)) {
        printf("Pthread_cond_signal error. Terminating program.\n");
        exit(1);
    }
    return str;
}

/**
 * Prints out the statistics of a queue.
 *
 * @param Queue *q - the queue where the strings are contained.
 */
void PrintQueueStats(Queue *q) {
    // Must subtract 1 to account for the NULL string
    printf("enqueueCount: %d\n", q->enqueueCount - 1);
    printf("dequeueCount: %d\n", q->dequeueCount - 1);
    printf("enqueueBlockCount: %d\n", q->enqueueBlockCount);
    printf("dequeueBlockCount: %d\n", q->dequeueBlockCount);
}
