#ifndef AK_LIB_FONT_H
#define AK_LIB_FONT_H

#include "AKLib/common.h"
#include "AKLib/Point.h"
#include "AKLib/Vector.h"

#include <ft2build.h>
#include FT_FREETYPE_H

NSB(AKLib)

struct Character
{
	unsigned char* texture;
	Point2D<int> size;
	Point2D<int> bearing;
	unsigned int advance; // advance value is (1/64) pixels scale
};

class AKLIB_API Font
{
public:
	inline Font(const char* path);
	inline Font(unsigned char* buf, size_t bufSize);
	inline ~Font();

	inline void SetScale(Point2D<float> s);
	inline Point2D<float> GetScale();
	inline void SetHeight(unsigned int h);
	inline unsigned int GetHeight();

	inline Character& GetChar(char c);
	inline Point2D<float> GetDim(char c);
	inline Point2D<float> GetPos(char c, float x, float y);
	inline float Advance(char c, float x);

private:
	inline static bool Load(FT_Library& ft,
		Vector<Character>& characters,
		unsigned int height, const char* path);

	inline static bool Load(FT_Library& ft,
		Vector<Character>& characters,
		unsigned int height, unsigned char* buffer, size_t bufferSize);

	inline static void Unload(Vector<Character>& characters);

	inline static bool InitFont(FT_Library& ft);
	inline static bool FreeFont(FT_Library& ft);

	FT_Library ft;
	Vector<Character> characters;
	unsigned int height;
	Point2D<float> scale;
	unsigned char* buffer;
	size_t bufferSize;
};

NSE()

#include "AKLib/Font/Font.inl"

#endif // AK_LIB_FONT_H