import math

import pytest

from _vector import getRad

PI = math.pi
EPS = 1e-6

def _feq(a, b):
    return abs(a - b) < EPS

def test_getRad_invalid_vector_raise_ValueError():
    with pytest.raises(ValueError):
        getRad((), (1, 2))
    
    with pytest.raises(ValueError):
        getRad((1, 2), ())
    
    with pytest.raises(ValueError):
        getRad((), ())
    
    with pytest.raises(ValueError):
        getRad((1, 2, 3), (1, 2, 3, 4))

def test_getRad_zero_length_raise_ValueError():
    with pytest.raises(ValueError):
        getRad((0, 0), (1, 2))
    
    with pytest.raises(ValueError):
        getRad((1, 2), (0, 0))
    
    with pytest.raises(ValueError):
        getRad((0, 0), (0, 0))

def test_getRad_zero_angle():
    assert(_feq(getRad((1, 2,), (1, 2)), 0))
    assert(_feq(getRad((2, 3,), (4, 6)), 0))
    assert(_feq(getRad((3, -5,), (-3, 5)), 0))

def test_getRad_right_angle():
    assert(_feq(getRad((0, 1), (2, 0)), PI/2))
    # assert(_feq(getRad((2, 3,), (4, 6)), PI/2))
