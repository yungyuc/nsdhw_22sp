import cmath
import _vector
import pytest
import numpy as np

def unit_vector(vector):
    return vector / np.linalg.norm(vector)

def angle_between(v1, v2):
    v1_u = unit_vector(v1)
    v2_u = unit_vector(v2)
    return np.arccos(np.clip(np.dot(v1_u, v2_u), -1.0, 1.0))

def test_zero_length():
    vec1 = _vector.Vector(2, 2)
    vec2 = _vector.Vector(0, 0)
    with pytest.raises(_vector.invalid_argument) as excinfo:
        print(_vector.python_cal_angle(vec1, vec2))
    assert "Invalid input (zero length)." in str(excinfo.value)

def test_zero_angle():
    vec1 = _vector.Vector(1, 0)
    vec2 = _vector.Vector(2, 0)
    assert _vector.python_cal_angle(vec1, vec2) == angle_between((1, 0), (2, 0))

def test_right_angle():
    vec1 = _vector.Vector(1, 0)
    vec2 = _vector.Vector(0, 2)
    assert _vector.python_cal_angle(vec1, vec2) == angle_between((1, 0), (0, 2))

def test_other_angle():
    vec1 = _vector.Vector(1, 0)
    vec2 = _vector.Vector(1, 3)
    assert _vector.python_cal_angle(vec1, vec2) == angle_between((1, 0), (1, 3))


# result = cal.python_cal(3 ** 0.5, 1, 3 ** 0.5, 0)
# print("Python result angle is", result)