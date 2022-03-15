#ifndef AK_LIB_OBJ_LOADER_H
#define AK_LIB_OBJ_LOADER_H

#include "AKLib/common.h"
#include "AKLib/Vector.h"
#include "AKLib/Point.h"

NSB(AKLib)

struct ObjIndex 
{
	unsigned int vertexIndex;
	unsigned int textureIndex;
	unsigned int normalIndex;
};

class AKLIB_API ObjLoader
{
public:
	inline static bool LoadFile(const char* fileName, Vector<float>& vertices, 
		Vector<float>& texCoords, Vector<float>& normals,
		Vector<ObjIndex>& indices);
};

NSE()

#define AKL_INLINE inline
#include "AKLib/ObjLoader.inl"

#endif // AK_LIB_OBJ_LOADER_H