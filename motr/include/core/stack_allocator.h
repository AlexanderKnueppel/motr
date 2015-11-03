#ifndef __MOTR_CORE_STACKALLOCATOR_H__
#define __MOTR_CORE_STACKALLOCATOR_H__  

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

#include <core/stack_memory_pool.h>
#include <memory>

MOTR_NS_BEGIN

/**
* \brief Allocator for better memory management
*
* \ingroup core
*/
template<typename T, bool deallocationFlag = false, std::uint32_t alignmentBits = 16>
class StackAllocator {
	template<typename U, bool deallocationFlag_, std::uint32_t alignmentBits_>
	friend class StackAllocator;
public:
	// Typedefs
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using value_type = T;

	/// Default constructor
	StackAllocator() throw() {}
	/// Copy constructor
	StackAllocator(const StackAllocator& b) throw() {
		*this = b;
	}
	/// Copy constructor
	template<typename U>
	StackAllocator(
		const StackAllocator<U, deallocationFlag, alignmentBits>& b) throw() {
		*this = b;
	}
	/// Constuctor with size
	StackAllocator(size_type size) throw() {
		memory_pool_.reset(new StackMemoryPool(size, alignmentBits));
	}

	/// Destructor
	~StackAllocator() {}

	/// Copy
	StackAllocator& operator=(const StackAllocator& b) {
		memory_pool_ = b.memory_pool_;
		return *this;
	}
	/// Copy
	template<typename U>
	StackAllocator& operator=(
		const StackAllocator<U, deallocationFlag, alignmentBits>& b) {
		memory_pool_ = b.memory_pool_;
		return *this;
	}

	/// Get the address of a reference
	pointer address(reference x) const {
		return &x;
	}
	/// Get the const address of a const reference
	const_pointer address(const_reference x) const {
		return &x;
	}

	/// Allocate memory
	pointer allocate(size_type n, const void* hint = 0) {
		assert(memory_pool_ != nullptr);
		(void)hint;
		size_type size = n * sizeof(value_type);

		void* out = memory_pool_->allocate(size);

		if (out != nullptr) {
			// Everything ok
		} else {
			throw std::exception("Allocation failed. There is not enough room");
		}

		return (pointer)out;
	}

	/// Deallocate memory
	void deallocate(void* p, size_type n) {
		assert(memory_pool_ != nullptr);
		(void)p;
		(void)n;

		if (deallocationFlag) {
			bool ok = memory_pool_->free(p);

			if (!ok) {
				throw std::exception("Freeing wrong pointer. "
					"The deallocations on StackAllocator should be in order");
			}
		}
	}

	/// Call constructor
	void construct(pointer p, const T& val) {
		// Placement new
		new ((T*)p) T(val);
	}
	/// Call constructor with many arguments
	template<typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		// Placement new
		::new((void *)p) U(std::forward<Args>(args)...);
	}

	/// Call destructor
	void destroy(pointer p) {
		p->~T();
	}
	/// Call destructor
	template<typename U>
	void destroy(U* p) {
		p->~U();
	}

	/// Get the max allocation size
	size_type max_size() const {
		assert(memory_pool_ != nullptr);
		return memory_pool_->getSize();
	}

	/// A struct to rebind the allocator to another allocator of type U
	template<typename U>
	struct rebind {
		using other =  StackAllocator<U, deallocationFlag, alignmentBits>;
	};

	/// Reinit the allocator. All existing allocated memory will be lost
	void reset() {
		assert(memory_pool_ != nullptr);
		memory_pool_->reset();
	}

	const StackMemoryPool& getMemoryPool() const {
		assert(memory_pool_ != nullptr);
		return *memory_pool_;
	}

private:
	std::shared_ptr<StackMemoryPool> memory_pool_;
};

/// Another allocator of the same type can deallocate from this one
template<typename T1, typename T2, bool deallocationFlag, std::uint32_t alignmentBits>
inline bool operator==(
	const StackAllocator<T1, deallocationFlag, alignmentBits>&,
	const StackAllocator<T2, deallocationFlag, alignmentBits>&) {
	return true;
}

/// Another allocator of the another type cannot deallocate from this one
template<typename T1, typename AnotherAllocator, bool deallocationFlag,
	std::uint32_t alignmentBits>
	inline bool operator==(
	const StackAllocator<T1, deallocationFlag, alignmentBits>&,
	const AnotherAllocator&) {
	return false;
}

/// Another allocator of the same type can deallocate from this one
template<typename T1, typename T2, bool deallocationFlag, std::uint32_t alignmentBits>
inline bool operator!=(
	const StackAllocator<T1, deallocationFlag, alignmentBits>&,
	const StackAllocator<T2, deallocationFlag, alignmentBits>&) {
	return false;
}

/// Another allocator of the another type cannot deallocate from this one
template<typename T1, typename AnotherAllocator, bool deallocationFlag,
	std::uint32_t alignmentBits>
	inline bool operator!=(
	const StackAllocator<T1, deallocationFlag, alignmentBits>&,
	const AnotherAllocator&) {
	return true;
}

MOTR_NS_END

#endif