import pytest
import numpy as np
import random
import timeit
import _matrix

def matrix_implement(x, y, z, tsize):
    print(f"{x}, {y}, {z}, {tsize}")
    m1 = np.random.rand(x, y)
    m2 = np.random.rand(y, z)
    npdot = np.dot(m1, m2)

    _m1 = _matrix.Matrix(m1.shape[0], m1.shape[1])
    _m2 = _matrix.Matrix(m2.shape[0], m2.shape[1])
    _m1.load_from_array(m1)
    _m2.load_from_array(m2)

    naive = _matrix.multiply_naive(_m1, _m2)
    tile = _matrix.multiply_tile(_m1, _m2, tsize)
    mkl = _matrix.multiply_mkl(_m1, _m2)

    for i in range(npdot.shape[0]):
        for j in range(npdot.shape[1]):
            assert npdot[i, j] == pytest.approx(naive[i, j])
            assert npdot[i, j] == pytest.approx(tile[i, j])
            assert npdot[i, j] == pytest.approx(mkl[i, j])


def test_matrix_match():
    for _case in range(3):
        x = random.randint(1, 300)
        y = random.randint(1, 300)
        z = random.randint(1, 300)
        tsize = random.randint(1, 16)
        matrix_implement(x, y, z, tsize)


def test_performance():
    x, y, z, tsize = 1000, 1000, 1000, 16
    m1 = np.random.rand(x, y)
    m2 = np.random.rand(y, z)
    _m1 = _matrix.Matrix(m1.shape[0], m1.shape[1])
    _m2 = _matrix.Matrix(m2.shape[0], m2.shape[1])
    _m1.load_from_array(m1)
    _m2.load_from_array(m2)

    init = dict(_matrix=_matrix, _m1=_m1, _m2=_m2, tsize=tsize)
    naive = timeit.timeit("_matrix.multiply_naive(_m1, _m2)", number=1, globals=init)
    tile = timeit.timeit("_matrix.multiply_tile(_m1, _m2, tsize)", number=1, globals=init)
    mkl = timeit.timeit("_matrix.multiply_mkl(_m1, _m2, tsize)", number=1, globals=init)

    with open("performance.txt", "w") as f:
        f.write(f"multiply_naive: {naive:.4f} seconds.\n")
        f.write(f"multiply_tile: {tile:.4f} seconds.\n")
        f.write(f"multiply_mkl: {mkl:.4f} seconds.\n")
