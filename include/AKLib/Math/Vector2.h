#ifndef AK_LIB_VECTOR2_H
#define AK_LIB_VECTOR2_H

#include "AKLib/common.h"

NSB(AKLib)

NSB(Math)

template <typename T>
struct AKLIB_API Vector2
{
	union 
	{
		struct 
		{
			T x;
			T y;
		};

		T data[2];
	};

	static const Vector2<T> Up;
	static const Vector2<T> Down;
	static const Vector2<T> Left;
	static const Vector2<T> Right;

	inline Vector2(T x = 0);
	inline Vector2(T x, T y);
	inline ~Vector2();

	inline T Length();
	inline T LengthSq();
	inline Vector2<T> Normalized();
	inline T Dot(Vector2<T> other);

	inline Vector2<T> Add(Vector2<T> other);
	inline Vector2<T> Sub(Vector2<T> other);
	inline Vector2<T> Mul(Vector2<T> other);
	inline Vector2<T> Div(Vector2<T> other);

	inline Vector2<T> operator + (Vector2<T> other);
	inline Vector2<T> operator - (Vector2<T> other);
	inline Vector2<T> operator * (Vector2<T> other);
	inline Vector2<T> operator / (Vector2<T> other);

	inline Vector2<T>& operator += (Vector2<T> other);
	inline Vector2<T>& operator -= (Vector2<T> other);
	inline Vector2<T>& operator *= (Vector2<T> other);
	inline Vector2<T>& operator /= (Vector2<T> other);
};

NSE()

NSE()

#define AKL_INLINE inline
#include "AKLib/Math/Vector2.inl"

#endif // AK_LIB_VECTOR2_H