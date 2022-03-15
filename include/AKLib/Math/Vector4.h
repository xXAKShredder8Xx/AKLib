#ifndef AK_LIB_VECTOR4_H
#define AK_LIB_VECTOR4_H

#include "AKLib/common.h"
#include "AKLib/Math/Vector2.h"
#include "AKLib/Math/Vector3.h"

NSB(AKLib)

NSB(Math)

template <typename T>
struct AKLIB_API Vector4
{
	union
	{
		struct
		{
			T x;
			T y;
			T z;
			T w;
		};

		Vector2<T> xy;

		Vector3<T> xyz;

		T data[4];
	};

	inline Vector4(T x = 0);
	inline Vector4(T x, T y, T z, T w);
	inline ~Vector4();

	inline T Length();
	inline T LengthSq();
	inline Vector4<T> Normalized();
	inline T Dot(Vector4<T> other);

	inline Vector4<T> Add(Vector4<T> other);
	inline Vector4<T> Sub(Vector4<T> other);
	inline Vector4<T> Mul(Vector4<T> other);
	inline Vector4<T> Div(Vector4<T> other);

	inline Vector4<T> operator + (Vector4<T> other);
	inline Vector4<T> operator - (Vector4<T> other);
	inline Vector4<T> operator * (Vector4<T> other);
	inline Vector4<T> operator / (Vector4<T> other);

	inline Vector4<T>& operator += (Vector4<T> other);
	inline Vector4<T>& operator -= (Vector4<T> other);
	inline Vector4<T>& operator *= (Vector4<T> other);
	inline Vector4<T>& operator /= (Vector4<T> other);
};

NSE()

NSE()

#define AKL_INLINE inline
#include "AKLib/Math/Vector4.inl"

#endif // AK_LIB_VECTOR4_H