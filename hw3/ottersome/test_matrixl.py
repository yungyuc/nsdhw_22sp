import numpy as np
import pytest as pt
import sys
sys.path.append('./modules')
import _matrix as mtx

def print_matrix(m):
    for row in range(m.nrow()):
        print(" | ",end="")
        for col in range(m.ncol()):
            print(m[row,col],end=",")
        print(" | ",end="\n")

def test_indexing():
    m1 = mtx.ident(3,3)
    print("m[0,0]={}".format(m1[0,0]))
    print("m[1,1,]={}".format(m1[1,1]))
    print("m[1,0]={}".format(m1[1,0]))
    print("m[2,2]={}".format(m1[2,2]))

def test_t_mult_by_ident():
    print("Pun pun Optimized ")
    m1 = mtx.ident(4,4)
    m2 = mtx.Matrix(4,1)
    
    m1[0,3] = 1

    m2[0,0]=1
    m2[0,1]=2
    m2[0,2]=3
    m2[0,3]=4
    # m2[0,4]=5
    # m2[0,5]=6
    # m2[0,6]=7
    # m2[0,7]=8
    # m2[0,8]=9
    # m2[0,9]=10
    # m2[0,10]=11
    # m2[0,11]=12
    # m2[0,12]=13
    # m2[0,13]=14
    # m2[0,14]=15
    # m2[0,15]=16
    m3 = m1.t_mult(m2)
    print_matrix(m3)
def test_mult_by_ident():
    print("Pun pun")
    m1 = mtx.ident(3,3)
    m2 = mtx.Matrix(3,1)
    m2[0,0]=1
    m2[0,1]=2
    m2[0,2]=3
    m3 = m1.n_mult(m2)
    print_matrix(m3)

def test_times_identity():
    print("Testing identity")
    m1 = mtx.ident(3,3)
    # m2 = mtx(3,3)
    # print_matrix(m1)
    print("M1 has been initialized")
    pass



