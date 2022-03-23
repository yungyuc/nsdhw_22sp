all: StopWatch.hpp
	g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` -I /usr/include/python3.8

clean:
	mkdir hi
	rmdir hi
	rm *.so

test: StopWatch.hpp
	g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` -I /usr/include/python3.8
	pytest
