sources = main.cpp line.cpp
objects = $(addsuffix .o, $(basename $(sources)))
result_file = result.txt
exe = main

compiler = g++
flag = -std=c++11

all: $(exe)

%.o: %.cpp
	$(compiler) $(flag) -c -o $@ $<

$(exe): $(objects)
	$(compiler) $(flag) -o $@ $^

run:
	./$(exe)

check:
	./$(exe) > $(result_file)

clean:
	rm -rf $(exe) $(objects) $(result_file)