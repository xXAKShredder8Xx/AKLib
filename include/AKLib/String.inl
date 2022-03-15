#include "AKLib/Allocator.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <sstream>

NSB(AKLib)

AKL_INLINE String::String()
	: str{ Allocator<char>::Allocate(1, true) }, length{ 0 }, trueLength{ 0 }
{
	str[0] = '\0';
}

AKL_INLINE String::String(const char* s)
	: str{ NULL }, length{ 0 }, trueLength{ 0 }
{
	if (s == NULL)
	{
		length = 0;
		trueLength = 0;
		str = NULL;
		return;
	}

	size_t len = strlen(s);
	str = Allocator<char>::Duplicate(const_cast<char*>(s), len + 1, true);
	str[len] = '\0';
	length = len;
}

AKL_INLINE String::String(const char* s, size_t sLen)
	: str{ NULL }, length{ 0 }, trueLength{ 0 }
{
	size_t len = sLen;
	str = Allocator<char>::Duplicate(const_cast<char*>(s), len + 1, true);
	length = len;
	trueLength = length;
	str[len] = '\0';
}

AKL_INLINE String::String(const String& other)
	: str{ Allocator<char>::Duplicate(other.str, other.length + 1, true) }, length{ other.length }, trueLength{ other.length }
{
}

AKL_INLINE String& String::operator = (const char* s)
{
	if (s == NULL)
	{
		if (str != NULL) 
		{
			Allocator<char>::Deallocate(str, true);
		}

		length = 0;
		trueLength = 0;
		str = NULL;
		return *this;
	}

	size_t len = strlen(s);
	str = Allocator<char>::Duplicate(const_cast<char*>(s), len + 1, true);
	length = len;
	trueLength = length;
	str[len] = '\0';
	return *this;
}

AKL_INLINE String& String::operator = (const String& other)
{
	if (other.str == NULL) return *this;

	char* temp = Allocator<char>::Duplicate(other.str, other.length + 1, true);
	str = temp;
	length = other.length;
	trueLength = length;
	return *this;
}

AKL_INLINE String::~String()
{
	//printf("Freeing String: '%s'\n", str);
	if (str != NULL) str = Allocator<char>::Deallocate(str, true);
	length = 0;
}

AKL_INLINE char* String::begin()
{
	return str;
}

AKL_INLINE const char* String::begin() const
{
	return str;
}

AKL_INLINE char* String::end()
{
	return str + length;
}

AKL_INLINE const char* String::end() const
{
	return str + length;
}

AKL_INLINE void String::Append(char c)
{
	length++;
	str = Allocator<char>::Reallocate(str, length + 1, length, true);
	str[length - 1] = c;
	str[length] = '\0';
}

AKL_INLINE void String::Append(const char* s)
{
	size_t oldLen = length;
	size_t len = strlen(s);
	length += len;
	str = Allocator<char>::Reallocate(str, length + 1, length, true);
	Allocator<char>::Copy(const_cast<char*>(s), str + oldLen, len, true);
	str[length] = '\0';
}

AKL_INLINE void String::Append(const String& other)
{
	size_t oldLen = length;
	size_t len = other.length;
	length += len;
	str = Allocator<char>::Reallocate(str, length + 1, length, true);
	Allocator<char>::Copy(other.str, str + oldLen, len, true);
	str[length] = '\0';
}

AKL_INLINE char String::CharAt(int idx)
{
	return str[idx];
}

AKL_INLINE char* String::c_str()
{
	return str;
}

AKL_INLINE const char* String::c_str() const
{
	return str;
}

AKL_INLINE String String::substr(size_t begin)
{
	return String(str + begin);
}

AKL_INLINE String String::substr(size_t begin, size_t len)
{
	return String(str + begin, len);
}

AKL_INLINE Vector<String> String::Split(String tok)
{
	String temp = str;
	Vector<String> tokens;

	if (tok == "")
		return tokens;

	for (int i = 0; i <= temp.length; i++)
	{
		if (temp[i] == tok[0])
		{
			if (i + tok.length <= temp.length)
			{
				if (Allocator<char>::Compare(temp.c_str() + i, tok.c_str(), tok.length))
				{
					tokens.push_back(temp.substr(0, i));

					while (Allocator<char>::Compare(temp.c_str() + i, tok.c_str(), tok.length))
					{
						i++;
					}

					temp = temp.substr(i);

					i = 0;
				}
			}
			else
			{
				break;
			}
		}
	}

	if (temp != "")
		tokens.push_back(temp);

	return tokens;
}

AKL_INLINE bool String::Split(String tok, String& left, String& right)
{
	for (int i = 0; i < length; i++)
	{
		if (str[i] == tok[0])
		{
			if (i + tok.length < length)
			{
				if (Allocator<char>::Compare(str + i, tok.c_str(), tok.length))
				{
					left = substr(0, i);
					right = substr(i + tok.length);
					return true;
				}
			}
			else
			{
				break;
			}
		}
	}

	return false;
}

AKL_INLINE int String::IndexOf(String tok, int start)
{
	for (int i = start; i <= length; i++)
	{
		if (str[i] == tok[0])
		{
			if (i + tok.length <= length)
			{
				if (Allocator<char>::Compare(str + i, tok.c_str(), tok.length))
				{
					return i;
				}
			}
			else
			{
				break;
			}
		}
	}

	return -1;
}

AKL_INLINE size_t String::Size() const
{
	return length;
}

AKL_INLINE void String::Reset()
{
	str = Allocator<char>::Deallocate(str, true);
	length = 0;

	str = Allocator<char>::Allocate(1, true);
	str[0] = '\0';
}

AKL_INLINE void String::Erase(char* pos)
{
	if (pos != end())
	{
		if ((length - 1) < (trueLength >> 1))
		{
			trueLength >>= 1;
			str = Allocator<char>::Reallocate(str, trueLength >> 1, trueLength, true);
		}

		Allocator<char>::Copy(pos + 1, pos, length - GetIndex(pos), true);
		length--;
	}
}

AKL_INLINE char* String::Find(String tok)
{
	int i = IndexOf(tok, 0);
	
	if (i == -1)
		return end();

	return str + i;
}

AKL_INLINE size_t String::GetIndex(char* pos)
{
	return pos - str;
}

AKL_INLINE size_t String::GetIndex(const char* pos) const
{
	return pos - str;
}

AKL_INLINE bool String::StartsWith(String other)
{
	if (other.length > length)
		return false;

	for (int i = 0; i < other.length; i++) 
	{
		if (str[i] != other.str[i])
			return false;
	}

	return true;
}

AKL_INLINE bool String::StartsWith(const String& other) const
{
	if (other.length > length)
		return false;

	for (int i = 0; i < other.length; i++)
	{
		if (str[i] != other.str[i])
			return false;
	}

	return true;
}

AKL_INLINE bool String::EndsWith(String other)
{
	if (other.length > length) return false;

	char* pos = end() - other.length;

	for (int i = GetIndex(pos), j = 0; i < length; i++, j++)
	{
		if (str[i] != other.str[j])
			return false;
	}

	return true;
}

AKL_INLINE bool String::EndsWith(const String& other) const
{
	if (other.length > length) return false;

	const char* pos = end() - other.length;

	for (int i = GetIndex(pos), j = 0; i < length; i++, j++)
	{
		if (str[i] != other.str[j])
			return false;
	}

	return true;
}

AKL_INLINE bool String::Compare(const char* b, size_t len)
{
	if (length != len) return false;

	for (int i = 0; i < length; i++)
	{
		if (str[i] != b[i])
			return false;
	}

	return true;
}

AKL_INLINE bool String::Compare(String b)
{
	if (length != b.length) return false;

	for (int i = 0; i < length; i++)
	{
		if (str[i] != b.str[i])
			return false;
	}

	return true;
}

AKL_INLINE bool String::Compare(const String& b) const
{
	if (length != b.length) return false;

	for (int i = 0; i < length; i++)
	{
		if (str[i] != b.str[i])
			return false;
	}

	return true;
}

AKL_INLINE void String::Format(String& str, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int size = vsnprintf(NULL, 0, fmt, args);
	if (size < 0)
		fprintf(stderr, "Failed to format string!\nSize is less than zero or size exceeds maximum format size\n");
	char* buffer = Allocator<char>::Allocate((size_t)size + 1, true);
	vsnprintf(buffer, (size_t)size + 1, fmt, args);
	buffer[size] = '\0';
	va_end(args);
	str = buffer;
	buffer = Allocator<char>::Deallocate(buffer, true);
}

AKL_INLINE String String::Format(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int size = vsnprintf(NULL, 0, fmt, args);
	if (size < 0)
		fprintf(stderr, "Failed to format string!\nSize is less than zero or size exceeds maximum format size\n");
	char* buffer = Allocator<char>::Allocate((size_t)size + 1, true);
	vsnprintf(buffer, (size_t)size + 1, fmt, args);
	buffer[size] = '\0';
	va_end(args);
	String str = buffer;
	buffer = Allocator<char>::Deallocate(buffer, true);
	return str;
}

AKL_INLINE void String::FormatFloat(String& str, int precision, float value)
{
	if (precision > 0)
		precision++;
	int size = snprintf(NULL, 0, "%.0g", value) + precision;
	if (size < 0)
		fprintf(stderr, "Failed to format string!\nSize is less than zero or size exceeds maximum format size\n");
	char* buffer = Allocator<char>::Allocate((size_t)size + 1, true);

	if (precision > 0)
	{
		_gcvt_s(buffer, (size_t)size + 1, value, precision);
	}
	else
	{
		snprintf(buffer, (size_t)size + 1, "%d", (int)roundf(value));
	}

	buffer[size] = '\0';
	str = buffer;
	buffer = Allocator<char>::Deallocate(buffer, true);
}

AKL_INLINE String String::FormatFloat(int precision, float value)
{
	if (precision > 0)
		precision++;
	int size = snprintf(NULL, 0, "%.0g", value) + precision;
	if (size < 0)
		fprintf(stderr, "Failed to format string!\nSize is less than zero or size exceeds maximum format size\n");
	char* buffer = Allocator<char>::Allocate((size_t)size + 1, true);

	if (precision > 0)
	{
		_gcvt_s(buffer, (size_t)size + 1, value, precision);
	}
	else
	{
		snprintf(buffer, (size_t)size + 1, "%d", (int)round(value));
	}

	buffer[size] = '\0';
	String str = buffer;
	buffer = Allocator<char>::Deallocate(buffer, true);
	return str;
}

AKL_INLINE void String::FormatDouble(String& str, int precision, double value)
{
	if (precision > 0)
		precision++;
	int size = snprintf(NULL, 0, "%.0g", value) + precision;
	if (size < 0)
		fprintf(stderr, "Failed to format string!\nSize is less than zero or size exceeds maximum format size\n");
	char* buffer = Allocator<char>::Allocate((size_t)size + 1, true);

	if (precision > 0)
	{
		_gcvt_s(buffer, (size_t)size + 1, value, precision);
	}
	else
	{
		snprintf(buffer, (size_t)size + 1, "%d", (int)round(value));
	}

	buffer[size] = '\0';
	str = buffer;
	buffer = Allocator<char>::Deallocate(buffer, true);
}

AKL_INLINE String String::FormatDouble(int precision, double value)
{
	if (precision > 0)
		precision++;
	int size = snprintf(NULL, 0, "%.0g", value) + precision;
	if (size < 0)
		fprintf(stderr, "Failed to format string!\nSize is less than zero or size exceeds maximum format size\n");
	char* buffer = Allocator<char>::Allocate((size_t)size + 1, true);

	if (precision > 0)
	{
		_gcvt_s(buffer, (size_t)size + 1, value, precision);
	}
	else
	{
		snprintf(buffer, (size_t)size + 1, "%d", (int)round(value));
	}

	buffer[size] = '\0';
	String str = buffer;
	buffer = Allocator<char>::Deallocate(buffer, true);
	return str;
}

AKL_INLINE int32_t String::AsInt()
{
	return atoi(str);
}

AKL_INLINE int64_t String::AsLong()
{
	return atoll(str);
}

AKL_INLINE float String::AsFloat()
{
	return (float)atof(str);
}

AKL_INLINE double String::AsDouble()
{
	return atof(str);
}

AKL_INLINE bool String::EndsWith(const char* a, size_t lenA, const char* b, size_t lenB)
{
	if (lenB > lenA) return false;

	const char* pos = (a + lenA) - lenB;

	for (int i = pos - a, j = 0; i < lenA; i++, j++)
	{
		if (a[i] != b[j])
			return false;
	}

	return true;
}

AKL_INLINE bool String::EndsWith(const String& a, const String& b)
{
	return a.EndsWith(b);
}

AKL_INLINE bool String::Compare(const char* a, size_t lenA, const char* b, size_t lenB)
{
	if (lenA != lenB) return false;

	for (int i = 0; i < lenA; i++)
	{
		if (a[i] != b[i])
			return false;
	}

	return true;
}

AKL_INLINE bool String::Compare(const String& a, const String& b)
{
	return a.Compare(b);
}

AKL_INLINE String String::operator + (char c)
{
	String string(str, length);

	string.Append(c);

	return string;
}

AKL_INLINE String String::operator + (const char* s)
{
	String string(str, length);

	string.Append(s);

	return string;
}

AKL_INLINE String String::operator + (const String& other)
{
	String string(str, length);

	string.Append(other);

	return string;
}

AKL_INLINE String& String::operator += (char c)
{
	Append(c);
	return *this;
}

AKL_INLINE String& String::operator += (const char* s)
{
	Append(s);
	return *this;
}

AKL_INLINE String& String::operator += (const String& other)
{
	Append(other.str);
	return *this;
}

AKL_INLINE bool String::operator == (const String& other)
{
	return Compare(other.str, other.length);
}

AKL_INLINE bool String::operator != (const String& other)
{
	return !Compare(other.str, other.length);
}

AKL_INLINE bool String::operator == (const char* other)
{
	if (other == NULL)
	{
		return str == NULL;
	}

	return Compare(other, strlen(other));
}

AKL_INLINE bool String::operator != (const char* other)
{
	if (other == NULL)
	{
		return str != NULL;
	}

	return !Compare(other, strlen(other));
}

AKL_INLINE char& String::operator [](int idx)
{
	return str[idx];
}

AKL_INLINE const char& String::operator [](int idx) const
{
	return str[idx];
}

AKL_INLINE String::operator bool()
{
	if (str != NULL)
		return length > 0;

	return false;
}

template <>
AKL_INLINE String& String::operator << (const int8_t& other) 
{
	Append(Format("%d", other));
	return *this;
}

template <>
AKL_INLINE String& String::operator << (const uint8_t& other)
{
	Append(Format("%u", other));
	return *this;
}

template <>
AKL_INLINE String& String::operator << (const int16_t& other)
{
	Append(Format("%d", other));
	return *this;
}

template <>
AKL_INLINE String& String::operator << (const uint16_t& other)
{
	Append(Format("%u", other));
	return *this;
}

template <>
AKL_INLINE String& String::operator << (const int32_t& other)
{
	Append(Format("%d", other));
	return *this;
}

template <>
AKL_INLINE String& String::operator << (const uint32_t& other)
{
	Append(Format("%u", other));
	return *this;
}

template <>
AKL_INLINE String& String::operator << (const int64_t& other)
{
	Append(Format("%I64d", other));
	return *this;
}

template <>
AKL_INLINE String& String::operator << (const uint64_t& other)
{
	Append(Format("%I64u", other));
	return *this;
}

template <>
AKL_INLINE String& String::operator << (const String& other)
{
	Append(other);
	return *this;
}

template <>
AKL_INLINE String& String::operator << (const char* const& other)
{
	Append(other);
	return *this;
}

template <typename T>
AKL_INLINE String& String::operator << (const T& other) 
{
	std::stringstream ss;
	ss << other;
	Append(ss.str().c_str());

	return *this;
}

NSE()