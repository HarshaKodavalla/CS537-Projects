///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             parser.c
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

#define STAT "/stat"
#define DIRNAME "/proc/"
#define STATM "/statm"
#define CMDLINE "/cmdline"
#define BUFFERSIZE 50000
#define STATEINDEX 2
#define UTIMEINDEX 13
#define STIMEINDEX 14

/**
 * Parses the files contained in a given process's folder within
 * the /proc directory.
 * @param Options *options - contains the command line options
 * @param char *procName - contains the "name" of the process
 */
void parseFiles(Options *options, char *procName) {
    printf("%s: ", procName);

    // If an option that requires reading of the stat file is active, get 
    // the stat file's filepath.
    if ( 
        (options->stateDisplay) 
        || (options->uTimeDisplay)
        || (options->sTimeDisplay) 
        ) {
        char statFP[BUFFERSIZE];
        // Use sprintf to store the filepath into statFP
        if (sprintf(statFP, "%s%s%s", DIRNAME, procName, STAT) < 0) {
            printf("Sprintf error. Terminating program.\n");
            free(options);
            exit(1);
        }
        // Print the information needed from the stat file according to the 
        // command line options.
        printStatInfo(statFP, options);
    }

    // If the option requiring reading of the statm file is active, get 
    // the stat file's filepath.
    if (options->vmDisplay) {
        char statmFP[BUFFERSIZE];
        if (sprintf(statmFP, "%s%s%s", DIRNAME, procName, STATM) < 0) {
            printf("Sprintf error. Terminating program.\n");
            free(options);
            exit(1);
        }
        // Print the information needed from the statm file according to the 
        // command line options.
        printVMInfo(statmFP, options);
    }

    // If the option requiring reading of that cmdline file is active, get 
    // cmdline file's filepath.
    if (options->cmdLineDisplay) {
        char cmdlineFP[BUFFERSIZE];
        if (sprintf(cmdlineFP, "%s%s%s", DIRNAME, procName, CMDLINE) < 0) {
            printf("Sprintf error. Terminating program.\n");
            free(options);
            exit(1);
        }
        // Print the information needed from the cmdline file according to the 
        // command line options.
        printCMDInfo(cmdlineFP, options);
    }
    printf("\n");
}

/**
 * Prints the information from the stat file according to the command line
 * options.
 *
 * @param char *filePath - the filepath to the stat file
 * @param Options *options - the command line options.
 */
void printStatInfo(char *filePath, Options *options) {
    char statInfo[BUFFERSIZE];  // The stat file's contents will be placed here
    char *token;    // Used to parse the stat file's contents
    int i = 0;      
    
    // Read from the stat file
    FILE *fp = fopen(filePath, "r");
    if (fp == NULL) {
        printf("Fopen error1. Terminating program.\n");
        fclose(fp);
        free(options);
        exit(1);
    }

    // Get the contents of the stat file
    if (fgets(statInfo, BUFFERSIZE, fp) == NULL) {
        printf("Fgets error2. Terminating program.\n");
        fclose(fp);
        free(options);
        exit(1);
    }

    // Iterate through the contents of the stat file searching for the fields
    // specified by the command line options.
    token = strtok(statInfo, " ");
    while(token != NULL) {
        // Because " " is used as the delimiter, if the comm field contains
        // "(Web Content)" it must be accounted for and skipped without incrementing
        // the counter.
        if (i == STATEINDEX && strcmp(token, "Content)") == 0) {
            token = strtok(NULL, " ");
        }
        // If -s option, print the state information
        if (i == STATEINDEX && options->stateDisplay) {
            printf("%s ", token);
        }
        // If -U option, print the user time
        if (i == UTIMEINDEX && options->uTimeDisplay) {
            printf("utime=%s ", token);
        }
        // If -S option, print the system time
        if (i == STIMEINDEX && options->sTimeDisplay) {
            printf("stime=%s ", token);
            break;
        }
        i++;
        token = strtok(NULL, " ");
    }
    fclose(fp);
} 

/**
 * Prints the amount of virtual memory being used - located in the 
 * statm file.
 *
 * @param char *filePath - the filepath to the statm file
 * @param Options *options - the command line options.
 */
void printVMInfo(char *filePath, Options *options) {
    char vmInfo[BUFFERSIZE];    // The statm file's contents will be placed here
    
    // Read from the statm file
    FILE *fp = fopen(filePath, "r");
    if (fp == NULL) {
        printf("Fopen error3. Terminating program.\n");
        fclose(fp);
        free(options);
        exit(1);
    }

    // Get the contents of the statm file
    if (fgets(vmInfo, BUFFERSIZE, fp) == NULL) {
        printf("Fgets error4. Terminating program.\n");
        fclose(fp);
        free(options);
        exit(1);
    }
    printf("vmemory=%s ", strtok(vmInfo, " "));
    fclose(fp);
}

/**
 * Prints the command line that started the process - located in the 
 * cmdline file.
 *
 * @param char *filePath - the filepath to the cmdline file
 * @param Options *options - the command line options.
 */
void printCMDInfo(char *filePath, Options *options) {
    char cmdInfo[BUFFERSIZE];
    char *p;

    // Read from the cmdline file
    FILE *fp = fopen(filePath, "r");
    if (fp == NULL) {
        printf("Fopen error5. Terminating program.\n");
        fclose(fp);
        free(options);
        exit(1);
    }

    // Get the contents of the cmdline file
    if (fgets(cmdInfo, BUFFERSIZE, fp) == NULL) {
        if (feof(fp)) {
            return;
        }
        printf("Fgets error6. Terminating program.\n");
        fclose(fp);
        free(options);
        exit(1);
    }

    // Parse and print the cmdline contents
    bool startLoop = true;
    printf("[");
    p = cmdInfo;
    while (*p) {
      // Ensures printd output is formatted correctly
        if (startLoop) {
	        startLoop = false;
            printf("%s", p);
        } else {
	        printf(" %s",p);
        }
        p = strchr(p, '\0');
        p++;
    }
    printf("]");

    fclose(fp);
    
}
