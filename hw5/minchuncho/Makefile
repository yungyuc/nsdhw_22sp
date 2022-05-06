CC = g++
PYFLAG = $(shell python3-config --includes)
PYEXTS = $(shell python3-config --extension-suffix)

# ubuntu
CFLAGS = -O3 -Wall -shared -std=c++17 -fPIC
PYBIND = -I/usr/include/pybind11 -lpython3.8
NUMPY = -I$(shell python3 -c 'import numpy; import os; print(os.path.dirname(numpy.__file__));')
MKL = -I/usr/include/mkl -L/usr/lib/x86_64-linux-gnu/mkl -lblas
OBJS = $(shell find -name '*.cpp') ../mod.cpp

# osx
# CFLAGS = -O3 -Wall -shared -std=c++17 -fPIC -undefined dynamic_lookup
# PYBIND = -I/usr/local/include/pybind11
# NUMPY = -I/Library/Frameworks/Python.framework/Versions/3.9/lib/python3.9/site-packages/numpy/core/include
# MKL = -I/opt/intel/oneapi/mkl/2022.0.0/include -L/opt/intel/oneapi/mkl/2022.0.0/lib -lblas
# OBJS = matrix.cpp ../mod.cpp

EXEC = _matrix

all: $(OBJS)
	$(CC) $(CFLAGS) $(PYFLAG) $(PYBIND) $(NUMPY) -I. $(OBJS) -o $(EXEC)$(PYEXTS) $(MKL) 

test: $(OBJS)
	python3 -m pytest -s

.PHONY: clean
clean:
	$(RM) $(EXEC)$(PYEXTS) *.txt
	$(RM) -rf .pytest_cache __pycache__