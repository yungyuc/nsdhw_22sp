import _vector as vec
import numpy as np
import pytest as pt


def test_zero_length():
    try:
        invalid_vec = vec.Vector(0,0)
    except ValueError as exc:
        print("Seems like an an error with argument declartion: {}".format(exc))

def test_zero_angle():

    print("Zero angle")
    zero_angle_1 = vec.get_angle_rad(vec.Vector(4,0),vec.Vector(1,0))
    zero_angle_2 = vec.get_angle_rad(vec.Vector(1,0),vec.Vector(4,0))
    zero_angle_3 = vec.get_angle_rad(vec.Vector(2*np.cos(np.pi/4),2*np.sin(np.pi/4)),vec.Vector(11*np.cos(np.pi/4),11*np.sin(np.pi/4)))

    assert zero_angle_1 == pt.approx(0)
    assert zero_angle_2 == pt.approx(0)
    assert zero_angle_3 == pt.approx(0)

def test_90_angle():

    print("90-degree angle")
    vec1 = vec.Vector(4,0)
    vec2 = vec.Vector(0,3.4)
    angle_90 = vec.get_angle_rad(vec1,vec2)
    assert angle_90 == pt.approx(np.pi/2)

def test_other_angles():

    print("Other Angles")
    vec1 = vec.Vector(np.cos(np.pi/4),np.sin(np.pi/4))
    vec2 = vec.Vector(np.cos(np.pi/4),-np.sin(np.pi/4))
    angle3 = vec.get_angle_rad(vec1,vec2)
    assert angle3 == pt.approx(np.pi/2)

    vec1 = vec.Vector(-1,1)
    vec2 = vec.Vector(1,1)
    angle4 = vec.get_angle_rad(vec1,vec2)
    assert angle4 == pt.approx(np.pi/2)

    vec1 = vec.Vector(0,1)
    vec2 = vec.Vector(0,-1)
    angle5 = vec.get_angle_rad(vec1,vec2)
    assert angle5 == pt.approx(np.pi)

    vec1 = vec.Vector(10*np.cos(0),10*np.sin(0))
    vec2 = vec.Vector(23*np.cos(np.pi/6),23*np.sin(np.pi/6))
    angle6 = vec.get_angle_rad(vec1,vec2)
    assert angle6 == pt.approx(np.pi/6)

    vec1 = vec.Vector(11*np.cos(np.pi - np.pi/6),11*np.sin(np.pi - np.pi/6))
    vec2 = vec.Vector(23*np.cos(np.pi),23*np.sin(np.pi))
    angle7 = vec.get_angle_rad(vec1,vec2)
    assert angle7 == pt.approx(np.pi/6)

    vec1 = vec.Vector(1,-2)
    vec2 = vec.Vector(-2,1)
    angle8 = vec.get_angle_rad(vec1,vec2)
    assert angle8 == pt.approx(143.13/180*np.pi)

    print("All tests passed")
