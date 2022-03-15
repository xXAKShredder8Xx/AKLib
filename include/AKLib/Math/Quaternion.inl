#include "AKLib/Math/Math.h"
#include <math.h>

NSB(AKLib)

NSB(Math)

template <typename T>
AKL_INLINE Quaternion<T>::Quaternion()
	: x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 }
{
}

template <typename T>
AKL_INLINE Quaternion<T>::Quaternion(Vector3<T> v, T w)
	: x{ v.x }, y{ v.y }, z{ v.z }, w{ w }
{
}

template <typename T>
AKL_INLINE Quaternion<T>::Quaternion(T x, T y, T z, T w) 
	: x{ x }, y{ y }, z{ z }, w{ w }
{
}

template <typename T>
AKL_INLINE Quaternion<T>::~Quaternion() 
{
}

template <typename T>
AKL_INLINE T Quaternion<T>::Length() 
{
	return sqrt(LengthSq());
}

template <typename T>
AKL_INLINE T Quaternion<T>::LengthSq() 
{
	return x * x + y * y + z * z + w * w;
}

template <typename T>
AKL_INLINE Quaternion<T> Quaternion<T>::Normalized() 
{
	T length = Length();
	return Quaternion<T>(x / length, y / length, z / length, w / length);
}

template <typename T>
AKL_INLINE Quaternion<T> Quaternion<T>::Conjugate()
{
	return Quaternion<T>(-x, -y, -z, w);
}

template <typename T>
AKL_INLINE void Quaternion<T>::ToAxisAngle(Vector3<T>& axis, T& angle) 
{
	T _sin = asin(x);  
	
	axis = Vector3<T>(_sin, _sin, _sin);
	angle = _sin;
}

template <typename T>
AKL_INLINE Quaternion<T> Quaternion<T>::Mul(Quaternion<T> other) 
{
	return Quaternion<T>(xyz * other.w + other.xyz * w + xyz.Cross(other.xyz), 
w * other.w + xyz.Dot(other.xyz));
}

template <typename T>
AKL_INLINE Vector3<T> Quaternion<T>::Mul(Vector3<T> V)
{
	Quaternion<T> p(V, 0);

	/*
	could be done as such:

	Quaternion<T>& q = *this;
	return (q * p * q.Conjugate()).xyz;
	*/

	// but this is more optimised:

	Vector3<T> vcV = xyz.Cross(V);
	return V + vcV * (2 * w) + xyz.Cross(vcV) * 2;
}

template <typename T>
AKL_INLINE Quaternion<T> Quaternion<T>::operator * (Quaternion<T> other)
{
	return Mul(other);
}

template <typename T>
AKL_INLINE Vector3<T> Quaternion<T>::operator * (Vector3<T> other) 
{
	return Mul(other);
}

template <typename T>
AKL_INLINE Quaternion<T> Quaternion<T>::operator ^ (T t)
{
}

template <typename T>
AKL_INLINE Quaternion<T> Quaternion<T>::CreateRotationQuaternion(Vector3<T> n, T angle)
{
	Quaternion<T> q;
	
	angle *= DEG_TO_RAD<T>;

	T _sin = sin(angle * 0.5);

	q.w = cos(angle * 0.5);

	q.x = n.x * _sin;
	q.y = n.y * _sin;
	q.z = n.z * _sin;

	return q;
}

template <typename T>
AKL_INLINE Quaternion<T> Quaternion<T>::CreateSlerpQuaternion(Quaternion<T> other, T t) 
{
	Quaternion& q = *this;

	return q * ((other * q.Conjugate()) ^ t);
}

NSE()

NSE()