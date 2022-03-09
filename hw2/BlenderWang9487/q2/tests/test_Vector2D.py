import Angle
import pytest

class TestVector2D:
    def test_init(self):
        v = Angle.Vector2D()
        assert v.getLength() == 0.0

    def test_getLength(self):
        v = Angle.Vector2D(x = 3.0,y = 4.0)
        assert v.getLength() == pytest.approx(5.0,0.01)
    
    def test_dotProduct(self):
        v1 = Angle.Vector2D(x = 3.0,y = 4.0)
        v2 = Angle.Vector2D(x = -9.0,y = 2.0)
        assert v1.dotProduct(v2) == pytest.approx(-19.0,0.01)

