#ifndef AK_LIB_VECTOR3_H
#define AK_LIB_VECTOR3_H

#include "AKLib/common.h"
#include "AKLib/Math/Vector2.h"

NSB(AKLib)

NSB(Math)

template <typename T>
struct AKLIB_API Vector3
{
	union
	{
		struct
		{
			T x;
			T y;
			T z;
		};

		Vector2<T> xy;

		T data[3];
	};

	static const Vector3<T> Up;
	static const Vector3<T> Down;
	static const Vector3<T> Left;
	static const Vector3<T> Right;
	static const Vector3<T> Forward;
	static const Vector3<T> Back;

	inline Vector3(T x = 0);
	inline Vector3(T x, T y, T z);
	inline ~Vector3();

	inline T Length();
	inline T LengthSq();
	inline Vector3<T> Normalized();
	inline T Dot(Vector3<T> other);
	inline Vector3<T> Cross(Vector3<T> other);

	inline Vector3<T> Add(Vector3<T> other);
	inline Vector3<T> Sub(Vector3<T> other);
	inline Vector3<T> Mul(Vector3<T> other);
	inline Vector3<T> Div(Vector3<T> other);

	inline Vector3<T> operator + (Vector3<T> other);
	inline Vector3<T> operator - (Vector3<T> other);
	inline Vector3<T> operator * (Vector3<T> other);
	inline Vector3<T> operator / (Vector3<T> other);

	inline Vector3<T>& operator += (Vector3<T> other);
	inline Vector3<T>& operator -= (Vector3<T> other);
	inline Vector3<T>& operator *= (Vector3<T> other);
	inline Vector3<T>& operator /= (Vector3<T> other);
};

NSE()

NSE()

#define AKL_INLINE inline
#include "AKLib/Math/Vector3.inl"

#endif // AK_LIB_VECTOR3_H