import sys
import os
import timeit
import unittest
sys.path.append('./modules')

# The python module that wraps the matrix code.
import _matrix
import random
import time


class GradingTest(unittest.TestCase):

    def make_matrices(self, size):

        mat1 = _matrix.Matrix(size,size)
        mat2 = _matrix.Matrix(size,size)
        mat3 = _matrix.Matrix(size,size)

        for it in range(size):
            for jt in range(size):
                mat1[it, jt] = it * size + jt + 1
                mat2[it, jt] = it * size + jt + 1
                mat3[it, jt] = 0

        return mat1, mat2, mat3

    def make_random_matrices(self, size):

        mat1 = _matrix.Matrix(size,size)
        mat2 = _matrix.Matrix(size,size)
        mat3 = _matrix.Matrix(size,size)

        for it in range(size):
            for jt in range(size):
                mat1[it, jt] = random.gauss(0, 1)
                mat2[it, jt] = random.gauss(0, 1)
                mat3[it, jt] = 0

        return mat1, mat2, mat3

    def test_indexing(self):
        m1 = _matrix.ident(3,3)
        assert m1[0,0] == 1
        assert m1[1,1] == 1
        assert m1[0,1] == 0
        assert m1[1,0] == 0

    def test_basic(self):

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        self.assertEqual(size, mat1.nrow)
        self.assertEqual(size, mat1.ncol)
        self.assertEqual(size, mat2.nrow)
        self.assertEqual(size, mat2.ncol)
        self.assertEqual(size, mat3.nrow)
        self.assertEqual(size, mat3.ncol)

        self.assertEqual(2, mat1[0,1])
        self.assertEqual(size+2, mat1[1,1])
        self.assertEqual(size*2, mat1[1,size-1])
        self.assertEqual(size*size, mat1[size-1,size-1])

        for i in range(mat1.nrow):
            for j in range(mat1.ncol):
                self.assertNotEqual(0, mat1[i,j])
                self.assertEqual(mat1[i,j], mat2[i,j])
                self.assertEqual(0, mat3[i,j])

        self.assertEqual(mat1, mat2)
        self.assertTrue(mat1 is not mat2)

    def test_match_naive_mkl_tile(self):

        size = 100
        mat1, mat2, *_ = self.make_random_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)
        ret_tile = _matrix.multiply_tile(mat1, mat2,16)

        self.assertEqual(size, ret_naive.nrow)
        self.assertEqual(size, ret_naive.ncol)
        self.assertEqual(size, ret_mkl.nrow)
        self.assertEqual(size, ret_mkl.ncol)
        self.assertEqual(size, ret_tile.nrow)
        self.assertEqual(size, ret_tile.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertEqual(ret_naive[i,j], ret_mkl[i,j])
                self.assertEqual(ret_naive[i,j], ret_tile[i,j])

        # Now test the operator==
        self.assertEqual(ret_naive, ret_mkl)
        self.assertEqual(ret_mkl, ret_tile)

    def test_zero(self):

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        ret_naive = _matrix.multiply_naive(mat1, mat3)
        ret_mkl = _matrix.multiply_mkl(mat1, mat3)
        ret_tile= _matrix.multiply_tile(mat1, mat3,16)

        self.assertEqual(size, ret_naive.nrow)
        self.assertEqual(size, ret_naive.ncol)
        self.assertEqual(size, ret_mkl.nrow)
        self.assertEqual(size, ret_mkl.ncol)
        self.assertEqual(size, ret_tile.nrow)
        self.assertEqual(size, ret_tile.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertEqual(0, ret_naive[i,j])
                self.assertEqual(0, ret_mkl[i,j])
                self.assertEqual(0, ret_tile[i,j])


if __name__ == '__main__':
    random.seed(time.time_ns())
    unittest.main()
# vim: set fenc=utf8 ff=unix et sw=4 ts=4 sts=4:
