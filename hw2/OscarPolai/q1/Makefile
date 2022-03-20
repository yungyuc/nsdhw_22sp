#.PHONY: run check clean
CXX = g++ 
CFLAG = -std=c++11 -Wall

main: Line.o main.o
	$(CXX) $(CFLAG) $^ -o $@

%.o: %.cpp Line.h
	$(CXX) $(CFLAG) -c $< -o $@

check: main
	./main > result.txt

clean:
	rm -rf main *.txt *.o

run: main
	./main
