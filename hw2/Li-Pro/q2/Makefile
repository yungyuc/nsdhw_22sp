LIBVECTOR = libvector$(shell python3-config --extension-suffix)
PYBIND11_INCLUDE = $(shell python3 -m pybind11 --includes)
CPP = $(wildcard *.cpp)

PYTHON_INCLUDE = $(shell python3 -c 'print(__import__("sysconfig").get_paths()["include"])')
PYTHON_LIB = $(shell basename $(PYTHON_INCLUDE))

.PHONY: all test clean

all: $(LIBVECTOR)

test: $(LIBVECTOR)
	env PYTHONPATH=".:$PYTHONPATH" python3 -m pytest -v

clean:
	rm -rf $(LIBVECTOR)

$(LIBVECTOR): $(CPP)
	c++ \
		-O3 \
		-Wall \
		-Werror \
		-shared \
		-std=c++11 \
		-fPIC \
		-o $(LIBVECTOR) \
		-I$(PYTHON_INCLUDE) -l$(PYTHON_LIB) \
		$(PYBIND11_INCLUDE) $(CPP)
