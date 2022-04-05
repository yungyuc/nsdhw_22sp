# YABLAS
Yet Another Basic Linear Algebra Subprograms!

## Basic Information
> Github repository: https://github.com/PY-Chang/YABLAS

This is a reimplementation of BLAS based on C++ and Python hybrid system.

## Problem to Solve
In recent years there have been many implementations of the Netlib BLAS, such as Intel MKL, GSL, OpenBLAS, ATLAS, etc. In this project we try to reimplement BLAS based on C++ and wrap it into python to let the users use the same API to call BLAS routines as in C or Fortran.

There has been a project call [PyBlas](https://github.com/timleslie/pyblas) that has implemented BLAS standard in python, however, the linear algebra operation part was been done by pure python without NumPy(performance is not the goal of PyBlas). Therefore, in this project we will try to use C++ to implement BLAS to achieve better performance.

Moreover, according to the paper [Anatomy of High-Performance Matrix Multiplication](https://web.archive.org/web/20100614031840/http://www.cse.uconn.edu/~huang/spring10_5304/pool/reference/HPMM.pdf), most of the BLAS implementations take advantage of memory hierarchy optimization to obtain better `GEMM` performance. Thus, in this project we will try to using the Strassen algorithm in `GEMM` and compare the performance with existing method. 

## Prospective Users
* People who want to try BLAS routines in high level language.
* Algorithm developers who want to prototype their calculations in a high level language with the same APIs they will use in C.<sup>[3]</sup>

## System Architecture
The system would provide a class named `Matrix` to handle 2-D matrix and 1-D vector. Users can simply call the routine they want to perform to obtain the result. Also, we will try to support NumPy array as data input and output for convenience.

## API Description
In this project, we will cover some frequently used double precision BLAS routines.

### Level 1
* DSWAP - swap x and y
* DSCAL - `x = a*x`
* DCOPY - copy x into y
* DAXPY - `y = a*x + y`
* DDOT - dot product
* DNRM2 - Euclidean norm
* DZNRM2 - Euclidean norm
* DASUM - sum of absolute values
* IDAMAX - index of max abs value

### Level 2
* DGEMV - matrix vector multiply
* DSYMV - symmetric matrix vector multiply
* DTRMV - triangular matrix vector multiply
* DTRSV - solving triangular matrix problems
* DGER - performs the rank 1 operation `A := alpha*x*y' + A`

### Level 3
* DGEMM - matrix matrix multiply
* DSYMM - symmetric matrix matrix multiply
* DSYRK - symmetric rank-k update to a matrix
* DTRMM - triangular matrix matrix multiply

## Engineering Infrastructure
1. Using `make` to build the system. 
2. Using `git` to control version.
3. Since the API is build on python, we'll use `pytest` to do unit test.
4. The documentation will be provided as `README.md` on the GitHub repository. 

## Schedule
* Week 1 (4/4): Building the fundamental class for matrix.
* Week 2 (4/11): Implementing level 1 routines and building pytest framework.
* Week 3 (4/18): Implementing level 2 routines(1/2).
* Week 4 (4/25): Implementing level 2 routines(2/2).
* Week 5 (5/2): Implementing level 3 routines.
* Week 6 (5/9): Using `pybind11` to wrap into Python.
* Week 7 (5/16): Benchmarking and optimizing the performance.
* Week 8 (5/23): Refining and preparing the presentation.


## References
1. [Netlib BLAS](http://www.netlib.org/blas/)
2. [BLAS Wiki](https://zh.wikipedia.org/wiki/BLAS)
3. [PyBlas](https://github.com/timleslie/pyblas)
4. [Anatomy of High-Performance Matrix Multiplication](https://web.archive.org/web/20100614031840/http://www.cse.uconn.edu/~huang/spring10_5304/pool/reference/HPMM.pdf)