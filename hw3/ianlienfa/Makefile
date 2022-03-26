SHELL=/bin/bash
CXX_COMPILE_FLAG = -O3 -Wall -std=c++17 
INCLUDE_DIR_FLAG = -I/usr/include/python3.8 -I/usr/include/pybind11 -I/usr/include/mkl 
MKL_LAPACK_LINK_FLAG = -L/usr/lib/x86_64-linux-gnu/mkl -lblas
LINK_FLAG = -lpython3.8 $(MKL_LAPACK_LINK_FLAG)
PY_LIB_SUFFIX := $(shell python3-config --extension-suffix)

all:
	c++ $(CXX_COMPILE_FLAG) $(INCLUDE_DIR_FLAG) $(LINK_FLAG) -o Matrix Matrix.cpp

.PHONY: clean
clean:
	rm -f Matrix
	rm -f _matrix.*
	rm -rf __pycache__
	rm -rf .pytest_cache

.PHONY: test
test:
	c++ -shared -fPIC -DFORPYBIND $(CXX_COMPILE_FLAG) $(INCLUDE_DIR_FLAG) $(LINK_FLAG) Matrix.cpp -o _matrix$(PY_LIB_SUFFIX)	

.PHONY: mkl
mkl:
	c++ $(CXX_COMPILE_FLAG) $(INCLUDE_DIR_FLAG) $(LINK_FLAG) Lapack.cpp -o Lapack

# [[ ! -z "`ls | grep Matrix`" ]] && rm -f Matrix
