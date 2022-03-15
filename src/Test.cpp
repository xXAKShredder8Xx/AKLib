/*#include "AKLib/String.h"
#include "AKLib/File.h"
#include "AKLib/ObjLoader.h"
#include "AKLib/AllocatorObject.h"
#include "AKLib/Font/Font.h"
#include "AKLib/Image/Image.h"
#include "AKLib/Vector.h"
#include "AKLib/HashMap.h"
#include "AKLib/Point.h"
#include "AKLib/Gui/Container.h"
#include "AKLib/Gui/Button.h"
#include "AKLib/SharedPtr.h"
#include "AKLib/UniquePtr.h"
#include "AKLib/XML/Xml.h"
#include "AKLib/Stack.h"
#include "AKLib/Font/FontAtlas.h"
#include "AKLib/Math/Vector2.h"
#include "AKLib/Math/Vector3.h"
#include "AKLib/Math/Quaternion.h"*/
#include "AKLib/DLL/DLL_Loader.h"
#include <functional>
#include "AKLib/HashMap.h"

typedef void* (*Create)();
typedef void (*Delete)(void*);
typedef void(*GetFunctionNames)(const char**&, int**&, int*);
typedef void*(*GetFunctions)();
typedef void(*VoidFunc)();

int main()
{
	AKLib::DllLoader loader("TestDLL.dll");
	if (loader.Open()) printf("Success Loading\n");

	GetFunctions getFuncs;
	GetFunctionNames getNames;

	if (loader.GetFunction("GetFunctions", &getFuncs)) printf("Successfully found GetFunctions\n");
	if (loader.GetFunction("GetFunctionNames", &getNames)) printf("Successfully found GetFunctionNames\n");

	int len;
	int** lengths;
	const char** names;
	VoidFunc* functions;

	getNames(names, lengths, &len);
	functions = (VoidFunc*)getFuncs();

	for (int i = 0; i < len; i++) 
	{
		printf("%s : %p\n", names[i], functions[i]);
	}

	loader.Close();
}

/*
#include <stdio.h>
#include "stb_image/stb_image_write.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <math.h>

#define AK_SERIAL_DEFINE_ALL
#include <AKSerial/aks.h>

#define NUM_GLYPHS 128

struct glyph_info {
	int x0, y0, x1, y1;	// coords of glyph in the texture atlas
	int x_off, y_off;   // left & top bearing when rendering
	int advance;        // x advance when rendering
} info[NUM_GLYPHS];

int main(int argc, char** argv)
{
	if (argc < 4) {
		printf("usage: %s <font> <size> <outputName>\n", argv[0]);
		return 1;
	}

	FT_Library ft;
	FT_Face    face;

	FT_Init_FreeType(&ft);
	FT_New_Face(ft, argv[1], 0, &face);
	FT_Set_Char_Size(face, 0, atoi(argv[2]) << 6, 96, 96);

	// quick and dirty max texture size estimate

	int max_dim = (1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(NUM_GLYPHS));
	int tex_width = 1;
	while (tex_width < max_dim) tex_width <<= 1;
	int tex_height = tex_width;

	// render glyphs to atlas

	char* pixels = (char*)calloc(tex_width * tex_height, 1);
	int pen_x = 0, pen_y = 0;

	AKSerial::SerialDocument doc;

	doc.root = new AKSerial::SerialNode("", "", NULL);

	doc.root->AddChild(new AKSerial::SerialNode("root", NULL));

	AKSerial::SerialNode* root = doc.root->GetChild("root");

	AKSerial::SerialNode* _atlas = new AKSerial::SerialNode("atlas",
		AKSerial::String(argv[3]) + ".png", root);

	for (int i = 0; i < NUM_GLYPHS; ++i) {
		FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		FT_Bitmap* bmp = &face->glyph->bitmap;

		if (pen_x + bmp->width >= tex_width) {
			pen_x = 0;
			pen_y += ((face->size->metrics.height >> 6) + 1);
		}

		for (int row = 0; row < bmp->rows; ++row) {
			for (int col = 0; col < bmp->width; ++col) {
				int x = pen_x + col;
				int y = pen_y + row;
				pixels[y * tex_width + x] = bmp->buffer[row * bmp->pitch + col];
			}
		}

		// this is stuff you'd need when rendering individual glyphs out of the atlas

		info[i].x0 = pen_x;
		info[i].y0 = pen_y;
		info[i].x1 = pen_x + bmp->width;
		info[i].y1 = pen_y + bmp->rows;

		info[i].x_off = face->glyph->bitmap_left;
		info[i].y_off = face->glyph->bitmap_top;
		info[i].advance = face->glyph->advance.x >> 6;

		AKSerial::SerialNode* current = new AKSerial::SerialNode(
			AKSerial::String::Format("Character"), root);

		current->AddAttribute("char", AKSerial::String::Format("%d", i));

		AKSerial::SerialTypeNode<int>* _startPosX = new AKSerial::SerialTypeNode<int>(info[i].x0, current);
		AKSerial::SerialTypeNode<int>* _startPosY = new AKSerial::SerialTypeNode<int>(info[i].y0, current);
		_startPosX->AddAttribute("name", "startPosX");
		_startPosY->AddAttribute("name", "startPosY");

		AKSerial::SerialTypeNode<int>* _endPosX = new AKSerial::SerialTypeNode<int>(info[i].x1, current);
		AKSerial::SerialTypeNode<int>* _endPosY = new AKSerial::SerialTypeNode<int>(info[i].y1, current);
		_endPosX->AddAttribute("name", "endPosX");
		_endPosY->AddAttribute("name", "endPosY");

		AKSerial::SerialTypeNode<int>* _offsetX = new AKSerial::SerialTypeNode<int>(info[i].x_off, current);
		AKSerial::SerialTypeNode<int>* _offsetY = new AKSerial::SerialTypeNode<int>(info[i].y_off, current);

		_offsetX->AddAttribute("name", "offsetX");
		_offsetY->AddAttribute("name", "offsetY");

		AKSerial::SerialTypeNode<int>* _advance = new AKSerial::SerialTypeNode<int>(info[i].advance, current);

		_advance->AddAttribute("name", "advance");

		pen_x += bmp->width + 1;
	}

	FT_Done_FreeType(ft);

	// write png

	char* png_data = (char*)calloc(tex_width * tex_height * 4, 1);
	for (int i = 0; i < (tex_width * tex_height); ++i) {
		png_data[i * 4 + 0] |= pixels[i];
		png_data[i * 4 + 1] |= pixels[i];
		png_data[i * 4 + 2] |= pixels[i];
		png_data[i * 4 + 3] = 0xff;
	}

	stbi_write_png(AKSerial::String::Format("%s.png", argv[3]).c_str(), tex_width, tex_height, 4, png_data, tex_width * 4);

	free(png_data);
	free(pixels);

	doc.write(AKSerial::String::Format("%s.xml", argv[3]).c_str());

	return 0;
}

*/