#include "AKLib/Allocator.h"
#include "AKLib/File.h"

NSB(AKLib)

AKL_INLINE Font::Font(const char* path)
	: ft{ NULL }, characters{ }, height{ 32 }, scale{ 1.0f, 1.0f }, buffer{ NULL }, bufferSize{ 0 }
{
	File file(path, FileMode::FMODE_READ);
	file.Open();
	file.Read();
	
	file.Rewind();
	size_t size = file.Size();

	buffer = Allocator<unsigned char>::Duplicate((unsigned char*)file.Data(), size);
	bufferSize = size;

	file.Close();

	SetHeight(32);
}

AKL_INLINE Font::Font(unsigned char* buf, size_t bufSize)
	: ft{ NULL }, characters{ }, height{ 32 }, scale{ 1.0f, 1.0f }, 
	buffer{ Allocator<unsigned char>::Duplicate(buf, bufSize) }, bufferSize{ bufSize }
{
	SetHeight(32);
}

AKL_INLINE Font::~Font()
{
	Unload(characters);
	buffer = Allocator<unsigned char>::Deallocate(buffer);
	bufferSize = 0;
}

AKL_INLINE void Font::SetScale(Point2D<float> s)
{
	scale = s;
}

AKL_INLINE Point2D<float> Font::GetScale()
{
	return scale;
}

AKL_INLINE void Font::SetHeight(unsigned int h)
{
	height = h;

	InitFont(ft);
	Unload(characters);
	Load(ft, characters, h, buffer, bufferSize);
	FreeFont(ft);
}

AKL_INLINE unsigned int Font::GetHeight()
{
	return height;
}

AKL_INLINE Character& Font::GetChar(char c)
{
	return characters[c];
}

AKL_INLINE Point2D<float> Font::GetDim(char c)
{
	Character& ch = characters[c];

	Point2D<float> dim;

	dim.w = ch.size.w * scale.x;
	dim.h = ch.size.h * scale.y;

	return dim;
}

AKL_INLINE Point2D<float> Font::GetPos(char c, float x, float y)
{
	Character& ch = characters[c];

	Point2D<float> pos;
	pos.x = x + ch.bearing.x * scale.x;
	pos.y = y - (ch.size.h - ch.bearing.y) * scale.y; // characters might need to shift below baseline

	return pos;
}

AKL_INLINE float Font::Advance(char c, float x)
{
	return x + (characters[c].advance << 6) * scale.w;
}

AKL_INLINE bool Font::Load(FT_Library& ft,
	Vector<Character>& characters,
	unsigned int height, const char* path) 
{
	FT_Face face{};

	if (FT_New_Face(ft, path, 0, &face))
	{
		return false;
	}

	// set height & width
	FT_Set_Pixel_Sizes(face, 0, height); // 0 for width to allow dynamic resizing

	characters.resize(0);

	for (unsigned char c = 0; c < 128; c++)
	{
		// load the glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			continue;
		}

		// generate texture
		Character character;
		character.size.w = face->glyph->bitmap.width;
		character.size.h = face->glyph->bitmap.rows;
		character.texture = Allocator<unsigned char>::Duplicate(face->glyph->bitmap.buffer,
			(size_t)face->glyph->bitmap.width * face->glyph->bitmap.rows);

		character.bearing.x = face->glyph->bitmap_left;
		character.bearing.y = face->glyph->bitmap_top;
		character.advance = (unsigned int)face->glyph->advance.x;

		characters[c] = character;
	}

	FT_Done_Face(face);

	return true;
}

AKL_INLINE bool Font::Load(FT_Library& ft,
	Vector<Character>& characters,
	unsigned int height, unsigned char* buffer, size_t bufferSize)
{
	FT_Face face{};
	
	if (FT_New_Memory_Face(ft, buffer, bufferSize, 0, &face))
	{
		return false;
	}

	// set height & width
	FT_Set_Pixel_Sizes(face, 0, height); // 0 for width to allow dynamic resizing

	characters.resize(128);

	for (unsigned char c = 0; c < 128; c++)
	{
		// load the glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			continue;
		}

		// generate texture
		Character character;
		character.size.w = face->glyph->bitmap.width;
		character.size.h = face->glyph->bitmap.rows;
		character.texture = Allocator<unsigned char>::Duplicate(face->glyph->bitmap.buffer,
			(size_t)face->glyph->bitmap.width * face->glyph->bitmap.rows);

		character.bearing.x = face->glyph->bitmap_left;
		character.bearing.y = face->glyph->bitmap_top;
		character.advance = face->glyph->advance.x;

		characters[c] = character;
	}

	FT_Done_Face(face);

	return true;
}

AKL_INLINE void Font::Unload(Vector<Character>& characters)
{
	if (characters.size() > 0) 
	{
		for (Character& c : characters) 
		{
			c.texture = Allocator<unsigned char>::Deallocate(c.texture);
			c.advance = 0;
			c.bearing = { 0, 0 };
			c.size = { 0, 0 };
		}

		characters.resize(0);
	}
}

AKL_INLINE bool Font::InitFont(FT_Library& ft)
{
	if (FT_Init_FreeType(&ft))
	{
		return false;
	}

	return true;
}

AKL_INLINE bool Font::FreeFont(FT_Library& ft)
{
	if (FT_Done_FreeType(ft))
	{
		return false;
	}

	return true;
}

NSE()