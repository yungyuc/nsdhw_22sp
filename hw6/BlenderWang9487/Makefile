CXX = g++
MKL_INCLUDE = /usr/include/mkl
MKL_LIB = /usr/lib/x86_64-linux-gnu/mkl
TAG = -O3 -Wall -shared -std=c++17 -fPIC -I.
PY = /usr/bin/python3
PYBIND = /mnt/es/ness/blender/repos/DummyML/extern/pybind11/include

_matrix.so: _matrix.cpp
#	This cmd is for my computer
#	$(CXX) $(TAG) `$(PY)-config --includes` -I$(PYBIND) $< -o $@ -L$(MKL_LIB) -I$(MKL_INCLUDE) -lblas
	$(CXX) $(TAG) `$(PY)-config --includes` -I/usr/include $< -o $@ -L$(MKL_LIB) -I$(MKL_INCLUDE) -lblas

.PHONY: clean
clean:
	rm -rf *.so .pytest_cache __pycache__

.PHONY: test
test:
	python3 -m pytest validate.py