MKL_INC = -I/usr/include/mkl
PYBIND_INC = -I/usr/include/python3.8/
MKL_LIB = -L/usr/lib/x86_64-linux-gnu -lblas
OUT = _matrix$(shell python3-config --extension-suffix)

.PHONY: clean test

all:
	g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` $(PYBIND_INC) $(MKL_INC) $(MKL_LIB)

clean:
	rm -rf __pycache__ .pytest_cache *.o _matrix*.so performance.txt

test:
	g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` $(PYBIND_INC) $(MKL_INC) $(MKL_LIB)
	python3 -m pytest ../validate.py