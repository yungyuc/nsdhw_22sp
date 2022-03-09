Angle: Angle.cpp
#	g++ -O3 -Wall -shared -std=c++17 -fPIC `python3 -m pybind11 --includes` $< -o $@.so
	g++ -O3 -Wall -shared -std=c++17 -fPIC `pybind11 --includes` $< -o $@.so
.PHONY: clean
clean:
	rm -rf *.so .pytest_cache tests/__pycache__

.PHONY: test
test: Angle.so
	python3 -m pytest