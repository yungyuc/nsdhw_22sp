CXX = g++

SRC = _matrix.cc
EXE = _matrix$(shell python3-config --extension-suffix)
FLAGS = -O3 -Wall -shared -fPIC
PYBD11 = -I /usr/include/python3.8 -I /usr/include/pybind11 -lpython3.8 `python3 -m pybind11 --includes`
BLAS = -I /usr/include/mkl/ -L /usr/lib/x86_64-linux-gnu/mkl/ -lblas

all: $(SRC)
	$(CXX) $(FLAGS) $(shell python3 -m pybind11 --includes) $(PYBD11) $(BLAS) $(SRC) -o $(EXE)

run: $(SRC)
	$(CXX) $(FLAGS) $(shell python3 -m pybind11 --includes) $(PYBD11) $(BLAS) $(SRC) -o $(EXE)

test: $(SRC)
	$(CXX) $(FLAGS) $(PYBD11) $(SRC) -o $(EXE) $(BLAS)
	python3 -m pytest

clean:
	rm -rf $(EXE) __pycache__ .pytest* performance.txt

