CXX = g++
MKL_INCLUDE = /usr/include/mkl
MKL_LIB = /usr/lib/x86_64-linux-gnu/mkl
TAG = -O3 -Wall -shared -std=c++17 -fPIC

_matrix: _matrix.cpp
	$(CXX) $(TAG) `python3 -m pybind11 --includes` $< -o $@.so -L$(MKL_LIB) -I$(MKL_INCLUDE) -lblas
.PHONY: test
test:
	python3 -m pytest
.PHONY: clean
clean:
	rm -rf *.so .pytest_cache tests/__pycache__ *.txt