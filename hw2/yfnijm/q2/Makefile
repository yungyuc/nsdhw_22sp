

all:
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` vectors_angle.cpp -o _vector`python3-config --extension-suffix` `python3-config --ldflags`


test:
	python3 -m pytest

clean:
	rm -rf *.so  __*
