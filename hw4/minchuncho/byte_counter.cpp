#include "byte_counter.hpp"

ByteCounter::ByteCounter()
	: bc_ptr_(new ByteCounterStr)
{
	++bc_ptr_->refcount;
}

ByteCounter::ByteCounter(ByteCounter const& other)
	: bc_ptr_(other.bc_ptr_)
{
	++bc_ptr_->refcount;
}

ByteCounter::ByteCounter(ByteCounter && other)
	: bc_ptr_(other.bc_ptr_)
{
	++bc_ptr_->refcount;
}

ByteCounter& ByteCounter::operator=(ByteCounter const& other)
{
	if(&other != this){
		bc_ptr_ = other.bc_ptr_;
		++bc_ptr_->refcount;
	}

	return *this;
}

ByteCounter& ByteCounter::operator=(ByteCounter && other)
{
	if(&other != this){
		bc_ptr_ = other.bc_ptr_;
		++bc_ptr_->refcount;
	}

	return *this;
}

ByteCounter::~ByteCounter()
{
	if(bc_ptr_){
		if(bc_ptr_->refcount == 1){ // this is the last byte counter which references to the struct, and it's about to be destruct.
			delete bc_ptr_;
			bc_ptr_ = nullptr;
		}
		else{
			--bc_ptr_->refcount;
		}
	}
}

bool operator==(ByteCounter const& a, ByteCounter const& b)
{
	return a.bc_ptr_ == b.bc_ptr_;
}

void ByteCounter::inc_bytes(size_t bytes)
{
	bc_ptr_->allocated += bytes;
}

void ByteCounter::dec_bytes(size_t bytes)
{
	bc_ptr_->deallocated += bytes;
}

std::size_t ByteCounter::bytes() const
{
	return bc_ptr_->allocated - bc_ptr_->deallocated;
}

std::size_t ByteCounter::allocated() const
{
	return bc_ptr_->allocated;
}

std::size_t ByteCounter::deallocated() const
{
	return bc_ptr_->deallocated;
}