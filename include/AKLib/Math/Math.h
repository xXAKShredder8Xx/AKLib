#ifndef AK_LIB_MATH_H
#define AK_LIB_MATH_H

#include "AKLib/common.h"

NSB(AKLib)

NSB(Math)

template <typename T>
static const T PI = 3.141592653;

template <typename T>
static const T RAD_TO_DEG = ((T)180 / PI<T>);

template <typename T>
static const T DEG_TO_RAD = (PI<T> / (T)180);

template <typename T>
inline AKLIB_API T Interpolate(T goal, T current, T dt);

NSE()

NSE()

#define AKL_INLINE inline
#include "AKLib/Math/Math.inl"

#endif // AK_LIB_MATH_H