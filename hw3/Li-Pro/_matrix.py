#!/usr/bin/env python3
"""
Wrapper for libmatrix
"""

from collections.abc  import Sequence

from libmatrix  import _Matrix, _multiply_naive, _multiply_tile, _multiply_mkl

__all__ = ['_Matrix', 'multiply_naive', 'multiply_tile', 'multiply_mkl']

class Matrix(_Matrix):
    def __getitem__(self, key: Sequence):
        if isinstance(key, Sequence):
            assert(len(key) == 2)
            return self._getitem(*key)
        else:
            raise ValueError("key should be an sequence")
    
    def __setitem__(self, key: Sequence, value: float):
        if isinstance(key, Sequence):
            assert(len(key) == 2)
            return self._setitem(*key, value)
        else:
            raise ValueError("key should be an sequence")
    
    def __repr__(self):
        return 'Matrix[{}]'.format(
            ', '.join(
                '[{}]'.format(
                    ', '.join(
                        map(lambda val: '{:.3f}'.format(val), (self[i, j]   for j in range(self.ncol)))
                    )
                )  for i in range(self.nrow)
            )
        )


def multiply_naive(matA, matB):
    return Matrix(_multiply_naive(matA, matB))

def multiply_tile(matA, matB):
    return Matrix(_multiply_tile(matA, matB))

def multiply_mkl(matA, matB):
    return Matrix(_multiply_mkl(matA, matB))
