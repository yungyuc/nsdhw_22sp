#!/usr/bin/python3

import _vector

def test_forty_five_degree():
	v1 = _vector.Vec(1, 1)
	v2 = _vector.Vec(0, 1)
    assert int(v1.angle(v1, v2)) == 45

def test_ninety_degree():
	v1 = _vector.Vec(1, 0);
	v2 = _vector.Vec(0, 1);
    assert int(v1.angle(v1, v2)) == 90
