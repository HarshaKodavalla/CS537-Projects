## Shared Memory Producer/Consumer Using Synchronization

This program takes text input and replaces all spaces with asteriks, and converts
all lowercase letters to uppercase. Four different threads are used to implement
this behavior:

- Thread 1: The Reader thread will read from standard input, one line at a time.
  Reader will take the each line of the input and pass it to thread Munch1 through a queue of character strings.

- Thread 2: Munch1 will scan the line and replace each space character (not tabs or newlines) with an asterisk ("*") character. 
  It will then pass the line to thread Munch2 through another queue of character strings.

- Thread 3: Munch2 will scan the line and convert all lower case letters to upper case (e.g., convert "a" to "A"). 
  It will then pass the line to thread Writer though yet another queue of character strings.

- Thread 4: Writer will write the line to standard output.


To compile the program:

%make scan-build

to run the progarm:

%./prodcomm

You may type input or pass a file as input using stdin.

To clean the directory:

%make clean

Authors:

Harsha Kodavalla
Julius Brendle

Copyright 2018
