CXX = g++
SRC = _matrix.cpp
EXE = _matrix$(shell python3-config --extension-suffix)

FLAGS = -O3 -Wall -shared -fPIC -std=c++17
PYBD11 = -I /usr/include/python3.8 -I /usr/include/pybind11 -lpython3.8 `python3 -m pybind11 --includes`
BLAS = -I /usr/include/mkl/ -L /usr/lib/x86_64-linux-gnu -lblas

.PHONY: clean

# SRC must put before BLAS #
all: $(SRC)
	$(CXX) $(FLAGS) $(shell python3 -m pybind11 --includes) $(SRC) $(PYBD11) $(BLAS) -o $(EXE)	

test: 
	python3 -m pytest test_matrix.py

clean:
	rm -rf *.so .pytest_cache __pycache__
