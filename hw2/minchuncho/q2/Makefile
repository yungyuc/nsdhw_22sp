CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
INCLUDE = -Ipybind11/include -I/usr/include/python3.9
PYFLAG = $(shell python3-config --includes)
PYEXTS = $(shell python3-config --extension-suffix)

OBJS = $(shell find -maxdepth 1 -name '*.cpp')
EXEC = _vector

# Substitution reference
# OBJS := $(OBJS:%.cpp=%.o)

all: $(OBJS) 
	$(CC) $(CFLAGS) $(PYFLAG) $(INCLUDE) $(OBJS) -o $(EXEC)$(PYEXTS)

.PHONY: test
test:
	PYTHONPATH=./src pytest

.PHONY: clean
clean:
	$(RM) $(OBJS) $(EXEC)
