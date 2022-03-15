#ifndef AK_LIB_FONT_ATLAS_H
#define AK_LIB_FONT_ATLAS_H

#include "AKLib/common.h"
#include "AKLib/Point.h"
#include "AKLib/XML/Xml.h"
#include "AKLib/Image/Image.h"
#include "AKLib/Vector.h"

#include <ft2build.h>
#include FT_FREETYPE_H

NSB(AKLib)

struct AKLIB_API FontAtlasInfo : public XMLSerial
{
	Point2D<int> startPos;
	Point2D<int> endPos;
	Point2D<int> offset;
	int advance;

	inline virtual XMLNode* Serialize() override;
	inline virtual void Deserialize(XMLNode* node) override;
};

class AKLIB_API FontAtlas : public XMLSerial
{
public:
	inline FontAtlas(const char* fileName);
	inline FontAtlas(unsigned char* data, size_t size);
	inline FontAtlas(XMLNode* node);
	inline ~FontAtlas();

	inline void SetOutputFile(const char* fileName);

	inline void SetHeight(int h);
	inline int GetHeight();
	inline int GetWidth();
	inline int GetTrueHeight();

	inline FontAtlasInfo& GetCharInfo(char c);

	inline FontAtlasInfo& operator [] (char c);

	inline unsigned char* Data();
	inline int Width();
	inline int Height();

	inline void Write();

	inline virtual XMLNode* Serialize() override;
	inline virtual void Deserialize(XMLNode* node) override;
private:
	inline static void Load(FT_Library& ft, int height, int& width, int& trueHeight, Vector<FontAtlasInfo>& info, 
		Image& image, unsigned char* buffer, size_t bufferSize);

	inline static bool InitFont(FT_Library& ft);
	inline static bool FreeFont(FT_Library& ft);

	FT_Library ft;
	Vector<FontAtlasInfo> info;
	Image image;
	int height;
	int width;
	int trueHeight;
	unsigned char* buffer;
	size_t bufferSize;
	String fname;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/Font/FontAtlas.inl"

#endif // AK_LIB_FONT_ATLAS_H