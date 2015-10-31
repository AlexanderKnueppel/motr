#ifndef __MOTR_CORE_VERSION_H__
#define __MOTR_CORE_VERSION_H__  

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
* \brief Current release of Chromosome
* \ingroup libcore
*/
#define MOTR_VERSION "0.1.0"

/**
* \brief Year of current release
* \ingroup libcore
*/
#define MOTR_YEAR "2015"

/**
* \brief Data structure for comparing versions of Chromosome
*
* \ingroup core
*/
struct MOTR_API Version {
public:
	/// Default constructor: initialize to an invalid version (0.0.0)
	inline Version() : major_(0), minor_(0), release_(0) { }

	/// Initialize with the specified version number
	inline Version(int major, int minor, int release)
		: major_(major), minor_(minor), release_(release) { }

	/**
	* \brief Parse a version string of the form "major.minor.release"
	* and turn it into a \ref Version structure
	*/
	Version(const std::string &versionString);

	/// Check if this program version is \a older than \c other
	inline bool operator<(const Version &other) const {
		if (major_ < other.major_)
			return true;
		else if (major_ > other.major_)
			return false;
		else if (minor_ < other.minor_)
			return true;
		else if (minor_ > other.minor_)
			return false;
		else if (release_ < other.release_)
			return true;
		else
			return false;
	}

	/// Check if this program version is \a older than or equal to \c other
	inline bool operator<=(const Version &other) const {
		return (*this < other) || (*this == other);
	}

	/// Check if two program versions match
	inline bool operator==(const Version &other) const {
		return major_ == other.major_
			&& minor_ == other.minor_
			&& release_ == other.release_;
	}

	/// Is this a valid version number?
	inline bool isValid() {
		return major_ != 0 || minor_ != 0 || release_ != 0;
	}

	/// Are the following two versions compatible?
	inline bool isCompatible(const Version &other) const {
		return major_ == other.major_ &&
			minor_ == other.minor_;
	}

	/// Turn into a string of the form "major.minor.release"
	std::string toString() const;

	/// Turn into a string of the form "major.minor.release (Architecture)"
	std::string toStringComplete() const;

	/// Return the major version
	inline int major() const { return major_; }

	/// Return the minor version
	inline int minor() const { return minor_; }

	/// Return the release
	inline int release() const { return release_; }
private:
	int major_;
	int minor_;
	int release_;
};

MOTR_NS_END

#endif