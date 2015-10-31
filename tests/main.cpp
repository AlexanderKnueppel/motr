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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

/*
* Entry point for unit tests with google test
*/
int main(int argc, char** argv) {
	testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
