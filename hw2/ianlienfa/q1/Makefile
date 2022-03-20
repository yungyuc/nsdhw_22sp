CC = g++
COMPILE_FLAG = -std=c++11

all: 
	g++ -std=c++11 main.cpp line.cpp -o main
	
.PHONY: run
run:
	make clean
	make all
	./main

.PHONY: clean
clean: 
	rm -f main
	rm -f *.txt

.PHONY: check
check:
	make clean
	make all
	./main > result.txt
	
	