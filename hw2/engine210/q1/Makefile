CXX = g++
CXXFLAGS = -std=c++17 -O3
TARGETS = main

.PHONY: all
all: $(TARGETS)

.PHONY: clean
clean:
	rm -f $(TARGETS) result.txt

test: test

run: $(TARGETS)
	./main

check: $(TARGETS)
	./main > result.txt
