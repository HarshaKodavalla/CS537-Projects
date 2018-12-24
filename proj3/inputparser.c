///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             inputparser.c
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
#include "./inputparser.h"

#define INPUTBUFFER 1024    // Maximum allowed line size
#define TAB 9
#define RETURN 13
#define NEWLINE 10


int totalLineCount = 0; // keeps track of the current line number

/**
 * Reads a makefile called 'makefile' or 'Makefile' in the same folder
 * this exectuable is located in. Returns the array of makefile lines as arrays.
 *
 * @param int *numLines - a pointer to an int that stores the number of
 *                        content lines (lines that contain makefile)
 * @param int *numTargets - a pointer to an int that store the number of
 *                          targets in the makefile
 * @return char ***makefileLines - return a (clean) array of the makefiles lines
 *                                 as arrays of strings
 */
char *** readMakefileLines(int * numLines, int * numTargets){
    char ***makefileLines = malloc(INPUTBUFFER*sizeof(char **));
    FILE * makefile;
    makefile = fopen("./makefile", (const char*) "r");
    if (makefile == NULL) {
        makefile = fopen("./Makefile", (const char*) "r");
        if (makefile == NULL) { 
            perror("Error opening makefile");
            exit(1);
        }
    }
    int c; // stores the current next char of the makefile
           // only used to detect EOF
    int lineNumber = 0; // keeps track of which line we are currently on
    int lineCount = 0; // counts the total amount of lines
    int targetCount = 0; //counts the total amount of targets

    // get first char of the makefile for detecting EOF
    c = getc(makefile);
    
    // iterate over all makefile lines if getc did not return EOF (-1)
    while (c != -1) {
        // put the char only used to test EOF again on the makefile
        if (ungetc(c, makefile) == -1){
            perror("Error performing ungetc");
            exit(1);
        }
        totalLineCount++;
        // read the individual makefile line
        makefileLines[lineNumber++] = readMakefileLine(makefile);
        lineCount++;
        

        // if the line is a comment or blank line
        // - set the line to 0 and decrease linenumber
        if (!makefileLines[lineNumber-1] ||
            !makefileLines[lineNumber-1][0] ||
            *makefileLines[lineNumber-1][0] == '#' ) {
            
            // decrease lineNumber (we dont want to store this line)
            lineNumber--;

            // free the memory at the pointer that points to the comment
            // erase the pointer to the comment
            memset (makefileLines[lineNumber], 0, sizeof(char **)*INPUTBUFFER);
            free(makefileLines[lineNumber]);
            
            // decrease lineCount bc line does not count as an executable line
            lineCount--;
        } else if(!lineIsCorrect(makefileLines[lineNumber-1])) {
            // throw error if line is incorrect
            printf("\n%d: Invalid line:", totalLineCount);
            int i = 0;
            while (makefileLines[lineNumber-1][i]) {
                printf(" %s", makefileLines[lineNumber-1][i]);
                i++;
            }
            printf("\n");
            exit(1);
        } else if (lineIsCorrect(makefileLines[lineNumber-1]) == 1) {
            // if line is a target line increase targetCount
            targetCount++;
        }
        // getc to detect EOF and potentially end loop
        c = getc(makefile);
    }
    // close the makefile
    if(fclose(makefile)) {
        perror("Error closing makefile");
        exit(1);
    }
    
    // update pointers with correct values
    *numLines = lineCount;
    *numTargets = targetCount;
    
    // return array
    return makefileLines;
}

/**
 * Checks if a line is a correct makefile line (contains a tab as the first char
 * or a colon as the second element and is thus a target line.
 * @param FILE **line - pointer to the line that we check for correctness
 *
 * @return int - returns 0 if line is incorrect. returns 1 if it is a target
 *               line, returns 2 if it is not a command line.
 */
int lineIsCorrect(char **line) {
    //checks if line starts with tab or target
    if (*line[0] == 9){
        // if it starts with a tab return 2
        return 2;
    } else if (line[1] != NULL && *line[1] == ':') {
        // check if ":" is in the string array
        // and thus is a target line
        return 1;
    }
    // if the first object is not a target or starts with a tab -> the line is
    // incorrect
    return 0;
}

/**
 * Reads the next line in the makefile and returns it as an array of strings.
 * Modifies the array by filtering out for a makefile redundant spaces and tabs
 *
 * @param FILE *makefile - pointer to the makefile from which we want to read
 *                         the next line from
 *
 * @return char **makefileLine - returns an array of strings with all
 *                               information containing words in a makefile
 */
char **readMakefileLine(FILE *makefile){
    int c;
    char **stringField = malloc(INPUTBUFFER*sizeof(char *));
    memset(stringField, '\0', INPUTBUFFER*sizeof(char *));
    /// malloc new charfield
    char *allocatedString = allocateString(INPUTBUFFER);
    int charCounter = 0; // counts the char position in the string at a specific
                         // array position (charCounter per Word)
    int totalCharCount = 0; // counts total chars in the line processed
    int arrayIndex = 0;
    int colonCount = 0;
    
    // Main loop - iterates through all characters in the line
    while(1){
        // get the next char from the makefile
        c = getc(makefile);
        if (charCounter <= INPUTBUFFER) {
            //
            if (c == ' ' || (!(charCounter <= 1 && arrayIndex == 0) && c == TAB )){
                // If a tab or space are found, do not push onto the array
                if(*allocatedString) {
                    stringField[arrayIndex] = allocatedString;
                    allocatedString = allocateString(INPUTBUFFER);
                    arrayIndex++;
                } else{ free(allocatedString); }
                charCounter = -1;
            } else if ( c == -1 || c == NEWLINE || c == RETURN){
                // end of line encountered (through newline, return or eof)
                // push the last allocated string onto the array
                // if it is not null
                if(*allocatedString){
                    stringField[arrayIndex] = allocatedString;
                    arrayIndex++;
                } else{ free(allocatedString); }
                // break - we read the line - the loop has done its work
                break;
            } else if (c == ':') {
                if(c == ':' && colonCount > 0){
                    // if there is multiple colons in the line - invalid line
                    printf("\n%d: InvalidLine - multiple targets: ",
                           totalLineCount);
                    if(*allocatedString) {
                        stringField[arrayIndex] = allocatedString;
                    } else{ free(allocatedString); }
                    for (int i = 0; i <= arrayIndex; i++) {
                        if(stringField[i]){
                            printf("%s ", stringField[i]);
                        }
                    }
                    printf(":");
                    
                    while ( c!= -1 && c != NEWLINE && c != RETURN ) {
                        c = getc(makefile);
                        printf("%c", c);
                    }
                    
                    exit(1);
                } else {
                    // push the string that is stored at the moment onto the
                    // array
                    if(*allocatedString){
                        colonCount++;
                        stringField[arrayIndex] = allocatedString;
                        allocatedString = allocateString(INPUTBUFFER);
                        arrayIndex++;
                    } else{ free(allocatedString); }
                    // push the colon onto to the array
                    charCounter = 0;
                    *(allocatedString + charCounter) = c;
                    colonCount++;
                    stringField[arrayIndex] = allocatedString;
                    // and set up the next array element
                    allocatedString = allocateString(INPUTBUFFER);
                    arrayIndex++;
                    charCounter = -1;
                }
            } else if (c == 0) {
                // if there is a null character on the line - invalid line
                printf("\n%d: InvalidLine - contains null character: ",
                       totalLineCount);
                if(*allocatedString) {
                    stringField[arrayIndex] = allocatedString;
                } else{ free(allocatedString); }
                for (int i = 0; i <= arrayIndex; i++) {
                    if(stringField[i]){
                        printf("%s ", stringField[i]);
                    }
                }
                printf("\\0");
                
                while ( c!= -1 && c != NEWLINE && c != RETURN ) {
                    c = getc(makefile);
                    printf("%c", c);
                }
                exit(1);
            } else {
                /// push each character onto that field
                // if it is not a tab
                if (c !=9){
                    *(allocatedString + charCounter) = c;
                // if the tab is the very first character - save it
                } else if (charCounter==0 && arrayIndex==0){
                    // if it's the first tab save it
                    *(allocatedString + charCounter) = c;
                    // ignore it otherwise
                } else { charCounter--;}
            }
        } else {
            // if a word exceeds the INPUTBUFFER in length
            printf("Makefile line exceeds buffer.\n");
            exit(1);
        }
        charCounter++;
        totalCharCount++;
        // if there are arbitrarily many word in a makefile line that exceed
        // the buffer of words per line -> throw error
        if ( arrayIndex > INPUTBUFFER ){
            printf("Makefile line exceeds buffer (Word count).\n");
            exit(1);
        }
        // if there are arbitrarily many characters in a line
        if ( totalCharCount > INPUTBUFFER ){
            printf("Makefile line exceeds buffer (Total char count).\n");
            exit(1);
        }
    }
    // if there was a tab in the line but nothing else
    if (stringField[0] && stringField[0][0] == TAB
        && (stringField[0][1] == '\0' || stringField[0][1] == 13) ) {
        // throw it out - just mark the whole line as a comment for simplicity
        stringField[0][0] = '#';
    }
    return stringField;
}


/**
 * Allocates a new string in memory at returns a pointer to it.
 *
 * @param size_t buffersize - how much memory we want to allocate for the string
 *
 * @return char *allocatedString - returns a pointer to the allocated memory
 *                                 structure
 */
char* allocateString(size_t buffersize) {
    /// malloc new charfield
    char *allocatedString = malloc(buffersize);
    memset(allocatedString, '\0', buffersize);
    if(allocatedString == NULL) {
        printf("Memory allocation failed. Terminating program.\n");
        exit(1);
    }
    return allocatedString;
}