CC = g++

MKL_INC = -I/usr/include/mkl
PYBIND_INC = -I/usr/include/python3.8/
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas
OBJS = $(shell find -name '*.cpp')


.PHONY: clean test

all:
	$(CC) -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` $(OBJS) -o _matrix`python3-config --extension-suffix` $(PYBIND_INC) $(MKL_INC) $(MKL_LIB)

clean:
	rm -rf __pycache__ .pytest_cache *.o _matrix*.so performance.txt

test:
	python3 -m pytest
# g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` $(PYBIND_INC) $(MKL_INC) $(MKL_LIB)
	