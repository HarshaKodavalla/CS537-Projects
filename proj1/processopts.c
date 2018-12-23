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
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "ps537.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

/**
 * Takes the command line arguments as input sets the fields in
 * the options class.
 *
 * @param int argc - the number of command line arguments
 * @param char *argv[] - the command line arguments
 */
void processOpts(int argc, char *argv[], Options *options) {
    // Initialize each field in options to its default setting
    options->pidDisplay = false;
    options->stateDisplay = false;
    options->uTimeDisplay = true;
    options->sTimeDisplay = false;
    options->vmDisplay = false;
    options->cmdLineDisplay = true; 

    int opt;

    // Input handling using switch case
    // If an option is followed by a hypen, ensure appropriate behavior is executed.
    while (true) {
        opt = getopt(argc, argv, "p:s::U::S::v::c::");
        if (opt == -1) { break; }
        switch (opt) {
            case 'p' :
                options->pidDisplay = true;
                options->pid = atoi(optarg);
                // Ensure given pid is in valid format
                if (options->pid == 0) {
                    printf("Invalid PID.\n");
                    printUsage();
                    free(options);
                    exit(0);
                }
                break;
            case 's' :
                // State display
                if (!hasHyphen(optarg, options)) {
                    options->stateDisplay = true;
                }
                break;
            case 'U' :
                // User time display
                if (hasHyphen(optarg, options)) {
                    options->uTimeDisplay = false;
                }
                break;
            case 'S' :
                // System time display 
                if (!hasHyphen(optarg, options)) {
                    options->sTimeDisplay = true;
                }
                break;
            case 'v' :
                // Virtual memory display   
                if (!hasHyphen(optarg, options)) {
                    options->vmDisplay = true;
                }
                break;
            case 'c' :
                // Command line display
                if (hasHyphen(optarg, options)) {
                    options->cmdLineDisplay = false;
                }
                break;
            default:
                // Inalid input handling
                printUsage();
                free(options);
                exit(0);
        }
    }
    // Check for invalid characters/args 
    if (optind < argc) {
        printUsage();
        free(options);
        exit(0);
    }
}

/**
 * Checks if optarg contains a hyphen. If it does, return true. If it contains
 * no character, return false. If it contains a non-hyphen character print usage
 * and terminate the program.
 *
 * @param char *optarg - holds the character following the command line option,
 * or 0 if no such character exists.
 * @param Options *options - the class containing the options fields.
 * @return - returns true if a hyphen does follow the command line option and false if 
 * no characters follow.
 */
bool hasHyphen(char *optarg, Options *options) {
    // Check if a character does follow the command line option
    if (optarg != 0) {
        if (strcmp(optarg, "-") != 0) {
            printUsage();
            free(options);
            exit(0);
        } else {
            return true;
        }
    } else {
        return false;
    }
}



/**
 * Prints the usage if an invalid command line option is given.
 */
void printUsage() {
    printf("usage: 537ps [-p <pid>] [-s] [-U-] [-S] [-v] [-c-]\n");
}