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

using ::motr::Version;
using ::testing::Eq;
using ::testing::HasSubstr;

/*
* Test if initial version equals 0/0/0
*/
TEST(Version, HoldsZeroVersionIfUninitialized) {
	Version version;
	ASSERT_THAT(version.major(), Eq(0));
	ASSERT_THAT(version.minor(), Eq(0));
	ASSERT_THAT(version.release(), Eq(0));
}
/*
* Test if versioning works as expected
*/
TEST(Version, HoldsCorrectVersionIfInitialized) {
	Version version(1,2,3);
	ASSERT_THAT(version.major(), Eq(1));
	ASSERT_THAT(version.minor(), Eq(2));
	ASSERT_THAT(version.release(), Eq(3));
}
/*
* Test if version as string is parsed correctly
*/
TEST(Version, TokenizesGivenVersionStringCorrectly) {
	Version version("1.2.3");
	ASSERT_THAT(version.major(), Eq(1));
	ASSERT_THAT(version.minor(), Eq(2));
	ASSERT_THAT(version.release(), Eq(3));
}
/*
* Test if a version string with expected format is returned
*/
TEST(Version, ReturnsVersionAsString) {
	Version version(1,2,3);
	ASSERT_THAT(version.toString(), Eq("1.2.3"));
}
/*
* Test if a more meaningful version as string with expected format is returned
*/
TEST(Version, ReturnsVersionAsStringWithArchitecture) {
	Version version(1, 2, 3);

	ASSERT_THAT(version.toStringComplete(), HasSubstr("1.2.3"));

#ifdef MOTR_PLATFORM_WINDOWS
	ASSERT_THAT(version.toStringComplete(), testing::HasSubstr("Windows"));
#elif defined(MOTR_PLATFORM_LINUX)
	ASSERT_THAT(version.toStringComplete(), testing::HasSubstr("Linux"));
#else
	ASSERT_THAT(version.toStringComplete(), testing::HasSubstr("Unknown"));
#endif

#if defined(_M_X64) || defined(__amd64__)
	ASSERT_THAT(version.toStringComplete(), testing::HasSubstr("64 Bit"));
#else
	ASSERT_THAT(version.toStringComplete(), testing::HasSubstr("32 Bit"));
#endif
}
