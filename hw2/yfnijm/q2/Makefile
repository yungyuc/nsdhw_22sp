all:_vector

_vector: vectors_angle.cpp
	g++ -O3 vectors_angle.cpp -o _vector.so -shared -fPIC -I/usr/include/python3 -I/usr/include/pybind11  `python3 -m pybind11 --includes`


test:
	python3 -m pytest

clean:
	rm -rf *.so  __*
