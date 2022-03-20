.PHONY: run check clean
CXX = g++ 
CXXFLAG = -std=c++17

main: Line.o main.o
	$(CXX) $(CXXFLAG) $^ -o $@

%.o: %.cpp Line.h
	$(CXX) $(CXXFLAG) -c $< -o $@

run: main
	./main

check: main
	./main > result.txt

clean:
	rm -rf main result.txt *.o






