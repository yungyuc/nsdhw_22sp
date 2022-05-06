sources = _matrix.cpp allocator.cpp byte_counter.cpp
exe = _matrix
CC = g++

cflag = -O3 -Wall -shared -std=c++11 -fPIC
pyflag = $(shell python3-config --includes)
pyext = $(shell python3-config --extension-suffix)
pybind = -I/usr/include/pybind11 -lpython3.8
BLAS = -I/usr/include/mkl -L/usr/lib/x86_64-linux-gnu/mkl -lblas

# local
# pybind = -I/usr/include/python3.8 $(shell python3 -m pybind11 --includes)
# BLAS = -I/opt/intel/oneapi/mkl/2022.0.2/include -L/opt/intel/oneapi/mkl/2022.0.2/lib/intel64 -lblas

all: $(sources)
	$(CC) $(cflag) $(pyflag) $(pybind) $(sources) -o $(exe)$(pyext) $(BLAS)

test: $(sources)
	python3 -m pytest

.PHONY: clean
clean:
	rm -rf $(exe)$(pyext) __pycache__ .pytest*