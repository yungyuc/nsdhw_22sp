all:
	c++ -O3 -Wall -shared -std=c++11 -fPIC $(shell python3 -m pybind11 --includes) _vector.cpp -o _vector$(shell python3-config --extension-suffix)

.PHONY: clean
clean:
	rm -f _vector$(python3-config --extension-suffix)
	rm -rf __pycache__ .pytest_cache

.PHONY: test
test:
	make all
	pytest test.py

