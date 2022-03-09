import Angle
import math
import pytest

def test_calcAngle():
    v1 = Angle.Vector2D(x = 1.0,y = 0.0)
    v2 = Angle.Vector2D(x = -1.0,y = 0.0)
    assert Angle.calcAngle(v1,v2) == pytest.approx(math.pi,0.01)

    v1 = Angle.Vector2D(x = 2.0,y = 0.0)
    v2 = Angle.Vector2D(x = 0.0,y = 2.0)
    assert Angle.calcAngle(v1,v2) == pytest.approx(math.pi/2,0.01)