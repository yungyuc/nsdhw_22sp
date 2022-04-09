#!/usr/bin/python3

import pytest
import numpy as np
import timeit
import _matrix

@pytest.fixture
def generate_matrix():
	size = 1000
	tsize = 16
	mat1 = _matrix.Matrix(size, size, np.random.rand(1, size*size).flatten())
	mat2 = _matrix.Matrix(size, size, np.random.rand(1, size*size).flatten())
	return mat1, mat2, tsize

def test_matrix_multiplication(generate_matrix):
	mat1, mat2, tsize = generate_matrix

	naive = _matrix.multiply_naive(mat1, mat2)
	tile = _matrix.multiply_tile(mat1, mat2, tsize)
	mkl = _matrix.multiply_mkl(mat1, mat2)
	
	assert naive == tile
	assert naive == mkl

def test_performace(generate_matrix):
	mat1, mat2, tsize = generate_matrix

	ns = dict(_matrix=_matrix, mat1=mat1, mat2=mat2, tsize=tsize)
	t_naive = timeit.timeit('_matrix.multiply_naive(mat1, mat2)', globals=ns, number=1)
	t_tile = timeit.timeit('_matrix.multiply_tile(mat1, mat2, tsize)', globals=ns, number=1)
	t_mkl = timeit.timeit('_matrix.multiply_mkl(mat1, mat2)', globals=ns, number=1)

	times = (t_naive/t_tile)-1
	
	f = open("performance.txt", "w")    
	f.write(f'multiply_naive: {t_naive: 10.4f} seconds.\n') # python3.6 f-string
	f.write(f'multiply_tile: {t_tile: 10.4f} seconds.\n')
	f.write(f'multiply_mkl: {t_mkl: 10.4f} seconds.\n')
	f.write(f'multiply_tile runs faster than multiply_naive by {times: 10.4f} times.\n')
	f.close()
