#ifndef AK_LIB_STRING_H
#define AK_LIB_STRING_H

#include "AKLib/common.h"
#include "AKLib/Vector.h"

NSB(AKLib)

class AKLIB_API String
{
public:
	inline String();
	inline String(const char* s);
	inline String(const char* s, size_t sLen);
	inline String(const String& other);
	inline String& operator = (const char* s);
	inline String& operator = (const String& other);
	inline ~String();

	inline char* begin();
	inline char* end();
	inline const char* begin() const;
	inline const char* end() const;

	inline void Append(char c);
	inline void Append(const char* s);
	inline void Append(const String& other);
	inline char CharAt(int idx);
	inline char* c_str();
	inline const char* c_str() const;
	inline String substr(size_t begin);
	inline String substr(size_t begin, size_t len);
	inline Vector<String> Split(String tok);
	inline bool Split(String tok, String& left, String& right);
	inline int IndexOf(String tok, int start = 0);
	inline size_t Size() const;
	inline void Reset();
	inline void Erase(char* pos);
	inline char* Find(String tok);
	inline size_t GetIndex(char* pos);
	inline size_t GetIndex(const char* pos) const;
	inline bool StartsWith(String other);
	inline bool StartsWith(const String& other) const;
	inline bool EndsWith(String other);
	inline bool EndsWith(const String& other) const;
	inline bool Compare(const char* b, size_t len);
	inline bool Compare(String b);
	inline bool Compare(const String& b) const;
	inline static void Format(String& str, const char* fmt, ...);
	inline static String Format(const char* fmt, ...);
	inline static void FormatFloat(String& str, int precision, float value);
	inline static String FormatFloat(int precision, float value);
	inline static void FormatDouble(String& str, int precision, double value);
	inline static String FormatDouble(int precision, double value);
	inline int32_t AsInt();
	inline int64_t AsLong();
	inline float AsFloat();
	inline double AsDouble();

	inline static bool EndsWith(const char* a, size_t lenA, const char* b, size_t lenB);
	inline static bool EndsWith(const String& a, const String& b);
	inline static bool Compare(const char* a, size_t lenA, const char* b, size_t lenB);
	inline static bool Compare(const String& a, const String& b);

	inline String operator + (char c);
	inline String operator + (const char* s);
	inline String operator + (const String& other);
	inline String& operator += (char c);
	inline String& operator += (const char* s);
	inline String& operator += (const String& other);
	inline bool operator == (const String& other);
	inline bool operator != (const String& other);
	inline bool operator == (const char* other);
	inline bool operator != (const char* other);
	inline char& operator [](int idx);
	inline const char& operator [](int idx) const;
	inline operator bool();

	template <typename T>
	inline String& operator << (const T& other);
private:
	char* str;
	size_t length;
	size_t trueLength;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/String.inl"

#endif // AK_LIB_STRING_H