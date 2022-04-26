#include <vector>
#include <cstdlib>
#include <new>
#include <memory>
#include <limits>

#include "byte_counter.cpp"

template<class T>
class Allocator {
public:
	typedef T value_type;

    Allocator() = default;

	T* allocate(size_t n)
    {
        if (n > std::numeric_limits<size_t>::max()/sizeof(T))
        {
            throw std::bad_alloc();
        }
        
        const size_t bytes = n*sizeof(T);

        T* p = static_cast<T*>(std::malloc(bytes));
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

    void deallocate(T* p, size_t n) noexcept
    {
        std::free(p);

        const size_t bytes = n * sizeof(T);
        counter.decrease(bytes);
    }

    static ByteCounter counter;
};

template <class T>
ByteCounter Allocator<T>::counter{};

template <class T, class U>
bool operator==(const Allocator<T> & a, const Allocator<U> & b)
{
    return (a == b);
}

template <class T, class U>
bool operator!=(const Allocator<T> & a, const Allocator<U> & b)
{
    return !(a == b);
}