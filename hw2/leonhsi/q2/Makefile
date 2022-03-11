all: _vector.so

_vector.so: _vector.cpp
	g++ -Wall -O3 _vector.cpp -o _vector.so -shared -std=c++11 -fPIC -I/usr/include/python3.8 -I/usr/include/pybind11 -lpython3.8 `python3 -m pybind11 --includes`

clean:
	rm -rf *.so __pycache__

test:
	python3 -m pytest

