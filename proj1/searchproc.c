///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             searchproc.c
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
// CS Login:         Brendle
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
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "ps537.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

#define DIRNAME "/proc/"
#define LOGINUID "/loginuid"
#define BUFFERSIZE 50000

/**
 * Searches the /proc directory for a specific PID and prints out that 
 * process's information according to the command line options.
 *
 * @param Options *options - contains the command line options information
 */
void searchPID(Options *options) {
    char *dirName = DIRNAME;
    DIR *procDir = opendir(dirName);    // Open the /proc directory
    if (procDir == NULL) {
        printf("Opendir error. Terminating program.\n");
        free(options);
        exit(1);
    }

    struct dirent *procFile;

    // Iterate through the /proc directory and search for the matching PID
    while( (procFile = readdir(procDir)) ) {
        if (procFile == NULL) {
            printf("Readdir error. Terminating program.\n");
            closedir(procDir);
            free(options);
            exit(1);
        }

        // If the matching PID is fine, parse its files
        if (atoi(procFile->d_name) == options->pid) {
            parseFiles(options, procFile->d_name);
            return;
        }
    }
    closedir(procDir);

    // The PID was not found.
    printf("The given PID was not found. Terminating program.\n");
    return;
}

/**
 * Searches the entire /proc directory for any process's belonging to the 
 * user who invoked the program. Prints these process's information
 * according to the command line options. 
 *
 * @param Options *options - contains the command line options information
 */
void searchProc(Options *options) {
    bool belongs = false;
    char *dirName = DIRNAME;
    DIR *procDir = opendir(dirName);
    if (procDir == NULL) {
        printf("Opendir error. Terminating program.\n");
        free(options);
        exit(1);
    }
    struct dirent *procFile;
    
    // Iterate through the /proc directory and search for the matching PID
    while( (procFile = readdir(procDir)) ) {
        // Check if the folder is a process folder
        if (!isStringDigit(procFile->d_name)) {
            continue;
        }
        // Check if the process belongs to the user who invoked the program
        belongs = belongsToUser(procFile);
        if (belongs) {
            // If it belongs, parse its files
            parseFiles(options, procFile->d_name);
        } else if (belongs != true && belongs != false) {
            // An error has occurred
            free(options);
            closedir(procDir);
            exit(1);
        }
    }
    closedir(procDir);
    return;
}

/**
 * Determines if a process belongs to the user who invoked the program.
 *
 * @param struct dirent *procFile - the process file to be checked.
 * @return returns false if the file does not belong to the user who 
 * invoked the program. Returns true if the file does belong. Returns NULL
 * if an error has occurred.
 */
bool belongsToUser(struct dirent *procFile) {
    // Creates a string containing the filepath to the process's
    // loginuid file.
    char loginFP[BUFFERSIZE];    
    if (sprintf(loginFP, "%s%s%s", DIRNAME, procFile->d_name, LOGINUID) < 0) {
        printf("Sprintf error. Terminating program.\n");
       return NULL;
    }
    
    // Read the loginuid file to determine the process's user.
    FILE *fp = fopen(loginFP, "r");
    if (fp == NULL) {
        printf("Fopen error. Terminating program.\n");
        return NULL;
    }
    char userProcID[BUFFERSIZE];
    if (fgets(userProcID, BUFFERSIZE, fp) == NULL) {
        printf("Fgets error. Terminating program.\n");
        return NULL;
    }
    // Check the process ID against the user ID
    if (atoi(userProcID) == (int)getuid()) {
        return true;
    }
    return false;
}

/**
 * Determines if a string contains only digits.
 *
 * @param char *s - the string to be checked.
 * @return returns false if the string contains any non-digit chracters.
 * returns true if the string contains only digit characters.
 */
bool isStringDigit(char *s) {
    for (size_t i = 0; i < strlen(s); i++) {
        if (!isdigit(*s)) {
            return false;
        }
    }
    return true;
}