import _vector
import cmath
import pytest
import numpy as np

class TestInstance:

	def test_zero_length(self):
		with pytest.raises(Exception) as e_info:
			_vector.py_get_angle(0, 0, 1, 2)

	def test_zero_angle(self):
		assert _vector.py_get_angle(2, 5, 2, 5) == 0

	def test_right_angle(self):

		answer = np.arccos(np.dot((3,4), (-4,3)) / 25)
		assert _vector.py_get_angle(3, 4, -4, 3) == answer

	def test_another_angle(self):
		
		v1_len = np.sqrt(5)
		v2_len = np.sqrt(25 + 49)
		answer = np.arccos(np.dot((1,2), (-5, 7)) / (v1_len * v2_len))
		assert _vector.py_get_angle(1, 2, -5, 7) == answer
