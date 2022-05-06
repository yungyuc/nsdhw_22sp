CXX = g++
CFLAGS = -shared -std=c++17  -fPIC -m64
PWD = $(shell pwd)
MKL_Flags = -L/usr/include/mkl/intel64 -I/usr/include/mkl/ -lmkl_rt -Wl,--no-as-needed -lpthread -lm -ldl -lblas
MKL_DIR := /usr/lib/x86_64-linux-gnu
MKL_LIBS := ${MKL_DIR}/libmkl_def.so \
		        ${MKL_DIR}/libmkl_avx2.so \
		        ${MKL_DIR}/libmkl_core.so \
		        ${MKL_DIR}/libmkl_intel_lp64.so \
		        ${MKL_DIR}/libmkl_sequential.so

INC = -I/usr/include/python3.8 -I/usr/include/pybind11 -lpython3.8

_matrix:
	$(CXX) $(CFLAGS) $(MKL_Flags) $(MKL_LIBS) `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` $(INC)

.PHONY: clean

clean:
	rm -rf *.so __pycache__ .pytest_cache