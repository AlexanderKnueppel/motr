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
#include <motr.h>

#include <gmock/gmock.h>

using ::motr::Allocator;
using ::motr::StackAllocator;
using ::testing::Eq;
using ::testing::HasSubstr;

/*
* Test 
*/
TEST(StackAllocator, BasicUsageStackAllocator) {
	StackAllocator<char, false> alloc(128);
	typedef std::basic_string<char, std::char_traits<char>,
		StackAllocator<char, false >> Str;

	Str str(alloc);

	str = "lalala";
	str = "lalalalo";

	ASSERT_THAT(alloc.getMemoryPool().getSize(), Eq(128));
	ASSERT_THAT(str.size(), Eq(8));
}

TEST(Allocator, BasicUsageAllocator) {
	char *p = nullptr;
	ASSERT_THAT(sizeof(p), Eq(0));
	{
		Allocator<char> alloc;
		p = alloc.allocate(5);
		ASSERT_THAT(motr::gAllocatedMemory, Eq(5));
		ASSERT_THAT(p != nullptr, Eq(true));
		ASSERT_THAT(p, Eq(alloc.address(*p)));
		// use p
		strcpy(p, "test");
		// check p
		ASSERT_THAT(strcmp(p, "test") == 0, Eq(true));
		// deallocate memory (don't need to use alloc.address here but whatever)
		alloc.deallocate(alloc.address(*p), 5);
	}
	ASSERT_THAT(motr::gAllocatedMemory, Eq(0));
}
