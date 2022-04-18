#!/bin/bash

''':'
export PYTHONPATH=".:$PYTHONPATH"
export VECLIB_MAXIMUM_THREADS=1
export MKL_NUM_THREADS=1
export NUMEXPR_NUM_THREADS=1
export OMP_NUM_THREADS=1

if [ -f "PRELOAD_MKL" ] ; then
    PRELOAD_MKL=1
fi

test_path="${BASH_SOURCE[0]}"

if [[ (-n "$PRELOAD_MKL") && ("Linux" == "$(uname)") ]] ; then
    # Workaround for cmake + MKL in conda.
    MKL_LIB_DIR=/usr/lib/x86_64-linux-gnu
    MKL_LIBS=$MKL_LIB_DIR/libmkl_def.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_avx2.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_core.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_intel_lp64.so
    MKL_LIBS=$MKL_LIBS:$MKL_LIB_DIR/libmkl_sequential.so
    export LD_PRELOAD=$MKL_LIBS
    echo "INFO: set LD_PRELOAD=$LD_PRELOAD for MKL"
else
    echo "INFO: set PRELOAD_MKL if you see (Linux) MKL linking error"
fi

fail_msg="*** validation failed"

echo "INFO: 'make clean' must work"
make clean ; ret=$?
if [ 0 -ne $ret ] ; then echo "$fail_msg" ; exit $ret ; fi

if [ -n "$(ls _matrix*.so 2> /dev/null)" ] ; then
    echo "$fail_msg for uncleanness"
    exit 1
fi

echo "INFO: 'make' should work"
make ; ret=$?
if [ 0 -ne $ret ] ; then echo "$fail_msg" ; exit $ret ; fi

echo "INFO: validating tests should pass"
python3 -m pytest $test_path -v -s ; ret=$?
if [ 0 -ne $ret ] ; then echo "$fail_msg" ; exit $ret ; fi

echo "validation pass"
exit 0
':'''

import sys
import os
import timeit
import unittest

# The python module that wraps the matrix code.
import _matrix


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

    def test_basic(self):

        self.assertEqual(0, _matrix.bytes())

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)
        self.assertEqual(3*8 * size*size, _matrix.bytes())
        base_alloc = _matrix.allocated()
        base_dealloc = _matrix.deallocated()

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

        self.assertEqual(3*8 * size*size, _matrix.bytes())
        self.assertEqual(base_alloc, _matrix.allocated())
        self.assertEqual(base_dealloc, _matrix.deallocated())

    def test_match_naive_mkl(self):

        self.assertEqual(0, _matrix.bytes())

        size = 100
        mat1, mat2, *_ = self.make_matrices(size)
        self.assertEqual(3*8 * size*size, _matrix.bytes())
        base_alloc = _matrix.allocated()
        base_dealloc = _matrix.deallocated()

        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        self.assertEqual(size, ret_naive.nrow)
        self.assertEqual(size, ret_naive.ncol)
        self.assertEqual(size, ret_mkl.nrow)
        self.assertEqual(size, ret_mkl.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertNotEqual(mat1[i,j], ret_mkl[i,j])
                self.assertEqual(ret_naive[i,j], ret_mkl[i,j])

        self.assertEqual(5*8 * size*size, _matrix.bytes())
        self.assertEqual(base_alloc + 2*8 * size*size, _matrix.allocated())
        self.assertEqual(base_dealloc, _matrix.deallocated())

    def test_zero(self):

        self.assertEqual(0, _matrix.bytes())

        size = 200
        mat1, mat2, mat3, *_ = self.make_matrices(size)
        self.assertEqual(3*8 * size*size, _matrix.bytes())
        base_alloc = _matrix.allocated()
        base_dealloc = _matrix.deallocated()

        ret_naive = _matrix.multiply_naive(mat1, mat3)
        ret_mkl = _matrix.multiply_mkl(mat1, mat3)

        self.assertEqual(size, ret_naive.nrow)
        self.assertEqual(size, ret_naive.ncol)
        self.assertEqual(size, ret_mkl.nrow)
        self.assertEqual(size, ret_mkl.ncol)

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                self.assertEqual(0, ret_naive[i,j])
                self.assertEqual(0, ret_mkl[i,j])

        self.assertEqual(5*8 * size*size, _matrix.bytes())
        self.assertEqual(base_alloc + 2*8 * size*size, _matrix.allocated())
        self.assertEqual(base_dealloc, _matrix.deallocated())

    def test_memory(self):

        self.assertEqual(0, _matrix.bytes())
        base_alloc = _matrix.allocated()
        base_dealloc = _matrix.deallocated()

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)
        self.assertEqual(3*8 * size*size, _matrix.bytes())
        # New allocation.
        self.assertEqual(base_alloc + 3*8 * size*size, _matrix.allocated())
        # No deallocation.
        self.assertEqual(base_dealloc, _matrix.deallocated())
        mat1 = mat2 = mat3 = None
        # Matrices are deallocated.
        self.assertEqual(0, _matrix.bytes())
        self.assertEqual(base_dealloc + 3*8 * size*size, _matrix.deallocated())
        self.assertEqual(base_alloc + 3*8 * size*size, _matrix.allocated())

# vim: set fenc=utf8 ff=unix et sw=4 ts=4 sts=4:
