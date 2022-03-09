.PHONY: run check clean
CXX = g++-11

main: Line.o main.o
	$(CXX) $^ -o $@

%.o: %.cpp Line.h
	$(CXX) -c $< -o $@

run: main
	./main

check: main
	./main > result.txt

clean:
	rm -rf main result.txt *.o






