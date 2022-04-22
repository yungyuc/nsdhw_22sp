#include <cstdlib>
#include <new>
#include <memory>
#include <atomic>

struct ByteCounterStr {
	std::atomic_size_t allocated;
    std::atomic_size_t deallocated;
    std::atomic_size_t refcount;		// the number of byte counters which reference to this struct
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
	ByteCounterStr* bc_ptr_; 
};