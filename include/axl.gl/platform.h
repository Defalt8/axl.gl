#pragma once
#ifndef _PLATFORM_H___
#define _PLATFORM_H___ 1

#define PLATFORM_UNKNOWN 0
#define PLATFORM_WINDOWS 1
#define PLATFORM_IPHONE 2
#define PLATFORM_ANDROID 3
#define PLATFORM_MAC 4
#define PLATFORM_LINUX 5
#define PLATFORM_UNIX 6
#define PLATFORM_POSIX 7

#define ABI_UNKNOWN 0
#define ABI_WIN32 1
#define ABI_WIN64 2
#define ABI_ANDROID_ARM 3
#define ABI_ANDROID_ARM64 4
#define ABI_ANDROID_X86 5
#define ABI_ANDROID_X86_64 6
#define ABI_ANDROID_MIPS64 7
#define ABI_ANDROID_MIPS 8
#define ABI_LINUX_I386 9
#define ABI_LINUX_ARM 10
#define ABI_LINUX_X86_64 11

#ifndef _PLATFORM_DEFINED_
#define _PLATFORM_DEFINED_ 1
#	if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) // WINDOWS
#		define PLATFORM PLATFORM_WINDOWS
#	elif defined(__ANDROID__) 		// android
#		define PLATFORM PLATFORM_ANDROID
#	elif defined(__APPLE__) 		// apple
#		include <TargetConditionals.h>
#		if defined(TARGET_OS_IPHONE)	// apple iphone
#			define PLATFORM PLATFORM_IPHONE
#		elif defined(TARGET_OS_MAC)	// apple mac os
#			define PLATFORM PLATFORM_MAC
#		else						// unknown apple product
#			define PLATFORM PLATFORM_UNKNOWN
#		endif
#	elif defined(__linux__) 		// linux
#		define PLATFORM PLATFORM_LINUX
#	elif defined(__unix__) 		// all unixes not caught above
#		define PLATFORM PLATFORM_UNIX
#	elif defined(_POSIX_VERSION) 	// posix
#		define PLATFORM PLATFORM_POSIX
#	else 							// unknown
#		define PLATFORM PLATFORM_UNKNOWN
#	endif
#endif // _PLATFORM_DEFINED_

#ifndef _ABI_DEFINED_
#define _ABI_DEFINED_ 1
#	if PLATFORM==PLATFORM_WINDOWS
#		ifdef _WIN64
#			define ABI ABI_WIN64
#		elif defined(_WIN32)
#			define ABI ABI_WIN32
#		else
#			define ABI ABI_UNKNOWN
#		endif
#	elif PLATFORM==PLATFORM_ANDROID
#		if defined(__arm__)
#			define ABI ABI_ANDROID_ARM
#		elif defined(__aarch64__)
#			define ABI ABI_ANDROID_ARM64
#		elif defined(__i386__)
#			define ABI ABI_ANDROID_X86
#		elif defined(__x86_64__)
#			define ABI ABI_ANDROID_X86_64
#		elif defined(__mips64)
#			define ABI ABI_ANDROID_MIPS64
#		elif defined(__mips__)
#			define ABI ABI_ANDROID_MIPS
#		else
#			define ABI ABI_UNKNOWN
#		endif
#	elif PLATFORM==PLATFORM_LINUX
#		if defined(__arm__)
#			define ABI ABI_LINUX_ARM
#		elif defined(__aarch64__)
#			define ABI ABI_LINUX_ARM64
#		elif defined(__i386__)
#			define ABI ABI_LINUX_X86
#		elif defined(__x86_64__)
#			define ABI ABI_LINUX_X86_64
#		else
#			define ABI ABI_UNKNOWN
#		endif
#	else
#		define ABI ABI_UNKNOWN
#	endif
#endif // _ABI_DEFINED_

#endif // _PLATFORM_H___