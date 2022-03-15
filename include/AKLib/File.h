#ifndef AK_LIB_FILE_H
#define AK_LIB_FILE_H

#include "AKLib/common.h"
#include "AKLib/String.h"
#include "AKLib/Vector.h"
#include <stdio.h>

NSB(AKLib)

enum FileMode : uint8_t
{
	FMODE_READ,
	FMODE_WRITE,
	FMODE_APPEND
};

class AKLIB_API File
{
public:
	inline File(const char* name, FileMode m);
	inline ~File();

	inline bool SetFile(const char* name, FileMode m);
	inline bool SetMode(FileMode m);
	inline bool Open();
	inline bool Close();
	inline bool GoTo(int loc);
	inline bool Rewind();
	inline char ReadChar();
	inline String ReadLine();
	inline bool ReadLine(String& str);
	inline bool Read();
	inline bool Read(char* buf);
	inline bool Write(char* buf, size_t len);
	inline char* Data();
	inline String Str();
	inline size_t Size();
	inline size_t BufSize();
	inline int Location();
	inline bool IsOpen();

private:
	static const char* GetModeString(FileMode m);

	FILE* file;
	Vector<char> buffer;
	int location;
	int currentLocation;
	const char* fname;
	FileMode mode;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/File.inl"

#endif // AK_LIB_FILE_H