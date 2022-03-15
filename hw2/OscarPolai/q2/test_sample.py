import _vector as v
import cmath
import pytest
import numpy as np

def test_zero_len():
	with pytest.raises(ValueError):
		v.get_angle([0, 0], [2, 0])
	
def test_zero_angle():
	print(v.get_angle([0, 2], [0, 1]))
	assert v.get_angle([0, 2], [0, 1]) == pytest.approx(0)

def test_right_angle():
	assert v.get_angle([0,1], [1,0]) == pytest.approx(np.pi/2)

def test_45_angle():
	assert v.get_angle([1,0], [2,2]) == pytest.approx(np.pi/4)