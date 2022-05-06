/**
 * Very simple allocator that counts the number of bytes allocated through it.
 *
 * It's made to demonstrate the STL allocator and only works in this example.
 * A lot of modification is needed to use it in a real application.
 */

#include "byte_counter.hpp"

template <class T>
struct MyAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    MyAllocator() = default;

    T * allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n*sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if (p)
        {
            counter.increase(bytes);
            return p;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);

        const std::size_t bytes = n*sizeof(T);
        counter.decrease(bytes);
    }

    static ByteCounter counter;

}; /* end struct MyAllocator */

template <class T>
ByteCounter MyAllocator<T>::counter{};

template <class T, class U>
bool operator==(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
    return a.counter == b.counter;
}

template <class T, class U>
bool operator!=(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
    return !(a == b);
}