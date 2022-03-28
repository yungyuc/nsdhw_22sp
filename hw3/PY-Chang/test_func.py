import sys
sys.path.append(sys.path.__dir__)
import _matrix
import pytest
import time

def test_run():
    matrixSize = 512
    tileSize = 32

    mat1 = _matrix.Matrix(matrixSize, matrixSize)
    mat2 = _matrix.Matrix(matrixSize, matrixSize)
    _matrix.initialize(mat1)
    _matrix.initialize(mat2)

    mklAns = _matrix.Matrix(matrixSize, matrixSize) 
    mklAns = _matrix.multiply_mkl(mat1, mat2)

    naiveAns = _matrix.Matrix(matrixSize, matrixSize) 
    naiveAns = _matrix.multiply_naive(mat1, mat2)
    
    tileAns = _matrix.Matrix(matrixSize, matrixSize) 
    tileAns = _matrix.multiply_tile(mat1, mat2, tileSize)

    assert mklAns == naiveAns
    assert mklAns == tileAns

def benchmark():

    matrixSize = 1024
    tileSize = 16

    mat1 = _matrix.Matrix(matrixSize, matrixSize)
    mat2 = _matrix.Matrix(matrixSize, matrixSize)
    _matrix.initialize(mat1)
    _matrix.initialize(mat2)
    runTimes = 1
    timeNaive = 0.0
    naiveAns = _matrix.Matrix(matrixSize, matrixSize) 
    for i in range(runTimes):
        time_start = time.time()
        naiveAns = _matrix.multiply_naive(mat1, mat2)
        time_end = time.time()
        timeNaive += (time_end - time_start)

    timeTile = 0.0
    tileAns = _matrix.Matrix(matrixSize, matrixSize) 
    for i in range(runTimes):
        time_start = time.time()
        naiveAns = _matrix.multiply_tile(mat1, mat2, tileSize)
        time_end= time.time()
        timeTile += (time_end - time_start)

    print(timeNaive/runTimes)
    print(timeTile/runTimes)

    file = open('performance.txt', 'w')
    file.write("Matrix Size = 1024x1024" + '\n')
    file.write("Tile Size = 16" + '\n')
    file.write("Each approach runs three times and calculates average run time..." + '\n')
    file.write('naive approach:' + str((timeNaive/runTimes)) + "s" + '\n')
    file.write('tile approach:' + str(timeTile/runTimes) + "s"+ '\n')
    file.write("Tile approach is " + str((float)(timeNaive/runTimes)/(timeTile/runTimes)) + " times faster then naive approach." + '\n')
    file.close()
    
if __name__ == '__main__':
    benchmark()