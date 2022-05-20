#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>
#include <atomic>
using std::cout, std::endl;
using std::ostream;

struct ByteCounterImpl {
    std::atomic_size_t allocated = 0;
    std::atomic_size_t deallocated = 0;
    std::atomic_size_t refcount = 0;
};

class ByteCounter {
private:
    ByteCounterImpl* impl_;
    void incref(){
        ++(impl_->refcount);
    }
    void decref(){
        if(impl_ == nullptr)
            return;
        else if(1 == impl_->refcount)
        {
            delete impl_;
            impl_ = nullptr;
        }
        else
            --(impl_->refcount);
    }

public:
    // default constructor
    ByteCounter(): impl_(new ByteCounterImpl){
        incref();
    }

    // copy constructor
    ByteCounter(const ByteCounter& other): impl_(other.impl_){
        incref();
    }

    // operator=
    ByteCounter& operator=(const ByteCounter& other){
        if(this != &other){
            // Why??
            decref();
            impl_ = other.impl_;
            incref();            
        }
        return *this;
    }

    ~ByteCounter(){
        decref();
    }

    void swap(ByteCounter &other)
    {
        std::swap(impl_, other.impl_);
    }
    
    void increase(std::size_t amount)
    {
        impl_->allocated += amount;
    }

    void decrease(std::size_t amount)
    {
        impl_->deallocated += amount;
    }

    std::size_t bytes() const {return impl_->allocated - impl_->deallocated;}
    std::size_t allocated() const {return impl_->allocated;}
    std::size_t deallocated() const {return impl_->deallocated;}
    std::size_t refcount() const {return impl_->refcount;}
};

// A simple allocator that tracks the allocated bytes
template <typename T>
struct MyAllocator
{
    static ByteCounter counter;        
    using value_type = T;  
    MyAllocator() = default;

    template <class U> constexpr
    MyAllocator(const MyAllocator<U>& other) noexcept {
        counter = other.counter;
    }

    T* allocate(std::size_t n)
    {
        if(n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n * sizeof(T);

        // allocate in bytes but cast into T
        T * p = static_cast<T*> (std::malloc(bytes));
        if(p)
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

        const size_t bytes = n*sizeof(T);
        counter.decrease(bytes);
    }

    friend ostream &operator<<(ostream &os, const MyAllocator &alloc)
    {
        os << "Allocated: " << alloc.counter.allocated() << " Deallocated: " << alloc.counter.deallocated() << " Bytes: " << alloc.counter.bytes() << endl;
        return os;
    }

    static std::size_t bytes() {return counter.bytes();}
    static std::size_t allocated() {return counter.allocated();}
    static std::size_t deallocated() {return counter.deallocated();}
};

template<typename T>
ByteCounter MyAllocator<T>::counter;



#endif