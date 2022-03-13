CXX = c++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PY_INCLUDES = -I/usr/include/python3.8 -lpython3.8
PYBIND_INCLUDES = $(shell python3 -m pybind11 --includes)
TARGETS = _vector$(shell python3-config --extension-suffix)

$(TARGETS): _vector.cpp
	$(CXX) $(CXXFLAGS) $(PY_INCLUDES) $(PYBIND_INCLUDES) $^ -o $@

.PHONY: clean
clean:
	rm -rf $(TARGETS) __pycache__

.PHONY: test
test:
	python3 -m pytest