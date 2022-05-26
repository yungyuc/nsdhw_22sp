CXX = g++
SRC = ../mod.cpp
EXE = _matrix$(shell python3-config --extension-suffix)

FLAGS = -O3 -Wall -shared -fPIC -std=c++17 -I./
PYBD11 = -I /usr/include/python3.8 -lpython3.8 -I /usr/include/pybind11 `python3 -m pybind11 --includes` `python3-config --includes`
NUMPY = -I`python3 -c 'import numpy;print(numpy.get_include())'`
BLAS = -I /usr/include/mkl/ -L /usr/lib/ -I/usr/include/openblas/openblas/ -lblas

.PHONY: clean

# SRC must put before BLAS #
all: $(SRC)
	$(CXX) $(SRC) $(FLAGS) $(PYBD11) $(BLAS) $(NUMPY) -o $(EXE) 

test: all
	python3 -m pytest test_matrix.py

clean:
	rm -rf *.so .pytest_cache __pycache__