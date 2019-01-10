#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include "rbtree.h"
#include "537malloc.h"

static void *insertHelper(void *low, size_t size);

/**
 * Allocates a block of memory of given size. Inserts a node into the tree
 * representing this newly allocated block.
 *
 * @param size_t size - Requested size of the allocated block.
 * @return - Returns a pointer to the block of memory.
 */
void *malloc537(size_t size) {
    // Guard against no size
    if (size == 0) {
        printf("Warning, size of requested memory is 0.\n");
    }

    void *low = malloc(size);
    if (low == NULL) {
        printf("Malloc error. Terminating.\n");
        exit(1);
    }
    
    // Insert a new node representing the allocated block
    insertHelper(low, size);
    return low;
}

/**
 * Frees a given block of memory. Marks the associated node in the tree as free.
 * Checks to make sure that:
 *      - The freed memory has been allocated by malloc537
 *      - The freed memory is the first byte of the range of allocated memory
 *      - The freed memory has not been previously freed.
 *
 * @param void *ptr - The block of memory to be freed.
 */
void free537(void *ptr) {
    // Search for the block of memory in the tree
    Node *result = findNode(ptr);
    if (result == NULL) {
        /* That specific address is not in the tree */
       
        // See if it lies in an interval
        result = intervalSearch(ptr, ptr);

        if (result == NULL) {
            // Address did not lie iwithin any memory range
            printf("Freeing memory %p that has not been allocated by malloc537().\n", ptr);
            exit(-1);
        } else {
            // Address did lie within a memory range - report errors
            printf("Freeing memory %p that is not the first byte of the range of memory that was allocated.\n", ptr);
            if (result->freed) {
                printf("Memory range was also previously freed.\n");
            }
            exit(-1);
        }
    } else {
        /* Address was found */

        if (result->freed) {
            // Double free error
            printf("Freeing memory %p that was previously freed.\n", ptr);
            exit(-1);
        } else {
            // Correct usage - report no errors
            result->freed = 1;
			free(ptr);
        }
    }
}

/**
 * Given a block of memory, alter its size to the requested size. Remove the 
 * old node from the tree, and replace it with the new node.
 *
 * @param void *ptr - The pointer of the block of memory to be reallocated.
 * @param size_t size - Requested size of the reallocated block.
 * @return - Returns a pointer to the block of memory.
 */
void *realloc537(void *ptr, size_t size) {
    // If given a null pointer, simply call malloc537
    if (ptr == NULL) {
        return malloc537(size);
    }

    // If given size of 0, simply call free537
    if (size == 0) {
        free537(ptr);
        return NULL;
    }

    // Search the tree for the node to be replaced
    Node *old = findNode(ptr);

    if (old == NULL) {
       /* The address is not in the tree */
       
       // See if it lies in an interval
       old = intervalSearch(ptr, ptr);

       if (old == NULL) {
           // Address did not lie within any memory range
           printf("Reallocing memory that has not been allocated by malloc537().\n");
           exit(-1);
       } else {
           // Address did lie within a memory range - report errors
            printf("Reallocing memory that is not the first byte of the range of memory that was allocated.\n");
            if (old->freed) {
                printf("Memory range was also freed.\n");
            }
            exit(-1);
       }
    } else {
        /* Address was found */

        if (old->freed) {
            // Realloc freed block error
            printf("Reallocing memory that was previously freed.\n");
            exit(-1);
        } else {
            // Correct usage - no errors
            // Delete the old node and realloc a new node
            free(delete(old));
            void *newPtr = realloc(ptr, size);
            if (newPtr == NULL) {
                printf("Realloc error. Terminating.\n");
                exit(1);
            } 
            // Insert newly realloc'd node
            return insertHelper(newPtr, size);
        }
    }
}

/**
 * Checks to see if the given address range (ptr to ptr + size) is fully within
 * a block of memory allocated by malloc537 or realloc537. 
 *
 * @param void *ptr - The start of the memory range to be checked.
 * @param size_t size - Requested size of the memory range to be checked.
 * @return - Returns a pointer to the block of memory.
 */
void memcheck537(void *ptr, size_t size) {
    void *low = ptr;
    void *high = low + size;

    // Perform an interval search to find a matching interval
    Node *result = intervalSearch(low, high);

    if (result == NULL) {
        printf("The given range of memory has not been allocated by malloc537.\n");
        exit(-1);
    } else {
        // An interval was found
        if (result->freed) {
            printf("The given range of memory has been freed.\n");
            exit(-1);
        } else {
            if (result->low <= low && result->high >= high) {
                // The interval fully contains the given range
                return;
            } else {
                printf("The given range of memory is not fully contained by a range allocated by malloc537.\n");
                exit(-1);
            }
        }
    }
}

/**
 * Assists with insertion into the RB tree. Handles various overlap cases
 *
 * @param void *low - The start of the memory block.
 * @param size_t size - The size of the memory block.
 * @return - Returns a pointer to the block of memory.
 */
static void *insertHelper(void *low, size_t size) {
    void *high = low + size;

    // To insert, must handle potential splicing
    // Call interval search to find any potential overlaps
    Node *overlapper;

    while (1) { 
        overlapper = intervalSearch(low, high);
        // If no overlaps are found, search is exhausted
        if (overlapper == NULL) {
            break;
        }

        // If an overlapping node exists it must represent a freed block as
        // malloc will not allocate overlapping blocks of memory
		assert(overlapper->freed);

		void *tempLow;
		void *tempHigh;

        // Handle various overlap cases
		
        if (overlapper->low == low && overlapper->high == high) {
            // Case 1: A freed block perfectly matches the alloc'd block

            // Simply set the block to allocated.
            overlapper->freed = 0;
            return low;
        } 
        else if (overlapper->low == low && overlapper->high > high) {
            // Case 2: A freed block starts on the same address as the alloc'd
            // but ends beyond the alloc'd block.

            // Delete the existing node
            tempHigh = overlapper->high;
            free(delete(overlapper));

            // Insert the frontal alloc'd block
            insert(low, high, 0);

            // Insert the rear freed block
            insert(high, tempHigh, 1);
            return low;
        } 
        else if (overlapper->low == low && overlapper->high < high) {
            // Case 3: A freed block starts on the same address as the alloc'd
            // but ends fully within the alloc'd block.

            // Delete the existing node and continue searching for further overlaps.
            free(delete(overlapper));
            continue;
        } 
        else if (overlapper->low < low && overlapper->high == high) {
            // Case 4: A freed block ends on the same address as the alloc'd
            // but extends before the alloc'd block.

            // Delete the existing node
            tempLow = overlapper->low;
            free(delete(overlapper));

            // Insert the frontal freed block
            insert(tempLow, low-1, 1);

            // Insert the rear alloc'd block
            insert(low, high, 0);
            return low;
        } 
        else if (overlapper->low > low && overlapper->high == high) {
            // Case 5: A freed block ends on the same address as the alloc'd
            // but begins fully within the alloc'd block.

            // Delete the existing node and continue searching for further overlaps.
            free(delete(overlapper));
            continue;
        } 
        else if (overlapper->low < low && overlapper->high < high) {
            // Case 6: A freed block intersects with the low interval of
            // the alloc'd block.

            // Delete the existing node
            tempLow = overlapper->low;
            free(delete(overlapper));

            // Insert the frontal freed block and continue searching
            insert(tempLow, low, 1);
            continue;
        } 
        else if (overlapper->low > low && overlapper->high < high) {
            // Case 7: A freed block is fully contained within an alloc'd block

            // Delete the block and continue searching
            free(delete(overlapper));
            continue;
        } 
        else if (overlapper->low > low && overlapper->high > high) {
            // Case 8: A freed block intersects with the high interval of
            // the alloc'd block.

            // Delete the existing node
            tempHigh = overlapper->high;
            free(delete(overlapper));

            // Insert the rear freed block and continue searching
            insert(high, tempHigh, 1);
            continue;
        } 
        else if (overlapper->low < low && overlapper->high > high) {
            // Case 9: A freed block intersects both ends of the alloc'd block.

            // Delete the existing node \
	        tempLow = overlapper->low;
	        tempHigh = overlapper->high;
            free(delete(overlapper));

            // Insert the frontal freed block
            insert(tempLow, low, 1);

            // Insert the center allocated block
	        insert(low, high, 0);

            // Insert the rear freed block
	        insert(high, tempHigh, 1);
            return low;
        } 
        else {
			assert(0); // Should not reach here
		}

    }
    // The search ended and the new block must be added now
    insert(low, high, 0);
    return low;
}
