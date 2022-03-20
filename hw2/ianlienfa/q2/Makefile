all:
	c++ -O3 -Wall -shared -std=c++11 -fPIC  -I/usr/include/python3.8 -I/usr/include/pybind11  -lpython3.8  _vector.cpp -o _vector$(shell python3-config --extension-suffix)

.PHONY: clean
clean:
	rm -f _vector$(shell python3-config --extension-suffix)
	rm -rf __pycache__ .pytest_cache

.PHONY: test
test:
	make all
	python -m pytest test.py

