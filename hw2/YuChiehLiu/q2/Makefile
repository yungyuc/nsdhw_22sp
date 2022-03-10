all: _vector.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix`

test:
	python3 -m pytest

clean:
	rm -rf *.so .pytest* __pycache__