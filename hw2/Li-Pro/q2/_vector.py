"""
Wrapper for libvector.
"""

from libvector import _get_rad

def getRad(vec1, vec2):
    """
    getRad(vec1, vec2)
        calculate radians between two given vector vec1=(ax, ay) and vec2=(bx, by)
    """

    if len(vec1) != 2:
        raise ValueError("invalid 1st vector of length {}".format(len(vec1)))
    
    if len(vec2) != 2:
        raise ValueError("invalid 2nd vector of length {}".format(len(vec2)))

    # asserts that all values are float
    return _get_rad(vec1[0], vec1[1], vec2[0], vec2[1])
