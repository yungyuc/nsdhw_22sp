_matrix: _matrix.cpp
#	these commands only work on my machine :(
#	g++ -O3 -Wall -shared -std=c++17 -fPIC `python3 -m pybind11 --includes` $< -o $@.so
	g++ -O3 -Wall -shared -std=c++17 -fPIC `python3-config --includes` -I/usr/include $< -o $@.so

.PHONY: clean
clean:
	rm -rf *.so .pytest_cache tests/__pycache__