#include <cstdlib>
#include <new>
#include <memory>
#include <atomic>
#include <algorithm>

struct ByteCounterStr {
	std::size_t allocated = 0;
    std::size_t deallocated = 0;
    std::size_t refcount = 0;		// the number of byte counters which reference to this struct
} ;

class ByteCounter {
public:
	ByteCounter();
	ByteCounter(ByteCounter const& other);
	ByteCounter(ByteCounter && other);
	ByteCounter& operator=(ByteCounter const& other);
	ByteCounter& operator=(ByteCounter && other);
	~ByteCounter();

	void inc_bytes(size_t bytes);
	void dec_bytes(size_t bytes);
	// getter
	std::size_t bytes() const;
	std::size_t allocated() const;
	std::size_t deallocated() const;

private:
	// std::size_t allocated_;
 //    std::size_t deallocated_;
	ByteCounterStr* bc_ptr_;
};