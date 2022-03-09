_vector: _vector.cpp
#	this command only works on my machine :(
	g++ -O3 -Wall -shared -std=c++17 -fPIC `python3 -m pybind11 --includes` $< -o $@.so
#	g++ -O3 -Wall -shared -std=c++17 -fPIC `python3-config --includes` -I/usr/include $< -o $@.so

.PHONY: clean
clean:
	rm -rf *.so .pytest_cache tests/__pycache__

.PHONY: test
test: _vector.so
	python3 -m pytest