all:
	g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix` -I /usr/include/python3.10/

test:
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache