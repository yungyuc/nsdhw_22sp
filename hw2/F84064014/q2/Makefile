all:
	g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` cppmodule.cpp -o _vector`python3-config --extension-suffix` -I /usr/include/python3.8m

test:
	pytest

clean:
	rm *.so
	rm -rf __pycache__
