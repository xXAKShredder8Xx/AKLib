
NSB(AKLib)

template <typename T>
AKL_INLINE SharedPtr<T>::SharedPtr() 
	: ptr{ NULL }, ref{ NULL }
{
}

template <typename T>
AKL_INLINE SharedPtr<T>::SharedPtr(T* ptr)
	: ptr{ ptr }, ref{ new int }
{
	*ref = 1;
}

template <typename T>
template <typename ...Args>
AKL_INLINE SharedPtr<T>::SharedPtr(Args... args) 
	: ptr{ new T{ args... }}, ref{ new int }
{
	*ref = 1;
}

template <typename T>
AKL_INLINE SharedPtr<T>::SharedPtr(const SharedPtr<T>& other)
	: ptr{ other.ptr }, ref{ other.ref }
{
	inc();
}

template <typename T>
AKL_INLINE SharedPtr<T>& SharedPtr<T>::operator = (const SharedPtr<T>& other)
{
	dec();

	ptr = other.ptr;
	ref = other.ref;

	inc();
	return *this;
}

template <typename T>
AKL_INLINE SharedPtr<T>& SharedPtr<T>::operator = (const T* other)
{
	dec();

	if (other != NULL)
	{
		ptr = (T*)other;
		ref = new int;
		*ref = 1;
	}

	return *this;
}

template <typename T>
AKL_INLINE SharedPtr<T>::~SharedPtr()
{
	dec();
}

template <typename T>
AKL_INLINE void SharedPtr<T>::reset(const T* other) 
{
	dec();

	if (other != NULL)
	{
		ptr = (T*)other;
		ref = new int;
		*ref = 1;
	}
}

template <typename T>
AKL_INLINE T* SharedPtr<T>::operator -> ()
{
	return ptr;
}

template <typename T>
AKL_INLINE const T* SharedPtr<T>::operator -> () const
{
	return ptr;
}

template <typename T>
AKL_INLINE T& SharedPtr<T>::operator * ()
{
	return *ptr;
}

template <typename T>
AKL_INLINE const T& SharedPtr<T>::operator * () const
{
	return *ptr;
}

template <typename T>
AKL_INLINE T*& SharedPtr<T>::Get()
{
	return ptr;
}

template <typename T>
AKL_INLINE int*& SharedPtr<T>::Ref()
{
	return ref;
}

template <typename T>
AKL_INLINE void SharedPtr<T>::inc()
{
	if (ref != NULL) (*ref)++;
}

template <typename T>
AKL_INLINE void SharedPtr<T>::dec()
{
	if (ptr != NULL && ref != NULL && *ref == 1)
	{
		delete ref;
		delete ptr;

		ptr = NULL;
		ref = NULL;
	}
	else
	{
		if (ref != NULL) (*ref)--;
	}
}

NSE()