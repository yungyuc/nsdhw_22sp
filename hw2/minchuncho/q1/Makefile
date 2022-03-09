CC = g++
CFLAGS = -Wall -std=gnu++11 -g
OUTPUT = result.txt

OBJS = $(shell find -name '*.cpp')
EXEC = main

# Substitution reference
OBJS := $(OBJS:%.cpp=%.o)

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

run:
	./$(EXEC)

check:
	./$(EXEC) 1>$(OUTPUT)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(EXEC)