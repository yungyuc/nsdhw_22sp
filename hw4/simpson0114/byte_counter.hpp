
#include <cstdlib>
#include <new>
#include <memory>
#include <limits>
#include <atomic>
#include <vector>
#include <iostream>

/**
 * One instance of this counter is shared among a set of allocators.
 *
 * The counter keeps track of the bytes allocated and deallocated, and report
 * those two numbers in addition to bytes that remain allocated.
 */
// struct ByteCounterImpl
// {

//     std::size_t allocated = 0;
//     std::size_t deallocated = 0;
//     std::size_t refcount = 0;

// }; /* end struct ByteCounterImpl */

class ByteCounter
{

public:

    ByteCounter();
    ByteCounter(ByteCounter const & other);
    ByteCounter & operator=(ByteCounter const & other);
    ByteCounter(ByteCounter && other);
    ByteCounter & operator=(ByteCounter && other);
    ~ByteCounter();

    friend bool operator==(ByteCounter const& a, ByteCounter const& b);

    // void swap(ByteCounter & other);
    void increase(std::size_t amount);
    void decrease(std::size_t amount);

    std::size_t bytes() const;
    std::size_t allocated() const;
    std::size_t deallocated() const;

private:

    std::size_t allocated_;
    std::size_t deallocated_;

}; /* end class ByteCounter */