#ifndef AK_LIB_ALLOCATOR_OBJECT_H
#define AK_LIB_ALLOCATOR_OBJECT_H

#include "AKLib/common.h"

NSB(AKLib)

class AKLIB_API AllocatorObject
{
public:
	inline static void* operator new(size_t size);
	inline static void* operator new[](size_t size);
	inline static void operator delete(void* ptr, size_t size);
	inline static void operator delete[](void* ptr, size_t size);
};

NSE()

#define AKL_INLINE inline
#include "AKLib/AllocatorObject.inl"

#endif // AK_LIB_ALLOCATOR_OBJECT_H