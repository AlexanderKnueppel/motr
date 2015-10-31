#ifndef __MOTR_CONFIGURATION_H__
#define __MOTR_CONFIGURATION_H__  

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

#if defined(__WIN32__) || defined(_MSC_VER)
#   define MOTR_PLATFORM_WINDOWS
#   define MOTR_PLATFORM_STRING "Windows"
#elif defined(__linux__) || defined(__unix__)
#   define MOTR_PLATFORM_LINUX
#   define MOTR_PLATFORM_STRING "Linux"
#elif defined(__APPLE__) || defined(MACOSX)
#   define MOTR_PLATFORM_MACOSX // unsupported yet
#   error motr is not supported under "Apple MacOSX" yet!
#   define MOTR_PLATFORM_STRING "Mac OS"
#elif defined(__FreeBSD__)
#   define MOTR_PLATFORM_FREEBSD // unsupported yet
#   error motr is not supported under "FreeBSD" yet!
#   define MOTR_PLATFORM_STRING "FreeBSD"
#elif defined(__sparc__) || defined(__sun__)
#   define MOTR_PLATFORM_SOLARIS // unsupported yet
#   error motr is not supported under "Sun Solaris" yet!
#   define MOTR_PLATFORM_STRING "Sun Solaris"
#else
#   error Unknown platform, no support!
#   define MOTR_PLATFORM_STRING "Unknown"
#endif

#if defined(_DEBUG) 
#	define MOTR_DEBUG
#else
#	define MOTR_RELEASE
#endif

#ifdef MOTR_EXPORTS
#   if defined(MOTR_PLATFORM_WINDOWS)
#       define MOTR_API __declspec(dllexport)
#   elif defined(MOTR_PLATFORM_LINUX)
#       define MOTR_API
#   endif
#else
#   if defined(MOTR_PLATFORM_WINDOWS)
#		define MOTR_API __declspec(dllimport)
#	else
#		define MOTR_API
#   endif
#endif

#if defined(_M_X64) || defined(__amd64__)
#	define MOTR_64BIT
#else
#	define MOTR_32BIT
#endif

#if defined(__GNUC__)
#   define MOTR_COMPILER_GCC  // GNU Compiler Collection
#elif defined(_MSC_VER)
#   define MOTR_COMPILER_VC   // VisualC++
#endif

#if defined(MOTR_COMPILER_GCC)
// optimization here
#	define MOTR_NOEXCEPT noexcept
#elif defined(MOTR_COMPILER_VC)
//Only on windows
#	pragma warning( disable: 4251 ) //Warning: needs to have dll-interface to be used by clients, it's a nuisance (template issues...)
// optimization here
#	define MOTR_NOEXCEPT 
#endif

#include <cstdint>
#include <exception>

#define MOTR_NS motr

#define MOTR_NS_BEGIN namespace MOTR_NS {
#define MOTR_NS_END }

#endif
