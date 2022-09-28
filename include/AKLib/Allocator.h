#ifndef AK_SERIAL_ALLOCATOR_H
#define AK_SERIAL_ALLOCATOR_H

#include "AKLib/common.h"

#define AKLIB_NEW(type, ...) ::AKLib::Allocator<type>::Allocate(1, false, __VA_ARGS__)
#define AKLIB_DELETE(type, ptr) ::AKLib::Allocator<type>::Deallocate(ptr, false)

NSB(AKLib)

template <typename T>
struct AKLIB_API AllocatorPtr 
{
	bool IsCPtr;
	uint32_t Count;
	T* Ptr;
};

template <typename T>
class AKLIB_API Allocator
{
public:
	template <typename ...Args>
	inline static T* Allocate(size_t size, bool cPtr, Args... args);
	inline static T* Deallocate(T* ptr, bool cPtr);
	template <typename ...Args>
	inline static T* Reallocate(T* ptr, size_t size, size_t oldSize, bool cPtr, Args... args);
	template <typename ...Args>
	inline static T* Duplicate(T* ptr, size_t size, bool cPtr, Args... args);
	inline static void Copy(void* src, void* dest, size_t size, bool cPtr);
	inline static void Zero(void* ptr, size_t size);
	inline static bool Compare(void* ptr1, void* ptr2, size_t size);
	inline static void SetSize(void* ptr, size_t size);
	inline static size_t GetSize(void* ptr);
};

struct MemoryHelper 
{
	template <typename T>
	inline static bool Alloc(T*& ptr, size_t count);

	template <typename T>
	inline static bool Free(T*& ptr, size_t count);
};

NSE()

#define AKL_INLINE inline
#include "AKLib/Allocator.inl"

#endif // AK_SERIAL_ALLOCATOR_H