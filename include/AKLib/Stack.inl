#include "AKLib/Allocator.h"

NSB(AKLib) 

template <typename T>
AKL_INLINE Stack<T>::Stack() 
	: elements{ NULL }, size{ 0 }, idx{ 0 }, static_size{ false }
{
}

template <typename T>
AKL_INLINE Stack<T>::Stack(size_t size)
	: elements{ Allocator<T>::Allocate(size) }, size{ size }, idx{ 0 }, static_size{ true }
{
}

template <typename T>
AKL_INLINE Stack<T>::~Stack() 
{
	if (elements != NULL) 
	{
		elements = Allocator<T>::Deallocate(elements);
		size = 0;
		idx = 0;
	}
}

template <typename T>
AKL_INLINE void Stack<T>::Resize(size_t newSize, bool staticSize)
{
	elements = Allocator<T>::Reallocate(elements, newSize, size);
	size = newSize;

	if (size > 0 && idx >= size)
	{
		idx = size - 1;
	}

	if (size == 0) idx = 0;

	static_size = staticSize;
}

template <typename T>
AKL_INLINE void Stack<T>::Reset(size_t newSize, bool staticSize)
{
	Resize(newSize, staticSize);
	idx = 0;
}

template <typename T>
AKL_INLINE T* Stack<T>::begin() 
{
	return elements;
}

template <typename T>
AKL_INLINE T* Stack<T>::end() 
{
	return elements + idx;
}

template <typename T>
AKL_INLINE void Stack<T>::Push(const T& v) 
{
	Check((size << 1) + 1);
	elements[idx] = v;
	idx++;
}

template <typename T>
AKL_INLINE T Stack<T>::Pop() 
{
	CheckLess((size >> 1));
	idx--;
	return elements[idx];
}

template <typename T>
AKL_INLINE T* Stack<T>::Data() 
{
	return elements;
}

template <typename T>
AKL_INLINE size_t Stack<T>::Size() 
{
	return size;
}

template <typename T>
AKL_INLINE int Stack<T>::Index() 
{
	return idx - 1;
}

template <typename T>
AKL_INLINE T& Stack<T>::operator [] (int index) 
{
	if ((index + 1) > size) 
	{
		printf("Index(%d) exceeds stack bounds!\n", index);
		exit(1);
	}

	return elements[index];
}

template <typename T>
AKL_INLINE Stack<T>& Stack<T>::operator << (const T& v)
{
	Push(v);
	return *this;
}

template <typename T>
AKL_INLINE Stack<T>& Stack<T>::operator >> (T& v) 
{
	v = Pop();
	return *this;
}

template <typename T>
AKL_INLINE void Stack<T>::Check(size_t newSize) 
{
	if (idx >= size) 
	{
		if (!static_size)
		{
			Resize(newSize);
		}
		else 
		{
			printf("Overflow of stack constraints!\n");
			exit(1);
		}
	}
}

template <typename T>
AKL_INLINE void Stack<T>::CheckLess(size_t newSize)
{
	if (idx <= (size >> 1)) 
	{
		Resize(newSize);
	}
}

NSE()