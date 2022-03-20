import _vector
import pytest
import math

def test_zero_length():
    with pytest.raises(ValueError):
        _vector.vectors_angle([0, 0], [0, 1])

def test_zero_angle():
    assert _vector.vectors_angle([1, 1], [1, 1]) == pytest.approx(0)
    
def test_right_angle():
    assert _vector.vectors_angle([1, 0], [0, 1]) == pytest.approx(math.pi / 2)

def test_other_angle():
    assert _vector.vectors_angle([1, 1], [0, 1]) == pytest.approx(math.pi / 4)
    assert _vector.vectors_angle([-1, 1], [1, 0]) == pytest.approx(math.pi / 4 * 3)


