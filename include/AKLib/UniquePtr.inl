
NSB(AKLib)

template <typename T>
AKL_INLINE UniquePtr<T>::UniquePtr() 
	: ptr{ NULL }
{
}

template <typename T>
AKL_INLINE UniquePtr<T>::UniquePtr(const T* other)
	: ptr{ (T*)other }
{
}

template <typename T>
AKL_INLINE UniquePtr<T>& UniquePtr<T>::operator = (const T* other) 
{
	if (ptr != NULL) delete ptr;

	ptr = (T*)other;

	return *this;
}

template <typename T>
AKL_INLINE UniquePtr<T>& UniquePtr<T>::operator = (const UniquePtr<T>& other)
{
	if (ptr != NULL) delete ptr;

	ptr = (T*)other.ptr;
	((UniquePtr<T>*)&other)->ptr = NULL;

	return *this;
}

template <typename T>
AKL_INLINE void UniquePtr<T>::reset(const T* other) 
{
	if (ptr != NULL) delete ptr;
	ptr = (T*)other;
}

template <typename T>
AKL_INLINE UniquePtr<T>::~UniquePtr()
{
	if (ptr != NULL) delete ptr;
}

template <typename T>
AKL_INLINE T& UniquePtr<T>::operator * () const
{
	return *ptr;
}

template <typename T>
AKL_INLINE T* UniquePtr<T>::operator -> () const
{
	return ptr;
}

NSE()