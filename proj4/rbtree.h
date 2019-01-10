///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             rbtree.h
// Semester:         CS 537 Fall 2018
//
// Author:           Harsha Kodavalla kodavalla@wisc.edu
// CS Login:         harsha
// Lecturer's Name:  Miller
// Lab Section:      303
//
//////////////////// STUDENTS WHO GET HELP FROM OTHER THAN THEIR PARTNER //////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Introduction to Algorithms by Cormen, Leiserson, Rivest
//                   and Stein
//////////////////////////// 80 columns wide //////////////////////////////////

#ifndef RBTREE_H_
#define RBTREE_H_
#include <stdbool.h> 
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Node {
    // Start address, which is used as the key
    void *low;  
    // Start address + length of memory block
    void *high; 
    bool freed;
    // 1 for red, 0 for black
    bool red;
    // The largest high address in this node's subtree
    void *max;

    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

/**
 * Insert a node into the tree.
 *
 * @param void *addr - The starting address of the memory block.
 * @param void *addrHigh - The ending address of the memory block.
 * @param bool freed - If the block is a freed block.
 */
void insert(void *addr, void *addrHigh, bool freed);

/**
 * Deletes a node from the tree. 
 *
 * @param Node *curr - The node to delete.
 * @return - Returns the node being removed.
 */	
Node *delete(Node *node);

/**
 * Searches for a given key. 
 *
 * @param void *addr - The key.
 * @return - Returns the node if it is found, NULL otherwise.
 */
Node *findNode(void *addr);

/**
 * Searches for any nodes that overlap with the given interval.
 *
 * @param void *low - Low point of the interval.
 * @param void *high - High point of the interval.
 * @return - Returns an overlapping node if found, NULL otherwise.
 */
Node *intervalSearch(void *low, void *high);

#endif /* RBTREE_H */
