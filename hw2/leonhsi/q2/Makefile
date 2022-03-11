all: _vector.so

_vector.so: _vector.cpp
	g++ -Wall -O3 _vector.cpp -o _vector.so -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` `python3-config --ldflags`  

clean:
	rm -rf *.so __pycache__

test:
	python3 -m pytest

