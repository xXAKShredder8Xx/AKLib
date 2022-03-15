#ifndef AK_LIB_IMAGE_H
#define AK_LIB_IMAGE_H

#include "AKLib/common.h"
#include "AKLib/Math/Point.h"

NSB(AKLib)

class AKLIB_API Image
{
public:
	inline Image();
	inline Image(const char* fileName);
	inline Image(unsigned char* data, int width, int height, int numChannels);
	inline ~Image();

	inline bool WriteBmp(const char* fileName);
	inline bool WriteJpg(const char* fileName, int quality);
	inline bool WritePng(const char* fileName);
	inline bool Read(const char* fileName);
	inline bool Set(unsigned char* d, int w, int h, int ch);

	inline unsigned char* Data();
	inline int Width();
	inline int Height();
	inline int NumChannels();
	inline size_t Size();
	inline void Get(int* w, int* h, int* ch);
	inline void Resize(int w, int h);
	inline bool Add(Point2D<int> startPos, unsigned char* d, int w, int h, int ch);
	void Delete();
private:
	unsigned char* data;
	int width;
	int height;
	int numChannels;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/Image/Image.inl"

#endif // AK_LIB_IMAGE_H