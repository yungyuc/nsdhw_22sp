all:
	cp ./matrix.hpp ../matrix.hpp
	g++ -std=c++17 -O3 -Wall -shared -fPIC -I. `/usr/bin/python3-config --includes` `python3 -m pybind11 --includes` ../mod.cpp -o _matrix.so -I /usr/include/python3.8 -I /usr/include/numpy -I /usr/include/mkl -I /usr/include -L /usr/lib/x86_64-linux-gnu/mkl -lmkl_rt
	rm ../matrix.hpp

clean:
	rm -rf *.so .pytest_cache __pycache__ 
