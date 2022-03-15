#include <math.h>

NSB(AKLib)

NSB(Math)

template <typename T>
const Vector2<T> Vector2<T>::Up = { 0,  1 };

template <typename T>
const Vector2<T> Vector2<T>::Down = { 0, -1 };

template <typename T>
const Vector2<T> Vector2<T>::Left = { -1,  1 };

template <typename T>
const Vector2<T> Vector2<T>::Right = { 1,  1 };

template <typename T>
AKL_INLINE Vector2<T>::Vector2(T x)
	: x{ x }, y{ x }
{
}

template <typename T>
AKL_INLINE Vector2<T>::Vector2(T x, T y)
	: x{ x }, y{ y }
{
}

template <typename T>
AKL_INLINE Vector2<T>::~Vector2() 
{
}

template <typename T>
AKL_INLINE T Vector2<T>::Length() 
{
	return sqrt(LengthSq());
}

template <typename T>
AKL_INLINE T Vector2<T>::LengthSq() 
{
	return x * x + y * y;
}

template <typename T>
AKL_INLINE Vector2<T> Vector2<T>::Normalized()
{
	T length = Length();
	return Vector2<T>(x / length, y / length);
}

template <typename T>
AKL_INLINE T Vector2<T>::Dot(Vector2<T> other)
{
	return x * other.x + y * other.y;
}

template <typename T>
AKL_INLINE Vector2<T> Vector2<T>::Add(Vector2<T> other) 
{
	return Vector2<T>(x + other.x, y + other.y);
}

template <typename T>
AKL_INLINE Vector2<T> Vector2<T>::Sub(Vector2<T> other)
{
	return Vector2<T>(x - other.x, y - other.y);
}

template <typename T>
AKL_INLINE Vector2<T> Vector2<T>::Mul(Vector2<T> other)
{
	return Vector2<T>(x * other.x, y * other.y);
}

template <typename T>
AKL_INLINE Vector2<T> Vector2<T>::Div(Vector2<T> other)
{
	return Vector2<T>(x / other.x, y / other.y);
}

template <typename T>
AKL_INLINE Vector2<T> Vector2<T>::operator + (Vector2<T> other)
{
	return Add(other);
}

template <typename T>
AKL_INLINE Vector2<T> Vector2<T>::operator - (Vector2<T> other)
{
	return Sub(other);
}

template <typename T>
AKL_INLINE Vector2<T> Vector2<T>::operator * (Vector2<T> other)
{
	return Mul(other);
}

template <typename T>
AKL_INLINE Vector2<T> Vector2<T>::operator / (Vector2<T> other)
{
	return Div(other);
}

template <typename T>
AKL_INLINE Vector2<T>& Vector2<T>::operator += (Vector2<T> other)
{
	Vector2<T> res = Add(other);
	x = res.x;
	y = res.y;
	return *this;
}

template <typename T>
AKL_INLINE Vector2<T>& Vector2<T>::operator -= (Vector2<T> other)
{
	Vector2<T> res = Sub(other);
	x = res.x;
	y = res.y;
	return *this;
}

template <typename T>
AKL_INLINE Vector2<T>& Vector2<T>::operator *= (Vector2<T> other)
{
	Vector2<T> res = Mul(other);
	x = res.x;
	y = res.y;
	return *this;
}

template <typename T>
AKL_INLINE Vector2<T>& Vector2<T>::operator /= (Vector2<T> other)
{
	Vector2<T> res = Div(other);
	x = res.x;
	y = res.y;
	return *this;
}

NSE()

NSE()