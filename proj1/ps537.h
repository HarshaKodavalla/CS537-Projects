///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             ps537.h
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

#include <stdbool.h>
#ifndef PS537_H
#define PS537_H

// Contains options for the program. These are specified from the command line
// when the program is invoked. 
typedef struct Options {
    // Display information for a single process
    bool pidDisplay;
    // The pid for the single process
    int pid;
    // Display the single character state information
    // Defaults to false
    bool stateDisplay;
    // Display the amount of user time consumed 
    // Defaults to true
    bool uTimeDisplay;
    // Display the amount of system time consumed
    // Defaults to false
    bool sTimeDisplay;
    // Display the amount of virtual memory currently being used
    // Defaults to false
    bool vmDisplay;
    // Display the command-line information
    // Defaults to true
    bool cmdLineDisplay;
} Options;

// Prints the usage for invoking the program.
void printUsage();

// Processes and stores command line options
void processOpts(int argc, char *argv[], Options *options);

// Checks if there is a hyphen following a given command line option
bool hasHyphen(char *optarg, Options *options);

// Searches the proc directory for process folders whose userid matches
// the user who invoked the program.
void searchProc(Options *options);

// Searches the /proc directory for a single PID specified by the -p flag.
void searchPID(Options *options);

// Checks if the given process folder belongs to the user. 
// who invoked the program.
bool belongsToUser();

// Checks if string contains only digits
bool isStringDigit(char *stringName);

// Reads information from the process's files
void parseFiles(Options *options, char *fileName);

// Prints the information stored in the stat file
void printStatInfo(char *filePath, Options *options);

// Prints the VM information stored in the statm file
void printVMInfo(char *filePath, Options *options);

// Prints the information stored in the cmdline file
void printCMDInfo(char *filePath, Options *options);

#endif /* PS537_H */