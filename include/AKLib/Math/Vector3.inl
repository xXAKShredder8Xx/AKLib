#include <math.h>

NSB(AKLib)

NSB(Math)

template <typename T>
const Vector3<T> Vector3<T>::Up =		{ 0,  1,  0 };

template <typename T>
const Vector3<T> Vector3<T>::Down =		{ 0, -1,  0 };

template <typename T>
const Vector3<T> Vector3<T>::Left =		{ -1,  0,  0 };

template <typename T>
const Vector3<T> Vector3<T>::Right =		{ 1,  0,  0 };

template <typename T>
const Vector3<T> Vector3<T>::Forward =	{ 0,  0,  1 };

template <typename T>
const Vector3<T> Vector3<T>::Back =		{ 0,  0, -1 };

template <typename T>
AKL_INLINE Vector3<T>::Vector3(T x)
	: x{ x }, y{ x }, z{ x }
{
}

template <typename T>
AKL_INLINE Vector3<T>::Vector3(T x, T y, T z)
	: x{ x }, y{ y }, z{ z }
{
}

template <typename T>
AKL_INLINE Vector3<T>::~Vector3()
{
}

template <typename T>
AKL_INLINE T Vector3<T>::Length()
{
	return sqrt(LengthSq());
}

template <typename T>
AKL_INLINE T Vector3<T>::LengthSq()
{
	return x * x + y * y + z * z;
}

template <typename T>
AKL_INLINE Vector3<T> Vector3<T>::Normalized()
{
	T length = Length();
	return Vector3<T>(x / length, y / length, z / length);
}

template <typename T>
AKL_INLINE T Vector3<T>::Dot(Vector3<T> other)
{
	return x * other.x + y * other.y + z * other.z;
}

template <typename T>
AKL_INLINE Vector3<T> Vector3<T>::Cross(Vector3<T> other)
{
	return Vector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

template <typename T>
AKL_INLINE Vector3<T> Vector3<T>::Add(Vector3<T> other)
{
	return Vector3<T>(x + other.x, y + other.y, z + other.z);
}

template <typename T>
AKL_INLINE Vector3<T> Vector3<T>::Sub(Vector3<T> other)
{
	return Vector3<T>(x - other.x, y - other.y, z - other.z);
}

template <typename T>
AKL_INLINE Vector3<T> Vector3<T>::Mul(Vector3<T> other)
{
	return Vector3<T>(x * other.x, y * other.y, z * other.z);
}

template <typename T>
AKL_INLINE Vector3<T> Vector3<T>::Div(Vector3<T> other)
{
	return Vector3<T>(x / other.x, y / other.y, z / other.z);
}

template <typename T>
AKL_INLINE Vector3<T> Vector3<T>::operator + (Vector3<T> other)
{
	return Add(other);
}

template <typename T>
AKL_INLINE Vector3<T> Vector3<T>::operator - (Vector3<T> other)
{
	return Sub(other);
}

template <typename T>
AKL_INLINE Vector3<T> Vector3<T>::operator * (Vector3<T> other)
{
	return Mul(other);
}

template <typename T>
AKL_INLINE Vector3<T> Vector3<T>::operator / (Vector3<T> other)
{
	return Div(other);
}

template <typename T>
AKL_INLINE Vector3<T>& Vector3<T>::operator += (Vector3<T> other)
{
	Vector3<T> res = Add(other);
	x = res.x;
	y = res.y;
	z = res.z;
	return *this;
}

template <typename T>
AKL_INLINE Vector3<T>& Vector3<T>::operator -= (Vector3<T> other)
{
	Vector3<T> res = Sub(other);
	x = res.x;
	y = res.y;
	z = res.z;
	return *this;
}

template <typename T>
AKL_INLINE Vector3<T>& Vector3<T>::operator *= (Vector3<T> other)
{
	Vector3<T> res = Mul(other);
	x = res.x;
	y = res.y;
	z = res.z;
	return *this;
}

template <typename T>
AKL_INLINE Vector3<T>& Vector3<T>::operator /= (Vector3<T> other)
{
	Vector3<T> res = Div(other);
	x = res.x;
	y = res.y;
	z = res.z;
	return *this;
}

NSE()

NSE()