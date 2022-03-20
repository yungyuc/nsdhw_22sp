import _vector
import pytest

class TestVector2D:
    def test_init(self):
        v = _vector.Vector2D()
        assert v.getLength() == 0.0

    def test_getLength(self):
        v = _vector.Vector2D(x = 3.0,y = 4.0)
        assert v.getLength() == pytest.approx(5.0,0.01)
    
    def test_dotProduct(self):
        v1 = _vector.Vector2D(x = 3.0,y = 4.0)
        v2 = _vector.Vector2D(x = -9.0,y = 2.0)
        assert v1.dotProduct(v2) == pytest.approx(-19.0,0.01)

