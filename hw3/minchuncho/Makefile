CC = g++
CFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PYFLAG = $(shell python3-config --includes)
PYEXTS = $(shell python3-config --extension-suffix)

# ubuntu
PYBIND = -I/usr/include/pybind11 -lpython3.8
MKL = -I/usr/include/mkl -L/usr/lib/x86_64-linux-gnu/mkl -lblas
OBJS = $(shell find -name '*.cpp')

EXEC = _matrix

all: $(OBJS)
	$(CC) $(CFLAGS) $(PYFLAG) $(PYBIND) $(OBJS) -o $(EXEC)$(PYEXTS) $(MKL) 

test: $(OBJS)
	$(CC) $(CFLAGS) $(PYFLAG) $(PYBIND) $(OBJS) -o $(EXEC)$(PYEXTS) $(MKL)
	python3 -m pytest

.PHONY: clean
clean:
	$(RM) $(EXEC)$(PYEXTS) *.txt
	$(RM) -rf .pytest_cache __pycache__