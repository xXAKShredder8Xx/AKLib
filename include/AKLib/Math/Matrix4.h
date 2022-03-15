#ifndef AK_LIB_MATRIX4_H
#define AK_LIB_MATRIX4_H

#include "AKLib/common.h"
#include "AKLib/Math/Vector4.h"

NSB(AKLib)

NSB(Math)

template <typename T>
struct AKLIB_API Mat4
{
	union 
	{
		struct 
		{
			T	m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33;
		};

		T data[4][4];
	};

	inline Mat4();
	inline Mat4(Vector3<T> forward, Vector3<T> up, Vector3<T> right);
	inline ~Mat4();

	inline static Mat4<T> CreateIdentityMatrix();
	inline static Mat4<T> CreateTranslationMatrix(Vector3<T> translation);
	inline static Mat4<T> CreateScaleMatrix(Vector3<T> scale);
	inline static Mat4<T> CreateRotationMatrix(T angle, Vector3<T> axis);
	inline static Mat4<T> CreateRotationMatrix(Vector3<T> angles);
	inline static Mat4<T> CreatePerspectiveMatrix(T fov, T left, T right, T bottom, T top, T near, T far);
	inline static Mat4<T> CreatePerspectiveMatrix(T fov, T aspect, T near, T far);
	inline static Mat4<T> CreateOrthographicMatrix(T left, T right, T bottom, T top, T far, T near);
	inline static Mat4<T> CreateTransformationMatrix(Vector3<T> translation, Vector3<T> rotation, Vector3<T> scale);

	inline T* operator [] (int idx);

	inline Vector4<T> operator * (Vector4<T> vec);
	inline Mat4<T> operator * (Mat4<T> other);
};

NSE()

NSE()

#define AKL_INLINE inline
#include "AKLib/Math/Matrix4.inl"

#endif // AK_LIB_MATRIX4_H