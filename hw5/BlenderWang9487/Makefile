CXX = g++
MKL_INCLUDE = /usr/include/mkl
MKL_LIB = /usr/lib/x86_64-linux-gnu/mkl
TAG = -O3 -Wall -shared -std=c++17 -fPIC -I.
PY = /usr/bin/python3
PYBIND = /home/nfs_home/blender/repos/DummyML/extern/pybind11/include
NUMPY = `python3 -c 'import numpy;print(numpy.get_include())'`

_matrix: ../mod.cpp
#	This cmd is for my computer
#	$(CXX) $(TAG) `$(PY)-config --includes` -I$(PYBIND) -I$(NUMPY) $< -o $@.so -L$(MKL_LIB) -I$(MKL_INCLUDE) -lblas
	$(CXX) $(TAG) `$(PY)-config --includes` -I/usr/include -I$(NUMPY) $< -o $@.so -L$(MKL_LIB) -I$(MKL_INCLUDE) -lblas

.PHONY: clean
clean:
	rm -rf *.so .pytest_cache __pycache__