import _matrix
import pytest
import timeit


def test_size(matrix_init):
    x, y, z, tsize, npdot, _m1, _m2 = matrix_init

    assert x == _m1.nrow
    assert y == _m1.ncol
    assert y == _m2.nrow
    assert z == _m2.ncol


def test_matrix_implement(matrix_init):
    x, y, z, tsize, npdot, _m1, _m2 = matrix_init

    naive = _matrix.multiply_naive(_m1, _m2)
    tile = _matrix.multiply_tile(_m1, _m2, tsize)
    mkl = _matrix.multiply_mkl(_m1, _m2)

    for i in range(npdot.shape[0]):
        for j in range(npdot.shape[1]):
            assert npdot[i, j] == pytest.approx(naive[i, j])
            assert npdot[i, j] == pytest.approx(tile[i, j])
            assert npdot[i, j] == pytest.approx(mkl[i, j])


def test_performance(matrix_init_big):
    x, y, z, tsize, npdot, _m1, _m2 = matrix_init_big

    init = dict(_matrix=_matrix, _m1=_m1, _m2=_m2, tsize=tsize)
    naive = timeit.timeit(
        "_matrix.multiply_naive(_m1, _m2)", number=1, globals=init)
    tile = timeit.timeit(
        "_matrix.multiply_tile(_m1, _m2, tsize)", number=1, globals=init)
    mkl = timeit.timeit("_matrix.multiply_mkl(_m1, _m2)", number=1, globals=init)

    with open("performance.txt", "w") as f:
        f.write(f"multiply_naive: {naive:.4f} seconds.\n")
        f.write(f"multiply_tile: {tile:.4f} seconds.\n")
        f.write(f"multiply_mkl: {mkl:.4f} seconds.\n")
