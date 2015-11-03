#ifndef __MOTR_CORE_STACKMEMORYPOOL_H__
#define __MOTR_CORE_STACKMEMORYPOOL_H__  

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

#include <string>
#include "../configuration.h"

#include <atomic>

MOTR_NS_BEGIN

/**
* \brief 
*
* \ingroup core
*/
class MOTR_API StackMemoryPool{
public:
	/// Default constructor
	StackMemoryPool(size_t size, std::uint32_t alignmentBits = 16);

	/// Move
	StackMemoryPool(StackMemoryPool&& other) {
		*this = std::move(other);
	}

	/// Destroy
	~StackMemoryPool();

	/// Move
	StackMemoryPool& operator=(StackMemoryPool&& other);

	/// Access the total size
	size_t getSize() const {
		return memsize_;
	}

	/// Get the allocated size
	size_t getAllocatedSize() const;

	/// Allocate memory
	/// @return The allocated memory or nullptr on failure
	void* allocate(size_t size) throw();

	/// Free memory in StackMemoryPool. If the ptr is not the last allocation
	/// then nothing happens and the method returns false
	///
	/// @param[in] ptr Memory block to deallocate
	/// @return True if the deallocation actually happened and false otherwise
	bool free(void* ptr) throw();

	/// Reinit the pool. All existing allocated memory will be lost
	void reset();

private:
	/// Pre-allocated memory chunk
	std::uint8_t* memory_ = nullptr;

	/// Size of the pre-allocated memory chunk
	size_t  memsize_ = 0;

	/// Points to the memory and more specifically to the top of the stack
	std::atomic<std::uint8_t*> top_ = { nullptr };

	/// Alignment of allocations
	std::uint32_t alignment_bits_;

	/// Calculate tha aligned size of an allocation
	size_t calcAlignSize(size_t size) const;
};

MOTR_NS_END

#endif