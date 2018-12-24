COMMAND = gcc
XNAME = 537make
FLAGS = -Wall -g -Wextra -Wpedantic -O0
SCAN_BUILD_DIR = scan-build-out

all: compile clean

# this is a comment line

compile: main.o inputparser.o fork.o hashTab.o depgraph.o forkDelegator.o
	$(COMMAND) $(FLAGS) -pthread -lpthread -v -o $(XNAME) *.o

main.o: main.c
	$(COMMAND) $(FLAGS) -c main.c

inputparser.o: inputparser.c inputparser.h
	$(COMMAND) $(FLAGS) -c inputparser.c

fork.o: fork.c fork.h
	$(COMMAND) $(FLAGS) -c fork.c

hashTab.o: hashTab.c hashTab.h
	$(COMMAND) $(FLAGS) -c hashTab.c

depgraph.o: depgraph.c depgraph.h
	$(COMMAND) $(FLAGS) -c depgraph.c

forkDelegator.o: forkDelegator.c forkDelegator.h
	$(COMMAND) $(FLAGS) -c forkDelegator.c

clean:
	rm -f $(XNAME)
	rm -f *.o
	rm -rf $(SCAN_BUILD_DIR)

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html 
run-vg: 
	valgrind --leak-check=full --track-origins=yes --read-var-info=yes 537make
