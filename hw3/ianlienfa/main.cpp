#include <iostream>
#include <vector>
#include <algorithm>
#include <mkl.h>
#include <iomanip>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Tiler.h"
#include "Matrix.h"
#include "Util.h"
using std::cout, std::endl;
using std::setw;
using std::ostream;
using std::string, std::vector;
using std::copy;
namespace py = pybind11;


int main(int argc, char* argv[])
{
    srand(0);
    vector<double> v1(500 * 500), v2(500 * 500);    
    std::generate(v1.begin(), v1.end(), [](){return std::rand() % 10;});
    std::generate(v2.begin(), v2.end(), [](){return std::rand() % 10;});
    Matrix mtrx(500, 500, v1);
    Matrix mtrx2(500, 500, v2);
    
    // Time tracker vars
    long long vanilla_time = 0;
    long long tile_time = 0;
    long long lapack_time = 0;
    long long hard_tile = 0;

    /* Vanilla mul */
    TIME_ELAPSE_START();
    Matrix res(500, 500);
    res = Matrix::multiply_naive(mtrx, mtrx2);    
    res.brief("Vanilla");
    TIME_ELAPSE_END(vanilla_time);

    /* Lapack mul*/
    TIME_ELAPSE_START();
    Matrix res_mkl(500, 500);
    res_mkl = Matrix::multiply_mkl(mtrx, mtrx2);    
    res_mkl.brief("MKL");
    TIME_ELAPSE_END(lapack_time);


    /* Tiler mul*/
    TIME_ELAPSE_START();
    Matrix t_res(500, 500);
    t_res = Matrix::multiply_tile(mtrx, mtrx2, 16);    
    t_res.brief("Tiled");
    TIME_ELAPSE_END(tile_time);

    // TIME_ELAPSE_START();
    // Matrix hard_res(1024, 1024);
    // hard_res = Matrix::multiply_tile(mtrx, mtrx2, 2);    
    // hard_res.brief("Hard Tile");
    // TIME_ELAPSE_END(hard_tile);



    // cout << "Tile construct time: " << Tracker::tile_construct_time << endl;
    // cout << "Tile mul time: " << Tracker::tile_mul_time << endl;
    // cout << "Tile fill time: " << Tracker::tile_fill_time << endl;
    // cout << "Tile res time: " << Tracker::tile_res_time << endl;

    cout << setw(30) << "Vanilla time: "  << vanilla_time << endl;
    cout << setw(30) << "Lapack time: " << lapack_time << endl;
    cout << setw(30) << "Tile time: " << tile_time << endl;
    // cout << setw(30) << "Hard Tile time: " << hard_tile << endl;
}