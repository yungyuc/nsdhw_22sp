#include "byte_counter.hpp"

ByteCounter::ByteCounter()
	: allocated_(0), deallocated_(0)
{
	
}

ByteCounter::ByteCounter(ByteCounter const& other)
	: allocated_(other.allocated_), deallocated_(other.deallocated_)
{
}

ByteCounter::ByteCounter(ByteCounter && other)
{
	std::swap(allocated_, other.allocated_);
	std::swap(deallocated_, other.deallocated_);
}

ByteCounter& ByteCounter::operator=(ByteCounter const& other)
{
	if(&other != this){
		allocated_ = other.allocated_;
		deallocated_ = other.deallocated_;
	}

	return *this;
}

ByteCounter& ByteCounter::operator=(ByteCounter && other)
{
	if(&other != this){
		std::swap(allocated_, other.allocated_);
		std::swap(deallocated_, other.deallocated_);
	}

	return *this;
}

ByteCounter::~ByteCounter(){}

bool operator==(ByteCounter const& a, ByteCounter const& b)
{
	return &a == &b;
}

void ByteCounter::inc_bytes(size_t bytes)
{
	allocated_ += bytes;
}

void ByteCounter::dec_bytes(size_t bytes)
{
	deallocated_ += bytes;
}

std::size_t ByteCounter::bytes() const
{
	return allocated_ - deallocated_;
}

std::size_t ByteCounter::allocated() const
{
	return allocated_;
}

std::size_t ByteCounter::deallocated() const
{
	return deallocated_;
}