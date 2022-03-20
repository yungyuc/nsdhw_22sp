import sys
sys.path.append(sys.path.__dir__)
import _vector

import pytest


def test_zero_length():
    vec1 = _vector.VectorCord(2, 2)
    vec2 = _vector.VectorCord(0, 0)
    with pytest.raises(_vector.invalid_argument) as excinfo:
        print(_vector.vector_angle(vec1, vec2))
    assert "Invalid input (zero length)." in str(excinfo.value)

def test_zero_angle():
    vec1 = _vector.VectorCord(2, 2)
    vec2 = _vector.VectorCord(2, 2)
    #print(_vector.vector_angle(vec1, vec2))
    assert _vector.vector_angle(vec1, vec2) == 0.0000

def test_right_angle():
    vec1 = _vector.VectorCord(2, 0)
    vec2 = _vector.VectorCord(0, 2)
    #print(_vector.vector_angle(vec1, vec2))
    assert _vector.vector_angle(vec1, vec2) == pytest.approx(1.5707963705)

def test_another_angle():
    vec1 = _vector.VectorCord(2, 5)
    vec2 = _vector.VectorCord(-3, 15)
    #print(_vector.vector_angle(vec1, vec2))
    assert _vector.vector_angle(vec1, vec2) == pytest.approx(0.577901902)