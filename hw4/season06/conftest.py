import _matrix
import pytest
import numpy as np
import random


@pytest.fixture
def matrix_init():
    x = random.randint(1, 300)
    y = random.randint(1, 300)
    z = random.randint(1, 300)
    tsize = random.randint(1, 16)

    m1 = np.random.rand(x, y)
    m2 = np.random.rand(y, z)
    npdot = np.dot(m1, m2)

    _m1 = _matrix.Matrix(m1.shape[0], m1.shape[1])
    _m2 = _matrix.Matrix(m2.shape[0], m2.shape[1])
    _m1.load_from_array(m1)
    _m2.load_from_array(m2)

    return x, y, z, tsize, npdot, _m1, _m2


@pytest.fixture
def matrix_init_big():
    x, y, z, tsize = 1000, 1000, 1000, 16

    m1 = np.random.rand(x, y)
    m2 = np.random.rand(y, z)
    npdot = np.dot(m1, m2)

    _m1 = _matrix.Matrix(m1.shape[0], m1.shape[1])
    _m2 = _matrix.Matrix(m2.shape[0], m2.shape[1])
    _m1.load_from_array(m1)
    _m2.load_from_array(m2)

    return x, y, z, tsize, npdot, _m1, _m2
