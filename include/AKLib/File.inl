#include "AKLib/Allocator.h"
#include "AKLib/Vector.h"
#include <stdio.h>

NSB(AKLib)

AKL_INLINE File::File(const char* name, FileMode m)
	: file{ NULL}, location{ 0 }, currentLocation{ 0 }, fname{ NULL }
{
	SetFile(name, m);
}

AKL_INLINE File::~File()
{
	Close();
}

AKL_INLINE bool File::SetFile(const char* name, FileMode m)
{
	Close();
	fname = name;
	mode = m;
	file = NULL;

	return true;
}

AKL_INLINE bool File::SetMode(FileMode m)
{
	Close();
	SetFile(fname, m);
	return Open();
}

AKL_INLINE bool File::Open()
{
	if (file != NULL)
		return false;

	return (fopen_s(&file, fname, GetModeString(mode))) == 0;
}

AKL_INLINE bool File::Close()
{
	if (file == NULL)
		return false;

	if (fclose(file) != 0)
		return false;

	file = NULL;
	buffer.resize(0);

	return true;
}

AKL_INLINE bool File::GoTo(int loc)
{
	if (file == NULL)
		return false;

	if (fseek(file, loc, SEEK_SET) == 0) 
	{
		location = loc;
		currentLocation = loc;
		return true;
	}

	return false;
}

AKL_INLINE bool File::Rewind()
{
	if (file == NULL)
		return false;

	return GoTo(0);
}

AKL_INLINE char File::ReadChar()
{
	if (file != NULL)
	{
		return fgetc(file);
	}

	return '\0';
}

AKL_INLINE String File::ReadLine()
{
	if (currentLocation >= buffer.size())
		return "";

	int startLoc = currentLocation;
	int endLoc = startLoc;
	char c = buffer[endLoc];

	while (c != '\n' && c != '\0') 
	{
		c = buffer[endLoc];
		endLoc++;
	}

	size_t size = endLoc - startLoc - 1;

	currentLocation = endLoc;

	Vector<char> buf(size + 1);
	Allocator<char>::Copy(buffer.data() + startLoc, buf.data(), size, false);
	buf[size] = '\0';

	return buf.data();
}

AKL_INLINE bool File::ReadLine(String& str)
{
	if (currentLocation >= buffer.size())
	{
		str = NULL;
		return false;
	}

	int startLoc = currentLocation;
	int endLoc = startLoc;
	char c = buffer[endLoc];

	while (c != '\n' && c != '\0')
	{
		c = buffer[endLoc];
		endLoc++;
	}

	size_t size = endLoc - startLoc - 1;

	currentLocation = endLoc;

	Vector<char> buf(size + 1);
	Allocator<char>::Copy(buffer.data() + startLoc, buf.data(), size, true);
	buf[size] = '\0';

	str = buf.data();
	return true;
}

AKL_INLINE bool File::Read()
{
	if (file == NULL)
		return false;

	size_t len = Size();

	Rewind();

	buffer.resize(len + 1);

	fread(buffer.data(), sizeof(char), len, file);

	buffer[len] = '\0';

	return true;
}

AKL_INLINE bool File::Read(char* buf)
{
	if (file == NULL)
		return false;

	size_t len = Size();

	Rewind();

	buffer.resize(len + 1);

	fread(buffer.data(), sizeof(char), len, file);

	buffer[len] = '\0';

	Allocator<char>::Copy(buffer.data(), buf, len, true);

	return true;
}

AKL_INLINE bool File::Write(char* buf, size_t len)
{
	if (file == NULL)
		return false;

	fwrite(buf, sizeof(char), len, file);

	return true;
}

AKL_INLINE char* File::Data()
{
	return buffer.data();
}

AKL_INLINE String File::Str()
{
	if (buffer.size() > 0)
		return buffer.data();

	return "";
}

AKL_INLINE size_t File::Size()
{
	if (file == NULL)
		return 0;

	size_t len = 0;

	while (fgetc(file) != EOF)
	{
		len++;
	}

	return len;
}

AKL_INLINE size_t File::BufSize()
{
	return buffer.size();
}

AKL_INLINE int File::Location()
{
	return location;
}

AKL_INLINE bool File::IsOpen()
{
	return file != NULL;
}

AKL_INLINE const char* File::GetModeString(FileMode m)
{
	switch (m)
	{
		case AKLib::FMODE_READ: return "rb";
		case AKLib::FMODE_WRITE: return "wb";
		case AKLib::FMODE_APPEND: return "ab";
	}

	return "";
}

NSE()