537make - A simple version of the make command

This program searches for a file named makefile or Makefile, and builds according to the file's specifications.
If no specific build target is given, it will build the first target listed. 537make lacks the ability to read
makefiles with variables so any makefile given must contain no variables. A makefile with cyclic dependencies is 
also not allowed.

To run the program:

>make compile
>537make

This will read from "makefile". If "makefile" does not exist, it will try to read from "Makefile" instead.

To run the program on a specific build specification:

>537make [target_name]

To clean:
>make clean

Authors:

Harsha Kodavalla
Julius P. Brendle

Copyright 2018
