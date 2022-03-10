import _vector
import math
import pytest

# Q2 testcase 1:
# zero-length 2-vector (invalid input)
def test_zero_length():
    v1 = _vector.Vector2D() # default with x = y = 0.0
    v2 = _vector.Vector2D(0.1,1.0)
    with pytest.raises(Exception) as e:
        _vector.calcAngle(v1,v2)

# testcase 2:
# zero angle
def test_zero_angle():
    v1 = _vector.Vector2D(x = 2.0,y = 0.0)
    v2 = _vector.Vector2D(x = 2.0,y = 0.0)
    assert _vector.calcAngle(v1,v2) == 0.0

# testcase 3:
# right angle (90-deg)
def test_right_angle():
    v1 = _vector.Vector2D(x = 2.0,y = 0.0)
    v2 = _vector.Vector2D(x = 0.0,y = 2.0)
    assert _vector.calcAngle(v1,v2) == pytest.approx(math.pi/2,0.01)

# testcase 4:
# one other angle
def test_other_angle():
    v1 = _vector.Vector2D(x = 2.0,y = 0.0)
    v2 = _vector.Vector2D(x = -2.0,y = 0.0)
    assert _vector.calcAngle(v1,v2) == pytest.approx(math.pi,0.01)
