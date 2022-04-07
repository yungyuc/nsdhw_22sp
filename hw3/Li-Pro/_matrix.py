#!/usr/bin/env python3
"""
Wrapper for libmatrix
"""

from collections.abc  import Sequence

from libmatrix  import _Matrix, multiply_naive, multiply_tile, multiply_mkl

class Matrix(_Matrix):
    def __getitem__(self, key: Sequence):
        if isinstance(key, Sequence):
            assert(len(key) == 2)
            return _Matrix[key[0]][key[1]]
        else:
            # return _Matrix[key]
            raise ValueError("key should be an sequence")
    
    def __setitem__(self, key: Sequence, value: float):
        if isinstance(key, Sequence):
            assert(len(key) == 2)
            _Matrix[key[0]][key[1]] = value
        else:
            raise ValueError("key should be an sequence")
