CXX = g++
CFLAG = -g -Wall

all: main

main: 
	$(CXX) $(CFLAG) main.cpp -o main

run:
	./main
check: 
	./main > result.txt

.PHONY: clean
clean:
	rm -f main result.txt
