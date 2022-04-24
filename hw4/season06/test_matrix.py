import _matrix
import pytest


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


def test_memory_bytes():
    row, col = 10, 15

    base_alloc = _matrix.allocated()
    base_dealloc = _matrix.deallocated()

    m1 = _matrix.Matrix(row, col)
    assert _matrix.bytes() == 8 * row * col
    assert _matrix.allocated() == base_alloc + 8 * row * col
    assert _matrix.deallocated() == base_dealloc

    m2 = _matrix.Matrix(row, col)
    assert _matrix.bytes() == 8 * row * col * 2
    assert _matrix.allocated() == base_alloc + 8 * row * col * 2
    assert _matrix.deallocated() == base_dealloc

    m1 = None
    assert _matrix.bytes() == 8 * row * col
    assert _matrix.allocated() == base_alloc + 8 * row * col * 2
    assert _matrix.deallocated() == base_dealloc + 8 * row * col

    m2 = None
    assert _matrix.bytes() == 0
    assert _matrix.allocated() == base_alloc + 8 * row * col * 2
    assert _matrix.deallocated() == base_dealloc + 8 * row * col * 2
