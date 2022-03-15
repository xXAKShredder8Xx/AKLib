#include "AKLib/Allocator.h"

NSB(AKLib)

//template <typename T>
AKL_INLINE void* AllocatorObject::operator new(size_t size)
{
	return Allocator<AllocatorObject>::Allocate(size, true);
}

//template <typename T>
AKL_INLINE void* AllocatorObject::operator new[](size_t size)
{
	return Allocator<AllocatorObject>::Allocate(size, true);
}

//template <typename T>
AKL_INLINE void AllocatorObject::operator delete(void* ptr, size_t size)
{
	Allocator<AllocatorObject>::Deallocate(static_cast<AllocatorObject*>(ptr), true);
}

//template <typename T>
AKL_INLINE void AllocatorObject::operator delete[](void* ptr, size_t size)
{
	Allocator<AllocatorObject>::Deallocate(static_cast<AllocatorObject*>(ptr), true);
}

NSE()