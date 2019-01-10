## Safe malloc/realloc/free - Augmented alloc library

This program contains special versions of the malloc, realloc and free library routines called 537malloc, 537free and 537realloc. They check for common heap allocation and usage mistakes. This is done by creating an RB-tree with interval augmentation to represent the heap, and using this tree to give more detailed errors when usage errors occur. 

To run the program:

>make compile
>537malloc

This will run the test program located in the main.c file.

To clean:
>make clean

Authors:

Harsha Kodavalla

Copyright 2018
