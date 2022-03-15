#ifndef AK_LIB_COMMON_H
#define AK_LIB_COMMON_H

#define NSB(ns) namespace ns {
#define NSE() }

#include <stdint.h>
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#define AKLIB_PLATFORM_WINDOWS
#else
#define AKLIB_PLATFORM_LINUX
#endif

#if defined(AKLIB_ENABLE_DLL) 
	#if defined(AKLIB_PLATFORM_WINDOWS)
		#if defined(AKLIB_BUILD_DLL)
			#define AKLIB_API __declspec(dllexport)
		#else
			#define AKLIB_API __declspec(dllimport)
		#endif
	#else
		#error "AKLib dll mode only supports windows!"
	#endif
#else
	#define AKLIB_API 
#endif

#endif // AK_LIB_COMMON_H 