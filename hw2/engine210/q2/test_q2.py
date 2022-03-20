import _vector
import numpy as np
import random

def pyAngleofTwoVector(v1, v2):
    unit_vector_1 = v1 / np.linalg.norm(v1)
    unit_vector_2 = v2 / np.linalg.norm(v2)
    dot_product = np.dot(unit_vector_1, unit_vector_2)
    angle = np.arccos(dot_product)
    return angle

def test_zero_len():
    v1 = [0, 0]
    v2 = [0, 0]
    assert np.isnan(pyAngleofTwoVector(v1, v2)) and np.isnan(_vector.angleofTwoVector(v1, v2))

def test_90degree():
    v1 = [0, 1]
    v2 = [1, 0]
    assert pyAngleofTwoVector(v1, v2) == _vector.angleofTwoVector(v1, v2)

def test_random_degree():
    for i in range(100):
        v1 = [random.uniform(-100, 100), random.uniform(-100, 100)]
        v2 = [random.uniform(-100, 100), random.uniform(-100, 100)]
        assert abs(pyAngleofTwoVector(v1, v2) - _vector.angleofTwoVector(v1, v2)) < 10e-6
