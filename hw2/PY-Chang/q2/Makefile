CXX = g++
CFLAGS = -shared -std=c++11 -Wall -fPIC
PWD = $(shell pwd)
INC = -I/usr/include/python3.8 -I/usr/include/pybind11 -lpython3.8

_vector:
	$(CXX) $(CFLAGS) `python3 -m pybind11 --includes` main.cpp -o _vector`python3-config --extension-suffix` $(INC)

.PHONY: test clean

test:
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache