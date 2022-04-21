#pragma once
#include <stdio.h>
#include <limits>
#include <new>
#include <cstdlib>


//Static Class to count bytes across matrices
struct ByteCounter{
    public:
        static void increase_by_bytes(size_t bytes){bytes_allocated+=bytes;}
        static void decrease_by_bytes(size_t bytes){bytes_deallocated+=bytes;}
        static size_t cur_alloc_bytes(){return bytes_allocated-bytes_deallocated;}
        static size_t tot_alloc_bytes(){return bytes_allocated;}
        static size_t tot_dealloc_bytes(){return bytes_deallocated;}

        static std::size_t bytes_allocated;
        static std::size_t bytes_deallocated;
};

template<class T>
class CustomAllocator{

    //private:
        //size_t allocated_bytes=0;
        //size_t deallocated_bytes=0;

    public:
    typedef T value_type;
    CustomAllocator() = default;
    //Lets see if it breaks :]
    CustomAllocator(CustomAllocator && ) = delete;
    CustomAllocator(const CustomAllocator &) = delete;

    T * allocate(size_t n_elem){
        if(n_elem > std::numeric_limits<std::size_t>::max() / sizeof(T)){
            throw std::bad_alloc();
        }
        const std::size_t n_bytes = n_elem*sizeof(T);
        T * loc = static_cast<T*>(std::malloc(n_bytes));
        if(!loc){
            throw std::bad_alloc();
        }else{
            //allocated_bytes+=n_elem*sizeof(T);
            ByteCounter::increase_by_bytes(n_elem*sizeof(T));
        }
        return loc;
    }

    void deallocate(T * loc, std::size_t n_elem){
        std::free(loc);
        //deallocated_bytes-=n_elem*sizeof(T);
        ByteCounter::decrease_by_bytes(n_elem*sizeof(T));
    }
    size_t bytes() {return ByteCounter::cur_alloc_bytes();}
};
