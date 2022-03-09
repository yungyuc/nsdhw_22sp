import pytest
import math
import _vector as v
import random

def test_invalid():
  with pytest.raises(ValueError):
    v.calc_angle([71, 22], [0, 0]) == 0
  with pytest.raises(ValueError):
    v.calc_angle([0, 0], [71, 22]) == 0

def test_zero():
  assert v.calc_angle([0, 7122], [0, 1]) == pytest.approx(0)
  assert v.calc_angle([7122, 0], [7123, 0]) == pytest.approx(0)
  assert v.calc_angle([71, 22], [71, 22]) == pytest.approx(0)

def test_right_angle():
  assert v.calc_angle([1, 1], [2, -2]) == pytest.approx(math.pi / 2)

def test_other_angle():
  assert v.calc_angle((71, 23), [71, 22]) == pytest.approx(0.012799012621094518);


