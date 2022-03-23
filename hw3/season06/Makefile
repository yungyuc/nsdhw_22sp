sources = _matrix.cpp
exe = _matrix$(shell python3-config --extension-suffix)
BLAS = -I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lblas

compiler = g++
flag = -O3 -Wall -shared -std=c++11 -fPIC

all: $(sources)
	# $(compiler) $(flag) $(shell python3 -m pybind11 --includes) $(sources) -o $(exe)
	$(compiler) $(flag) -I/usr/include/python3.8 -I/usr/include/pybind11 -lpython3.8 $(sources) -o $(exe) $(BLAS)

test:
	python3 -m pytest

clean:
	rm -rf $(exe) __pycache__ .pytest*