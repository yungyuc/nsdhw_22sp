
all:  ../mod.cpp
	g++ -O2 -shared -fPIC `python3 -m pybind11 --includes` -I./ -I`python3 -c "import numpy; print(numpy.get_include())"` ../mod.cpp -o _matrix`python3-config --extension-suffix` `python3-config --includes` -lblas -I/usr/include/mkl -L/usr/lib/x86_64-linux-gnu 	
test: all
	python3 -m pytest unit_test.py

clean:
	rm -rf  *.o *.so __pycache__ *.txt
