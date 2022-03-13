CC = g++
CFLAGS = -O3 -Wall -shared -std=gnu++11 -fPIC
INCLUDE = -Iextern/pybind11/include
PYFLAG := $(python3-config --includes)
PYEXTS := $(python3-config --extension-suffix)

# OBJS = $(shell find -name '*.cpp')
OBJS = $(shell find -name '*.cpp')
EXEC = _vector

# Substitution reference
OBJS := $(OBJS:%.cpp=%.o)

all: $(OBJS)
	$(CC) $(CFLAGS) $(PYFLAG) $(INCLUDE) $(OBJS) -o $(EXEC)$(PYEXTS)

test:
	pytest

.PHONY: clean
clean:
	$(RM) $(OBJS) $(EXEC)
