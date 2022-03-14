# declare variables
CC = g++

main: vector.cpp
	$(CC) -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` vector.cpp -o _vector`python3-config --extension-suffix` `python3-config --ldflags`

test:
	pytest test_calc_rad.py

clean:
	rm -rf __pycache__
	rm *.so 
