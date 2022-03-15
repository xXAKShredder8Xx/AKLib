#include "AKLib/File.h"
#include "AKLib/Allocator.h"
#include "AKLib/Stack.h"

NSB(AKLib)

AKL_INLINE XMLNode* FontAtlasInfo::Serialize()
{
	XMLNode* base = new XMLNode("FontAtlasInfo", NULL);

	XMLNode* _startPos = new XMLNode("startPos", base);
	XMLTypeNode<int>* _startPosX = new XMLTypeNode<int>(startPos.x, _startPos);
	_startPosX->AddAttribute("name", "x");

	XMLTypeNode<int>* _startPosY = new XMLTypeNode<int>(startPos.y, _startPos);
	_startPosY->AddAttribute("name", "y");

	XMLNode* _endPos = new XMLNode("endPos", base);

	XMLTypeNode<int>* _endPosX = new XMLTypeNode<int>(endPos.x, _endPos);
	_endPosX->AddAttribute("name", "x");

	XMLTypeNode<int>* _endPosY = new XMLTypeNode<int>(endPos.y, _endPos);
	_endPosY->AddAttribute("name", "y");

	XMLNode* _offset = new XMLNode("offset", base);

	XMLTypeNode<int>* _offsetX = new XMLTypeNode<int>(offset.x, _offset);
	_offsetX->AddAttribute("name", "x");

	XMLTypeNode<int>* _offsetY = new XMLTypeNode<int>(offset.y, _offset);
	_offsetY->AddAttribute("name", "y");

	XMLNode* _advance = new XMLNode("advance", String::Format("%d", advance), base);

	return base;
}

AKL_INLINE void FontAtlasInfo::Deserialize(XMLNode* node)
{
	if (node->tag == "FontAtlasInfo")
	{
		XMLNode* _startPos = node->GetChild("startPos");

		Vector<XMLNode*> values = _startPos->GetChildren("value");

		startPos.x = values[0]->inner_text.AsInt();
		startPos.y = values[1]->inner_text.AsInt();

		XMLNode* _endPos = node->GetChild("endPos");

		values = _endPos->GetChildren("value");

		endPos.x = values[0]->inner_text.AsInt();
		endPos.y = values[1]->inner_text.AsInt();

		XMLNode* _offset = node->GetChild("offset");

		values = _offset->GetChildren("value");

		offset.x = values[0]->inner_text.AsInt();
		offset.y = values[1]->inner_text.AsInt();

		advance = node->GetChild("advance")->inner_text.AsInt();
	}
}

AKL_INLINE FontAtlas::FontAtlas(const char* fileName)
	: ft{ NULL }, fname{ "" }, buffer{NULL}, bufferSize{0}, height{32}
{
	File file(fileName, FMODE_READ);

	file.Open();

	if (file.IsOpen()) 
	{
		file.Read();
		file.Rewind();

		size_t size = file.Size();

		buffer = Allocator<unsigned char>::Duplicate((unsigned char*)file.Data(), size);
		bufferSize = size;

		file.Close();

		SetHeight(32);
	}
}

AKL_INLINE FontAtlas::FontAtlas(unsigned char* data, size_t size) 
	: ft{ NULL }, fname{ "" }, buffer{ Allocator<unsigned char>::Duplicate(data, size) }, bufferSize{ size }
{
	SetHeight(32);
}

AKL_INLINE FontAtlas::FontAtlas(XMLNode* node) 
{
	Deserialize(node);
}

AKL_INLINE FontAtlas::~FontAtlas() 
{
	buffer = Allocator<unsigned char>::Deallocate(buffer);
	bufferSize = 0;
}

AKL_INLINE void FontAtlas::SetOutputFile(const char* fileName) 
{
	fname = fileName;
}

AKL_INLINE void FontAtlas::SetHeight(int h) 
{
	height = h;
	InitFont(ft);
	Load(ft, h, width, trueHeight, info, image, buffer, bufferSize);
	FreeFont(ft);
}

AKL_INLINE int FontAtlas::GetHeight() 
{
	return height;
}

AKL_INLINE int FontAtlas::GetWidth()
{
	return width;
}

AKL_INLINE int FontAtlas::GetTrueHeight() 
{
	return trueHeight;
}

AKL_INLINE FontAtlasInfo& FontAtlas::GetCharInfo(char c) 
{
	return info[c];
}

AKL_INLINE FontAtlasInfo& FontAtlas::operator [] (char c) 
{
	return info[c];
}

AKL_INLINE unsigned char* FontAtlas::Data() 
{
	return image.Data();
}

AKL_INLINE int FontAtlas::Width() 
{
	return image.Width();
}

AKL_INLINE int FontAtlas::Height() 
{
	return image.Height();
}

AKL_INLINE void FontAtlas::Write() 
{
	image.WritePng(fname.c_str());
}

AKL_INLINE XMLNode* FontAtlas::Serialize() 
{
	XMLNode* base = new XMLNode("FontAtlas", NULL);

	base->AddAttribute("fileName", fname);
	base->AddAttribute("fileWidth", String::Format("%d", image.Width()));
	base->AddAttribute("fileHeight", String::Format("%d", image.Height()));
	base->AddAttribute("fileChannels", String::Format("%d", image.NumChannels()));
	base->AddAttribute("fontHeight", String::Format("%d", height));

	for (int i = 0; i < info.size(); i++) 
	{
		FontAtlasInfo in = info[i];
		base->AddChild(in.Serialize());
	}

	return base;
}

AKL_INLINE void FontAtlas::Deserialize(XMLNode* node) 
{
	if (node->tag == "FontAtlas") 
	{
		fname = node->GetAttribute("fileName")->value;
		int fileWidth = node->GetAttribute("fileWidth")->value.AsInt();
		int fileHeight = node->GetAttribute("fileHeight")->value.AsInt();
		int fileChannels = node->GetAttribute("fileChannels")->value.AsInt();
		height = node->GetAttribute("fontHeight")->value.AsInt();

		image.Read(fname.c_str());

		File file(fname.c_str(), FMODE_READ);

		file.Open();

		if (file.IsOpen()) 
		{
			file.Read();
			file.Rewind();
			size_t size = file.Size();

			buffer = Allocator<unsigned char>::Duplicate((unsigned char*)file.Data(), size);
			bufferSize = size;
		}

		file.Close();

		Vector<XMLNode*> infos = node->GetChildren("FontAtlasInfo");

		info.resize(infos.size());

		for (int i = 0; i < infos.size(); i++) 
		{
			FontAtlasInfo in;
			in.Deserialize(infos[i]);
			info[i] = in;
		}
	}
}

AKL_INLINE void FontAtlas::Load(FT_Library& ft, int height, int& width, int& trueHeight, Vector<FontAtlasInfo>& info, 
	Image& image, unsigned char* buffer, size_t bufferSize) 
{
	FT_Face face;

	FT_New_Memory_Face(ft, buffer, bufferSize, 0, &face);
	FT_Set_Char_Size(face, 0, height << 6, 96, 96);
	//FT_Set_Pixel_Sizes(face, 0, height << 6);

	// quick and dirty max texture size estimate

	int max_dim = (1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(128));
	int tex_width = 1;
	while (tex_width < max_dim) tex_width <<= 1;
	int tex_height = tex_width >> 2;

	width = tex_width;
	trueHeight = tex_height;

	// render glyphs to atlas

	char* pixels = Allocator<char>::Allocate((size_t)tex_width * tex_height, true);
	int pen_x = 0, pen_y = 0;

	info.resize(128);

	for (int i = 0; i < 128; i++) 
	{
		FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		FT_Bitmap* bmp = &face->glyph->bitmap;

		if (pen_x + bmp->width >= tex_width) 
		{
			pen_x = 0;
			pen_y += ((face->size->metrics.height >> 6) + 1);
		}

		for (int row = 0; row < bmp->rows; row++) 
		{
			for (int col = 0; col < bmp->width; col++) 
			{
				int x = pen_x + col;
				int y = pen_y + row;
				pixels[y * tex_width + x] = bmp->buffer[row * bmp->pitch + col];
			}
		}

		// this is stuff you'd need when rendering individual glyphs out of the atlas

		info[i].startPos.x = pen_x;
		info[i].startPos.y = pen_y;
		info[i].endPos.x = pen_x + bmp->width;
		info[i].endPos.y = pen_y + bmp->rows;

		info[i].offset.x = face->glyph->bitmap_left;
		info[i].offset.y = face->glyph->bitmap_top;
		info[i].advance = face->glyph->advance.x >> 6;

		pen_x += bmp->width + 1;
	}

	// write png

	/*char* png_data = Allocator<char>::Allocate((size_t)tex_width * tex_height * 4, false);
	for (int i = 0; i < (tex_width * tex_height); ++i) 
	{
		png_data[i * 4 + 0] = pixels[i];
		png_data[i * 4 + 1] = pixels[i];
		png_data[i * 4 + 2] = pixels[i];
		png_data[i * 4 + 3] = 0xff;
	}

	image.Set((unsigned char*)png_data, tex_width, tex_height, 4);

	free(png_data);*/

	image.Set((unsigned char*)pixels, tex_width, tex_height, 1);

	//free(pixels);
}

AKL_INLINE bool FontAtlas::InitFont(FT_Library& ft) 
{
	if (FT_Init_FreeType(&ft)) 
	{
		return false;
	}

	return true;
}

AKL_INLINE bool FontAtlas::FreeFont(FT_Library& ft)
{
	if (FT_Done_FreeType(ft))
	{
		return false;
	}

	return true;
}

NSE()