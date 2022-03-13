import _vector
import cmath
import pytest

class TestInstance:

	def test_zero_length(self):
		with pytest.raises(Exception) as e_info:
			_vector.py_get_angle(0, 0, 1, 2)

	def test_zero_angle(self):
		assert _vector.py_get_angle(2, 5, 2, 5) == 0

	def test_right_angle(self):

		# the correct answer should be pi
		# but we cant get exact solution, just test if it's close enough

		assert abs(_vector.py_get_angle(3, 4, -4, 3)-cmath.pi/2) < 1e-5

	#def test_another_angle():
	#	pass
