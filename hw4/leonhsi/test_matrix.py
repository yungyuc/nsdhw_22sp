import pytest
import numpy
import time
import _matrix

def valid_res(size, mat_py, mat):
	for it in range(size):
		for jt in range(size):
			assert(mat_py[it, jt] == pytest.approx(mat[it, jt]))
	return 
    
def time_cost(size):
	mat1_py = numpy.random.random((size, size))
	mat2_py = numpy.random.random((size, size))


	time_tmp = time.time()
	res_py = numpy.matmul(mat1_py, mat2_py)
	time_bulidin = time.time() - time_tmp	

	mat1 = _matrix.Matrix(size,size)
	mat2 = _matrix.Matrix(size,size)

	for it in range(size):
		for jt in range(size):
			mat1[it, jt] = mat1_py[it, jt]
			mat2[it, jt] = mat2_py[it, jt]


	time_tmp = time.time()
	res_naive = _matrix.multiply_naive(mat1, mat2)
	time_naive = time.time() - time_tmp
	valid_res(size, res_py, res_naive)

	time_tmp = time.time()
	res_tile = _matrix.multiply_tile(mat1, mat2, 16)
	time_tile = time.time() - time_tmp
	valid_res(size, res_py, res_tile)

	time_tmp = time.time()
	res_mkl = _matrix.multiply_mkl(mat1, mat2)
	time_mkl = time.time() - time_tmp
	valid_res(size, res_py, res_mkl)

	return time_bulidin, time_naive, time_tile, time_mkl

def test_performance():
	time_bulidin, time_naive, time_tile, time_mkl = time_cost(1000)

	fout = open("performance.txt", "w")

	fout.write("Python bulidin time cost :" + str(time_bulidin) + "\n")
	fout.write("Naive time cost :" + str(time_naive) + "\n")
	fout.write("Tile time cost :" + str(time_tile) + "\n")
	fout.write("Mlk time cost :" + str(time_mkl) + "\n")
	fout.write("Ratio of tile / naive :" + str(time_tile / time_naive) + "X\n")
	fout.write("Ratio of tile / mlk :" + str(time_tile / time_mkl) + "X\n")
	fout.write("Ratio of mkl / buildin :" + str(time_mkl / time_bulidin) + "X\n")

	fout.close()
	return
