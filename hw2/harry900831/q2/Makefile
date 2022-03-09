.PHONY: test clean
CXX = g++
CXXFLAG = -O3 -Wall -shared -std=c++11 -fPIC # -undefined dynamic_lookup 
PYBINDPATH = -I/usr/include/python3.8 -I/usr/include/pybind11 -lpython3.8
OUTPUTFNAME = _vector.so

_vector: _vector.cpp
	$(CXX) $(CXXFLAG) $(PYBINDPATH) $^ -o $(OUTPUTFNAME)

test: _vector test_vector.py
	python3 -m pytest

clean:
	rm -rf $(OUTPUTFNAME) __pycache__
