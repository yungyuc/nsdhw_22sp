CXX = g++
MKL_INCLUDE = /usr/include/mkl
MKL_LIB = /usr/lib/x86_64-linux-gnu/mkl
TAG = -O3 -Wall -shared -std=c++17 -fPIC
PY = /usr/bin/python3

_matrix: _matrix.cpp
	$(CXX) $(TAG) `$(PY) -m pybind11 --includes` $< -o $@.so -L$(MKL_LIB) -I$(MKL_INCLUDE) -lblas
.PHONY: test
test: _matrix benchmark.py
	$(PY) benchmark.py && $(PY) -m pytest

.PHONY: clean
clean:
	rm -rf *.so .pytest_cache __pycache__ *.txt