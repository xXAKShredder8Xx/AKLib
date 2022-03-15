#ifndef AK_LIB_UNIQUE_PTR_H
#define AK_LIB_UNIQUE_PTR_H

#include "AKLib/common.h"

NSB(AKLib)

template <typename T>
class AKLIB_API UniquePtr
{
public:
	inline UniquePtr();
	inline UniquePtr(const T* ptr);
	inline UniquePtr<T>& operator = (const T* other);
	inline UniquePtr<T>& operator = (const UniquePtr<T>& other);
	inline ~UniquePtr();

	inline void reset(const T* other);

	inline T* operator -> () const;
	inline T& operator * () const;


private:
	T* ptr;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/UniquePtr.inl"

#endif // AK_LIB_UNIQUE_PTR_H