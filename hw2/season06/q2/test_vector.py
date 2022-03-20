import pytest
import math
from _vector import cal_angle

def test_zero_length():
    with pytest.raises(ValueError):
        cal_angle([0, 0], [1, 1])
    with pytest.raises(ValueError):
        cal_angle([1, 1], [0, 0])
    with pytest.raises(ValueError):
        cal_angle([0, 0], [0, 0])


def test_zero_angle():
    assert cal_angle([0, 1], [0, 1]) == pytest.approx(0)
    assert cal_angle([1, 0], [2, 0]) == pytest.approx(0)
    assert cal_angle([1, 1], [2, 2]) == pytest.approx(0)


def test_right_angle():
    assert cal_angle([0, 1], [1, 0]) == pytest.approx(math.pi / 2)
    assert cal_angle([1, 2], [-2, 1]) == pytest.approx(math.pi / 2)
    assert cal_angle([2, -1], [1, 2]) == pytest.approx(math.pi / 2)


def test_angle(): 
    assert cal_angle([3, 0], [-3, 0]) == pytest.approx(math.pi)  # 180
    assert cal_angle([2, 0], [5, 5]) == pytest.approx(math.pi / 4)  # 45
    assert cal_angle([1, 0], [1, math.sqrt(3)]) == pytest.approx(math.pi / 3) # 60
    