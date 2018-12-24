///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             threadDelegator.c
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
#include "./threadDelegator.h"
#include "./module.h"

#define MUNCH1CHAR 32
#define BUFFERSIZE 1023
#define NEWLINE 10


/**
 * Reads Stdin and appends each line as a string to the first queue
 * in the given queue q array. Enqueues a Null String as flag for completion.
 * 
 *
 * @param (*q) (A queue array pointer, that points to an array of queues)
 * @return (returns NULL upon completion)
 */
void *readStdIn(void *q) {
    Queue** queues = *(Queue***) q;

    int c;  // Receives input from stdin

    char *str = malloc(BUFFERSIZE+1);   // Holds the string to be passed along
    if (str == NULL) {
        printf("Malloc error. Terminating program.\n");
        exit(1);
    }

    int count = 0;  // Keeps count of the number of characters read

    // Reads from stdin
    while (1) {
        c = getc(stdin);
        // String is too large, print error message and flush.
        if (count > BUFFERSIZE) {
            memset(str, 0, BUFFERSIZE+1);
            str = malloc(BUFFERSIZE+1);
            if (str == NULL) {
                printf("Malloc error. Terminating program.\n");
                exit(1);
            }

            // Search until newline is found, to start parsing next string
            if (c != NEWLINE) {
                while(getc(stdin) != NEWLINE) {}
            }
            fprintf(stderr, "String is too large. Flushing.\n");
            count = 0;
            continue;

        } else if (c == NEWLINE) {
            // Newline encountered, push currently held string onto queue
            // and start reading for a new string

            str[count] = c;
            str[++count] = 0;   // Append null character
            EnqueueString(queues[0], str);

            str = malloc(BUFFERSIZE+1);
            if (str == NULL) {
                printf("Malloc error. Terminating program.\n");
                exit(1);
            }
            count = 0;
            continue;

        } else if (c == -1) {
            // EOF encountered, push current string on queue, as well as NULL string
            // and terminate the reader.

            // Check for getc error
            if (ferror(stdin) != 0) {
                printf("Getc error. Terminating program.\n");
                exit(1);
            }

            // Enqueues string before EOF if it exists
            if (count > 0) {
                str[count] = 0;
                EnqueueString(queues[0], str);
            }

            // Enqueue the NULL string to indicate that reading has terminated
            str = malloc(1);
            if (str == NULL) {
                printf("Malloc error. Terminating program.\n");
                exit(1);
            }
            str = NULL;
            EnqueueString(queues[0], str);
            return NULL;
        }
        str[count++] = c;
    }
    return NULL;
}

/**
 * ( Dequeues string from the first queue in a given queue array.
 * Replaces every white space with an aterisk. Enqueues strings to the second
 * queue in the given queue array.)
 *
 * @param (*q) (A queue array pointer, that points to an array of queues)
 * @return (returns NULL upon completion)
 */
void *munch1(void *q) {
    Queue** queues = *(Queue***) q;

    char *str;
    char *spaceIndex;
    while (1) { 
        // Get the string from the queue
        str = DequeueString(queues[0]);
        // Terminate if NULL string is found
        if(str == NULL) {
            EnqueueString(queues[1], str);
            return NULL;
        }

        // Replaces space with *
        spaceIndex = index(str, MUNCH1CHAR);
        while (spaceIndex != NULL) {
            *(spaceIndex) = '*';
            // find a new occurence
            spaceIndex = index((spaceIndex + 1), MUNCH1CHAR);
        }
        // Push modified string onto the next queue
        EnqueueString(queues[1], str);
    }
    return NULL;
}

/**
 * ( Dequeues string from the second queue in a given queue array.
 * Replaces every lower case character with an upper case char.
 * Enqueues strings to the third queue in the given queue array.)
 *
 * @param (*q) (A queue array pointer, that points to an array of queues)
 * @return (returns NULL upon completion)
 */
void *munch2(void *q) {
    Queue** queues = *(Queue***) q;

    char *str;
    while (1) {
        // Get the string from the queue
        str = DequeueString(queues[1]);
        // Terminate if NULL string is found
        if(str == NULL) {
            EnqueueString(queues[2], str);
            return NULL;
        }
        // Change all lowercase chars to uppercase
        for (size_t i = 0; i < strlen((const char*) str); i++) {
            if (islower(*(str + i))) {
                *(str + i) = toupper(*(str + i));
            }
        }
        // Push modified string onto the next queue
        EnqueueString(queues[2], str);
        //printf("TWO PUT\n");
    }
    return NULL;
}

/**
 * ( Dequeues string from the third queue in a given queue array.
 * Prints the strings to stdOut.)
 *
 * @param (*q) (A queue array pointer, that points to an array of queues)
 * @return (returns NULL upon completion)
 */
void *writeStdOut(void *q) {
    Queue** queues = *(Queue***) q;
    char *str;
    int count = 0;
    while (1) {
        // Get string from queue
        str = DequeueString(queues[2]);
        if(str == NULL) {
            break;
        }
        // Write to stdout
        printf("%s", str);
        free(str);
        count++;
    }
    printf("\nProcessed this many strings: %d\n", count);
    return NULL;
}
