all:
	g++ -shared -std=c++14 -fPIC `python3 -m pybind11 --includes` matrix.cpp -o _matrix.so -I /usr/include/python3.8 -I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lmkl_rt 

clean:
	rm -f *.so
	rm -rf __pycache__
	rm -f *.txt
