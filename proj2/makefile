COMMAND = gcc
XNAME = prodcomm
FLAGS = -Wall -g -Wextra -Wpedantic
SCAN_BUILD_DIR = scan-build-out

all: compile 

compile: main.o module.o threadDelegator.o
	$(COMMAND) $(FLAGS) -v -o $(XNAME) *.o -lpthread

main.o: main.c module.h
	$(COMMAND) $(FLAGS) -c main.c

module.o: module.c module.h
	$(COMMAND) $(FLAGS) -c module.c

threadDelegator.o: threadDelegator.c threadDelegator.h
	$(COMMAND) $(FLAGS) -c threadDelegator.c

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
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.htm