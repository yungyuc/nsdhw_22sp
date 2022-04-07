all:  _matrix.cpp
	g++ -O3 -shared -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` `python3-config --includes` -lblas -I/usr/include/mkl -L/usr/lib/x86_64-linux-gnu

test: all
	python3 -m pytest test_matrix.py

clean:
	rm -rf *.txt *.o *.so __pycache__

