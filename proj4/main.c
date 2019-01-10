#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "537malloc.h"

typedef struct {
	void *addr;
	size_t size;
} MEMNODE;

#define ARRAY_SIZE 1000
#define MAXITER 1000*1000*1000
#define MAXSIZE 1000

/**
 * Test function that randomly allocates, frees and reallocates memory
 * over a large number of iterations.
 */	
int main()
{
	MEMNODE mem[ARRAY_SIZE];
	int i, j;
	size_t size;

	// Initialize the array
	for (i = 0; i < ARRAY_SIZE; i++) {
		mem[i].addr = NULL;
		mem[i].size = 0;
	}

	// Test allocating and freeing for a large number of times
	for (j = 0; j < MAXITER; j++) {
		// Pick a random location in the array
		i = rand() % ARRAY_SIZE;

		if (j%10000 == 0) {
			fprintf(stdout, "\n\n****** %dth iteration\n", j);
		}

		// If unoccupied, malloc, otherwise free
		if (mem[i].addr == NULL) {
			size = rand() % MAXSIZE + 1;
			mem[i].addr = malloc537(size);
			if (mem[i].addr == NULL) {
				fprintf(stderr, "ERROR safemalloc returned null\n");
				exit(-1);
			}
			mem[i].size = size;
			// Check the range in between
			if (size > 10) {
				memcheck537((char *)(mem[i].addr) +size/2, size/4);
			}
		} else {
			free537(mem[i].addr);
			mem[i].addr = NULL;
		}
	}
}
	
