#ifndef AK_LIB_VECTOR_H
#define AK_LIB_VECTOR_H

#include "AKLib/common.h"
#include <initializer_list>
#include <functional>

NSB(AKLib)

template <typename T>
struct AKLIB_API Iterator 
{
	inline Iterator() 
		: ptr{ NULL }
	{
	}

	inline Iterator(T* ptr) 
		: ptr{ ptr }
	{
	}

	inline Iterator(const Iterator<T>& other)
		: ptr{ other.ptr }
	{
	}

	inline T* get() const
	{
		return ptr;
	}

	inline T* operator -> () const
	{
		return ptr;
	}

	inline T& operator * () const
	{
		return *ptr;
	}

	inline Iterator<T>& operator ++ ()
	{
		ptr++;
		return *this;
	}

	inline Iterator<T>& operator -- ()
	{
		ptr--;
		return *this;
	}

	inline bool operator == (Iterator<T> other) 
	{
		return ptr == other.ptr;
	}

	inline bool operator != (Iterator<T> other)
	{
		return ptr != other.ptr;
	}

	inline operator bool() 
	{
		return ptr != NULL;
	}

private:
	T* ptr;
};

template <typename T>
struct AKLIB_API ConstIterator
{
	inline ConstIterator()
		: ptr{ NULL }
	{
	}

	inline ConstIterator(const T* ptr)
		: ptr{ ptr }
	{
	}

	inline ConstIterator(const Iterator<T>& other)
		: ptr{ other.get() }
	{
	}

	inline ConstIterator(const ConstIterator<T>& other)
		: ptr{ other.ptr }
	{
	}

	inline const T* get() const 
	{
		return ptr;
	}

	inline const T* operator -> () const 
	{
		return ptr;
	}

	inline const T& operator * () const 
	{
		return *ptr;
	}

	inline ConstIterator<T>& operator ++ ()
	{
		ptr++;
		return *this;
	}

	inline ConstIterator<T>& operator -- ()
	{
		ptr--;
		return *this;
	}

	inline bool operator == (ConstIterator<T> other) const
	{
		return ptr == other.ptr;
	}

	inline bool operator != (ConstIterator<T> other) const
	{
		return ptr != other.ptr;
	}

	inline operator bool()
	{
		return ptr != NULL;
	}

private:
	const T* ptr;
};

template <typename T>
class AKLIB_API Vector
{
public:
	using iterator = Iterator<T>;
	using const_iterator = ConstIterator<T>;

	inline Vector();
	template <typename ...Args>
	inline Vector(size_t size, Args... defaultArgs);
	inline Vector(const std::initializer_list<T>& list);
	inline Vector<T>& operator = (const Vector<T>& other);
	inline ~Vector();

	inline Iterator<T> begin();
	inline ConstIterator<T> begin() const;
	inline Iterator<T> end();
	inline ConstIterator<T> end() const;

	inline Iterator<T> find(const T& t);
	inline ConstIterator<T> find(const T& t) const;

	inline bool erase(Iterator<T> pos);
	inline bool erase(ConstIterator<T> pos) const;

	inline bool push_back(const T& t);

	template <typename ...Args>
	inline bool emplace_back(Args... args);

	template <typename ...Args>
	inline Iterator<T> emplace(Iterator<T> pos, Args... args);

	template <typename ...Args>
	inline bool reserve(size_t newCapacity, Args... defaultArgs);
	template <typename ...Args>
	inline bool resize(size_t newSize, Args... defaultArgs);

	inline T& operator [] (int idx);

	inline size_t Grow(size_t newSize) const;
	inline T* data() const { return m_Elements; };
	inline size_t size() const { return m_Size; };
	inline size_t capacity() const { return m_Capacity; }

private:
	template <typename ...Args>
	inline bool AdjustCapacity(size_t size, Args... defaultArgs);
	size_t m_Size;
	size_t m_Capacity;
	T* m_Elements;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/Vector.inl"

#endif // AK_LIB_VECTOR_H