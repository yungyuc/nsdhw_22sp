#pragma once
#include <stdio.h>
#include <limits>
#include <new>
#include <cstdlib>


//Static Class to count bytes across matrices
//Who cares about thread safety here
struct ByteCounter{
    private:
        ByteCounter()
            :bytes_allocated(0),bytes_deallocated(0)
        { }//Make our constructor private

        static ByteCounter * instance;
        std::size_t bytes_allocated;
        std::size_t bytes_deallocated;

    public:
        ByteCounter(ByteCounter const &) = delete;
        void operator=(ByteCounter const &) = delete;

        static ByteCounter * getInstance();
        void increase_by_bytes(size_t bytes){bytes_allocated+=bytes;}
        void decrease_by_bytes(size_t bytes){bytes_deallocated+=bytes;}
        size_t cur_alloc_bytes(){return bytes_allocated-bytes_deallocated;}
        size_t tot_alloc_bytes(){return bytes_allocated;}
        size_t tot_dealloc_bytes(){return bytes_deallocated;}

};

template<class T>
class CustomAllocator{

    private:
        ByteCounter * byte_counter;

    public:
    typedef T value_type;
    CustomAllocator() 
        : byte_counter(ByteCounter::getInstance())
    { }
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
            byte_counter->increase_by_bytes(n_elem*sizeof(T));
        }
        return loc;
    }

    void deallocate(T * loc, std::size_t n_elem){
        std::free(loc);
        //deallocated_bytes-=n_elem*sizeof(T);
        byte_counter->decrease_by_bytes(n_elem*sizeof(T));
    }
    size_t bytes() {return byte_counter->cur_alloc_bytes();}
};
