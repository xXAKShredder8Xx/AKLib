#ifndef AKLIB_DLL_LOADER_H
#define AKLIB_DLL_LOADER_H

#include "AKLib/common.h"

#ifdef AKLIB_PLATFORM_WINDOWS 
#include <Windows.h>
#define AKLIB_DLL_TYPE HMODULE
#define AKLIB_DLL_OPEN(fileName) LoadLibraryA(fileName)
#define AKLIB_DLL_CLOSE(handle) FreeLibrary(handle)
#define AKLIB_DLL_GET_FUNC(name, handle) GetProcAddress(handle, name)
#endif

#ifdef AKLIB_PLATFORM_LINUX
#include <dlfcn.h>
#define AKLIB_DLL_TYPE void*
#define AKLIB_DLL_OPEN(fileName) dlopen(fileName, RTLD_LAZY)
#define AKLIB_DLL_CLOSE(handle) dlclose(handle)
#define AKLIB_DLL_GET_FUNC(name, handle) dlsym(handle, name)
#endif

NSB(AKLib)

class AKLIB_API DllLoader 
{
public:
	inline DllLoader(const char* dllName);
	inline ~DllLoader();

	inline bool Open();
	inline bool Close();

	inline bool IsOpen();

	template <typename FuncType>
	inline bool GetFunction(const char* functionName, FuncType* func);
private:
	bool isOpen;
	const char* name;
	AKLIB_DLL_TYPE handle;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/DLL/DLL_Loader.inl"

#endif // AKLIB_DLL_LOADER_H