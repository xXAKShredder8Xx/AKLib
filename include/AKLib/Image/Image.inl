#include "AKLib/Allocator.h"
#include "AKLib/File.h"

#include <stb_image/stb_image.h>
#include <stb_image/stb_image_write.h>

NSB(AKLib)

AKL_INLINE Image::Image()
	: data{ NULL }, width{ 0 }, height{ 0 }, numChannels{ 0 }
{
}

AKL_INLINE Image::Image(const char* fileName)
	: data{ NULL }, width{ 0 }, height{ 0 }, numChannels{ 0 }
{
	Read(fileName);
}

AKL_INLINE Image::Image(unsigned char* data, int width, int height, int numChannels)
	: data{ Allocator<unsigned char>::Duplicate(data, (size_t)width * height * numChannels) }, 
	width{ width }, height{ height }, numChannels{ numChannels }
{
}

AKL_INLINE Image::~Image()
{
	Delete();
}

AKL_INLINE bool Image::WriteBmp(const char* fileName)
{
	if (stbi_write_bmp(fileName, width, height, numChannels, data)) 
	{
		return true;
	}

	return false;
}

AKL_INLINE bool Image::WriteJpg(const char* fileName, int quality)
{
	if (stbi_write_jpg(fileName, width, height, numChannels, data, quality))
	{
		return true;
	}

	return false;
}

AKL_INLINE bool Image::WritePng(const char* fileName)
{
	if (stbi_write_png(fileName, width, height, numChannels, data, width * numChannels))
	{
		return true;
	}

	return false;
}

AKL_INLINE bool Image::Read(const char* fileName)
{
	File file(fileName, FileMode::FMODE_READ);
	file.Open();

	if (file.IsOpen()) 
	{
		file.Read();

		file.Rewind();

		Delete();

		data = stbi_load_from_memory((unsigned char*)file.Data(), file.Size(), 
				&width, &height, &numChannels, 0);

		return true;
	}

	return false;
}

AKL_INLINE bool Image::Set(unsigned char* d, int w, int h, int ch)
{
	Delete();

	data = Allocator<unsigned char>::Duplicate(d, (size_t)w * h * ch);

	if (data == NULL) return false;

	width = w;
	height = h;
	numChannels = ch;

	return true;
}

AKL_INLINE unsigned char* Image::Data()
{
	return data;
}

AKL_INLINE int Image::Width()
{
	return width;
}

AKL_INLINE int Image::Height()
{
	return height;
}

AKL_INLINE int Image::NumChannels()
{
	return numChannels;
}

AKL_INLINE size_t Image::Size()
{
	return (size_t)width * height * numChannels;
}

AKL_INLINE void Image::Get(int* w, int* h, int* ch)
{
	*w = width;
	*h = height;
	*ch = numChannels;
}

AKL_INLINE void Image::Resize(int w, int h) 
{
	Delete();

	data = Allocator<unsigned char>::Reallocate(data, (size_t)w * h * numChannels, 
		(size_t)width * height * numChannels);

	width = w;
	height = h;
}

AKL_INLINE bool Image::Add(Point2D<int> startPos, unsigned char* d, int w, int h, int ch) 
{
	if (numChannels != ch) return false;

	size_t s = ((size_t)width + w) * (height + h) * ch;

	data = Allocator<unsigned char>::Reallocate(data, s, (size_t)width * height * ch);

	Allocator<unsigned char>::Copy(d, data + ((size_t)startPos.x * startPos.y * ch), (size_t)w * h * ch);

	width += w;
	height += h;

	return true;
}

AKL_INLINE void Image::Delete()
{
	if (data != NULL)
	{
		data = Allocator<unsigned char>::Deallocate(data);
		width = 0;
		height = 0;
		numChannels = 0;
	}
}

NSE()