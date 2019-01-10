#ifndef MALLOC537_H
#define MALLOC537_H
#include <stdbool.h> 
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "rbtree.h"

/**
 * Allocates a block of memory of given size. Inserts a node into the tree
 * representing this newly allocated block.
 *
 * @param size_t size - Requested size of the allocated block.
 * @return - Returns a pointer to the block of memory.
 */
void * malloc537(size_t size);

/**
 * Frees a given block of memory. Marks the associated node in the tree as free.
 * Checks to make sure that:
 *      - The freed memory has been allocated by malloc537
 *      - The freed memory is the first byte of the range of allocated memory
 *      - The freed memory has not been previously freed.
 *
 * @param void *ptr - The block of memory to be freed.
 */
void free537(void *ptr);

/**
 * Given a block of memory, alter its size to the requested size. Remove the 
 * old node from the tree, and replace it with the new node.
 *
 * @param void *ptr - The pointer of the block of memory to be reallocated.
 * @param size_t size - Requested size of the reallocated block.
 * @return - Returns a pointer to the block of memory.
 */
void *realloc537(void *ptr, size_t size);

/**
 * Checks to see if the given address range (ptr to ptr + size) is fully within
 * a block of memory allocated by malloc537 or realloc537. 
 *
 * @param void *ptr - The start of the memory range to be checked.
 * @param size_t size - Requested size of the memory range to be checked.
 * @return - Returns a pointer to the block of memory.
 */
void memcheck537(void *ptr, size_t size);

#endif /* MALLOC537_H */
