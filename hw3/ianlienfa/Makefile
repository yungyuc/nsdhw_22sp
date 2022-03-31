SHELL=/bin/bash
PYTHON_VERSION_SUFFIX := $(shell python -V | awk '{print $$2}' | awk -F'.' '{print $$1"."$$2}')
CXX_COMPILE_FLAG = -O3 -Wall -std=c++17 -g
INCLUDE_DIR_FLAG = -I/usr/include/python$(PYTHON_VERSION_SUFFIX) -I/usr/include/pybind11 -I/usr/include/mkl 
# MKL_LAPACK_LINK_FLAG = -L /usr/lib/x86_64-linux-gnu/ -Wl,--no-as-needed -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl -L/usr/lib/x86_64-linux-gnu/mkl -lblas -llapack 
MKL_LAPACK_LINK_FLAG = -L/usr/lib/x86_64-linux-gnu/mkl -lblas 
LINK_FLAG = -lpython$(PYTHON_VERSION_SUFFIX) $(MKL_LAPACK_LINK_FLAG)
PY_LIB_SUFFIX := $(shell python3-config --extension-suffix)

all:
	c++ $(CXX_COMPILE_FLAG) $(INCLUDE_DIR_FLAG) $(LINK_FLAG) main.cpp Matrix.cpp Tiler.cpp -o main 

.PHONY: clean
clean:
	rm -f Matrix
	rm -f _matrix.*
	rm -rf __pycache__
	rm -rf .pytest_cache
	rm -f main
	rm -f Lapack
	rm -f performance.txt


.PHONY: test
test: 
	c++ -shared -fPIC -DFORPYBIND $(CXX_COMPILE_FLAG) $(INCLUDE_DIR_FLAG) $(LINK_FLAG) Matrix.cpp Tiler.cpp -o _matrix$(PY_LIB_SUFFIX)		

.PHONY: mkl
mkl:
	c++ $(CXX_COMPILE_FLAG) $(INCLUDE_DIR_FLAG) $(LINK_FLAG) Lapack.cpp -o Lapack

# [[ ! -z "`ls | grep Matrix`" ]] && rm -f Matrix
