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

echo "INFO: 'make test' should work"
make test ; ret=$?
if [ 0 -ne $ret ] ; then echo "$fail_msg" ; exit $ret ; fi

# Benchmark
echo "INFO: benchmark"
python3 $test_path

echo "INFO: validating tests should pass (except timing on CI)"
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

    def test_match_naive_mkl(self):

        size = 100
        mat1, mat2, *_ = self.make_matrices(size)

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

    def test_zero(self):

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)

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

    def check_tile(self, mat1, mat2, tsize):

        if 0 == tsize:
            ret_tile = _matrix.multiply_naive(mat1, mat2)
            tile_str = "_matrix.multiply_naive(mat1, mat2)"
        else:
            ret_tile = _matrix.multiply_tile(mat1, mat2, tsize)
            tile_str = "_matrix.multiply_tile(mat1, mat2, tsize)"
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        for i in range(ret_tile.nrow):
            for j in range(ret_tile.ncol):
                self.assertNotEqual(mat1[i,j], ret_mkl[i,j])
                self.assertEqual(ret_tile[i,j], ret_mkl[i,j])

        ns = dict(_matrix=_matrix, mat1=mat1, mat2=mat2, tsize=tsize)
        t_tile = timeit.Timer(tile_str, globals=ns)
        t_mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', globals=ns)

        time_tile = min(t_tile.repeat(10, 1))
        time_mkl = min(t_mkl.repeat(10, 1))
        ratio = time_tile/time_mkl

        return ratio, time_tile

    def test_tile(self):

        show_ratio = bool(os.environ.get('SHOW_RATIO', False))

        mat1, mat2, *_ = self.make_matrices(500)

        ratio0, time0 = self.check_tile(mat1, mat2, 0)
        if show_ratio:
            print("naive ratio:", ratio0)

        ratio16, time16 = self.check_tile(mat1, mat2, 16)
        if show_ratio:
            print("tile 16 ratio:", ratio16)
            print("time16/time0:", time16/time0)
        self.assertLess(ratio16/ratio0, 0.8)

        ratio17, time17 = self.check_tile(mat1, mat2, 17)
        if show_ratio:
            print("tile 17 ratio:", ratio17)
            print("time17/time0:", time17/time0)
        self.assertLess(ratio17/ratio0, 0.8)

        ratio19, time19 = self.check_tile(mat1, mat2, 19)
        if show_ratio:
            print("tile 19 ratio:", ratio19)
            print("time19/time0:", time19/time0)
        self.assertLess(ratio19/ratio0, 0.8)


class Writer:

    def __init__(self, streams):

        self.streams = streams

    def write(self, msg):

        for stream in self.streams:

            stream.write(msg)


def benchmark():

    setup = '''
import _matrix

size = 1000

mat1 = _matrix.Matrix(size,size)
mat2 = _matrix.Matrix(size,size)

for it in range(size):
    for jt in range(size):
        mat1[it, jt] = it * size + jt + 1
        mat2[it, jt] = it * size + jt + 1
'''

    naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', setup=setup)
    mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', setup=setup)

    repeat = 5

    with open('performance.txt', 'w') as fobj:

        w = Writer([sys.stdout, fobj])

        w.write(f'Start multiply_naive (repeat={repeat}), take min = ')
        naivesec = minsec = min(naive.repeat(repeat=repeat, number=1))
        w.write(f'{minsec} seconds\n')

        w.write(f'Start multiply_mkl (repeat={repeat}), take min = ')
        mklsec = minsec = min(mkl.repeat(repeat=repeat, number=1))
        w.write(f'{minsec} seconds\n')

        w.write('MKL speed-up over naive: %g x\n' % (naivesec/mklsec))

if __name__ == '__main__':
    benchmark()

# vim: set fenc=utf8 ff=unix et sw=4 ts=4 sts=4:
