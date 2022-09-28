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

NSB(AKLib)
typedef signed __int8 i8;
typedef signed __int16 i16;
typedef signed __int32 i32;
typedef signed __int64 i64;

typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
NSE()

#endif // AK_LIB_COMMON_H 