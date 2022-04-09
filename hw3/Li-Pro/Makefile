SHELL = /bin/bash

LIBMATRIX = libmatrix$(shell python3-config --extension-suffix)
PYBIND11_INCLUDE = $(shell python3 -m pybind11 --includes)
CPP = $(wildcard *.cpp)

# pybind vars
PYTHON_INCLUDE = $(shell python3 -c 'print(__import__("sysconfig").get_paths()["include"])')
PYTHON_SRCDIR = $(shell python3 -c 'print(__import__("sysconfig").get_config_vars()["srcdir"])')
PYTHON_LIB = $(shell python3 -c 'print(__import__("sysconfig").get_config_vars()["PYTHON_FOR_REGEN"])')

# MKL vars
ifndef IFACE_COMP_PART
    IFACE_COMP_PART = intel
endif

ifndef interface
    interface = lp64
endif

MKL_IFACE_LIB = mkl_$(IFACE_COMP_PART)_$(interface)
MKL_THREADING_LIB = mkl_sequential
MKL_CORE_LIB = mkl_core
MKL_LINK = -l $(MKL_IFACE_LIB) -l $(MKL_THREADING_LIB) -l $(MKL_CORE_LIB)

# Test vars
RES_FILE = performance.txt

.PHONY: all test clean

all: $(LIBMATRIX)

benchmark: $(LIBMATRIX)
	python3 test_matrix.py | tee $(RES_FILE)

test: $(LIBMATRIX) benchmark
	env PYTHONPATH=".:$$PYTHONPATH" python3 -m pytest -v

clean:
	rm -rf $(LIBMATRIX) $(RES_FILE) __pycache__ *.a *.so

$(LIBMATRIX): $(CPP)
	c++ \
		-O3 \
		-Wall \
		-Wno-unknown-pragmas \
		-Werror \
		-shared \
		-std=c++11 \
		-fPIC \
		-Wl,--no-as-needed \
		-o $(LIBMATRIX) \
		-I $(PYTHON_INCLUDE) \
		-L $(PYTHON_SRCDIR) \
		-l $(PYTHON_LIB) \
		$(PYBIND11_INCLUDE) \
		$(MKL_LINK) \
		$(CPP)
