CXX = g++
CFLAGS = -shared -std=c++11 -Wall -fPIC -m64
PWD = $(shell pwd)
MKL = -L/usr/include/mkl/intel64 -I/usr/include/mkl/ -lmkl_rt -Wl,--no-as-needed -lpthread -lm -ldl -lblas
INC = -I/usr/include/python3.8 -I/usr/include/pybind11 -lpython3.8

_matrix:
	$(CXX) $(CFLAGS) $(MKL) `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` $(INC)

.PHONY: test clean

test: _matrix
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache