sources = _vector.cpp
exe = _vector$(shell python3-config --extension-suffix)

compiler = g++
flag = -O3 -Wall -shared -std=c++11 -fPIC

all: $(sources)
	$(compiler) $(flag) -I/usr/include/python3.8 -I/usr/include/pybind11 -lpython3.8 $(sources) -o $(exe)

test:
	python3 -m pytest

clean:
	rm -rf $(exe) __pycache__ .pytest*