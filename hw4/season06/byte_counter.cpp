#include <cstdlib>
#include <new>
#include <memory>
#include <atomic>
#include <algorithm>

class ByteCounter {
public:
	ByteCounter() : m_allocated(0), m_deallocated(0) {};
	ByteCounter(ByteCounter const& other) : m_allocated(other.m_allocated), m_deallocated(other.m_deallocated) {};
	ByteCounter(ByteCounter && other)
    {
        std::swap(m_allocated, other.m_allocated);
        std::swap(m_deallocated, other.m_deallocated);
    }
	ByteCounter& operator=(ByteCounter const& other)
    {
        if (&other != this)
        {
            m_allocated = other.m_allocated;
            m_deallocated = other.m_deallocated;
        }
        return *this;
    }
	ByteCounter& operator=(ByteCounter && other)
    {
        if (&other != this)
        {
            std::swap(m_allocated, other.m_allocated);
            std::swap(m_deallocated, other.m_deallocated);
        }
        return *this;
    }
	~ByteCounter() {};

	void increase(size_t bytes) { m_allocated += bytes; }
	void decrease(size_t bytes) { m_deallocated += bytes; }

	size_t bytes() const { return m_allocated - m_deallocated; };
	size_t allocated() const { return m_allocated; };
	size_t deallocated() const { return m_deallocated; };

private:
    size_t m_allocated;
    size_t m_deallocated;
};
