#ifndef __MOTR_CORE_ALLOCATOR_H__
#define __MOTR_CORE_ALLOCATOR_H__  

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

MOTR_NS_BEGIN

/**
* \brief Allocator for better memory management
*
* \ingroup core
*/
template<typename T>
class Allocator {
public:
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using value_type = T;

	/// Default constructor
	Allocator() throw() {}

	/// Copy constructor
	Allocator(const Allocator& other) throw() {}

	/// Copy constructor with another type
	template<typename U>
	Allocator(const Allocator<U>&) throw() {}

	/// Copy
	Allocator<T>& operator=(const Allocator& other) {
		return *this;
	}

	/// Copy with another type
	template<typename U>
	Allocator& operator=(const Allocator<U>& other) {
		return *this;
	}

	/// Get address of a reference
	pointer address(reference x) const {
		return &x;
	}

	/// Get const address of a const reference
	const_pointer address(const_reference x) const {
		return &x;
	}

	/// Allocate n elements of type T
	pointer allocate(size_type n, const void* hint = 0) {
		size_type size = n * sizeof(value_type);
		gAllocatedMemory += size;
		return (pointer)::malloc(size);
	}

	/// Free memory of pointer p
	void deallocate(void* p, size_type n) {
		size_type size = n * sizeof(T);
		gAllocatedMemory -= size;
		::free(p);
	}

	/// Call the constructor of p
	void construct(pointer p, const T& val) {
		new ((T*)p) T(val);
	}

	/// Call the constructor of p with many arguments. C++11
	template<typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		::new((void*)p) U(std::forward<Args>(args)...);
	}

	/// Call the destructor of p
	void destroy(pointer p) {
		p->~T();
	}

	/// Call the destructor of p of type U
	template<typename U>
	void destroy(U* p) {
		p->~U();
	}

	/// Get the max allocation size
	size_type max_size() const {
		return size_type(-1);
	}

	/// A struct to rebind the allocator to another allocator of type U
	template<typename U>
	struct rebind {
		using other = Allocator<U>;
	};
};

/// Global size of allocaed memory
MOTR_API extern size_t gAllocatedMemory;

MOTR_NS_END

#endif