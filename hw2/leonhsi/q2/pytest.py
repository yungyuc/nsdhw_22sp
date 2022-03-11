import math
import pytest
import angles

def test_passes():
    with pytest.raises(Exception) as e_info:
        angles.caculate_angle([0,0],[1,1])

def test_zero_angle():
    assert angles.caculate_angle([1,1],[1,1]) == 0

def test_right_angle():
    assert angles.caculate_angle([1,0],[0,1]) == pytest.approx(math.pi / 2)
    assert angles.caculate_angle([3,3],[-5,5]) == pytest.approx(math.pi / 2)
    assert angles.caculate_angle([2,4],[-4,2]) == pytest.approx(math.pi / 2)

def test_normal_angle():
    assert angles.caculate_angle([1,2],[3,4]) == pytest.approx(0.17985349893569946)
    assert angles.caculate_angle([1,0],[math.sqrt(3),1]) == pytest.approx(math.pi / 6)
    assert angles.caculate_angle([2,2],[1,0]) == pytest.approx(math.pi / 4)
    assert angles.caculate_angle([1,0],[-1,0]) == pytest.approx(math.pi)
