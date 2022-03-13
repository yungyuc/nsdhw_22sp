CC = g++
CFLAGS = -O3 -Wall -shared -std=gnu++11 -fPIC
INCLUDE = -I extern/pybind11/include
CPYTHON = -undefined dynamic_lookup

# OBJS = $(shell find -name '*.cpp')
OBJS = $(find . -maxdepth 1 -name '*.cpp')
EXEC = _vector

# Substitution reference
OBJS := $(OBJS:%.cpp=%.o)

all: $(OBJS)
	$(CC) $(CFLAGS) $(python3-config --include) $(INCLUDE) $(OBJS) -o $(EXEC)$(python3-config --extension-suffix) $(CPYTHON)

test:
	pytest

.PHONY: clean
clean:
	$(RM) $(OBJS) $(EXEC)