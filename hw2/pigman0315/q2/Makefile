# declare variables
CC = g++

main: vector.cpp
	pip3 install pybind11
	$(CC) -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` vector.cpp -o _vector`python3-config --extension-suffix`

test:
	python3 -m pytest -v

clean:
	rm -rf __pycache__
	rm *.so 
