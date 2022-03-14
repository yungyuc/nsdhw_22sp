import _vector
import pytest

PI = 3.14159265359

def test_zero_length():
	l1 = _vector.Line(2)
	l2 = _vector.Line(2)

	l1.x(0,1);l1.y(0,0);l1.x(1,1);l1.y(1,0);
	l2.x(0,0);l2.y(0,1);l2.x(1,0);l2.y(1,2);

	with pytest.raises(ValueError):
		rad = _vector.calc_radius(l1,l2)

def test_deg0():
	l1 = _vector.Line(2)
	l2 = _vector.Line(2)

	l1.x(0,0);l1.y(0,0);l1.x(1,1);l1.y(1,1);
	l2.x(0,1);l2.y(0,1);l2.x(1,2);l2.y(1,2);

	rad = _vector.calc_radius(l1,l2)
	assert abs(rad-(0.0/180.0)*PI) <= 0.0001

def test_deg45():
	l1 = _vector.Line(2)
	l2 = _vector.Line(2)

	l1.x(0,1);l1.y(0,1);l1.x(1,2);l1.y(1,2);
	l2.x(0,0);l2.y(0,1);l2.x(1,0);l2.y(1,2);

	rad = _vector.calc_radius(l1,l2)
	assert abs(rad-(45.0/180.0)*PI) <= 0.0001

def test_deg90():
	l1 = _vector.Line(2)
	l2 = _vector.Line(2)

	l1.x(0,1);l1.y(0,0);l1.x(1,2);l1.y(1,0);
	l2.x(0,0);l2.y(0,1);l2.x(1,0);l2.y(1,2);

	rad = _vector.calc_radius(l1,l2)
	assert abs(rad-(90.0/180.0)*PI) <= 0.0001