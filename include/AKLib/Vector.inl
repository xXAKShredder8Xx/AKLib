#include "AKLib/Allocator.h"

NSB(AKLib)

template <typename T>
AKL_INLINE Vector<T>::Vector()
	: m_Elements{ NULL }, m_Size{ 0 }, m_Capacity{ 0 }
{
}

template <typename T>
template <typename ...Args>
AKL_INLINE Vector<T>::Vector(size_t len, Args... defaultArgs)
	: m_Elements{ NULL }, m_Size{ 0 }, m_Capacity{ 0 }
{
	m_Size = 0;
	m_Capacity = Grow(len);
	m_Elements = Allocator<T>::Allocate(m_Capacity, false, defaultArgs...);
}

template <typename T>
AKL_INLINE Vector<T>::Vector(const std::initializer_list<T>& list)
	: m_Size{ list.size() }, m_Capacity{ Grow(m_Size) }, 
	m_Elements{ Allocator<T>::Duplicate((T*)list.begin(), list.size(), false) }
{
}

template <typename T>
Vector<T>& Vector<T>::operator = (const Vector<T>& other) 
{
	m_Size = other.m_Size;
	m_Capacity = other.m_Capacity;
	m_Elements = Allocator<T>::Duplicate(other.m_Elements, m_Capacity, false);

	return *this;
}

template <typename T>
AKL_INLINE Vector<T>::~Vector() 
{
	if (m_Elements != NULL) 
	{
		Allocator<T>::Deallocate(m_Elements, false);
		m_Capacity = 0;
		m_Size = 0;
		m_Elements = NULL;
	}
}

template <typename T>
AKL_INLINE Iterator<T> Vector<T>::begin() 
{
	return m_Elements;
}

template <typename T>
AKL_INLINE ConstIterator<T> Vector<T>::begin() const
{
	return m_Elements;
}

template <typename T>
AKL_INLINE Iterator<T> Vector<T>::end()
{
	return m_Elements + m_Size;
}

template <typename T>
AKL_INLINE ConstIterator<T> Vector<T>::end() const
{
	return m_Elements + m_Size;
}

template <typename T>
AKL_INLINE Iterator<T> Vector<T>::find(const T& t) 
{
	for (Iterator<T> i = begin(); i != end(); ++i) 
	{
		if (*i == t) return i;
	}

	return end();
}

template <typename T>
AKL_INLINE ConstIterator<T> Vector<T>::find(const T& t) const
{
	for (ConstIterator<T> i = begin(); i != end(); ++i)
	{
		if (*i == t) return i;
	}

	return end();
}

template <typename T>
AKL_INLINE bool Vector<T>::erase(Iterator<T> pos) 
{
	if (pos != end())
	{
		Allocator<T>::Copy(pos.get(), pos.get() + 1, end() - pos + 1, false);
		return true;
	}

	return false;
}

template <typename T>
AKL_INLINE bool Vector<T>::erase(ConstIterator<T> pos) const
{
	if (pos != end())
	{
		Allocator<T>::Copy((void*)pos.get(), (void*)(pos.get() + 1), end() - pos + 1);
		return true;
	}

	return false;
}

template <typename T>
AKL_INLINE bool Vector<T>::push_back(const T& t) 
{
	if (m_Size == m_Capacity) 
	{
		if (!AdjustCapacity(m_Capacity + 1)) return false;
	}

	*end().get() = t;
	m_Size++;
	return true;
}

template <typename T>
template <typename ...Args>
AKL_INLINE bool Vector<T>::emplace_back(Args... args)
{
	if (m_Size == m_Capacity)
	{
		if (!AdjustCapacity(m_Capacity + 1)) return false;
	}

	*end().get() = T(args...);
	
	m_Size++;
	return true;
}

template <typename T>
template <typename ...Args>
AKL_INLINE Iterator<T> Vector<T>::emplace(Iterator<T> pos, Args... args)
{
	size_t index = pos.get() - m_Elements;
	size_t size = m_Size - index;

	if (index < 0 || index > m_Size) 
	{
		return end();
	}

	if (m_Size == m_Capacity) 
	{
		if (!AdjustCapacity(m_Capacity + 1)) return end();
	}

	Iterator<T> it = &m_Elements[index];

	Allocator<T>::Copy(it.get(), it.get() + 1, size, false);

	*it = T(args...);

	m_Size++;
	return it;
}

template <typename T>
template <typename ...Args>
AKL_INLINE bool Vector<T>::reserve(size_t newCapacity, Args... defaultArgs)
{
	bool result = AdjustCapacity(m_Capacity + newCapacity, defaultArgs...);
	m_Capacity = newCapacity;
	return result;
}

template <typename T>
template <typename ...Args>
AKL_INLINE bool Vector<T>::resize(size_t newSize, Args... defaultArgs)
{
	size_t oldSize = m_Capacity;
	m_Size = newSize;
	m_Capacity = Grow(newSize);
	m_Elements = Allocator<T>::Reallocate(m_Elements, m_Capacity, oldSize, false, defaultArgs...);
	return m_Elements != NULL;
}

template <typename T>
AKL_INLINE T& Vector<T>::operator [] (int idx) 
{
	return m_Elements[idx];
}

template <typename T>
AKL_INLINE size_t Vector<T>::Grow(size_t newSize) const
{
	if (m_Capacity > (SIZE_MAX - (m_Capacity >> 1))) 
	{
		return newSize;
	}

	size_t calcSize = m_Capacity + (m_Capacity >> 1);

	if (calcSize < newSize) 
	{
		return newSize;
	}

	return calcSize;
}

template <typename T>
template <typename ...Args>
AKL_INLINE bool Vector<T>::AdjustCapacity(size_t size, Args... defaultArgs)
{
	size_t newCapacity = Grow(size);

	T* newData = Allocator<T>::Reallocate(m_Elements, newCapacity, m_Capacity, false, defaultArgs...);

	m_Elements = newData;
	m_Capacity = newCapacity;

	return newData != NULL;
}

NSE()