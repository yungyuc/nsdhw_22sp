# YAMM
Yet Another Matrix Multiplier!

## Basic Information
> Github repository: https://github.com/PY-Chang/YAMM.git

This is a matrix multiplier based on C++ and Python hybrid system. This project implements the classic Strassen algorithm with the help of parallel computing to achieve better performance on modern computer.

## Problem to Solve and Prospective Users
Matrix multiplication is one of the most important linear algebra operation in the modern computing world. For decades, many researches have been invested to improve the efficiency of matrix multiplication. 

In 1969, Volker Strassen developed an algorithm, namely, Strassen algorithm which redeuces the time complicity of the matrix multiplication down to around O(n<sup>2.8074</sup>) in the first time. 


Although in recent years researchers have developed faster algorithms than Strassen's, those algorithms have been viewed as galactic algorithms which means non-practical to present-day computers.

Therefore in this project, we will implement the Strassen algorithm based on C++ and provide an easy-to-use python API to the user who is not familiar with the algorithm and parallel computing.

## System Architecture
The system would provide a class named `Matrix` to handle input and output. Also, for the sake of convenience, the system would be designed to take NumPy array as input and output as well. Once the input matrixes are provided, the system would use the Strassen algorithm to calculate the result of the matrix multiplication.

## API Description
#### Basic API
* `Matrix()`: declare a `Matrix` instance
* `matrix(i, j)`: referencing elements in the matrix
* `(operator) x`: overloading operator `x` to do basic matrix multiplication
* `(operator) +`: overloading operator `+` to do basic matrix addition
* `(operator) -`: overloading operator `-` to do basic matrix subtraction
* `(operator) =`: overloading operator `=` to do assignment
* `(operator) ==`: overloading operator `==` to do matrix comparison

#### Advanced API
> Since the performance of the Strassen algorithm is heavily dependent on the hardware specification of the computer, we will provide additional APIs to let users tweak some parameters of the system to gain better performance based on their machine.

* `Matrix.multiply(matrix, matrix, threshold, thread)`
  * `threshold`: means when the size of the matrix is divided smaller than the threshold, then use the naive approach to do the rest of the multiplication to reduce memory usage and the cost of recursion
  * `thread`: tells the system to use how many threads to calculate the result

## Engineering Infrastructure
1. Using `make` to build the system. 
2. Using `git` to control version.
3. Since the API is build on python, we'll use `pytest` to do unit test.
4. The documentation will be provided as `README.md` on the GitHub repository. 

## Schedule
* Week 1 (4/4): Building the class `Matrix`.
* Week 2 (4/11): Implementing the Strassen algorithm.
* Week 3 (4/18): Improving performance by adding parallel computing.(1/2)
* Week 4 (4/25):  Improving performance by adding parallel computing.(2/2)
* Week 5 (5/2): Using `pybind11` to wrap into Python.
* Week 6 (5/9): Building python unit test to test the correctness of the calculation.
* Week 7 (5/16): Benchmarking and optimizing the performance.
* Week 8 (5/23): Refining and preparing the presentation.


## References
* [Strassen algorithm](https://en.wikipedia.org/wiki/Strassen_algorithm)
* [Computational complexity of matrix multiplication](https://en.wikipedia.org/wiki/Computational_complexity_of_matrix_multiplication#cite_note-strassen69-1)
* [Galactic algorithm](https://en.wikipedia.org/wiki/Galactic_algorithm)
