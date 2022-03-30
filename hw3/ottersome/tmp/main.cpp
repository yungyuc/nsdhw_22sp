#include <iostream>
#include "StopWatch.hpp"

int main(){
    size_t nrow = 1000;
    size_t ncol = 1000;
    size_t * buffer = new size_t[nrow*ncol];
    // Populate by last index.
    //
    StopWatch sw;
    sw.lap();
    //Last
    for (size_t i=0; i<nrow; ++i) // the i-th row
    {
        for (size_t j=0; j<ncol; ++j) // the j-th column
        {
            buffer[i*ncol + j] = i*ncol + j;
        }
    }
    std::cout << "TIme elaped for last major : "<<sw.lap()<<std::endl;

    // Populate by first index.
    // First
    for (size_t j=0; j<ncol; ++j) // the j-th column
    {
        for (size_t i=0; i<nrow; ++i) // the i-th row
        {
            buffer[i*ncol + j] = i*ncol + j;
        }
    }
    std::cout << "TIme elaped for first major : "<<sw.lap()<<std::endl;
    std::cout << "Done"<<std::endl;

    delete []  buffer;

}
