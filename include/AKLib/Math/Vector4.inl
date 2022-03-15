#include <math.h>

NSB(AKLib)

NSB(Math)

template <typename T>
AKL_INLINE Vector4<T>::Vector4(T x)
	: x{ x }, y{ x }, z{ x }, w{ x}
{
}

template <typename T>
AKL_INLINE Vector4<T>::Vector4(T x, T y, T z, T w)
	: x{ x }, y{ y }, z{ z }, w{ w }
{
}

template <typename T>
AKL_INLINE Vector4<T>::~Vector4()
{
}

template <typename T>
AKL_INLINE T Vector4<T>::Length()
{
	return sqrt(LengthSq());
}

template <typename T>
AKL_INLINE T Vector4<T>::LengthSq()
{
	return x * x + y * y + z * z + w * w;
}

template <typename T>
AKL_INLINE Vector4<T> Vector4<T>::Normalized()
{
	T length = Length();
	return Vector4<T>(x / length, y / length, z / length, w / length);
}

template <typename T>
AKL_INLINE T Vector4<T>::Dot(Vector4<T> other)
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

template <typename T>
AKL_INLINE Vector4<T> Vector4<T>::Add(Vector4<T> other)
{
	return Vector4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
}

template <typename T>
AKL_INLINE Vector4<T> Vector4<T>::Sub(Vector4<T> other)
{
	return Vector4<T>(x - other.x, y - other.y, z - other.z, w - other.w);
}

template <typename T>
AKL_INLINE Vector4<T> Vector4<T>::Mul(Vector4<T> other)
{
	return Vector4<T>(x * other.x, y * other.y, z * other.z, w * other.w);
}

template <typename T>
AKL_INLINE Vector4<T> Vector4<T>::Div(Vector4<T> other)
{
	return Vector4<T>(x / other.x, y / other.y, z / other.z, w / other.w);
}

template <typename T>
AKL_INLINE Vector4<T> Vector4<T>::operator + (Vector4<T> other)
{
	return Add(other);
}

template <typename T>
AKL_INLINE Vector4<T> Vector4<T>::operator - (Vector4<T> other)
{
	return Sub(other);
}

template <typename T>
AKL_INLINE Vector4<T> Vector4<T>::operator * (Vector4<T> other)
{
	return Mul(other);
}

template <typename T>
AKL_INLINE Vector4<T> Vector4<T>::operator / (Vector4<T> other)
{
	return Div(other);
}

template <typename T>
AKL_INLINE Vector4<T>& Vector4<T>::operator += (Vector4<T> other)
{
	Vector4<T> res = Add(other);
	x = res.x;
	y = res.y;
	z = res.z;
	w = res.w;
	return *this;
}

template <typename T>
AKL_INLINE Vector4<T>& Vector4<T>::operator -= (Vector4<T> other)
{
	Vector4<T> res = Sub(other);
	x = res.x;
	y = res.y;
	z = res.z;
	w = res.w;
	return *this;
}

template <typename T>
AKL_INLINE Vector4<T>& Vector4<T>::operator *= (Vector4<T> other)
{
	Vector4<T> res = Mul(other);
	x = res.x;
	y = res.y;
	z = res.z;
	w = res.w;
	return *this;
}

template <typename T>
AKL_INLINE Vector4<T>& Vector4<T>::operator /= (Vector4<T> other)
{
	Vector4<T> res = Div(other);
	x = res.x;
	y = res.y;
	z = res.z;
	w = res.w;
	return *this;
}

NSE()

NSE()