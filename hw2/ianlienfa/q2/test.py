import pytest 
from _vector import Vector

class TestClass:
    def test__zero_length_vec(self):
        assert (Vector.getAngle(Vector(0,0), Vector(0,0))) == 0

    def test__zero_angle(self):
        assert (Vector.getAngle(Vector(1,1), Vector(1,1))) == 0
    