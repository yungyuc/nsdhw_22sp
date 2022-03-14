# declare variables
CC = g++

main: main.cpp
	$(CC) main.cpp -std=c++11 -o main

run: main
	./main

check: main
	./main > result.txt
	
clean:
	rm main