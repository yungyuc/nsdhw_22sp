all: StopWatch.hpp
	g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` -I /usr/include/python3.8 -I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lmkl_rt 
	# g++ main.cpp -I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lmkl_rt

clean:
	rm -f *.so
	rm -rf __pycache__
	rm -f *.txt

test: StopWatch.hpp
	g++ -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _matrix.cpp -o _matrix`python3-config --extension-suffix` -I /usr/include/python3.8 -I /usr/include/mkl -L /usr/lib/x86_64-linux-gnu/mkl -lmkl_rt
	python3 -m pytest
	python3 test_sample.py
