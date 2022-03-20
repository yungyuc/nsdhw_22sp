CXX = g++
CFLAGS = -std=c++11 -Wall
PWD = $(shell pwd)
INC = -I $(PWD)/Line.hpp

main: main.o Line.o
	$(CXX) $(INC) $(CFLAGS) $^ -o $@

main.o: main.cpp Line.hpp
	$(CXX) $(INC) $(CFLAGS) -c $< 

Line.o: Line.cpp Line.hpp
	$(CXX) $(INC) $(CFLAGS) -c $<

.PHONY: clean run check

run:
	./main

check:
	./main > result.txt

clean:
	rm -rf main *.o *.txt