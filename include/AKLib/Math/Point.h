#ifndef AK_LIB_POINT_H
#define AK_LIB_POINT_H

#include "AKLib/common.h"

NSB(AKLib)

template <typename T>
struct Point2D 
{
	union 
	{
		struct 
		{
			T x;
			T y;
		};

		struct 
		{
			T s;
			T t;
		};

		struct 
		{
			T w; // width
			T h; // height
		};

		T arr[2];
	};
};

template <typename T>
struct Point3D
{
	union
	{
		struct
		{
			T x;
			T y;
			T z;
		};

		struct
		{
			T s;
			T t;
			T r;
		};

		struct 
		{
			T w; // width
			T h; // height
			T d; // depth
		};

		struct 
		{
			T r;
			T g;
			T b;
		};

		T arr[3];
	};
};

NSE()

#endif // AK_LIB_POINT_H