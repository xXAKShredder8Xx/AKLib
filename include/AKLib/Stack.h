#ifndef AK_LIB_STACK_H
#define AK_LIB_STACK_H

#include "AKLib/common.h"

NSB(AKLib)

template <typename T>
class AKLIB_API Stack
{
public:
	inline Stack();
	inline Stack(size_t size);
	inline ~Stack();

	inline void Resize(size_t newSize, bool staticSize = false);
	inline void Reset(size_t newSize = 0, bool staticSize = false);

	inline T* begin();
	inline T* end();

	inline void Push(const T& t);
	inline T Pop();

	inline T* Data();
	inline size_t Size();
	inline int Index();

	inline T& operator [] (int index);
	inline Stack<T>& operator << (const T& v);
	inline Stack<T>& operator >> (T& v);
private:
	inline void Check(size_t newSize);
	inline void CheckLess(size_t newSize);
	T* elements;
	size_t size;
	int idx;
	bool static_size;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/Stack.inl"

#endif // AK_LIB_STACK_H