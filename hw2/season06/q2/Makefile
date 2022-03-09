sources = _vector.cpp
exe = _vector$(shell python3-config --extension-suffix)

compiler = g++
flag = -O3 -Wall -shared -std=c++11 -fPIC

all: $(sources)
	$(compiler) $(flag) $(shell python3 -m pybind11 --includes) $(sources) -o $(exe)

test:
	python3 -m pytest

clean:
	rm -rf $(exe) __pycache__ .pytest*