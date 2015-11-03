/*
* ****************************************
*
* \Brief This file is part of motr (Model transformation test framework),
*	     a library for solving model transformation problems in the domain of feature diagrams.
*
* Copyright (c) 2015 motr. All rights reserved.
*
* For the full copyright and license information, please view the LICENSE.txt
* file that was distributed with this source code.
*
* \Author Alexander Knueppel
*
* ****************************************
*/

#include <sstream>
#include <core/stack_memory_pool.h>

#include <exception>

MOTR_NS_BEGIN

struct MemoryBlockHeader {
	std::uint32_t size;
};

//==============================================================================
StackMemoryPool::StackMemoryPool(size_t size, std::uint32_t alignmentBits)
: memsize_(size), alignment_bits_(alignmentBits)
{
	assert(memsize_ > 0);
	memory_ = (std::uint8_t*)::malloc(memsize_);

	if (memory_ != nullptr) {
		top_ = memory_;
	} else {
		throw std::exception("malloc() failed");
	}
}

//==============================================================================
StackMemoryPool::~StackMemoryPool()
{
	if (memory_ != nullptr) {
		::free(memory_);
	}
}

//==============================================================================
StackMemoryPool& StackMemoryPool::operator=(StackMemoryPool&& other) {
	if (memory_ != nullptr) {
		::free(memory_);
	}

	memory_ = other.memory_;
	memsize_ = other.memsize_;
	top_.store(other.top_.load());
	alignment_bits_ = other.alignment_bits_;

	other.memory_ = nullptr;
	other.memsize_ = 0;
	other.top_ = nullptr;

	return *this;
}

//==============================================================================
size_t StackMemoryPool::getAllocatedSize() const
{
	return top_.load() - memory_;
}

//==============================================================================
void* StackMemoryPool::allocate(size_t size) throw()
{
	// memory is nullptr if moved
	assert(memory_ != nullptr);

	size_t size_tmp = calcAlignSize(size + sizeof(MemoryBlockHeader));

	assert(size_tmp < std::numeric_limits<std::uint32_t>::max() && "Too big allocation");

	std::uint8_t* out = top_.fetch_add(size_tmp);

	if (out + size <= memory_ + memsize_) {
		// Write the block header
		((MemoryBlockHeader*)out)->size = size_tmp;

		// Set the correct output
		out += sizeof(MemoryBlockHeader);
	} else {
		// Error
		out = nullptr;
	}

	return out;
}

//==============================================================================
bool StackMemoryPool::free(void* ptr) throw() {
	// memory is nullptr if moved
	assert(memory_ != nullptr);

	// Correct the p
	std::uint8_t* realptr = (std::uint8_t*)ptr - sizeof(MemoryBlockHeader);

	// realptr should be inside the pool's preallocated memory
	assert(realptr >= memory_ && realptr < memory_ + memsize_);

	// Get block size
	std::uint32_t size = ((MemoryBlockHeader*)realptr)->size;

	// Atomic stuff
	std::uint8_t* expected = realptr + size;
	std::uint8_t* desired = realptr;

	// if(top == expected) {
	//     top = desired;
	//     exchange = true;
	// } else {
	//     expected = top;
	//     exchange = false;
	// }
	bool exchange = top_.compare_exchange_strong(expected, desired);

	return exchange;
}

//==============================================================================
void StackMemoryPool::reset() {
	// memory is nullptr if moved
	assert(memory_ != nullptr);

	top_ = memory_;
}

//==============================================================================
size_t StackMemoryPool::calcAlignSize(size_t size) const {
	return size + (size % (alignment_bits_ / 8));
}

MOTR_NS_END