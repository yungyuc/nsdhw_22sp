import _vector as vec
import numpy as np

def func(x):
    return x + 1


def test_answer():
    err_diff = 1e-4
    try:
        invalid_vec = vec.Vector(0,0)
    except ValueError as exc:
        print("Seems like an an error with argument declartion: {}".format(exc))

    print("Zero angle")
    vec1 = vec.Vector(4,0)
    vec2 = vec.Vector(1,0)
    zero_angle = vec.get_angle_rad(vec1,vec2)
    assert np.abs(zero_angle - 0) < err_diff

    print("90-degree angle")
    vec1 = vec.Vector(4,0)
    vec2 = vec.Vector(0,3.4)
    angle_90 = vec.get_angle_rad(vec1,vec2)
    assert np.abs(angle_90 - np.pi/2) < err_diff

    print("Other Angles")
    vec1 = vec.Vector(np.cos(np.pi/4),np.sin(np.pi/4))
    vec2 = vec.Vector(np.cos(np.pi/4),-np.sin(np.pi/4))
    angle3 = vec.get_angle_rad(vec1,vec2)
    assert np.abs(angle3 - np.pi/2) < err_diff

    vec1 = vec.Vector(-1,1)
    vec2 = vec.Vector(1,1)
    angle4 = vec.get_angle_rad(vec1,vec2)
    assert np.abs(angle4 - np.pi/2) < err_diff

    vec1 = vec.Vector(0,1)
    vec2 = vec.Vector(0,-1)
    angle5 = vec.get_angle_rad(vec1,vec2)
    assert np.abs(angle5 - np.pi) < err_diff

    print("All tests passed")
