all: angles

angles: angles.cpp
	g++ -Wall -O3 angles.cpp -o angles.so -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` `python3-config --extension-suffix` 

clean:
	rm angles

test:
	python3 -m pytest.py

