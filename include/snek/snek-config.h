#pragma once

#include <cassert>

#define SN_WIN 1
#define SN_MAC 2
#define SN_LINUX 3

#ifndef SN_PLATFORM
#	if defined(WIN32) || defined(_WIN32)
#		define SN_PLATFORM SN_WIN
#	elif defined(__APPLE__)
#		define SN_PLATFORM SN_MAC
#	elif defined(__linux)
#		define SN_PLATFORM SN_LINUX
#	else
#		error Platform not supported.
#	endif
#endif

#define SN_X86 1
#define SN_X64 2

#ifndef SN_ARCH
#	if defined(_M_X64)
#		define SN_ARCH SN_X64
#	elif defined(_M_IX86)
#		define SN_ARCH SN_X86
#	else
#		error Architecture not supported.
#	endif
#endif

#if defined(_DEBUG) && !defined(NDEBUG)
#	define SN_DEBUG
#endif

#define SN_ASSERT assert