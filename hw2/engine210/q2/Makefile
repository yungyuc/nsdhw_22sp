CXX = c++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
INCLUDES = $(shell python3 -m pybind11 --includes)
TARGETS = _vector$(shell python3-config --extension-suffix)

$(TARGETS): _vector.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

.PHONY: clean
clean:
	rm -rf $(TARGETS) __pycache__

.PHONY: test
test:
	pytest