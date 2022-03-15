#ifndef AK_LIB_SHARED_PTR_H
#define AK_LIB_SHARED_PTR_H

#include "AKLib/common.h"

NSB(AKLib)

template <typename T>
class AKLIB_API SharedPtr
{
public:
	inline SharedPtr();
	inline SharedPtr(T* ptr);
	template <typename ...Args>
	inline SharedPtr(Args... args);
	inline SharedPtr(const SharedPtr<T>& other);
	inline SharedPtr<T>& operator = (const SharedPtr<T>& other);
	inline SharedPtr<T>& operator = (const T* other);
	inline ~SharedPtr();

	inline void reset(const T* other);

	inline T* operator -> ();
	inline const T* operator -> () const;
	inline T& operator * ();
	inline const T& operator * () const;

	inline T*& Get();
	inline int*& Ref();
private:
	inline void inc();
	inline void dec();
	T* ptr;
	int* ref;
};

template <typename T>
inline AKLIB_API SharedPtr<T> MakeShared(const T& t);

NSE()

#define AKL_INLINE inline
#include "AKLib/SharedPtr.inl"

#endif // AK_LIB_SHARED_PTR_H