sources = ../mod.cpp
exe = _matrix
CC = g++

cflag = -O3 -Wall -shared -std=c++17 -fPIC
pyflag = $(shell python3-config --includes)
pyext = $(shell python3-config --extension-suffix)
pybind = -I/usr/include/pybind11 -lpython3.8
numpy = -I`python3 -c "import numpy; print(numpy.get_include())"`
mkl = -I/usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas

# pybind = -I/usr/include/python3.8 $(shell python3 -m pybind11 --includes)

all: $(sources)
	cp ./matrix.hpp ../matrix.hpp
	$(CC) $(cflag) $(pyflag) $(pybind) $(numpy) -I. $(sources) -o $(exe)$(pyext) $(mkl)
	rm ../matrix.hpp

test: $(sources)
	python3 -m pytest

.PHONY: clean
clean:
	rm -rf $(exe)$(pyext) __pycache__ .pytest*