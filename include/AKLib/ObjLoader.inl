#include "AKLib/Allocator.h"
#include "AKLib/Vector.h"
#include "AKLib/File.h"

NSB(AKLib)

AKL_INLINE bool ObjLoader::LoadFile(const char* fileName, Vector<float>& vertices,
	Vector<float>& texCoords, Vector<float>& normals,
	Vector<ObjIndex>& indices)
{
	File file(fileName, FileMode::FMODE_READ);
	
	if (!file.Open())
		return false;

	file.Read();

	String line;
	String temp;

	while (file.ReadLine(line))
	{
		Vector<String> parts = line.Split(" ");

		if (parts.size() > 0)
		{
			if (line.StartsWith("v")) // Vertex pos
			{
				vertices.push_back(parts[1].AsFloat());
				vertices.push_back(parts[2].AsFloat());
				vertices.push_back(parts[3].AsFloat());
			}
			else if (line.StartsWith("vt"))
			{
				texCoords.push_back(parts[1].AsFloat());
				texCoords.push_back(parts[2].AsFloat());
			}
			else if (line.StartsWith("vn"))
			{
				normals.push_back(parts[1].AsFloat());
				normals.push_back(parts[2].AsFloat());
				normals.push_back(parts[3].AsFloat());
			}
			else if (line.StartsWith("f"))
			{
				break;
			}
		}

		while (line != (char*)NULL) 
		{
			if (!line.StartsWith("f"))
			{
				file.ReadLine(line);
				continue;
			}

			Vector<String> currentLine = line.Split(" ");
			
			for (int i = 1; i < currentLine.size(); i++) 
			{
				Vector<String> vertex = currentLine[i].Split("/");

				ObjIndex index;
				index.vertexIndex = vertex[0].AsInt();
				index.textureIndex = vertex[1].AsInt();
				index.normalIndex = vertex[2].AsInt();

				indices.push_back(index);
			}

			file.ReadLine(line);
		}
	}

	return true;
}

NSE()