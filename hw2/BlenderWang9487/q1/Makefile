CXX = g++
STD = c++17

Line: Line.cpp Line.hpp
	$(CXX) --std=$(STD) $< -o $@

.PHONY: clean
clean:
	rm -rf Line

.PHONY: run
run: Line
	./Line

.PHONY: check
check:
	./Line > result.txt