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
    assert(_feq(getRad((1, 2), (1, 2)), 0))
    assert(_feq(getRad((2, 3), (4, 6)), 0))
    assert(_feq(getRad((-3, 5), (-9, 15)), 0))

def test_getRad_right_angle():
    assert(_feq(getRad((0, 1), (2, 0)), PI/2))
    assert(_feq(getRad((1.4142135, 1.4142135), (-1.4142135, 1.4142135)), PI/2))

    for i in range(30):
        base = 2 * PI * (i/30)
        avec = (math.cos(base), math.sin(base))
        bvec = (math.cos(base + PI/2), math.sin(base + PI/2))
        assert(_feq(getRad(avec, bvec), PI/2))

def test_getRad_straight_angle():
    assert(_feq(getRad((0, 1), (0, -2)), 0))
    assert(_feq(getRad((3, 0), (-4, 0)), 0))
    assert(_feq(getRad((3, -5), (-3, 5)), 0))

    for i in range(30):
        base = 2 * PI * (i/30)
        avec = (math.cos(base), math.sin(base))
        bvec = (-math.cos(base), -math.sin(base))
        assert(_feq(getRad(avec, bvec), 0))

def test_getRad_common_angle():
    assert(_feq(getRad((1.7320508, 0), (1.7320508, 1)), PI/6))
    assert(_feq(getRad((1, 0), (1, 1.7320508)), PI/3))
    assert(_feq(getRad((4, 0), (4, 3)), 0.6435011))

