#include <stdlib.h>

NSB(AKLib)

template <typename T>
template <typename ...Args>
AKL_INLINE T* Allocator<T>::Allocate(size_t size, bool cPtr, Args... args)
{
	T* ptr = NULL;

	ptr = (T*)((char*)malloc(size * sizeof(T) + sizeof(size_t)) + sizeof(size_t));

	if ((ptr - sizeof(size_t)) == NULL) return NULL;

	SetSize(ptr, size);

	if (!cPtr)
	{
		for (int i = 0; i < size; i++)
		{
			new (ptr + i) T{ std::forward<Args>(args)... };
		}
	}

	return ptr;
}

template <typename T>
AKL_INLINE T* Allocator<T>::Deallocate(T* ptr, bool cPtr)
{
	if (ptr == NULL) return NULL;
	
	size_t size = GetSize(ptr);

	if (!cPtr)
	{
		for (int i = 0; i < size; i++)
		{
			(ptr + i)->~T();
		}
	}

	free((char*)ptr - sizeof(size_t));

	return NULL;
}

template <typename T>
template <typename ...Args>
AKL_INLINE T* Allocator<T>::Reallocate(T* ptr, size_t size, size_t oldSize, bool cPtr, Args... args)
{
	if (size == 0)
	{
		if (ptr == NULL)
		{
			return NULL;
		}

		return Deallocate(ptr, cPtr);
	}

	if (ptr == NULL)
	{
		return Allocate(size, cPtr, args...);
	}

	if (size == oldSize) return ptr;

	T* ptr2 = Allocate(size, cPtr, args...);

	if (ptr2 == NULL)
	{
		return ptr;
	}

	if (oldSize < size)
	{
		Copy(ptr, ptr2, oldSize, cPtr);
	}
	else 
	{
		Copy(ptr, ptr2, size, cPtr);
	}

	Deallocate(ptr, cPtr);

	return ptr2;
}

template <typename T>
template <typename ...Args>
AKL_INLINE T* Allocator<T>::Duplicate(T* ptr, size_t size, bool cPtr, Args... args)
{
	T* ptr2 = Allocate(size, cPtr, args...);
	if (ptr2 == NULL) return NULL;
	Copy(ptr, ptr2, size, cPtr);
	return ptr2;
}

template <typename T>
AKL_INLINE void Allocator<T>::Copy(void* src, void* dest, size_t size, bool cPtr)
{
	if (src == NULL || dest == NULL) return;

	if (!cPtr) 
	{
		size_t numLoops = size;

		T* _src = static_cast<T*>(src);
		T* _dest = static_cast<T*>(dest);

		for (int i = 0; i < numLoops; i++)
		{
			*(_dest + i) = *(_src + i);
		}

		return;
	}

	size = size * sizeof(T);

	if (size % 4 == 0)
	{
		size_t numLoops = size >> 2;

		uint32_t* _src = static_cast<uint32_t*>(src);
		uint32_t* _dest = static_cast<uint32_t*>(dest);

		for (int i = 0; i < numLoops; i++)
		{
			*(_dest + i) = *(_src + i);
		}
	}
	else if (size % 2 == 0)
	{
		size_t numLoops = size >> 1;

		uint16_t* _src = static_cast<uint16_t*>(src);
		uint16_t* _dest = static_cast<uint16_t*>(dest);

		for (int i = 0; i < numLoops; i++)
		{
			*(_dest + i) = *(_src + i);
		}
	}
	else
	{
		uint8_t* _src = static_cast<uint8_t*>(src);
		uint8_t* _dest = static_cast<uint8_t*>(dest);

		for (int i = 0; i < size; i++)
		{
			*(_dest + i) = *(_src + i);
		}
	}
}

template <typename T>
AKL_INLINE void Allocator<T>::Zero(void* ptr, size_t size)
{
	if (ptr == NULL) return;

	size = size * sizeof(T);

	if (size % 4 == 0)
	{
		size_t numLoops = size >> 2;

		uint32_t* _ptr = static_cast<uint32_t*>(ptr);

		for (int i = 0; i < numLoops; i++)
		{
			*(_ptr + i) = 0;
		}
	}
	else if (size % 2 == 0)
	{
		size_t numLoops = size >> 1;

		uint16_t* _ptr = static_cast<uint16_t*>(ptr);

		for (int i = 0; i < numLoops; i++)
		{
			*(_ptr + i) = 0;
		}
	}
	else
	{
		uint8_t* _ptr = static_cast<uint8_t*>(ptr);

		for (int i = 0; i < size; i++)
		{
			*(_ptr + i) = 0;
		}
	}
}

template <typename T>
AKL_INLINE bool Allocator<T>::Compare(void* ptr1, void* ptr2, size_t size)
{
	char* _ptr1 = static_cast<char*>(ptr1);
	char* _ptr2 = static_cast<char*>(ptr2);

	for (int i = 0; i < size; i++) 
	{
		if (_ptr1[i] != _ptr2[i])
			return false;
	}

	return true;
}

template <typename T>
AKL_INLINE void Allocator<T>::SetSize(void* ptr,  size_t size)
{
	size_t* _s_ptr = (size_t*)((char*)ptr - sizeof(size_t));
	*_s_ptr = size;
}

template <typename T>
AKL_INLINE size_t Allocator<T>::GetSize(void* ptr)
{
	size_t* _s_ptr = (size_t*)((char*)ptr - sizeof(size_t));
	return *_s_ptr;
}

template <typename T>
AKL_INLINE bool MemoryHelper::Alloc(T*& ptr, size_t count)
{
	return (ptr = Allocator<T>::Allocate(count, false)) != NULL;
}

template <typename T>
AKL_INLINE bool MemoryHelper::Free(T*& ptr, size_t count)
{
	return (ptr = Allocator<T>::Deallocate(ptr, false)) == NULL;
}

NSE()