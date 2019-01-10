COMMAND = gcc
XNAME = 537malloc
FLAGS = -Wall -g

compile: main.o 537malloc.o rbtree.o
	$(COMMAND) $(FLAGS) -o $(XNAME) *.o

main.o: main.c
	$(COMMAND) $(FLAGS) -c main.c

537malloc.o: 537malloc.c 537malloc.h
	$(COMMAND) $(FLAGS) -c 537malloc.c

rbtree.o: rbtree.c rbtree.h
	$(COMMAND) $(FLAGS) -c rbtree.c

clean:
	rm -f $(XNAME)
	rm -f *.o
	rm -rf $(SCAN_BUILD_DIR)