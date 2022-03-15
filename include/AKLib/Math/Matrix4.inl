#include "AKLib/Math/Math.h"

NSB(AKLib)

NSB(Math)

template <typename T>
AKL_INLINE Mat4<T>::Mat4() 
	: data{ 0 }
{
}

template <typename T>
AKL_INLINE Mat4<T>::Mat4(Vector3<T> forward, Vector3<T> up, Vector3<T> right) 
	: data{ 0 }
{
	data[4 * 0 + 0] = forward.x;
	data[4 * 1 + 0] = forward.y;
	data[4 * 2 + 0] = forward.z;

	data[4 * 0 + 1] = up.x;
	data[4 * 1 + 1] = up.y;
	data[4 * 2 + 1] = up.z;

	data[4 * 0 + 2] = right.x;
	data[4 * 1 + 2] = right.y;
	data[4 * 2 + 2] = right.z;

	data[4 * 3 + 3] = 1;
}

template <typename T>
AKL_INLINE Mat4<T>::~Mat4() 
{
}

template <typename T>
AKL_INLINE Mat4<T> Mat4<T>::CreateIdentityMatrix() 
{
	Mat4<T> mat;

	mat.data[0][0] = 1;
	mat.data[0][1] = 0;
	mat.data[0][2] = 0;
	mat.data[0][3] = 0;

	mat.data[1][0] = 0;
	mat.data[1][1] = 1;
	mat.data[1][2] = 0;
	mat.data[1][3] = 0;

	mat.data[2][0] = 0;
	mat.data[2][1] = 0;
	mat.data[2][2] = 1;
	mat.data[2][3] = 0;

	mat.data[3][0] = 0;
	mat.data[3][1] = 0;
	mat.data[3][2] = 0;
	mat.data[3][3] = 1;

	return mat;
}

template <typename T>
AKL_INLINE Mat4<T> Mat4<T>::CreateTranslationMatrix(Vector3<T> translation)
{
	Mat4<T> mat = CreateIdentityMatrix();

	mat.data[3][0] = translation.x;
	mat.data[3][1] = translation.y;
	mat.data[3][2] = translation.z;

	return mat;
}

template <typename T>
AKL_INLINE Mat4<T> Mat4<T>::CreateScaleMatrix(Vector3<T> scale)
{
	Mat4<T> mat = CreateIdentityMatrix();

	mat.data[0][0] = scale.x;
	mat.data[1][1] = scale.y;
	mat.data[2][2] = scale.z;

	return mat;
}

template <typename T>
AKL_INLINE Mat4<T> Mat4<T>::CreateRotationMatrix(T angle, Vector3<T> axis)
{
	Mat4<T> mat = CreateIdentityMatrix();

	angle *= DEG_TO_RAD<T>;

	T _cos = cos(angle);
	T _sin = sin(angle);
	T icos = (T)1 - _cos;

	T& x = axis.x;
	T& y = axis.y;
	T& z = axis.z;
	T x2 = x * x;
	T y2 = y * y;
	T z2 = z * z;

	mat.data[0][0]= _cos + x2 * icos;
	mat.data[1][0] = x * y * icos - z * _sin;
	mat.data[2][0] = x * z * icos + y * _sin;

	mat.data[0][1] = y * x * icos + z * _sin;
	mat.data[1][1] = _cos + y2 * icos;
	mat.data[2][1] = y * z * icos - x * _sin;

	mat.data[0][2] = z * x * icos - y * _sin;
	mat.data[1][2] = z * y * icos + x * _sin;
	mat.data[2][2] = _cos + z2 * icos;
		
	return mat;
}

template <typename T>
AKL_INLINE Mat4<T> Mat4<T>::CreateRotationMatrix(Vector3<T> angles)
{
	Mat4<T> rotX = CreateRotationMatrix(angles.x, { 1, 0, 0 });
	Mat4<T> rotY = CreateRotationMatrix(angles.y, { 0, 1, 0 });
	Mat4<T> rotZ = CreateRotationMatrix(angles.z, { 0, 0, 1 });

	return rotZ * rotY * rotX;
}

template <typename T>
AKL_INLINE Mat4<T> Mat4<T>::CreatePerspectiveMatrix(T fov, T left, T right, T bottom, T top, T near, T far) 
{
	Mat4<T> mat = CreateIdentityMatrix();

	T tanfov = tan((fov * 0.5) * DEG_TO_RAD<T>);
	T aspect = (right - left) / (top - bottom);

	mat.data[0][0] = (T)1 / (aspect * tanfov);
	mat.data[1][1] = (T)1 / tanfov;
	mat.data[2][2] = -((far + near) / (far - near));
	mat.data[3][2] = -((2 * far * near) / (far - near));
	mat.data[2][3] = -1;

	return mat;
}

template <typename T>
AKL_INLINE Mat4<T> Mat4<T>::CreatePerspectiveMatrix(T fov, T aspect, T near, T far)
{
	Mat4<T> mat = CreateIdentityMatrix();

	T tanfov = tan((fov * 0.5) * DEG_TO_RAD<T>);

	mat.data[0][0] = (T)1 / (aspect * tanfov);
	mat.data[1][1] = (T)1 / tanfov;
	mat.data[2][2] = -((far + near) / (far - near));
	mat.data[3][2] = -((2 * far * near) / (far - near));
	mat.data[2][3] = -1;

	return mat;
}

template <typename T>
AKL_INLINE Mat4<T> Mat4<T>::CreateOrthographicMatrix(T left, T right, T bottom, T top, T far, T near) 
{
	Mat4<T> mat = CreateIdentityMatrix();

	mat.data[0][0] = (T)2 / (right - left);
	mat.data[1][1] = (T)2 / (top - bottom);
	mat.data[2][2] = (T)-2 / (far - near);
	mat.data[3][0] = -((right + left) / (right - left));
	mat.data[3][1] = -((top + bottom) / (top - bottom));
	mat.data[3][2] = -((far + near) / (far - near));

	return mat;
}

template <typename T>
AKL_INLINE Mat4<T> Mat4<T>::CreateTransformationMatrix(Vector3<T> translation, Vector3<T> rotation, Vector3<T> scale) 
{
	Mat4<T> rot = CreateRotationMatrix(rotation);
	Mat4<T> sc = CreateScaleMatrix(scale);
	Mat4<T> tr = CreateTranslationMatrix(translation);

	return tr * rot * sc;
}

template <typename T>
AKL_INLINE T* Mat4<T>::operator [] (int idx) 
{
	return data[idx];
}

template <typename T>
AKL_INLINE Vector4<T> Mat4<T>::operator * (Vector4<T> other)
{
	Vector4<T> vec;

	for (int i = 0; i < 4; i++) 
	{
		vec.data[i] = data[i][0] * other.x + data[i][1] * other.y + data[i][2] * 
			other.z + data[i][3] * other.w;
	}

	return vec;
}

template <typename T>
AKL_INLINE Mat4<T> Mat4<T>::operator * (Mat4<T> other)
{
	Mat4<T> mat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) 
		{
			mat.data[i][j] = (data[i][0] * other.data[0][j] +
							  data[i][1] * other.data[1][j] +
							  data[i][2] * other.data[2][j] +
							  data[i][3] * other.data[3][j]);
		}
	}

	return mat;
}

NSE()

NSE()