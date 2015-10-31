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
#include <core/version.h>

MOTR_NS_BEGIN

Version::Version(const std::string &versionString) { 
	std::istringstream iss(versionString);
	std::string str[3]; 

	for (int i = 0; i < 3; ++i)
		std::getline(iss, str[i], '.');

	std::istringstream(str[0]) >> major_;
	std::istringstream(str[1]) >> minor_;
	std::istringstream(str[2]) >> release_;
}

std::string Version::toString() const {
	std::stringstream ss;
	ss << major_ << "." << minor_ << "." << release_;
	return ss.str();
}
std::string Version::toStringComplete() const {
	std::stringstream ss;
	ss << toString() << " (" << MOTR_PLATFORM_STRING;
#ifdef MOTR_64BIT
	ss << ", 64 Bit";
#else
	ss << ", 32 Bit";
#endif
	ss << ")";
	return ss.str();
}

MOTR_NS_END

