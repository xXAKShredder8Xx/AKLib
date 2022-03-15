#ifndef AK_LIB_QUATERNION_H
#define AK_LIB_QUATERNION_H

#include "AKLib/common.h"
#include "AKLib/Math/Vector3.h"

NSB(AKLib)

NSB(Math)

template <typename T>
struct AKLIB_API Quaternion
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

		struct
		{
			Vector3<T> xyz;
			T w;
		};

		T data[4];
	};

	inline Quaternion();
	inline Quaternion(Vector3<T> v, T w);
	inline Quaternion(T x, T y, T z, T w);
	inline ~Quaternion();

	inline T Length();
	inline T LengthSq();
	inline Quaternion<T> Normalized();
	inline Quaternion<T> Conjugate();
	inline void ToAxisAngle(Vector3<T>& axis, T& angle);

	inline Quaternion<T> Mul(Quaternion<T> other);
	inline Vector3<T> Mul(Vector3<T> other);

	inline Quaternion<T> operator * (Quaternion<T> other);
	inline Vector3<T> operator * (Vector3<T> other);
	inline Quaternion<T> operator ^ (T t);

	inline static Quaternion<T> CreateRotationQuaternion(Vector3<T> n, T angle);
	inline static Quaternion<T> CreateSlerpQuaternion(Quaternion<T> other, T t);
};

NSE()

NSE()

#define AKL_INLINE inline
#include "AKLib/Math/Quaternion.inl"

#endif // AK_LIB_QUATERNION_H