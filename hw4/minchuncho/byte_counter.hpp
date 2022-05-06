#include <cstdlib>
#include <new>
#include <memory>
#include <atomic>
#include <algorithm>

class ByteCounter {
public:
	ByteCounter();
	ByteCounter(ByteCounter const& other);
	ByteCounter(ByteCounter && other);
	ByteCounter& operator=(ByteCounter const& other);
	ByteCounter& operator=(ByteCounter && other);
	~ByteCounter();

	friend bool operator==(ByteCounter const& a, ByteCounter const& b);

	void inc_bytes(size_t bytes);
	void dec_bytes(size_t bytes);
	// getter
	std::size_t bytes() const;
	std::size_t allocated() const;
	std::size_t deallocated() const;

private:
	std::size_t allocated_;
    std::size_t deallocated_;
};