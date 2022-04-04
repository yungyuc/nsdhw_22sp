import sys
import os
import timeit
import unittest
sys.path.append('./modules')

# The python module that wraps the matrix code.
import _matrix


class GradingTest(unittest.TestCase):

    def make_matrices(self, size):
        print("Making matrices")

        mat1 = _matrix.Matrix(size,size)
        mat2 = _matrix.Matrix(size,size)
        mat3 = _matrix.Matrix(size,size)

        for it in range(size):
            for jt in range(size):
                mat1[it, jt] = it * size + jt + 1
                mat2[it, jt] = it * size + jt + 1
                mat3[it, jt] = 0

        print("Finished making matrices")
        return mat1, mat2, mat3
    def test_zero(self):
        print("Test zero")
        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)
        print("Made the zero matrices")

        print("Multiplying zero native")
        ret_naive = _matrix.multiply_naive(mat1, mat3)
        print("Multiplying zero mekl")
        # ret_mkl = _matrix.multiply_mkl(mat1, mat3)

        print("Daiyobu")
        print("Size on the other hand : {}".format(size))
        print("Asserting zero size : {}".format(ret_naive.nrow))
        self.assertEqual(size, ret_naive.nrow)
        print("Asserting zero 1")
        self.assertEqual(size, ret_naive.ncol)
        print("Asserting zero 2")
        self.assertEqual(size, ret_mkl.nrow)
        print("Asserting zero 3")
        self.assertEqual(size, ret_mkl.ncol)

        print("Asserting zero specifics")
        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertEqual(0, ret_naive[i,j])
                self.assertEqual(0, ret_mkl[i,j])
        print("Done with zero testing")



if __name__ == '__main__':
    unittest.main()

# vim: set fenc=utf8 ff=unix et sw=4 ts=4 sts=4:
